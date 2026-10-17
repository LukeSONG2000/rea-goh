"""Export original character visibility, events and attachment identities.

No geometry/animation rewriting or name-based attachment inference. The same
JSON is consumable by Unity and the web viewer. Raw pathIDs have string identity
companions because JavaScript numbers cannot represent signed 64-bit IDs.
"""
import argparse
import json
import struct
from pathlib import Path

from export_ability_presentation import Bundle, MASK, ROOT, dump

COMPONENTS = {'UnitSubObjectAnimationEventHandler', 'UnitComponent',
              'CharacterUnitPresentation', 'BattleUnitChildPrefabLoader'}
RENDERERS = {'MeshRenderer', 'SkinnedMeshRenderer', 'ParticleSystemRenderer'}
SHARED_BUNDLE = None
STATE_CATALOG = None


def original_clips(bundle):
    """Use existing exporter aliases; event timing comes only from raw clips."""
    global SHARED_BUNDLE, STATE_CATALOG
    from export_animation_v2 import clip_priority
    if SHARED_BUNDLE is None:
        SHARED_BUNDLE = Bundle('shared_animationcontainer')
    if STATE_CATALOG is None:
        STATE_CATALOG = json.loads((ROOT / 'catalog/char_anim_states.json').read_text(encoding='utf-8'))
    states = STATE_CATALOG.get(bundle.name, {}).get('states', [])
    bound = set()
    for obj in bundle.objects.values():
        if obj.type.name == 'AnimatorController':
            for p in obj.read_typetree()['m_AnimationClips']:
                bound.add((p['m_FileID'] == 0, p['m_PathID'] & MASK))
    used, result = set(), []
    indexed = sorted(enumerate(states), key=lambda x: (not x[1]['default'], x[1]['layer'], x[0]))
    for _, state in indexed:
        label = state['name'].split('.')[-1].strip()
        motions = [m for m in state['motion'] if m.get('path_id')]
        multi = sum('threshold' in m for m in motions) > 1
        resolved = []
        for motion in motions:
            local = motion['source'] not in ('shared_animationcontainer',) and not str(motion['source']).startswith('ext')
            source = bundle if local else SHARED_BUNDLE
            obj = source.objects.get(motion['path_id'] & MASK)
            if obj is None or obj.type.name != 'AnimationClip':
                continue
            raw = obj.read_typetree()
            resolved.append((motion, source, obj, raw))
        resolved.sort(key=lambda x: (clip_priority(x[3]['m_Name'] or label), float(x[0].get('threshold') or 0)))
        for motion, source, obj, raw in resolved:
            base = f"{label}@{motion['threshold']}" if multi and 'threshold' in motion else label
            alias, suffix = base, 2
            while alias in used:
                alias = f'{base} ({suffix})'; suffix += 1
            used.add(alias)
            verified = (source is bundle, obj.path_id & MASK) in bound
            muscle = raw.get('m_MuscleClip', {})
            result.append({'animationName': alias, 'originalState': state['name'], 'layer': state['layer'],
                'defaultState': state['default'], 'stateSpeed': state['speed'], 'blendThreshold': motion.get('threshold'),
                'clipName': raw['m_Name'], 'clipPathId': str(obj.path_id), 'sourceBundle': source.name,
                'controllerPointerVerified': verified, 'status': 'EXACT_SOURCE_CLIP' if verified else 'PENDING_CONTROLLER_BINDING',
                'loopTime': muscle.get('m_LoopTime'), 'startTime': muscle.get('m_StartTime'), 'stopTime': muscle.get('m_StopTime'),
                'events': raw.get('m_Events', []),
                'timeBasis': 'Original AnimationClip event time; not inferred from semantic names.',
                'aliasMappingSource': 'catalog/char_anim_states.json + export_animation_v2 semantic naming'})
    return result


def glb_nodes(path):
    with path.open('rb') as stream:
        header = stream.read(20)
        if header[:4] != b'glTF':
            raise ValueError(f'Not a GLB: {path}')
        if header[16:20] != b'JSON':
            raise ValueError(f'Malformed GLB: JSON chunk type missing at offset 16 ({header[16:20].hex()})')
        data = json.loads(stream.read(struct.unpack_from('<I', header, 12)[0]))
    result = {}
    def walk(index, prefix):
        node = data['nodes'][index]
        full = prefix + '/' + node.get('name', '') if prefix else node.get('name', '')
        result.setdefault(full, []).append({'index': index, 'hasMesh': 'mesh' in node,
            'activeExtension': node.get('extensions', {}).get('SWGOH_active'),
            'rendererEnabledExtension': node.get('extensions', {}).get('SWGOH_renderer_enabled')})
        for child in node.get('children', []):
            walk(child, full)
    for root in data['scenes'][data.get('scene', 0)]['nodes']:
        walk(root, '')
    return result


