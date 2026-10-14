# -*- coding: utf-8 -*-
"""Export a SWGoH humanoid muscle clip as a glTF animation attached to the
character's extracted GLB.

Run:  py tools/export_animation.py            (writes extracted/<char>/<char>.anim.glb)
      py tools/export_animation.py --probe    (diagnostics only, no file written)

Pipeline (clip format: tools/MUSCLE_FORMAT.md):

  1. char_*_pre bundle -> Avatar typetree (jynerso: "jynerso_meshAvatar")
       m_Avatar.m_Human.data.m_HumanBoneIndex   : human id -> skeleton node
       m_Avatar.m_Human.data.m_Skeleton.data    : m_Node (m_AxesId), m_ID (name CRC32),
                                                  m_AxesArray (PreQ/PostQ/Sgn/Limit, radians)
       m_Avatar.m_Human.data.m_Scale            : humanScale
       m_TOS                                    : name CRC32 -> transform path
  2. shared_animationcontainer -> clip "hmn_homescrn_any01" via tools/muscle_clip
  3. muscle value -> joint rotation, Unity's humanoid runtime formula
     (invariant documented by ShaderMotion Script/Common/HumanAxes.cs and
      discussions.unity.com/t/muscle-values-to-bone-rotation/717448):

         bone.localRotation * PostQ == PreQ * SwingTwist(Sgn * angle)
         =>  localRotation = PreQ * SwingTwist(Sgn * angle) * conj(PostQ)
         SwingTwist(a) = AngleAxis(|(0,a_y,a_z)|, normalize(0,a_y,a_z)) * AngleAxis(a_x, X)

     Verified against this game's data:
     - the dense curve values are BAKED DoF ANGLES IN RADIANS, clamped to the
       joint's own m_AxesArray m_Limit range (48/52 body slots fit inside the
       radian limits; the 4 clavicle slots exceed theirs and clamp);
     - muscle dof i drives the i-th NON-FROZEN axis of m_AxesArray in
       REVERSED order (dof0 -> z, dof1 -> middle, dof2 -> x): twist DoFs end
       on the bone long axis (x, +/-90 deg limits) and swing DoFs on y/z,
       matching every joint's frozen-axis layout;
     - the whole product is composed in Unity quaternion space and then
       X-mirrored (conv_q) for the glTF rig - conv_q is only a frame map, the
       SwingTwist angles themselves stay in the avatar/Unity convention;
     - validated at t=0: the knee lands at 29 deg from the T-pose (2-bone IK
       over m_StartX/m_LeftFootStartX predicts 18-33 deg) and the feet touch
       the ground at the m_Left/RightFootStartX sole heights.
  4. RootT/RootQ (humanoid slots 7-13, == m_StartX) are the body transform in
     the clip's root-motion frame.  The muscle local rotations are relative
     to the avatar T-pose pelvis frame (m_HumanDescription pelvicJNT
     m_Rotation), so the skeleton-space pelvis rotation is
     rootQ(t) * descPelvisQ; translation is RootT as-is.  With this, the t=0
     pose stands upright: ballL y=0.022, head y=1.58 (T-pose head 1.64).
  5. plain Transform curves of the clip (weapon layer; CRC32 paths resolved to
     node names via the Avatar's own m_TOS) are merged in, sampled at 24 Hz.

Deliberately skipped humanoid slots (see delivery report):
  - 0-6   MotionT/MotionQ   : not driven by SWGoH clips (index -1)
  - 14-41 LimbT/LimbQ       : hand/foot IK end-effector targets consumed by
                              Unity's humanoid solver; the FK muscle pose is
                              exported instead
  - 97+   finger DoF / TDoF : rig has no finger bones, m_HasTDoF = False
"""

from __future__ import annotations

import array
import glob
import json
import math
import os
import struct
import sys

TOOLS = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(TOOLS)
sys.path.insert(0, TOOLS)

import UnityPy  # noqa: E402

from muscle_clip import (  # noqa: E402
    binding_dimensions,
    muscle_slot_curves,
    parse_animation_clip,
    sample_curve,
)

