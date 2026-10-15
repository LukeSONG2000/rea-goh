# -*- coding: utf-8 -*-
"""SWGoH environment / scene extractor  (env_* bundles -> static textured GLB).

Usage:
    py tools/export_env.py list [prefix]          # census of env_* bundles
    py tools/export_env.py extract <name> [...]   # -> extracted_env/<name>/
    py tools/export_env.py extract --all          # full run (resume: skips
                                                  #  existing GLBs unless --force)

Output: extracted_env/<name>/<name>.glb + textures/ + report.json

Design notes
------------
* Reuses the production building blocks of tools/swgoh_extract.py (imported
  as ``se``): GLTFBuilder (glTF writer), conv_v3 / quat_to_mat (Unity->glTF
  X mirror), rkey / go_name (PPtr cache keys), newest_bundle /
  resolve_deps_recursively (cache handling; every env_* bundle depends on
  shared_resourcecontainer + shared_shadercontainer, meshes may live in the
  shared pool).  The character-specific machinery (skins, bind poses, SMR
  bone resolution) is replaced by a static MeshRenderer + MeshFilter path.
* Scene root: env prefabs expose a top-level transform whose GameObject is
  named ``env.<bundle_name>`` (verified on env_bespinroom_01_pre,
  env_empire_hangar_pre, env_skb_halls_01_pre, env_space_starfield02_pre,
  env_nab_raid_pre).  Some bundles contain the root more than once
  (env_nab_raid_pre ships two identical 2485-node copies) -> first wins.
* Branch pruning: camera rigs, battle-layout markers, lights and most
  ParticleSystem VFX subtrees carry no meshes; any node whose subtree has
  no usable static renderer is dropped from the node tree.
* Vertex colors: measured, not assumed.  Recon found vcol on 0 of 63
  referenced meshes in bespinroom/hangar/skb_halls; the only bundle that
  carries them (env_nab_raid_pre, 7/34 meshes) stores flat runtime masks
  (uniform RGB 0.142 on rubble, 0/1 channel selectors on vehicle chassis) -
  the same non-albedo class as the character tint masks (UNPACK_GUIDE 5.4).
  COLOR_0 is therefore skipped and affected mesh names land in report.json.
* Env materials: shader names are stripped (empty string).  They expose
  _MainTex plus BOTH _Color and _Tint; the game multiplies them, so
  baseColorFactor = _Color * _Tint (clamped to 1: some outdoor tints are
  >1, e.g. tat_moseisley_building_02_mat at 1.58 - the game shader
  HDR-brightens, a glTF viewer would just clip to white).  _IllumAmt is
  NOT mapped to emissive: measured 1.0 on every material of whole outdoor
  scenes (tat_moseisley_*) - it is the outdoor shader's sun-brightness,
  not a glow amount, and emissive = baseColor * 1.0 double-lights every
  surface into a whiteout.  _Mask is a shader-private mask that cannot be
  interpreted without the stripped shader and is skipped; alpha-blended
  decal materials are exported opaque for the same reason (limitations
  are recorded in report.json).
* SkinnedMeshRenderers (staged characters inside big scenes such as
  env_nab_raid_pre) are out of scope for static scenes and only counted.
* Ground: arenas stage their terrain either with real meshes inside the env
  bundle (tat_moseisley_ground_mesh, tb_terrain_hoth_01_mesh) or with Unity
  BUILTIN primitives -- the MeshFilter mesh PPtr points into the external
  "unity default resources" file (pathID 10210 Quad / 10209 Plane / 10202
  Cube, ...), which does not ship in the cache.  Those renderers used to be
  dropped as mr_without_mesh, which is why many env GLBs had no ground
  (catalog/BUGS.md P2, e.g. env_arv7_nikto_base_pre).  Builtin primitives are
  now synthesized (exact Quad/Plane/Cube geometry; canonical approximations
  for Sphere/Cylinder/Capsule) and listed in report builtin_meshes.  A census
  found 222/269 env bundles referencing builtin meshes; the heavy users are
  mostly fog/billboard quads plus the missing grounds.
"""
import argparse
import gc
import json
import sys
import time
from collections import Counter
from pathlib import Path

import numpy as np
import UnityPy
from UnityPy.helpers.MeshHelper import MeshHandler

sys.path.insert(0, str(Path(__file__).resolve().parent))
import swgoh_extract as se   # shared primitives, see module docstring

OUT_ROOT = Path(__file__).resolve().parent.parent / "extracted_env"

# battle-camera anchor nodes: kept in the GLB (as empty transforms) even
# though they carry no meshes -- they are where the game frames the arena
# from, which makes them the canonical preview viewpoint
CAM_NODE_NAMES = ("Std Cam", "Flip Cam")

