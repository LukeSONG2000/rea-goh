# -*- coding: utf-8 -*-
"""Fleet-wide fidelity audit over ALL character GLBs.

Checks (per exported .anim.glb / .glb, no bundle round-trip):
  V1  no mesh named like particle/VFX debris (vfx|trail|beam|hot$|Quad_[ABC])
  V2  every material with an effect-ish name uses alphaMode BLEND
  V3  stale exports: GLB mtime older than the fidelity-fix reference stamp
Exit: nonzero iff any V1/V2 offender (V3 is informational and drives re-export).
"""
import glob
import json
import os
import struct
import sys
import time

REF_STAMP = time.time() - 6 * 3600     # exports older than 6h are pre-fix
VFX_NAME = ("vfx", "trail", "beam", "quad_a", "quad_b", "quad_c", "quad_hot")
FX_MAT = ("glow", "beam", "trail", "slash", "bolt", "vfx", "additive", "particle")


def glb_json(path):
    b = open(path, "rb").read()
    if len(b) < 20 or b[:4] != b"glTF":
        return None
    ln = struct.unpack("<I", b[12:16])[0]
    try:
        return json.loads(b[20:20 + ln])
    except Exception:
        return None


def main():
    bad_v1, bad_v2, stale = [], [], []
    files = sorted(glob.glob("extracted/char_*/[a-z]*.glb"))
    for p in files:
        g = glb_json(p)
        if not g:
            continue
        meshes = {m.get("name", "").lower() for m in g.get("meshes", [])}
        hit1 = sorted(n for n in meshes if any(k in n for k in VFX_NAME))
        if hit1:
            bad_v1.append((p, hit1[:3]))
        for m in g.get("materials", []):
            n = (m.get("name") or "").lower()
            if any(k in n for k in FX_MAT) and m.get("alphaMode") != "BLEND":
                bad_v2.append((p, m.get("name")))
        if os.path.getmtime(p) < REF_STAMP:
            stale.append(p)
    print(f"scanned {len(files)} GLBs")
    print(f"V1 particle-mesh offenders: {len(bad_v1)}")
    for p, n in bad_v1[:10]:
        print("   ", p, n)
    print(f"V2 opaque-fx-material offenders: {len(bad_v2)}")
    for p, n in bad_v2[:10]:
        print("   ", p, n)
    print(f"V3 stale (pre-fix): {len(stale)}")
    json.dump({"v1": [p for p, _ in bad_v1], "v2": [p for p, _ in bad_v2],
               "stale": stale}, open("catalog/audit_fidelity.json", "w"), indent=1)
    return 1 if (bad_v1 or bad_v2) else 0


if __name__ == "__main__":
    sys.exit(main())