BUNDLE_ROOT = r"C:\Users\PPfavorite\AppData\LocalLow\Unity\Electronic Arts_SWGoH"
CHAR = "char_jynerso_pre"
CLIP_NAME = "hmn_homescrn_any01"
# clips embedded in every batch .anim.glb (retargeted per char):
#   any01   -- home-screen idle (what the game shows on the character page)
#   taunt1  -- taunt gesture, more demonstrative than an idle
BATCH_CLIPS = ["hmn_homescrn_any01", "hmn_spcl_taunt1"]

_CLIP_CACHE = {}  # clip name -> parsed clip dict (shared_animationcontainer is
                  # the same 178-clip bundle for every character: parse once)

# ---------------------------------------------------------------------------
# Unity humanoid tables
# ---------------------------------------------------------------------------

# Unity's internal human bone id order = index space of Avatar
# m_Human.data.m_HumanBoneIndex.  NOTE: unlike Unity's HumanBodyBones enum,
# SWGoH avatars group the arm joints by joint type (both upper arms, then both
# forearms, then both hands).  Verified against this avatar: every id resolves
# through m_TOS to the rig leaf expected by the naming convention, and the
# per-id m_AxesArray limit shapes match the joint type (3-DoF shoulder, 2-DoF
# elbow/wrist, ...).
HUMAN_ID_BONES = [
    "Hips",             # 0
    "LeftUpperLeg",     # 1
    "RightUpperLeg",    # 2
    "LeftLowerLeg",     # 3
    "RightLowerLeg",    # 4
    "LeftFoot",         # 5
    "RightFoot",        # 6
    "Spine",            # 7
    "Chest",            # 8
    "UpperChest",       # 9
    "Neck",             # 10
    "Head",             # 11
    "LeftShoulder",     # 12
    "RightShoulder",    # 13
    "LeftUpperArm",     # 14  Left_shldrJNT
    "RightUpperArm",    # 15  Right_shldrJNT
    "LeftLowerArm",     # 16  Left_elbwJNT
    "RightLowerArm",    # 17  Right_elbwJNT
    "LeftHand",         # 18  Left_wrstJNT
    "RightHand",        # 19  Right_wrstJNT
    "LeftToes",         # 20
    "RightToes",        # 21
    "LeftEye",          # 22
    "RightEye",         # 23
    "Jaw",              # 24
]

# HumanoidMuscleType slots 42..96 (uTinyRipper MuscleType enum, Unity 5.6+
# layout incl. UpperChest): (human bone, number of muscle DoF) in slot order.
# Confirmed against this clip's driven-slot set: UpperChest (48-50), eyes/jaw
# (57-62) and fingers (97-126) are all undriven for this rig.
MUSCLE_JOINTS = [
    ("Spine", 3), ("Chest", 3), ("UpperChest", 3),
    ("Neck", 3), ("Head", 3),
    ("LeftEye", 2), ("RightEye", 2), ("Jaw", 2),
    ("LeftUpperLeg", 3), ("LeftLowerLeg", 2), ("LeftFoot", 2), ("LeftToes", 1),
    ("RightUpperLeg", 3), ("RightLowerLeg", 2), ("RightFoot", 2), ("RightToes", 1),
    ("LeftShoulder", 2), ("LeftUpperArm", 3), ("LeftLowerArm", 2), ("LeftHand", 2),
    ("RightShoulder", 2), ("RightUpperArm", 3), ("RightLowerArm", 2), ("RightHand", 2),
]
MUSCLE_SLOT_BASE = 42  # HumanoidMuscleType.Muscles


def muscle_slot_table():
    """slot -> (human bone name, dof index) for the body-muscle slots."""
    table = {}
    slot = MUSCLE_SLOT_BASE
    for bone, ndof in MUSCLE_JOINTS:
        for dof in range(ndof):
            table[slot] = (bone, dof)
            slot += 1
    return table


