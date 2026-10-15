# -*- coding: utf-8 -*-
"""Programmatic acceptance checks for BUGS.md P5 / P7 / P8 fixes.

Per character bundle:

  P5  (1) every GameObject the prefab authored with m_IsActive=0 is HIDDEN
          in the GLB: stamped `extensions.SWGOH_active=false` when the node
          is kept, or ABSENT entirely when the attachment planner dropped
          the subtree (hidden means ABSENT, not flagged -- e.g. jynerso's
          unselected swd_jynerso_mesh loadout variant under its active
          pistol).  Every absence must be accounted for by a report.json
          dropped_inactive entry, so an accidental drop cannot masquerade
          as a deliberate one;
  P8  (2) zero ParticleSystem-derived meshes: no GLB mesh node sits on a
          ParticleSystem GameObject or under one (the particle GO names are
          listed from the bundle for cross-checking);
      (4) every glow material (particle/additive shader, glow/trail/beam/vfx
          name, or mostly-black+white gradient texture) that survives into
          the GLB has alphaMode BLEND;
  P7  (3) cape/cloth-weighted vertices -- at rest AND at t=0 of the first
          animation -- stay within dislocation range of their anchors, AND
          the exported cloth-joint animation channels equal the source
          clip's curves (binding fidelity).  The old 0.35 body-AABB gate
          was calibrated on the injured idle, whose cape hugs the body
          (<=0.22); the real idle (vaderduelsend Idle@1.0) legitimately
          drapes its cape up to ~0.94 from the body AABB over the whole
          cycle -- and its t=0 pose IS the authored rest pose (147 verts /
          max 0.76 measured identically at rest and t=0), while source vs
          GLB agree to float32 precision (3.6e-05).  Distance alone cannot
          separate that from the pre-fix offset bug (~0.84), so binding
          truth is enforced by re-sampling the source clip's cloth curves
          at t=0 (catalog/char_anim_states.json resolves the clip) and
          comparing against the GLB channels.

Run:  py tools/verify_variants_vfx.py [char_a_pre ...]
      (default: the four acceptance bundles)
      py tools/verify_variants_vfx.py --regression-probe [char ...]
      (re-injects the pre-fix foreign-curve bug into a copy of the anim
       GLB and asserts the P7 gate FAILS -- the regression drill)
"""
import json
import os
import re
import struct
import sys
import zlib

import numpy as np

TOOLS = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(TOOLS)
sys.path.insert(0, TOOLS)

from UnityPy import load as unity_load
from export_animation import latest_bundle, conv_q, conv_v3, q_norm
from muscle_clip import parse_animation_clip, binding_dimensions, sample_curve

COMP_T = {5126: "f4", 5123: "u2", 5125: "u4", 5121: "u1"}
NCOMP = {"SCALAR": 1, "VEC2": 2, "VEC3": 3, "VEC4": 4, "MAT4": 16}

GLOW_MAT_NAME = re.compile(r"(glow|beam|add(itive)?($|_)|trail|slash|bolt|flare|lens|spark|halo|smoke|fire|burn|flash|muzzle|(^|_)vfx(_|$))", re.I)
GLOW_SHADER_NAME = re.compile(r"(additive|alpha\s*blended|particles|vfx)", re.I)

CAPE_PAT = ("cape", "cloak")


# ---------------------------------------------------------------- GLB utils

def load_glb(path):
    d = open(path, "rb").read()
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
        m = n["matrix"]
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


def node_globals(g, overrides=None):
    L = {}
    for i, n in enumerate(g["nodes"]):
        M = node_matrix(n)
        if overrides:
            for prop, mod in overrides.get(i, {}).items():
                mod(M)
        L[i] = M
    G = {}

    def rec(nid, parent):
        M = L[nid]
        G[nid] = (parent @ M) if parent is not None else M
        for c in g["nodes"][nid].get("children", []):
            rec(c, G[nid])

    for r in g["scenes"][g.get("scene", 0)]["nodes"]:
        rec(r, None)
    return G


