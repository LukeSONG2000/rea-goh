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
]


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
            try:
                gparsed = self.go_parsed(r.m_GameObject)
                for comp in gparsed.m_Component:
                    f = self.deref(comp.component)
                    if f is not None and type(f).__name__ == "MeshFilter":
                        mesh = self.deref(f.m_Mesh)
                        break
            except Exception:
                pass
            if mesh is None:
                counts["mr_without_mesh"] += 1
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