LIMITATIONS = [
    "COLOR_0 skipped: env vertex colors are runtime tint/channel masks, not albedo "
    "(measured; see module docstring). Per-mesh list in vertex_color_meshes.",
    "_Mask texture slot ignored: shader stripped, semantics unknown.",
    "_IllumAmt ignored: outdoor env shaders set it to 1.0 scene-wide "
    "(sun-brightness, not glow); mapping it to glTF emissive whiteouts surfaces.",
    "_Color*_Tint baseColorFactor clamped to 1.0 (outdoor tints reach 1.58; "
    "game HDR-brightens, a glTF viewer would clip).",
    "Alpha-blended decal / particle materials exported opaque (no shader to tell).",
    "Lights, cameras, particle VFX, sprites not exported (glTF static mesh scope).",
    "Unity builtin-primitive meshes (Quad/Plane/Cube/Sphere/Cylinder/Capsule: "
    "exact for Quad/Plane/Cube, canonical-shape approximations for Sphere/"
    "Cylinder/Capsule) are synthesized; see report builtin_meshes. Real meshes "
    "living in unresolvable external bundles are still skipped "
    "(renderers.external_mesh).",
]

# ---------------------------------------------------------------------------
# Unity builtin "unity default resources" primitives
# ---------------------------------------------------------------------------
# Many envs stage their ground / fog / AOE decals with Unity's PRIMITIVE
# meshes: the MeshFilter's mesh PPtr points into an external file ("unity
# default resources", fileID != 0, e.g. pathID 10210) that is not shipped in
# the SWGoH bundle cache -- previously these renderers were dropped
# ("mr_without_mesh"), which is exactly why those arenas had no ground.
# pathIDs are Unity's well-known builtin ids; Quad (10210) and Cube (10202)
# are confirmed against GO names/scales in this app (Quad-billboards face the
# -Z camera at identity rotation, 'Ground' nodes lay the same quad flat with a
# +90deg X rotation; 'Cube' grounds scale 1x1 unit slabs).  Geometry is
# canonical Unity: Quad 1x1 XY facing -Z, Plane 10x10 XZ facing +Y (11x11
# verts), Cube 1x1x1; Sphere/Cylinder/Capsule are canonical-shape
# approximations (radius .5, height 2, modest tessellation).
BUILTIN_QUAD = 10210
BUILTIN_PLANE = 10209
BUILTIN_CUBE = 10202
BUILTIN_SPHERE = 10206
BUILTIN_CYLINDER = 10207
BUILTIN_CAPSULE = 10208
BUILTIN_NAMES = {
    BUILTIN_CUBE: "cube", BUILTIN_SPHERE: "sphere", BUILTIN_CYLINDER: "cylinder",
    BUILTIN_CAPSULE: "capsule", BUILTIN_PLANE: "plane", BUILTIN_QUAD: "quad",
}


def _builtin_quad():
    v = [(-.5, -.5, 0), (.5, -.5, 0), (.5, .5, 0), (-.5, .5, 0)]
    uv = [(0, 0), (1, 0), (1, 1), (0, 1)]
    n = (0, 0, -1)                       # Unity Quad faces -Z (the camera)
    tri = [(0, 2, 1), (0, 3, 2)]         # winding consistent with normal
    return v, [n] * 4, uv, tri


def _builtin_cube():
    faces = [   # (axis, sign): 1x1x1 centered, per-face uv square
        ((0, 0, -1), [( -.5, -.5, -.5), (.5, -.5, -.5), (.5, .5, -.5), (-.5, .5, -.5)]),
        ((0, 0, 1), [(.5, -.5, .5), (-.5, -.5, .5), (-.5, .5, .5), (.5, .5, .5)]),
        ((-1, 0, 0), [(-.5, -.5, .5), (-.5, -.5, -.5), (-.5, .5, -.5), (-.5, .5, .5)]),
        ((1, 0, 0), [(.5, -.5, -.5), (.5, -.5, .5), (.5, .5, .5), (.5, .5, -.5)]),
        ((0, 1, 0), [(-.5, .5, -.5), (.5, .5, -.5), (.5, .5, .5), (-.5, .5, .5)]),
        ((0, -1, 0), [(-.5, -.5, .5), (.5, -.5, .5), (.5, -.5, -.5), (-.5, -.5, -.5)]),
    ]
    v, n, uv, tri = [], [], [], []
    uvs = [(0, 0), (1, 0), (1, 1), (0, 1)]
    for nrm, quad in faces:
        base = len(v)
        v += quad
        n += [nrm] * 4
        uv += uvs
        tri += [(base, base + 2, base + 1), (base, base + 3, base + 2)]
    return v, n, uv, tri


