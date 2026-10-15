# -*- coding: utf-8 -*-
"""Export per-character animations from the game's OWN AnimatorController
bindings (catalog/char_anim_bindings.json), replacing the blanket-retarget
batch.  Two clip routes:

  * muscle clips  -> humanoid retarget via MusclePoseEvaluator (needs Avatar)
  * generic clips -> plain transform curves matched onto GLB nodes by
                     CRC32 of the Scaler-relative path (zlib/IEEE; see
                     catalog/ANIMATIONS.md) -- works for ANY rig, humanoid
                     or not

Clips referenced from shared_animationcontainer are resolved by path_id;
clips referenced from the char's own bundle are loaded from there.

Run:  py tools/export_animation_v2.py char_x_pre [char_y_pre ...]
      py tools/export_animation_v2.py --all [--force]
"""
import glob
import json
import os
import struct
import sys
import zlib

TOOLS = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(TOOLS)
sys.path.insert(0, TOOLS)

import UnityPy
from muscle_clip import parse_animation_clip

from export_animation import (
    BUNDLE_ROOT, HUMAN_BONE_TO_RIG_LEAF, RigMismatch,
    MusclePoseEvaluator, build_animation, build_joint_table, desc_pelvis_q,
    latest_bundle, load_avatar, load_glb, save_glb, node_index_by_name,
    sample_curve, conv_q, conv_v3, q_norm,
)

SHARED = "shared_animationcontainer"
SHARED_ENV = None       # lazy, shared across all chars


def shared_env():
    global SHARED_ENV
    if SHARED_ENV is None:
        SHARED_ENV = UnityPy.load(latest_bundle(SHARED))
    return SHARED_ENV


def clip_by_pathid(env, path_id):
    # Deliberately uncached: a previous id(env)-keyed cache could recycle heap
    # addresses across sequential char envs and poison lookups, and parsing is
    # ~10ms per clip anyway.
    for obj in env.objects:
        if obj.type.name == "AnimationClip" and obj.path_id == path_id:
            return parse_animation_clip(obj.get_raw_data())
    raise SystemExit(f"clip path_id {path_id} not in bundle")


def shared_clip_names():
    env = shared_env()
    return {o.path_id: o.read_typetree()["m_Name"]
            for o in env.objects if o.type.name == "AnimationClip"}


def crc_paths(js):
    """{crc32: node_index} covering every subtree path in the GLB.

    Generic-clip binding paths are relative to whatever transform the
    character's Animator sits on -- that differs per prefab ('root/pelvis/...'
    for acklay, 'pelvicJNT/...' for 4lom) -- so enumerate paths from EVERY
    node as base instead of guessing one.  Hash matching is exact, so extra
    entries are harmless."""
    nodes = js["nodes"]
    out = {}
    for base in range(len(nodes)):
        stack = [(base, [])]
        while stack:
            i, prefix = stack.pop()
            name = nodes[i].get("name", "")
            path = "/".join(prefix + [name])
            out.setdefault(zlib.crc32(path.encode()) & 0xFFFFFFFF, i)
            stack.extend((ch, prefix + [name])
                         for ch in nodes[i].get("children", []))
    return out


def clip_priority(name):
    """Stand first: plain idles before hurt/sit variants, then everything else."""
    n = (name or "").lower()
    if "idle" in n and not any(w in n for w in ("hurt", "injured", "sit", "sleep")):
        return 0
    if "homescrn" in n:
        return 1
    return 2


def generic_channels(js, idx, cmap, clip, times, verbose):
    """Channels for plain transform-curve bindings via CRC path matching."""
    channels, skipped = [], 0
    acc = 0
    from muscle_clip import binding_dimensions
    for b, d in zip(clip["bindings"]["generic_bindings"], binding_dimensions(clip)):
        node = cmap.get(b["path"] & 0xFFFFFFFF)
        curves = list(range(acc, acc + d))
        acc += d
        if b["type_id"] != 4 or node is None:
            if b["type_id"] == 4:
                skipped += 1
            continue
        if b["attribute"] == 1:      # translation
            vals = [v for t in times for v in conv_v3([sample_curve(clip, c, t) for c in curves])]
            channels.append((node, "translation", vals))
        elif b["attribute"] == 2:    # rotation
            vals = [v for t in times
                    for v in conv_q(q_norm(tuple(sample_curve(clip, c, t) for c in curves)))]
            channels.append((node, "rotation", vals))
        elif b["attribute"] == 3:    # scale
            vals = [v for t in times for v in [sample_curve(clip, c, t) for c in curves]]
            channels.append((node, "scale", vals))
    if skipped and verbose:
        print(f"    .. {skipped} transform paths unmatched (no GLB node)")
    return channels


