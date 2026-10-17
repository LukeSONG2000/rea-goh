import sys,json,hashlib
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
import swgoh_extract as se
import UnityPy
import argparse
ap=argparse.ArgumentParser(description='Export original Endor space material bindings and textures without shader substitutions.')
ap.add_argument('--out',type=Path,default=Path(__file__).resolve().parents[1]/'unity/SWGoH-Restored/Assets/Resources/SpaceMaterials')
root=ap.parse_args().out
(root/'textures').mkdir(parents=True,exist_ok=True)
source=se.newest_bundle(se.CACHE/'env_space_endor_01_pre')
env=UnityPy.load(*map(str,se.resolve_deps_recursively(source)))
transforms={}
for o in env.objects:
    if o.type.name=='Transform':
        t=o.read()
        try: transforms[se.rkey(t.m_GameObject.deref())]=t
        except Exception: pass
def object_path(go):
    t=transforms.get(se.rkey(go.object_reader))
    if t is not None and t.m_Father.m_PathID:
        parent=t.m_Father.read().m_GameObject.read()
        return object_path(parent)+'/'+go.m_Name
    return go.m_Name
names={'end_planet_mat','stars32_mat','end_planet_atmosphere_mat','end_planet_atmosphere_ring_mat'}
materials=[]
for o in env.objects:
    if o.type.name!='Material': continue
    m=o.read()
    if m.m_Name not in names: continue
    shader=m.m_Shader.read_typetree()['m_ParsedForm']; saved=o.read_typetree()['m_SavedProperties']
    item=dict(name=m.m_Name,pathId=str(o.path_id),shader=shader['m_Name'],textures=[],colors=[dict(name=k,value=v) for k,v in saved['m_Colors']],floats=[dict(name=k,value=v) for k,v in saved['m_Floats']],passes=[])
    for sub in shader['m_SubShaders']:
        for p in sub['m_Passes']:
            s=p['m_State'];b=s['rtBlend0']
            item['passes'].append(dict(srcBlend=b['srcBlend']['val'],dstBlend=b['destBlend']['val'],srcBlendAlpha=b['srcBlendAlpha']['val'],dstBlendAlpha=b['destBlendAlpha']['val'],zWrite=s['zWrite']['val'],zTest=s['zTest']['val'],cull=s['culling']['val'],colorMask=b['colMask']['val'],tags=s['m_Tags']['tags']))
    for k,v in m.m_SavedProperties.m_TexEnvs:
        if not v.m_Texture.m_PathID:continue
        t=v.m_Texture.read();path=root/'textures'/(t.m_Name+'.png');t.image.save(path)
        item['textures'].append(dict(property=k,name=t.m_Name,path=str(path.resolve()),resourcePath='SpaceMaterials/textures/'+t.m_Name,pathId=str(v.m_Texture.m_PathID),scale=dict(x=v.m_Scale.x,y=v.m_Scale.y),offset=dict(x=v.m_Offset.x,y=v.m_Offset.y),sha256=hashlib.sha256(path.read_bytes()).hexdigest()))
    materials.append(item)
bindings=[]
for o in env.objects:
    if o.type.name!='MeshRenderer':continue
    r=o.read();go=r.m_GameObject.read(); slots=[]
    for i,p in enumerate(r.m_Materials):
        try:
            m=p.read()
            if m.m_Name in names:slots.append(dict(slot=i,material=m.m_Name,pathId=str(p.m_PathID)))
        except Exception: pass
    if slots:bindings.append(dict(renderer=go.m_Name,path=object_path(go),gameObjectPathId=str(r.m_GameObject.m_PathID),rendererPathId=str(o.path_id),layer=go.m_Layer,enabled=r.m_Enabled,slots=slots))
(root/'materials.json').write_text(json.dumps(dict(source=str(source),sourceSha256=hashlib.sha256(source.read_bytes()).hexdigest(),limitations=['Original shader programs are not exported; planet MaskTex/cloud/lighting program remains PENDING-ORIGINAL.','Pass states and saved properties are original serialized evidence; stale material properties do not prove shader usage.'],materials=materials,bindings=bindings),indent=2),encoding='utf8')
print('materials',len(materials),'bindings',bindings)
