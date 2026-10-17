"""Reproduce battle layout/environment configuration and exact HUD sprite crops.

Reads original local bundles only. No CDN/API access, GLB export, Unity launch,
or shared catalog/export writes. Source Unity transforms remain unmirrored;
runtime import owns any coordinate conversion needed for existing GLBs.
Run: py tools/export_battle_presentation.py
"""
import hashlib
import json
import uuid
from pathlib import Path

import numpy as np
import UnityPy
from PIL import Image
from scipy.spatial.transform import Rotation

import dump_battle_staging as staging

ROOT = Path(__file__).resolve().parents[1]
ASSETS = ROOT / 'unity/SWGoH-Restored/Assets'
LAYOUTS = ('battle_character_pve_layout', 'battle_ship_pve_layout', 'battle_ship_pvp_layout')
ENVIRONMENTS = ('env_space_endor_01_pre', 'env_tatooine_spaceport_pre')
CHROME = {
    'battle_btn_attack_basic_frame': 'battleui_view_rgba_atlas',
    'nav_arrow': 'battleui_view_rgba_atlas',
    'battle_raids_enemy_health_foreground': 'battleui_view_rgba_atlas',
    'unit_health_bg': 'battleui_view_rgba_atlas',
    'unit_health_fill': 'battleui_view_rgba_atlas',
    'btn_circle_idle': 'standard_atlas',
}


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def write_json(path, data):
    path.parent.mkdir(parents=True, exist_ok=True)
    payload = json.dumps(data, ensure_ascii=False, indent=2) + '\n'
    if not path.exists() or path.read_text(encoding='utf-8') != payload:
        path.write_text(payload, encoding='utf-8')


def source_bundle(name):
    directory = ROOT / 'raw_bundles' / name
    if directory.is_dir():
        source = staging.newest_data(directory)
        if source:
            return source
    source = ROOT / 'il2cpp/cdn_fetch/bundles' / (name + '.bundle')
    if not source.is_file():
        raise FileNotFoundError(f'Original local bundle not found: {name}')
    return source


def vec(values, keys):
    return dict(zip(keys, map(float, values)))


class OriginalBundle:
    def __init__(self, name):
        self.name = name
        self.source = source_bundle(name)
        self.env = UnityPy.load(str(self.source))
        self.objects = {o.path_id: o for o in self.env.objects}
        staging.TRS.clear()
        staging.GOS.clear()
        for obj in self.objects.values():
            if obj.type.name == 'Transform':
                staging.TRS[obj.path_id] = obj.read()
            elif obj.type.name == 'GameObject':
                staging.GOS[obj.path_id] = obj.read()
        self.by_go = {t.m_GameObject.m_PathID: t for t in staging.TRS.values()}

    def provenance(self):
        return {'id': self.name, 'bundle': str(self.source.resolve()),
                'sourceSha256': sha(self.source)}

    def hierarchy_path(self, transform):
        parent = staging.TRS.get(transform.m_Father.m_PathID)
        prefix = self.hierarchy_path(parent) + '/' if parent else ''
        return prefix + staging.go_of(transform).m_Name

    def node(self, path_id):
        obj = self.objects[path_id]
        transform = staging.TRS.get(path_id)
        if transform is None:
            go_id = path_id if obj.type.name == 'GameObject' else obj.read_typetree()['m_GameObject']['m_PathID']
            transform = self.by_go[go_id]
        matrix = staging.xform_chain(transform)
        scale = np.linalg.norm(matrix[:3, :3], axis=0)
        if np.any(scale == 0):
            raise ValueError('Zero-scale original staging transform: ' + self.hierarchy_path(transform))
        rotation = matrix[:3, :3] / scale
        if np.linalg.det(rotation) < 0 or not np.allclose(rotation.T @ rotation, np.eye(3), atol=1e-5):
            raise ValueError('Reflection/shear requires explicit transform handling: ' + self.hierarchy_path(transform))
        return {'path': self.hierarchy_path(transform), 'componentPathId': str(path_id),
                'transformPathId': str(transform.object_reader.path_id),
                'position': vec(matrix[:3, 3], 'xyz'),
                'rotation': vec(Rotation.from_matrix(rotation).as_quat(), 'xyzw'),
                'scale': vec(scale, 'xyz')}