def _builtin_plane():
    # Unity Plane: 10x10 units in XZ, normal +Y, 11x11 vertices
    v, uv, tri = [], [], []
    N = 10
    for j in range(N + 1):
        for i in range(N + 1):
            x, z = -5 + i, -5 + j
            v.append((x, 0, z))
            uv.append((i / N, j / N))

    def vid(i, j):
        return j * (N + 1) + i
    for j in range(N):
        for i in range(N):
            a, b, c, d = vid(i, j), vid(i + 1, j), vid(i, j + 1), vid(i + 1, j + 1)
            tri += [(a, c, b), (c, d, b)]    # +Y winding
    n = (0, 1, 0)
    return v, [n] * len(v), uv, tri


def _builtin_uv_sphere(seg_u=24, seg_v=16):
    # radius .5 sphere centered at origin (canonical Unity Sphere, lower tess)
    import math
    v, uv, n, tri = [], [], [], []
    for j in range(seg_v + 1):
        phi = math.pi * j / seg_v
        for i in range(seg_u + 1):
            th = 2 * math.pi * i / seg_u
            p = (0.5 * math.sin(phi) * math.cos(th), 0.5 * math.cos(phi),
                 0.5 * math.sin(phi) * math.sin(th))
            v.append(p)
            n.append(p)
            uv.append((i / seg_u, 1 - j / seg_v))
    def vid(i, j):
        return j * (seg_u + 1) + i
    for j in range(seg_v):
        for i in range(seg_u):
            a, b, c, d = vid(i, j), vid(i + 1, j), vid(i, j + 1), vid(i + 1, j + 1)
            tri += [(a, b, d), (a, d, c)]
    return v, n, uv, tri


def _builtin_cylinder(seg=24):
    # radius .5, height 2, Y-centered, capped (canonical Unity Cylinder)
    import math
    v, uv, n, tri = [], [], [], []
    ring = seg + 1
    for y, ny in ((-1.0, 0.0), (1.0, 0.0)):
        for i in range(ring):
            th = 2 * math.pi * i / seg
            cx, cz = math.cos(th), math.sin(th)
            v.append((0.5 * cx, y, 0.5 * cz))
            n.append((cx, 0, cz))
            uv.append((i / seg, 0.0 if y < 0 else 1.0))
    for i in range(seg):
        a, b, c, d = i, i + 1, ring + i, ring + i + 1
        tri += [(a, c, b), (b, c, d)]
    for cy, ny, cidx in ((-1.0, -1.0, 0), (1.0, 1.0, 1)):
        center = len(v)
        v.append((0, cy, 0))
        n.append((0, ny, 0))
        uv.append((0.5, 0.5))
        for i in range(ring):
            th = 2 * math.pi * i / seg
            v.append((0.5 * math.cos(th), cy, 0.5 * math.sin(th)))
            n.append((0, ny, 0))
            uv.append((0.5 + 0.5 * math.cos(th), 0.5 + 0.5 * math.sin(th)))
        for i in range(seg):
            a, b = center + 1 + i, center + 1 + i + 1
            if ny > 0:
                tri.append((center, b, a))
            else:
                tri.append((center, a, b))
    return v, n, uv, tri


def _builtin_capsule(seg_u=16, seg_v=8):
    # radius .5, total height 2: cylinder core + hemispherical caps (approx)
    import math
    half = 0.5                       # cylinder half-height
    # profile from bottom pole to top pole: (y, radius, ny, nr)
    profile = []
    for j in range(seg_v, -1, -1):   # bottom hemisphere: t = pi..pi/2
        t = math.pi * j / seg_v
        profile.append((-half + half * math.cos(t), half * math.sin(t),
                        math.cos(t), math.sin(t)))
    profile.append((-half, half, 0.0, 1.0))     # wall bottom ring
    profile.append((half, half, 0.0, 1.0))      # wall top ring
    for j in range(seg_v, -1, -1):   # top hemisphere: t = pi/2..0
        t = math.pi * j / seg_v
        profile.append((half + half * math.cos(t), half * math.sin(t),
                        math.cos(t), math.sin(t)))
    v, uv, n, tri = [], [], [], []
    for i in range(seg_u + 1):
        th = 2 * math.pi * i / seg_u
        cx, cz = math.cos(th), math.sin(th)
        for y, r, ny, nr in profile:
            v.append((r * cx, y, r * cz))
            n.append((nr * cx, ny, nr * cz))
            uv.append((i / seg_u, (y + 1) / 2))
    rows = len(profile)

    def vid(i, j):
        return j * (seg_u + 1) + i
    for j in range(rows - 1):
        for i in range(seg_u):
            a, b, c, d = vid(i, j), vid(i + 1, j), vid(i, j + 1), vid(i + 1, j + 1)
            tri += [(a, b, d), (a, d, c)]
    return v, n, uv, tri


