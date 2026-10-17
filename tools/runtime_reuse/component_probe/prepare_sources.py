"""Prepare exact local AssetBundle dependencies for the direct-component probe."""
import sys,json,hashlib,collections
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
sys.path.insert(0,str(ROOT/'tools'))
from export_ability_presentation import Bundle
NAMES=['shared_resourcecontainer','shared_uicontainer','shared_shadercontainer','shared_animationcontainer',
       'char_vader_pre','ability_char_saber_jumpstrikesingle_basic','ability_char_force_crush_aoe_lb2',
       'ability_char_saber_projectile_singletarget','ability_char_buff_merciless_lb3']
def main():
    rows=[];evidence=[]
    for name in NAMES:
        b=Bundle(name);a=next(iter(b.objects.values())).assets_file
        assets=list(b.env.container)
        rows.append(dict(id=name,path=str(b.source.resolve()),sha256=b.provenance()['sha256'],
                         cab=a.name,version=a.unity_version,targetPlatform=int(a.target_platform),dependencies=[e.path for e in a.externals],
                         prefabs=[s for s in assets if s.endswith('.prefab')] if not name.startswith('shared_') else []))
        scripts=[]
        for o in b.objects.values():
            if o.type.name=='MonoScript':
                t=o.read_typetree();scripts.append({k:t.get(k) for k in ['m_Name','m_ClassName','m_Namespace','m_AssemblyName']})
        missing_boundary=[]
        for o in b.objects.values():
            if o.type.name!='MonoBehaviour':continue
            t=o.read_typetree();go=t.get('m_GameObject',{}).get('m_PathID',0)
            missing_boundary.append(dict(pathId=str(o.path_id),hierarchyPath=b.path(go) if go else None,
                originalClass=b.scripts.get(t['m_Script']['m_PathID']&((1<<64)-1),'EXTERNAL_SCRIPT'),scriptReference=t['m_Script']))
        evidence.append(dict(id=name,source=rows[-1],types=dict(collections.Counter(o.type.name for o in b.objects.values())),scripts=scripts,scriptComponents=missing_boundary))
    cabs={r['cab'] for r in rows}
    missing=sorted({d for r in rows for d in r['dependencies'] if d.startswith('archive:') and d.rsplit('/',1)[-1] not in cabs})
    assert not missing,missing
    data=dict(schemaVersion=1,requiredVersion='6000.3.17f1',bundles=rows)
    p=ROOT/'unity/OriginalComponentProbe/Assets/StreamingAssets/sources.json';p.parent.mkdir(parents=True,exist_ok=True)
    p.write_text(json.dumps(data,indent=2),encoding='utf-8')
    q=ROOT/'logs/high_reuse/components/source-evidence.json';q.parent.mkdir(parents=True,exist_ok=True)
    q.write_text(json.dumps(dict(sources=evidence,unresolvedArchiveDependencies=missing),indent=2),encoding='utf-8')
    print(f'{len(rows)} original bundles, all external archive dependencies resolved; versions={set(r["version"] for r in rows)}')
if __name__=='__main__':main()