# expected rig leaf names (catalog/ANIMATIONS.md section 3 naming convention),
# used as a runtime sanity check between avatar and GLB skeleton
HUMAN_BONE_TO_RIG_LEAF = {
    "Hips": "pelvicJNT",
    "Spine": "spine1JNT",
    "Chest": "spine2JNT",
    "Neck": "neckJNT",
    "Head": "headJNT",
    "LeftShoulder": "Left_clavJNT",
    "RightShoulder": "Right_clavJNT",
    "LeftUpperArm": "Left_shldrJNT",
    "RightUpperArm": "Right_shldrJNT",
    "LeftLowerArm": "Left_elbwJNT",
    "RightLowerArm": "Right_elbwJNT",
    "LeftHand": "Left_wrstJNT",
    "RightHand": "Right_wrstJNT",
    "LeftUpperLeg": "Left_hipJNT",
    "RightUpperLeg": "Right_hipJNT",
    "LeftLowerLeg": "Left_kneeJNT",
    "RightLowerLeg": "Right_kneeJNT",
    "LeftFoot": "Left_ankleJNT",
    "RightFoot": "Right_ankleJNT",
    "LeftToes": "Left_ballJNT",
    "RightToes": "Right_ballJNT",
}

# ---------------------------------------------------------------------------
# quaternion math (x, y, z, w)
# ---------------------------------------------------------------------------


def q_mul(a, b):
    ax, ay, az, aw = a
    bx, by, bz, bw = b
    return (
        aw * bx + ax * bw + ay * bz - az * by,
        aw * by - ax * bz + ay * bw + az * bx,
        aw * bz + ax * by - ay * bx + az * bw,
        aw * bw - ax * bx - ay * by - az * bz,
    )


def q_conj(q):
    return (-q[0], -q[1], -q[2], q[3])


def q_norm(q):
    n = math.sqrt(sum(v * v for v in q)) or 1.0
    return (q[0] / n, q[1] / n, q[2] / n, q[3] / n)


def q_axis_angle(axis, ang):
    n = math.sqrt(sum(v * v for v in axis))
    if n < 1e-12:
        return (0.0, 0.0, 0.0, 1.0)
    axis = (axis[0] / n, axis[1] / n, axis[2] / n)
    s = math.sin(ang * 0.5)
    return (axis[0] * s, axis[1] * s, axis[2] * s, math.cos(ang * 0.5))


def swing_twist(angle):
    """Unity muscle evaluation: dof0 = twist about the muscle-frame X axis,
    dofs 1/2 = swing about the combined (y, z) axis."""
    ax, ay, az = angle
    swing = q_axis_angle((0.0, ay, az), math.sqrt(ay * ay + az * az))
    twist = q_axis_angle((1.0, 0.0, 0.0), ax)
    return q_mul(swing, twist)


def lerp(a, b, t):
    return a + (b - a) * t


# Unity (LH, Y-up) -> glTF (RH, Y-up) X-mirror, identical to swgoh_extract.py
def conv_v3(v):
    return (-v[0], v[1], v[2])


def conv_q(q):
    return (q[0], -q[1], -q[2], q[3])


# 3x4 matrix helpers for the FK probe
def quat_mat(q):
    x, y, z, w = q
    return [
        [1 - 2 * (y * y + z * z), 2 * (x * y - z * w), 2 * (x * z + y * w)],
        [2 * (x * y + z * w), 1 - 2 * (x * x + z * z), 2 * (y * z - x * w)],
        [2 * (x * z - y * w), 2 * (y * z + x * w), 1 - 2 * (x * x + y * y)],
    ]


def mat_mul(a, b):
    out = []
    for i in range(3):
        row = [sum(a[i][k] * b[k][j] for k in range(3)) for j in range(3)]
        row.append(sum(a[i][k] * b[k][3] for k in range(3)) + a[i][3])
        out.append(row)
    return out


def trs_mat(t, q):
    m = quat_mat(q)
    m[0].append(t[0]); m[1].append(t[1]); m[2].append(t[2])
    return m


def mat_apply(m, v):
    return [m[i][0] * v[0] + m[i][1] * v[1] + m[i][2] * v[2] + m[i][3] for i in range(3)]


# ---------------------------------------------------------------------------
# bundle loading
# ---------------------------------------------------------------------------


def latest_bundle(name):
    paths = glob.glob(os.path.join(BUNDLE_ROOT, name, "*", "__data"))
    if not paths:
        raise SystemExit(f"bundle not found: {name}")
    paths.sort(key=os.path.getmtime)
    return paths[-1]