_BUILTIN_BUILDERS = {
    BUILTIN_QUAD: _builtin_quad,
    BUILTIN_PLANE: _builtin_plane,
    BUILTIN_CUBE: _builtin_cube,
    BUILTIN_SPHERE: _builtin_uv_sphere,
    BUILTIN_CYLINDER: _builtin_cylinder,
    BUILTIN_CAPSULE: _builtin_capsule,
}


# --------------------------------------------------------------------------
# exporter
# --------------------------------------------------------------------------

class EnvExporter(se.CharacterExporter):
    """Static-scene variant of CharacterExporter (same GLTFBuilder/mirroring)."""

    def __init__(self, env, name, out_dir, report):
        super().__init__(env, name, out_dir, report)
        self._go_cache = {}        # go rkey -> parsed GameObject (or None)
        self._mesh_scan = {}       # mesh rkey -> scan result (vcol / counts)
        self.root = None

    # -- helpers -----------------------------------------------------------
    def go_parsed(self, ptr):
        """Cached PPtr -> parsed GameObject (deref+parse is hot in census)."""
        if ptr is None or ptr.m_PathID == 0:
            return None
        try:
            reader = ptr.deref()
        except Exception:
            return None
        k = se.rkey(reader)
        if k not in self._go_cache:
            try:
                self._go_cache[k] = reader.parse_as_object()
            except Exception:
                self._go_cache[k] = None
        return self._go_cache[k]

    def find_root(self, bundle_name):
        """Top-level transform named 'env.<bundle>' (fallback: any 'env.*')."""
        exact, loose = [], []
        for obj in self.env.objects:
            if obj.type.name != "Transform":
                continue
            try:
                t = obj.parse_as_object()
            except Exception:
                continue
            father = getattr(t, "m_Father", None)
            if father is None or father.m_PathID != 0:
                continue
            go = self.go_parsed(t.m_GameObject)
            n = se.go_name(go) if go else ""
            if n == f"env.{bundle_name}":
                exact.append(t)
            elif n.startswith("env."):
                loose.append((n, t))
        if exact:
            self.root = exact[0]
            self.report["duplicate_env_roots"] = len(exact) - 1
        elif loose:
            loose.sort(key=lambda x: x[0])
            self.root = loose[0][1]
            self.report["root_fallback"] = loose[0][0]
        return self.root

    # -- scene graph -------------------------------------------------------
    def _index_subtree(self, root):
        """rkey -> node info for the whole scene subtree (iterative: the big
        raid scenes hold ~2500 transforms and Python recursion is tight)."""
        info = {}
        root_rk = se.rkey(root.object_reader)
        info[root_rk] = {"t": root, "go": self.go_parsed(root.m_GameObject),
                         "parent": None, "mesh": False}
        stack = [root_rk]
        while stack:
            rk = stack.pop()
            e = info[rk]
            e["go"] = self.go_parsed(e["t"].m_GameObject)
            e["name"] = se.go_name(e["go"]) if e["go"] else "unnamed"
            e["active"] = bool(getattr(e["go"], "m_IsActive", True)) if e["go"] else True
            e["children"] = []
            e["go_rk"] = se.rkey(e["go"].object_reader) if e["go"] else None
            for ch in e["t"].m_Children:
                try:
                    ct = ch.deref_parse_as_object()
                except Exception:
                    continue
                ckey = se.rkey(ct.object_reader)
                if ckey in info:          # sane guard against shared children
                    continue
                info[ckey] = {"t": ct, "go": None, "parent": rk, "mesh": False}
                e["children"].append(ckey)
                stack.append(ckey)
        # the prefab root GO is often stored m_IsActive=False (the game enables
        # it when staging the scene) -- that must not mute the whole tree;
        # non-root inactive nodes stay pruned (real hidden decor)
        info[root_rk]["active"] = True
        return info, root_rk

    @staticmethod
    def _chain_active(info, rk):
        """m_IsActive ANDed down the ancestor chain (a disabled layout group
        hides every renderer below it at runtime)."""
        while rk is not None:
            if not info[rk]["active"]:
                return False
            rk = info[rk]["parent"]
        return True

    def _scan_renderers(self, info):
        """Classify every renderer in the env: usable static pairs vs skipped.

        Returns (usable, info): usable is a list of
        (transform_rkey, mesh, [material PPtrs], label); info nodes get
        .mesh=True for every transform carrying a usable mesh, which drives
        branch pruning.
        """
        tr_of_go = {e.get("go_rk"): rk for rk, e in info.items()
                    if e.get("go_rk") is not None}
        usable, counts, smr_under = [], Counter(), []

        def under(go_ptr):
            go = self.go_parsed(go_ptr)
            if go is None:
                return None
            return tr_of_go.get(se.rkey(go.object_reader))

        for obj in self.env.objects:
            tn = obj.type.name
            if tn not in ("MeshRenderer", "SkinnedMeshRenderer",
                          "ParticleSystemRenderer", "SpriteRenderer"):
                continue
            try:
                r = obj.parse_as_object()
            except Exception:
                counts[f"{tn}_unparsable"] += 1
                continue
            trk = under(getattr(r, "m_GameObject", None))
            if trk is None:
                counts[f"{tn}_outside_scene"] += 1
                continue
            if not self._chain_active(info, trk):
                counts[f"{tn}_inactive"] += 1
                continue
            counts[f"{tn}_under_scene"] += 1
            if tn != "MeshRenderer":
                if tn == "SkinnedMeshRenderer":
                    g = self.go_parsed(r.m_GameObject)
                    smr_under.append(se.go_name(g) if g else "?")
                continue
            # shadow-caster duplicates: skip by GameObject and mesh name
            g = self.go_parsed(r.m_GameObject)
            gname = se.go_name(g) if g else ""
            if any(p in gname for p in se.SKIP_GO_NAME_PARTS):
                counts["mr_shadow_go"] += 1
                continue
            mesh = None
            builtin_pid = None
            try:
                gparsed = self.go_parsed(r.m_GameObject)
                for comp in gparsed.m_Component:
                    f = self.deref(comp.component)
                    if f is not None and type(f).__name__ == "MeshFilter":
                        mesh = self.deref(f.m_Mesh)
                        if mesh is None:
                            mp = getattr(f, "m_Mesh", None)
                            pid = getattr(mp, "m_PathID", 0)
                            if pid in _BUILTIN_BUILDERS:
                                builtin_pid = pid
                        break
            except Exception:
                pass
            if mesh is None and builtin_pid is None:
                counts["mr_without_mesh"] += 1
                continue
            if mesh is None:
                # Unity builtin primitive ("unity default resources"): the
                # game stages grounds/fog/AOE decals with these; synthesize
                # the canonical geometry (see BUILTIN_* notes above)
                usable.append((trk, ("builtin", builtin_pid), list(r.m_Materials or []),
                               f"{gname}:builtin:{BUILTIN_NAMES[builtin_pid]}"))
                info[trk]["mesh"] = True
                p = info[trk]["parent"]
                while p is not None:                 # keep the ancestor chain
                    info[p]["mesh"] = True
                    p = info[p]["parent"]
                counts["mr_builtin_mesh"] += 1
                continue
            mname = se.go_name(mesh)
            if (any(p in mname for p in se.SKIP_GO_NAME_PARTS)
                    or "shadowmesh" in mname.lower()):
                counts["mr_shadow_mesh"] += 1
                continue
            usable.append((trk, mesh, list(r.m_Materials or []),
                           f"{gname}:{mname}"))
            info[trk]["mesh"] = True
            p = info[trk]["parent"]
            while p is not None:                 # keep the whole ancestor chain
                info[p]["mesh"] = True
                p = info[p]["parent"]
        return usable, counts, smr_under

    def _scan_meshes(self, usable):
        """One MeshHandler pass per unique mesh: vertex-color detection and
        triangle/vertex totals for the report."""
        seen = set()
        tri_total = vert_total = 0
        vcol = []
        for _, mesh, _, _ in usable:
            if isinstance(mesh, tuple) and mesh[0] == "builtin":
                continue                # synthesized primitives: no source mesh
            k = se.rkey(mesh.object_reader)
            if k in seen:
                continue
            seen.add(k)
            try:
                h = MeshHandler(mesh)
                h.process()
            except Exception as e:
                self.report["mesh_errors"].append(f"{se.go_name(mesh)}: scan {e}")
                continue
            vc = h.m_VertexCount
            vert_total += vc
            try:
                tri_total += sum(len(t) for t in (h.get_triangles() or []))
            except Exception:
                pass
            cols = getattr(h, "m_Colors", None)
            if cols and len(cols) == vc:
                arr = np.asarray(cols, dtype=np.float64)
                vcol.append({"mesh": se.go_name(mesh),
                             "rgb_mean": [round(float(x), 4) for x in arr[:, :3].mean(axis=0)],
                             "rgb_max": [round(float(x), 4) for x in arr[:, :3].max(axis=0)]})
        self.report["vertex_color_meshes"] = vcol
        self.report["unique_meshes"] = len(seen)
        self.report["triangles"] = tri_total
        self.report["vertices"] = vert_total

    def _walk_kept(self, info, root_rk):
        """Create glTF nodes for pruned subtree (Unity->glTF X mirror; the
        transform math is se.CharacterExporter.walk applied to a subset)."""
        def visit(rk, parent_idx):
            e = info[rk]
            t = e["t"]
            lp, lr, ls = t.m_LocalPosition, t.m_LocalRotation, t.m_LocalScale
            node = {"name": e["name"]}
            if (lp.x, lp.y, lp.z) != (0.0, 0.0, 0.0):
                node["translation"] = [-lp.x, lp.y, lp.z]
            if (lr.x, lr.y, lr.z, lr.w) != (0.0, 0.0, 0.0, 1.0):
                node["rotation"] = [lr.x, -lr.y, -lr.z, lr.w]
            if (ls.x, ls.y, ls.z) != (1.0, 1.0, 1.0):
                node["scale"] = [ls.x, ls.y, ls.z]
            self.g.nodes.append(node)
            idx = len(self.g.nodes) - 1
            self.node_of[rk] = idx
            if parent_idx is not None:
                self.parent_of[idx] = parent_idx
                self.g.nodes[parent_idx].setdefault("children", []).append(idx)
            L = np.eye(4)
            L[:3, :3] = se.quat_to_mat((lr.x, -lr.y, -lr.z, lr.w)) * np.array(
                [ls.x, ls.y, ls.z])
            L[:3, 3] = (-lp.x, lp.y, lp.z)
            self.local_M[idx] = L
            for ck in e.get("children", ()):
                if info[ck]["mesh"]:
                    visit(ck, idx)
            return idx
        return visit(root_rk, None)

    # -- materials (env shaders) -------------------------------------------
    def get_material_index(self, mat_ptr):
        """Env-variant of se.CharacterExporter.get_material_index: multiplies
        _Color * _Tint, adds _IllumAmt self-illumination, drops _Mask."""
        if not mat_ptr or mat_ptr.m_PathID == 0:
            return None
        try:
            reader = mat_ptr.deref()
        except Exception:
            return None
        k = se.rkey(reader)
        if k in self.mat_cache:
            return self.mat_cache[k]
        mat = reader.parse_as_object()
        props = mat.m_SavedProperties
        texs = {t[0]: t[1] for t in props.m_TexEnvs}
        cols = {c[0]: c[1] for c in props.m_Colors}
        floats = {f[0]: f[1] for f in props.m_Floats}

        def texptr(name):
            env = texs.get(name)
            return getattr(env, "m_Texture", None) if env is not None else None

        mat_json = {"name": se.go_name(mat), "doubleSided": True}
        pbr = {}
        base = [1.0, 1.0, 1.0, 1.0]
        for key in ("_Color", "_Tint"):        # env shaders multiply both
            c = cols.get(key)
            if c is not None:
                base = [base[0] * c.r, base[1] * c.g,
                        base[2] * c.b, base[3] * c.a]
        base = [min(max(v, 0.0), 1.0) for v in base]   # outdoor tints > 1
        if any(abs(v - 1.0) > 1e-6 for v in base):
            pbr["baseColorFactor"] = [round(v, 6) for v in base]
        main = self.get_tex_index(texptr("_MainTex"))
        if main is not None:
            pbr["baseColorTexture"] = {"index": main}
        pbr["metallicFactor"] = floats.get("_Metallic", 0.0)
        rough = 1.0 - floats.get("_Glossiness", 0.1)
        pbr["roughnessFactor"] = max(0.0, min(1.0, rough))
        mat_json["pbrMetallicRoughness"] = pbr

        bump = self.get_tex_index(texptr("_BumpMap"))
        if bump is not None:
            mat_json["normalTexture"] = {"index": bump}
        occ = self.get_tex_index(texptr("_OcclusionMap"))
        if occ is not None:
            mat_json["occlusionTexture"] = {"index": occ}
        cutoff = floats.get("_Cutoff")
        if cutoff:
            mat_json["alphaMode"] = "MASK"
            mat_json["alphaCutoff"] = cutoff
        # _IllumAmt deliberately ignored -- see module docstring / LIMITATIONS
        self.g.materials.append(mat_json)
        idx = len(self.g.materials) - 1
        self.mat_cache[k] = idx
        return idx

    def _drop_flip_layout(self, info):
        """Some scenes ship the whole room twice: once under 'Std Cam' and a
        mirrored variant under 'Flip Cam' (the game enables one layout at
        runtime).  Both active in the raw data -> keep the Std layout, mute
        the Flip subtree."""
        names = {e["name"] for e in info.values()}
        if "Std Cam" in names and "Flip Cam" in names:
            for rk, e in info.items():
                if e["name"] == "Flip Cam":
                    e["active"] = False
                    self.report["dropped_flip_layout"] = True
                    break

    def _keep_battle_cameras(self, info):
        """Mark Std Cam / Flip Cam subtrees (+ ancestor chains) as kept."""
        kept = []
        for rk, e in info.items():
            if e["name"] not in CAM_NODE_NAMES or not self._chain_active(info, rk):
                continue
            kept.append(e["name"])
            stack = [rk]
            while stack:
                k = stack.pop()
                info[k]["mesh"] = True
                stack.extend(info[k].get("children", ()))
            p = info[rk]["parent"]
            while p is not None:
                info[p]["mesh"] = True
                p = info[p]["parent"]
        self.report["battle_camera_nodes"] = kept

    def _export_builtin_primitive(self, path_id, node_idx, mat_ptrs, label):
        """Synthesize a Unity builtin primitive mesh (see BUILTIN_* above) and
        attach it to node_idx.  Unity->glTF X-mirror applied by hand: mirrored
        positions/normals and reversed winding, same as MeshHandler path."""
        verts, norms, uvs, tris = _BUILTIN_BUILDERS[path_id]()
        g = self.g
        pos = [c for p in verts for c in (-p[0], p[1], p[2])]
        nrm = [c for n in norms for c in (-n[0], n[1], n[2])]
        uvs = [c for t in uvs for c in (t[0], 1.0 - t[1])]
        attrs = {
            "POSITION": g.add_accessor(pos, "float", len(verts), "VEC3",
                                       minmax=True),
            "NORMAL": g.add_accessor(nrm, "float", len(verts), "VEC3"),
            "TEXCOORD_0": g.add_accessor(uvs, "float", len(verts), "VEC2"),
        }
        flat = []
        for a, b, c in tris:
            flat += (a, c, b)            # reverse winding after X-mirror
        prim = {"attributes": attrs, "mode": 4, "indices": g.add_indices(flat)}
        mi = None
        for p in (mat_ptrs or []):
            mi = self.get_material_index(p)
            if mi is not None:
                break
        if mi is not None:
            prim["material"] = mi
        name = f"builtin:{BUILTIN_NAMES[path_id]}"
        g.meshes.append({"name": name, "primitives": [prim]})
        mesh_idx = len(g.meshes) - 1
        g.nodes[node_idx]["mesh"] = mesh_idx
        self.report["exported_meshes"].append(label)
        self.report.setdefault("builtin_meshes", []).append(label)

    # -- run ----------------------------------------------------------------
    def export(self):
        """Build and write the GLB; returns (path, stats)."""
        self.png_dir.mkdir(parents=True, exist_ok=True)
        info, root_rk = self._index_subtree(self.root)
        self._drop_flip_layout(info)       # before the renderer scan: muted
        usable, counts, smr_under = self._scan_renderers(info)
        self.report["renderers_under_scene"] = dict(counts)
        self.report["skinned_renderers_skipped"] = smr_under
        self.report["gameobjects_in_scene"] = len(info)
        self._scan_meshes(usable)
        self._keep_battle_cameras(info)
        self._walk_kept(info, root_rk)
        root_idx = self.node_of.get(root_rk)
        for trk, mesh, mats, label in usable:
            node_idx = self.node_of.get(trk)
            if node_idx is None:
                continue
            if isinstance(mesh, tuple) and mesh[0] == "builtin":
                try:
                    self._export_builtin_primitive(mesh[1], node_idx, mats, label)
                except Exception as e:
                    import traceback
                    self.report["mesh_errors"].append(
                        f"{label}: builtin {type(e).__name__} {e}\n"
                        + traceback.format_exc())
                continue
            try:
                mesh_idx, _ = self.export_mesh_for_renderer(
                    mesh, None, mats, se.go_name(mesh))
            except Exception as e:
                import traceback
                self.report["mesh_errors"].append(
                    f"{label}: {type(e).__name__} {e}\n" + traceback.format_exc())
                continue
            if mesh_idx is None:
                continue
            self.g.nodes[node_idx]["mesh"] = mesh_idx
            self.report["exported_meshes"].append(label)
        glb = self.g.build_glb(
            [root_idx], generator="tools/export_env.py (swgoh_extract.py core)")
        out = self.out_dir / f"{self.char_name}.glb"
        out.write_bytes(glb)
        self.report["nodes_exported"] = len(self.g.nodes)
        self.report["meshes_exported"] = len(self.report["exported_meshes"])
        self.report["materials"] = len(self.g.materials)
        self.report["textures"] = len(self.g.textures)
        return out


