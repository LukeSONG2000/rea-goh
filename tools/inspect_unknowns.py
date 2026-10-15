# -*- coding: utf-8 -*-
"""Inspect the unknown generic bindings: which clip, which attribute, and
whether their curves are constant (harmless authoring leftovers) or moving
(real missing animation).  Run:  py tools/inspect_unknowns.py [char]"""
import collections
import json
import os
import sys
import zlib

TOOLS = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(TOOLS)
sys.path.insert(0, TOOLS)

import UnityPy

from export_animation import latest_bundle, load_glb
from export_animation_v2 import SHARED, clip_by_pathid, crc_paths, shared_clip_names, shared_env, u64
from muscle_clip import binding_dimensions, curve_values

ATTR = {1: "translation", 2: "rotation", 3: "scale", 4: "euler"}


def curve_span(clip, c):
    """All values of one scalar curve index; None if unrepresentable."""
    try:
        cv = curve_values(clip, c)
    except Exception:
        return None
    if cv["kind"] == "constant":
        return [cv["value"]]
    if cv["kind"] == "dense":
        return cv["values"]
    return [k["value"] for k in cv["keys"]]


def main():
    char = sys.argv[1] if len(sys.argv) > 1 else "char_jediconsular_pre"
    shared_names = shared_clip_names()
    env = UnityPy.load(latest_bundle(char))
    js, _ = load_glb(os.path.join(ROOT, "extracted", char, f"{char}.glb"))
    cmap = crc_paths(js)
    bindings = json.load(open(os.path.join(ROOT, "catalog", "char_anim_bindings.json"),
                              encoding="utf-8"))[char]

    def clip_label(env_, pid):
        for o in env_.objects:
            if o.type.name == "AnimationClip" and u64(o.path_id) == u64(pid):
                return o.read_typetree().get("m_Name", "?")
        return "?"

    seen = set()
    stats = collections.Counter()
    examples = []
    for k in bindings:
        for r in k["clips"]:
            key = u64(r["path_id"])
            if key in seen:
                continue
            seen.add(key)
            src = str(r["source"])
            if src.startswith("ext") and key not in shared_names:
                continue
            cenv = shared_env() if (src == SHARED or src.startswith("ext")) else env
            try:
                clip = clip_by_pathid(cenv, key)
            except SystemExit:
                continue
            cname = clip.get("m_Name") or clip_label(cenv, key)
            acc = 0
            for b, d in zip(clip["bindings"]["generic_bindings"],
                            binding_dimensions(clip)):
                crc = b["path"] & 0xFFFFFFFF
                curves = list(range(acc, acc + d))
                acc += d
                if b["type_id"] != 4 or crc in cmap:
                    continue
                attr = ATTR.get(b["attribute"], f"attr{b['attribute']}")
                stats[f"unknown.{attr}"] += 1
                vals = []
                for c in curves:
                    vs = curve_span(clip, c)
                    if not vs:
                        vals.append(None)
                    elif len({round(v, 6) for v in vs}) <= 1:
                        vals.append(vs[0])
                    else:
                        vals.append("MOVING")
                if any(v == "MOVING" for v in vals):
                    stats["unknown.MOVING"] += 1
                    if len(examples) < 25:
                        examples.append((cname, src[:6], attr, f"{crc:08x}", vals))
                else:
                    stats["unknown.CONST"] += 1

    print(f"==== {char} unknown binding stats ====")
    for k, v in sorted(stats.items()):
        print(f"  {k}: {v}")
    print("\n-- moving examples (clip, src, attr, crc, sample values) --")
    for e in examples:
        print(" ", e)


if __name__ == "__main__":
    main()
