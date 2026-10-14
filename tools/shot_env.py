# -*- coding: utf-8 -*-
"""Screenshot exported env GLBs with Playwright chromium (--use-gl=angle).

Serves the repo root over local http (GLTFLoader needs http), loads each
extracted_env/<name>/<name>.glb into a minimal three.js scene built from the
repo's vendored viewer scripts (viewer/vendor), tries a handful of candidate
viewpoints (orbit x4 / interior peek / top-down) and saves the most
informative one to shots/env_<name>.png.  Sky spheres, backdrop planes and
duplicated marker cubes are hidden for the shot.  Pixel stats (mean luma,
std, non-background share) gate the pass/fail so all-black / all-white /
empty renders are caught automatically.  Launch flags mirror
visual_accept.py.

Usage:  py tools/shot_env.py <name> [more...]      # extracted_env/<name>
"""
import argparse
import functools
import http.server
import threading
from pathlib import Path

from PIL import Image
from playwright.sync_api import sync_playwright

ROOT = Path(__file__).resolve().parent.parent
OUT = ROOT / "shots"

HTML = """<!DOCTYPE html><html><head><meta charset="utf-8">
<style>html,body{margin:0;height:100%;overflow:hidden;background:#20242c}canvas{display:block}</style>
</head><body>
<script src="/viewer/vendor/three.min.js"></script>
<script src="/viewer/vendor/GLTFLoader.js"></script>
<script>
(() => {
window.__ready = false; window.__err = null; window.__info = null;
const renderer = new THREE.WebGLRenderer({antialias: true});
renderer.setSize(innerWidth, innerHeight);
renderer.outputEncoding = THREE.sRGBEncoding;
document.body.appendChild(renderer.domElement);
const scene = new THREE.Scene();
scene.background = new THREE.Color(0x20242c);
const camera = new THREE.PerspectiveCamera(50, innerWidth / innerHeight, 0.01, 20000);
scene.add(new THREE.HemisphereLight(0xffffff, 0x445566, 0.45));
const dl = new THREE.DirectionalLight(0xffffff, 0.9); dl.position.set(3, 6, 4); scene.add(dl);
const dl2 = new THREE.DirectionalLight(0xffffff, 0.35); dl2.position.set(-4, 2, -3); scene.add(dl2);
let focus, radius;
window.__view = (i) => {
  const az = [45, 135, 225, 315][i] * Math.PI / 180;
  const p = new THREE.Vector3();
  if (i < 4) {                       // exterior orbit
    p.set(focus.x + Math.cos(az) * radius * 1.7,
          focus.y + radius * 0.55,
          focus.z + Math.sin(az) * radius * 1.7);
  } else if (i === 4) {              // interior peek: many envs are closed
    p.copy(focus)                    // rooms; an outside-only view shows walls
      .addScaledVector(new THREE.Vector3(1, 0.35, 1).normalize(), -radius * 0.45)
      .add(new THREE.Vector3(0, radius * 0.15, 0));
  } else {                           // top-down
    p.set(focus.x, focus.y + radius * 1.35, focus.z + radius * 0.01);
  }
  camera.position.copy(p);
  camera.lookAt(focus);
  renderer.render(scene, camera);
};
new THREE.GLTFLoader().load('__URL__', (g) => {
  try {
    // sky spheres / backdrop planes are legit GLB content but block the
    // view of the architecture; hide them for the acceptance shot
    g.scene.traverse(o => {
      if (o.isMesh && /skybox|backdrop/i.test(o.name)) o.visible = false;
    });
    // outlier-resistant focus: scenes park far-away markers / cinematic
    // hills that would balloon a plain Box3.  focus = median mesh center;
    // radius = distance covering 75% of triangles (long attached corridors
    // must not drag the camera off the main room).
    const items = [];
    let tris_total = 0;
    g.scene.traverse(o => {
      if (!o.isMesh || !o.visible) return;
      const geo = o.geometry;
      const tris = geo.index ? geo.index.count / 3 : geo.attributes.position.count / 3;
      tris_total += tris;
      items.push({ box: new THREE.Box3().setFromObject(o), tris });
    });
    if (!items.length) {
      items.push({ box: new THREE.Box3().setFromObject(g.scene), tris: 1 });
      tris_total = 1;
    }
    const med = a => { const s = a.slice().sort((x, y) => x - y);
                       return s[Math.floor(s.length / 2)]; };
    const centers = items.map(it => it.box.getCenter(new THREE.Vector3()));
    focus = new THREE.Vector3(med(centers.map(v => v.x)),
                              med(centers.map(v => v.y)),
                              med(centers.map(v => v.z)));
    let acc = 0, rmax = 1;
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
      if (acc >= tris_total * 0.75) { rmax = it.d; break; }
    }
    radius = Math.max(rmax, 0.001);
    camera.fov = 50;
    camera.near = radius / 200; camera.far = radius * 40;
    camera.updateProjectionMatrix();
    scene.add(g.scene);
    let meshes = 0, tris = 0;
    g.scene.traverse(o => { if (o.isMesh && o.visible) {
      meshes++;
      tris += o.geometry.index
        ? o.geometry.index.count / 3 : o.geometry.attributes.position.count / 3;
    }});
    window.__info = { meshes, tris, radius: +radius.toFixed(2) };
    window.__view(0);
    window.__ready = true;
  } catch (e) { window.__err = String(e); }
}, undefined, (e) => { window.__err = String(e); });
})();
</script></body></html>"""

