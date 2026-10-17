"""Export original ability FSMs, VFX parameters and bound animation events.

Read-only local sources. Default outputs go to logs, never shared extraction
directories. Names come from unit->skill->ability.unitPrefab/prefabName fields.
This is a lossless presentation evidence graph, not a replacement FSM runtime.
"""
import argparse
import hashlib
import json
import struct
from pathlib import Path

import UnityPy
from export_battle_presentation import ROOT, source_bundle

MASK = (1 << 64) - 1
PARAM_ARRAYS = {5: 'unityObjectParams', 14: 'animationCurveParams',
               15: 'fsmFloatParams', 16: 'fsmIntParams', 17: 'fsmBoolParams',
               18: 'fsmStringParams', 19: 'fsmGameObjectParams', 20: 'fsmOwnerDefaultParams',
               21: 'functionCallParams', 22: 'animationCurveParams', 23: 'stringParams',
               24: 'fsmObjectParams', 25: 'fsmColorParams', 28: 'fsmVector3Params',
               29: 'layoutOptionParams', 30: 'fsmRectParams', 31: 'fsmEventTargetParams',
               35: 'fsmQuaternionParams', 36: 'fsmPropertyParams', 37: 'fsmVector2Params',
               38: 'fsmTemplateControlParams', 39: 'fsmVarParams', 41: 'fsmArrayParams',
               42: 'fsmEnumParams'}


def read(path):
    return json.loads(path.read_text(encoding='utf-8'))


def dump(path, data):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(data, ensure_ascii=False, indent=2,
                              default=lambda x: {'bytesHex': bytes(x).hex()}) + '\n', encoding='utf-8')


class Bundle:
    def __init__(self, name):
        self.source = source_bundle(name.lower())
        self.name = name.lower()
        self.env = UnityPy.load(str(self.source))
        self.objects = {o.path_id & MASK: o for o in self.env.objects}
        self.scripts = {o.path_id & MASK: o.read().m_ClassName for o in self.objects.values()
                        if o.type.name == 'MonoScript'}
        self.transforms = {o.path_id & MASK: o.read_typetree() for o in self.objects.values()
                           if o.type.name == 'Transform'}
        self.gameobjects = {o.path_id & MASK: o.read_typetree() for o in self.objects.values()
                            if o.type.name == 'GameObject'}
        self.by_go = {v['m_GameObject']['m_PathID'] & MASK: k for k, v in self.transforms.items()}

    def provenance(self):
        return {'bundle': self.name, 'path': str(self.source.resolve()),
                'sha256': hashlib.sha256(self.source.read_bytes()).hexdigest()}

    def path(self, go):
        tid = self.by_go.get(go & MASK)
        if tid is None:
            return None
        t = self.transforms[tid]
        parent = self.transforms.get(t['m_Father']['m_PathID'] & MASK)
        prefix = self.path(parent['m_GameObject']['m_PathID']) + '/' if parent else ''
        return prefix + self.gameobjects[go & MASK]['m_Name']

    def record(self, obj, tree):
        return {'pathId': str(obj.path_id), 'type': obj.type.name,
                'hierarchyPath': self.path(obj.path_id if obj.type.name == 'GameObject' else tree.get('m_GameObject', {}).get('m_PathID', 0)), 'raw': tree}