def glb_node_paths(g):
    """{node_index: path} using '/' like Unity transform paths."""
    out = {}

    def rec(nid, prefix):
        name = g["nodes"][nid].get("name", "?")
        path = f"{prefix}/{name}" if prefix else name
        out[nid] = path
        for c in g["nodes"][nid].get("children", []):
            rec(c, path)

    for r in g["scenes"][g.get("scene", 0)]["nodes"]:
        rec(r, "")
    return out


def sample_anim_overrides(g, d, bin_off, anim, t):
    """{node: {prop: modifier}} sampling one animation at time t."""
    over = {}
    for ch in anim["channels"]:
        tgt = ch["target"]["node"]
        prop = ch["target"]["path"]
        s = anim["samplers"][ch["sampler"]]
        times = accessor(g, d, bin_off, s["input"]).astype(np.float64)
        vals = accessor(g, d, bin_off, s["output"])
        if t <= times[0]:
            v = vals[0]
        elif t >= times[-1]:
            v = vals[-1]
        else:
            i = int(np.searchsorted(times, t, side="right") - 1)
            frac = (t - times[i]) / (times[i + 1] - times[i])
            v = vals[i] * (1 - frac) + vals[i + 1] * frac
        v = np.asarray(v, dtype=np.float64)

        def apply(M, prop=prop, v=v):
            if prop == "translation":
                M[:3, 3] = v
            elif prop == "scale":
                cur = np.linalg.norm(M[:3, :3], axis=0) + 1e-12
                M[:3, :3] = M[:3, :3] / cur * v
            elif prop == "rotation":
                x, y, z, w = v
                R = np.array([
                    [1 - 2 * (y * y + z * z), 2 * (x * y - z * w), 2 * (x * z + y * w)],
                    [2 * (x * y + z * w), 1 - 2 * (x * x + z * z), 2 * (y * z - x * w)],
                    [2 * (x * z - y * w), 2 * (y * z + x * w), 1 - 2 * (x * x + y * y)]])
                s = np.linalg.norm(M[:3, :3], axis=0)
                M[:3, :3] = R * s

        over.setdefault(tgt, {})[prop] = apply
    return over


# ------------------------------------------------------------- source side

