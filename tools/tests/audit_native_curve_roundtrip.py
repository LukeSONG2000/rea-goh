"""Compare independent original compressed scalar decoding with Unity curve readback.

Usage: py tools/tests/audit_native_curve_roundtrip.py <native_sources/character/manifest.json>
The native sampler also validates streamed Hermite interiors against source cubics.
"""
import bisect
import json
from pathlib import Path
import sys

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from muscle_clip import parse_streamed, _streamed_segments


def sampler(raw):
    packed = raw["m_MuscleClip"]["m_Clip"]["data"]
    sc = packed["m_StreamedClip"]["curveCount"]
    dense = packed["m_DenseClip"]
    dc = dense["m_CurveCount"]
    frames = parse_streamed(packed["m_StreamedClip"]["data"])
    segments = _streamed_segments(frames)
    warmup = {c["index"]: c for frame in frames if frame["time"] <= -1e30 for c in frame["curves"]}
    def sample(index, time):
        if index < sc:
            times, keys = segments[index]
            if time < times[0]:
                before = warmup[index]
                if any(before["coeff"]) or before["value"] != keys[0][1]:
                    raise ValueError("Source pre-origin value is not proven constant")
                return before["value"]
            key = keys[max(0, bisect.bisect_right(times, time)-1)]
            t, value, coeff = key
            dt = time-t
            return value+coeff[2]*dt+coeff[1]*dt*dt+coeff[0]*dt*dt*dt
        index -= sc
        if index < dc:
            x = max(0, (time-dense["m_BeginTime"])*dense["m_SampleRate"])
            frame = min(int(x), dense["m_FrameCount"]-1)
            next_frame = min(frame+1, dense["m_FrameCount"]-1)
            values = dense["m_SampleArray"]
            return values[frame*dc+index]*(1-(x-frame))+values[next_frame*dc+index]*(x-frame)
        return packed["m_ConstantClip"]["data"][index-dc]
    return sample


def audit(manifest_path):
    directory = Path(manifest_path).parent
    manifest = json.loads((directory/"baked/manifest.json").read_text(encoding="utf-8-sig"))
    result = []
    for clip in manifest["clips"]:
        if clip["status"] != "BAKED_NATIVE":
            result.append({"source": clip["source"], "status": "PENDING_ORIGINAL"})
            continue
        baked_path = directory/"baked"/clip["file"]
        baked = json.loads(baked_path.read_text(encoding="utf-8-sig"))
        source = json.loads((directory/clip["source"]["file"]).read_text(encoding="utf-8-sig"))["raw"]
        readback = json.loads(baked_path.with_name(baked_path.stem+".clip-readback.json").read_text(encoding="utf-8-sig"))["AnimationClip"]
        curves = {(c["path"], c["attribute"]): c["curve"]["m_Curve"] for c in readback["m_FloatCurves"] if c["classID"] == 95}
        evaluate = sampler(source)
        count = 0
        max_error = 0
        failures = []
        for binding in baked["sourceCurveBindings"]:
            if binding["type"] != "Animator":
                continue
            key = binding["path"], binding["propertyName"]
            if key not in curves:
                failures.append({"binding": key, "error": "missing readback"})
                continue
            for frame in curves[key]:
                expected = evaluate(binding["sourceCurveIndex"], frame["time"])
                error = abs(frame["value"]-expected)
                max_error = max(max_error, error)
                count += 1
                if error > 1e-5*max(1, abs(expected)):
                    failures.append({"binding": key, "time": frame["time"], "source": expected, "readback": frame["value"]})
        result.append({"source": clip["source"], "keys": count, "maxAbsoluteError": max_error,
                       "status": "PASS" if count and not failures else "FAIL", "failures": failures})
    return result


if __name__ == "__main__":
    output = audit(sys.argv[1])
    print(json.dumps(output, indent=2))
    sys.exit(0 if output and all(x["status"] == "PASS" for x in output) else 1)
