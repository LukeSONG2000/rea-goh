# -*- coding: utf-8 -*-
"""Export char-bundle ParticleSystems -> extracted/<char>/vfx.json (+ vfx_tex).

Char emitters are ATTACHED to their GLB node (muzzle-flash nodes etc. ride
the skeleton), so entries carry "node": <GameObject name> and pos=[0,0,0]
local to that node.  The viewer/battle re-parents emitter groups by name and
drops entries whose node is missing.

Run:  py tools/export_char_vfx.py <char> [char ...]   (or --all)
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
from export_env_vfx import (
    _c, _f, flatten_minmax, minmax, shape_names,
)

MODULE_UV = ("tilesX", "tilesY", "frameCount")


def export_char_vfx(char, force=False):
    dst = os.path.join(ROOT, "extracted", char, "vfx.json")
    if os.path.exists(dst) and not force:
        return "cached"
    path = latest_bundle(char)
    if not path:
        return "no-bundle"
    tex_dir = os.path.join(ROOT, "extracted", char, "vfx_tex")
    os.makedirs(tex_dir, exist_ok=True)

    env = UnityPy.load(path)
    gos = {}
    for o in env.objects:
        if o.type.name == "GameObject":
            gos[o.path_id] = o.read_typetree().get("m_Name", "?")
    psr = {}
    for o in env.objects:
        if o.type.name == "ParticleSystemRenderer":
            t = o.read_typetree()
            psr[t.get("m_GameObject", {}).get("m_PathID", 0)] = t

    textures = {}
    def tex_png(pid):
        if pid in textures:
            return textures[pid]
        if not pid:
            return None
        for o in env.objects:
            if o.type.name == "Texture2D" and o.path_id == pid:
                t = o.read_typetree()
                nm = t.get("m_Name") or f"tex_{pid}"
                fn, i = nm, 2
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
        out = {}
        shader = None
        m = r.get("m_Materials") or []
        if m:
            pid = m[0].get("m_PathID", 0)
            for o in env.objects:
                if o.type.name == "Material" and o.path_id == pid:
                    t = o.read_typetree()
                    out["name"] = t.get("m_Name")
                    sh = t.get("m_Shader", {}).get("m_PathID", 0)
                    for o2 in env.objects:
                        if o2.type.name == "Shader" and o2.path_id == sh:
                            tt = o2.read_typetree()
                            shader = (tt.get("m_ParsedForm", {}) or {}).get("m_Name") or tt.get("m_Name")
                    props = t.get("m_SavedProperties", {})
                    for nm, data in (props.get("m_TexEnvs") or []):
                        if nm == "_MainTex":
                            tid = (data.get("m_Texture") or {}).get("m_PathID", 0)
                            if tid:
                                out["tex"] = tex_png(tid)
                    for nm, data in (props.get("m_Colors") or []):
                        if nm in ("_TintColor", "_Color", "_MainColor"):
                            out["tint"] = _c(data)
        return out, shader

    out_list = []
    for o in env.objects:
        if o.type.name != "ParticleSystem":
            continue
        t = o.read_typetree()
        go_pid = t.get("m_GameObject", {}).get("m_PathID", 0)
        init = t.get("InitialModule", {})
        mat, shader = mat_of(go_pid)
        uvmod = t.get("UVModule") or {}
        out_list.append({
            "name": gos.get(go_pid, "?"),
            "node": gos.get(go_pid, "?"),      # attach target in the GLB
            "pos": [0, 0, 0],
            "up": [0, 1, 0],
            "lifetime": flatten_minmax(init.get("startLifetime")),
            "speed": flatten_minmax(init.get("startSpeed")),
            "size": flatten_minmax(init.get("startSize")),
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
            "uv": {"tiles_x": uvmod.get("tilesX"), "tiles_y": uvmod.get("tilesY"),
                   "frames": uvmod.get("frameCount")} if uvmod.get("enabled") else None,
            "material": mat,
            "shader": shader,
        })

    if not out_list:
        return "no-ps"
    # crop UV-sheet textures to tile 0 (see export_env_vfx)
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
            im.crop((0, 0, max(w // tx, 1), max(h // ty, 1))).save(
                os.path.join(tex_dir, mat["tex"].rsplit(".", 1)[0] + "_f0.png"))
            mat["tex"] = mat["tex"].rsplit(".", 1)[0] + "_f0.png"
        except Exception:
            pass
    json.dump(out_list, open(dst, "w", encoding="utf-8"), ensure_ascii=False)
    return f"{len(out_list)} PS, {len(textures)} tex"


def main():
    args = [a for a in sys.argv[1:] if a != "--all"]
    if "--all" in sys.argv:
        args = sorted(os.listdir(os.path.join(ROOT, "extracted")))
    import collections
    stats = collections.Counter()
    for c in args:
        try:
            r = export_char_vfx(c, force=True)
        except Exception as ex:
            r = f"ERROR {type(ex).__name__}: {ex}"
        stats[r.split(",")[0]] += 1
        if not str(r).startswith(("cached", "no")):
            print(f"{c}: {r}", flush=True)
    print("\n==== char vfx summary ====")
    for k, v in sorted(stats.items()):
        print(f"  {k}: {v}")


if __name__ == "__main__":
    main()