def source_state(bundle):
    """From the prefab: authored-inactive GO path set, ParticleSystem GO
    paths (+descendant paths), glow material names, walk-root name."""
    env = unity_load(latest_bundle(bundle))
    _U64 = 0xFFFFFFFFFFFFFFFF

    def u64(p):
        return int(p) & _U64

    gos, trans = {}, {}
    ps_goids = set()
    for o in env.objects:
        tn = o.type.name
        if tn == "GameObject":
            try:
                gos[u64(o.path_id)] = o.read_typetree()
            except Exception:
                pass
        elif tn == "Transform":
            try:
                trans[u64(o.path_id)] = o.read_typetree()
            except Exception:
                pass
        elif tn == "ParticleSystem":
            try:
                ps_goids.add(u64(o.read_typetree()["m_GameObject"]["m_PathID"]))
            except Exception:
                pass

    t_go = {}
    kids = {}
    father = {}
    for pid, t in trans.items():
        t_go[pid] = u64(t["m_GameObject"]["m_PathID"])
        kids[pid] = [u64(c["m_PathID"]) for c in t.get("m_Children", [])]
        f = u64(t.get("m_Father", {}).get("m_PathID", 0))
        if f:
            father[pid] = f

    roots = [pid for pid in trans if pid not in father]
    root = next((r for r in roots
                 if gos.get(t_go.get(r), {}).get("m_Name", "").startswith("unit.")),
                roots[0] if roots else None)
    if root is None:
        return None

    inactive, ps_paths = set(), set()

    def rec(pid, prefix):
        go = gos.get(t_go.get(pid), {})
        name = go.get("m_Name", "?")
        path = f"{prefix}/{name}" if prefix else name
        if not go.get("m_IsActive", 1):
            inactive.add(path)
        if t_go.get(pid) in ps_goids:
            ps_paths.add(path)
        for c in kids.get(pid, []):
            rec(c, path)

    rec(root, "")
    # PS descendants: paths under any ps path
    ps_with_desc = {p for p in ps_paths}
    for p in list(ps_paths):
        ps_with_desc |= {q for q in (inactive | set()) if q.startswith(p + "/")}
    # descendant closure over ALL names: rebuild by prefix
    all_paths = set()

    def rec2(pid, prefix):
        go = gos.get(t_go.get(pid), {})
        name = go.get("m_Name", "?")
        path = f"{prefix}/{name}" if prefix else name
        all_paths.add(path)
        for c in kids.get(pid, []):
            rec2(c, path)

    rec2(root, "")
    ps_with_desc = {q for q in all_paths
                    if any(q == p or q.startswith(p + "/") for p in ps_paths)}

    # glow materials (same rules as the exporter)
    glow_mats = set()
    tex_blacklist = {}
    for o in env.objects:
        if o.type.name == "Texture2D":
            try:
                img = o.read().image.convert("L").resize((48, 48))
                h = img.histogram()
                n = sum(h) or 1
                tex_blacklist[u64(o.path_id)] = (sum(h[:16]) / n, sum(h[240:]) / n)
            except Exception:
                pass
    shaders = {}
    for o in env.objects:
        if o.type.name == "Shader":
            try:
                t = o.read_typetree()
                nm = (t.get("m_ParsedForm", {}) or {}).get("m_Name") or t.get("m_Name", "")
                shaders[u64(o.path_id)] = nm
            except Exception:
                pass
    for o in env.objects:
        if o.type.name != "Material":
            continue
        try:
            t = o.read_typetree()
        except Exception:
            continue
        name = t.get("m_Name", "?")
        sh = shaders.get(u64(t.get("m_Shader", {}).get("m_PathID", 0)), "")
        hit = None
        if GLOW_SHADER_NAME.search(sh):
            hit = f"shader:{sh}"
        elif GLOW_MAT_NAME.search(name):
            hit = f"name:{name}"
        else:
            for tname, tenv in (t.get("m_SavedProperties", {}).get("m_TexEnvs", []) or []):
                if tname == "_MainTex":
                    tid = u64(tenv.get("m_Texture", {}).get("m_PathID", 0))
                    st = tex_blacklist.get(tid)
                    if st and st[0] >= 0.45 and st[1] >= 0.05:
                        hit = "texture:black+white-gradient"
                    break
        if hit:
            glow_mats.add(name)
    return {"inactive": inactive, "ps_paths": ps_with_desc,
            "glow_mats": glow_mats, "root": gos.get(t_go[root], {}).get("m_Name", "?")}


# ------------------------------------------------------------------ checks

def check_p5_flags(g, paths, src, dropped_names=()):
    """P5: authored-inactive GOs must be HIDDEN in the GLB.

    A kept node carries extensions.SWGOH_active=false; a node the attachment
    planner dropped (unselected loadout variant, dead outfit branch) is
    hidden by ABSENCE -- but every absence must be accounted for by a
    report.json dropped_inactive entry (walk() records each dropped subtree
    root by name), so an accidental subtree loss cannot pose as deliberate.

    Returns (ok, missing, extra, unrecorded)."""
    marked = {paths[i] for i, n in enumerate(g["nodes"])
              if (n.get("extensions", {}) or {}).get("SWGOH_active") is False}
    present = set(paths.values())
    # present in the GLB but not stamped -> real P5 violation
    missing = {p for p in src["inactive"] if p in present} - marked
    # stamped but authored active -> viewer hides live geometry
    extra = marked - src["inactive"]
    # absent: legitimate iff some path segment (the dropped subtree root or
    # one of its ancestors) is a recorded dropped_inactive name
    absent = src["inactive"] - present
    unrecorded = {p for p in absent
                  if not any(seg in dropped_names for seg in p.split("/"))}
    ok = not missing and not extra and not unrecorded
    return ok, missing, extra, unrecorded


