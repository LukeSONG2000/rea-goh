import hashlib
import json
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch
from prepare_captured_trial import read_capture, prepare


class CaptureIntegrityTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.file = Path(self.tmp.name) / 'capture.pb'
        self.file.write_bytes(b'\x08\x07')
        self.meta = {'kind': 'inventory-proto-captured', 'encoding': 'direct-protobuf-v2',
                     'bytes': 2, 'sha256': hashlib.sha256(self.file.read_bytes()).hexdigest()}

    def save_meta(self):
        Path(str(self.file) + '.json').write_text(json.dumps(self.meta), encoding='utf-8')

    def test_exact_bytes_and_kind(self):
        self.save_meta()
        self.assertEqual(read_capture(self.file, 'inventory-proto-captured')[0], b'\x08\x07')
        with self.assertRaisesRegex(ValueError, 'Wrong capture type'):
            read_capture(self.file, 'original-battle-proto-captured')

    def test_tampered_or_truncated_capture(self):
        self.save_meta()
        for payload in (b'\x08\x08', b'\x08'):
            self.file.write_bytes(payload)
            with self.assertRaisesRegex(ValueError, 'integrity mismatch'):
                read_capture(self.file, 'inventory-proto-captured')

    def test_legacy_capture_rejected(self):
        self.meta['encoding'] = 'legacy-deepclone'
        self.save_meta()
        with self.assertRaisesRegex(ValueError, 'legacy clone'):
            read_capture(self.file, 'inventory-proto-captured')

    def test_invalid_configuration_precedes_file_access(self):
        for order in [[], [0, 0], [-1, 0], [0, 2], [True], '0', list(range(513))]:
            with self.assertRaisesRegex(ValueError, 'permutation'):
                prepare('missing', 'missing', 'missing', 'missing', 8, 'visual', 60, False, ai_priority_order=order)
        with self.assertRaisesRegex(ValueError, 'visual trial'):
            prepare('missing', 'missing', 'missing', 'missing', 8, 'headless', 60, False, ai_priority_order=[0])
        with self.assertRaisesRegex(ValueError, 'visual loop'):
            prepare('missing', 'missing', 'missing', 'missing', 8, 'headless', 60, False, True)
        with self.assertRaisesRegex(ValueError, 'visual loop'):
            prepare('missing', 'missing', 'missing', 'missing', 8, 'headless', 60, False, False, True)
        with self.assertRaisesRegex(ValueError, 'visual loop'):
            prepare('missing', 'missing', 'missing', 'missing', 8, 'headless', 60, False, False, False, True)
        with self.assertRaisesRegex(ValueError, 'either'):
            prepare('missing', 'missing', 'missing', 'missing', 8, 'visual', 60, False, False, True, True)
        for squad, mode, duration, auto in [(256, 'visual', 60, False), (1, 'visual', 0, False), (1, 'headless', 60, True)]:
            with self.assertRaises(ValueError):
                prepare('missing', 'missing', 'missing', 'missing', squad, mode, duration, auto)

    def test_visual_hud_precedes_observer_and_cleans_up(self):
        root = Path(self.tmp.name)
        dump = root / 'metadata.cs'
        dump.write_text('test schema fixture')
        capture = (b'\x08\x07', {'mission': {}}, {'sha256': 'test'})
        with patch('prepare_captured_trial.read_capture', return_value=capture), \
             patch('prepare_captured_trial.schema_from_dump', return_value={}), \
             patch('prepare_captured_trial.materialize', side_effect=lambda data, *_: data):
            visual = prepare('battle', 'inventory', dump, root / 'visual', 1, 'visual', 15, False)
            headless = prepare('battle', 'inventory', dump, root / 'headless', 1, 'headless', 15, False)
        source = Path(visual['source']).read_text(encoding='utf-8')
        self.assertTrue(visual['informationDisplay'])
        self.assertLess(source.index('globalThis.stopBattleInfo='), source.index('globalThis.readRuntimeSnapshot='))
        self.assertIn("kind:'trial-hud-cleanup'", source)
        self.assertIn("cleanup.errors.push('HUD cleanup:", source)
        self.assertFalse(headless['informationDisplay'])
        self.assertNotIn('globalThis.stopBattleInfo=', Path(headless['source']).read_text(encoding='utf-8'))


if __name__ == '__main__':
    unittest.main()
