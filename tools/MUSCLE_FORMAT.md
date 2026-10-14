# SWGoH AnimationClip / muscle clip binary format

Reverse-engineered layout of the AnimationClip objects inside
`shared_animationcontainer` (Unity **2022.3.62f2**, little-endian, typetree-style
serialization with type-dependent 4-byte alignment). Parser: `tools/muscle_clip.py`
(`parse_animation_clip(raw_bytes)`). Tests: `tools/test_muscle_clip.py`.

References used to confirm semantics (fetched via jsdelivr mirror / zread):

- `mafaca/UtinyRipper` — `uTinyRipperCore/Parser/Classes/AnimationClip/**`
  (`StreamedClip.cs`, `StreamedFrame.cs`, `StreamedCurveKey.cs`,
  `AnimationClipBindingConstant.cs`, `GenericBinding.cs`, `TransformType.cs`,
  `HumanoidMuscleType.cs`) and
  `uTinyRipperCore/Converters/Classes/AnimationClip/AnimationClipConverter.cs`
- `Perfare/AssetStudio` — reads the same structures via typetree
  (`AssetStudioUtility/SerializedTypeHelper.cs`)
- The authoritative byte layout is the AnimationClip **TypeTree embedded in the
  bundle itself** (dumped with UnityPy `SerializedType.node.traverse()`);
  the UnityPy 1.25.4 typetree reader in fact parses these clips fine, so the raw
  parser is cross-checked bit-for-bit against it in the test script.

## TL;DR findings

- All 173 muscle clips + 5 legacy FX clips of the container parse byte-exactly;
  the raw parser consumes every object to the last byte (`consumed == raw_size`).
- Curve data lives in three sections of `m_MuscleClip.m_Clip`:
  `m_StreamedClip` (sparse hermite keys), `m_DenseClip` (fixed-rate samples),
  `m_ConstantClip` (never-changing values). They form ONE curve index space:
  `[streamed][dense][constant]`.
- `m_ClipBindingConstant.genericBindings` (which IS present in the raw stream)
  maps onto that index space **by list position**, each Transform binding
  consuming 3-4 curves; muscle bindings (typeID 95) have `path=0` and
  `attribute` = humanoid value-array slot (HumanoidMuscleType).
  `ClipMuscleConstant.m_IndexArray[slot]` gives the curve driving each slot.
- `m_MuscleClipSize` is the runtime blob size, **not** the on-disk struct size:
  observed `on-disk + 388` bytes for every muscle clip (and 0 for the 5 legacy
  FX clips whose `m_MuscleClip` struct is still on disk, just empty). Do not use
  it as a parse bound.

## 1. Object layout (hmn_homescrn_any01 concrete offsets)

`raw` = `ObjectReader.get_raw_data()` (28,564 bytes for this clip). `@` =
absolute offset into `raw`.

| offset | size | field |
|---:|---|---|
| 0 | 4+18 | `m_Name` string (u32 len, utf-8 bytes, align4 -> 24) |
| 24 | 3 | `m_Legacy`, `m_Compressed`, `m_UseHighQualityCurve` (bools) + align4 -> 28 |
| 28 | 7x4 | empty vectors: `m_RotationCurves`, `m_CompressedRotationCurves`, `m_EulerCurves`, `m_PositionCurves`, `m_ScaleCurves`, `m_FloatCurves`, `m_PPtrCurves` (u32 count + entries + align4) |
| 56 | 4 | `m_SampleRate` float (24.0) |
| 60 | 4 | `m_WrapMode` int |
| 64 | 24 | `m_Bounds` (m_Center xyz, m_Extent xyz) |
| 88 | 4 | `m_MuscleClipSize` uint (24616; = runtime size, see TL;DR) |
| **92** | 24228 | **`m_MuscleClip` (ClipMuscleConstant)** — see section 2 |
| 24320 | 4+148x28 | `m_ClipBindingConstant.genericBindings` (u32 count + entries, align4; entry layout below) |
| 28468 | 4 | `pptrCurveMapping` count (0 here; entries are PPtr fileID i32 + pathID u64, align4) |
| 28472 | 2 | `m_HasGenericRootTransform`, `m_HasMotionFloatCurves` + align4 -> 28476 |
| 28476 | ... | `m_Events` vector: u32 count + per event: f32 time, string functionName, string data, PPtr (12B), f32 floatParameter, i32 intParameter, i32 messageOptions -> ends exactly at 28564 |

Non-muscle (legacy FX) clips put their data in `m_FloatCurves` etc.; each curve
entry is an AnimationCurve (2022.3 Keyframe = 28 B: time, value, inSlope,
outSlope, weightedMode, inWeight, outWeight) followed by path/attribute strings.

### GenericBinding entry (28 B, in genericBindings vector)

