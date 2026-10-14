"""Unity AnimationClip raw binary parser (Unity 2022.3.x layout, as shipped in SWGoH).

Parses the RAW serialized object bytes (UnityPy ObjectReader.get_raw_data()) of an
AnimationClip into a structured dict.  Works without a typetree: the field layout
below was recovered from the AnimationClip TypeTree nodes embedded in
shared_animationcontainer (Unity 2022.3.62f2) and cross-checked against
AssetRipper/AssetStudio's ClipMuscleConstant implementation.

Object layout (see tools/MUSCLE_FORMAT.md for the byte-level walkthrough):

    AnimationClip
      |- string            m_Name                      (u32 len + bytes + align4)
      |- bool              m_Legacy
      |- bool              m_Compressed
      |- bool              m_UseHighQualityCurve       + align4
      |- vector<...>       m_RotationCurves            (align4 after array)
      |- vector<...>       m_CompressedRotationCurves
      |- vector<...>       m_EulerCurves
      |- vector<...>       m_PositionCurves
      |- vector<...>       m_ScaleCurves
      |- vector<...>       m_FloatCurves
      |- vector<...>       m_PPtrCurves
      |- float             m_SampleRate
      |- int               m_WrapMode
      |- AABB              m_Bounds                    (6 floats)
      |- uint              m_MuscleClipSize
      |- ClipMuscleConstant m_MuscleClip
      |- AnimationClipBindingConstant m_ClipBindingConstant
      |- bool              m_HasGenericRootTransform
      |- bool              m_HasMotionFloatCurves      + align4
      |- vector<AnimationEvent> m_Events

    ClipMuscleConstant
      |- HumanPose  m_DeltaPose
      |    |- xform m_RootX                     (float3 t, float4 q, float3 s = 40B)
      |    |- float3 m_LookAtPosition
      |    |- float4 m_LookAtWeight
      |    |- vector<HumanGoal> m_GoalArray      (64B each)
      |    |- HandPose m_LeftHandPose            (xform + vector<float> DoF + 4 floats)
      |    |- HandPose m_RightHandPose
      |    |- vector<float>  m_DoFArray
      |    |- vector<float3> m_TDoFArray
      |- xform m_StartX
      |- xform m_StopX
      |- xform m_LeftFootStartX
      |- xform m_RightFootStartX
      |- float3 m_AverageSpeed
      |- Clip m_Clip
      |    |- StreamedClip  { vector<uint> data, u16 curveCount, u16 discreteCurveCount }
      |    |- DenseClip     { i32 frameCount, u32 curveCount, f32 sampleRate,
      |                       f32 beginTime, vector<float> sampleArray }
      |    |- ConstantClip  { vector<float> data }
      |- float m_StartTime
      |- float m_StopTime
      |- float m_OrientationOffsetY
      |- float m_Level
      |- float m_CycleOffset
      |- float m_AverageAngularSpeed
      |- vector<int> m_IndexArray                 (value-array slot -> curve index, -1 = unused)
      |- vector<ValueDelta> m_ValueArrayDelta     ({f32 start, f32 stop} per curve)
      |- vector<float> m_ValueArrayReferencePose
      |- 11x bool (mirror, loop_time, loop_blend, loop_blend_orientation,
      |            loop_blend_position_y, loop_blend_position_xz, start_at_origin,
      |            keep_original_orientation, keep_original_position_y,
      |            keep_original_position_xz, height_from_feet) + align4

    AnimationClipBindingConstant
      |- vector<GenericBinding> genericBindings   (28B each: u32 path, u32 attribute,
      |                                            PPtr script (12B), i32 typeID,
      |                                            u8 customType, u8 isPPtrCurve,
      |                                            u8 isIntCurve, u8 isSerializeReferenceCurve)
      |- vector<PPtr> pptrCurveMapping            (12B each)

    AnimationEvent: f32 time, string functionName, string data, PPtr (12B),
                    f32 floatParameter, i32 intParameter, i32 messageOptions

Alignment: little-endian; vectors are u32-size-prefixed; nodes flagged "align" in
the typetree pad the stream to the next 4-byte boundary AFTER their data.  Inside
ClipMuscleConstant no field needs padding (everything is a 4-byte multiple); the
11 trailing bools are padded to 4.

Binding semantics (verified on hmn_homescrn_any01 / hmn_pst_var01_idle /
gen_astromech_idle / shp_shared_aoe_atk):
  - All curve data lives in ONE index space: [streamed curves][dense curves]
    [constant curves].  Streamed curves number StreamedClip.curveCount, dense
    DenseClip.curveCount, the rest are ConstantClip entries.
  - genericBindings maps onto that space BY LIST POSITION: each Transform
    (typeID 4) binding consumes `attribute`-dependent dimensions (1=Translation
    3, 2=Rotation 4, 3=Scaling 3, 4=Euler 3), every other binding consumes 1
    curve (uTinyRipper AnimationClipBindingConstant.FindBinding).
  - typeID 4 (Transform): path = CRC32 of the transform path relative to the
    Animator root, attribute = TransformType (see above).
  - typeID 95 (Animator, customType 8): path = 0, attribute = humanoid
    value-array slot (HumanoidMuscleType: 0-6 Motion T/Q, 7-13 RootT/RootQ,
    14+ limbs x7, then muscles/fingers/TDoF).
  - ClipMuscleConstant.IndexArray[slot] = curve index driving that humanoid
    slot (-1 = slot unused); ValueArrayDelta[curve] = {start, stop} values of
    the curve for loop blending.

Public API:
    parse_animation_clip(raw: bytes) -> dict
    muscle_summary(parsed) -> dict            # stopTime/sampleRate/frameCount/...
    dense_frames(parsed) -> list[list[float]] # frames x curves sample matrix
    curve_values(parsed, curve_index) -> dict # dense samples or constant value
"""

