# -*- coding: utf-8 -*-
"""
SWGoH character model extractor.

Reads Unity AssetBundles from the local SWGoH download cache
(%USERPROFILE%/AppData/LocalLow/Unity/Electronic Arts_SWGoH) and exports
characters as rigged, textured glTF (.glb) files.

Usage:
    py swgoh_extract.py list
    py swgoh_extract.py extract <bundle_folder> [more...]
    py swgoh_extract.py extract-all [--only char]
Output:
    F:\\PPfavorite\\Work\\goh\\extracted\\<name>\\<name>.glb + textures\\

Per-object state honored from the source prefab (BUGS.md P5/P8):

  * GameObject m_IsActive is recorded per node as the custom node extension
    ``extensions.SWGOH_active = false`` (only written when false; declared in
    extensionsUsed).  That flag is the game's authored outfit-variant truth --
    helmet on/off, fist vs open hand, hidden VFX rigs -- so viewers must start
    those nodes (and their subtrees) hidden.  This is the single chosen
    mechanism; there is deliberately no top-level SWGOH_inactive array.
  * Renderers on ParticleSystem GameObjects (and their subtrees) are excluded
    from mesh export: particles are simulated effects, not static meshes.
  * Materials the engine blends as effects (particle/additive shaders, glow /
    trail / beam / vfx names, mostly-black+white gradient textures) are
    exported with alphaMode BLEND + an emissive term instead of opaque slabs.
  * Renderers with m_Enabled = 0 are skipped -- the engine never draws them.
  * Real VFX playback (particle emission, trails, sabers) is a separate
    future system; see tools/export_char_vfx.py for the point-sprite stopgap.
"""
import argparse
import io
import json
import os
import struct
import sys
import gc
from pathlib import Path

import UnityPy
import numpy as np
from UnityPy.helpers.MeshHelper import MeshHandler

CACHE = Path(os.path.expandvars(r"%USERPROFILE%\AppData\LocalLow\Unity\Electronic Arts_SWGoH"))
_TOOLS_DIR = Path(__file__).resolve().parent
OUT_ROOT = (_TOOLS_DIR.parent if _TOOLS_DIR.name == "tools" else _TOOLS_DIR) / "extracted"


# --------------------------------------------------------------------------
# cache handling
# --------------------------------------------------------------------------

def bundle_versions(folder: Path):
    """Return list of (mtime, __data path) for a bundle's cached versions."""
    out = []
    for sub in folder.iterdir():
        data = sub / "__data"
        if data.is_file():
            out.append((data.stat().st_mtime, data))
    out.sort()
    return out


def newest_bundle(folder: Path):
    v = bundle_versions(folder)
    return v[-1][1] if v else None


def resolve_deps_recursively(bundle_path: Path, _seen=None):
    """Load a bundle plus all its AssetBundle dependencies (recursively)."""
    if _seen is None:
        _seen = set()
    key = str(bundle_path).lower()
    if key in _seen:
        return []
    _seen.add(key)
    files = [bundle_path]
    env = UnityPy.load(str(bundle_path))
    dep_names = []
    for obj in env.objects:
        if obj.type.name == "AssetBundle":
            dep_names = [d for d in obj.read().m_Dependencies]
            break
    del env
    for dep in dep_names:
        folder = CACHE / dep[:-len(".bundle")] if dep.endswith(".bundle") else CACHE / dep
        if folder.is_dir():
            p = newest_bundle(folder)
            if p:
                files.extend(resolve_deps_recursively(p, _seen))
    return files


def list_bundles():
    names = sorted(p.name for p in CACHE.iterdir() if p.is_dir() and newest_bundle(p))
    chars = [n for n in names if n.startswith(("char_", "battle_character"))]
    print(f"character-like bundles ({len(chars)}):")
    for n in chars:
        print(" ", n)
    others = [n for n in names if n not in set(chars)]
    print(f"\nother bundles ({len(others)}):  (extract by name if needed)")
    print(" ", ", ".join(others[:40]), "...")


# --------------------------------------------------------------------------
# glTF building blocks
# --------------------------------------------------------------------------

class GLTFBuilder:
    """Minimal glTF 2.0 (GLB) writer with buffer/accessor helpers."""

    def __init__(self):
        self.buffer = bytearray()
        self.bufferViews = []
        self.accessors = []
        self.images = []
        self.textures = []
        self.samplers = [{
            "magFilter": 9729, "minFilter": 9987,
            "wrapS": 10497, "wrapT": 10497,
        }]
        self.materials = []
        self.meshes = []
        self.nodes = []
        self.skins = []
        self.extensionsUsed = []

    # -- buffer / accessors ------------------------------------------------
    def _add_view(self, data: bytes, target=None):
        while len(self.buffer) % 4:
            self.buffer.append(0)
        off = len(self.buffer)
        self.buffer += data
        bv = {"buffer": 0, "byteOffset": off, "byteLength": len(data)}
        if target:
            bv["target"] = target
        self.bufferViews.append(bv)
        return len(self.bufferViews) - 1

    def add_accessor(self, data, comp_type, count, acc_type, minmax=False):
        code = {"float": "f", "ubyte": "B", "ushort": "H", "uint": "I"}[comp_type]
        blob = struct.pack(f"<{len(data)}{code}", *data)
        view = self._add_view(blob, target=34962)
        acc = {
            "bufferView": view,
            "componentType": {"float": 5126, "ubyte": 5121,
                              "ushort": 5123, "uint": 5125}[comp_type],
            "count": count,
            "type": acc_type,
        }
        if minmax and acc_type == "VEC3" and comp_type == "float":
            step = 3
            mn = [min(data[i::step]) for i in range(step)]
            mx = [max(data[i::step]) for i in range(step)]
            acc["min"], acc["max"] = mn, mx
        self.accessors.append(acc)
        return len(self.accessors) - 1

    def add_indices(self, tris):
        fmt = "uint" if max(tris) > 65535 else "ushort"
        n = len(tris)
        blob = struct.pack(f"<{n}{'I' if fmt == 'uint' else 'H'}", *tris)
        view = self._add_view(blob, target=34963)
        self.accessors.append({
            "bufferView": view, "componentType": 5125 if fmt == "uint" else 5123,
            "count": n, "type": "SCALAR",
        })
        return len(self.accessors) - 1

    # -- textures ----------------------------------------------------------
    def add_texture(self, png_bytes: bytes, name: str):
        while len(self.buffer) % 4:
            self.buffer.append(0)
        off = len(self.buffer)
        self.buffer += png_bytes
        self.bufferViews.append(
            {"buffer": 0, "byteOffset": off, "byteLength": len(png_bytes)})
        self.images.append({"bufferView": len(self.bufferViews) - 1,
                            "mimeType": "image/png", "name": name})
        self.textures.append({"sampler": 0, "source": len(self.images) - 1})
        return len(self.textures) - 1

    # -- finish ------------------------------------------------------------
    def build_glb(self, scene_root_nodes, generator="swgoh_extract.py"):
        json_data = {
            "asset": {"version": "2.0", "generator": generator},
            "scene": 0,
            "scenes": [{"nodes": scene_root_nodes}],
            "buffers": [{"byteLength": len(self.buffer)}],
            "nodes": self.nodes,
            "meshes": self.meshes,
            "materials": self.materials,
            "textures": self.textures,
            "images": self.images,
            "samplers": self.samplers,
            "accessors": self.accessors,
            "bufferViews": self.bufferViews,
            "skins": self.skins,
        }
        if self.extensionsUsed:
            # declare only extensions actually stamped into the JSON by
            # walk() for BUGS.md P5 (authored m_IsActive); declaring an
            # unused extension misleads consumers into hunting for stamps
            json_data["extensionsUsed"] = list(self.extensionsUsed)
        for key in ("meshes", "materials", "textures", "images", "samplers",
                    "accessors", "bufferViews", "skins", "nodes"):
            if not json_data[key]:
                del json_data[key]
        js = json.dumps(json_data, separators=(",", ":")).encode("utf-8")
        while len(js) % 4:
            js += b" "
        while len(self.buffer) % 4:
            self.buffer.append(0)
        total = 12 + 8 + len(js) + 8 + len(self.buffer)
        out = io.BytesIO()
        out.write(struct.pack("<4sII", b"glTF", 2, total))
        out.write(struct.pack("<I4s", len(js), b"JSON"))
        out.write(js)
        out.write(struct.pack("<I4s", len(self.buffer), b"BIN\0"))
        out.write(self.buffer)
        return out.getvalue()


