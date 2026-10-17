import hashlib
import json
from pathlib import Path
import tempfile
import unittest
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
        for squad, mode, duration, auto in [(256, 'visual', 60, False), (1, 'visual', 0, False), (1, 'headless', 60, True)]:
            with self.assertRaises(ValueError):
                prepare('missing', 'missing', 'missing', 'missing', squad, mode, duration, auto)


if __name__ == '__main__':
    unittest.main()