from __future__ import annotations

import struct
import zlib

__all__ = [
    "ParseError",
    "parse_animation_clip",
    "parse_muscle_clip",
    "parse_binding_constant",
    "parse_streamed",
    "sample_streamed",
    "muscle_summary",
    "dense_frames",
    "curve_values",
    "sample_curve",
    "binding_for_curve",
    "binding_dimensions",
    "muscle_slot_curves",
    "crc32_path",
]


class ParseError(ValueError):
    """Raised when the raw bytes do not match the expected AnimationClip layout."""


# ---------------------------------------------------------------------------
# low level reader
# ---------------------------------------------------------------------------


class _Reader:
    """Type-dependent-aligned little-endian reader over a bytes buffer."""

    __slots__ = ("data", "off")

    def __init__(self, data: bytes, offset: int = 0):
        self.data = data
        self.off = offset

    # -- primitives ---------------------------------------------------------
    def _unpack(self, fmt: str, size: int):
        end = self.off + size
        if end > len(self.data):
            raise ParseError(
                f"read past end of data at offset {self.off} (need {size} bytes, "
                f"have {len(self.data) - self.off})"
            )
        (value,) = struct.unpack_from(fmt, self.data, self.off)
        self.off = end
        return value

    def u8(self) -> int:
        return self._unpack("<B", 1)

    def u16(self) -> int:
        return self._unpack("<H", 2)

    def u32(self) -> int:
        return self._unpack("<I", 4)

    def i32(self) -> int:
        return self._unpack("<i", 4)

    def u64(self) -> int:
        return self._unpack("<Q", 8)

    def f32(self) -> float:
        return self._unpack("<f", 4)

    def bytes(self, n: int) -> bytes:
        end = self.off + n
        if end > len(self.data):
            raise ParseError(f"read past end of data at offset {self.off} (need {n})")
        out = self.data[self.off : end]
        self.off = end
        return out

    def align4(self) -> None:
        self.off = (self.off + 3) & ~3

    # -- composite reads ----------------------------------------------------
    def string(self) -> str:
        """u32 length + utf-8 bytes + align4 (typetree string/Array node is align-flagged)."""
        n = self.u32()
        raw = self.bytes(n)
        self.align4()
        return raw.decode("utf-8", errors="replace")

    def vector(self, elem):
        """u32 count + count elements; no implicit alignment (align handled by callers)."""
        n = self.u32()
        return [elem() for _ in range(n)]

    def f32_vector(self):
        return self.vector(self.f32)

    def u32_vector(self):
        return self.vector(self.u32)

    def i32_vector(self):
        return self.vector(self.i32)

    def pptr(self) -> dict:
        return {"file_id": self.i32(), "path_id": self.u64()}