def check_p8_particles(g, paths, src):
    offenders = [paths[i] for i, n in enumerate(g["nodes"])
                 if "mesh" in n and paths[i] in src["ps_paths"]]
    return not offenders, offenders, sorted(p for p in src["ps_paths"])[:40]


def check_p8_blend(g, src):
    bad = []
    used = {prim.get("material") for m in g.get("meshes", [])
            for prim in m.get("primitives", [])}
    for i, m in enumerate(g.get("materials", [])):
        if m.get("name") in src["glow_mats"] and i in used:
            if m.get("alphaMode") != "BLEND":
                bad.append((m.get("name"), m.get("alphaMode", "OPAQUE")))
    return not bad, bad


def _skin_sample(g, d, bin_off, node, G):
    prim = g["meshes"][node["mesh"]]["primitives"][0]
    if "JOINTS_0" not in prim["attributes"]:
        return None
    P = accessor(g, d, bin_off, prim["attributes"]["POSITION"]).astype(np.float64)
    J = accessor(g, d, bin_off, prim["attributes"]["JOINTS_0"]).astype(int)
    W = accessor(g, d, bin_off, prim["attributes"]["WEIGHTS_0"]).astype(np.float64)
    skin = g["skins"][node["skin"]]
    jnames = [g["nodes"][j].get("name", "?") for j in skin["joints"]]
    raw = accessor(g, d, bin_off, skin["inverseBindMatrices"]).astype(np.float64).reshape(-1, 4, 4)
    IBM = np.ascontiguousarray(raw.transpose(0, 2, 1))
    Jm = np.stack([G[j] for j in skin["joints"]])
    Pal = Jm @ IBM
    Pv = np.einsum("vk,vkij->vij", W, Pal[J])
    hom = np.hstack([P, np.ones((len(P), 1))])
    sk = np.einsum("vij,vj->vi", Pv, hom)[:, :3]
    cape_joints = {i for i, nm in enumerate(jnames)
                   if any(p in nm.lower() for p in CAPE_PAT)}
    cape_w = np.zeros(len(P))
    if cape_joints:
        for k in range(W.shape[1]):
            cape_w += np.where(np.isin(J[:, k], list(cape_joints)), W[:, k], 0.0)
    dominant = np.array([jnames[J[vi, int(np.argmax(W[vi]))]] for vi in range(len(P))])
    is_cape_dom = np.array([any(p in d.lower() for p in CAPE_PAT) for d in dominant])
    cape_sel = (cape_w > 0.3) | is_cape_dom
    return P, sk, cape_sel


def _box_distance(pts, bmin, bmax):
    """Distance of each point to the AABB (0 when inside)."""
    d = np.zeros(len(pts))
    for a in range(3):
        d += np.maximum(bmin[a] - pts[:, a], 0) + np.maximum(pts[:, a] - bmax[a], 0)
    return d


