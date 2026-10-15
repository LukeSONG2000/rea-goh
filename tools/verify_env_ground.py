# -*- coding: utf-8 -*-
"""Verify ground/terrain presence + Y-extent in exported env GLBs (bug P2).

For each env GLB this walks the node tree, lifts every mesh primitive's
POSITION accessor into world space, splits ground-like meshes (Unity builtin
primitives synthesized by tools/export_env.py + ground/terrain/sand/floor
names) from props, and checks:

1. a ground mesh exists;
2. the ground's Y-extent underlies the props (ground top >= props' lowest
   point within tolerance, i.e. props sit ON the ground, not under it);
3. the ground XZ footprint covers the original staging area (from
   catalog/battle_staging.json env_index, Unity x mirrored to GLB space).

Usage: py tools/verify_env_ground.py <env_id> [...] (default: the 5 P2 envs)
"""
import json
import pathlib
import struct
import sys

import numpy as np

ROOT = pathlib.Path(__file__).resolve().parent.parent
ENV_ROOT = ROOT / "extracted_env"
STAGING = json.loads((ROOT / "catalog" / "battle_staging.json")
                     .read_text(encoding="utf-8"))

DEFAULT_ENVS = [
    "env_tatooine_moseisley_pre",
    "env_arv7_nikto_base_pre",
    "env_ata_chopperbase_pre",      # not in cache -> reported as missing
    "env_bar_conquest8_pre",
    "env_hoth_snow_plains_01_pre",
    "env_kash_beach_pre",
]

GROUND_NAME_PARTS = ("ground", "terrain", "sand", "floor")
BUILTIN_GROUND = ("builtin:quad", "builtin:plane", "builtin:cube")

COMP_TYPE = {5126: ("f4", 4), 5123: ("u2", 2), 5125: ("u4", 4), 5121: ("u1", 1)}
N_COMP = {"SCALAR": 1, "VEC2": 2, "VEC3": 3, "VEC4": 4, "MAT4": 16}


def read_glb(path):
    blob = path.read_bytes()
    magic, ver, total = struct.unpack_from("<4sII", blob, 0)
    assert magic == b"glTF" and ver == 2
    js_len, _ = struct.unpack_from("<II", blob, 12)
    js = json.loads(blob[20:20 + js_len])
    off = 20 + js_len
    bin_len, bin_type = struct.unpack_from("<II", blob, off)
    assert struct.pack("<4s", bin_type.to_bytes(4, "little")) == b"BIN\0"
    return js, blob[off + 8:off + 8 + bin_len]


def accessor(js, binbuf, idx):
    acc = js["accessors"][idx]
    bv = js["bufferViews"][acc["bufferView"]]
    off = bv.get("byteOffset", 0) + acc.get("byteOffset", 0)
    fmt, size = COMP_TYPE[acc["componentType"]]
    n = N_COMP[acc["type"]]
    count = acc["count"]
    arr = np.frombuffer(binbuf, dtype=fmt, count=count * n, offset=off)
    return arr.reshape(count, n).astype(np.float64)


def node_matrix(node):
    if "matrix" in node:
        # glTF column-major -> row-major 4x4
        return np.array(node["matrix"], dtype=np.float64).reshape(4, 4).T
    M = np.eye(4)
    t = node.get("translation", [0, 0, 0])
    r = node.get("rotation", [0, 0, 0, 1])
    s = node.get("scale", [1, 1, 1])
    x, y, z, w = r
    R = np.array([
        [1 - 2 * (y * y + z * z), 2 * (x * y - z * w), 2 * (x * z + y * w)],
        [2 * (x * y + z * w), 1 - 2 * (x * x + z * z), 2 * (y * z - x * w)],
        [2 * (x * z - y * w), 2 * (y * z + x * w), 1 - 2 * (x * x + y * y)]])
    M[:3, :3] = R @ np.diag(s)
    M[:3, 3] = t
    return M