def crc32_path(path: str) -> int:
    """GenericBinding.path = CRC32 (zlib/IEEE) of the transform path string."""
    return zlib.crc32(path.encode("utf-8")) & 0xFFFFFFFF


# ---------------------------------------------------------------------------
# math-ish structs inside ClipMuscleConstant
# ---------------------------------------------------------------------------


def _read_xform(r: _Reader) -> dict:
    return {
        "t": [r.f32() for _ in range(3)],
        "q": [r.f32() for _ in range(4)],
        "s": [r.f32() for _ in range(3)],
    }


def _read_hand_pose(r: _Reader) -> dict:
    grab_x = _read_xform(r)
    dof = r.f32_vector()
    return {
        "grab_x": grab_x,
        "dof_array": dof,
        "override": r.f32(),
        "close_open": r.f32(),
        "in_out": r.f32(),
        "grab": r.f32(),
    }


def _read_human_pose(r: _Reader) -> dict:
    pose = {
        "root_x": _read_xform(r),
        "look_at_position": [r.f32() for _ in range(3)],
        "look_at_weight": [r.f32() for _ in range(4)],
    }
    goals = r.u32()
    pose["goal_array"] = [
        {
            "x": _read_xform(r),
            "weight_t": r.f32(),
            "weight_r": r.f32(),
            "hint_t": [r.f32() for _ in range(3)],
            "hint_weight_t": r.f32(),
        }
        for _ in range(goals)
    ]
    pose["left_hand_pose"] = _read_hand_pose(r)
    pose["right_hand_pose"] = _read_hand_pose(r)
    pose["dof_array"] = r.f32_vector()
    n_tdof = r.u32()
    pose["tdof_array"] = [[r.f32() for _ in range(3)] for _ in range(n_tdof)]
    return pose


def _read_clip(r: _Reader) -> dict:
    # StreamedClip: vector<uint> data, u16 curveCount, u16 discreteCurveCount.
    # Clips without streamed curves carry a 2-word sentinel {0x7F800000, 0}
    # (+inf float marker) with curveCount == 0.
    streamed_data = r.u32_vector()
    streamed = {
        "data": streamed_data,
        "curve_count": r.u16(),
        "discrete_curve_count": r.u16(),
    }
    frame_count = r.i32()
    curve_count = r.u32()
    sample_rate = r.f32()
    begin_time = r.f32()
    sample_array = r.f32_vector()
    if len(sample_array) != frame_count * curve_count:
        raise ParseError(
            f"DenseClip sample array length {len(sample_array)} != "
            f"frameCount*curveCount = {frame_count}*{curve_count}"
        )
    dense = {
        "frame_count": frame_count,
        "curve_count": curve_count,
        "sample_rate": sample_rate,
        "begin_time": begin_time,
        "sample_array": sample_array,
    }
    constant = r.f32_vector()  # ConstantClip.data
    return {
        "streamed": streamed,
        "dense": dense,
        "constant": constant,
    }


# ---------------------------------------------------------------------------
# ClipMuscleConstant
# ---------------------------------------------------------------------------