def check_p7_cape(g, d, bin_off, char, src_clip=None):
    """BUGS.md P7 invariant, per skinned mesh carrying cape/cloth joints:

      * every cape joint must resolve to a real skeleton node (no
        __missing_bone dummies -- a dropped bone parks its vertices on a
        helper node away from the body);
      * REST sanity: the skinned cape stays within 1.0 of its OWN authored
        volume.  Mesh vertices are authored in bind space, so bind-vs-rest
        pose drift is legitimate and measured up to 0.76 on healthy rigs;
        the threshold only flags dislocation-scale breaks (a parked dummy
        palette throws verts units away);
      * ANIM t=0 (the task's acceptance criterion), two layers:
          - geometric: cape within 1.0 of the body AABB sampled at the SAME
            t.  The old 0.35 gate was calibrated on the injured idle whose
            cape hugs the body (<=0.22); the character's real idle
            (vaderduelsend Idle@1.0) legitimately drapes the cape 0.53-0.94
            from the body AABB over the WHOLE cycle, and its t=0 pose IS the
            authored rest pose (147 verts / max 0.76 measured identically at
            rest and t=0).  The threshold therefore only flags
            dislocation-scale breaks, like REST_TOL;
          - binding fidelity: the exported cloth-joint channels at t=0 must
            EQUAL the source clip's curves re-sampled from the bundle
            (vaderduelsend Idle@1.0 vs its GLB: max abs diff 3.6e-05 ==
            float32 quantization).  This is the gate that actually catches
            the pre-fix bug (curve offsets read from foreign bones,
            ~0.84 excursion) -- distance alone cannot separate it from the
            legitimate 0.94 idle swing.
    """
    REST_TOL = 1.0
    ANIM_TOL = 1.0
    anims = g.get("animations", [])
    scenarios = [("rest", None)]
    if anims:
        scenarios.append((f"t0:{anims[0].get('name')}", anims[0]))
    ok = True
    detail = []
    cape_seen = False
    for label, anim in scenarios:
        over = None
        if anim is not None:
            over = sample_anim_overrides(g, d, bin_off, anim, 0.0)
        G = node_globals(g, over)
        for n in g["nodes"]:
            if "mesh" not in n or "skin" not in n:
                continue
            s = _skin_sample(g, d, bin_off, n, G)
            if s is None:
                continue
            P, sk, cape_sel = s
            if not cape_sel.any():
                continue
            cape_seen = True
            skin = g["skins"][n["skin"]]
            dummies = [g["nodes"][j].get("name", "?") for j in skin["joints"]
                       if "missing_bone" in str(g["nodes"][j].get("name"))]
            cape = sk[cape_sel]
            if anim is None:
                dist = _box_distance(cape, P[cape_sel].min(axis=0),
                                     P[cape_sel].max(axis=0))
                outside = int((dist > REST_TOL).sum())
                this_ok = outside == 0 and not dummies
                detail.append(
                    f"{label} {g['meshes'][n['mesh']].get('name')}: "
                    f"cape verts={int(cape_sel.sum())} beyond {REST_TOL} of authored "
                    f"volume: {outside} (max {float(dist.max()):.2f})"
                    + (f" DUMMY JOINTS: {dummies}" if dummies else ""))
            else:
                body = sk[~cape_sel]
                if len(body):
                    dist = _box_distance(cape, body.min(axis=0), body.max(axis=0))
                    outside = int((dist > ANIM_TOL).sum())
                    this_ok = outside == 0
                    detail.append(
                        f"{label} {g['meshes'][n['mesh']].get('name')}: "
                        f"cape verts={int(cape_sel.sum())} beyond {ANIM_TOL} of body: "
                        f"{outside} (max {float(dist.max()):.2f})")
            ok &= this_ok
        if anim is not None and src_clip is not None:
            fok, checked, bad, ambiguous, noch, mm = \
                check_p7_binding_fidelity(g, d, bin_off, anim, src_clip)
            ok &= fok
            detail.append(
                f"{label} binding fidelity: {checked} cloth curves vs source, "
                f"{bad} mismatched"
                + (f" ({ambiguous} ambiguous multi-binding, {noch} without GLB channel)"
                   if ambiguous or noch else "")
                + (f" e.g. {mm[:2]}" if mm else ""))
    if not cape_seen:
        return True, ["no cape/cloth-weighted vertices in this character"]
    return ok, detail


# ------------------------------------------------- P7 binding fidelity gate

CLOTH_PAT = CAPE_PAT + ("cloth",)