def export_char_v2(char, refs, shared_names, force=False, verbose=True,
                   states=None):
    """refs: [{'source','path_id'}] -- the char's own controller bindings.
    states: optional state table from catalog/char_anim_states.json --
    when given, animations are ORDERED by the game's state machine
    (default state first) and NAMED by state name (the real action name),
    replacing the clip-name heuristics entirely.
    Returns (status, n_clips, bytes)."""
    glb_in = os.path.join(ROOT, "extracted", char, f"{char}.glb")
    glb_out = os.path.join(ROOT, "extracted", char, f"{char}.anim.glb")
    if not os.path.exists(glb_in):
        return "no-glb", 0, 0

    # externals store internal CAB names, not the friendly bundle name -- any
    # external clip ref is a shared_animationcontainer clip iff its path_id
    # exists there (verified against the 178-clip catalog)
    refs = [dict(r) for r in refs
            if r["source"] == SHARED or not str(r["source"]).startswith("ext")
            or r["path_id"] in shared_names]
    for r in refs:
        if str(r["source"]).startswith("ext"):
            r["source"] = SHARED

    char_env = UnityPy.load(latest_bundle(char))

    # humanoid table (only if this rig really has one)
    joints = pelvis_q = tos = None
    try:
        av = load_avatar(char_env)
        joints = build_joint_table(av)
        _HAND_ALT = ("wrstjnt", "wpn_offsetjnt", "wpnjnt")
        for bone, leaf in HUMAN_BONE_TO_RIG_LEAF.items():
            got = joints.get(bone, {}).get("leaf")
            if got is None:
                continue
            if got == leaf or got.lower() == leaf.lower():
                continue
            if bone.endswith("Hand") and got.lower().endswith(_HAND_ALT):
                continue
            raise RigMismatch(f"{bone}: {got!r} != {leaf!r}")
        pelvis_q = desc_pelvis_q(av)
        tos = {int(h): p for h, p in av["m_TOS"]}
    except (SystemExit, RigMismatch):
        joints = None   # non-humanoid rig -> generic route only

    if states:
        # state-machine order: default state first, then the game's own
        # layer/state order.  Animation name = state name (layer prefix
        # stripped; kept on collision).  Blend-tree children get @threshold
        # suffixes so both idle variants survive.
        named = []
        used_names = set()
        order = 0
        indexed = list(enumerate(states))
        indexed.sort(key=lambda t: (not t[1]["default"], t[1]["layer"], t[0]))
        for _, s in indexed:
            label = s["name"].split(".")[-1].strip()
            motions = [m for m in s["motion"] if m.get("path_id")]
            multi = len([m for m in motions if "threshold" in m]) > 1
            for m in motions:
                if m["source"] == SHARED:
                    nm = shared_names.get(m["path_id"])
                elif str(m["source"]).startswith("ext"):
                    nm = shared_names.get(m["path_id"])   # may be None -> drop later
                else:
                    nm = m.get("name")
                if nm is None and str(m["source"]).startswith("ext"):
                    continue      # not a shared clip; unresolvable external
                if multi and "threshold" in m:
                    anim = f"{label}@{m['threshold']}"
                else:
                    anim = label
                base = anim
                k = 2
                while anim in used_names:      # cross-layer collision
                    anim = f"{base} ({k})"
                    k += 1
                used_names.add(anim)
                named.append((order, anim, {"source": m["source"],
                                            "path_id": m["path_id"],
                                            "name": nm}))
                order += 1
    else:
        # order clips: standing idle first (legacy heuristic)
        named = []
        for r in refs:
            if r["source"] == SHARED:
                name = shared_names.get(r["path_id"], f"shared_{r['path_id']&0xFFFF}")
            else:
                name = r.get("name") or f"local_{r['path_id']&0xFFFF}"
            named.append((clip_priority(name), name, r))
        named.sort(key=lambda x: (x[0], x[1]))

    js, binbuf = load_glb(glb_in)
    if "animations" in js:
        js["animations"] = []          # regenerate from scratch
    idx = node_index_by_name(js)
    cmap = crc_paths(js)

    n_ok = 0
    used = set()
    for _, name, r in named:
        if name in used:
            continue
        used.add(name)
        env = shared_env() if r["source"] == SHARED else char_env
        try:
            clip = clip_by_pathid(env, r["path_id"])
        except SystemExit:
            continue
        times = None
        channels = []
        mc = clip.get("muscle_clip") or {}
        # Humanoid clips occupy the HumanoidMuscleType slot table (~127 slots
        # used, Motion slots 0-6 always -1); generic clips keep the index
        # array all -1 and address transform curves directly.
        humanoid_clip = joints is not None and \
            sum(1 for v in (mc.get("index_array") or []) if v >= 0) > 50
        if humanoid_clip:
            ev = MusclePoseEvaluator(clip, joints, desc_pelvis_q=pelvis_q)
            rate = mc["clip"]["dense"]["sample_rate"] or 24.0
            n_frames = int(round(mc["stop_time"] * rate)) + 1
            times = [i / rate for i in range(n_frames)]
            poses = [ev.evaluate(t) for t in times]
            if "__root__" in poses[0] and "pelvicJNT" in idx:
                tv = [v for p in poses for v in p["__root__"]["translation"]]
                channels.append((idx["pelvicJNT"], "translation", tv))
            for bone, rec in sorted(joints.items()):
                leaf = rec["leaf"]
                if leaf in idx and leaf in poses[0]:
                    channels.append((idx[leaf], "rotation",
                                     [v for p in poses for v in p[leaf]["rotation"]]))
            # transform-layer curves (weapon nodes etc.) via avatar m_TOS
            from muscle_clip import binding_dimensions
            acc = 0
            for b, d in zip(clip["bindings"]["generic_bindings"], binding_dimensions(clip)):
                if b["type_id"] != 4:
                    continue
                path = tos.get(b["path"])
                leaf2 = path.split("/")[-1] if path else None
                curves = list(range(acc, acc + d))
                acc += d
                if leaf2 not in idx:
                    continue
                if b["attribute"] == 1:
                    channels.append((idx[leaf2], "translation",
                        [v for t in times for v in conv_v3([sample_curve(clip, c, t) for c in curves])]))
                elif b["attribute"] == 2:
                    channels.append((idx[leaf2], "rotation",
                        [v for t in times for v in conv_q(q_norm(tuple(sample_curve(clip, c, t) for c in curves)))]))
        elif mc:
            # generic-binding clip: transform curves via CRC path matching
            rate = mc["clip"]["dense"]["sample_rate"] or 24.0
            n_frames = int(round(mc["stop_time"] * rate)) + 1
            times = [i / rate for i in range(n_frames)]
            channels = generic_channels(js, idx, cmap, clip, times, verbose)
        else:
            continue
        if not channels:
            continue
        js.setdefault("animations", []).append(
            build_animation(js, binbuf, name, times, channels))
        n_ok += 1

    if n_ok == 0:
        if os.path.exists(glb_out):
            os.remove(glb_out)         # stale blanket-retarget file
            return "stale-removed", 0, 0
        return "no-anim", 0, 0
    if os.path.exists(glb_out) and not force:
        pass
    total = save_glb(glb_out, js, binbuf)
    return "ok", n_ok, total


