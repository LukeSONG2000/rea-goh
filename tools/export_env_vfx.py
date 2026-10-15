# -*- coding: utf-8 -*-
"""Export env ParticleSystems -> vfx.json (+ vfx_tex/*.png) per env scene.

Coordinates: bundle world position with X negated (Unity LH -> glTF RH),
verified against the exported GLB (dath_firepit_01_mesh: (-20.25,-0.14,8.38)
-> (20.25,-0.14,8.38)).

Run:  py tools/export_env_vfx.py <env> [env ...]      (or --all)
"""
import json
import os
import sys

import numpy as np

TOOLS = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(TOOLS)
sys.path.insert(0, TOOLS)

import UnityPy
from export_animation import latest_bundle

SHAPE_NAMES = {0: "sphere", 1: "cone", 2: "box", 3: "mesh", 4: "circle",
               5: "edge", 6: "hemisphere", 7: "donut", 8: "rectangle",
               9: "sphere_shell", 10: "cone_shell"}
shape_names = SHAPE_NAMES   # alias used by export_char_vfx

# ---- min/max gradient & curve helpers -----------------------------------

def _c(d):
    try:
        return [round(float(d["r"]), 4), round(float(d["g"]), 4),
                round(float(d["b"]), 4), round(float(d["a"]), 4)]
    except Exception:
        return None

def _f(v, d=0.0):
    try:
        return round(float(v), 4)
    except (TypeError, ValueError):
        return d

def gradient(g):
    """Serialized Gradient (key0..key7 {time,color}) -> [[t, rgba], ...]."""
    stops = []
    if not isinstance(g, dict):
        return stops
    for i in range(8):
        k = g.get(f"key{i}")
        if not k:
            continue
        c = _c(k.get("color", {}))
        if c:
            stops.append([_f(k.get("time"), i / 7), c])
    if not stops and isinstance(g.get("m_ColorKeys"), list):
        for k in g["m_ColorKeys"]:
            c = _c(k.get("color", {}))
            if c:
                stops.append([_f(k.get("time")), c])
    return stops

def minmax(m, want="color"):
    """Serialized MinMaxGradient / MinMaxCurve -> compact form."""
    if not isinstance(m, dict):
        return None
    state = m.get("minMaxState", 0)
    if want == "color":
        if state in (2, 3):                    # gradient variants
            g = gradient(m.get("maxGradient") or {})
            return {"mode": "gradient", "stops": g} if g else None
        c = _c(m.get("maxColor") or {})
        return {"mode": "const", "value": c} if c else None
    # curve
    mc = ((m.get("maxCurve") or {}).get("m_Curve")) or []
    keys = [[_f(k.get("time")), _f(k.get("value"))] for k in mc if isinstance(k, dict)]
    if state in (1, 2, 3) or keys:
        out = {"mode": "curve", "scalar": _f(m.get("scalar"), 1.0)}
        if keys:
            out["keys"] = keys
        return out
    return {"mode": "const", "value": _f(m.get("scalar"), 1.0)}

def flatten_minmax(m, d=0.0):
    """Scalar convenience (start size/speed/lifetime)."""
    if isinstance(m, dict):
        return _f(m.get("scalar"), d)
    return _f(m, d)

# ---- hierarchy ----------------------------------------------------------

def build_worlds(env):
    trs, gos = {}, {}
    for o in env.objects:
        if o.type.name == "Transform":
            trs[o.path_id] = o.read_typetree()
        elif o.type.name == "GameObject":
            gos[o.path_id] = o.read_typetree().get("m_Name", "?")

    def qmat(q):
        x, y, z, w = q
        return np.array([
            [1 - 2 * (y * y + z * z), 2 * (x * y - z * w), 2 * (x * z + y * w)],
            [2 * (x * y + z * w), 1 - 2 * (x * x + z * z), 2 * (y * z - x * w)],
            [2 * (x * z - y * w), 2 * (y * z + x * w), 1 - 2 * (x * x + y * y)]])

    memo = {}
    def world(pid):
        if pid in memo:
            return memo[pid]
        tr = trs[pid]
        lp = tr["m_LocalPosition"]; lq = tr["m_LocalRotation"]; ls = tr["m_LocalScale"]
        M = qmat([lq["x"], lq["y"], lq["z"], lq["w"]]) * np.array([ls["x"], ls["y"], ls["z"]])
        f = tr.get("m_Father", {})
        fpid = f.get("m_PathID", 0)
        if f.get("m_FileID", 0) != 0 or fpid == 0 or fpid not in trs:
            w, Mw = M @ np.array([lp["x"], lp["y"], lp["z"]]), M
        else:
            pw, Mw = world(fpid)
            w = Mw @ np.array([lp["x"], lp["y"], lp["z"]]) + pw
            Mw = M @ Mw
        memo[pid] = (w, Mw)
        return memo[pid]
    return trs, gos, world