# --------------------------------------------------------------------------
# commands
# --------------------------------------------------------------------------

def _newest(path: Path):
    return se.newest_bundle(path)


def env_bundles(prefix="env_"):
    out = []
    for p in sorted(se.CACHE.iterdir()):
        if p.is_dir() and p.name.startswith(prefix) and _newest(p):
            out.append(p)
    return out


def list_env(prefix="env_"):
    rows = []
    for p in env_bundles(prefix):
        b = _newest(p)
        env = UnityPy.load(str(b))
        c = Counter(o.type.name for o in env.objects)
        del env
        rows.append((p.name, b.stat().st_size, c))
    print(f"{'bundle':<52}{'KB':>7}{'GO':>6}{'MR':>5}{'Mesh':>6}{'Mat':>5}{'Tex':>5}"
          f"{'PSR':>5}")
    total = 0
    for name, size, c in rows:
        total += size
        print(f"{name:<52}{size/1024:>7.0f}{c.get('GameObject', 0):>6}"
              f"{c.get('MeshRenderer', 0):>5}{c.get('Mesh', 0):>6}"
              f"{c.get('Material', 0):>5}{c.get('Texture2D', 0):>5}"
              f"{c.get('ParticleSystemRenderer', 0):>5}")
    print(f"\n{len(rows)} bundles, {total/1e6:.1f} MB raw")


