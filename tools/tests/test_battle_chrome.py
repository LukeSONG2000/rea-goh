"""Verify exported HUD pixels against the original bundle, not another export."""
import hashlib
import json
import re
import unittest
from pathlib import Path

from PIL import Image
import UnityPy

ROOT = Path(__file__).resolve().parents[2]
ASSETS = ROOT / 'unity/SWGoH-Restored/Assets'


class BattleChromeSourceTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.proof = json.loads((ASSETS / 'Resources/BattlePresentation/sprite_provenance.json').read_text(encoding='utf-8'))
        cls.source = Path(cls.proof['bundle'])
        cls.environment = UnityPy.load(str(cls.source))
        cls.objects = {obj.path_id: obj for obj in cls.environment.objects}

    def source_crop(self, row):
        atlas = self.objects[int(row['atlasPathId'])].read_typetree()
        raw = next(s for s in atlas['mSprites'] if s['name'] == row['name'])
        self.assertEqual(raw, row['rect'])
        material_id = atlas['material']['m_PathID']
        self.assertEqual(material_id, int(row['materialPathId']))
        material = self.objects[material_id].read_typetree()
        texture = dict(material['m_SavedProperties']['m_TexEnvs'])['_MainTex']['m_Texture']
        self.assertEqual(texture['m_FileID'], 0)
        self.assertEqual(texture['m_PathID'], int(row['texturePathId']))
        image = self.objects[texture['m_PathID']].read().image.convert('RGBA')
        return image.crop((raw['x'], raw['y'], raw['x'] + raw['width'], raw['y'] + raw['height']))

    def test_source_provenance_and_all_nonmirrored_pixels(self):
        self.assertEqual(hashlib.sha256(self.source.read_bytes()).hexdigest(), self.proof['sourceSha256'])
        for row in self.proof['sprites']:
            raw = row['rect']
            source = self.source_crop(row)
            self.assertEqual(hashlib.sha256(source.tobytes()).hexdigest(), row['rawRgbaSha256'])
            path = ASSETS / 'Resources' / (row['resource'] + '.png')
            self.assertEqual(hashlib.sha256(path.read_bytes()).hexdigest(), row['pngSha256'])
            if not raw['mirrorHorizontal'] and not raw['mirrorVertical']:
                actual = Image.open(path).convert('RGBA')
                self.assertEqual(actual.size, source.size)
                self.assertEqual(actual.tobytes(), source.tobytes(), row['name'])

    def test_actual_packed_circle_and_frame_repeat_complete_uv_rectangle(self):
        rows = {row['name']: row for row in self.proof['sprites']}
        for name, output_size in [('btn_circle_idle', (72, 72)),
                                  ('battle_btn_attack_basic_frame', (44, 100))]:
            row = rows[name]
            self.assertEqual(row['rect']['mirrorHorizontal'], 1)
            self.assertEqual(row['rect']['mirrorVertical'], 0)
            self.assertEqual(row['rect']['mirrorRotate'], 0)
            source = self.source_crop(row)
            actual = Image.open(ASSETS / 'Resources' / (row['resource'] + '.png')).convert('RGBA')
            self.assertEqual(actual.size, output_size)
            left = actual.crop((0, 0, source.width, source.height))
            right = actual.crop((source.width, 0, actual.width, actual.height))
            # Independent pixel-coordinate assertions protect against sampling
            # only half the source UV or flipping the entire output image.
            for y in range(source.height):
                for x in range(source.width):
                    self.assertEqual(left.getpixel((x, y)), source.getpixel((x, y)))
                    self.assertEqual(right.getpixel((x, y)), source.getpixel((source.width - 1 - x, y)))

    def test_imported_borders_preserve_verified_native_geometry_widths(self):
        expected = {'battle_btn_attack_basic_frame': (20, 22, 20, 22),
                    'unit_health_bg': (7, 0, 7, 0),
                    'unit_health_fill': (7, 0, 7, 0),
                    'btn_circle_idle': (0, 0, 0, 0)}
        for name, borders in expected.items():
            meta = (ASSETS / 'Resources/BattleChrome' / (name + '.png.meta')).read_text(encoding='utf-8')
            match = re.search(r'spriteBorder: \{x: (\d+), y: (\d+), z: (\d+), w: (\d+)\}', meta)
            self.assertIsNotNone(match, name)
            self.assertEqual(tuple(map(int, match.groups())), borders, name)


if __name__ == '__main__':
    unittest.main()