def decoded_actions(fsm):
    variables = {v['name']: v for entries in fsm['variables'].values()
                 if isinstance(entries, list) for v in entries if isinstance(v, dict) and 'name' in v}
    states = []
    for state in fsm['states']:
        data = state['actionData']
        actions = []
        for i, name in enumerate(data['actionNames']):
            start = data['actionStartIndex'][i]
            stop = data['actionStartIndex'][i + 1] if i + 1 < len(data['actionNames']) else len(data['paramName'])
            params = []
            for index in range(start, stop):
                kind, pos = data['paramDataType'][index], data['paramDataPos'][index]
                size = data['paramByteDataSize'][index]
                value = {'rawByteData': data['byteData'][pos:pos + size]}
                field = PARAM_ARRAYS.get(kind)
                if fsm['dataVersion'] >= 2 and field and pos < len(data.get(field, [])):
                    value = {'array': field, 'index': pos, 'value': data[field][pos]}
                elif kind == 1 and size == 1:
                    value = {'value': bool(data['byteData'][pos])}
                elif kind in (0, 2, 6, 7) and size == 4:
                    value = {'value': struct.unpack('<f' if kind == 2 else '<i', bytes(data['byteData'][pos:pos + 4]))[0]}
                parameter = {'name': data['paramName'][index], 'type': kind, **value}
                wrapped = value.get('value')
                if isinstance(wrapped, dict) and wrapped.get('useVariable') and wrapped.get('name') in variables:
                    parameter['resolvedInitialValue'] = variables[wrapped['name']].get('value')
                    parameter['variableName'] = wrapped['name']
                params.append(parameter)
            actions.append({'type': name, 'enabled': data['actionEnabled'][i], 'params': params})
        states.append({'name': state['name'], 'transitions': state['transitions'], 'actions': actions})
    return states


def ability_asset(name):
    bundle = Bundle(name)
    record = {'source': bundle.provenance(), 'fsm': [], 'components': [], 'particles': [],
              'particleRenderers': [], 'materials': [], 'transforms': [], 'gameObjects': [],
              'meshComponents': [], 'textureMetadata': [], 'dependencies': []}
    for obj in bundle.objects.values():
        kind = obj.type.name
        if kind not in ('MonoBehaviour', 'ParticleSystem', 'ParticleSystemRenderer', 'Material',
                        'Transform', 'GameObject', 'MeshFilter', 'MeshRenderer', 'Texture2D', 'AssetBundle'):
            continue
        tree = obj.read_typetree()
        row = bundle.record(obj, tree)
        if kind == 'MonoBehaviour':
            row['class'] = bundle.scripts.get(tree['m_Script']['m_PathID'] & MASK, 'EXTERNAL_SCRIPT')
            if row['class'] == 'PlayMakerFSM':
                row['decodedStates'] = decoded_actions(tree['fsm'])
                record['fsm'].append(row)
            record['components'].append(row)
        elif kind == 'ParticleSystem':
            record['particles'].append(row)
        elif kind == 'ParticleSystemRenderer':
            record['particleRenderers'].append(row)
        elif kind == 'Material':
            record['materials'].append(row)
        elif kind == 'Transform':
            record['transforms'].append(row)
        elif kind == 'GameObject':
            record['gameObjects'].append(row)
        elif kind in ('MeshFilter', 'MeshRenderer'):
            record['meshComponents'].append(row)
        elif kind == 'Texture2D':
            record['textureMetadata'].append({'pathId': str(obj.path_id),
                'name': tree['m_Name'], 'width': tree['m_Width'], 'height': tree['m_Height'],
                'textureFormat': tree['m_TextureFormat'], 'streamData': tree.get('m_StreamData')})
        elif kind == 'AssetBundle':
            record['assetBundle'] = tree
    for file in bundle.env.files.values():
        for asset in getattr(file, 'files', {}).values():
            for ext in getattr(asset, 'externals', []):
                record['dependencies'].append(str(ext.path))
    return record