def crc_node_registry(g):
    """{crc32(subtree path): node_index} over every node as base -- the same
    enumeration the exporter's binding matcher (crc_paths) uses."""
    out = {}
    for base in range(len(g["nodes"])):
        stack = [(base, [])]
        while stack:
            i, prefix = stack.pop()
            name = g["nodes"][i].get("name", "")
            path = "/".join(prefix + [name])
            out.setdefault(zlib.crc32(path.encode()) & 0xFFFFFFFF, i)
            stack.extend((ch, prefix + [name])
                         for ch in g["nodes"][i].get("children", []))
    return out


def resolve_source_clip(char, anim_name):
    """Parse the bundle's AnimationClip behind a GLB animation name via the
    states catalog ('Idle@1.0' -> state 'Idle', blend-tree threshold 1.0).
    Returns the parsed clip dict, or None when unresolvable (fidelity then
    skips with an info note)."""
    spath = os.path.join(ROOT, "catalog", "char_anim_states.json")
    if not os.path.exists(spath):
        return None
    entry = json.load(open(spath, encoding="utf-8")).get(char)
    states = (entry or {}).get("states") or []
    base = (anim_name or "").split(" (")[0]
    label, _, thr = base.partition("@")
    st = next((s for s in states
               if s["name"].split(".")[-1].strip() == label), None)
    if st is None:
        return None
    motions = [m for m in st["motion"] if m.get("path_id")]
    if thr:
        motions = [m for m in motions
                   if abs(float(m.get("threshold") or 0.0) - float(thr)) < 1e-6]
    if not motions:
        return None
    m = motions[0]
    src = str(m["source"])
    bundle = "shared_animationcontainer" if (
        src == "shared_animationcontainer" or src.startswith("ext")) else char
    env = unity_load(latest_bundle(bundle))
    pid = int(m["path_id"]) & 0xFFFFFFFFFFFFFFFF
    for o in env.objects:
        if o.type.name == "AnimationClip" and \
                (int(o.path_id) & 0xFFFFFFFFFFFFFFFF) == pid:
            return parse_animation_clip(o.get_raw_data())
    return None


def channel_t0_value(g, d, bin_off, anim, node, prop):
    """Raw GLB channel value at t=0 (first key; build_animation always
    starts at 0), or None when the animation has no such channel."""
    for ch in anim["channels"]:
        if ch["target"]["node"] == node and ch["target"]["path"] == prop:
            s = anim["samplers"][ch["sampler"]]
            times = accessor(g, d, bin_off, s["input"]).astype(np.float64)
            vals = accessor(g, d, bin_off, s["output"]).astype(np.float64)
            vals = vals.reshape(times.shape[0], -1)
            return vals[0]
    return None


def check_p7_binding_fidelity(g, d, bin_off, anim, clip):
    """The P7 regression gate: every cloth-family (cape/cloak/cloth)
    transform-layer source binding that resolves to a GLB node must carry
    the SOURCE clip's t=0 value in the GLB's t=0 channel -- exactly the
    analytical method that cleared Idle@1.0 as faithful.  Weapon/prop
    bindings stay in verify_attachments' scope; multiple source bindings
    converging on one (node, prop) with diverging values are ambiguous and
    skipped (the exporter's (node,prop) dedupe legitimately picks one)."""
    cmap = crc_node_registry(g)
    groups = {}          # (node, prop) -> {rounded source t0 tuples}
    acc = 0
    for b, dd in zip(clip["bindings"]["generic_bindings"],
                     binding_dimensions(clip)):
        curves = list(range(acc, acc + dd))
        acc += dd
        if b["type_id"] != 4 or b["attribute"] not in (1, 2):
            continue
        node = cmap.get(b["path"] & 0xFFFFFFFF)
        if node is None:
            continue
        leaf = g["nodes"][node].get("name", "").lower()
        if not any(p in leaf for p in CLOTH_PAT):
            continue
        if b["attribute"] == 1:
            v = conv_v3([sample_curve(clip, c, 0.0) for c in curves])
        else:
            v = conv_q(q_norm(tuple(sample_curve(clip, c, 0.0) for c in curves)))
        prop = "translation" if b["attribute"] == 1 else "rotation"
        groups.setdefault((node, prop), set()).add(
            tuple(round(float(x), 6) for x in v))
    checked = bad = ambiguous = noch = 0
    mismatches = []
    for (node, prop), src_vals in groups.items():
        if len(src_vals) > 1:
            ambiguous += 1
            continue
        got = channel_t0_value(g, d, bin_off, anim, node, prop)
        if got is None:
            noch += 1
            continue
        src = np.asarray(next(iter(src_vals)), dtype=np.float64)
        checked += 1
        if float(np.abs(src - got).max()) > 5e-4:
            bad += 1
            mismatches.append((g["nodes"][node].get("name"), prop,
                               [round(float(x), 4) for x in src],
                               [round(float(x), 4) for x in got]))
    return bad == 0, checked, bad, ambiguous, noch, mismatches