N_VIEWS = 6
VIEW_NAMES = ["orbit45", "orbit135", "orbit225", "orbit315", "interior", "top"]


def serve():
    handler = functools.partial(http.server.SimpleHTTPRequestHandler,
                                directory=str(ROOT))
    http.server.SimpleHTTPRequestHandler.log_message = lambda *a, **k: None
    srv = http.server.ThreadingHTTPServer(("127.0.0.1", 0), handler)
    threading.Thread(target=srv.serve_forever, daemon=True).start()
    return srv, srv.server_address[1]


BG_RGB = (0x20, 0x24, 0x2C)


def png_stats(data: bytes):
    """Detail metrics against the flat background: contrast (luma std) is
    computed over object pixels only, so a view that is 85% empty background
    cannot win just by contrasting with the backdrop."""
    import io
    import numpy as np
    im = np.asarray(Image.open(io.BytesIO(data)).convert("RGB"), dtype=np.uint8)
    flat = im.reshape(-1, 3)
    bg = (flat[:, 0] == BG_RGB[0]) & (flat[:, 1] == BG_RGB[1]) & (flat[:, 2] == BG_RGB[2])
    obj = flat[~bg]
    n_obj = len(obj)
    coverage = n_obj / len(flat)
    if n_obj < 100:
        return {"mean": 0.0, "std": 0.0, "nonbg_pct": coverage * 100, "uniq": 0}
    luma = obj[:, 0] * 0.299 + obj[:, 1] * 0.587 + obj[:, 2] * 0.114
    codes = (obj[:, 0].astype(np.uint32) * 65536
             + obj[:, 1].astype(np.uint32) * 256 + obj[:, 2])
    return {"mean": float(luma.mean()), "std": float(luma.std()),
            "nonbg_pct": coverage * 100, "uniq": int(np.unique(codes).size)}


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("names", nargs="+")
    ap.add_argument("--size", default="1280x720")
    args = ap.parse_args()
    w, h = (int(v) for v in args.size.split("x"))

    srv, port = serve()
    failures = []
    with sync_playwright() as p:
        b = p.chromium.launch(args=["--use-gl=angle"])
        pg = b.new_page(viewport={"width": w, "height": h})
        pg.goto(f"http://127.0.0.1:{port}/")   # base URL so /viewer/vendor resolves
        errors = []
        pg.on("pageerror", lambda e: errors.append(f"[PAGEERROR] {e}"))
        pg.on("console", lambda m: errors.append(f"[{m.type}] {m.text}")
              if m.type == "error" else None)
        OUT.mkdir(exist_ok=True)
        for name in args.names:
            errors.clear()
            glb = ROOT / "extracted_env" / name / f"{name}.glb"
            if not glb.is_file():
                print(f"{name}: MISSING {glb}")
                failures.append(name)
                continue
            pg.set_content(HTML.replace("__URL__", f"/extracted_env/{name}/{name}.glb"))
            try:
                pg.wait_for_function("window.__ready || window.__err",
                                     timeout=30000)
            except Exception as e:
                print(f"{name}: TIMEOUT waiting for render ({e})")
                for e2 in errors[:8]:
                    print(f"    {e2}")
                failures.append(name)
                continue
            if pg.evaluate("window.__err"):
                print(f"{name}: LOAD ERROR {pg.evaluate('window.__err')}")
                failures.append(name)
                continue
            info = pg.evaluate("window.__info")
            # try every candidate viewpoint, keep the most informative
            best = None
            per_view = []
            for i in range(N_VIEWS):
                pg.evaluate(f"window.__view({i})")
                s = png_stats(pg.screenshot())
                # reward object contrast + colour variety, mildly discount
                # views that are mostly empty background
                score = ((s["std"] * 0.6 + s["uniq"] * 0.008)
                         * min(s["nonbg_pct"] / 25.0, 1.25))
                per_view.append(
                    f"{VIEW_NAMES[i]}:{s['std']:.0f}/{s['nonbg_pct']:.0f}%"
                    f"/u{s['uniq']}={score:.1f}")
                if best is None or score > best[0]:
                    best = (score, i, s)
            _, bi, s = best
            png = OUT / f"env_{name}.png"
            pg.evaluate(f"window.__view({bi})")
            pg.screenshot(path=str(png))
            flag = ""
            if s["nonbg_pct"] < 1 or s["std"] < 5:
                flag = "  << SUSPECT (blank?)"
                failures.append(name)
            print(f"{name}: {png.name} {png.stat().st_size/1024:.0f} KB | "
                  f"view={VIEW_NAMES[bi]} meshes={info['meshes']} "
                  f"tris~{int(info['tris'])} r={info['radius']} | "
                  f"luma mean={s['mean']:.0f} std={s['std']:.1f} "
                  f"nonbg={s['nonbg_pct']:.1f}% uniq={s['uniq']}{flag}")
            print(f"    views: {' '.join(per_view)}")
            for e in errors[:5]:
                print(f"    {e}")
        b.close()
    srv.shutdown()
    if failures:
        print("SUSPECT/FAILED:", ", ".join(failures))
        raise SystemExit(1)
    print("all OK")


if __name__ == "__main__":
    main()