def character_animation(name):
    character = Bundle(name)
    shared = Bundle('shared_animationcontainer')
    record = {'source': character.provenance(), 'sharedSource': shared.provenance(),
              'animators': [], 'controllers': [], 'clips': [], 'boundStates': [], 'unitReferenceAreas': [], 'subObjectAnimationEvents': []}
    used = {}
    for obj in character.objects.values():
        if obj.type.name == 'MonoBehaviour':
            tree = obj.read_typetree()
            if character.scripts.get(tree['m_Script']['m_PathID'] & MASK) == 'UnitSubObjectAnimationEventHandler':
                entry = character.record(obj, tree)
                entry['objectBindings'] = []
                for event in tree['_animationEvents']:
                    for action in event['Actions']:
                        for field in ('UnitSubObject', 'Parent'):
                            pid = action[field]['m_PathID']
                            if pid:
                                entry['objectBindings'].append({'pathId': str(pid), 'hierarchyPath': character.path(pid),
                                    'rawGameObject': character.gameobjects.get(pid & MASK)})
                record['subObjectAnimationEvents'].append(entry)
            if character.scripts.get(tree['m_Script']['m_PathID'] & MASK) == 'UnitComponent':
                for key, refs in tree['_unitReferenceAreas'].items():
                    for index, pointer in enumerate(refs if isinstance(refs, list) else [refs]):
                        pid = pointer['m_PathID'] & MASK
                        target = character.objects.get(pid)
                        raw = target.read_typetree() if target else {}
                        go = pointer['m_PathID'] if target and target.type.name == 'GameObject' else raw.get('m_GameObject', {}).get('m_PathID', 0)
                        record['unitReferenceAreas'].append({'field': key, 'index': index, 'pointer': pointer,
                            'type': target.type.name if target else None, 'hierarchyPath': character.path(go),
                            'sourceComponentPathId': str(obj.path_id)})
        if obj.type.name == 'Animator':
            record['animators'].append(character.record(obj, obj.read_typetree()))
        elif obj.type.name == 'AnimatorController':
            tree = obj.read_typetree()
            record['controllers'].append(character.record(obj, tree))
            strings = dict(tree['m_TOS'])
            for pointer in tree['m_AnimationClips']:
                pid = pointer['m_PathID'] & MASK
                source = character if pointer['m_FileID'] == 0 else shared
                clip = source.objects.get(pid)
                if clip is None or clip.type.name != 'AnimationClip':
                    raise ValueError(f'Unresolved exact animation clip {pointer}')
                used[(source.name, pid)] = (source, clip)
            for machine_index, machine in enumerate(tree['m_Controller']['m_StateMachineArray']):
                machine = machine['data']
                states = machine['m_StateConstantArray']
                for state_index, raw in enumerate(states):
                    state = raw['data']
                    ids = []
                    for blend in state['m_BlendTreeConstantArray']:
                        for node in blend['data']['m_NodeArray']:
                            if not node['data']['m_ChildIndices']:
                                ids.append(node['data']['m_ClipID'])
                    clips = []
                    for clip_index in ids:
                        if clip_index >= len(tree['m_AnimationClips']):
                            raise ValueError('Invalid original controller clip index')
                        pointer = tree['m_AnimationClips'][clip_index]
                        source = character if pointer['m_FileID'] == 0 else shared
                        clip = source.objects[pointer['m_PathID'] & MASK]
                        clips.append({'controllerClipIndex': clip_index, 'name': clip.read_typetree()['m_Name'],
                                      'pathId': str(clip.path_id), 'sourceBundle': source.name})
                    record['boundStates'].append({'controllerPathId': str(obj.path_id),
                        'machineIndex': machine_index, 'stateIndex': state_index,
                        'name': strings.get(state['m_NameID']), 'fullPath': strings.get(state['m_FullPathID']),
                        'speed': state['m_Speed'], 'clips': clips,
                        'transitions': state['m_TransitionConstantArray'],
                        'anyStateTransitions': machine['m_AnyStateTransitionConstantArray'],
                        'defaultState': machine['m_DefaultState']})
    for source, clip in used.values():
        tree = clip.read_typetree()
        muscle = tree.get('m_MuscleClip', {})
        record['clips'].append({'name': tree['m_Name'], 'sourceBundle': source.name,
                                'pathId': str(clip.path_id), 'events': tree.get('m_Events', []),
                                'sampleRate': tree.get('m_SampleRate'),
                                'startTime': muscle.get('m_StartTime'), 'stopTime': muscle.get('m_StopTime'),
                                'muscleClipSettings': {k: v for k, v in muscle.items() if not isinstance(v, (dict, list))},
                                'animationClipSettings': tree.get('m_AnimationClipSettings')})
    return record


