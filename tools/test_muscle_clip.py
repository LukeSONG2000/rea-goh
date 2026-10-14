"""Test: parse muscle clips from the SWGoH shared animation container.

Run:  py tools/test_muscle_clip.py

Covers the four validation clips (humanoid muscle-only, humanoid mixed,
non-humanoid streamed, ship streamed) plus the legacy FX edge case, and
cross-checks the raw parser against UnityPy's typetree reader bit-for-bit.
"""

import math
import sys
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
sys.path.insert(0, str(TOOLS))

import UnityPy

from muscle_clip import (  # noqa: E402
    binding_dimensions,
    binding_for_curve,
    curve_values,
    dense_frames,
    muscle_slot_curves,
    muscle_summary,
    parse_animation_clip,
    parse_streamed,
    sample_curve,
    sample_streamed,
)

BUNDLE_GLOB = r"C:\Users\PPfavorite\AppData\LocalLow\Unity\Electronic Arts_SWGoH\shared_animationcontainer\*\__data"

TARGETS = [
    "hmn_homescrn_any01",   # humanoid, dense muscle curves + weapon transform layer
    "hmn_pst_var01_idle",   # humanoid idle, dense + streamed(1) + constant
    "gen_astromech_idle",   # non-humanoid: streamed sparse transform keys + constant
    "shp_shared_aoe_atk",   # ship: streamed sparse transform keys + constant
    "saber_on_01",          # legacy FX clip: no muscle data, float curves only
]

_failures = []


def check(cond, label):
    status = "ok " if cond else "FAIL"
    print(f"    [{status}] {label}")
    if not cond:
        _failures.append(label)


def load_clips():
    import glob

    paths = sorted(glob.glob(BUNDLE_GLOB), key=lambda p: Path(p).stat().st_mtime)
    assert paths, "shared_animationcontainer bundle not found"
    env = UnityPy.load(paths[-1])
    clips = {}
    raws = {}
    for obj in env.objects:
        if obj.type.name != "AnimationClip":
            continue
        tt = obj.read_typetree()
        clips[tt["m_Name"]] = tt
        raws[tt["m_Name"]] = obj.get_raw_data()
    return clips, raws


def test_clip(name, tt, raw):
    print(f"\n=== {name} ===")
    parsed = parse_animation_clip(raw)
    s = muscle_summary(parsed)
    print(
        f"    stop={s['stop_time']:.4f}s rate={s['sample_rate']:.0f}Hz "
        f"frames={s['frame_count']} dense_curves={s['curve_count']} "
        f"const={s['constant_count']} streamed={s['streamed_curves']} "
        f"bindings={s['binding_count']}"
    )
    print(
        f"    bytes: raw={parsed['raw_size']} consumed={parsed['bytes_consumed']} "
        f"muscle_declared={s['declared_muscle_clip_size']} "
        f"muscle_parsed={s['parsed_muscle_clip_bytes']}"
    )

    # byte-exactness: parser consumed the whole object
    check(parsed["bytes_consumed"] == parsed["raw_size"], "parsed full raw object")

    # cross-check against UnityPy typetree (bit-exact)
    tmc = tt["m_MuscleClip"]
    mc = parsed["muscle_clip"]
    check(abs(mc["stop_time"] - tmc["m_StopTime"]) < 1e-9, "stopTime == typetree")
    td = tmc["m_Clip"]["data"]["m_DenseClip"]
    check(mc["clip"]["dense"]["frame_count"] == td["m_FrameCount"], "frameCount == typetree")
    check(mc["clip"]["dense"]["curve_count"] == td["m_CurveCount"], "curveCount == typetree")
    tsa = td["m_SampleArray"]
    check(
        mc["clip"]["dense"]["sample_array"] == tsa,
        f"sample array == typetree ({len(tsa)} floats)",
    )
    tgb = tt["m_ClipBindingConstant"]["genericBindings"]
    gb = parsed["bindings"]["generic_bindings"]
    check(len(gb) == len(tgb), f"bindings count == typetree ({len(tgb)})")

    # sanity criteria
    if s["declared_muscle_clip_size"] > 0:
        check(0.5 <= s["stop_time"] <= 30.0, "stopTime plausible (0.5-30s)")
        check(
            s["sample_rate"] in (24.0, 30.0, 60.0), f"sampleRate in 24/30/60 ({s['sample_rate']})"
        )
        expect_frames = round(s["stop_time"] * s["sample_rate"]) + 1
        check(
            abs(s["frame_count"] - expect_frames) <= 2,
            f"frames ~ stopTime*rate ({s['frame_count']} vs {expect_frames}+1)",
        )
        if s["curve_count"]:
            check(
                s["sample_count"] == s["frame_count"] * s["curve_count"],
                "samples == frames * denseCurves",
            )
            lo = min(mc["clip"]["dense"]["sample_array"])
            hi = max(mc["clip"]["dense"]["sample_array"])
            check(
                -3.15 <= lo and hi <= 3.15,
                f"dense values in [-3.15, 3.15] ({lo:.3f}..{hi:.3f})",
            )

    # bindings / curve space consistency
    dims = binding_dimensions(parsed)
    total_curves = s["streamed_curves"] + s["curve_count"] + s["constant_count"]
    check(
        sum(dims) == total_curves,
        f"binding dims sum {sum(dims)} == total curves {total_curves}",
    )
    if total_curves:
        b, comp = binding_for_curve(parsed, total_curves - 1)
        check(b is not None, f"binding lookup works (last curve -> path={b['path']:#x})")

    # humanoid: muscle slot map non-empty and points at valid curves
    slots = muscle_slot_curves(parsed)
    if s["binding_count"] and any(b["type_id"] == 95 for b in gb):
        check(len(slots) > 0, f"muscle slot map: {len(slots)} slots driven")
        check(
            all(c < total_curves for c in slots.values()),
            "all slot->curve indices in range",
        )
        # RootT/RootQ slots (7-13) should be animated for humanoid clips
        driven_root = [c for sl, c in sorted(slots.items()) if 7 <= sl <= 13]
        check(bool(driven_root), f"RootT/RootQ slots driven (curves {driven_root[:7]})")

    # per-section value extraction
    for ci in range(min(total_curves, 4)):
        cv = curve_values(parsed, ci)
        check(cv["kind"] in ("dense", "streamed", "constant"), f"curve {ci} -> {cv['kind']}")

    # streamed decode
    if s["streamed_curves"]:
        frames = parse_streamed(mc["clip"]["streamed"]["data"])
        real = [f["time"] for f in frames if -1e30 < f["time"] < 1e30]
        check(len(frames) >= 2, f"streamed frames decoded: {len(frames)}")
        check(
            all(real[i] <= real[i + 1] for i in range(len(real) - 1)),
            f"streamed times monotonic ({real[0]:.3f}..{real[-1]:.3f}s, {len(real)} keys)",
        )
        check(
            abs(real[-1] - s["stop_time"]) < 0.5,
            "last streamed key near stopTime",
        )
        vals = sample_streamed(frames, real[0])
        check(bool(vals), f"streamed sample at t={real[0]:.3f}: {len(vals)} curves")
        # range check on streamed transform values (positions/quats)
        allv = [
            c["value"] for f in frames for c in f["curves"]
        ]
        check(
            min(allv) > -1000 and max(allv) < 1000,
            f"streamed values sane ({min(allv):.3f}..{max(allv):.3f})",
        )
    if s["constant_count"]:
        cv = curve_values(parsed, total_curves - 1)
        check(cv["kind"] == "constant", f"constant tail value = {cv['value']:.4f}")

    # events survived
    for ev in parsed["events"]:
        print(f"    event: t={ev['time']:.2f} {ev['function_name']}({ev['data']!r})")
    return parsed