# --------------------------------------------------------------------------
# coordinate conversion: Unity (LH, Y-up) -> glTF (RH, Y-up) by mirroring X
# --------------------------------------------------------------------------

def conv_v3(v):
    return (-v[0], v[1], v[2])


def conv_q(q):
    return (q[0], -q[1], -q[2], q[3])


def conv_ibm(m):
    """M' = Mx * M * Mx with Mx = diag(-1,1,1,1); Unity Matrix4x4f row-major."""
    e = [[m.e00, m.e01, m.e02, m.e03],
         [m.e10, m.e11, m.e12, m.e13],
         [m.e20, m.e21, m.e22, m.e23],
         [m.e30, m.e31, m.e32, m.e33]]
    e[0][1] *= -1; e[0][2] *= -1; e[0][3] *= -1
    e[1][0] *= -1; e[2][0] *= -1; e[3][0] *= -1
    flat = []
    for col in range(4):          # glTF is column-major
        for row in range(4):
            flat.append(e[row][col])
    return flat


def inv_flat(M):
    """Invert a numpy 4x4 and flatten column-major (glTF MAT4 layout)."""
    return np.linalg.inv(M).T.reshape(-1).tolist()


def quat_to_mat(q):
    x, y, z, w = q
    return np.array([
        [1 - 2 * (y * y + z * z), 2 * (x * y - z * w), 2 * (x * z + y * w)],
        [2 * (x * y + z * w), 1 - 2 * (x * x + z * z), 2 * (y * z - x * w)],
        [2 * (x * z - y * w), 2 * (y * z + x * w), 1 - 2 * (x * x + y * y)]])


def mat_to_quat(R):
    """Rotation matrix -> quaternion (x, y, z, w)."""
    tr = R[0, 0] + R[1, 1] + R[2, 2]
    if tr > 0:
        s = np.sqrt(tr + 1.0) * 2
        w = 0.25 * s
        x = (R[2, 1] - R[1, 2]) / s
        y = (R[0, 2] - R[2, 0]) / s
        z = (R[1, 0] - R[0, 1]) / s
    elif R[0, 0] > R[1, 1] and R[0, 0] > R[2, 2]:
        s = np.sqrt(1.0 + R[0, 0] - R[1, 1] - R[2, 2]) * 2
        w = (R[2, 1] - R[1, 2]) / s
        x = 0.25 * s
        y = (R[0, 1] + R[1, 0]) / s
        z = (R[0, 2] + R[2, 0]) / s
    elif R[1, 1] > R[2, 2]:
        s = np.sqrt(1.0 + R[1, 1] - R[0, 0] - R[2, 2]) * 2
        w = (R[0, 2] - R[2, 0]) / s
        x = (R[0, 1] + R[1, 0]) / s
        y = 0.25 * s
        z = (R[1, 2] + R[2, 1]) / s
    else:
        s = np.sqrt(1.0 + R[2, 2] - R[0, 0] - R[1, 1]) * 2
        w = (R[1, 0] - R[0, 1]) / s
        x = (R[0, 2] + R[2, 0]) / s
        y = (R[1, 2] + R[2, 1]) / s
        z = 0.25 * s
    q = np.array([x, y, z, w])
    q /= np.linalg.norm(q)
    return q


def decompose_trs(M):
    """Affine TRS matrix -> (translation, quaternion, scale); None if not
    decomposable (shear / near-singular)."""
    t = M[:3, 3]
    sx = float(np.linalg.norm(M[:3, 0]))
    sy = float(np.linalg.norm(M[:3, 1]))
    sz = float(np.linalg.norm(M[:3, 2]))
    if sx < 1e-9 or sy < 1e-9 or sz < 1e-9:
        return None
    if np.linalg.det(M[:3, :3]) < 0:
        sx = -sx                      # absorb reflection into one axis
    R = M[:3, :3] / np.array([sx, sy, sz])
    if abs(np.linalg.det(R) - 1.0) > 1e-3:
        return None                   # shear or garbage — leave node alone
    return t, mat_to_quat(R), (sx, sy, sz)


# --------------------------------------------------------------------------
# exporter
# --------------------------------------------------------------------------

SKIP_GO_NAME_PARTS = ("_shadow",)          # shadow-caster duplicates


def go_name(go):
    return getattr(go, "m_Name", "") or "unnamed"


def rkey(reader):
    """Stable hashable key for an ObjectReader within the loaded env."""
    return (id(reader.assets_file), reader.path_id)