def load_avatar(env):
    """The rig's real Avatar typetree (skips placeholder pst_* avatars whose
    m_HumanBoneIndex is all -1)."""
    for obj in env.objects:
        if obj.type.name != "Avatar":
            continue
        tt = obj.read_typetree()
        idx = tt["m_Avatar"]["m_Human"]["data"]["m_HumanBoneIndex"]
        if len(idx) == len(HUMAN_ID_BONES) and any(v >= 0 for v in idx):
            return tt
    raise SystemExit("no populated Avatar in bundle")


def build_joint_table(av):
    """{human bone name: {node, leaf, axes}} for every mapped humanoid joint.

    axes = {pre, post, sgn, min[3], max[3]}, limits in radians (m_AxesArray)."""
    hu = av["m_Avatar"]["m_Human"]["data"]
    skel = hu["m_Skeleton"]["data"]
    nodes = skel["m_Node"]
    ids = skel["m_ID"]
    axes_arr = skel["m_AxesArray"]
    bone_index = hu["m_HumanBoneIndex"]
    tos = {int(h): p for h, p in av["m_TOS"]}

    joints = {}
    for hid, node in enumerate(bone_index):
        name = HUMAN_ID_BONES[hid]
        if node < 0 or node >= len(nodes):
            continue
        rec = {"node": node, "leaf": tos.get(ids[node], "/").split("/")[-1], "axes": None}
        axes_id = nodes[node]["m_AxesId"]
        if 0 <= axes_id < len(axes_arr):
            ax = axes_arr[axes_id]
            rec["axes"] = {
                "pre": (ax["m_PreQ"]["x"], ax["m_PreQ"]["y"], ax["m_PreQ"]["z"], ax["m_PreQ"]["w"]),
                "post": (ax["m_PostQ"]["x"], ax["m_PostQ"]["y"], ax["m_PostQ"]["z"], ax["m_PostQ"]["w"]),
                "sgn": (ax["m_Sgn"]["x"], ax["m_Sgn"]["y"], ax["m_Sgn"]["z"]),
                "min": (ax["m_Limit"]["m_Min"]["x"], ax["m_Limit"]["m_Min"]["y"], ax["m_Limit"]["m_Min"]["z"]),
                "max": (ax["m_Limit"]["m_Max"]["x"], ax["m_Limit"]["m_Max"]["y"], ax["m_Limit"]["m_Max"]["z"]),
            }
        joints[name] = rec
    return joints


def load_clip(path, name):
    if name in _CLIP_CACHE:
        return _CLIP_CACHE[name]
    env = UnityPy.load(path)
    for obj in env.objects:
        if obj.type.name != "AnimationClip":
            continue
        if obj.read_typetree()["m_Name"] == name:
            clip = parse_animation_clip(obj.get_raw_data())
            _CLIP_CACHE[name] = clip
            return clip
    raise SystemExit(f"clip {name} not found in {path}")


def desc_pelvis_q(av):
    """Pelvis local rotation of the avatar T-pose skeleton (m_HumanDescription).

    The muscle local rotations are expressed relative to this pelvis frame,
    so the animated pelvis rotation is rootQ(t) * descPelvisQ."""
    for e in av["m_HumanDescription"]["m_Skeleton"]:
        if e["m_Name"] == "pelvicJNT":
            r = e["m_Rotation"]
            return (r["x"], r["y"], r["z"], r["w"])
    raise SystemExit("pelvicJNT missing from m_HumanDescription")


# ---------------------------------------------------------------------------
# muscle clip -> pose evaluation
# ---------------------------------------------------------------------------