def area_of_effect_layout(name):
    import numpy as np
    from scipy.spatial.transform import Rotation
    bundle = Bundle(name)
    rows = []
    for obj in bundle.objects.values():
        if obj.type.name != 'MonoBehaviour':
            continue
        tree = obj.read_typetree()
        if bundle.scripts.get(tree['m_Script']['m_PathID'] & MASK) != 'BattleLayout':
            continue
        for side in ('_playerAreaOfEffect', '_enemyAreaOfEffect'):
            for field, pointer in tree[side].items():
                tid = pointer['m_PathID'] & MASK
                if tid in bundle.gameobjects:
                    tid = bundle.by_go[tid]
                chain = []
                while tid:
                    transform = bundle.transforms[tid]
                    chain.insert(0, {'pathId': str(bundle.objects[tid].path_id), 'raw': transform,
                                     'hierarchyPath': bundle.path(transform['m_GameObject']['m_PathID'])})
                    tid = transform['m_Father']['m_PathID'] & MASK
                matrix = np.eye(4)
                for node in chain:
                    raw = node['raw']
                    local = np.eye(4)
                    local[:3, :3] = Rotation.from_quat([raw['m_LocalRotation'][k] for k in 'xyzw']).as_matrix() @ np.diag([raw['m_LocalScale'][k] for k in 'xyz'])
                    local[:3, 3] = [raw['m_LocalPosition'][k] for k in 'xyz']
                    matrix = matrix @ local
                rows.append({'sideField': side, 'field': field, 'pointer': pointer,
                    'sourceComponentPathId': str(obj.path_id), 'transformChain': chain,
                    'layoutWorldPosition': dict(zip('xyz', matrix[:3, 3].tolist())),
                    'layoutWorldMatrix': matrix.tolist()})
    return {'source': bundle.provenance(), 'markers': rows}


def export_particle_resources(abilities, target):
    """Resolve original renderer dependencies, never substitute shaders/meshes.

    PNGs contain decoded original texture pixels; meshes retain their complete
    native serialized channels/index data for a version-aware runtime importer.
    Unity builtin resources remain explicit references, not invented geometry.
    """
    sources = {name: Bundle(name) for name in ('shared_resourcecontainer', 'shared_shadercontainer')}
    sources.update({a['source']['bundle']: Bundle(a['source']['bundle']) for a in abilities})
    cab_sources = {}
    for source in sources.values():
        for container in source.env.files.values():
            for filename in getattr(container, 'files', {}):
                cab_sources[filename.lower()] = source
    records, visited = [], set()

    def pointers(value):
        if isinstance(value, dict):
            if 'm_FileID' in value and 'm_PathID' in value:
                yield value
            else:
                for child in value.values():
                    yield from pointers(child)
        elif isinstance(value, (list, tuple)):
            for child in value:
                yield from pointers(child)

    def follow(source, owner, pointer):
        pid = pointer['m_PathID']
        if not pid:
            return
        destination = source
        external = None
        if pointer['m_FileID']:
            external = str(owner.assets_file.externals[pointer['m_FileID'] - 1].path)
            destination = cab_sources.get(external.rsplit('/', 1)[-1].lower())
        edge = {'sourceBundle': source.name, 'ownerPathId': str(owner.path_id),
                'pointer': pointer, 'external': external}
        if destination is None:
            records.append({**edge, 'status': 'PENDING-ORIGINAL', 'reason': 'builtin or unresolved external resource'})
            return
        obj = destination.objects.get(pid & MASK)
        if obj is None:
            raise ValueError(f'Unresolved original resource {edge}')
        if obj.type.name not in ('Material', 'Texture2D', 'Mesh', 'Shader'):
            return
        key = destination.name, obj.path_id
        resource = f'assets/{destination.name}_{obj.path_id}'
        records.append({**edge, 'resolvedBundle': destination.name, 'resolvedPathId': str(obj.path_id),
                        'type': obj.type.name, 'resource': resource})
        if key in visited:
            return
        visited.add(key)
        tree = obj.read_typetree()
        evidence = {'source': destination.provenance(), 'pathId': str(obj.path_id),
                    'type': obj.type.name, 'raw': tree}
        if obj.type.name == 'Texture2D':
            path = target / (resource + '.png')
            path.parent.mkdir(parents=True, exist_ok=True)
            pixels = obj.read().image
            pixels.save(path)
            evidence['decodedTexture'] = {'file': resource + '.png', 'size': list(pixels.size),
                                         'sha256': hashlib.sha256(path.read_bytes()).hexdigest(),
                                         'rgbaSha256': hashlib.sha256(pixels.convert('RGBA').tobytes()).hexdigest()}
            # Preserve full source texture bytes/stream metadata in the raw JSON.
        dump(target / (resource + '.json'), evidence)
        if obj.type.name == 'Material':
            for child in pointers(tree):
                follow(destination, obj, child)

    for ability in abilities:
        source = sources[ability['source']['bundle']]
        for row in ability['particleRenderers'] + ability['meshComponents']:
            owner = source.objects[int(row['pathId']) & MASK]
            for pointer in pointers(row['raw']):
                follow(source, owner, pointer)
    manifest = {'schemaVersion': 1, 'sources': [s.provenance() for s in sources.values()],
                'references': records, 'limits': ['Original shader serialization is preserved; no substitute shader is claimed equivalent.',
                    'Mesh serialized channels require a version-aware importer.',
                    'Texture PNGs require original texture sampler/color-space settings from paired JSON.']}
    dump(target / 'particle-resources.json', manifest)
    return manifest