def extract(name, out_root=OUT_ROOT, force=False):
    out_dir = out_root / name
    glb_path = out_dir / f"{name}.glb"
    if glb_path.is_file() and not force:
        print(f"[{name}] already exported ({glb_path.stat().st_size/1024:.0f} KB),"
              f" skip (use --force to redo)")
        return True
    folder = se.CACHE / name
    bundle = _newest(folder)
    if bundle is None:
        print(f"  !! bundle not found: {name}")
        return False
    t0 = time.time()
    files = se.resolve_deps_recursively(bundle)
    env = UnityPy.load(*[str(f) for f in files])
    report = {
        "name": name,
        "kind": "environment",
        "source_bundle": str(bundle),
        "dependency_bundles": [str(f) for f in files],
        "exported_meshes": [], "mesh_errors": [], "texture_errors": [],
        "limitations": LIMITATIONS,
    }
    exp = EnvExporter(env, name, out_dir, report)
    try:
        if exp.find_root(name) is None:
            print(f"  !! no 'env.*' root transform found")
            return False
        glb_path = exp.export()
        report["glb_bytes"] = glb_path.stat().st_size
        report["seconds"] = round(time.time() - t0, 1)
        (out_dir / "report.json").write_text(
            json.dumps(report, indent=2, ensure_ascii=False), encoding="utf-8")
        print(f"  -> {glb_path}  ({glb_path.stat().st_size/1024:.0f} KB, "
              f"{report['seconds']}s)")
        print(f"     nodes={report['nodes_exported']} "
              f"meshes={report['meshes_exported']}/{report['unique_meshes']} uniq "
              f"tris={report['triangles']} mats={report['materials']} "
              f"tex={report['textures']} | "
              f"err: {len(report['mesh_errors'])} mesh / "
              f"{len(report['texture_errors'])} tex")
        if report["mesh_errors"] or report["texture_errors"]:
            for e in (report["mesh_errors"] + report["texture_errors"])[:8]:
                print(f"       ! {e}")
        return True
    finally:
        del env
        gc.collect()


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("cmd", choices=["list", "extract"])
    ap.add_argument("names", nargs="*")
    ap.add_argument("--prefix", default="env_")
    ap.add_argument("--all", action="store_true",
                    help="extract every env_* bundle (resume: existing GLBs "
                         "are skipped unless --force)")
    ap.add_argument("--force", action="store_true")
    args = ap.parse_args()

    if args.cmd == "list":
        list_env(args.prefix or "env_")
        return
    names = args.names
    if args.all:
        names = [p.name for p in env_bundles(args.prefix)]
    if not names:
        print("specify bundle name(s), or --all; use 'list' to see them")
        sys.exit(1)
    ok = 0
    t0 = time.time()
    for i, n in enumerate(names, 1):
        print(f"=== {i}/{len(names)} {n}")
        try:
            if extract(n, force=args.force):
                ok += 1
        except Exception as e:
            import traceback
            print(f"  !! FAILED: {type(e).__name__}: {e}")
            traceback.print_exc()
    print(f"done: {ok}/{len(names)} in {time.time()-t0:.0f}s")


if __name__ == "__main__":
    main()