class MusclePoseEvaluator:
    """Evaluates the muscle clip into per-node glTF-space local transforms.

    Convention (validated against m_Start/m_LeftFootStartX sole targets, the
    desc T-pose and 2-bone leg IK at t=0):

    * dense curve values are BAKED DoF ANGLES IN RADIANS (not normalized
      [-1,1] muscles): clamped to the joint's per-axis m_Limit range.
    * muscle dof i drives the i-th NON-FROZEN axis of m_AxesArray in REVERSED
      order (dof0 -> z, dof1 -> y/middle, dof2 -> x), scaled by m_Sgn[axis].
      This puts twist DoFs on the bone's long axis (x) and swing DoFs on
      y/z, and matches the frozen-axis layout of every joint in this avatar.
    * Unity humanoid composition (ShaderMotion HumanAxes invariant):
          localRotation * postQ == preQ * SwingTwist(a)
      =>  localRotation      == preQ * SwingTwist(a) * conj(postQ)
      composed entirely in Unity quaternion space, then X-mirrored
      (conv_q) for the glTF rig.  SwingTwist(a) = swing about the combined
      (y,z) axis * twist about x; a[] is indexed by axis.
    * hips ("body") transform: RootT/RootQ curves (== m_StartX) are the body
      transform in the clip's root-motion frame.  The muscle local rotations
      are relative to the avatar T-pose pelvis frame, so the skeleton-space
      pelvis rotation is rootQ(t) * descPelvisQ (verified: lands the feet on
      the ground with the legs in IK-consistent poses)."""

    def __init__(self, clip, joints, desc_pelvis_q):
        self.clip = clip
        self.joints = joints
        self.desc_pelvis_q = desc_pelvis_q
        self.slots = muscle_slot_curves(clip)
        self.stop = clip["muscle_clip"]["stop_time"]
        self.rate = clip["muscle_clip"]["clip"]["dense"]["sample_rate"] or 24.0

        # slot -> (bone, dof); dof -> axis = REVERSED order of non-frozen axes
        table = muscle_slot_table()
        self.joint_axes = {}   # bone -> {dof: axis}
        self.joint_slots = {}  # bone -> {slot: dof} (only slots driven by clip)
        for slot, (bone, dof) in table.items():
            if slot not in self.slots or bone not in joints:
                continue
            ax = joints[bone]["axes"]
            if ax is None:
                continue
            free = [i for i in (2, 1, 0) if abs(ax["max"][i] - ax["min"][i]) > 1e-9]
            if dof >= len(free):
                continue
            self.joint_axes.setdefault(bone, {})[dof] = free[dof]
            self.joint_slots.setdefault(bone, {})[slot] = dof

    def evaluate(self, t):
        """-> {leaf: {'rotation': [x,y,z,w]}} plus '__root__' hips transform
        (both already converted to glTF space)."""
        pose = {}
        # RootT / RootQ (humanoid slots 7-13) = hips ("body") transform in
        # the root-motion frame
        if all(s in self.slots for s in range(7, 14)):
            root_t = [sample_curve(self.clip, self.slots[s], t) for s in (7, 8, 9)]
            root_q = q_norm(tuple(sample_curve(self.clip, self.slots[s], t)
                                  for s in (10, 11, 12, 13)))
            pelvis_q = q_norm(q_mul(root_q, self.desc_pelvis_q))
            pose["__root__"] = {"translation": conv_v3(root_t),
                                "rotation": conv_q(pelvis_q)}
        # body muscles -> joint local rotations
        for bone, slotmap in self.joint_slots.items():
            rec = self.joints[bone]
            ax = rec["axes"]
            angle = [0.0, 0.0, 0.0]
            for slot, dof in slotmap.items():
                axis = self.joint_axes[bone][dof]
                v = sample_curve(self.clip, self.slots[slot], t)
                v = max(ax["min"][axis], min(ax["max"][axis], v))
                angle[axis] += v * ax["sgn"][axis]
            q = q_norm(q_mul(q_mul(ax["pre"], swing_twist(angle)), q_conj(ax["post"])))
            pose[rec["leaf"]] = {"rotation": conv_q(q)}
        return pose


# ---------------------------------------------------------------------------
# glTF helpers
# ---------------------------------------------------------------------------


def load_glb(path):
    with open(path, "rb") as f:
        data = f.read()
    magic, _ver, _total = struct.unpack_from("<4sII", data, 0)
    if magic != b"glTF":
        raise SystemExit("not a GLB")
    jlen, jtype = struct.unpack_from("<II", data, 12)
    if jtype != 0x4E4F534A:
        raise SystemExit("JSON chunk missing")
    js = json.loads(data[20:20 + jlen])
    off = 20 + jlen
    blen, btype = struct.unpack_from("<II", data, off)
    if btype != 0x004E4942:
        raise SystemExit("BIN chunk missing")
    return js, bytearray(data[off + 8:off + 8 + blen])


