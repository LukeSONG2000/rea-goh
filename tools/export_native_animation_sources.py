"""Lossless source inputs for Unity-native Avatar/AnimationClip evaluation."""
import argparse
from pathlib import Path
from export_ability_presentation import Bundle, MASK, ROOT, dump
from export_character_presentation import original_clips


def node(value):
    return {'name': value.m_Name, 'type': value.m_Type, 'version': value.m_Version,
            'children': [node(c) for c in value.m_Children]}


def export(character, output):
    bundle = Bundle(character)
    shared = Bundle('shared_animationcontainer')
    folder = output / character
    manifest = {'character': character, 'source': bundle.provenance(), 'avatars': [],
                'selectedAvatarFile': None, 'clips': [], 'controllers': [],
                'hierarchyFile': 'hierarchy.json', 'animatorBindings': []}
    def save_object(obj, source, filename):
        dump(folder / filename, {'source': source.provenance(), 'unityVersion': obj.assets_file.unity_version,
            'pathId': str(obj.path_id), 'raw': obj.read_typetree(), 'typeTree': node(obj.serialized_type.node)})
    hierarchy = {'source': bundle.provenance(), 'gameObjects': [], 'transforms': [], 'animators': []}
    primary_animators = set()
    for obj in bundle.objects.values():
        if obj.type.name == 'MonoBehaviour':
            raw = obj.read_typetree()
            if bundle.scripts.get(raw['m_Script']['m_PathID'] & MASK) == 'UnitFSMController' and raw['_animator']['m_PathID']:
                primary_animators.add(raw['_animator']['m_PathID'] & MASK)
    if len(primary_animators) > 1:
        raise ValueError('Multiple UnitFSMController Animator bindings need explicit selection')
    primary = bundle.objects[next(iter(primary_animators))].read_typetree() if primary_animators else None
    primary_controller = primary['m_Controller']['m_PathID'] & MASK if primary else None
    manifest['selectedAnimatorPathId'] = str(bundle.objects[next(iter(primary_animators))].path_id) if primary_animators else None
    manifest['animatorSelectionEvidence'] = 'UnitFSMController._animator' if primary else 'single unambiguous Avatar reference'
    selected = set()
    clip_refs = set()
    for obj in bundle.objects.values():
        kind = obj.type.name
        if kind in ('GameObject', 'Transform', 'Animator'):
            row = bundle.record(obj, obj.read_typetree())
            hierarchy[{'GameObject': 'gameObjects', 'Transform': 'transforms', 'Animator': 'animators'}[kind]].append(row)
            if kind == 'Animator':
                raw = row['raw']
                binding = {'pathId': str(obj.path_id), 'hierarchyPath': row['hierarchyPath'],
                           'avatar': raw['m_Avatar'], 'controller': raw['m_Controller']}
                manifest['animatorBindings'].append(binding)
                if raw['m_Avatar']['m_PathID']:
                    if raw['m_Avatar']['m_FileID'] != 0:
                        raise ValueError('External Avatar needs explicit external resolution')
                    selected.add(raw['m_Avatar']['m_PathID'] & MASK)
        if kind == 'Avatar':
            filename = f'avatar_{obj.path_id}.json'
            save_object(obj, bundle, filename)
            manifest['avatars'].append({'file': filename, 'pathId': str(obj.path_id)})
        elif kind == 'AnimatorController':
            filename = f'controller_{obj.path_id}.json'
            save_object(obj, bundle, filename)
            manifest['controllers'].append({'file': filename, 'pathId': str(obj.path_id)})
            if primary_controller is None or obj.path_id & MASK == primary_controller:
                for pointer in obj.read_typetree()['m_AnimationClips']:
                    clip_refs.add((pointer['m_FileID'] == 0, pointer['m_PathID'] & MASK))
    if primary:
        selected = {primary['m_Avatar']['m_PathID'] & MASK}
    if len(selected) != 1:
        raise ValueError(f'Expected one explicitly Animator-bound Avatar, got {len(selected)}')
    chosen = next(iter(selected))
    manifest['selectedAvatarFile'] = next(a['file'] for a in manifest['avatars'] if int(a['pathId']) & MASK == chosen)
    aliases = original_clips(bundle)
    for local, pid in sorted(clip_refs):
        source = bundle if local else shared
        obj = source.objects.get(pid)
        if obj is None or obj.type.name != 'AnimationClip':
            raise ValueError(f'Original bound clip unresolved: {local}/{pid}')
        filename = f'clip_{source.name}_{obj.path_id}.json'
        save_object(obj, source, filename)
        bindings = [a for a in aliases if a['sourceBundle'] == source.name and int(a['clipPathId']) & MASK == pid]
        manifest['clips'].append({'file': filename, 'pathId': str(obj.path_id), 'sourceBundle': source.name,
            'semanticNames': [a['animationName'] for a in bindings], 'semanticBindings': bindings})
    dump(folder / 'hierarchy.json', hierarchy)
    dump(folder / 'manifest.json', manifest)
    print(character, 'clips', len(manifest['clips']), 'avatar', manifest['selectedAvatarFile'], flush=True)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('characters', nargs='+')
    parser.add_argument('--out', type=Path, default=ROOT / 'logs/pose_recovery/native_sources')
    args = parser.parse_args()
    for character in args.characters:
        export(character, args.out)


if __name__ == '__main__':
    main()