class CharacterExporter:
    def __init__(self, env, char_name, out_dir: Path, report: dict):
        self.env = env
        self.char_name = char_name
        self.out_dir = out_dir
        self.report = report
        self.g = GLTFBuilder()
        self.node_of = {}          # Transform ObjectReader -> node index
        self.trans_of_go = {}      # GameObject ObjectReader -> Transform ObjectReader
        self.go_of_node = {}       # node index -> GameObject ObjectReader
        self.name_of_node = {}     # node index -> GameObject name
        self.tex_cache = {}        # texture ObjectReader -> glTF texture index
        self.tex_stats_cache = {}  # texture ObjectReader -> (black, white) ratios
        self.mesh_cache = {}       # (mesh reader, bones tuple) -> glTF mesh idx
        self.mat_cache = {}        # material reader -> glTF material idx
        self.parent_of = {}        # node index -> parent node index
        self.local_M = {}          # node index -> mirrored local matrix (np 4x4)
        self.bind_globals = {}     # node index -> mirrored bind global (from IBM)
        self.bind_pose_mode = False
        self.png_dir = out_dir / "textures"
        # -- source-hierarchy survey (filled by _survey_hierarchy) ----------
        # P3/P6/+1: the game parents props to hand sockets at RUNTIME, so the
        # prefab serializes them either as INACTIVE staging copies or inside
        # MOVER scaffolds outside the skeleton (measured: jedimaster_staff
        # RIFLE_CONSTRAINT/RIFLE_MOVER -> socket RIFLE_LEFT_HAND under
        # Left_wpnJNT; leia_princess root-level blaster duplicate of the
        # in-skeleton pst_defendersportingblaster_pre).  The survey below
        # feeds the walk filters and the attachment plan; everything is read
        # from serialized state, never guessed.
        self.go_info = {}          # transform rkey -> (name, own m_IsActive)
        self.children_of = {}      # transform rkey -> [child transform rkeys]
        self.root_tkeys = []       # transforms with no father
        self.skin_bone_tkeys = set()
        self.subtree_bone = {}     # rkey -> subtree (incl. self) holds a skin bone
        self.subtree_staged = {}   # rkey -> subtree holds a runtime-attached prop
        self.prune_tkeys = set()   # subtrees removed from the scene graph
        self.staged_parent = {}    # prop transform rkey -> socket transform rkey
        self.pending_reparent = [] # (prop node idx, socket node idx)
        self.inactive_kept = set() # node idxs kept for skins, meshes skipped
        self.spawned_props = set() # transform rkeys: inactive socket weapons
        self.spawned_nodes = set() # their glTF node idxs (mesh still exported)

    # -- object helpers ----------------------------------------------------
    def deref(self, ptr):
        try:
            return ptr.deref_parse_as_object()
        except Exception:
            return None

    # -- source-hierarchy survey / serialized attachment plan --------------
    def _survey_hierarchy(self):
        """One typetree pass over the bundle's Transform/GameObject pairs:
        names, own m_IsActive flags, children map and the skinned-mesh bone
        set.  Feeds the walk filters and _plan_attachments."""
        go_of = {}      # go rkey -> (name, active)
        go_trans = {}   # go rkey -> transform rkey
        for obj in self.env.objects:
            if obj.type.name == "GameObject":
                try:
                    g = obj.parse_as_object()
                    go_of[rkey(obj)] = (go_name(g),
                                        bool(getattr(g, "m_IsActive", True)))
                except Exception:
                    continue
            elif obj.type.name == "Transform":
                try:
                    t = obj.parse_as_object()
                except Exception:
                    continue
                tk = rkey(obj)
                gp = getattr(t, "m_GameObject", None)
                if gp is not None and gp.m_PathID:
                    try:
                        gk = rkey(gp.deref())
                        go_trans[gk] = tk
                        # _attach_renderers keys renderers by GO rkey
                        self.trans_of_go[gk] = tk
                    except Exception:
                        pass
                father = getattr(t, "m_Father", None)
                if father is None or not father.m_PathID:
                    self.root_tkeys.append(tk)
                else:
                    try:
                        fk = rkey(father.deref())
                        self.children_of.setdefault(fk, []).append(tk)
                    except Exception:
                        self.root_tkeys.append(tk)
                ch = []
                for c in getattr(t, "m_Children", None) or []:
                    try:
                        ch.append(rkey(c.deref()))
                    except Exception:
                        pass
                self.children_of.setdefault(tk, ch)
        for gk, (name, active) in go_of.items():
            tk = go_trans.get(gk)
            if tk is not None:
                self.go_info[tk] = (name, active)
        # skin bones: every transform referenced by any SkinnedMeshRenderer
        for obj in self.env.objects:
            if obj.type.name != "SkinnedMeshRenderer":
                continue
            try:
                r = obj.parse_as_object()
            except Exception:
                continue
            for bp in getattr(r, "m_Bones", None) or []:
                try:
                    self.skin_bone_tkeys.add(rkey(bp.deref()))
                except Exception:
                    pass

    def _has_skin_bone(self, tk):
        """Subtree rooted at tk contains a skinned-mesh bone (memoized)."""
        v = self.subtree_bone.get(tk)
        if v is not None:
            return v
        v = tk in self.skin_bone_tkeys
        if not v:
            for c in self.children_of.get(tk, ()):
                if self._has_skin_bone(c):
                    v = True
                    break
        self.subtree_bone[tk] = v
        return v

    def _descendants(self, tk, include_self=True):
        out = [tk] if include_self else []
        stack = list(self.children_of.get(tk, ()))
        while stack:
            c = stack.pop()
            out.append(c)
            stack.extend(self.children_of.get(c, ()))
        return out

    def _renderer_tkeys(self):
        """Transform rkeys whose GameObject carries a (skinned) mesh renderer.
        Requires the GO->transform map built by _survey_hierarchy."""
        out = set()
        for obj in self.env.objects:
            if obj.type.name not in ("SkinnedMeshRenderer", "MeshRenderer"):
                continue
            try:
                r = obj.parse_as_object()
                gp = r.m_GameObject
                if gp is None or not gp.m_PathID:
                    continue
                tk = self.trans_of_go.get(rkey(gp.deref()))
                if tk is not None:
                    out.add(tk)
            except Exception:
                continue
        return out

    def _plan_attachments(self, root_tkey):
        """Classify props the engine does not render where they are authored
        (all truth from serialized state; BUGS.md P3②/P6/+1):

        * inactive GO without any skin bone in its subtree -> pruned
          (m_IsActive is the outfit/loadout truth: raid-Grievous sabers,
          leia's R2-D2 companion, fist/hand mesh variants);
        * renderer-bearing subtree OUTSIDE the bone container with a
          same-named counterpart inside -> runtime staging duplicate, pruned
          (leia princess root-level blaster copy);
        * renderer-bearing subtree OUTSIDE the bone container whose name
          shares a token with a socket under a hand joint -> re-parented
          onto that socket post-walk, authored world pose preserved
          expressed in the socket's rest space (jedimaster_staff rifle
          RIFLE_CONSTRAINT/RIFLE_MOVER -> RIFLE_LEFT_HAND).
        """
        self._survey_hierarchy()
        if root_tkey is None:
            root_tkey = self.root_tkeys[0] if self.root_tkeys else None
        if root_tkey is None:
            return
        parent_src = {}
        for tk, chs in self.children_of.items():
            for c in chs:
                parent_src[c] = tk

        def chain_of(tk):
            out = []
            while tk is not None:
                out.append(tk)
                tk = parent_src.get(tk)
            return out

        # inside = descendants of the bone-bearing containers (Scaler, or the
        # mesh node itself on rigs without a Scaler)
        inside = set()
        containers = [c for c in self.children_of.get(root_tkey, ())
                      if self._has_skin_bone(c)]
        if not containers and self._has_skin_bone(root_tkey):
            containers = [root_tkey]
        for c in containers:
            inside.update(self._descendants(c))
        inside_names = {self.go_info[t][0] for t in inside if t in self.go_info}
        renderers = self._renderer_tkeys()

        # Weapon joints carry the character's loadout.  A renderer-bearing
        # GO the prefab serialized INACTIVE under a hand/weapon joint is a
        # spawned weapon (Grievous's four sabers are inactive in every
        # bundle -- the game enables them on spawn): keep it stamped
        # inactive UNLESS the joint already parents an active weapon (then
        # the inactive one is an unselected loadout variant -- jedimaster
        # Obi-Wan's spare swd_anakin_mesh next to his active saber).
        def hand_joint_ancestor(tk):
            for a in chain_of(tk)[:-1]:
                an = self.go_info.get(a, ("", ""))[0].lower()
                if ("wpn" in an or "wrst" in an or "weapon" in an or
                        "hand" in an) and "jnt" in an:
                    return a
            return None

        for t in renderers & inside:
            name_t, active_t = self.go_info.get(t, ("", True))
            if active_t:
                continue
            j = hand_joint_ancestor(t)
            if j is None:
                continue
            # shadow-caster duplicates don't count as the joint's weapon
            siblings = [c for c in self.children_of.get(j, ())
                        if c in renderers
                        and not any(p in self.go_info.get(c, ("", ""))[0]
                                    for p in SKIP_GO_NAME_PARTS)]
            if not any(self.go_info.get(c, ("", False))[1] for c in siblings):
                self.spawned_props.update(self._descendants(t))

        # only transforms that belong to THIS prefab's graph; dependency
        # packs bring their own renderers whose chains never reach the root
        staged = sorted(
            (t for t in renderers
             if t not in inside and root_tkey in chain_of(t)),
            key=lambda t: len(chain_of(t)))          # top-down
        handled = set()

        for prop in staged:
            if prop in handled:
                continue
            name, active = self.go_info.get(prop, ("", True))
            subtree = self._descendants(prop)
            if not active:
                continue
            # 1) duplicate of an in-skeleton counterpart -> prune the whole
            #    staging subtree (the in-skeleton instance is the rendered one)
            if name in inside_names:
                for t in subtree:
                    if t not in inside:
                        self.prune_tkeys.add(t)
                handled.update(subtree)
                self.report["dropped_staging"].append(
                    f"{name} (duplicate of in-skeleton {name})")
                continue
            # 2) socket match under a hand joint -> re-parent post-walk
            tokens = set()
            for a in chain_of(prop):
                aname = self.go_info.get(a, ("", ""))[0]
                for tok in aname.replace("_", " ").split():
                    tok = "".join(ch for ch in tok.lower() if ch.isalpha())
                    if len(tok) >= 3:
                        tokens.add(tok)
            sockets = []
            for t in inside:
                sname, sactive = self.go_info.get(t, ("", True))
                low = sname.lower()
                if not any(tok in low for tok in tokens):
                    continue
                tchain = chain_of(t)
                tchain_names = [self.go_info.get(a, ("", ""))[0].lower()
                                for a in tchain]
                if low.endswith("hand"):
                    rank = 0
                elif any(k in n for n in tchain_names
                         for k in ("wrst", "wpn", "wrist")):
                    rank = 1
                else:
                    continue
                ntok = sum(1 for tok in tokens if tok in low)
                sockets.append((rank, not sactive, -ntok, t))
            if sockets:
                sockets.sort()
                socket = sockets[0][-1]
                self.staged_parent[prop] = socket
                handled.update(subtree)
                self.report.setdefault("reattach_planned", []).append(
                    f"{name} -> {self.go_info[socket][0]}")
            else:
                self.report.setdefault("staged_unattached", []).append(name)

    def _apply_pending_reparents(self):
        """Move staged props onto their sockets, preserving the authored
        world transform expressed in the socket's rest space (glTF space)."""
        for prop_idx, socket_tkey in self.pending_reparent:
            socket_idx = self.node_of.get(socket_tkey)
            if socket_idx is None:
                self.report.setdefault("staged_unattached", []).append(
                    f"{self.g.nodes[prop_idx].get('name')} (socket missing)")
                continue
            old_parent = self.parent_of.get(prop_idx)
            Gp = self._global_of(socket_idx)
            W = self._global_of(prop_idx)
            L = np.linalg.inv(Gp) @ W
            trs = decompose_trs(L)
            nd = self.g.nodes[prop_idx]
            nd.pop("matrix", None)
            if trs is None:
                for k in ("translation", "rotation", "scale"):
                    nd.pop(k, None)
                nd["matrix"] = [float(x) for x in L.T.reshape(-1)]
            else:
                t, q, s = trs
                nd["translation"] = [float(t[0]), float(t[1]), float(t[2])]
                nd["rotation"] = [float(q[0]), float(q[1]), float(q[2]), float(q[3])]
                nd["scale"] = [float(s[0]), float(s[1]), float(s[2])]
            self.local_M[prop_idx] = L
            self.parent_of[prop_idx] = socket_idx
            if old_parent is not None:
                ch = self.g.nodes[old_parent].get("children", [])
                if prop_idx in ch:
                    ch.remove(prop_idx)
            self.g.nodes[socket_idx].setdefault("children", []).append(prop_idx)
            self.report["reattached_props"].append(
                f"{self.g.nodes[prop_idx].get('name')} -> "
                f"{self.g.nodes[socket_idx].get('name')}")
        self.pending_reparent = []

    # -- textures ----------------------------------------------------------
    def texture_png(self, tex):
        """Return (png_bytes, name) or (None, reason)."""
        try:
            img = tex.image
            buf = io.BytesIO()
            img.save(buf, "PNG")
            return buf.getvalue(), go_name(tex)
        except Exception as e:
            return None, f"{getattr(tex, 'm_Name', '?')}: {type(e).__name__} {e}"

    def get_tex_index(self, tex_ptr):
        if not tex_ptr or tex_ptr.m_PathID == 0:
            return None
        reader = None
        try:
            reader = tex_ptr.deref()
        except Exception:
            return None
        if rkey(reader) in self.tex_cache:
            return self.tex_cache[rkey(reader)]
        tex = reader.parse_as_object()
        png, name = self.texture_png(tex)
        if png is None:
            self.report["texture_errors"].append(name)
            return None
        self.png_dir.mkdir(parents=True, exist_ok=True)
        safe = "".join(c if c.isalnum() or c in "._-" else "_" for c in name)
        (self.png_dir / f"{safe}.png").write_bytes(png)
        idx = self.g.add_texture(png, name)
        self.tex_cache[rkey(reader)] = idx
        return idx

    def tex_luma_stats(self, tex_ptr):
        """(near_black_ratio, near_white_ratio) of a texture, on a 48x48
        thumbnail of its luminance.  Used by the glow-material heuristic;
        cached per texture."""
        if not tex_ptr or tex_ptr.m_PathID == 0:
            return None
        reader = None
        try:
            reader = tex_ptr.deref()
        except Exception:
            return None
        key = rkey(reader)
        if key in self.tex_stats_cache:
            return self.tex_stats_cache[key]
        stats = None
        try:
            img = reader.parse_as_object().image.convert("L").resize((48, 48))
            h = img.histogram()
            n = sum(h)
            stats = (sum(h[:16]) / n, sum(h[240:]) / n)
        except Exception:
            stats = None
        self.tex_stats_cache[key] = stats
        return stats

    # -- materials ---------------------------------------------------------
    # Materials the engine renders with special particle/effect blending
    # (additive sabers, trails, muzzle flashes).  Exported as OPAQUE these
    # turn into solid white slabs (BUGS.md P8), so they are flagged glow and
    # re-exported as alphaMode BLEND with an emissive term.
    GLOW_MAT_NAME = r"(glow|beam|add(itive)?($|_)|trail|slash|bolt|flare|lens|spark|halo|smoke|fire|burn|flash|muzzle|(^|_)vfx(_|$))"
    GLOW_SHADER_NAME = r"(additive|alpha\s*blended|particles|vfx)"

    def shader_name(self, mat):
        ptr = getattr(mat, "m_Shader", None)
        if not ptr or ptr.m_PathID == 0:
            return ""
        try:
            sh = ptr.deref_parse_as_object()
        except Exception:
            return ""
        form = getattr(sh, "m_ParsedForm", None)
        nm = getattr(form, "m_Name", "") if form is not None else ""
        return nm or getattr(sh, "m_Name", "") or ""

    def is_glow_material(self, mat, mat_name, texs):
        import re
        shader = self.shader_name(mat)
        if re.search(self.GLOW_SHADER_NAME, shader, re.I):
            return f"shader:{shader}"
        if re.search(self.GLOW_MAT_NAME, mat_name, re.I):
            return f"name:{mat_name}"
        # additive-gradient texture: mostly near-black with a bright core
        main = texs.get("_MainTex")
        stats = self.tex_luma_stats(main) if main is not None else None
        if stats is not None:
            black, white = stats
            if black >= 0.45 and white >= 0.05:
                return "texture:black+white-gradient"
        return None

    def get_material_index(self, mat_ptr):
        if not mat_ptr or mat_ptr.m_PathID == 0:
            return None
        reader = mat_ptr.deref() if mat_ptr.m_PathID else None
        if reader is None:
            return None
        if rkey(reader) in self.mat_cache:
            return self.mat_cache[rkey(reader)]
        mat = reader.parse_as_object()
        props = mat.m_SavedProperties
        texs = {t[0]: t[1] for t in props.m_TexEnvs}
        cols = {c[0]: c[1] for c in props.m_Colors}
        floats = {f[0]: f[1] for f in props.m_Floats}

        def texptr(name):
            env = texs.get(name)
            return getattr(env, "m_Texture", None) if env is not None else None

        g = self.g
        mat_json = {"name": go_name(mat), "doubleSided": True}
        pbr = {}
        base = cols.get("_Color")
        if base is not None:
            pbr["baseColorFactor"] = [base.r, base.g, base.b, base.a]
        main = self.get_tex_index(texptr("_MainTex"))
        if main is not None:
            pbr["baseColorTexture"] = {"index": main}
        rough = 1.0 - floats.get("_Glossiness", 0.1)
        pbr["metallicFactor"] = floats.get("_Metallic", 0.0)
        pbr["roughnessFactor"] = max(0.0, min(1.0, rough))
        mat_json["pbrMetallicRoughness"] = pbr

        bump = self.get_tex_index(texptr("_BumpMap"))
        if bump is not None:
            mat_json["normalTexture"] = {"index": bump}
        emis_tex = self.get_tex_index(texptr("_EmissionMap"))
        ec = cols.get("_EmissionColor")
        if emis_tex is not None or (ec is not None and (ec.r or ec.g or ec.b)):
            mat_json["emissiveTexture"] = {"index": emis_tex} if emis_tex else None
            mat_json["emissiveFactor"] = [ec.r, ec.g, ec.b] if ec else [1.0, 1.0, 1.0]
            mat_json["emissiveTexture"] = mat_json.get("emissiveTexture")
            if mat_json["emissiveTexture"] is None:
                del mat_json["emissiveTexture"]
        occ = self.get_tex_index(texptr("_OcclusionMap"))
        if occ is not None:
            mat_json["occlusionTexture"] = {"index": occ}
        cutoff = floats.get("_Cutoff")
        if cutoff:
            mat_json["alphaMode"] = "MASK"
            mat_json["alphaCutoff"] = cutoff

        # P8: particle/effect materials must blend, not render as solid slabs.
        ptrs = {k: getattr(v, "m_Texture", None) for k, v in texs.items()}
        glow = self.is_glow_material(mat, go_name(mat), ptrs)
        if glow:
            mat_json["alphaMode"] = "BLEND"
            mat_json.pop("alphaCutoff", None)
            mat_json["doubleSided"] = True
            if "emissiveTexture" not in mat_json and main is not None:
                mat_json["emissiveTexture"] = {"index": main}
            if "emissiveFactor" not in mat_json:
                tint = cols.get("_TintColor")
                if tint is not None and (tint.r or tint.g or tint.b):
                    mat_json["emissiveFactor"] = [tint.r, tint.g, tint.b]
                else:
                    mat_json["emissiveFactor"] = [1.0, 1.0, 1.0]
            self.report.setdefault("glow_materials", []).append(
                f"{go_name(mat)} ({glow})")

        g.materials.append(mat_json)
        idx = len(g.materials) - 1
        self.mat_cache[rkey(reader)] = idx
        return idx

    # -- meshes ------------------------------------------------------------
    def export_mesh_primitive(self, mesh, h, material_idx, n_bones=0):
        g = self.g
        vc = h.m_VertexCount
        attrs = {}
        pos = [c for v in h.m_Vertices for c in conv_v3(v)]
        attrs["POSITION"] = g.add_accessor(pos, "float", vc, "VEC3", minmax=True)
        if h.m_Normals is not None and len(h.m_Normals) == vc:
            nrm = [c for v in h.m_Normals for c in conv_v3(v)]
            attrs["NORMAL"] = g.add_accessor(nrm, "float", vc, "VEC3")
        if h.m_UV0 is not None and len(h.m_UV0) == vc:
            uv = [c for v in h.m_UV0 for c in (v[0], 1.0 - v[1])]
            attrs["TEXCOORD_0"] = g.add_accessor(uv, "float", vc, "VEC2")
        # Baked vertex colors are NOT albedo paint in this game's character
        # shaders (ventress is pale-skinned in game while her baked RGB is
        # ~0.08) — they feed runtime tint masks.  glTF viewers multiply
        # COLOR_0 into baseColor, which renders 149 characters near-black,
        # so we deliberately don't export it.  (m_Colors stay in the cache;
        # re-derive if ever needed.)
        # if h.m_Colors and len(h.m_Colors) == vc:
        #     col = [c for v in h.m_Colors for c in v]
        #     attrs["COLOR_0"] = g.add_accessor(col, "float", vc, "VEC4")
        joints_arr = self._skin_arrays(h, vc, n_bones)
        if joints_arr is not None:
            joints, weights = joints_arr
            attrs["JOINTS_0"] = g.add_accessor(joints.ravel().tolist(), "ubyte", vc, "VEC4")
            attrs["WEIGHTS_0"] = g.add_accessor(weights.ravel().tolist(), "float", vc, "VEC4")

        prim = {"attributes": attrs, "mode": 4}
        if material_idx is not None:
            prim["material"] = material_idx
        return prim

    @staticmethod
    def _skin_arrays(h, vc, n_bones):
        """Return (joints, weights) as (vc,4) arrays, or None when the mesh
        carries no usable skin data.

        Handles meshes with a variable number of influences per vertex
        (m_BoneIndices rows of 1..4 entries) — writing those verbatim would
        leave the VEC4 accessors half-filled with garbage from the buffer.
        """
        if h.m_BoneIndices is None or h.m_BoneWeights is None:
            return None
        bi = np.asarray(h.m_BoneIndices)
        bw = np.asarray(h.m_BoneWeights, dtype=np.float64)
        if bi.size == 0 or bw.shape != bi.shape:
            return None
        k = bi.size // vc
        if k == 0 or bi.size != vc * k:
            return None
        bi = bi.reshape(vc, k)[:, :4]
        bw = bw.reshape(vc, k)[:, :4]
        if k < 4:  # pad missing influence slots
            pad = 4 - k
            bi = np.concatenate([bi, np.zeros((vc, pad), dtype=bi.dtype)], axis=1)
            bw = np.concatenate([bw, np.zeros((vc, pad))], axis=1)

        # fix UnityPy CompressedMesh decompression bug: the 4th weight
        # was computed as (1 - int_sum) instead of (31 - int_sum)/31,
        # yielding large negative weights on some vertices
        bad = (bw < 0).any(axis=1) | (np.abs(bw.sum(axis=1) - 1.0) > 0.02)
        for row in np.nonzero(bad)[0]:
            w = bw[row]
            head = float(w[:3].sum())
            if 0.0 <= head <= 1.0:
                w[3] = 1.0 - head
            else:  # fall back to renormalizing positive weights
                s = w[w > 0].sum() or 1.0
                w[:] = np.where(w > 0, w / s, 0.0)
        bw = np.clip(bw, 0.0, 1.0)

        # joint indices must land inside the renderer's bone list; slots with
        # no weight are parked on joint 0
        if n_bones > 0:
            overflow = (bi >= n_bones) | (bi < 0)
            if overflow.any():
                bw[overflow & (bw > 0)] = 0.0
                bi[overflow] = 0
        # renormalize whatever survived the clamps
        s = bw.sum(axis=1, keepdims=True)
        s[s == 0] = 1.0
        bw /= s
        return bi.astype(np.uint8), bw.astype(np.float32)

    def export_mesh_for_renderer(self, mesh, smr_bones, material_ptrs, mesh_name,
                                 node_idx=None):
        """Build glTF mesh (with skin if bones given). Returns (mesh_idx, skin_idx)."""
        key = (rkey(mesh.object_reader), tuple(id(b) for b in (smr_bones or [])))
        if key in self.mesh_cache:
            return self.mesh_cache[key]

        h = MeshHandler(mesh)
        h.process()
        g = self.g

        # materials per submesh
        mats = [self.get_material_index(p) for p in (material_ptrs or [])]
        triangles = h.get_triangles()
        prims = []
        for si, tri_sub in enumerate(triangles or []):
            mi = mats[si] if si < len(mats) else (mats[0] if mats else None)
            flat = []
            for t in tri_sub:
                if len(t) == 3:
                    a, b, c = t
                    flat += (a, c, b)      # reverse winding after X-mirror
            if not flat:
                continue
            prim = self.export_mesh_primitive(mesh, h, mi, n_bones=len(smr_bones or []))
            prim["indices"] = g.add_indices(flat)
            prims.append(prim)
        if not prims:
            return None, None
        g.meshes.append({"name": mesh_name, "primitives": prims})
        mesh_idx = len(g.meshes) - 1

        # a skin is only valid when the mesh actually carries joint/weight data
        skinned = smr_bones and all("JOINTS_0" in p["attributes"] for p in prims)
        skin_idx = None
        if skinned:
            joints = []
            dummies = set()
            bindposes = getattr(mesh, "m_BindPose", None) or []
            for bi, bp in enumerate(smr_bones):
                treader = None
                try:
                    treader = bp.deref()
                except Exception:
                    pass
                if treader is not None and rkey(treader) in self.node_of:
                    joints.append(self.node_of[rkey(treader)])
                else:
                    # bone transform missing from this prefab: park a helper
                    # node under the mesh node so it stays in the scene graph
                    joints.append(self.dummy_joint_node(bi, node_idx))
                    dummies.add(bi)
            ibm = []
            for bi in range(len(smr_bones)):
                if bi in dummies:
                    # palette identity: vertices stay at mesh-space positions
                    ibm += inv_flat(self._global_of(node_idx))
                elif bi < len(bindposes):
                    ibm += conv_ibm(bindposes[bi])
                else:
                    ibm += [1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1]
            # register bind globals (used only by --bind-pose; see the
            # _snap_all_joints_to_bind docstring for why it is not the default)
            for k in range(len(smr_bones)):
                if k in dummies:
                    continue
                jn = joints[k]
                if jn not in self.bind_globals:
                    M = np.asarray(ibm[16 * k:16 * k + 16], dtype=np.float64).reshape(4, 4).T
                    self.bind_globals[jn] = np.linalg.inv(M)
            ibm_acc = self.g.add_accessor(ibm, "float", len(smr_bones), "MAT4")
            self.g.skins.append({
                "inverseBindMatrices": ibm_acc,
                "joints": joints,
            })
            skin_idx = len(self.g.skins) - 1

        self.mesh_cache[key] = (mesh_idx, skin_idx)
        return mesh_idx, skin_idx

    def _global_of(self, idx):
        """Current global matrix (mirrored space) of a glTF node."""
        M = self.local_M.get(idx)
        if M is None:
            return np.eye(4)
        p = self.parent_of.get(idx)
        return self._global_of(p) @ M if p is not None else M

    def _snap_all_joints_to_bind(self):
        """--bind-pose: overwrite joint rest transforms with bind-pose-derived
        locals, top-down.  NOT the default: many characters are assembled from
        sub-meshes whose bindposes CONFLICT on shared bones (measured up to
        2.0 units across 19/29 bones on princesskneesaa) — for those, the
        prefab rest pose is the only pose where all parts cohere (it is what
        the engine renders at load).  Useful for single-skeleton characters
        whose rest is visibly broken but whose binds agree (krayt dragon)."""
        if not self.bind_globals:
            return
        depth = {}

        def dep(n):
            if n in depth:
                return depth[n]
            p = self.parent_of.get(n)
            depth[n] = (dep(p) + 1) if p is not None else 0
            return depth[n]

        for jn in sorted(self.bind_globals, key=dep):
            p = self.parent_of.get(jn)
            if p in self.bind_globals:
                Gp = self.bind_globals[p]      # parent is a joint: bind space
            elif p is not None:
                Gp = self._global_of(p)
            else:
                Gp = np.eye(4)
            L = np.linalg.inv(Gp) @ self.bind_globals[jn]
            trs = decompose_trs(L)
            if trs is None:
                # shear from non-uniform parent scale: glTF nodes accept a raw
                # matrix, which is still exact — better than skipping
                nd = self.g.nodes[jn]
                nd.pop("translation", None)
                nd.pop("rotation", None)
                nd.pop("scale", None)
                nd["matrix"] = [float(x) for x in L.T.reshape(-1)]
            else:
                t, q, s = trs
                nd = self.g.nodes[jn]
                nd["translation"] = [float(t[0]), float(t[1]), float(t[2])]
                nd["rotation"] = [float(q[0]), float(q[1]), float(q[2]), float(q[3])]
                nd["scale"] = [float(s[0]), float(s[1]), float(s[2])]
            self.local_M[jn] = L

    def dummy_joint_node(self, i, parent=None):
        name = f"__missing_bone_{i}"
        self.g.nodes.append({"name": name})
        idx = len(self.g.nodes) - 1
        self.local_M[idx] = np.eye(4)
        if parent is not None:
            self.parent_of[idx] = parent
            self.g.nodes[parent].setdefault("children", []).append(idx)
        return idx

    # -- hierarchy ---------------------------------------------------------
    def collect_root(self):
        """Find the 'unit.*' root GameObject transform, else any root."""
        best = None
        for obj in self.env.objects:
            if obj.type.name != "Transform":
                continue
            t = obj.parse_as_object()
            father = getattr(t, "m_Father", None)
            if father is None or father.m_PathID == 0:
                go = self.deref(t.m_GameObject)
                n = go_name(go) if go else ""
                if n.startswith("unit."):
                    return t
                best = best or t
        return best

    def walk(self, transform, parent=None, chain_active=True):
        """Depth-first: create glTF nodes for the whole hierarchy.

        Honors the serialized state and the attachment plan from
        _plan_attachments:

        * subtrees marked in prune_tkeys are skipped wholesale;
        * an inactive GO whose subtree holds neither skin bones nor a staged
          prop is dropped together with its subtree (m_IsActive is the
          outfit/loadout truth -- BUGS.md P5/P6/+1);
        * inactive GOs kept only for skinning are stamped with the
          SWGOH_active extension so viewers start them hidden;
        * staged props keep their natural parenting here and are moved onto
          their hand sockets afterwards (_apply_pending_reparents).
        """
        tkey = rkey(transform.object_reader)
        if tkey in self.node_of:
            return self.node_of[tkey]
        if tkey in self.prune_tkeys:
            return None
        name, own_active = self.go_info.get(tkey, (None, True))
        if name is None:
            go = self.deref(transform.m_GameObject)
            if go is not None:
                self.trans_of_go[rkey(go.object_reader)] = tkey
            name = go_name(go) if go else "unnamed"
            own_active = bool(getattr(go, "m_IsActive", 1)) if go is not None else True
        active_chain = chain_active and own_active
        has_bone = self._has_skin_bone(tkey)
        has_staged = tkey in self.staged_parent or any(
            self._has_staged(c) for c in self.children_of.get(tkey, ()))
        spawned_here = tkey in self.spawned_props
        if not active_chain and not has_bone and not has_staged \
                and not spawned_here:
            # P5/P6/+1: what the engine renders at load is the serialized
            # m_IsActive state; a GO that is off (or sits under an off
            # parent) without skin bones, a runtime-attached prop or a
            # spawned weapon stays out of the scene graph entirely.
            self.prune_tkeys.add(tkey)
            self.subtree_bone[tkey] = False
            self.report["dropped_inactive"].append(name)
            return None
        lp = transform.m_LocalPosition
        lr = transform.m_LocalRotation
        ls = transform.m_LocalScale
        node = {"name": name}
        if not own_active:
            # P5: the prefab's authored GameObject m_IsActive IS the
            # outfit-variant truth (helmet on/off, fist vs open hand).
            # Kept nodes (bones / staged-prop carriers) are stamped so
            # viewers start them hidden.
            node["extensions"] = {"SWGOH_active": False}
        if (lp.x, lp.y, lp.z) != (0.0, 0.0, 0.0):
            node["translation"] = [-lp.x, lp.y, lp.z]
        if (lr.x, lr.y, lr.z, lr.w) != (0.0, 0.0, 0.0, 1.0):
            node["rotation"] = [lr.x, -lr.y, -lr.z, lr.w]
        if (ls.x, ls.y, ls.z) != (1.0, 1.0, 1.0):
            node["scale"] = [ls.x, ls.y, ls.z]
        self.g.nodes.append(node)
        idx = len(self.g.nodes) - 1
        self.node_of[tkey] = idx
        go_reader = getattr(transform, "m_GameObject", None)
        if go_reader is not None and go_reader.m_PathID:
            try:
                self.go_of_node[idx] = go_reader.deref()
            except Exception:
                pass
        self.name_of_node[idx] = name
        if not own_active:
            self.inactive_kept.add(idx)
        if spawned_here:
            # spawned weapon: keep the mesh exported (it is the character's
            # loadout) while the inactive stamp stays for consumers
            self.spawned_nodes.add(idx)
            self.inactive_kept.discard(idx)
        if parent is not None:
            self.parent_of[idx] = parent
        L = np.eye(4)
        L[:3, :3] = quat_to_mat((lr.x, -lr.y, -lr.z, lr.w)) * np.array([ls.x, ls.y, ls.z])
        L[:3, 3] = (-lp.x, lp.y, lp.z)
        self.local_M[idx] = L

        if tkey in self.staged_parent:
            # runtime-attached prop: moved onto its hand socket after the
            # walk, authored world pose preserved in the socket rest space.
            # Once attached it defines its own visibility, so its children
            # are walked active regardless of the staging ancestors.
            self.pending_reparent.append((idx, self.staged_parent[tkey]))
            active_chain = True

        children = []
        node_visible = active_chain or has_bone or has_staged
        for ch in transform.m_Children:
            ct = self.deref(ch)
            if ct is not None:
                gltf_parent = idx if node_visible else parent
                c = self.walk(ct, parent=gltf_parent, chain_active=active_chain)
                if c is not None:
                    children.append(c)
        if children:
            node["children"] = children
        return idx

    def _has_staged(self, tk):
        """Subtree rooted at tk contains a runtime-attached prop root."""
        v = self.subtree_staged.get(tk)
        if v is not None:
            return v
        v = tk in self.staged_parent
        if not v:
            for c in self.children_of.get(tk, ()):
                if self._has_staged(c):
                    v = True
                    break
        self.subtree_staged[tk] = v
        return v

    # -- run ---------------------------------------------------------------
    def run(self, root_transform):
        self.png_dir.mkdir(parents=True, exist_ok=True)
        # First pass: build hierarchy nodes for EVERYTHING (so bones resolve),
        # without renderers; second pass attaches meshes.
        self._build_all_nodes(root_transform)
        root_idx = self.node_of.get(rkey(root_transform.object_reader))
        if root_idx is None:
            raise RuntimeError("root transform not indexed")
        if any("SWGOH_active" in n.get("extensions", {}) for n in self.g.nodes):
            self.g.extensionsUsed.append("SWGOH_active")
        glb = self.g.build_glb([root_idx])
        out = self.out_dir / f"{self.char_name}.glb"
        out.write_bytes(glb)
        return out

    def _build_all_nodes(self, root_transform):
        # runtime-attachment plan (staged props / inactive pruning), then
        # walk hierarchy from the character root (bones live inside it)
        self._plan_attachments(rkey(root_transform.object_reader))
        self.walk(root_transform)
        # move staged props onto their hand sockets (authored world pose
        # preserved in the socket rest space)
        self._apply_pending_reparents()
        # then attach renderer meshes
        self._attach_renderers()

    def _particle_go_set(self):
        """GO rkeys that own a ParticleSystem component, extended with every
        descendant GO (BUGS.md P8a).  Particle quads are simulated effects,
        not exportable static meshes -- in-game the ParticleSystemRenderer
        draws them; here they either fail to resolve (built-in Quad mesh,
        path_id 10210) or render as white blobs."""
        ps_gos = set()
        for obj in self.env.objects:
            if obj.type.name != "ParticleSystem":
                continue
            try:
                ps = obj.parse_as_object()
            except Exception:
                continue
            goptr = getattr(ps, "m_GameObject", None)
            if goptr is None or goptr.m_PathID == 0:
                continue
            try:
                ps_gos.add(rkey(goptr.deref()))
            except Exception:
                continue
        if not ps_gos:
            return set()
        out = set()
        for n in range(len(self.g.nodes)):
            gored = self.go_of_node.get(n)
            if gored is None or rkey(gored) not in ps_gos:
                continue
            stack = [n]
            while stack:
                cur = stack.pop()
                if cur in self.go_of_node:
                    out.add(rkey(self.go_of_node[cur]))
                stack.extend(ch for ch in self.g.nodes[cur].get("children", []))
        return out

    def _under_vfx_branch(self, idx):
        """True if the node's chain (self excluded) carries a 'vfx' name."""
        p = self.parent_of.get(idx)
        while p is not None:
            if "vfx" in (self.name_of_node.get(p) or "").lower():
                return True
            p = self.parent_of.get(p)
        return False

    def _attach_renderers(self):
        # map GameObject reader -> renderers
        go_renderers = {}
        for obj in self.env.objects:
            if obj.type.name in ("SkinnedMeshRenderer", "MeshRenderer"):
                try:
                    r = obj.parse_as_object()
                except Exception:
                    continue
                goptr = getattr(r, "m_GameObject", None)
                if goptr is None or goptr.m_PathID == 0:
                    continue
                try:
                    go = goptr.deref()
                except Exception:
                    continue
                go_renderers.setdefault(rkey(go), []).append((go, r))
        # P8a: GOs owning a ParticleSystem + their whole subtree are excluded
        # from mesh export (needs the walk, which built go_of_node).
        particle_gos = self._particle_go_set()
        # Shared mesh packs ship several renderers for the same mesh with
        # different bone-list lengths (e.g. b1_mesh with 21 and 25 bones).
        # boneWeight indices address the mesh's bindpose space, so a renderer
        # whose bone count differs from the bindpose count is an invalid
        # pairing — prefer one that matches.
        best_bones = {}                     # mesh rkey -> bones list
        for obj in self.env.objects:
            if obj.type.name != "SkinnedMeshRenderer":
                continue
            try:
                r = obj.parse_as_object()
                mesh = self.deref(r.m_Mesh)
            except Exception:
                continue
            if mesh is None or not getattr(r, "m_Bones", None):
                continue
            nb = len(getattr(mesh, "m_BindPose", None) or [])
            key = rkey(mesh.object_reader)
            cur = best_bones.get(key)
            if cur is None or abs(len(r.m_Bones) - nb) < abs(len(cur) - nb):
                best_bones[key] = list(r.m_Bones)
        for rs in go_renderers.values():
            go, _ = rs[0]          # ObjectReader from PPtr.deref()
            # parse for the name check: the raw reader has no m_Name
            try:
                name = go_name(go.parse_as_object())
            except Exception:
                continue
            if any(p in name for p in SKIP_GO_NAME_PARTS):
                continue
            treader = self.trans_of_go.get(rkey(go))
            node_idx = self.node_of.get(treader) if treader is not None else None
            if node_idx is None:
                continue
            # P3/P6: a GO the prefab serialized inactive is not rendered --
            # nodes kept only for skinning get no mesh either.
            if node_idx in self.inactive_kept:
                self.report.setdefault("inactive_skipped", []).append(name)
                continue
            # P8a: renderers owned by (or nested under) a ParticleSystem are
            # simulated effects -- never export them as static meshes.
            if rkey(go) in particle_gos:
                self.report.setdefault("particle_excluded", []).append(name)
                continue
            # audit fleet run showed 9 stragglers whose VFX meshes hang under
            # plain GameObjects (vfx_geo_/vfx_cone_...) with no ParticleSystem
            # anywhere up the chain -- the vfx_ name family is conclusive.
            if name.lower().startswith("vfx"):
                self.report.setdefault("particle_excluded", []).append(name + " (vfx-named)")
                continue
            for _, r in rs:
                # a renderer the prefab disabled is not rendered by the engine
                if not getattr(r, "m_Enabled", 1):
                    self.report.setdefault("renderer_disabled_skipped", []).append(name)
                    continue
                rtype = type(r).__name__
                if rtype == "SkinnedMeshRenderer":
                    mesh = self.deref(r.m_Mesh)
                else:  # MeshRenderer pulls its mesh from a sibling MeshFilter
                    mesh = None
                    try:
                        gparsed = go.parse_as_object()
                        for comp in gparsed.m_Component:
                            f = self.deref(comp.component)
                            if type(f).__name__ == "MeshFilter":
                                mesh = self.deref(f.m_Mesh)
                                break
                    except Exception:
                        pass
                if mesh is None:
                    continue
                mname = go_name(mesh)
                if any(p in mname for p in SKIP_GO_NAME_PARTS) or "shadowmesh" in mname.lower():
                    continue
                # trail/beam quads inside a vfx branch are effect geometry
                if self._under_vfx_branch(node_idx) and \
                        any(p in mname.lower() for p in ("trail", "beam")):
                    self.report.setdefault("particle_excluded", []).append(
                        f"{name} ({mname})")
                    continue
                if rtype == "SkinnedMeshRenderer":
                    bones = (best_bones.get(rkey(mesh.object_reader))
                             or list(getattr(r, "m_Bones", []) or []))
                else:
                    bones = None
                mname = go_name(mesh)
                try:
                    mesh_idx, skin_idx = self.export_mesh_for_renderer(
                        mesh, bones, list(r.m_Materials), mname, node_idx=node_idx)
                except Exception as e:
                    import traceback
                    self.report["mesh_errors"].append(
                        f"{mname}: {type(e).__name__} {e}\n"
                        + traceback.format_exc())
                    continue
                if mesh_idx is None:
                    continue
                nd = self.g.nodes[node_idx]
                nd["mesh"] = mesh_idx
                if skin_idx is not None:
                    nd["skin"] = skin_idx
                self.report["exported_meshes"].append(
                    mname + (f" [skinned x{len(bones)}]" if bones else " [static]"))
        if self.bind_pose_mode:
            # opt-in: settle the whole skeleton into bind pose
            self._snap_all_joints_to_bind()