def export_layout(name):
    bundle = OriginalBundle(name)
    record = dict(bundle.provenance(), groups=[])
    for obj in bundle.objects.values():
        if obj.type.name == 'Camera':
            if 'camera' in record:
                raise ValueError(f'Ambiguous multiple cameras in {name}')
            tree = obj.read_typetree()
            record['camera'] = dict(bundle.node(obj.path_id), fov=tree['field of view'],
                                    near=tree['near clip plane'], far=tree['far clip plane'],
                                    orthographic=tree['orthographic'])
        elif obj.type.name == 'MonoBehaviour':
            tree = obj.read_typetree()
            if '_unitMarkers' in tree:
                group = bundle.node(obj.path_id)
                group['slots'] = [bundle.node(p['m_PathID']) for p in tree['_unitMarkers']]
                group['restricted'] = [dict(tag=p['RestrictedTag'], marker=bundle.node(p['Marker']['m_PathID']))
                                       for p in tree['_markerRestrictions']]
                record['groups'].append(group)
    if not record['groups'] or 'camera' not in record:
        raise ValueError(f'Missing original groups/camera in {name}')
    return record


def export_environment(name):
    bundle = OriginalBundle(name)
    glb = ROOT / 'extracted_env' / name / (name + '.glb')
    record = dict(bundle.provenance(), glb=str(glb.resolve()), layouts=[], lights=[], layers=[])
    for transform in staging.TRS.values():
        game_object = staging.go_of(transform)
        record['layers'].append({'path': bundle.hierarchy_path(transform),
                                 'gameObjectPathId': str(game_object.object_reader.path_id),
                                 'transformPathId': str(transform.object_reader.path_id),
                                 'layer': game_object.m_Layer})
    for obj in bundle.objects.values():
        if obj.type.name not in ('MonoBehaviour', 'Light'):
            continue
        tree = obj.read_typetree()
        if '_battleLayouts' in tree:
            if record['layouts']:
                raise ValueError(f'Ambiguous environment layout controllers in {name}')
            record['environmentComponentPathId'] = str(obj.path_id)
            record['layouts'] = [dict(layout=v['Layout'], marker=bundle.node(v['Marker']['m_PathID']))
                                 for v in tree['_battleLayouts']]
            record['fog'] = tree['_environmentFogSettings']
        if obj.type.name == 'Light':
            record['lights'].append(dict(bundle.node(obj.path_id), type=tree['m_Type'],
                                         color=tree['m_Color'], intensity=tree['m_Intensity'],
                                         enabled=tree['m_Enabled'],
                                         cullingMask=tree['m_CullingMask']['m_Bits'],
                                         renderingLayerMask=tree.get('m_RenderingLayerMask', 1)))
    if not record['layouts'] or 'fog' not in record:
        raise ValueError(f'Missing original environment settings in {name}')
    return record


def sprite_meta(name, sprite):
    # Stable per-resource GUID, not a copy of another imported sprite identity.
    guid = uuid.uuid5(uuid.NAMESPACE_URL, 'swgoh-restored/BattleChrome/' + name).hex
    horizontal = 2 if sprite.get('mirrorHorizontal') else 1
    vertical = 2 if sprite.get('mirrorVertical') else 1
    return f'''fileFormatVersion: 2
guid: {guid}
TextureImporter:
  externalObjects: {{}}
  serializedVersion: 13
  mipmaps:
    enableMipMap: 0
    sRGBTexture: 1
  isReadable: 0
  textureType: 8
  textureShape: 1
  maxTextureSize: 4096
  alphaIsTransparency: 1
  spriteMode: 1
  spritePixelsToUnits: 100
  spriteMeshType: 1
  spriteAlignment: 0
  spritePivot: {{x: 0.5, y: 0.5}}
  spriteBorder: {{x: {sprite['borderLeft'] * horizontal}, y: {sprite['borderBottom'] * vertical}, z: {sprite['borderRight'] * horizontal}, w: {sprite['borderTop'] * vertical}}}
  textureSettings:
    serializedVersion: 2
    filterMode: 1
    aniso: 1
    mipBias: 0
    wrapU: 1
    wrapV: 1
    wrapW: 1
  platformSettings:
  - serializedVersion: 3
    buildTarget: DefaultTexturePlatform
    maxTextureSize: 4096
    textureFormat: -1
    textureCompression: 0
    compressionQuality: 50
    overridden: 0
  spriteSheet:
    serializedVersion: 2
    sprites: []
    outline: []
    physicsShape: []
    bones: []
    spriteID: {uuid.uuid5(uuid.NAMESPACE_URL, 'swgoh-restored/BattleChrome/sprite/' + name).hex}
    internalID: 0
    vertices: []
    indices:
    edges: []
    weights: []
    secondaryTextures: []
    nameFileIdTable: {{}}
  userData:
  assetBundleName:
  assetBundleVariant:
'''


