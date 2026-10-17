"""Build shared GLB animations from Unity-native samples, without retargeting math.

This writes only the explicitly requested output. Run by the sole asset exporter.
Example: py tools/import_native_animation_bake.py --manifest <baked/manifest.json>
         --base <static.glb> --semantic-order <old.anim.glb> --output <new.anim.glb>
"""
import argparse
from collections import defaultdict
import json
from pathlib import Path

from export_animation import build_animation, conv_q, conv_v3, load_glb, save_glb


def suffix_paths(nodes):
    parents = {c: i for i, n in enumerate(nodes) for c in n.get("children", [])}
    result = defaultdict(set)
    for i in range(len(nodes)):
        names = []
        current = i
        while current is not None:
            names.insert(0, nodes[current].get("name", ""))
            result["/".join(names)].add(i)
            current = parents.get(current)
    return result


def moving(track):
    return any(any(abs(a-b) > 1e-7 for a, b in zip(row, values[0]))
               for key in ("positions", "rotations", "scales")
               for values in [track[key]] for row in values[1:])


def build(manifest_path, base_path, order_path, output_path):
    manifest_path = Path(manifest_path)
    manifest = json.loads(manifest_path.read_text(encoding="utf-8-sig"))
    if any(c["status"] != "BAKED_NATIVE" for c in manifest["clips"]):
        raise ValueError("Native bake has pending source clips; refusing partial replacement.")
    doc, buffer = load_glb(str(base_path))
    order_doc, _ = load_glb(str(order_path))
    order = [a["name"] for a in order_doc.get("animations", [])]
    paths = suffix_paths(doc["nodes"])
    animations = {}
    diagnostics = []
    for record in manifest["clips"]:
        baked = json.loads((manifest_path.parent / record["file"]).read_text(encoding="utf-8-sig"))
        names = record["source"].get("semanticNames", [])
        if not names:
            raise ValueError("Source clip has no controller-backed semantic name: " + record["file"])
        times = baked["times"]
        if not times or abs(times[-1] - baked["stopTime"]) > 1e-6:
            raise ValueError("Native sampling does not end at the original clip stop.")
        channels = []
        for track in baked["tracks"]:
            active = track.get("humanBone") or track.get("sourceTransformBinding") or moving(track)
            if not active:
                continue
            targets = paths.get(track["path"], set())
            if len(targets) != 1:
                raise ValueError(f"Original animated path {track['path']!r} has {len(targets)} GLB matches; no leaf-name fallback.")
            node = next(iter(targets))
            if "matrix" in doc["nodes"][node]:
                raise ValueError("Animated GLB node must expose TRS rather than matrix: " + track["path"])
            for source, target, convert in (("positions", "translation", conv_v3),
                                             ("rotations", "rotation", conv_q),
                                             ("scales", "scale", tuple)):
                if len(track[source]) != len(times):
                    raise ValueError("Native sample count differs from time count.")
                values = [convert(row) for row in track[source]]
                default = {"translation": [0, 0, 0], "rotation": [0, 0, 0, 1], "scale": [1, 1, 1]}[target]
                rest = doc["nodes"][node].get(target, default)
                # Unchanged local components already exist in the original GLB.
                # Retain constant channels whenever native pose differs from it.
                if all(all(abs(a-b) <= 1e-7 for a,b in zip(row,rest)) for row in values):
                    continue
                channels.append((node, target, [v for row in values for v in row]))
        pairs = [(node, prop) for node, prop, _ in channels]
        if len(pairs) != len(set(pairs)):
            raise ValueError("Native bake generated duplicate node/property channels.")
        if not channels:
            raise ValueError("No native animation channels survived exact-path binding.")
        for name in names:
            if name in animations:
                raise ValueError("Multiple original clips claim the same semantic state: " + name)
            animation = build_animation(doc, buffer, name, times, channels)
            animation["extras"] = {
                "SWGOH_events": [{"time": e["time"], "functionName": e["functionName"], "data": e.get("data", "")}
                                  for e in baked.get("events", [])],
                "SWGOH_nativeBake": {"source": record["source"], "unityVersion": baked["unityVersion"],
                                     "avatarMode": baked["avatarMode"], "stopTime": baked["stopTime"],
                                     "loopTime": baked["loopTime"]},
            }
            animations[name] = animation
        diagnostics.append({"source": record["file"], "names": names, "channels": len(channels), "samples": len(times)})
    missing = set(order) - set(animations)
    if missing:
        raise ValueError("Existing semantic clips missing from native bake: " + ", ".join(sorted(missing)))
    order.extend(n for n in animations if n not in order)
    doc["animations"] = [animations[name] for name in order]
    Path(output_path).parent.mkdir(parents=True, exist_ok=True)
    save_glb(str(output_path), doc, buffer)
    return {"character": manifest["character"], "output": str(output_path), "clips": diagnostics}


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    for field in ("manifest", "base", "semantic-order", "output"):
        parser.add_argument("--" + field, required=True)
    args = parser.parse_args()
    print(json.dumps(build(args.manifest, args.base, args.semantic_order, args.output), indent=2))