# --------------------------------------------------------------------------

def extract(bundle_name: str, bind_pose: bool = False):
    folder = CACHE / bundle_name
    bundle = newest_bundle(folder)
    if bundle is None:
        print(f"  !! bundle not found: {bundle_name}")
        return False
    print(f"[{bundle_name}] loading {bundle}")
    files = resolve_deps_recursively(bundle)
    env = UnityPy.load(*[str(f) for f in files])

    out_dir = OUT_ROOT / bundle_name
    out_dir.mkdir(parents=True, exist_ok=True)
    report = {"exported_meshes": [], "mesh_errors": [], "texture_errors": [],
              "particle_excluded": [], "renderer_disabled_skipped": [],
              "glow_materials": [],
              "dropped_inactive": [], "dropped_staging": [],
              "reattached_props": []}

    exp = CharacterExporter(env, bundle_name, out_dir, report)
    exp.bind_pose_mode = bind_pose
    try:
        root = exp.collect_root()
        if root is None:
            print("  !! no root transform found")
            return False
        glb_path = exp.run(root)
        (out_dir / "report.json").write_text(
            json.dumps(report, indent=2, ensure_ascii=False), encoding="utf-8")
        print(f"  -> {glb_path}  ({glb_path.stat().st_size/1024:.0f} KB)")
        print(f"     meshes: {len(report['exported_meshes'])}, "
              f"errors: {len(report['mesh_errors'])} mesh / "
              f"{len(report['texture_errors'])} tex")
        if report.get("particle_excluded"):
            print(f"     particle-excluded renderers: {report['particle_excluded']}")
        if report.get("glow_materials"):
            print(f"     glow (BLEND) materials: {report['glow_materials']}")
        for m in report["exported_meshes"]:
            print(f"       - {m}")
        if report["mesh_errors"] or report["texture_errors"]:
            for e in (report["mesh_errors"] + report["texture_errors"])[:10]:
                print(f"       ! {e}")
        return True
    finally:
        del env
        gc.collect()


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("cmd", choices=["list", "extract", "extract-all"])
    ap.add_argument("names", nargs="*")
    ap.add_argument("--only", default="char",
                    help="extract-all filter prefix (default: char)")
    ap.add_argument("--bind-pose", action="store_true",
                    help="snap the skeleton to the mesh bind pose instead of "
                         "keeping the prefab rest pose (see source docs)")
    args = ap.parse_args()

    if args.cmd == "list":
        list_bundles()
    elif args.cmd == "extract":
        if not args.names:
            print("specify bundle folder name(s); use 'list' to see them")
            sys.exit(1)
        ok = all(extract(n, bind_pose=args.bind_pose) for n in args.names)
        sys.exit(0 if ok else 1)
    elif args.cmd == "extract-all":
        folders = sorted(p.name for p in CACHE.iterdir()
                         if p.is_dir() and p.name.startswith(args.only)
                         and newest_bundle(p))
        print(f"extracting {len(folders)} bundles (filter: {args.only})")
        done = 0
        for i, n in enumerate(folders, 1):
            print(f"=== {i}/{len(folders)} ===")
            try:
                if extract(n):
                    done += 1
            except Exception as e:
                print(f"  !! FAILED: {type(e).__name__}: {e}")
        print(f"done: {done}/{len(folders)}")


if __name__ == "__main__":
    main()