def main():
    clips, raws = load_clips()
    print(f"bundle loaded: {len(clips)} AnimationClips")
    missing = [n for n in TARGETS if n not in clips]
    assert not missing, f"clips missing: {missing}"

    for name in TARGETS:
        test_clip(name, clips[name], raws[name])

    # humanoid muscle spot-check: RootQ values (slots 10-13) should be quat-like
    print("\n=== humanoid spot check (hmn_homescrn_any01) ===")
    p = parse_animation_clip(raws["hmn_homescrn_any01"])
    slots = muscle_slot_curves(p)
    rootq_curves = [slots[sl] for sl in (10, 11, 12, 13)]
    print(f"    RootQ xyzw -> dense curves {rootq_curves}")
    fr = dense_frames(p)
    ok_norm = 0
    for fvals in fr:
        x, y, z, w = (fvals[c] for c in rootq_curves)
        if abs(x * x + y * y + z * z + w * w - 1.0) < 1e-3:
            ok_norm += 1
    check(ok_norm == len(fr), f"RootQ norm==1 for {ok_norm}/{len(fr)} frames")

    # sampling continuity at 3 times
    t_end = muscle_summary(p)["stop_time"]
    for t in (0.0, t_end / 2, t_end):
        v = sample_curve(p, rootq_curves[0], t)
        check(abs(v) <= 1.0 + 1e-6, f"sample_curve(RootQ.x, t={t:.3f}) = {v:+.4f}")

    # streamed hermite spot-check: ship rotation (streamed curves 0-3) must stay
    # a unit quaternion across the whole clip -> validates the segment evaluator
    print("\n=== streamed hermite check (shp_shared_aoe_atk) ===")
    shp = parse_animation_clip(raws["shp_shared_aoe_atk"])
    t_end = shp["muscle_clip"]["stop_time"]
    worst = 0.0
    for i in range(41):
        t = t_end * i / 40
        q = [sample_curve(shp, ci, t) for ci in (0, 1, 2, 3)]
        worst = max(worst, abs(math.sqrt(sum(v * v for v in q)) - 1.0))
    check(worst < 0.02, f"ship rotation norm 1.0 across clip (worst dev {worst:.4f})")

    print()
    if _failures:
        print(f"FAILED ({len(_failures)}):")
        for f in _failures:
            print("  -", f)
        sys.exit(1)
    print("ALL CHECKS PASSED")


if __name__ == "__main__":
    main()
