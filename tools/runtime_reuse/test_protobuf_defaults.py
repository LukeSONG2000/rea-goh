import unittest
from pathlib import Path
from protobuf_defaults import materialize, schema_from_dump, varint


class DefaultsTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.schemas = schema_from_dump(Path(__file__).resolve().parents[2] / 'il2cpp/dump/cs/il2cpp.cs')

    def test_original_struct_contract(self):
        self.assertEqual(self.schemas['Stat'][3]['default'], 1)
        self.assertEqual(self.schemas['Stat'][7]['default'], -1)
        self.assertEqual(self.schemas['BucketItem'][1]['default'], '')
        self.assertEqual(self.schemas['BucketItem'][6]['default'], 1)

    def test_explicit_zero_and_negative_preserved(self):
        # This includes the invalid zero ID from the legacy capture: never repair it.
        wire = b'\x18\x00\x38' + varint(-7)
        result = materialize(wire, 'Stat', self.schemas)
        self.assertTrue(result.startswith(wire))
        self.assertNotIn(b'\x18\x01', result)

    def test_nested_repeated_messages(self):
        child = materialize(b'', 'Stat', self.schemas)
        key = varint(19 * 8 + 2)
        wire = (key + b'\x00') * 2
        self.assertEqual(materialize(wire, 'StatDef', self.schemas),
                         (key + varint(len(child)) + child) * 2)

    def test_unknown_bytes_preserved(self):
        wire = varint(900 * 8 + 2) + b'\x03\xff\x00\x80'
        self.assertTrue(materialize(wire, 'Stat', self.schemas).startswith(wire))

    def test_absent_message_not_created(self):
        self.assertEqual(materialize(b'', 'BattleData', self.schemas), b'')

    def test_idempotent(self):
        once = materialize(b'', 'BucketItem', self.schemas)
        self.assertEqual(materialize(once, 'BucketItem', self.schemas), once)

    def test_duplicate_explicit_fields_kept(self):
        wire = b'\x18\x01\x18\x02'
        self.assertTrue(materialize(wire, 'Stat', self.schemas).startswith(wire))

    def test_corruption_rejected(self):
        for wire in (b'\x00', b'\x80', b'\x0a\x05\x00', b'\x0d\x00', b'\x0b', b'\x80' * 10):
            with self.subTest(wire=wire), self.assertRaises(ValueError):
                materialize(wire, 'Stat', self.schemas)


if __name__ == '__main__':
    unittest.main()