def regression_probe(char):
    """P7 regression drill: re-inject the pre-fix bug (cloth joints receive
    a FOREIGN node's curves -- the curve-offset desync) into a copy of the
    anim GLB and assert the gate FAILS, while the authentic export PASSES.
    Returns True when the gate still discriminates."""
    base = os.path.join(ROOT, "extracted", char)
    anim_path = os.path.join(base, f"{char}.anim.glb")
    if not os.path.exists(anim_path):
        print(f"  {char}: [probe skip] no anim GLB")
        return True
    ga, da, offa = load_glb(anim_path)
    anims = ga.get("animations", [])
    if not anims:
        print(f"  {char}: [probe skip] no animations")
        return True
    src_clip = resolve_source_clip(char, anims[0].get("name", ""))
    ok_auth, _ = check_p7_cape(ga, da, offa, char, src_clip)

    import copy
    g2 = copy.deepcopy(ga)
    anim2 = g2["animations"][0]
    cloth = {i for i, n in enumerate(g2["nodes"])
             if any(p in n.get("name", "").lower() for p in CLOTH_PAT)}
    donors = {}
    for ch in anim2["channels"]:
        if ch["target"]["node"] not in cloth:
            donors.setdefault(ch["target"]["path"], ch["sampler"])
    injected = 0
    for ch in anim2["channels"]:
        if ch["target"]["node"] in cloth and ch["target"]["path"] in donors:
            ch["sampler"] = donors[ch["target"]["path"]]
            injected += 1
    ok_inj, det_inj = check_p7_cape(g2, da, offa, char, src_clip)
    if injected == 0:
        # no cloth-family joints at all: the P7 drill does not apply
        print(f"  {char}: [probe skip] authentic PASS; no cloth joints to "
              f"inject (P7 N/A)")
        return ok_auth
    passed = ok_auth and not ok_inj
    verdict = "FAIL (gate holds)" if not ok_inj else "PASS (GATE BROKEN)"
    print(f"  {char}: [probe] authentic {'PASS' if ok_auth else 'FAIL'}; "
          f"injected {injected} foreign curves -> {verdict}"
          + (f"  [{det_inj[-1]}]" if det_inj else ""))
    return passed