def parse_muscle_clip(data: bytes, offset: int = 0, muscle_clip_size: int | None = None):
    """Parse a ClipMuscleConstant blob (the m_MuscleClip field payload).

    `data` is normally the full raw AnimationClip object with `offset` pointing at
    the m_MuscleClip field; a standalone blob (offset 0) works too.  Returns the
    parsed dict; the reader position is advanced past the struct so callers can
    keep parsing the enclosing object (dict key "end_offset").
    """
    r = _Reader(data, offset)
    start = offset
    mc = {}
    mc["delta_pose"] = _read_human_pose(r)
    mc["start_x"] = _read_xform(r)
    mc["stop_x"] = _read_xform(r)
    mc["left_foot_start_x"] = _read_xform(r)
    mc["right_foot_start_x"] = _read_xform(r)
    mc["average_speed"] = [r.f32() for _ in range(3)]
    mc["clip"] = _read_clip(r)
    mc["start_time"] = r.f32()
    mc["stop_time"] = r.f32()
    mc["orientation_offset_y"] = r.f32()
    mc["level"] = r.f32()
    mc["cycle_offset"] = r.f32()
    mc["average_angular_speed"] = r.f32()
    mc["index_array"] = r.i32_vector()
    n_delta = r.u32()
    mc["value_array_delta"] = [
        {"start": r.f32(), "stop": r.f32()} for _ in range(n_delta)
    ]
    mc["value_array_reference_pose"] = r.f32_vector()
    mc["mirror"] = bool(r.u8())
    mc["loop_time"] = bool(r.u8())
    mc["loop_blend"] = bool(r.u8())
    mc["loop_blend_orientation"] = bool(r.u8())
    mc["loop_blend_position_y"] = bool(r.u8())
    mc["loop_blend_position_xz"] = bool(r.u8())
    mc["start_at_origin"] = bool(r.u8())
    mc["keep_original_orientation"] = bool(r.u8())
    mc["keep_original_position_y"] = bool(r.u8())
    mc["keep_original_position_xz"] = bool(r.u8())
    mc["height_from_feet"] = bool(r.u8())
    r.align4()  # m_HeightFromFeet is align-flagged in the typetree
    mc["bytes_consumed"] = r.off - start
    mc["start_offset"] = start
    mc["end_offset"] = r.off
    if muscle_clip_size is not None:
        mc["declared_size"] = muscle_clip_size
        # Unity stores the RUNTIME blob size here, which is >= the on-disk struct
        # size (runtime appends per-curve metadata).  Only warn on mismatch.
        mc["size_matches"] = mc["bytes_consumed"] == muscle_clip_size
    return mc


# ---------------------------------------------------------------------------
# AnimationClipBindingConstant
# ---------------------------------------------------------------------------


def _read_generic_binding(r: _Reader) -> dict:
    b = {
        "path": r.u32(),
        "attribute": r.u32(),
        "script": r.pptr(),
        "type_id": r.i32(),
        "custom_type": r.u8(),
        "is_pptr_curve": r.u8(),
        "is_int_curve": r.u8(),
    }
    r.u8()  # isSerializeReferenceCurve (align-flagged; struct size 28 -> no pad)
    r.align4()
    return b


def parse_binding_constant(data: bytes, offset: int = 0):
    """Parse AnimationClipBindingConstant (genericBindings + pptrCurveMapping)."""
    r = _Reader(data, offset)
    start = offset
    n = r.u32()
    bindings = [_read_generic_binding(r) for _ in range(n)]
    r.align4()
    n_pptr = r.u32()
    pptr = [r.pptr() for _ in range(n_pptr)]
    r.align4()
    return {
        "generic_bindings": bindings,
        "pptr_curve_mapping": pptr,
        "end_offset": r.off,
        "bytes_consumed": r.off - start,
    }


# ---------------------------------------------------------------------------
# plain curve containers (non-muscle / extra-transform curves)
# ---------------------------------------------------------------------------


def _read_keyframe_quat(r: _Reader) -> dict:
    kf = {"time": r.f32()}
    kf["value"] = [r.f32() for _ in range(4)]
    kf["in_slope"] = [r.f32() for _ in range(4)]
    kf["out_slope"] = [r.f32() for _ in range(4)]
    kf["weighted_mode"] = r.i32()
    kf["in_weight"] = [r.f32() for _ in range(4)]
    kf["out_weight"] = [r.f32() for _ in range(4)]
    return kf