def save_glb(path, js, binbuf):
    out = bytearray()
    jbytes = json.dumps(js, separators=(",", ":")).encode()
    jbytes += b" " * ((4 - len(jbytes) % 4) % 4)
    bl = len(binbuf)
    bpad = (4 - bl % 4) % 4
    total = 12 + 8 + len(jbytes) + 8 + bl + bpad
    out += struct.pack("<4sII", b"glTF", 2, total)
    out += struct.pack("<I4s", len(jbytes), b"JSON")
    out += jbytes
    out += struct.pack("<I4s", bl + bpad, b"BIN\0")
    out += bytes(binbuf) + b"\0" * bpad
    with open(path, "wb") as f:
        f.write(out)
    return total


def node_index_by_name(js):
    return {n.get("name", ""): i for i, n in enumerate(js["nodes"])}


def build_animation(js, binbuf, anim_name, times, channels):
    """channels: [(node_idx, 'rotation'|'translation', flat values)].
    Returns the glTF animation object; appends bufferViews/accessors."""

    def accessor(values, ctype, comps):
        payload = array.array("f", values).tobytes()
        while len(binbuf) % 4:
            binbuf.append(0)
        js.setdefault("bufferViews", []).append(
            {"buffer": 0, "byteOffset": len(binbuf), "byteLength": len(payload)}
        )
        binbuf.extend(payload)
        js.setdefault("accessors", []).append(
            {
                "bufferView": len(js["bufferViews"]) - 1,
                "componentType": 5126,
                "count": len(values) // comps,
                "type": ctype,
                "min": [min(values[i::comps]) for i in range(comps)],
                "max": [max(values[i::comps]) for i in range(comps)],
            }
        )
        return len(js["accessors"]) - 1

    time_acc = accessor(list(times), "SCALAR", 1)
    samplers, chan_objs = [], []
    for node_idx, prop, values in channels:
        comps, ctype = (4, "VEC4") if prop == "rotation" else (3, "VEC3")
        out_acc = accessor(values, ctype, comps)
        samplers.append({"input": time_acc, "output": out_acc, "interpolation": "LINEAR"})
        chan_objs.append(
            {"sampler": len(samplers) - 1, "target": {"node": node_idx, "path": prop}}
        )
    return {"name": anim_name, "channels": chan_objs, "samplers": samplers}


def transform_curve_bindings(clip, tos):
    """[(leaf, attribute, [curve indices])] for plain Transform bindings."""
    out = []
    acc = 0
    for b, d in zip(clip["bindings"]["generic_bindings"], binding_dimensions(clip)):
        if b["type_id"] == 4:
            path = tos.get(b["path"])
            leaf = path.split("/")[-1] if path else None
            out.append((leaf, b["attribute"], list(range(acc, acc + d))))
        acc += d
    return out


# ---------------------------------------------------------------------------
# FK probe (world-space points using GLB rest translations + pose rotations)
# ---------------------------------------------------------------------------


def make_fk(js, pose):
    parent = {}
    for i, n in enumerate(js["nodes"]):
        for c in n.get("children", []):
            parent[c] = i
    idx = node_index_by_name(js)

    def world(leaf):
        chain = []
        i = idx[leaf]
        while i is not None:
            chain.append(i)
            i = parent.get(i)
        m = None
        for i in reversed(chain):
            nd = js["nodes"][i]
            name = nd.get("name", "")
            t = list(nd.get("translation", [0.0, 0.0, 0.0]))
            q = tuple(nd.get("rotation", [0.0, 0.0, 0.0, 1.0]))
            if name == "pelvicJNT" and "__root__" in pose:
                t = pose["__root__"]["translation"]
                q = pose["__root__"]["rotation"]
            elif name in pose and "rotation" in pose[name]:
                q = pose[name]["rotation"]
            lm = trs_mat(t, q)
            m = lm if m is None else mat_mul(m, lm)
        return mat_apply(m, [0.0, 0.0, 0.0])

    return world


# ---------------------------------------------------------------------------
# main
# ---------------------------------------------------------------------------


class RigMismatch(Exception):
    """Avatar leaf names don't follow the game-wide rig naming convention."""