```
u32   path          # CRC32 (zlib) of transform path relative to Animator root; 0 for muscles
u32   attribute     # Transform: 1=Translation 2=Rotation 3=Scaling 4=Euler
                    # Animator muscle (typeID 95, customType 8): HumanoidMuscleType slot
PPtr  script        # i32 m_FileID + u64 m_PathID (12 B)
i32   typeID        # 4=Transform, 95=Animator, ...
u8    customType    # 8 = AnimatorMuscle
u8    isPPtrCurve
u8    isIntCurve
u8    isSerializeReferenceCurve   # (align4 after entry; 28 % 4 == 0 -> no pad)
```

## 2. ClipMuscleConstant (m_MuscleClip) — offsets for the example clip

| offset (rel. to 92) | field |
|---:|---|
| +0 (`@92`) | `m_DeltaPose` (HumanPose, 1088 B here): xform m_RootX (t xyz, q xyzw, s xyz = 40 B), float3 m_LookAtPosition, float4 m_LookAtWeight, vector<HumanGoal> m_GoalArray (goal = xform + 4 floats + float3 + float = 64 B), 2x HandPose m_Left/RightHandPose (xform m_GrabX + vector<float> m_DoFArray + 4 floats), vector<float> m_DoFArray (55 muscles), vector<float3> m_TDoFArray (21 = root 3 + hands 2x9) |
| +1088 (`@1180`) | `m_StartX` xform (40 B) |
| +1128 | `m_StopX` xform |
| +1168 | `m_LeftFootStartX` xform |
| +1208 | `m_RightFootStartX` xform |
| +1248 | `m_AverageSpeed` float3 |
| +1260 (`@1352`) | `m_Clip` (see section 3) |
| `@21896` | `m_StartTime`, `m_StopTime` (2.1667), `m_OrientationOffsetY`, `m_Level`, `m_CycleOffset`, `m_AverageAngularSpeed` (6 floats) |
| `@21920` | `m_IndexArray`: u32 count (200) + i32 per slot. **slot = HumanoidMuscleType value, entry = curve index driving it, -1 = unused.** 200 = humanoid value-array size for this rig (127 driven) |
| `@22724` | `m_ValueArrayDelta`: u32 count (197 = total curves) + per curve {f32 start, f32 stop}. start = curve value at t=0, stop = value at t=stopTime (verified: start == dense frame 0 / constant value; stop == last dense frame). Used for loop blending |
| `@24304` | `m_ValueArrayReferencePose`: u32 count + floats (empty here; additive reference pose) |
| `@24308` | 11 bools: `m_Mirror`, `m_LoopTime`, `m_LoopBlend`, `m_LoopBlendOrientation`, `m_LoopBlendPositionY`, `m_LoopBlendPositionXZ`, `m_StartAtOrigin`, `m_KeepOriginalOrientation`, `m_KeepOriginalPositionY`, `m_KeepOriginalPositionXZ`, `m_HeightFromFeet` + align4 -> 24320 |

No alignment padding occurs inside ClipMuscleConstant (every field is a
4-multiple); only the final bool block pads to 4.

## 3. m_Clip — the three data sections

```
@1352  m_StreamedClip:  u32 wordCount + words (u32 each), u16 curveCount, u16 discreteCurveCount
@1368  m_DenseClip:     i32 m_FrameCount, u32 m_CurveCount, f32 m_SampleRate, f32 m_BeginTime,
                        u32 sampleCount + f32 samples (frame-major: sample[frame*curveCount+curve])
@21476 m_ConstantClip:  u32 count + f32 values (values for constant curves)
```

Curve index space = `[streamed curves (curveCount)][dense curves][constant
curves]`. For `hmn_homescrn_any01`: streamed 0 + dense 93 + constant 104 = 197
curves; `m_ValueArrayDelta` has one entry per curve.

### Dense clip

Fixed-rate sampling: frame f covers time `f / m_SampleRate`; `m_FrameCount =
ceil(m_StopTime * m_SampleRate) + 1` (occasionally +2; 54 frames for
2.1667s*24). Values are muscle/pose values in roughly [-1, 1.4]; dense curves
0..11 here are three Transform rotation quaternions (weapon JNT layer, per the
genericBindings position mapping), curves 12+ are the humanoid root/muscle
values (verified: curves 15..18 = RootQ xyzw, unit norm on all 54 frames).

### Streamed clip

Sparse keys, encoded as a stream of frames packed into the u32 words:

```
repeat until end of words:
  f32  time                  # first frame = -FLT_MAX (0xFF7FFFFF) slope warm-up,
                             # last frame = +INF terminator
  u32  curveCount
  curveCount x StreamedCurveKey:
      i32  curveIndex        # index in the [streamed|dense|constant] space
      f32  coeff.x           # hermite coefficient
      f32  coeff.y
      f32  coeff.z = outSlope
      f32  value
```