def export_chrome():
    bundle = OriginalBundle('shared_uicontainer')
    # Bars/auto use battleui_view_rgba_atlas_ref; the keyboard badge instead
    # uses standard_atlas_ref. Resolve explicitly, not by global sprite name.
    atlases = {}
    for obj in bundle.objects.values():
        if obj.type.name != 'MonoBehaviour':
            continue
        tree = obj.read_typetree()
        if tree.get('m_Name') in set(CHROME.values()) and tree.get('mSprites'):
            name = tree['m_Name']
            if name in atlases:
                raise ValueError('Ambiguous original atlas ' + name)
            atlases[name] = (obj, tree)
    output = ASSETS / 'Resources/BattleChrome'
    output.mkdir(parents=True, exist_ok=True)
    records = []
    for name, atlas_name in CHROME.items():
        atlas, tree = atlases[atlas_name]
        material_id = tree['material']['m_PathID']
        material = bundle.objects[material_id].read_typetree()
        texture_ref = dict(material['m_SavedProperties']['m_TexEnvs'])['_MainTex']['m_Texture']
        if texture_ref['m_FileID'] != 0:
            raise ValueError('External texture requires explicit source resolution')
        texture_id = texture_ref['m_PathID']
        image = bundle.objects[texture_id].read().image
        sprites = {s['name']: s for s in tree['mSprites']}
        sprite = sprites[name]
        path = output / (name + '.png')
        crop = image.crop((sprite['x'], sprite['y'], sprite['x'] + sprite['width'],
                           sprite['y'] + sprite['height']))
        raw_pixel_sha = hashlib.sha256(crop.tobytes()).hexdigest()
        # Native SimpleFillMirror/UVCoord repeats the FULL source rectangle on
        # each geometry half. This is not an instance mFlip or half-UV crop.
        # SlicedFillMirror doubles the corresponding geometry border widths;
        # sprite_meta preserves them. The native sliced inner UV mapping still
        # requires separate visual validation, not a blanket parity claim.
        if sprite.get('mirrorRotate'):
            raise ValueError('Rotated mirror reconstruction not verified: ' + name)
        if sprite.get('mirrorHorizontal'):
            result = Image.new('RGBA', (crop.width * 2, crop.height))
            result.paste(crop, (0, 0))
            result.paste(crop.transpose(Image.Transpose.FLIP_LEFT_RIGHT), (crop.width, 0))
            crop = result
        if sprite.get('mirrorVertical'):
            result = Image.new('RGBA', (crop.width, crop.height * 2))
            result.paste(crop, (0, 0))
            result.paste(crop.transpose(Image.Transpose.FLIP_TOP_BOTTOM), (0, crop.height))
            crop = result
        crop.save(path)
        meta = path.with_suffix('.png.meta')
        desired_meta = sprite_meta(name, sprite)
        if not meta.exists() or meta.read_text(encoding='utf-8') != desired_meta:
            meta.write_text(desired_meta, encoding='utf-8')
        reference_atlas = ('standard_atlas_ref' if atlas_name == 'standard_atlas'
                           else 'battleui_view_rgba_atlas_ref')
        records.append(dict(name=name, resource='BattleChrome/' + name, atlas=atlas_name,
                            referenceAtlas=reference_atlas,
                            atlasPathId=str(atlas.path_id), materialPathId=str(material_id),
                            texturePathId=str(texture_id), rect=sprite,
                            outputSize=[crop.width, crop.height],
                            rawRgbaSha256=raw_pixel_sha,
                            reconstructedAtlasMirroring=bool(sprite.get('mirrorHorizontal') or sprite.get('mirrorVertical')),
                            reconstruction={
                                'horizontal': 'original|flipX(original)' if sprite.get('mirrorHorizontal') else 'none',
                                'vertical': 'original above flipY(original)' if sprite.get('mirrorVertical') else 'none',
                                'sourceVA': ['0x180EC5AC0', '0x180EE8390', '0x180EE9410', '0x180EE9360'],
                                'borderSourceVA': '0x180EC6CB0',
                                'borderScale': {'horizontal': 2 if sprite.get('mirrorHorizontal') else 1,
                                                'vertical': 2 if sprite.get('mirrorVertical') else 1},
                                'limits': 'No rotation supported; reconstructed sliced inner-UV parity needs visual validation.'},
                            pngSha256=sha(path)))
    return dict(bundle.provenance(), sprites=records)


def main():
    layouts = [export_layout(name) for name in LAYOUTS]
    environments = [export_environment(name) for name in ENVIRONMENTS]
    config = {'schemaVersion': 1, 'coordinateSystem': 'original Unity native world TRS',
              'layouts': layouts, 'environments': environments, 'chrome': export_chrome()}
    write_json(ASSETS / 'StreamingAssets/battle_presentation.json', config)
    resources = ASSETS / 'Resources/BattlePresentation'
    write_json(resources / 'stage_configuration.json', {'layouts': layouts})
    for environment in environments:
        write_json(resources / (environment['id'] + '_configuration.json'), environment)
    write_json(resources / 'sprite_provenance.json', config['chrome'])
    print(f'Exported {len(layouts)} original layouts, {len(environments)} environments, {len(CHROME)} HUD sprites.')


if __name__ == '__main__':
    main()
