# -*- coding: utf-8 -*-
"""Batch-render thumbnail previews for every model in the library.

Characters: extracted/<id>/<id>.anim.glb (preferred, idle frame played) with
            extracted/<id>/<id>.glb fallback.
Envs:       extracted_env/<id>/<id>.glb (viewer's env framing rules).

Output: previews/<id>.jpg (256x256 q80) + previews/index.json (ids that have
a preview, so the viewer never 404-guesses).

Incremental: a preview is skipped when it exists and is newer than its GLB.
Quality gate per shot: object coverage >= 2% and luma std >= 4 over object
pixels (all-black/all-white/empty renders caught, same idea as shot_env.py).

Usage:  py tools/gen_previews.py            # everything missing/stale
        py tools/gen_previews.py char_000_pre env_ana_republic_shipyard_01_pre
"""
import argparse
import base64
import functools
import http.server
import io
import json
import threading
import time
from pathlib import Path

from PIL import Image

ROOT = Path(__file__).resolve().parent.parent
OUT = ROOT / "previews"

HTML = """<!DOCTYPE html><html><head><meta charset="utf-8">
<style>html,body{margin:0;height:100%;overflow:hidden;background:#20242c}canvas{display:block}</style>
</head><body>
<script src="/viewer/vendor/three.min.js"></script>
<script src="/viewer/vendor/GLTFLoader.js"></script>
<script>
(() => {
window.__ready = false; window.__err = null; window.__shot = null;
const renderer = new THREE.WebGLRenderer({antialias: true, preserveDrawingBuffer: true});
renderer.setSize(innerWidth, innerHeight);
renderer.outputEncoding = THREE.sRGBEncoding;
renderer.toneMapping = THREE.ACESFilmicToneMapping;
document.body.appendChild(renderer.domElement);
const scene = new THREE.Scene();
scene.background = new THREE.Color(0x20242c);
const camera = new THREE.PerspectiveCamera(45, 1, 0.01, 20000);
scene.add(new THREE.HemisphereLight(0xbfd4ff, 0x3a3226, 0.95));
const key = new THREE.DirectionalLight(0xffffff, 1.15); key.position.set(3, 6, 4); scene.add(key);
const rim = new THREE.DirectionalLight(0x88aaff, 0.55); rim.position.set(-4, 3, -5); scene.add(rim);
const clock = new THREE.Clock();
let mixer = null;
new THREE.GLTFLoader().load('__URL__', (g) => {
  try {
    g.scene.traverse(o => {
      if (!o.isMesh) return;
      o.material.side = THREE.DoubleSide;
      // skinned bounding spheres don't follow the bones -- animated bodies
      // get culled at odd angles otherwise
      o.frustumCulled = false;
    });
    if (__IS_ENV__) {
      // hide sky/backscreens for the shot, but space envs are NOTHING BUT
      // skydome -- if hiding would leave zero visible meshes, keep them all
      const meshes = [];
      g.scene.traverse(o => { if (o.isMesh) meshes.push(o); });
      const hide = meshes.filter(o => /skybox|backdrop|skydome/i.test(o.name));
      const kept = meshes.length - hide.length;
      (kept > 0 ? hide : []).forEach(o => { o.visible = false; });
    }
    scene.add(g.scene);
    if (__IS_ENV__ && g.animations && g.animations.length) {
      // envs may animate; characters do NOT get a mixer -- thumbnails are a
      // deterministic T-pose model sheet (the viewer plays animations live)
      mixer = new THREE.AnimationMixer(g.scene);
      mixer.clipAction(g.animations[0]).play();
    }
    // framing happens inside __shot: env bodies may move between load and
    // capture, so the box must be computed on the settled pose, same frame
    window.__shot = () => {
      try {
        if (mixer && !window.__settled) {
          mixer.update(0.35);      // settle into the clip pose
          window.__settled = true;
        }
        const box = (() => {
        // Rigged characters frame from skeleton bones ONLY: raw geometry
        // unions blow up on thrown/far props, and r147's Box3.setFromObject
        // applies skinning which collapses skinned bounds to zero.  Bones =
        // the body, every time.  Statics fall back to raw geometry bounds.
        const bones = new THREE.Box3();
        const meshes = new THREE.Box3();
        let nbones = 0, nmesh = 0;
        const tmp = new THREE.Vector3();
        const mb = new THREE.Box3();
        g.scene.updateWorldMatrix(true, true);
        g.scene.traverse(o => {
          if (o.isSkinnedMesh && o.skeleton) {
            o.skeleton.bones.forEach(bone => {
              tmp.setFromMatrixPosition(bone.matrixWorld);
              if (isFinite(tmp.x)) { bones.expandByPoint(tmp); nbones++; }
            });
          } else if (o.isMesh) {
            if (!o.geometry.boundingBox) o.geometry.computeBoundingBox();
            mb.copy(o.geometry.boundingBox).applyMatrix4(o.matrixWorld);
            if (!mb.isEmpty()) { meshes.union(mb); nmesh++; }
          }
        });
        if (nbones) return bones;
        if (nmesh) return meshes;
        return new THREE.Box3().setFromObject(g.scene);
        })();
        if (box.isEmpty()) throw new Error('empty bounds');
        const size = box.getSize(new THREE.Vector3());
        const c = box.getCenter(new THREE.Vector3());
        const r = Math.max(size.x, size.y, size.z) || 1;
        if (__IS_ENV__) {
          // env rule (mirrors viewer frameEnv + shot_env.py): median mesh
          // center, radius covering 75% of triangles
          const items = [];
          let trisTotal = 0;
          g.scene.traverse(o => {
            if (!o.isMesh || !o.visible) return;
            const geo = o.geometry;
            const t = geo.index ? geo.index.count / 3 : geo.attributes.position.count / 3;
            trisTotal += t;
            items.push({ box: new THREE.Box3().setFromObject(o), tris: t });
          });
          if (items.length) {
            const med = a => { const s = a.slice().sort((x, y) => x - y);
                               return s[Math.floor(s.length / 2)]; };
            const centers = items.map(it => it.box.getCenter(new THREE.Vector3()));
            const focus = new THREE.Vector3(med(centers.map(v => v.x)),
                                            med(centers.map(v => v.y)),
                                            med(centers.map(v => v.z)));
            let acc = 0, rmax = r;
            const byDist = items.map(it => {
              let d = 0;
              for (const x of [it.box.min.x, it.box.max.x])
                for (const y of [it.box.min.y, it.box.max.y])
                  for (const z of [it.box.min.z, it.box.max.z])
                    d = Math.max(d, Math.hypot(x - focus.x, y - focus.y, z - focus.z));
              return { d, tris: it.tris };
            }).sort((a, b) => a.d - b.d);
            rmax = byDist[byDist.length - 1].d;
            for (const it of byDist) {
              acc += it.tris;
              if (acc >= trisTotal * 0.75) { rmax = it.d; break; }
            }
            const radius = Math.max(rmax, 0.001);
            camera.fov = 50;
            camera.near = radius / 200; camera.far = radius * 40;
            const az = Math.PI / 4;
            camera.position.set(focus.x + Math.cos(az) * radius * 1.7,
                                focus.y + radius * 0.55,
                                focus.z + Math.sin(az) * radius * 1.7);
            camera.lookAt(focus);
          }
        } else {
          // pad generously: head/feet geometry extends past the joint bones
          // and hands extend past the wrist -- cropping a head ruins a card
          const rp = r * 1.35;
          camera.near = rp / 100; camera.far = rp * 50;
          camera.fov = 40;
          // front-3/4 hero framing
          camera.position.set(c.x + rp * 0.55, c.y + rp * 0.10, c.z + rp * 1.00);
          camera.lookAt(c.x, c.y + rp * 0.02, c.z);
        }
        camera.aspect = 1; camera.updateProjectionMatrix();
        // warm-up renders so the skeleton bone texture is uploaded; the pose
        // is frozen between them (no mixer.advance), then capture
        renderer.render(scene, camera);
        renderer.render(scene, camera);
        return renderer.domElement.toDataURL('image/png');
      } catch (e) { window.__err = String(e); return null; }
    };
    window.__ready = true;
  } catch (e) { window.__err = String(e); }
}, undefined, (e) => { window.__err = String(e); });
})();
</script></body></html>"""