def export_char(char, clip_names, probe=False, verbose=True):
    """Convert `clip_names` onto `char`'s humanoid rig and write
    extracted/<char>/<char>.anim.glb (all clips in one glTF animations array).

    Raises SystemExit (no populated Avatar / missing assets) or RigMismatch
    (leaf naming doesn't match the convention -> retargeting unsafe)."""
    glb_in = os.path.join(ROOT, "extracted", char, f"{char}.glb")
    glb_out = os.path.join(ROOT, "extracted", char, f"{char}.anim.glb")
    if not os.path.exists(glb_in):
        raise SystemExit(f"missing {glb_in} (run swgoh_extract.py first)")

    av = load_avatar(UnityPy.load(latest_bundle(char)))
    joints = build_joint_table(av)
    tos = {int(h): p for h, p in av["m_TOS"]}

    # sanity: the avatar's human map must resolve to the rig's naming scheme,
    # otherwise muscle angles would be retargeted onto wrong nodes.
    # Accepted variants (all verified in the 0.40.7 cache): case differences
    # (rancor 'left_hipJNT'), and wrist leaves named for the weapon joint some
    # rigs hang the hand on ('*_wpn_offsetJNT', '*_wpnJNT') -- the retarget
    # itself always uses the avatar's own m_TOS leaf, never these constants.
    _HAND_ALT = ("wrstJNT", "wpn_offsetJNT", "wpnJNT")
    for bone, leaf in HUMAN_BONE_TO_RIG_LEAF.items():
        got = joints.get(bone, {}).get("leaf")
        if got is None or got == leaf:
            continue
        if got.lower() == leaf.lower():
            continue
        if bone.endswith("Hand") and got.lower().endswith(tuple(a.lower() for a in _HAND_ALT)):
            continue
        raise RigMismatch(f"human bone {bone}: avatar leaf {got!r} != expected {leaf!r}")

    pelvis_q = desc_pelvis_q(av)
    anim_bundle = latest_bundle("shared_animationcontainer")

    js, binbuf = load_glb(glb_in)
    idx = node_index_by_name(js)

    for clip_name in clip_names:
        clip = load_clip(anim_bundle, clip_name)
        ev = MusclePoseEvaluator(clip, joints, desc_pelvis_q=pelvis_q)
        n_frames = int(round(clip["muscle_clip"]["stop_time"] * ev.rate)) + 1
        times = [i / ev.rate for i in range(n_frames)]
        poses = [ev.evaluate(t) for t in times]

        if probe:
            print(f"\n[{char} / {clip_name}] frames {n_frames} @ {ev.rate:.0f}Hz")
            print("max rotation delta vs GLB rest (deg):")
            for bone, rec in sorted(joints.items()):
                leaf = rec["leaf"]
                if leaf not in idx or leaf not in poses[0]:
                    continue
                rest = js["nodes"][idx[leaf]].get("rotation")
                if not rest:
                    continue
                worst = 0.0
                for p in poses:
                    dot = abs(sum(a * b for a, b in zip(p[leaf]["rotation"], rest)))
                    worst = max(worst, math.degrees(2 * math.acos(min(1.0, dot))))
                print(f"  {leaf:20s} {worst:7.2f}")
            print("stance probe (world y: ballL ballR ankleL ankleR head):")
            for t in (times[0], times[len(times) // 2], times[-1]):
                fk = make_fk(js, ev.evaluate(t))
                ys = [fk(leaf)[1] for leaf in ("Left_ballJNT", "Right_ballJNT",
                                               "Left_ankleJNT", "Right_ankleJNT", "headJNT")]
                print(f"  t={t:.3f}  " + " ".join(f"{v:.4f}" for v in ys))

        channels = []
        animated = set()
        # hips root transform (translation + rotation)
        if "__root__" in poses[0]:
            tv = []
            for p in poses:
                tv.extend(p["__root__"]["translation"])
            channels.append((idx["pelvicJNT"], "translation", tv))
            animated.add("pelvicJNT")
        # muscle-driven joints
        for bone, rec in sorted(joints.items()):
            leaf = rec["leaf"]
            if leaf not in idx or leaf not in poses[0]:
                continue
            vals = []
            for p in poses:
                vals.extend(p[leaf]["rotation"])
            channels.append((idx[leaf], "rotation", vals))
            animated.add(leaf)
        # clip transform curves (weapon layer)
        for leaf, attr, curves in transform_curve_bindings(clip, tos):
            if leaf is None or leaf not in idx:
                # Hash not in m_TOS: animates a prefab attachment node that has
                # no counterpart in the GLB (weapon FX / shadow meshes).
                if verbose:
                    print(f"  .. skipped transform binding (no GLB node)")
                continue
            if attr == 1:    # translation
                vals = []
                for t in times:
                    vals.extend(conv_v3([sample_curve(clip, c, t) for c in curves]))
                channels.append((idx[leaf], "translation", vals))
            elif attr == 2:  # rotation
                vals = []
                for t in times:
                    vals.extend(conv_q(q_norm(tuple(sample_curve(clip, c, t) for c in curves))))
                channels.append((idx[leaf], "rotation", vals))
            else:
                # attribute 3 = scale: verified constant (1,1,1) on every weapon
                # node, already baked into GLB bind pose -- skipping is a no-op.
                vals = []
                for t in times:
                    vals.extend([sample_curve(clip, c, t) for c in curves])
                if len({tuple(v) for v in (vals[i:i+3] for i in range(0, len(vals), 3))}) != 1 \
                        and verbose:
                    print(f"  !! unhandled non-constant scale on {leaf}")
                continue
            animated.add(leaf)

        js.setdefault("animations", []).append(
            build_animation(js, binbuf, clip_name, times, channels))

    if probe:
        print("\n[probe] no file written")
        return {"clips": clip_names, "animated": len(idx)}

    total = save_glb(glb_out, js, binbuf)
    return {"clips": clip_names, "bytes": total, "animated": len(animated)}


def main():
    import argparse
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("chars", nargs="*", default=[], help="char_*_pre ids (default: char_jynerso_pre)")
    ap.add_argument("--clips", default=",".join(BATCH_CLIPS), help="comma-separated clip names")
    ap.add_argument("--all", action="store_true", help="batch: every char with a .glb export")
    ap.add_argument("--force", action="store_true", help="re-export even if .anim.glb exists")
    ap.add_argument("--probe", action="store_true", help="diagnostics only, no file written")
    args = ap.parse_args()
    clip_names = [c.strip() for c in args.clips.split(",") if c.strip()]

    if args.all:
        import collections
        models = json.load(open(os.path.join(ROOT, "models.json"), encoding="utf-8"))
        chars = [m["id"] if isinstance(m, dict) else m for m in models]
        stats = collections.Counter()
        skipped = {}
        for i, char in enumerate(chars):
            glb_out = os.path.join(ROOT, "extracted", char, f"{char}.anim.glb")
            if os.path.exists(glb_out) and not args.force:
                stats["exists"] += 1
                continue
            if not os.path.exists(os.path.join(ROOT, "extracted", char, f"{char}.glb")):
                stats["no-glb"] += 1
                continue
            try:
                r = export_char(char, clip_names)
                stats["ok"] += 1
                print(f"[{i+1}/{len(chars)}] {char}: ok {r['bytes']}B "
                      f"({len(r['clips'])} clips, {r['animated']} nodes)", flush=True)
            except (SystemExit, RigMismatch, Exception) as e:
                stats["skipped"] += 1
                kind = type(e).__name__
                skipped.setdefault(f"{kind}: {e}", []).append(char)
                print(f"[{i+1}/{len(chars)}] {char}: SKIP {kind}: {e}", flush=True)
        print("\n==== batch summary ====")
        for k, v in stats.items():
            print(f"  {k}: {v}")
        if skipped:
            with open(os.path.join(ROOT, "anim_batch_skipped.json"), "w", encoding="utf-8") as f:
                json.dump(skipped, f, ensure_ascii=False, indent=1)
            print(f"  skip reasons -> anim_batch_skipped.json ({len(skipped)} reasons)")
        return

    chars = args.chars or [CHAR]
    for char in chars:
        try:
            r = export_char(char, clip_names, probe=args.probe)
        except RigMismatch as e:
            raise SystemExit(f"rig mismatch: {e}")
        if not args.probe:
            print(f"{char}: wrote {r['bytes']}B, clips {r['clips']}, "
                  f"{r['animated']} animated nodes")


if __name__ == "__main__":
    main()