def export(name):
    bundle = Bundle(name)
    rows = {'schemaVersion': 1, 'bundle': name, 'source': bundle.provenance(),
            'gameObjects': [], 'renderers': [], 'components': [], 'constraints': [],
            'glbAudit': [], 'limits': ['Authored initial state is not a substitute for original spawn/presentation transitions.',
                'Missing GLB event targets need original geometry restored, not a guessed replacement.',
                'GLB node identity matches require complete original hierarchy paths; ambiguous mappings remain pending.']}
    def pointer_binding(pointer):
        pid = pointer['m_PathID']
        obj = bundle.objects.get(pid & MASK) if pointer['m_FileID'] == 0 else None
        tree = obj.read_typetree() if obj else {}
        go = pid if obj and obj.type.name == 'GameObject' else tree.get('m_GameObject', {}).get('m_PathID', 0)
        return {'pathId': str(pid), 'fileId': pointer['m_FileID'],
                'type': obj.type.name if obj else None, 'hierarchyPath': bundle.path(go)}
    def references(value, path=''):
        out = []
        if isinstance(value, dict):
            if 'm_PathID' in value and 'm_FileID' in value:
                return [{'fieldPath': path, **pointer_binding(value)}]
            for key, child in value.items():
                out.extend(references(child, path + '.' + key if path else key))
        elif isinstance(value, (list, tuple)):
            for index, child in enumerate(value):
                out.extend(references(child, f'{path}[{index}]'))
        return out
    for obj in bundle.objects.values():
        kind = obj.type.name
        if kind == 'GameObject':
            raw = obj.read_typetree()
            tid = bundle.by_go.get(obj.path_id & MASK)
            transform = bundle.transforms.get(tid)
            rows['gameObjects'].append({'pathId': str(obj.path_id), 'name': raw['m_Name'],
                'hierarchyPath': bundle.path(obj.path_id), 'activeSelf': bool(raw['m_IsActive']),
                'layer': raw['m_Layer'], 'transformPathId': str(bundle.objects[tid].path_id) if tid else None,
                'rawTransform': transform})
        elif kind in RENDERERS:
            raw = obj.read_typetree()
            rows['renderers'].append({**bundle.record(obj, raw), 'enabled': bool(raw['m_Enabled']),
                                      'referenceBindings': references(raw)})
        elif kind == 'MonoBehaviour':
            raw = obj.read_typetree()
            cls = bundle.scripts.get(raw['m_Script']['m_PathID'] & MASK)
            if cls in COMPONENTS:
                rows['components'].append({**bundle.record(obj, raw), 'class': cls,
                                           'referenceBindings': references(raw)})
        elif kind.endswith('Constraint'):
            raw = obj.read_typetree()
            rows['constraints'].append({**bundle.record(obj, raw), 'referenceBindings': references(raw)})
    event_paths = {ref['hierarchyPath'] for c in rows['components'] if c['class'] == 'UnitSubObjectAnimationEventHandler'
                   for ref in c['referenceBindings'] if '.UnitSubObject' in ref['fieldPath'] and ref['hierarchyPath']}
    for suffix in ('.glb', '.anim.glb'):
        path = ROOT / 'extracted' / name / (name + suffix)
        if not path.exists():
            continue
        try:
            nodes = glb_nodes(path)
        except Exception as error:
            rows['glbAudit'].append({'file': path.relative_to(ROOT).as_posix(), 'error': str(error), 'bindings': []})
            continue
        bindings = []
        for go in rows['gameObjects']:
            matches = nodes.get(go['hierarchyPath'], [])
            bindings.append({'sourcePathId': go['pathId'], 'hierarchyPath': go['hierarchyPath'],
                'nodes': matches, 'status': 'EXACT' if len(matches) == 1 else 'AMBIGUOUS' if matches else 'ABSENT',
                'eventTarget': go['hierarchyPath'] in event_paths})
        rows['glbAudit'].append({'file': path.relative_to(ROOT).as_posix(), 'bindings': bindings})
    rows['clips'] = original_clips(bundle)
    rows['sharedAnimationSource'] = SHARED_BUNDLE.provenance()
    return rows


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('bundles', nargs='*')
    parser.add_argument('--all', action='store_true')
    parser.add_argument('--out', type=Path, default=ROOT / 'unity/SWGoH-Restored/Assets/Resources/CharacterPresentation')
    args = parser.parse_args()
    names = set(args.bundles)
    if args.all:
        names.update(p.name for p in (ROOT / 'extracted').iterdir() if p.is_dir() and p.name.startswith('char_'))
        units = json.loads((ROOT / 'catalog/units.json').read_text(encoding='utf-8'))
        names.update(u['unitPrefab'].split('.', 1)[1].lower() for u in units if u.get('unitPrefab', '').startswith('unit.char_'))
    if not names:
        names.add('char_vader_pre')
    summary_path = ROOT / 'logs/character_presentation/export-summary.json'
    summary = [] if args.all or not summary_path.exists() else [r for r in json.loads(summary_path.read_text(encoding='utf-8')) if r['bundle'] not in names]
    for name in sorted(names):
        try:
            data = export(name)
            dump(args.out / (name + '.json'), data)
            missing = [{'file': a['file'], 'hierarchyPath': b['hierarchyPath']} for a in data['glbAudit'] for b in a['bindings'] if b['eventTarget'] and b['status'] != 'EXACT']
            item = {'bundle': name, 'sourceSha256': data['source']['sha256'], 'gameObjects': len(data['gameObjects']),
                'renderers': len(data['renderers']), 'events': sum(len(c['raw'].get('_animationEvents', [])) for c in data['components']),
                'glbErrors': [{'file': a['file'], 'error': a['error']} for a in data['glbAudit'] if 'error' in a],
                'missingOrAmbiguousEventTargets': missing}
            print(name, 'GO', item['gameObjects'], 'events', item['events'], 'missing targets', len(missing), flush=True)
        except Exception as error:
            item = {'bundle': name, 'error': str(error)}
            print(name, 'ERROR', str(error), flush=True)
        summary.append(item)
    dump(summary_path, sorted(summary, key=lambda r: r['bundle']))


if __name__ == '__main__':
    main()