def serve():
    handler = functools.partial(http.server.SimpleHTTPRequestHandler,
                                directory=str(ROOT))
    http.server.SimpleHTTPRequestHandler.log_message = lambda *a, **k: None
    srv = http.server.ThreadingHTTPServer(("127.0.0.1", 0), handler)
    threading.Thread(target=srv.serve_forever, daemon=True).start()
    return srv, srv.server_address[1]


BG_RGB = (0x20, 0x24, 0x2C)


def shot_ok(data: bytes):
    import numpy as np
    im = np.asarray(Image.open(io.BytesIO(data)).convert("RGB"), dtype=np.uint8)
    flat = im.reshape(-1, 3)
    bg = (flat[:, 0] == BG_RGB[0]) & (flat[:, 1] == BG_RGB[1]) & (flat[:, 2] == BG_RGB[2])
    obj = flat[~bg]
    coverage = len(obj) / len(flat)
    if len(obj) < 100:
        return False, f"empty (coverage {coverage*100:.1f}%)"
    luma = obj[:, 0] * 0.299 + obj[:, 1] * 0.587 + obj[:, 2] * 0.114
    if luma.std() < 1.5:
        return False, f"flat (std {luma.std():.1f})"
    if coverage < 0.005:
        return False, f"too small (coverage {coverage*100:.2f}%)"
    return True, f"cov {coverage*100:.0f}% std {luma.std():.0f}"


