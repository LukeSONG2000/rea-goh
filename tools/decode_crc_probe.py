# -*- coding: utf-8 -*-
"""Brute-force decode unknown generic-binding CRC32 paths.

Strategy: unknown paths are <prefix>/<known-suffix> where prefix is a runtime
wrapper (Animator parent, "(Clone)" container, variant rig root) absent from
both the GLB and the prefab's own hierarchy.  CRC each candidate
prefix/suffix combination and match against the unknown set.

Run:  py tools/decode_crc_probe.py
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

from export_animation import latest_bundle, load_glb
from export_animation_v2 import SHARED, clip_by_pathid, shared_clip_names, shared_env, u64

CRC = lambda s: zlib.crc32(s.encode()) & 0xFFFFFFFF


def glb_suffix_paths(char):
    js, _ = load_glb(os.path.join(ROOT, "extracted", char, f"{char}.glb"))
    out = {}
    for base in range(len(js["nodes"])):
        stack = [(base, [])]
        while stack:
            i, prefix = stack.pop()
            name = js["nodes"][i].get("name", "")
            path = "/".join(prefix + [name])
            out.setdefault(CRC(path), path)
            stack.extend((ch, prefix + [name])
                         for ch in js["nodes"][i].get("children", []))
    return out


def unknown_crcs(char, shared_names):
    env = UnityPy.load(latest_bundle(char))
    bindings = json.load(open(os.path.join(ROOT, "catalog", "char_anim_bindings.json"),
                              encoding="utf-8"))[char]
    unk = collections.Counter()
    seen = set()
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
            for b in clip["bindings"]["generic_bindings"]:
                if b["type_id"] != 4 and (b["path"] & 0xFFFFFFFF) == 0:
                    continue
                crc = b["path"] & 0xFFFFFFFF
                unk[crc] += 0    # placeholder to collect all crcs
    return unk, env


def decode(target, suffixes, prefixes):
    """Try <prefix>/<suffix> for every combo; return matched path or None."""
    for pre in prefixes:
        for suf in suffixes:
            if CRC(pre + "/" + suf) == target:
                return pre + "/" + suf
    return None


def main():
    shared_names = shared_clip_names()
    VADER = "char_vader_pre"
    JC = "char_jediconsular_pre"

    # global vocabulary: node names from a diverse handful of GLBs
    name_vocab = set()
    path_vocab = {}
    for c in (VADER, JC, "char_grievous_pre", "char_greatmothers_pre",
              "char_jaxxon_pre", "char_hk47_pre", "char_stormtrooper_pre"):
        p = os.path.join(ROOT, "extracted", c, f"{c}.glb")
        if os.path.exists(p):
            vp = glb_suffix_paths(c)
            path_vocab.update(vp)
            for s in vp.values():
                name_vocab.update(s.split("/"))

    wrapper_prefixes = set()
    for c in (VADER, JC, "char_grievous_pre"):
        for v in (c, c + "(Clone)", c.replace("_pre", "") + "(Clone)",
                  c.replace("_pre", ""), "Armature", "Root", "root", "rig",
                  "Rig", "model", "Model", "charroot", "Root/pelvicJNT"):
            wrapper_prefixes.add(v)

    all_suffixes = list(path_vocab.values()) + list(name_vocab)
    print(f"vocab: {len(all_suffixes)} suffixes x {len(wrapper_prefixes)} prefixes")

    # ---- ubiquitous trio ----
    trio = [0x05E41BF4, 0xFDE6A988, 0xBA2B8BAD]
    for t in trio:
        hit = decode(t, all_suffixes, wrapper_prefixes)
        print(f"ubiquitous {t:08x} -> {hit}")

    # ---- jediconsular unknowns: collect, then decode ----
    unk, env = unknown_crcs(JC, shared_names)
    # recompute properly: collect crcs that don't match the char's own GLB
    own = glb_suffix_paths(JC)
    unk = [c for c in unk if c not in own]
    print(f"\njediconsular: {len(unk)} unknown crcs; decoding …")
    hits = collections.Counter()
    misses = []
    for t in sorted(set(unk)):
        hit = decode(t, all_suffixes, wrapper_prefixes)
        if hit:
            hits[hit] += 1
        else:
            misses.append(t)
    print(f"decoded {sum(hits.values())}/{len(set(unk))}")
    for p, c in hits.most_common(20):
        print(f"  {c:4d}  {p}")
    print(f"still unknown: {len(misses)} e.g. {[f'{m:08x}' for m in misses[:8]]}")

    # what GameObject names exist in the bundle that the GLB lacks?
    env2 = UnityPy.load(latest_bundle(JC))
    go_names = set()
    for o in env2.objects:
        if o.type.name == "GameObject":
            go_names.add(o.read_typetree().get("m_Name", "?"))
    js, _ = load_glb(os.path.join(ROOT, "extracted", JC, f"{JC}.glb"))
    glb_names = {n.get("name", "") for n in js["nodes"]}
    print(f"\nbundle-only GO names ({len(go_names - glb_names)}):")
    for n in sorted(go_names - glb_names)[:40]:
        print(f"  {n}")


if __name__ == "__main__":
    main()