def export_particle_prefab(ability, target):
    """Candidate native-module YAML. Compatibility must be checked in Unity.

    Only original hierarchy and particle components are included. Unresolved
    external materials are explicitly null and recorded, never approximated.
    """
    bundle = Bundle(ability['source']['bundle'])
    subtree = ability.get('subtreeHierarchyPath')
    def in_subtree(obj):
        raw = obj.read_typetree()
        go = obj.path_id if obj.type.name == 'GameObject' else raw['m_GameObject']['m_PathID']
        path = bundle.path(go)
        return not subtree or path == subtree or path.startswith(subtree + '/')
    included = {o.path_id for o in bundle.objects.values()
                if o.type.name in ('GameObject', 'Transform', 'ParticleSystem', 'ParticleSystemRenderer') and in_subtree(o)}
    pending = []

    def convert(value, node=None):
        if isinstance(value, dict):
            if 'm_PathID' in value and 'm_FileID' in value:
                if value['m_FileID'] == 0 and (not value['m_PathID'] or value['m_PathID'] in included):
                    return {'fileID': value['m_PathID']}
                pending.append(value)
                return {'fileID': 0}
            children = {n.m_Name: n for n in node.m_Children} if node else {}
            result = {'serializedVersion': node.m_Version} if node and node.m_Version > 1 else {}
            result.update({k: convert(v, children.get(k)) for k, v in value.items()})
            return result
        if isinstance(value, (list, tuple)):
            array = next((n for n in node.m_Children if n.m_Name == 'Array'), None) if node else None
            data = next((n for n in array.m_Children if n.m_Name == 'data'), None) if array else None
            return [convert(v, data) for v in value]
        return int(value) if isinstance(value, bool) else value

    def yaml(value, indent=0):
        prefix = ' ' * indent
        if isinstance(value, dict):
            lines = []
            for key, child in value.items():
                if isinstance(child, (dict, list)) and child:
                    lines.append(prefix + key + ':')
                    lines.extend(yaml(child, indent + 2))
                else:
                    lines.append(prefix + key + ': ' + json.dumps(child, ensure_ascii=False))
            return lines
        lines = []
        for child in value:
            if isinstance(child, (dict, list)) and child:
                body = yaml(child, indent + 2)
                lines.append(prefix + '- ' + body[0].lstrip())
                lines.extend(body[1:])
            else:
                lines.append(prefix + '- ' + json.dumps(child, ensure_ascii=False))
        return lines

    text = ['%YAML 1.1', '%TAG !u! tag:unity3d.com,2011:']
    mapping = []
    root_transform = next(t for t in bundle.transforms.values() if
                          bundle.path(t['m_GameObject']['m_PathID']) == subtree) if subtree else next(t for t in bundle.transforms.values() if t['m_Father']['m_PathID'] == 0)
    root_go = root_transform['m_GameObject']['m_PathID']
    mapping_id = 910000000000000001
    if mapping_id in included:
        raise ValueError('Object mapping fileID collision')
    for obj in bundle.objects.values():
        if obj.path_id not in included:
            continue
        raw = obj.read_typetree()
        if obj.type.name == 'GameObject':
            raw['m_Component'] = [r for r in raw['m_Component'] if r['component']['m_PathID'] in included]
            transform = bundle.by_go.get(obj.path_id & MASK)
            if transform is not None:
                mapping.append({'id': str(obj.path_id), 'transformFileID': bundle.objects[transform].path_id})
        elif obj.type.name == 'Transform':
            mapping.append({'id': str(obj.path_id), 'transformFileID': obj.path_id})
        tree = convert(raw, obj.serialized_type.node)
        if obj.path_id == root_go:
            tree['m_Component'].append({'component': {'fileID': mapping_id}})
        tree = {'m_ObjectHideFlags': 0, 'm_CorrespondingSourceObject': {'fileID': 0},
                'm_PrefabInstance': {'fileID': 0}, 'm_PrefabAsset': {'fileID': 0}, **tree}
        text.extend([f'--- !u!{obj.type.value} &{obj.path_id}', obj.type.name + ':'])
        text.extend(yaml(tree, 2))
    text.extend([f'--- !u!114 &{mapping_id}', 'MonoBehaviour:'])
    text.extend(yaml({'m_ObjectHideFlags': 0, 'm_CorrespondingSourceObject': {'fileID': 0},
        'm_PrefabInstance': {'fileID': 0}, 'm_PrefabAsset': {'fileID': 0},
        'm_GameObject': {'fileID': root_go}, 'm_Enabled': 1, 'm_EditorHideFlags': 0,
        'm_Script': {'fileID': 11500000, 'guid': '2b9f552c8c2c4df0a92f2cfcdcb71963', 'type': 3},
        'm_Name': '', 'm_EditorClassIdentifier': '', 'ids': [r['id'] for r in mapping],
        'objects': [{'fileID': r['transformFileID']} for r in mapping]}, 2))
    target.mkdir(parents=True, exist_ok=True)
    path = target / (ability['id'] + '.prefab')
    path.write_text('\n'.join(text) + '\n', encoding='utf-8')
    dump(path.with_suffix('.provenance.json'), {'source': bundle.provenance(),
         'sourceUnityVersion': next(iter(bundle.objects.values())).assets_file.unity_version,
         'sha256': hashlib.sha256(path.read_bytes()).hexdigest(), 'objectIds': mapping,
         'unresolvedPointers': pending, 'status': 'PENDING-ORIGINAL Unity 6000 to 2022 readback validation and original materials/shaders'})


