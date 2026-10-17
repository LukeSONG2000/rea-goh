"""A source mesh without normals (original starSkyBox) must not be dropped."""
import sys
import unittest
from pathlib import Path
from types import SimpleNamespace

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from swgoh_extract import CharacterExporter, GLTFBuilder


class OptionalMeshChannels(unittest.TestCase):
    def export(self, normals, uv):
        exporter = CharacterExporter.__new__(CharacterExporter)
        exporter.g = GLTFBuilder()
        source = SimpleNamespace(m_VertexCount=3,
            m_Vertices=[(0, 0, 0), (1, 0, 0), (0, 1, 0)],
            m_Normals=normals, m_UV0=uv, m_BoneIndices=None, m_BoneWeights=None)
        return exporter.export_mesh_primitive(None, source, 0)

    def test_missing_channels_keep_original_positions(self):
        primitive = self.export(None, None)
        self.assertEqual(set(primitive['attributes']), {'POSITION'})
        self.assertEqual(primitive['material'], 0)

    def test_present_source_channels_are_retained(self):
        primitive = self.export([(0, 0, 1)] * 3, [(0, 0), (1, 0), (0, 1)])
        self.assertEqual(set(primitive['attributes']), {'POSITION', 'NORMAL', 'TEXCOORD_0'})


if __name__ == '__main__':
    unittest.main()
