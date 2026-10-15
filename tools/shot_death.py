# -*- coding: utf-8 -*-
"""Load one char's anim glb, play the Death clip, screenshot mid-fall.
Needs game/server.py (8138) up.  Writes viewer/_shot.html temporarily."""
import json
import os
import sys
from playwright.sync_api import sync_playwright

CHAR = sys.argv[1] if len(sys.argv) > 1 else "char_vader_pre"
T_MS = int(sys.argv[2]) if len(sys.argv) > 2 else 1600
OUT = rf"F:\PPfavorite\Work\goh\shots\death_{CHAR}_{T_MS}.png"
TMP = r"F:\PPfavorite\Work\goh\viewer\_shot.html"

HTML = """<!DOCTYPE html><html><head><meta charset="utf-8"><style>
html,body{margin:0;overflow:hidden;background:#202838}canvas{display:block}</style></head>
<body><script src="/viewer/vendor/three.min.js"></script>
<script src="/viewer/vendor/GLTFLoader.js"></script><script>
const R=new THREE.WebGLRenderer({antialias:true});R.setSize(800,700);
R.outputEncoding=THREE.sRGBEncoding;document.body.appendChild(R.domElement);
const S=new THREE.Scene();S.background=new THREE.Color(0x202838);
const C=new THREE.PerspectiveCamera(40,800/700,.01,100);
S.add(new THREE.HemisphereLight(0xbfd4ff,0x3a3226,1));
const d=new THREE.DirectionalLight(0xffffff,1.2);d.position.set(3,6,4);S.add(d);
S.add(new THREE.GridHelper(6,12,0x446,0x335));
window.ADD=m=>{S.add(m);
  const box=new THREE.Box3(),v=new THREE.Vector3();
  m.updateWorldMatrix(true,true);
  m.traverse(o=>{if(o.isSkinnedMesh&&o.skeleton)o.skeleton.bones.forEach(b=>{
    v.setFromMatrixPosition(b.matrixWorld);box.expandByPoint(v);});});
  const c=box.getCenter(new THREE.Vector3()),s=box.getSize(new THREE.Vector3());
  C.position.set(c.x+s.x*.2, c.y+s.y*.7, c.z+s.z*2.1);C.lookAt(c);};
const clock=new THREE.Clock();
R.setAnimationLoop(()=>{const dt=clock.getDelta();
 if(window.MIX)window.MIX.update(dt);R.render(S,C);});
</script></body></html>"""

with open(TMP, "w", encoding="utf-8") as f:
    f.write(HTML)

with sync_playwright() as p:
    b = p.chromium.launch(args=["--use-gl=angle"])
    pg = b.new_page(viewport={"width": 800, "height": 700})
    errs = []
    pg.on("pageerror", lambda e: errs.append(str(e)))
    pg.goto(f"http://127.0.0.1:8138/viewer/_shot.html?t={os.urandom(4).hex()}")
    names = pg.evaluate("""(() => new Promise((res,rej)=>{
      fetch('/extracted/%s/%s.anim.glb').then(r=>r.arrayBuffer()).then(b=>
       new THREE.GLTFLoader().parse(b,'',g=>{
        window.GLB=g;
        const m=g.scene;m.traverse(o=>{if(o.isMesh){
          o.material.side=THREE.DoubleSide;o.frustumCulled=false;}});
        window.ADD(m);
        res(g.animations.map(a=>a.name));},rej),rej);
    }))""" % (CHAR, CHAR))
    print("clips:", names)
    death = next((n for n in names if "death" in n.lower()), None)
    if not death:
        print("NO DEATH CLIP"); b.close(); sys.exit(1)
    pg.evaluate("""(() => {
      const c=window.GLB.animations.find(a=>a.name===%s);
      const mixer=new THREE.AnimationMixer(window.GLB.scene);
      const a=mixer.clipAction(c);a.setLoop(THREE.LoopOnce,1);
      a.clampWhenFinished=true;a.play();window.MIX=mixer;
    })()""" % json.dumps(death))
    pg.wait_for_timeout(T_MS)
    pg.screenshot(path=OUT)
    b.close()
os.remove(TMP)
print("shot:", OUT, "errors:", errs or "none")