def character_overlays(character, out):
    name = character['source']['bundle']
    full = ability_asset(name)
    wanted = {e['data'] for clip in character['clips'] for e in clip['events'] if e['functionName'] == 'StartParticleSystem'}
    inventory = [{'pathId': r['pathId'], 'hierarchyPath': r['hierarchyPath'], 'name': r['hierarchyPath'].rsplit('/', 1)[-1]} for r in full['particles']]
    child_inventories = []
    for component in full['components']:
        if component['class'] == 'BattleUnitChildPrefabLoader':
            for prefab in component['raw']['_childPrefabs']:
                child = ability_asset(prefab.split('.', 1)[1])
                child_inventories.append({'prefabName': prefab, 'source': child['source'],
                    'particles': [{'pathId': r['pathId'], 'hierarchyPath': r['hierarchyPath'], 'name': r['hierarchyPath'].rsplit('/', 1)[-1]} for r in child['particles']]})
    character['particleNameInventory'] = inventory
    character['childPrefabParticleInventories'] = child_inventories
    character['particleEventResolution'] = [{'eventName': n,
        'status': 'MATCH_ORIGINAL_CHARACTER_PARTICLE' if any(p['name'] == n for p in inventory) else
                  'MATCH_ORIGINAL_CHILD_PARTICLE' if any(p['name'] == n for c in child_inventories for p in c['particles']) else 'ORIGINAL_NO_MATCH_NO_OP',
        'nativeRule': 'UnitAnimationEventHandler caches GetComponentsInChildren<ParticleSystem>(true); first exact name match, no match does nothing.'} for n in sorted(wanted)]
    roots = [p for p in inventory if not any(p['hierarchyPath'].startswith(other['hierarchyPath'] + '/') for other in inventory)]
    rows, unresolved = [], []
    for event_name in sorted(set(p['name'] for p in roots)):
        hits = [g for g in full['gameObjects'] if g['raw']['m_Name'] == event_name]
        for go in hits:
            path = go['hierarchyPath']
            row = {'id': 'character_' + event_name.replace('.', '_'), 'eventName': event_name,
                   'source': full['source'], 'subtreeHierarchyPath': path, 'attachParentHierarchyPath': path.rsplit('/', 1)[0]}
            for key in ('gameObjects', 'transforms', 'particles', 'particleRenderers', 'meshComponents', 'components'):
                row[key] = [r for r in full[key] if r.get('hierarchyPath') == path or (r.get('hierarchyPath') or '').startswith(path + '/')]
            export_particle_prefab(row, out / 'prefabs')
            rows.append(row)
    return rows, unresolved


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--unit', default='VADER')
    parser.add_argument('--out', type=Path, default=ROOT / 'logs/vader_complete')
    parser.add_argument('--resources', action='store_true', help='Also write raw JSON configuration to Resources/AbilityPresentation')
    args = parser.parse_args()
    units = read(ROOT / 'catalog/units.json')
    unit = next(row for row in units if row['baseId'] == args.unit)
    skills = {r['id']: r for r in read(ROOT / 'catalog/gamedata/skill.json')['data']}
    abilities = {r['id']: r for r in read(ROOT / 'catalog/gamedata/ability.json')['data']}
    result = {'schemaVersion': 1, 'unit': args.unit, 'character': None, 'abilities': [],
              'limits': ['Decoded action params retain undecoded bytes; raw FSM is authoritative.',
                         'Wait finishEvent values can be safety timeouts, not animation hit timestamps.',
                         'Presentation graph export does not imply an implemented FSM/particle runtime.']}
    for ref in unit['skillReference']:
        sid = ref['skillId']
        if sid.split('_', 1)[0] not in ('basicskill', 'specialskill'):
            continue
        ability = abilities[skills[sid]['abilityReference']]
        if ability['abilityType'] not in (7, 10):
            continue
        name = ability['prefabName'].split('.', 1)[1]
        row = {'id': ability['id'], 'skillId': sid, 'prefabName': ability['prefabName'],
               **ability_asset(name)}
        result['abilities'].append(row)
        dump(args.out / (ability['id'] + '.json'), row)
        export_particle_prefab(row, args.out / 'prefabs')
    result['character'] = character_animation(unit['unitPrefab'].split('.', 1)[1])
    result['characterOverlays'], result['unresolvedParticleEvents'] = character_overlays(result['character'], args.out)
    result['areaOfEffectLayouts'] = [area_of_effect_layout('battle_character_pve_layout')]
    result['actionTypes'] = sorted({action['type'] for ability in result['abilities']
                                   for fsm in ability['fsm'] for state in fsm['decodedStates']
                                   for action in state['actions'] if action['enabled']})
    dump(args.out / 'ability-presentation.json', result)
    if args.resources:
        target = ROOT / 'unity/SWGoH-Restored/Assets/Resources/AbilityPresentation'
        dump(target / (args.unit + '.json'), result)
        export_particle_resources(result['abilities'] + result['characterOverlays'], target)
    print('Exported original presentation:', args.unit, len(result['abilities']), 'abilities,',
          len(result['character']['clips']), 'bound clips')


if __name__ == '__main__':
    main()