def _read_keyframe_vec3(r: _Reader) -> dict:
    kf = {"time": r.f32()}
    kf["value"] = [r.f32() for _ in range(3)]
    kf["in_slope"] = [r.f32() for _ in range(3)]
    kf["out_slope"] = [r.f32() for _ in range(3)]
    kf["weighted_mode"] = r.i32()
    kf["in_weight"] = [r.f32() for _ in range(3)]
    kf["out_weight"] = [r.f32() for _ in range(3)]
    return kf


def _read_keyframe_float(r: _Reader) -> dict:
    return {
        "time": r.f32(),
        "value": r.f32(),
        "in_slope": r.f32(),
        "out_slope": r.f32(),
        "weighted_mode": r.i32(),
        "in_weight": r.f32(),
        "out_weight": r.f32(),
    }


def _read_animation_curve(r: _Reader, kind: str) -> dict:
    keys = r.u32()
    if kind == "quat":
        read_kf, comps = _read_keyframe_quat, 4
    elif kind == "vec3":
        read_kf, comps = _read_keyframe_vec3, 3
    else:
        read_kf, comps = _read_keyframe_float, 1
    curve = {"keys": [read_kf(r) for _ in range(keys)]}
    curve["pre_infinity"] = r.i32()
    curve["post_infinity"] = r.i32()
    curve["rotation_order"] = r.i32()
    curve["components"] = comps
    return curve


def _read_string_direct(r: _Reader) -> str:
    return r.string()


# ---------------------------------------------------------------------------
# whole object
# ---------------------------------------------------------------------------


def parse_animation_clip(raw: bytes) -> dict:
    """Parse raw serialized AnimationClip object bytes into a structured dict."""
    r = _Reader(raw, 0)
    clip = {
        "name": r.string(),
        "legacy": bool(r.u8()),
        "compressed": bool(r.u8()),
        "use_high_quality_curve": bool(r.u8()),
    }
    r.align4()

    def curve_vector(kind):
        n = r.u32()
        out = []
        for _ in range(n):
            c = _read_animation_curve(r, kind)
            c["path"] = r.string()
            out.append(c)
        r.align4()
        return out

    clip["rotation_curves"] = curve_vector("quat")
    # CompressedAnimationCurve: only m_Legacy/compressed clips carry these; SWGoH
    # clips never do (m_Compressed = false).  Parse defensively if ever needed.
    n = r.u32()
    if n:
        raise ParseError(
            "m_CompressedRotationCurves not empty - compressed clips unsupported"
        )
    clip["euler_curves"] = curve_vector("vec3")
    clip["position_curves"] = curve_vector("vec3")
    clip["scale_curves"] = curve_vector("vec3")

    def float_curve_vector():
        n = r.u32()
        out = []
        for _ in range(n):
            c = _read_animation_curve(r, "float")
            c["attribute"] = r.string()
            c["path"] = r.string()
            c["class_id"] = r.i32()
            c["script"] = r.pptr()
            c["flags"] = r.i32()
            out.append(c)
        r.align4()
        return out

    clip["float_curves"] = float_curve_vector()

    n = r.u32()
    pptr_curves = []
    for _ in range(n):
        nkeys = r.u32()
        keys = [{"time": r.f32(), "value": r.pptr()} for _ in range(nkeys)]
        attribute = r.string()
        path = r.string()
        class_id = r.i32()
        script = r.pptr()
        flags = r.i32()
        pptr_curves.append(
            {
                "keys": keys,
                "attribute": attribute,
                "path": path,
                "class_id": class_id,
                "script": script,
                "flags": flags,
            }
        )
    r.align4()
    clip["pptr_curves"] = pptr_curves

    clip["sample_rate"] = r.f32()
    clip["wrap_mode"] = r.i32()
    clip["bounds"] = {
        "center": [r.f32() for _ in range(3)],
        "extent": [r.f32() for _ in range(3)],
    }
    muscle_clip_size = r.u32()
    clip["muscle_clip_size"] = muscle_clip_size
    clip["muscle_clip"] = parse_muscle_clip(raw, r.off, muscle_clip_size)
    r.off = clip["muscle_clip"]["end_offset"]

    clip["bindings"] = parse_binding_constant(raw, r.off)
    r.off = clip["bindings"]["end_offset"]

    clip["has_generic_root_transform"] = bool(r.u8())
    clip["has_motion_float_curves"] = bool(r.u8())
    r.align4()

    n_events = r.u32()
    events = []
    for _ in range(n_events):
        ev = {"time": r.f32()}
        ev["function_name"] = r.string()
        ev["data"] = r.string()
        ev["object_reference_parameter"] = r.pptr()
        ev["float_parameter"] = r.f32()
        ev["int_parameter"] = r.i32()
        ev["message_options"] = r.i32()
        events.append(ev)
    r.align4()
    clip["events"] = events
    clip["bytes_consumed"] = r.off
    clip["raw_size"] = len(raw)
    if r.off != len(raw):
        raise ParseError(
            f"parsed {r.off} bytes but object has {len(raw)} "
            f"({len(raw) - r.off} leftover)"
        )
    return clip


