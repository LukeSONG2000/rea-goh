# -*- coding: utf-8 -*-
"""Strip COLOR_0 from every prim of every exported GLB.

The game's character shaders don't multiply baked vertex-color RGB into
albedo (they use it for runtime tint masks), but glTF viewers do — 149
characters rendered near-black because of it.  glTF allows orphan
accessors, so we only edit the JSON chunk; the BIN chunk is copied
verbatim and the vertex-color data stays addressable if ever needed.
"""
import json
import struct
from pathlib import Path

ROOT = Path(__file__).parent / "extracted"
stripped = none = bad = 0
for glb in sorted(ROOT.glob("char_*/*.glb")):
    d = glb.read_bytes()
    if d[:4] != b"glTF":
        bad += 1
        continue
    jlen = struct.unpack("<I", d[12:16])[0]
    js = json.loads(d[20:20 + jlen])
    if "meshes" not in js:
        continue
    off = 20 + jlen
    blen, btag = struct.unpack("<I4s", d[off:off + 8])
    assert btag == b"BIN\0", glb
    changed = False
    for m in js["meshes"]:
        for p in m["primitives"]:
            if p["attributes"].pop("COLOR_0", None) is not None:
                changed = True
    if not changed:
        none += 1
        continue
    new_js = json.dumps(js, separators=(",", ":")).encode("utf-8")
    while len(new_js) % 4:
        new_js += b" "
    bin_data = d[off + 8: off + 8 + blen]
    total = 12 + 8 + len(new_js) + 8 + len(bin_data)
    out = (struct.pack("<4sII", b"glTF", 2, total)
           + struct.pack("<I4s", len(new_js), b"JSON") + new_js
           + struct.pack("<I4s", blen, b"BIN\0") + bin_data)
    glb.write_bytes(out)
    stripped += 1
print(f"stripped COLOR_0: {stripped}, already clean: {none}, bad: {bad}")
