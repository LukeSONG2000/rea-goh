"""Acceptance test for animated GLB exports.

Loads <char>/<char>.anim.glb in headless Chromium (three.js GLTFLoader),
plays the embedded clip, screenshots two distinct poses and diffs them.
A pose-visible pixel diff + zero console errors = pass.

Usage:  py tools/anim_accept.py [char_name]   (default char_jynerso_pre)
"""
import sys, os, time
from playwright.sync_api import sync_playwright

CHAR = sys.argv[1] if len(sys.argv) > 1 else 'char_jynerso_pre'
BASE = 'http://127.0.0.1:8137'
GLB = f'{BASE}/extracted/{CHAR}/{CHAR}.anim.glb'
OUT = 'shots'
os.makedirs(OUT, exist_ok=True)

PAGE = """<!DOCTYPE html><html><head><meta charset="utf-8"></head><body>
<script src="%VENDOR%/three.min.js"></script>
<script src="%VENDOR%/GLTFLoader.js"></script>
<script>
window.errors = [];
window.ready = false;
const scene = new THREE.Scene();
const camera = new THREE.PerspectiveCamera(45, 1.6, 0.01, 500);
const renderer = new THREE.WebGLRenderer({antialias:true, preserveDrawingBuffer:true});
renderer.setSize(800, 500);
document.body.appendChild(renderer.domElement);
scene.add(new THREE.HemisphereLight(0xffffff, 0x555566, 1.4));
const key = new THREE.DirectionalLight(0xffffff, 1.0); key.position.set(3,6,4); scene.add(key);
const rim = new THREE.DirectionalLight(0x88aaff, 0.6); rim.position.set(-4,3,-5); scene.add(rim);

let model, mixer, action, bones = [];
window.start = (url) => fetch(url).then(r=>r.arrayBuffer()).then(b=>{
  return new Promise((res,rej)=>{
    new THREE.GLTFLoader().parse(b, '', g=>{
      model = g.scene; scene.add(model);
      window.errors = [];
      if (!g.animations || !g.animations.length){ window.errors.push('NO ANIMATIONS'); return res(false); }
      mixer = new THREE.AnimationMixer(model);
      action = mixer.clipAction(g.animations[0]);
      action.play();
      bones = [];
      model.traverse(o=>{ if (o.isBone) bones.push(o); });
      // frame camera on the skinned bounds
      const box = new THREE.Box3().setFromObject(model);
      const size = box.getSize(new THREE.Vector3()), c = box.getCenter(new THREE.Vector3());
      const r = Math.max(size.x, size.y, size.z) || 1;
      camera.near = r/50; camera.far = r*50; camera.updateProjectionMatrix();
      camera.position.set(c.x + r*0.2, c.y + r*0.3, c.z + r*1.1);
      camera.lookAt(c);
      window.ready = true; res(true);
    }, e=>{ window.errors.push('parse: '+e); rej(e); });
  });
});
window.seek = (t) => { if(mixer){ mixer.setTime(t); renderer.render(scene,camera); } };
// world position of every bone at the currently posed time (render first!)
window.snap = () => bones.map(b=>{ const v=new THREE.Vector3(); b.getWorldPosition(v); return [v.x,v.y,v.z]; });
</script></body></html>"""

VENDOR = f'{BASE}/viewer/vendor'
html = PAGE.replace('%VENDOR%', VENDOR)
# same-origin serving: about:blank (set_content) can't fetch 127.0.0.1 (CORS),
# so drop the harness next to the server root and goto it
harness = os.path.join(os.path.dirname(__file__), '_anim_harness.html')
with open(harness, 'w', encoding='utf-8') as f:
    f.write(html)

with sync_playwright() as pw:
    browser = pw.chromium.launch(args=['--use-gl=angle'])
    page = browser.new_page(viewport={'width': 820, 'height': 520})
    console_errors = []
    page.on('console', lambda m: console_errors.append(m.text) if m.type == 'error' else None)
    page.on('pageerror', lambda e: console_errors.append(str(e)))
    page.goto(f'{BASE}/tools/_anim_harness.html')
    ok = page.evaluate(f'start("{GLB}")')
    for _ in range(300):
        if page.evaluate('window.ready'): break
        time.sleep(0.05)
    assert page.evaluate('window.ready'), 'model never became ready'

    page.evaluate('seek(0.0)')
    page.wait_for_timeout(150)
    t0 = f'{OUT}/{CHAR}.anim_t0.png'
    snap_a = page.evaluate('snap()')
    page.screenshot(path=t0)

    page.evaluate('seek(1.05)')   # half-way through the 2.17s clip
    page.wait_for_timeout(150)
    t1 = f'{OUT}/{CHAR}.anim_t1.png'
    snap_b = page.evaluate('snap()')
    page.screenshot(path=t1)

    browser.close()
    os.remove(harness)

# bone displacement: the authoritative playback signal (idle clips move bones
# far more reliably than they move pixels)
disp = [max(abs(pa[i]-pb[i]) for i in range(3)) for pa, pb in zip(snap_a, snap_b)]
moved = sum(1 for d in disp if d > 0.001)
top = sorted(disp, reverse=True)[:5]

from PIL import Image, ImageChops
import numpy as np
a = np.asarray(Image.open(t0).convert('RGB'), dtype=np.int16)
b = np.asarray(Image.open(t1).convert('RGB'), dtype=np.int16)
diff = np.abs(a - b).max(axis=2)
changed = float((diff > 5).mean() * 100)   # low threshold: idle is subtle

print(f'animation accepted: {CHAR}')
print(f'  frames       : {t0} / {t1}')
print(f'  bones        : {len(disp)} total, {moved} moved >0.001 (max {top[0]:.4f}, top5 {["%.3f"%d for d in top]})')
print(f'  pixels moved : {changed:.2f}% (>5/255 channel delta)')
print(f'  console errs : {len(console_errors)}', console_errors[:3] or '')
passed = (len(disp) > 0 and moved >= len(disp) * 0.5 and top[0] > 0.005
          and changed > 0.2 and not console_errors)
print('RESULT:', 'PASS' if passed else 'FAIL')
sys.exit(0 if passed else 1)