def world_meshes(js, binbuf):
    """[(mesh_name, prim_idx, Nx3 world positions)] for every primitive."""
    out = []
    roots = js["scenes"][js.get("scene", 0)]["nodes"]

    def visit(ni, parent_M):
        node = js["nodes"][ni]
        M = parent_M @ node_matrix(node)
        if "mesh" in node:
            mesh = js["meshes"][node["mesh"]]
            for pi, prim in enumerate(mesh["primitives"]):
                pos = accessor(js, binbuf, prim["attributes"]["POSITION"])
                pts4 = np.hstack([pos, np.ones((len(pos), 1))])
                out.append((mesh.get("name", f"mesh{node['mesh']}"), pi,
                            (M @ pts4.T).T[:, :3]))
        for ch in node.get("children", []):
            visit(ch, M)
    for r in roots:
        visit(r, np.eye(4))
    return out


def aabb(pts):
    return pts.min(axis=0), pts.max(axis=0)


def staging_points(env):
    """Staging marker XZ points in GLB space (Unity x mirrored), or None."""
    layout = STAGING["env_index"].get(env) or \
        STAGING["_meta"]["defaults"]["pvp"]
    lay = STAGING["layouts"].get(layout)
    if not lay or lay.get("kind") != "battle":
        return None, layout
    pts = []
    for side in ("player", "enemy"):
        for p in lay.get(side, []):
            pts.append((-p[0], p[2]))    # mirror x, keep z
    return pts, layout


def verify(env, verbose=True):
    glb_path = ENV_ROOT / env / f"{env}.glb"
    if not glb_path.is_file():
        return False, f"{env}: no GLB at {glb_path}"
    js, binbuf = read_glb(glb_path)
    meshes = world_meshes(js, binbuf)

    ground, props = [], []
    for name, pi, pts in meshes:
        key = f"{name}#{pi}"
        if any(k in key for k in GROUND_NAME_PARTS) or \
           any(key.startswith(k) or f":{k}" in key for k in BUILTIN_GROUND):
            ground.append((key, pts))
        else:
            props.append((key, pts))

    if not ground:
        return False, f"{env}: NO ground mesh in GLB"

    gmin = np.min([p.min(axis=0) for _, p in ground], axis=0)
    gmax = np.max([p.max(axis=0) for _, p in ground], axis=0)
    pmin = np.min([p.min(axis=0) for _, p in props], axis=0)
    pmax = np.max([p.max(axis=0) for _, p in props], axis=0)

    checks = []
    # props sit on/above ground top (tolerance 0.15 for terrain blending)
    checks.append(("ground_under_props", gmax[1] >= pmin[1] - 0.15,
                   f"ground_top={gmax[1]:.2f} props_min_y={pmin[1]:.2f}"))
    # ground reaches y ~ 0 (unit plane)
    checks.append(("ground_at_y0", gmin[1] <= 0.05,
                   f"ground_min_y={gmin[1]:.2f}"))
    # footprint covers staging area
    spts, layout = staging_points(env)
    if spts:
        sx = np.array([p[0] for p in spts])
        sz = np.array([p[1] for p in spts])
        covered = (sx >= gmin[0]).all() and (sx <= gmax[0]).all() and \
                  (sz >= gmin[2]).all() and (sz <= gmax[2]).all()
        checks.append(("ground_covers_staging", bool(covered),
                       f"staging x[{sx.min():.1f},{sx.max():.1f}] "
                       f"z[{sz.min():.1f},{sz.max():.1f}] vs ground "
                       f"x[{gmin[0]:.1f},{gmax[0]:.1f}] "
                       f"z[{gmin[2]:.1f},{gmax[2]:.1f}] layout={layout}"))
    ok = all(c[1] for c in checks)
    if verbose:
        print(f"== {env}")
        print(f"   ground meshes: {len(ground)}  props: {len(props)}")
        print(f"   ground AABB y[{gmin[1]:.2f},{gmax[1]:.2f}] "
              f"x[{gmin[0]:.1f},{gmax[0]:.1f}] z[{gmin[2]:.1f},{gmax[2]:.1f}]")
        print(f"   props  AABB y[{pmin[1]:.2f},{pmax[1]:.2f}]")
        for k, v, info in checks:
            print(f"   [{'PASS' if v else 'FAIL'}] {k}: {info}")
    return ok, None


def main():
    envs = sys.argv[1:] or DEFAULT_ENVS
    ok = 0
    for env in envs:
        good, msg = verify(env)
        if msg:
            print(msg)
        ok += good
    print(f"\n{ok}/{len(envs)} envs pass ground verification")
    return 0 if ok == len(envs) else 1


if __name__ == "__main__":
    sys.exit(main())
