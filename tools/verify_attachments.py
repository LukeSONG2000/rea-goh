# -*- coding: utf-8 -*-
"""Verify weapon/prop attachment fidelity in exported character GLBs
(BUGS.md P3/P6/+1 acceptance gate).

For every character given (extracted/<id>/<id>.anim.glb preferred, plain
.glb fallback) it computes, straight from the GLB:

  * every prop node = mesh-bearing node whose ancestor chain passes a
    hand/weapon joint (wrst|wpn|weapon|hand ...jnt, or a *_HAND socket --
    the game's runtime attachment points);
  * the WORLD position of the prop node and of that joint at t=0 of an
    animation (default: the first one, i.e. the representative idle),
    channels sampled at exactly t=0 with LINEAR lookup;
  * PASS when the two sit within --tol (default 0.5 units): a prop the
    game keeps in the hand must not drift away from its joint;
  * info-only upright probe for the P3 tilt symptom: world Y of
    pelvis/head/balls at t=0 (a resting character has feet at y~0 and head
    above pelvis).

Usage:
    py tools/verify_attachments.py char_a [char_b ...] [--anim NAME]
                                    [--tol 0.5] [--glb PATH]

Exit status 1 if any prop fails its joint-distance check (or a listed
character has no prop joints at all -- a config error, not a pass).
"""
import argparse
import bisect
import json
import os
import re
import struct
import sys

import numpy as np

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

HAND_JOINT = re.compile(r"(wrst|wpn|wrist|weapon|hand).*jnt|jnt.*(wrst|wpn|wrist|weapon)|_hand$", re.I)


def load_glb(path):
    data = open(path, "rb").read()
    magic, _ver, _total = struct.unpack_from("<4sII", data, 0)
    if magic != b"glTF":
        raise SystemExit(f"not a GLB: {path}")
    jlen, jtype = struct.unpack_from("<II", data, 12)
    if jtype != 0x4E4F534A:
        raise SystemExit(f"JSON chunk missing: {path}")
    js = json.loads(data[20:20 + jlen])
    off = 20 + jlen
    blen, btype = struct.unpack_from("<II", data, off)
    if btype != 0x004E4942:
        raise SystemExit(f"BIN chunk missing: {path}")
    return js, data[off + 8:off + 8 + blen]


def read_accessor(js, buf, ai):
    a = js["accessors"][ai]
    bv = js["bufferViews"][a["bufferView"]]
    ncomp = {"SCALAR": 1, "VEC2": 2, "VEC3": 3, "VEC4": 4, "MAT4": 16}[a["type"]]
    off = bv.get("byteOffset", 0)
    return np.frombuffer(buf, np.float32, a["count"] * ncomp, off)\
             .reshape(a["count"], ncomp)


def quat_mat(q):
    x, y, z, w = q
    return np.array([
        [1 - 2 * (y * y + z * z), 2 * (x * y - z * w), 2 * (x * z + y * w)],
        [2 * (x * y + z * w), 1 - 2 * (x * x + z * z), 2 * (y * z - x * w)],
        [2 * (x * z - y * w), 2 * (y * z + x * w), 1 - 2 * (x * x + y * y)]])


class Glb:
    def __init__(self, path):
        self.js, self.buf = load_glb(path)
        js = self.js
        self.parent = {}
        for i, n in enumerate(js["nodes"]):
            for c in n.get("children", []):
                self.parent[c] = i
        self.names = {}
        for i, n in enumerate(js["nodes"]):
            self.names.setdefault(n.get("name", ""), i)

    def chain(self, i):
        out = [i]
        while out[-1] in self.parent:
            out.append(self.parent[out[-1]])
        return out

    def find_hand_joint(self, i):
        """Nearest ancestor-or-self that is a hand/weapon joint."""
        for j in self.chain(i):
            if HAND_JOINT.search(self.js["nodes"][j].get("name", "")):
                return j
        return None

    def local_at(self, i, anim, t):
        nd = self.js["nodes"][i]
        tr = np.asarray(nd.get("translation", [0.0, 0.0, 0.0]), float)
        q = np.asarray(nd.get("rotation", [0.0, 0.0, 0.0, 1.0]), float)
        sc = np.asarray(nd.get("scale", [1.0, 1.0, 1.0]), float)
        if anim is not None:
            for ch in anim.get("channels", []):
                if ch["target"]["node"] != i:
                    continue
                s = anim["samplers"][ch["sampler"]]
                tin = read_accessor(self.js, self.buf, s["input"]).ravel()
                vout = read_accessor(self.js, self.buf, s["output"])
                k = min(bisect.bisect_right(tin, t) - 1, len(vout) - 1)
                v = vout[max(k, 0)]
                p = ch["target"]["path"]
                if p == "translation":
                    tr = v[:3].astype(float)
                elif p == "rotation":
                    q = v.astype(float)
                elif p == "scale":
                    sc = v[:3].astype(float)
        M = np.eye(4)
        M[:3, :3] = quat_mat(q) * sc
        M[:3, 3] = tr
        return M

    def world(self, i, anim=None, t=0.0):
        M = np.eye(4)
        for j in reversed(self.chain(i)):
            M = M @ self.local_at(j, anim, t)
        return M

    def props(self):
        """[(node idx, hand joint idx)] for every mesh under a hand joint."""
        out = []
        for i, n in enumerate(self.js["nodes"]):
            if "mesh" not in n:
                continue
            h = self.find_hand_joint(i)
            if h is not None and h != i:
                out.append((i, h))
        return out


