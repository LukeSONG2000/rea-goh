# -*- coding: utf-8 -*-
"""Recompute GPU skinning for every skinned mesh in a GLB and report
vertices that end up outside a sane volume (the 'stretching trail' bug
put vertices at Y ~ -30).  Usage:
    py verify_skin.py extracted/char_jynerso_pre/char_jynerso_pre.glb
    py verify_skin.py            (batch over extracted/*/*.glb)
"""
import json
import struct
import sys
import numpy as np
from pathlib import Path

COMP_T = {5126: "f4", 5123: "u2", 5125: "u4", 5121: "u1"}
NCOMP = {"SCALAR": 1, "VEC2": 2, "VEC3": 3, "VEC4": 4, "MAT4": 16}


def load_glb(path):
    d = Path(path).read_bytes()
    jlen = struct.unpack("<I", d[12:16])[0]
    return json.loads(d[20:20 + jlen]), d, 20 + jlen + 8


def accessor(g, d, bin_off, aid):
    a = g["accessors"][aid]
    bv = g["bufferViews"][a["bufferView"]]
    off = bin_off + bv.get("byteOffset", 0) + a.get("byteOffset", 0)
    n = NCOMP[a["type"]]
    arr = np.frombuffer(d, dtype=COMP_T[a["componentType"]],
                        count=a["count"] * n, offset=off)
    return arr.reshape(a["count"], n) if n > 1 else arr


def node_matrix(n):
    M = np.eye(4)
    if "matrix" in n:
        m = n["matrix"]  # column-major
        return np.array([[m[0], m[4], m[8], m[12]], [m[1], m[5], m[9], m[13]],
                         [m[2], m[6], m[10], m[14]], [m[3], m[7], m[11], m[15]]])
    t, r, s = (n.get("translation", [0, 0, 0]), n.get("rotation", [0, 0, 0, 1]),
               n.get("scale", [1, 1, 1]))
    x, y, z, w = r
    R = np.array([[1 - 2 * (y * y + z * z), 2 * (x * y - z * w), 2 * (x * z + y * w)],
                  [2 * (x * y + z * w), 1 - 2 * (x * x + z * z), 2 * (y * z - x * w)],
                  [2 * (x * z - y * w), 2 * (y * z + x * w), 1 - 2 * (x * x + y * y)]])
    M[:3, :3] = R * np.array(s)
    M[:3, 3] = t
    return M


def check_glb(path):
    g, d, bin_off = load_glb(path)
    G = {}

    def rec(nid, parent):
        M = node_matrix(g["nodes"][nid])
        G[nid] = (parent @ M) if parent is not None else M
        for c in g["nodes"][nid].get("children", []):
            rec(c, G[nid])

    for r in g["scenes"][g.get("scene", 0)]["nodes"]:
        rec(r, None)

    worst = None          # min Y seen after skinning
    bad_below = 0         # verts below Y=-0.5
    bad_wsum = 0          # verts whose weights don't sum to ~1
    skinned = 0
    for n in g["nodes"]:
        if "mesh" not in n or "skin" not in n:
            continue
        skinned += 1
        prim = g["meshes"][n["mesh"]]["primitives"][0]
        if "JOINTS_0" not in prim["attributes"]:
            continue  # exported static despite a skin reference
        P = accessor(g, d, bin_off, prim["attributes"]["POSITION"]).astype(np.float64)
        J = accessor(g, d, bin_off, prim["attributes"]["JOINTS_0"]).astype(int)
        W = accessor(g, d, bin_off, prim["attributes"]["WEIGHTS_0"]).astype(np.float64)
        skin = g["skins"][n["skin"]]
        if J.max() >= len(skin["joints"]):
            # joint index outside the palette — would crash three.js too
            bad_below += len(J)
            continue
        Jm = np.stack([G[j] for j in skin["joints"]])
        ibm_acc = skin.get("inverseBindMatrices")
        if ibm_acc is not None:
            # accessor yields (count,16) -> (count,4,4) read row-major; GLTF
            # stores matrices column-major, so the true matrix is the transpose
            raw = accessor(g, d, bin_off, ibm_acc).astype(np.float64).reshape(-1, 4, 4)
            IBM = np.ascontiguousarray(raw.transpose(0, 2, 1))
        else:
            IBM = np.stack([np.linalg.inv(Jm[k]) for k in range(len(skin["joints"]))])
        Palette = Jm @ IBM                                     # (K,4,4)
        # each vertex carries 4 (joint,weight) pairs into the palette
        Pv = np.einsum("vk,vkij->vij", W, Palette[J])          # (V,4,4)
        hom = np.hstack([P, np.ones((len(P), 1))])
        sk = np.einsum("vij,vj->vi", Pv, hom)
        y = sk[:, 1]
        bad_below += int((y < -0.5).sum())
        bad_wsum += int((np.abs(W.sum(axis=1) - 1.0) > 0.02).sum())
        ymin = float(y.min())
        if worst is None or ymin < worst[0]:
            worst = (ymin, n.get("name", "?"))
    return skinned, bad_below, bad_wsum, worst


def main():
    if len(sys.argv) > 1:
        files = [Path(sys.argv[1])]
    else:
        files = sorted(Path("extracted").glob("char_*/*.glb"))
    total_bad = total_w = nerr = 0
    flagged = []
    for f in files:
        try:
            sk, bb, bw, worst = check_glb(f)
        except Exception as e:
            nerr += 1
            print(f"  [ERR] {f}: {type(e).__name__} {e}")
            continue
        total_bad += bb
        total_w += bw
        if bb or bw:
            flagged.append((f.parent.name, bb, bw, worst))
    print(f"\nchecked {len(files)} GLBs ({nerr} errors)")
    print(f"skinned vertices below Y=-0.5 : {total_bad}")
    print(f"vertices with weight sum != 1 : {total_w}")
    if flagged:
        print("\nflagged characters (name, verts<-0.5, badWeightSum, worst mesh Y):")
        for name, bb, bw, worst in flagged:
            print(f"  {name}: {bb} {bw} {worst}")
    if nerr:
        print("NOT VERIFIED — fix errors above first")
    elif not flagged:
        print("ALL CLEAN")


if __name__ == "__main__":
    main()
