# -*- coding: utf-8 -*-
"""Inventory ParticleSystems across env and char bundles.
Each PS: GameObject name, which modules are enabled, and a few headline
params (rate, lifetime, maxParticles, texture of the renderer material).
Writes catalog/particles.json.
Run:  py tools/inventory_particles.py [--envs] [--chars N]"""
import collections
import json
import os
import sys

TOOLS = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(TOOLS)
sys.path.insert(0, TOOLS)

import UnityPy
from export_animation import latest_bundle

MODULES = [
    "InitialModule", "shapeModule", "EmissionModule", "sizeModule",
    "rotationModule", "colorModule", "UVModule", "velocityModule",
    "ForceModule", "clampVelocityModule", "CollisionModule",
    "SubModule", "trailModule",
]


def go_name(env, gid):
    for o in env.objects:
        if o.type.name == "GameObject" and o.path_id == gid:
            return o.read_typetree().get("m_Name", "?")
    return "?"


def mat_tex(env, ps):
    """First renderer material's main texture name."""
    try:
        ren = ps.read_typetree().get("Renderer", {})
        for m in ren.get("m_Materials", []):
            pid = m.get("m_PathID", 0)
            if not pid:
                continue
            for o in env.objects:
                if o.type.name == "Material" and o.path_id == pid:
                    t = o.read_typetree()
                    tp = t.get("m_TexEnvs", {}).get("_MainTex", {})
                    tid = tp.get("m_Texture", {}).get("m_PathID", 0)
                    nm = "?"
                    for o2 in env.objects:
                        if o2.type.name == "Texture2D" and o2.path_id == tid:
                            nm = o2.read_typetree().get("m_Name", "?")
                    return f"{t.get('m_Name','?')}:{nm}"
    except Exception:
        pass
    return None


def scan(bundle_name):
    path = latest_bundle(bundle_name)
    if not path:
        return None
    env = UnityPy.load(path)
    out = []
    for o in env.objects:
        if o.type.name != "ParticleSystem":
            continue
        t = o.read_typetree()
        init = t.get("InitialModule", {})
        emiss = t.get("EmissionModule", {})
        rate = None
        try:
            rate = emiss["rateOverTime"]["scalar"]
        except Exception:
            pass
        rec = {
            "name": go_name(env, t.get("m_GameObject", {}).get("m_PathID", 0)),
            "enabled": [m for m in MODULES
                        if isinstance(t.get(m), dict) and t[m].get("enabled")],
            "maxParticles": init.get("maxNumParticles"),
            "lifetime": round(init.get("startLifetime", {}).get("scalar") or 0, 2),
            "speed": round(init.get("startSpeed", {}).get("scalar") or 0, 2),
            "size": round(init.get("startSize", {}).get("scalar") or 0, 3),
            "rate": round(rate, 2) if rate is not None else None,
            "gravity": round(t.get("gravityModifier", {}).get("scalar") or 0, 2),
            "material": mat_tex(env, o),
        }
        out.append(rec)
    return out or None


def main():
    args = sys.argv[1:]
    do_envs = "--envs" in args or not args
    n_chars = 0
    if "--chars" in args:
        n_chars = int(args[args.index("--chars") + 1])
    out = {}
    if do_envs:
        envs = sorted(os.listdir(os.path.join(ROOT, "extracted_env")))
        for i, e in enumerate(envs):
            r = scan(e)
            if r:
                out[e] = r
                names = [p["name"] for p in r]
                print(f"[{i+1}/{len(envs)}] {e}: {len(r)} PS {names[:6]}", flush=True)
    if n_chars:
        roster = json.load(open(os.path.join(ROOT, "game_roster.json"), encoding="utf-8"))
        folders = sorted({v["char"] for v in roster.values()})[:n_chars]
        for i, c in enumerate(folders):
            r = scan(c)
            if r:
                out[c] = r
                print(f"char {c}: {len(r)} PS {[p['name'] for p in r][:6]}", flush=True)
    dst = os.path.join(ROOT, "catalog", "particles.json")
    json.dump(out, dst and open(dst, "w", encoding="utf-8"),
              ensure_ascii=False, indent=1)
    n = sum(len(v) for v in out.values())
    print(f"\n{len(out)} bundles, {n} particle systems -> catalog/particles.json")


if __name__ == "__main__":
    main()
