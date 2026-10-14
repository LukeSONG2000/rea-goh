# -*- coding: utf-8 -*-
"""Patch existing .glb files: add the missing buffers[0].byteLength declaration.
Only rewrites the JSON chunk; the BIN chunk is copied verbatim."""
import json
import struct
from pathlib import Path

ROOT = Path(__file__).parent / "extracted"
fixed = bad = 0
for glb in sorted(ROOT.glob("*/*.glb")):
    d = glb.read_bytes()
    if d[:4] != b"glTF":
        bad += 1
        continue
    jlen = struct.unpack("<I", d[12:16])[0]
    js = json.loads(d[20:20 + jlen])
    if "buffers" in js:
        continue
    off = 20 + jlen
    blen, btag = struct.unpack("<I4s", d[off:off + 8])
    assert btag == b"BIN\0", glb
    js["buffers"] = [{"byteLength": blen}]
    new_js = json.dumps(js, separators=(",", ":")).encode("utf-8")
    while len(new_js) % 4:
        new_js += b" "
    bin_data = d[off + 8: off + 8 + blen]
    total = 12 + 8 + len(new_js) + 8 + len(bin_data)
    out = (struct.pack("<4sII", b"glTF", 2, total)
           + struct.pack("<I4s", len(new_js), b"JSON") + new_js
           + struct.pack("<I4s", blen, b"BIN\0") + bin_data)
    glb.write_bytes(out)
    fixed += 1
print(f"fixed: {fixed}, skipped(already ok): not tracked, bad: {bad}")
