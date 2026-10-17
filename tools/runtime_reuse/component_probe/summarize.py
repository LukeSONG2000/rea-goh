"""Compare native runtime component counts against read-only bundle inventory."""
import json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[3]
LOG=ROOT/'logs/high_reuse/components'
def main():
    r=json.loads((LOG/'player/report.json').read_text(encoding='utf-8-sig'))
    sources={s['id']:s for s in json.loads((LOG/'source-evidence.json').read_text())['sources']}
    rows=[]
    for p in r['prefabs']:
        raw=sources[p['source']]
        rows.append(dict(source=p['source'],loaded=p['loaded'],error=p.get('error'),
            originalGameObjects=raw['types'].get('GameObject',0),loadedTransforms=p['transforms'],
            originalParticles=raw['types'].get('ParticleSystem',0),loadedParticles=len(p['particles']),
            emittingParticles=sum(x['particles'] for x in p['particles']),
            missingMeshes=p['missingMeshes'],missingMeshPaths=p.get('missingMeshPaths',[]),missingScripts=p['missingScripts'],
            shaderMaterials=len(p['materials']),unsupportedShaders=[m for m in p['materials'] if not m['supported']],
            animators=p['animators']))
    out=dict(exactVersion=r['exactVersion'],completed=r['completed'],frames=r['frames'],
             graphicsDevice=r['graphicsDevice'],graphicsApi=r['graphicsApi'],
             bundlesLoaded=sum(b['loaded'] for b in r['bundles']),bundleFailures=[b for b in r['bundles'] if not b['loaded']],
             prefabs=rows,limits=['Missing original game scripts are not recreated or declared functional.',
                 'Diagnostic sampling sets Animator AlwaysAnimate and enables prefab roots; original child active states and assets are not altered.',
                 'Tron Trail has a null mesh pointer in the original bundle; its missing script must generate the mesh.',
                 'Particle Simulate validates native modules; missing FSM spawn/timing remains a separate boundary.',
                 'Native shader support plus screenshot is component evidence, not full original scene lighting parity.'])
    (LOG/'summary.json').write_text(json.dumps(out,indent=2),encoding='utf-8')
    print(json.dumps(out,indent=2))
if __name__=='__main__':main()