A clip without streamed curves stores the 2-word sentinel `{0x7F800000 (+INF),
0}` and curveCount = 0. Non-humanoid clips (`gen_*`, `shp_*`) keep ALL their
animated data here (dense has 0 curves but still defines frameCount/sampleRate
= clip timing).

Each key encodes the cubic segment **from its own time to that key's next
appearance** (uTinyRipper `CalculateStreamedFrame` is the encoder):

```
dt = t - key.time
v(t) = key.value + coeff.z*dt + coeff.y*dt^2 + coeff.x*dt^3
```

The sentinel first frame (-FLT_MAX) and terminator frame (+INF) only exist for
slope warm-up / loop closure and are skipped when sampling. Verified: the ship
clip's streamed rotation quaternion keeps norm 1.0 across the whole timeline
(worst deviation 0.001) with this evaluator.

### Constant clip

One float per constant curve, in curve-index order; the driving binding is found
by continuing the genericBindings dimension walk past the dense curves.

## 4. Binding -> curve resolution (the algorithm)

1. `total = streamedCurveCount + denseCurveCount + constantCount`.
2. Walk `genericBindings` in order accumulating dimensions
   (`Transform attribute 1/3/4 -> 3, 2 -> 4, everything else -> 1`); the entry
   where the running sum passes curve index `i` is the binding for curve `i`,
   the overshoot is the vector component (x/y/z/w).
3. For muscle bindings, `ClipMuscleConstant.m_IndexArray[slot]` additionally
   tells which curve drives humanoid slot `slot` (HumanoidMuscleType:
   0-6 Motion T/Q, 7-13 RootT/RootQ xyzw, 14+ limbs 7-per-limb, then body
   muscles, finger DoF, translation DoF).
4. Transform path strings: hash = CRC32 of the path relative to the Animator
   root (`catalog/ANIMATIONS.md` has the cracked path table; AnimatorController
   `m_TOS` maps remaining hashes to full paths).

## 5. Validation results (py tools/test_muscle_clip.py)

| clip | stopTime | rate | frames | dense | const | streamed | bindings |
|---|---:|---:|---:|---:|---:|---:|---:|
| hmn_homescrn_any01 | 2.1667 s | 24 | 54 | 93 curves (5022 floats) | 104 | 0 | 148 (21 transform + 127 muscle) |
| hmn_pst_var01_idle | 1.3333 s | 24 | 33 | 79 curves (2607 floats) | 47 | 1 (3 keys) | 127 (muscle) |
| gen_astromech_idle | 3.2917 s | 24 | 81 | 0 | 186 | 4 (16 keys) | 57 (transform) |
| shp_shared_aoe_atk | 5.7917 s | 24 | 141 | 0 | 6 | 4 (16 keys) | 3 (transform; 10 FireProjectile events) |
| saber_on_01 (edge) | 1.0 s | – | 0 | 0 | 0 | 0 | 0 (+4 float curves) |

All five parse with `consumed == raw_size`, all fields bit-match UnityPy's
typetree read, dense value ranges stay in [-0.96, 1.40], RootQ norm == 1.0 on
all frames, streamed keys monotonic and ending at stopTime.

Container-wide: 173 muscle clips (24/30/60 Hz; 111/62/5 clips) + 5 legacy FX
clips (`saber_*`, muscle size 0).

## 6. Usage

```python
import sys; sys.path.insert(0, "tools")
from muscle_clip import parse_animation_clip, muscle_summary, curve_values, sample_curve

raw = obj.get_raw_data()                    # UnityPy ObjectReader
clip = parse_animation_clip(raw)
print(muscle_summary(clip)["stop_time"])
v = sample_curve(clip, curve_index=15, t=0.5)   # RootQ.x at t=0.5s
```

## 7. Open questions / leftovers

- `m_MuscleClipSize` = on-disk struct + **388** bytes, constant across all
  muscle clips (runtime blob overhead). Meaning of the 388 B not chased.
- Humanoid slot -> bone naming needs each character's Avatar
  (`m_Human`/muscle definitions in the `char_*_pre` bundles); slot indices here
  follow HumanoidMuscleType order but the rig-specific limb/muscle set (200
  slots vs Unity's stock layout) is only confirmed empirically.
- Frames can be `ceil(stop*rate)+1` or `+2` (loop blend keeps an extra frame);
  playback should trust `m_StopTime`/`m_SampleRate`, not frame count.
- Dense-curve playback interpolation: samples are per-frame values; Unity's
  runtime interpolation between dense frames is not exactly characterised here
  (linear assumed in `sample_curve`).
- Muscle value -> joint rotation conversion (Humanoid muscle ranges, mirroring
  into the exported glTF skeleton) is the next stage; this parser only yields
  raw per-slot values.