def verify(char):
    print(f"\n=== {char} ===")
    fails = []
    base = os.path.join(ROOT, "extracted", char)
    glb_path = os.path.join(base, f"{char}.glb")
    if not os.path.exists(glb_path):
        print("  [FAIL] no static GLB exported")
        return False
    g, d, bin_off = load_glb(glb_path)
    paths = glb_node_paths(g)

    src = source_state(char)
    if src is None:
        print("  [FAIL] no root transform in bundle")
        return False

    # dropped_inactive names recorded by the exporter's walk() -- an
    # authored-inactive GO absent from the GLB must be covered by one of
    # these (hidden means ABSENT, but only a RECORDED absence)
    dropped_names = set()
    rep_path = os.path.join(base, "report.json")
    if os.path.exists(rep_path):
        try:
            with open(rep_path, encoding="utf-8") as fh:
                dropped_names = set(json.load(fh).get("dropped_inactive", []))
        except Exception as e:
            print(f"  [warn] unreadable report.json: {e}")

    ok, missing, extra, unrecorded = check_p5_flags(g, paths, src, dropped_names)
    if ok:
        n_stamped = sum(1 for n in g["nodes"]
                        if (n.get("extensions", {}) or {}).get("SWGOH_active") is False)
        print(f"  [PASS] P5 flags: {len(src['inactive'])} authored-inactive nodes "
              f"({n_stamped} stamped hidden, "
              f"{len(src['inactive']) - n_stamped} absent = dropped, recorded) "
              f"(viewer parts panel defaults follow)")
    else:
        fails.append("P5 flags")
        print(f"  [FAIL] P5 flags: missing={sorted(missing)[:8]} "
              f"extra={sorted(extra)[:8]} "
              f"absent-unrecorded={sorted(unrecorded)[:8]}")

    ok, offenders, ps_names = check_p8_particles(g, paths, src)
    if ok:
        print(f"  [PASS] P8 particles: 0 particle-derived meshes "
              f"(bundle has {len([p for p in src['ps_paths'] if '/' not in p]) or len(src['ps_paths'])} PS paths, "
              f"e.g. {ps_names[:4]})")
    else:
        fails.append("P8 particles")
        print(f"  [FAIL] P8 particles exported: {offenders[:8]}")

    ok, bad = check_p8_blend(g, src)
    if ok:
        print(f"  [PASS] P8 blend: {len(src['glow_mats'] & {m.get('name') for m in g.get('materials', [])})} "
              f"glow materials present, all alphaMode=BLEND")
    else:
        fails.append("P8 blend")
        print(f"  [FAIL] P8 blend: {bad}")

    ok, detail = check_p7_cape(g, d, bin_off, char)
    print(f"  [{'PASS' if ok else 'FAIL'}] P7 cape containment: {'; '.join(detail)}")
    if not ok:
        fails.append("P7 cape")

    anim_path = os.path.join(base, f"{char}.anim.glb")
    if os.path.exists(anim_path):
        ga, da, offa = load_glb(anim_path)
        src_clip = None
        if ga.get("animations"):
            src_clip = resolve_source_clip(char, ga["animations"][0].get("name", ""))
            if src_clip is None:
                print("  [info] source clip not resolvable from states catalog "
                      "-- binding fidelity skipped")
        ok, detail = check_p7_cape(ga, da, offa, char, src_clip)
        print(f"  [{'PASS' if ok else 'FAIL'}] P7 cape containment (anim t=0): {'; '.join(detail)}")
        if not ok:
            fails.append("P7 cape (anim)")
    else:
        print("  [info] no anim GLB -- static only")

    return not fails


DEFAULT_CHARS = [
    "char_jynerso_pre",               # saber glow / hidden blaster (P5+P8)
    "char_vaderduelsend_pre",         # outfit variants (P5)
    "char_anakin_gl_remastered_pre",  # Lord Vader cape (P7)
    "char_vader_pre",                 # dedicated cape bones (P7)
]

if __name__ == "__main__":
    chars = [a for a in sys.argv[1:] if not a.startswith("--")] or DEFAULT_CHARS
    probe = "--regression-probe" in sys.argv[1:]
    all_ok = True
    for c in chars:
        if probe:
            try:
                all_ok &= regression_probe(c)
            except Exception as e:
                import traceback
                traceback.print_exc()
                print(f"  [FAIL] {c}: probe error {type(e).__name__}: {e}")
                all_ok = False
            continue
        try:
            all_ok &= verify(c)
        except Exception as e:
            import traceback
            traceback.print_exc()
            print(f"  [FAIL] {c}: {type(e).__name__}: {e}")
            all_ok = False
    print("\n====", "ALL PASS" if all_ok else "FAILURES PRESENT", "====")
    sys.exit(0 if all_ok else 1)