# ---------------------------------------------------------------------------
# streamed clip decode (sparse hermite keys)
# ---------------------------------------------------------------------------


def parse_streamed(data_words: list[int]) -> list[dict]:
    """Decode m_StreamedClip.data (uint words) into streamed frames.

    Buffer layout: a sequence of StreamedFrame records until end of buffer:
        float                    time
        int                      curveCount
        curveCount x StreamedCurveKey { int index, float3 coeff, float value }
    The first frame carries time = -FLT_MAX (sentinel 0xFF7FFFFF, used only for
    slope warm-up) and the last carries time = +INF; a clip without streamed
    curves stores the 2-word sentinel {0x7F800000, 0} (curveCount 0).
    """
    import math

    frames = []
    n = len(data_words)
    i = 0
    while i < n:
        if i + 2 > n:
            break
        time = struct.unpack_from("<f", struct.pack("<I", data_words[i]))[0]
        count = data_words[i + 1]
        if count < 0 or i + 2 + count * 5 > n:
            raise ParseError(
                f"streamed frame at word {i} claims {count} curves, "
                f"only {max(0, (n - i - 2) // 5)} fit"
            )
        curves = []
        base = i + 2
        for c in range(count):
            j = base + c * 5
            idx = struct.unpack_from("<i", struct.pack("<I", data_words[j]))[0]
            coeff = [
                struct.unpack_from("<f", struct.pack("<I", data_words[j + k]))[0]
                for k in (1, 2, 3)
            ]
            value = struct.unpack_from("<f", struct.pack("<I", data_words[j + 4]))[0]
            curves.append(
                {"index": idx, "value": value, "coeff": coeff, "out_slope": coeff[2]}
            )
        frames.append({"time": time, "curves": curves})
        i = base + count * 5
    return frames


def _hermite_eval(value: float, coeff: list[float], dt: float) -> float:
    """Evaluate one streamed segment: v(dt) = value + outSlope*dt + cy*dt^2 + cx*dt^3.

    coeff = (cx, cy, cz=outSlope) exactly as Unity encodes it
    (uTinyRipper StreamedCurveKey.CalculateStreamedFrame is the encoder).
    """
    return value + coeff[2] * dt + coeff[1] * dt * dt + coeff[0] * dt * dt * dt


def _hermite(dt_frac: float, left: float, out_tan: float, right: float, in_tan: float) -> float:
    t2 = dt_frac * dt_frac
    t3 = t2 * dt_frac
    v1 = dt_frac + t3 - 2.0 * t2 * out_tan + (3.0 * t2 - t3 * 2.0 + 1.0) * left
    v2 = (t3 - t2) * in_tan
    v3 = 3.0 * t2 - t3 * 2.0
    return v1 + v2 + v3 * right


def streamed_frame_times(frames: list[dict]) -> list[float]:
    """Usable time values (drops the -FLT_MAX warm-up and +INF terminator)."""
    import math

    return [
        f["time"]
        for f in frames
        if -1e30 < f["time"] < 1e30
    ]