def _generic_stop(clip):
    """Stop time for generic clips: last keyframe time across curves."""
    from muscle_clip import binding_dimensions
    acc = 0
    stop = 0.0
    for b, d in zip(clip["bindings"]["generic_bindings"], binding_dimensions(clip)):
        for c in range(acc, acc + d):
            curve = clip["curves"]["scalar_curves"][c] if c < len(clip["curves"]["scalar_curves"]) else None
        acc += d
    # fallback: use bindings-independent m_StopTime if muscle_clip parser kept it
    stop = clip.get("m_StopTime") or clip.get("stop_time") or 0.0
    return stop


def main():
    import argparse
    ap = argparse.ArgumentParser()
    ap.add_argument("chars", nargs="*", default=[])
    ap.add_argument("--all", action="store_true")
    ap.add_argument("--force", action="store_true")
    args = ap.parse_args()

    bpath = os.path.join(ROOT, "catalog", "char_anim_bindings.json")
    bindings = json.load(open(bpath, encoding="utf-8"))
    spath = os.path.join(ROOT, "catalog", "char_anim_states.json")
    states_map = {}
    if os.path.exists(spath):
        states_map = json.load(open(spath, encoding="utf-8"))
    shared_names = shared_clip_names()

    if args.all:
        chars = sorted(bindings.keys())
    else:
        chars = args.chars

    import collections
    stats = collections.Counter()
    for i, char in enumerate(chars):
        entry = bindings.get(char)
        st = states_map.get(char)
        if not entry and not st:
            stats["no-bindings"] += 1
            continue
        refs = [r for k in entry for r in k["clips"]] if entry else []
        try:
            status, n, total = export_char_v2(
                char, refs, shared_names, force=args.force,
                states=st["states"] if st else None)
            stats[status] += 1
            if status == "ok":
                print(f"[{i+1}/{len(chars)}] {char}: {n} clips, {total}B", flush=True)
        except Exception as e:
            stats[f"error:{type(e).__name__}"] += 1
            print(f"[{i+1}/{len(chars)}] {char}: ERROR {type(e).__name__}: {e}", flush=True)
    print("\n==== v2 summary ====")
    for k, v in sorted(stats.items()):
        print(f"  {k}: {v}")


if __name__ == "__main__":
    main()
