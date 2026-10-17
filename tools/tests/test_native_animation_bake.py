"""Controlled transport fixtures; these tests do not assert original pose fidelity."""
import json
from pathlib import Path
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))
from export_animation import load_glb, save_glb
from import_native_animation_bake import build


class NativeTransport(unittest.TestCase):
    def setUp(self):
        directory = ROOT / "logs" / "pose_recovery"
        directory.mkdir(parents=True, exist_ok=True)
        self.temp = tempfile.TemporaryDirectory(dir=directory)
        self.path = Path(self.temp.name)
        self.base = {"asset": {"version": "2.0"}, "nodes": [{"name": "Scaler", "children": [1]},
                     {"name": "pelvicJNT"}], "scenes": [{"nodes": [0]}], "scene": 0}
        save_glb(str(self.path / "base.glb"), self.base, bytearray())
        self.base["animations"] = [{"name": "Idle"}]
        save_glb(str(self.path / "old.glb"), self.base, bytearray())
        self.manifest = {"character": "controlled_fixture", "clips": [
            {"status": "BAKED_NATIVE", "file": "clip.json", "source": {"semanticNames": ["Idle"]}}]}
        self.baked = {"times": [0, 1], "stopTime": 1, "unityVersion": "fixture", "avatarMode": "fixture", "loopTime": True,
                      "events": [{"time": .25, "functionName": "FISTON", "data": "original-event-fixture"}],
                      "tracks": [{"path": "pelvicJNT", "name": "pelvicJNT", "humanBone": True,
                                  "positions": [[1, 2, 3], [1, 2.1, 3]], "rotations": [[0, 0, 0, 1]] * 2, "scales": [[1, 1, 1]] * 2}]}

    def tearDown(self):
        self.temp.cleanup()

    def run_build(self):
        (self.path / "manifest.json").write_text(json.dumps(self.manifest))
        (self.path / "clip.json").write_text(json.dumps(self.baked))
        return build(self.path / "manifest.json", self.path / "base.glb", self.path / "old.glb", self.path / "new.glb")

    def test_event_and_unique_tracks(self):
        self.run_build()
        doc, _ = load_glb(str(self.path / "new.glb"))
        clip = doc["animations"][0]
        self.assertEqual(clip["name"], "Idle")
        self.assertEqual(len(clip["channels"]), 1)  # Unchanged original rotation/scale need no channels.
        self.assertEqual(clip["extras"]["SWGOH_events"], self.baked["events"])
        position = doc["accessors"][clip["samplers"][0]["output"]]
        self.assertEqual(position["min"], [-1, 2, 3])

    def test_no_leaf_fallback(self):
        self.baked["tracks"][0]["path"] = "different_parent/pelvicJNT"
        with self.assertRaisesRegex(ValueError, "no leaf-name fallback"):
            self.run_build()
        self.assertFalse((self.path / "new.glb").exists())

    def test_no_partial_fallback(self):
        self.manifest["clips"][0]["status"] = "PENDING_ORIGINAL"
        with self.assertRaisesRegex(ValueError, "partial replacement"):
            self.run_build()


if __name__ == "__main__":
    unittest.main()