def sample_streamed(frames: list[dict], t: float) -> dict[int, float]:
    """Evaluate every streamed curve at time t.

    Each key's (value, coeff) encodes the cubic segment from its own time to the
    key's NEXT appearance: v = value + cz*dt + cy*dt^2 + cx*dt^3.  The sentinel
    first frame (-FLT_MAX) and terminator frame (+INF) only carry slope data and
    are skipped.
    """
    real = [f for f in frames if -1e30 < f["time"] < 1e30]
    if not real:
        return {}
    # per curve index: sorted (time, value, coeff) segments
    segs: dict[int, list] = {}
    for f in real:
        for c in f["curves"]:
            segs.setdefault(c["index"], []).append((f["time"], c["value"], c["coeff"]))
    out = {}
    for idx, keys in segs.items():
        keys.sort(key=lambda k: k[0])
        if t <= keys[0][0]:
            if t == keys[0][0]:
                out[idx] = keys[0][1]
            else:
                # extrapolate backwards into the first segment
                t0, v0, cf0 = keys[0]
                out[idx] = _hermite_eval(v0, cf0, t - t0)
            continue
        # last key with time <= t
        k = 0
        for j in range(len(keys)):
            if keys[j][0] <= t:
                k = j
            else:
                break
        t0, v0, cf0 = keys[k]
        if k + 1 < len(keys):
            dt = min(t - t0, keys[k + 1][0] - t0)
        else:
            dt = t - t0 if t <= keys[k][0] + 1e30 else 0.0
            dt = t - t0
        out[idx] = _hermite_eval(v0, cf0, dt)
    return out


# ---------------------------------------------------------------------------
# curve index space helpers
# ---------------------------------------------------------------------------

# GenericBinding.attribute for Transform (classID 4) = TransformType:
# 1 = Translation (3 comps), 2 = Rotation (4 comps), 3 = Scaling (3 comps),
# 4 = EulerRotation (3 comps).  Other bindings consume 1 curve each.
_TRANSFORM_DIM = {1: 3, 2: 4, 3: 3, 4: 3}


def binding_dimensions(parsed: dict) -> list[int]:
    """Curve count consumed by each GenericBinding (uTinyRipper FindBinding rule)."""
    dims = []
    for b in parsed["bindings"]["generic_bindings"]:
        if b["type_id"] == 4:  # Transform
            dims.append(_TRANSFORM_DIM.get(b["attribute"], 1))
        else:
            dims.append(1)
    return dims


def binding_for_curve(parsed: dict, curve_index: int):
    """Map a curve index (in [streamed|dense|constant] space) to its binding.

    Returns (binding_dict, component_offset).  Mirrors
    AnimationClipBindingConstant.FindBinding: walk the genericBindings list,
    each Transform binding consumes 3-4 curves, everything else 1.
    """
    dims = binding_dimensions(parsed)
    acc = 0
    for b, d in zip(parsed["bindings"]["generic_bindings"], dims):
        if curve_index < acc + d:
            return b, curve_index - acc
        acc += d
    raise IndexError(
        f"curve index {curve_index} beyond binding table ({acc} curves)"
    )


def muscle_slot_curves(parsed: dict) -> dict[int, int]:
    """Humanoid value-array slot -> curve index (IndexArray; -1 entries dropped).

    Slot numbering = HumanoidMuscleType: 0-6 Motion T/Q, 7-13 RootT/RootQ,
    14+ limbs (7 per limb: T xyz, Q xyzw), then body muscles, finger DoF and
    translation DoF (see uTinyRipper HumanoidMuscleType).
    """
    return {s: c for s, c in enumerate(parsed["muscle_clip"]["index_array"]) if c >= 0}


# ---------------------------------------------------------------------------
# convenience views over the parsed data
# ---------------------------------------------------------------------------


