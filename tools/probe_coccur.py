# -*- coding: utf-8 -*-
"""For clips containing unknown binding CRCs, print the sibling paths that
DID match (resolved via the char GLB), and brute-force the unknowns as
<known-path>/<extra-name> using a global node-name vocabulary.
Run:  py tools/probe_coccur.py [char ...]"""
import collections
import json
import os
import struct
import sys
import zlib

TOOLS = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(TOOLS)
sys.path.insert(0, TOOLS)

import UnityPy

from export_animation import latest_bundle
from export_animation_v2 import SHARED, clip_by_pathid, shared_clip_names, shared_env, u64
from muscle_clip import binding_dimensions, curve_values

CRC = lambda s: zlib.crc32(s.encode()) & 0xFFFFFFFF
ATTR = {1: "T", 2: "R", 3: "S", 4: "E"}


def glb_paths(char):
    p = os.path.join(ROOT, "extracted", char, f"{char}.glb")
    raw = open(p, "rb").read()
    clen, _ = struct.unpack("<II", raw[12:20])
    j = json.loads(raw[20:20 + clen].decode("utf-8"))
    out = {}
    for base in range(len(j["nodes"])):
        stack = [(base, [])]
        while stack:
            i, pre = stack.pop()
            nm = j["nodes"][i].get("name", "")
            path = "/".join(pre + [nm])
            out.setdefault(CRC(path), path)
            stack.extend((ch, pre + [nm]) for ch in j["nodes"][i].get("children", []))
    names = {n.get("name", "") for n in j["nodes"]}
    return out, names


def span(clip, c):
    try:
        cv = curve_values(clip, c)
    except Exception:
        return []
    if cv["kind"] == "constant":
        return [cv["value"]]
    if cv["kind"] == "dense":
        return cv["values"]
    return [k["value"] for k in cv["keys"]]


def main():
    chars = sys.argv[1:] or ["char_fennecshand_pre", "char_jediconsular_pre"]
    shared_names = shared_clip_names()

    # global name vocabulary: all roster GLB node names
    vocab = set()
    ex_dir = os.path.join(ROOT, "extracted")
    for d in sorted(os.listdir(ex_dir)):
        gp = os.path.join(ex_dir, d, f"{d}.glb")
        if not os.path.exists(gp):
            continue
        try:
            raw = open(gp, "rb").read()
            clen, _ = struct.unpack("<II", raw[12:20])
            j = json.loads(raw[20:20 + clen].decode("utf-8"))
            vocab.update(n.get("name", "") for n in j["nodes"])
        except Exception:
            pass
    print(f"name vocab: {len(vocab)}")

    for char in chars:
        pmap, _ = glb_paths(char)
        env = UnityPy.load(latest_bundle(char))
        bindings = json.load(open(os.path.join(ROOT, "catalog", "char_anim_bindings.json"),
                                  encoding="utf-8"))[char]
        print(f"\n==== {char} ====")
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
                cname = clip.get("name", "?")
                acc = 0
                ok_paths, unk = [], []
                for b, d in zip(clip["bindings"]["generic_bindings"],
                                binding_dimensions(clip)):
                    crc = b["path"] & 0xFFFFFFFF
                    cs = list(range(acc, acc + d))
                    acc += d
                    if b["type_id"] != 4:
                        continue
                    if crc in pmap:
                        ok_paths.append((pmap[crc], ATTR.get(b["attribute"], "?")))
                    else:
                        moving = any(
                            len({round(v, 6) for v in span(clip, c)}) > 1
                            for c in cs if span(clip, c))
                        unk.append((crc, ATTR.get(b["attribute"], "?"), moving))
                if not unk:
                    continue
                print(f"-- {cname}  ok={len(ok_paths)} unk={len(unk)}")
                for pth, a in ok_paths[:12]:
                    print(f"     ok {a} {pth}")
                for crc, a, mv in unk[:12]:
                    # decode attempt: known_path + "/" + name
                    hit = None
                    for kp in {p for p, _ in ok_paths}:
                        for nm in vocab:
                            if CRC(kp + "/" + nm) == crc:
                                hit = f"{kp}/{nm}"
                                break
                        if hit:
                            break
                    if not hit:
                        for nm in vocab:
                            if CRC(nm) == crc:
                                hit = nm
                                break
                    print(f"     unk {a} {crc:08x} moving={mv} -> {hit or '?'}")


if __name__ == "__main__":
    main()
