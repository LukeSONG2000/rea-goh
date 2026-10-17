"""Read-only GLB pose audit; measurements are diagnostics, not native fidelity assertions.

Run: py tools/tests/audit_animation_pose.py [character ...]
No asset writes. JSON goes to stdout; redirect reports under logs/.
"""
import collections
import json
import math
from pathlib import Path
import struct
import sys

import numpy as np

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))
from export_animation import load_glb, quat_mat


def read_value(doc, blob, index):
    accessor = doc["accessors"][index]
    view = doc["bufferViews"][accessor["bufferView"]]
    size = {"VEC3": 3, "VEC4": 4}[accessor["type"]]
    assert accessor["componentType"] == 5126
    return struct.unpack_from("<" + "f" * size, blob,
                              view.get("byteOffset", 0) + accessor.get("byteOffset", 0))


def positions(doc, blob, animation):
    overrides = {}
    duplicates = []
    if animation:
        for channel in animation["channels"]:
            target = channel["target"]
            key = (target["node"], target["path"])
            if key in overrides:
                duplicates.append(key)
            overrides[key] = read_value(doc, blob, animation["samplers"][channel["sampler"]]["output"])
    parents = {child: i for i, node in enumerate(doc["nodes"]) for child in node.get("children", [])}
    cache = {}
    def world(index):
        if index in cache:
            return cache[index]
        node = doc["nodes"][index]
        if "matrix" in node:
            local = np.array(node["matrix"]).reshape(4, 4).T
        else:
            local = np.eye(4)
            rotation = overrides.get((index, "rotation"), node.get("rotation", [0, 0, 0, 1]))
            scale = overrides.get((index, "scale"), node.get("scale", [1, 1, 1]))
            local[:3, :3] = np.array(quat_mat(rotation)) @ np.diag(scale)
            local[:3, 3] = overrides.get((index, "translation"), node.get("translation", [0, 0, 0]))
        cache[index] = world(parents[index]) @ local if index in parents else local
        return cache[index]
    names = ("pelvicJNT", "headJNT", "Left_hipJNT", "Left_kneeJNT", "Left_ankleJNT", "Left_ballJNT",
             "Right_hipJNT", "Right_kneeJNT", "Right_ankleJNT", "Right_ballJNT")
    points = {node["name"]: world(i)[:3, 3].tolist() for i, node in enumerate(doc["nodes"])
              if node.get("name") in names}
    angle = None
    if "headJNT" in points and "pelvicJNT" in points:
        delta = np.array(points["headJNT"]) - points["pelvicJNT"]
        angle = math.degrees(math.atan2(math.hypot(delta[0], delta[2]), delta[1]))
    return {"points": points, "pelvisHeadTiltDegrees": angle, "duplicateChannels": duplicates}


def audit(character):
    path = ROOT / "extracted" / character / (character + ".anim.glb")
    doc, blob = load_glb(str(path))
    animations = doc.get("animations", [])
    idle = next((a for a in animations if "idle" in a.get("name", "").lower()), None)
    counts = collections.Counter(n.get("name") for n in doc["nodes"])
    return {"character": character, "firstClip": animations[0]["name"] if animations else None,
            "idleClip": idle["name"] if idle else None,
            "duplicateNodeNames": {k: v for k, v in counts.items() if v > 1},
            "rest": positions(doc, blob, None), "idleT0": positions(doc, blob, idle)}


if __name__ == "__main__":
    characters = sys.argv[1:] or sorted(p.parent.name for p in (ROOT / "extracted").glob("*/*.anim.glb"))
    result = []
    for character in characters:
        try:
            result.append(audit(character))
        except (Exception, SystemExit) as error:
            result.append({"character": character, "error": str(error)})
    print(json.dumps(result, ensure_ascii=False, indent=2))