def muscle_summary(parsed: dict) -> dict:
    """Key numbers of the muscle clip: duration, rate, frame count, curve counts."""
    mc = parsed["muscle_clip"]
    dense = mc["clip"]["dense"]
    return {
        "name": parsed["name"],
        "stop_time": mc["stop_time"],
        "start_time": mc["start_time"],
        "sample_rate": dense["sample_rate"],
        "frame_count": dense["frame_count"],
        "curve_count": dense["curve_count"],
        "constant_count": len(mc["clip"]["constant"]),
        "sample_count": len(dense["sample_array"]),
        "streamed_curves": mc["clip"]["streamed"]["curve_count"],
        "total_curves": mc["clip"]["streamed"]["curve_count"]
        + dense["curve_count"]
        + len(mc["clip"]["constant"]),
        "loop_time": mc["loop_time"],
        "mirror": mc["mirror"],
        "binding_count": len(parsed["bindings"]["generic_bindings"]),
        "declared_muscle_clip_size": parsed["muscle_clip_size"],
        "parsed_muscle_clip_bytes": mc["bytes_consumed"],
    }


def dense_frames(parsed: dict) -> list[list[float]]:
    """DenseClip sample array as [frame][curve] (frame-major storage)."""
    dense = parsed["muscle_clip"]["clip"]["dense"]
    ncurves = dense["curve_count"]
    sa = dense["sample_array"]
    if ncurves == 0:
        return []
    return [sa[i : i + ncurves] for i in range(0, len(sa), ncurves)]


def curve_values(parsed: dict, curve_index: int) -> dict:
    """Values of one curve across time, from whichever section stores it.

    Returns one of:
      {"kind": "dense", "values": [...], "sample_rate": r}   per-frame samples
      {"kind": "streamed", "keys": [{"time","value","out_slope"}, ...]}
      {"kind": "constant", "value": v, "start": s, "stop": e}
    """
    mc = parsed["muscle_clip"]
    clip = mc["clip"]
    stream_count = clip["streamed"]["curve_count"]
    dense = clip["dense"]
    dense_count = dense["curve_count"]
    if curve_index < stream_count:
        keys = []
        for frame in parse_streamed(clip["streamed"]["data"]):
            for c in frame["curves"]:
                if c["index"] == curve_index and -1e30 < frame["time"] < 1e30:
                    keys.append(
                        {"time": frame["time"], "value": c["value"], "out_slope": c["out_slope"]}
                    )
        return {"kind": "streamed", "keys": keys}
    if curve_index < stream_count + dense_count:
        ci = curve_index - stream_count
        return {
            "kind": "dense",
            "values": dense["sample_array"][ci::dense_count],
            "sample_rate": dense["sample_rate"],
        }
    const_idx = curve_index - stream_count - dense_count
    constant = clip["constant"]
    if const_idx >= len(constant):
        raise IndexError(f"curve {curve_index} out of range")
    delta = None
    if const_idx < len(mc["value_array_delta"]):
        delta = mc["value_array_delta"][const_idx]
    return {
        "kind": "constant",
        "value": constant[const_idx],
        "start": delta["start"] if delta else None,
        "stop": delta["stop"] if delta else None,
    }


def sample_curve(parsed: dict, curve_index: int, t: float) -> float:
    """Value of one curve at time t (dense frames are linearly interpolated)."""
    cv = curve_values(parsed, curve_index)
    if cv["kind"] == "dense":
        rate = cv["sample_rate"]
        x = t * rate
        i0 = int(x)
        vals = cv["values"]
        if i0 >= len(vals) - 1:
            return vals[-1]
        frac = x - i0
        return vals[i0] * (1.0 - frac) + vals[i0 + 1] * frac
    if cv["kind"] == "constant":
        return cv["value"]
    # streamed: evaluate hermite
    mc = parsed["muscle_clip"]
    frames = parse_streamed(mc["clip"]["streamed"]["data"])
    return sample_streamed(frames, t).get(curve_index, float("nan"))


# ---------------------------------------------------------------------------
# CLI: inspect a raw dump (debug helper)
# ---------------------------------------------------------------------------

if __name__ == "__main__":
    import sys

    if len(sys.argv) != 2:
        print("usage: py muscle_clip.py <raw_object_dump.bin>")
        raise SystemExit(2)
    data = open(sys.argv[1], "rb").read()
    clip = parse_animation_clip(data)
    for k, v in muscle_summary(clip).items():
        print(f"{k}: {v}")