def verify(char, anim_name=None, tol=0.5, glb_override=None, verbose=True):
    d = os.path.join(ROOT, "extracted", char)
    path = glb_override
    if not path:
        for cand in (f"{char}.anim.glb", f"{char}.glb"):
            p = os.path.join(d, cand)
            if os.path.exists(p):
                path = p
                break
    if not path or not os.path.exists(path):
        return None, "no GLB"
    g = Glb(path)
    anims = g.js.get("animations", [])
    if anim_name:
        anim = next((a for a in anims if a.get("name") == anim_name), None)
        if anim is None:
            return None, f"animation {anim_name!r} not found"
    else:
        anim = anims[0] if anims else None
    props = g.props()
    if not props:
        return [], f"no hand-attached props (path={os.path.basename(path)}, anim={anim.get('name') if anim else 'rest'})"
    results = []
    for pi, hi in props:
        pname = g.js["nodes"][pi].get("name", f"node{pi}")
        hname = g.js["nodes"][hi].get("name", f"node{hi}")
        Wp = g.world(pi, anim, 0.0)[:3, 3]
        Wh = g.world(hi, anim, 0.0)[:3, 3]
        dist = float(np.linalg.norm(Wp - Wh))
        results.append({
            "prop": pname, "joint": hname, "dist": dist,
            "pass": bool(dist <= tol),
            "prop_world": [round(float(v), 4) for v in Wp],
            "joint_world": [round(float(v), 4) for v in Wh],
            "anim": anim.get("name") if anim else "rest",
        })
    if verbose:
        an = anim.get("name") if anim else "rest"
        print(f"  {char}  [{os.path.basename(path)}]  anim={an!r}  tol={tol}")
        for r in results:
            flag = "PASS" if r["pass"] else "FAIL"
            print(f"    {flag}  {r['prop']:40s} -> {r['joint']:24s} "
                  f"d={r['dist']:.3f}  prop@{r['prop_world']} joint@{r['joint_world']}")
    return results, None


def upright_probe(char, verbose=True):
    """Info-only P3 tilt probe: world Y of pelvis/head/feet at t=0 of the
    first animation vs the rest pose."""
    d = os.path.join(ROOT, "extracted", char)
    path = None
    for cand in (f"{char}.anim.glb", f"{char}.glb"):
        p = os.path.join(d, cand)
        if os.path.exists(p):
            path = p
            break
    if not path:
        return
    g = Glb(path)
    anims = g.js.get("animations", [])
    anim = anims[0] if anims else None

    def y(name, a):
        i = g.names.get(name)
        if i is None:
            return None
        return float(g.world(i, a, 0.0)[1, 3])

    marks = {}
    for leaf in ("pelvicJNT", "headJNT", "Left_ballJNT", "Right_ballJNT",
                 "RootJNTBND", "HeadJNTBND", "leftBallJNTBND", "rightBallJNTBND"):
        v = y(leaf, anim)
        if v is not None:
            marks[leaf] = round(v, 3)
    if verbose and marks:
        an = anim.get("name") if anim else "rest"
        print(f"  upright probe [{an!r}]: " +
              "  ".join(f"{k}={v}" for k, v in marks.items()))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("chars", nargs="+")
    ap.add_argument("--anim", default=None, help="animation name (default: first)")
    ap.add_argument("--tol", type=float, default=0.5)
    ap.add_argument("--glb", default=None, help="explicit GLB path (single char)")
    args = ap.parse_args()

    failures = 0
    for char in args.chars:
        results, err = verify(char, args.anim, args.tol, args.glb)
        if err is not None:
            if results is None:
                print(f"  {char}: ERROR {err}")
                failures += 1
            else:
                print(f"  {char}: OK (no props to check) -- {err}")
            continue
        failures += sum(1 for r in results if not r["pass"])
        upright_probe(char)
    if failures:
        print(f"\nFAILED: {failures} prop attachment(s) exceed tol")
        sys.exit(1)
    print("\nall prop attachments within tolerance")


if __name__ == "__main__":
    main()
