# -*- coding: utf-8 -*-
"""Decode recurring unknown CRCs against the union of ALL roster GLB subtree
paths (fast: O(paths) lookups + O(kp x paths) concatenations with the
crc32-combine trick).  Run:  py tools/decode_global.py [char]"""
import json
import os
import struct
import sys
import zlib

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

CRC = lambda s: zlib.crc32(s.encode()) & 0xFFFFFFFF

TARGETS = {
    0x05E41BF4: "ubiquitous-1", 0xFDE6A988: "ubiquitous-2", 0xBA2B8BAD: "ubiquitous-3",
    0xED869DA1: "sleeve-R", 0xB4C44BDF: "sleeve-L",
    0xED469027: "jc-1", 0xB8B973F8: "jc-2", 0x9F1C3196: "jc-3", 0x08898C15: "jc-4",
    0xCC25F742: "jc-5", 0x6AD759BA: "jc-6", 0xB5C3F779: "jc-7", 0x1A7DD93F: "jc-8",
    0xC86183C5: "jc-9", 0xACD5C8D3: "jc-10", 0x234F7277: "jc-11",
}


def all_glb_paths():
    out = set()
    ex = os.path.join(ROOT, "extracted")
    for d in sorted(os.listdir(ex)):
        gp = os.path.join(ex, d, f"{d}.glb")
        if not os.path.exists(gp):
            continue
        try:
            raw = open(gp, "rb").read()
            clen, _ = struct.unpack("<II", raw[12:20])
            j = json.loads(raw[20:20 + clen].decode("utf-8"))
        except Exception:
            continue
        for base in range(len(j["nodes"])):
            stack = [(base, [])]
            while stack:
                i, pre = stack.pop()
                nm = j["nodes"][i].get("name", "")
                p = "/".join(pre + [nm])
                out.add(p)
                stack.extend((ch, pre + [nm]) for ch in j["nodes"][i].get("children", []))
    return out


def main():
    char = sys.argv[1] if len(sys.argv) > 1 else "char_jediconsular_pre"
    paths = all_glb_paths()
    print(f"global subtree paths: {len(paths)}")

    # direct membership
    for t, label in TARGETS.items():
        for p in paths:
            if CRC(p) == t:
                print(f"  {label} {t:08x} -> direct: {p}")
                break

    # concat: <char's own kp> + "/" + <any global path>
    raw = open(os.path.join(ROOT, "extracted", char, f"{char}.glb"), "rb").read()
    clen, _ = struct.unpack("<II", raw[12:20])
    j = json.loads(raw[20:20 + clen].decode("utf-8"))
    own = set()
    for base in range(len(j["nodes"])):
        stack = [(base, [])]
        while stack:
            i, pre = stack.pop()
            nm = j["nodes"][i].get("name", "")
            p = "/".join(pre + [nm])
            own.add(p)
            stack.extend((ch, pre + [nm]) for ch in j["nodes"][i].get("children", []))

    remaining = {t: l for t, l in TARGETS.items()}
    found = {}
    for kp in own:
        base = zlib.crc32((kp + "/").encode()) & 0xFFFFFFFF
        for p in paths:
            v = zlib.crc32(p.encode(), base) & 0xFFFFFFFF
            if v in remaining:
                found[v] = f"{kp}/{p}"
                del remaining[v]
    for v, p in sorted(found.items()):
        print(f"  {TARGETS[v]} {v:08x} -> concat: {p}")
    for t, l in sorted(remaining.items()):
        print(f"  {l} {t:08x} -> STILL UNKNOWN")


if __name__ == "__main__":
    main()
