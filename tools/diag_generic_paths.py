# -*- coding: utf-8 -*-
"""Diagnose unmatched generic-binding CRC paths (weapon-follow bug, route 2).

For each generic-route char, resolve every generic transform binding's CRC
three ways:
  glb_ok     -- CRC matches a subtree path inside the exported GLB
                (exporter already handles these)
  bundle_only-- path exists in the char prefab's Transform hierarchy but no
                GLB node -> the node was dropped by the static export, or is
                reachable by leaf name
  unknown    -- not even in the prefab hierarchy (foreign prefix)

Builds ground truth from the bundle's own Transform/GameObject components,
so no guessing: every ancestor-relative path of every transform gets CRCed.

Run:  py tools/diag_generic_paths.py [limit]
"""
import collections
import json
import os
import sys
import zlib

TOOLS = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(TOOLS)
sys.path.insert(0, TOOLS)

import UnityPy
from muscle_clip import parse_animation_clip  # noqa: F401  (clip parser)

from export_animation import (
    HUMAN_BONE_TO_RIG_LEAF, RigMismatch, latest_bundle, load_avatar,
    build_joint_table, load_glb,
)
from export_animation_v2 import (
    SHARED, clip_by_pathid, crc_paths, shared_clip_names, shared_env, u64,
)

_U64 = 0xFFFFFFFFFFFFFFFF


def bundle_paths(env):
    """{crc32: representative relative path} over the prefab hierarchy."""
    trs, gos = {}, {}
    for o in env.objects:
        tn = o.type.name
        if tn == "Transform":
            trs[u64(o.path_id)] = o.read_typetree()
        elif tn == "GameObject":
            gos[u64(o.path_id)] = o.read_typetree().get("m_Name", "?")

    full = {}

    def build(pid, depth=0):
        if pid in full:
            return full[pid]
        if depth > 64:
            return "?"
        tr = trs[pid]
        name = gos.get(u64(tr.get("m_GameObject", {}).get("m_PathID", 0)), "?")
        f = tr.get("m_Father", {})
        fpid = u64(f.get("m_PathID", 0))
        if f.get("m_FileID", 0) != 0 or fpid == 0 or fpid not in trs:
            p = name
        else:
            p = build(fpid, depth + 1) + "/" + name
        full[pid] = p
        return p

    for pid in trs:
        build(pid)

    crc_map = {}
    for p in full.values():
        comps = p.split("/")
        for i in range(len(comps)):
            rel = "/".join(comps[i:])
            crc_map.setdefault(zlib.crc32(rel.encode()) & 0xFFFFFFFF, rel)
    return crc_map


def main():
    limit = int(sys.argv[1]) if len(sys.argv) > 1 else 150
    roster = json.load(open(os.path.join(ROOT, "game_roster.json"), encoding="utf-8"))
    folders = sorted({v["char"] for v in roster.values()})
    bpath = os.path.join(ROOT, "catalog", "char_anim_bindings.json")
    bindings = json.load(open(bpath, encoding="utf-8"))
    shared_names = shared_clip_names()

    agg = collections.Counter()
    path_freq = collections.Counter()      # unmatched bundle path -> chars
    miss_name_freq = collections.Counter()  # bundle-only leaf name -> chars
    sample = folders[:limit]
    for i, char in enumerate(sample):
        entry = bindings.get(char)
        if not entry:
            agg["no-bindings"] += 1
            continue
        glb_in = os.path.join(ROOT, "extracted", char, f"{char}.glb")
        if not os.path.exists(glb_in):
            agg["no-glb"] += 1
            continue
        try:
            js, _ = load_glb(glb_in)
        except Exception:
            agg["glb-load-fail"] += 1
            continue
        cmap = crc_paths(js)
        glb_names = {n.get("name", "") for n in js["nodes"]}
        try:
            av = load_avatar(UnityPy.load(latest_bundle(char)))
            build_joint_table(av)
            route = "humanoid"
        except SystemExit:
            route = "generic"
        except RigMismatch:
            route = "generic"
        except Exception:
            route = "generic-error"
        env = UnityPy.load(latest_bundle(char))
        bmap = bundle_paths(env)

        seen = set()
        cnt = collections.Counter()
        for k in entry:
            for r in k["clips"]:
                key = u64(r["path_id"])
                if key in seen:
                    continue
                seen.add(key)
                src = str(r["source"])
                if src.startswith("ext") and key not in shared_names:
                    continue          # unresolvable external, v2 drops these too
                try:
                    clip = clip_by_pathid(
                        shared_env() if (src == SHARED or src.startswith("ext"))
                        else env, key)
                except SystemExit:
                    cnt["clip-missing"] += 1
                    continue
                for b in clip["bindings"]["generic_bindings"]:
                    if b["type_id"] != 4:
                        continue
                    crc = b["path"] & 0xFFFFFFFF
                    if crc in cmap:
                        cnt["glb_ok"] += 1
                    elif crc in bmap:
                        p = bmap[crc]
                        cnt["bundle_only"] += 1
                        leaf = p.split("/")[-1]
                        if leaf in glb_names:
                            cnt["bundle_only_recoverable_by_name"] += 1
                        else:
                            cnt["bundle_only_missing_node"] += 1
                            miss_name_freq[leaf] += 1
                        path_freq[p] += 1
                    else:
                        cnt["unknown"] += 1
                        path_freq["<unknown crc %08x>" % crc] += 1
        agg[route] += 1
        for k, v in cnt.items():
            agg[f"{route}.{k}"] += v
        if cnt.get("bundle_only") or cnt.get("unknown"):
            print(f"[{i+1}] {char} ({route}): "
                  f"ok={cnt['glb_ok']} bundle_only={cnt['bundle_only']} "
                  f"(by_name={cnt['bundle_only_recoverable_by_name']} "
                  f"missing={cnt['bundle_only_missing_node']}) "
                  f"unknown={cnt['unknown']}", flush=True)
        if (i + 1) % 25 == 0:
            print(f"  .. {i+1}/{len(sample)}", flush=True)

    print("\n==== diag summary (first %d roster folders) ====" % len(sample))
    for k, v in sorted(agg.items()):
        print(f"  {k}: {v}")
    print("\n-- top unmatched prefab paths --")
    for p, c in path_freq.most_common(15):
        print(f"  {c:4d}  {p}")
    print("\n-- top leaf names missing from GLB --")
    for n, c in miss_name_freq.most_common(20):
        print(f"  {c:4d}  {n}")


if __name__ == "__main__":
    main()