# ---- main ----------------------------------------------------------------

def export_env_vfx(env, force=False):
    glb_dir = os.path.join(ROOT, "extracted_env", env)
    dst = os.path.join(glb_dir, "vfx.json")
    if os.path.exists(dst) and not force:
        return "cached"
    path = latest_bundle(env)
    if not path:
        return "no-bundle"
    tex_dir = os.path.join(glb_dir, "vfx_tex")
    os.makedirs(tex_dir, exist_ok=True)

    env_ = UnityPy.load(path)
    trs, gos, world = build_worlds(env_)

    # go -> its ParticleSystemRenderer (for materials)
    psr = {}
    for o in env_.objects:
        if o.type.name == "ParticleSystemRenderer":
            t = o.read_typetree()
            psr[t.get("m_GameObject", {}).get("m_PathID", 0)] = t

    textures = {}   # path_id -> png name
    def tex_png(pid):
        if pid in textures:
            return textures[pid]
        if not pid:
            return None
        for o in env_.objects:
            if o.type.name == "Texture2D" and o.path_id == pid:
                t = o.read_typetree()
                nm = t.get("m_Name") or f"tex_{pid}"
                fn = nm
                i = 2
                while os.path.exists(os.path.join(tex_dir, fn + ".png")):
                    fn = f"{nm}_{i}"; i += 1
                try:
                    o.read().image.save(os.path.join(tex_dir, fn + ".png"))
                    textures[pid] = fn + ".png"
                    return textures[pid]
                except Exception:
                    return None
        return None

    def mat_of(go_pid):
        r = psr.get(go_pid)
        if not r:
            return None, None
        m = r.get("m_Materials") or []
        out = {}
        shader = None
        if m:
            pid = m[0].get("m_PathID", 0)
            for o in env_.objects:
                if o.type.name == "Material" and o.path_id == pid:
                    t = o.read_typetree()
                    out["name"] = t.get("m_Name")
                    sh = t.get("m_Shader", {}).get("m_PathID", 0)
                    for o2 in env_.objects:
                        if o2.type.name == "Shader" and o2.path_id == sh:
                            shader = o2.read_typetree().get("m_ParsedForm", {}).get("m_Name") \
                                     or o2.read_typetree().get("m_Name")
                    props = t.get("m_SavedProperties", {})
                    # m_TexEnvs/m_Colors come out as (name, data) tuple pairs
                    for nm, data in (props.get("m_TexEnvs") or []):
                        if nm == "_MainTex":
                            tid = (data.get("m_Texture") or {}).get("m_PathID", 0)
                            if tid:
                                out["tex"] = tex_png(tid)
                    for nm, data in (props.get("m_Colors") or []):
                        if nm in ("_TintColor", "_Color", "_MainColor"):
                            out["tint"] = _c(data)
        return out, shader

    shape_names = {0: "sphere", 1: "cone", 2: "box", 3: "mesh", 4: "circle",
                   5: "edge", 6: "hemisphere", 7: "donut", 8: "rectangle",
                   9: "sphere_shell", 10: "cone_shell"}

    out_list = []
    for o in env_.objects:
        if o.type.name != "ParticleSystem":
            continue
        t = o.read_typetree()
        go_pid = t.get("m_GameObject", {}).get("m_PathID", 0)
        # find the transform belonging to this GO
        tpid = None
        for tp, tr in trs.items():
            if tr.get("m_GameObject", {}).get("m_PathID", 0) == go_pid:
                tpid = tp
                break
        pos = [0, 0, 0]
        up = [0, 1, 0]
        if tpid:
            wp, Mw = world(tpid)
            pos = [round(-wp[0], 3), round(wp[1], 3), round(wp[2], 3)]   # X flip
            u = Mw @ np.array([0, 1, 0])
            n = np.linalg.norm(u)
            if n > 1e-6:
                up = [round(float(-u[0] / n), 3), round(float(u[1] / n), 3),
                      round(float(u[2] / n), 3)]

        init = t.get("InitialModule", {})
        mat, shader = mat_of(go_pid)
        rec = {
            "name": gos.get(go_pid, "?"),
            "pos": pos,
            "up": up,
            "lifetime": flatten_minmax(init.get("startLifetime")),
            "speed": flatten_minmax(init.get("startSpeed")),
            "size": flatten_minmax(init.get("startSize")),
            "size3": [flatten_minmax(init.get("startSize", {}).get(k)) for k in ("x", "y", "z")]
                     if isinstance(init.get("startSize"), dict) and "x" in init.get("startSize", {}) else None,
            "maxParticles": init.get("maxNumParticles"),
            "color": minmax(init.get("startColor"), "color"),
            "rate": flatten_minmax(t.get("EmissionModule", {}).get("rateOverTime")),
            "gravity": flatten_minmax(t.get("gravityModifier")),
            "shape": {"type": shape_names.get((t.get("ShapeModule") or {}).get("type"),
                                              str((t.get("ShapeModule") or {}).get("type"))),
                      "radius": _f((t.get("ShapeModule") or {}).get("radius"))},
            "velocity": {k: flatten_minmax((t.get("VelocityModule") or {}).get(k))
                         for k in ("x", "y", "z")},
            "colorOverLifetime": minmax((t.get("ColorModule") or {}).get("gradient"), "color"),
            "sizeOverLifetime": minmax((t.get("SizeModule") or {}).get("curve"), "curve"),
            "uv": {"tiles_x": (t.get("UVModule") or {}).get("tilesX"),
                   "tiles_y": (t.get("UVModule") or {}).get("tilesY"),
                   "frames": (t.get("UVModule") or {}).get("frameCount")} if (t.get("UVModule") or {}).get("enabled") else None,
            "material": mat,
            "shader": shader,
        }
        out_list.append(rec)

    if not out_list:
        return "no-ps"
    # UV sheet textures (e.g. fire_2x2_01): crop tile 0 so the viewer can use
    # the png directly as a point sprite.
    n_crop = 0
    for rec in out_list:
        uv, mat = rec.get("uv"), rec.get("material")
        if not uv or not mat or not mat.get("tex"):
            continue
        tx, ty = uv.get("tiles_x") or 1, uv.get("tiles_y") or 1
        if tx * ty <= 1:
            continue
        src = os.path.join(tex_dir, mat["tex"])
        if not os.path.exists(src):
            continue
        try:
            from PIL import Image
            im = Image.open(src)
            w, h = im.size
            crop = im.crop((0, 0, max(w // tx, 1), max(h // ty, 1)))
            out = mat["tex"].rsplit(".", 1)[0] + "_f0.png"
            crop.save(os.path.join(tex_dir, out))
            mat["tex"] = out
            n_crop += 1
        except Exception:
            pass
    json.dump(out_list, open(dst, "w", encoding="utf-8"), ensure_ascii=False)
    return f"{len(out_list)} PS, {len(textures)} tex, {n_crop} cropped"


def main():
    args = [a for a in sys.argv[1:] if a != "--all"]
    if "--all" in sys.argv:
        args = sorted(os.listdir(os.path.join(ROOT, "extracted_env")))
    import collections
    stats = collections.Counter()
    for e in args:
        try:
            r = export_env_vfx(e, force=True)
        except Exception as ex:
            r = f"ERROR {type(ex).__name__}: {ex}"
        stats[r.split(",")[0]] += 1
        if not r.startswith("cached") and not r.startswith("no"):
            print(f"{e}: {r}", flush=True)
    print("\n==== vfx summary ====")
    for k, v in sorted(stats.items()):
        print(f"  {k}: {v}")


if __name__ == "__main__":
    main()