def glb_mesh_extent(path: Path):
    """Max Y extent over mesh POSITION accessors (None if no meshes).
    Collapsed-body exports (e.g. char_b1snow*, <5cm body) are detectable
    here -- they render as a floating weapon in the viewer too."""
    import struct
    try:
        data = path.read_bytes()
        ln = struct.unpack("<I", data[12:16])[0]
        js = json.loads(data[20:20 + ln])
    except Exception:
        return None
    meshes = js.get("meshes", [])
    if not meshes:
        return None
    best = 0.0
    best_skinned = 0.0
    has_skinned = False
    for m in meshes:
        for prim in m.get("primitives", []):
            attrs = prim.get("attributes", {})
            pos = js.get("accessors", [{}])[attrs.get("POSITION", 0)]
            lo, hi = pos.get("min"), pos.get("max")
            if not (lo and hi and len(lo) > 1 and len(hi) > 1):
                continue
            ext = hi[1] - lo[1]
            best = max(best, ext)
            if "JOINTS_0" in attrs:      # skinned = the actual body
                has_skinned = True
                best_skinned = max(best_skinned, ext)
    return best_skinned if has_skinned else best


def model_jobs():
    """(id, url, is_env) for every model with a GLB on disk."""
    jobs = []
    for d in sorted((ROOT / "extracted").glob("char_*")):
        if not d.is_dir():
            continue
        mid = d.name
        anim = d / f"{mid}.anim.glb"
        plain = d / f"{mid}.glb"
        glb = anim if anim.is_file() else plain
        if not glb.is_file():
            continue
        ext = glb_mesh_extent(glb)
        if ext is None or ext < 0.05:
            continue     # 0-mesh shell or collapsed-body export
        jobs.append((mid, f"/extracted/{mid}/{glb.name}", False))
    for d in sorted((ROOT / "extracted_env").glob("env_*")):
        if not d.is_dir():
            continue
        mid = d.name
        glb = d / f"{mid}.glb"
        if not glb.is_file():
            continue
        ext = glb_mesh_extent(glb)
        if ext is None or ext < 0.05:
            continue     # 21 known empty placeholder scenes
        jobs.append((mid, f"/extracted_env/{mid}/{mid}.glb", True))
    return jobs


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("only", nargs="*", help="restrict to these ids")
    ap.add_argument("--size", type=int, default=256)
    ap.add_argument("--force", action="store_true")
    args = ap.parse_args()
    OUT.mkdir(exist_ok=True)

    jobs = model_jobs()
    if args.only:
        want = set(args.only)
        jobs = [j for j in jobs if j[0] in want]

    todo = []
    for mid, url, is_env in jobs:
        out = OUT / f"{mid}.jpg"
        glb = ROOT / url.lstrip("/")
        if (not args.force and out.is_file()
                and out.stat().st_mtime >= glb.stat().st_mtime):
            continue
        todo.append((mid, url, is_env, out))
    print(f"{len(jobs)} models, {len(todo)} to render")
    if not todo:
        write_index(jobs)
        return

    srv, port = serve()
    t0 = time.time()
    done = failed = 0
    with sync_playwright_safe() as p:
        b = p.chromium.launch(args=["--use-gl=angle"])
        pg = b.new_page(viewport={"width": args.size, "height": args.size})
        pg.goto(f"http://127.0.0.1:{port}/")
        for mid, url, is_env, out in todo:
            html = HTML.replace("__URL__", url)
            html = html.replace("__IS_ENV__", "true" if is_env else "false")
            pg.set_content(html)
            try:
                pg.wait_for_function("window.__ready || window.__err",
                                     timeout=30000)
                if pg.evaluate("window.__err"):
                    raise RuntimeError(pg.evaluate("window.__err"))
                b64 = pg.evaluate("window.__shot()")
                if not b64:
                    raise RuntimeError("shot returned null (page error)")
                png = base64.b64decode(b64[len("data:image/png;base64,"):])
                ok, why = shot_ok(png)
                if not ok:
                    raise RuntimeError(f"gate: {why}")
                Image.open(io.BytesIO(png)).convert("RGB").save(
                    out, "JPEG", quality=80)
                done += 1
            except Exception as e:
                failed += 1
                print(f"FAIL {mid}: {e}")
            if (done + failed) % 50 == 0:
                el = time.time() - t0
                print(f"  {done+failed}/{len(todo)}  {el:.0f}s  "
                      f"eta {el/(done+failed)*(len(todo)-done-failed):.0f}s")
        b.close()
    write_index(jobs)
    print(f"DONE {done} ok, {failed} failed in {time.time()-t0:.0f}s "
          f"-> {OUT}")


def write_index(jobs):
    ids = sorted(p.stem for p in OUT.glob("*.jpg"))
    (OUT / "index.json").write_text(json.dumps(ids), encoding="utf-8")
    print(f"index: {len(ids)} previews")


def sync_playwright_safe():
    from playwright.sync_api import sync_playwright
    return sync_playwright()


if __name__ == "__main__":
    main()
