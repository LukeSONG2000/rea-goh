"""Original visibility/event identity regression, independent of screenshots."""
import importlib.util
import json
import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'tools'))
from export_ability_presentation import Bundle, MASK


class CharacterPresentationTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.source = Bundle('char_vader_pre')
        cls.data = json.loads((ROOT / 'unity/SWGoH-Restored/Assets/Resources/CharacterPresentation/char_vader_pre.json').read_text(encoding='utf-8'))

    def test_all_original_gameobjects_and_renderer_states_retained(self):
        source = self.source
        self.assertEqual(len(self.data['gameObjects']), len(source.gameobjects))
        for row in self.data['gameObjects']:
            self.assertEqual(row['activeSelf'], bool(source.gameobjects[int(row['pathId']) & MASK]['m_IsActive']))
        for row in self.data['renderers']:
            self.assertEqual(row['raw'], source.objects[int(row['pathId']) & MASK].read_typetree())

    def test_original_fist_event_switches_two_distinct_original_meshes(self):
        component = next(c for c in self.data['components'] if c['class'] == 'UnitSubObjectAnimationEventHandler')
        events = {e['Id']: e['Actions'] for e in component['raw']['_animationEvents']}
        self.assertEqual([a['Type'] for a in events['FISTON']], [3, 4])
        self.assertEqual([a['Type'] for a in events['FISTOFF']], [4, 3])
        ids = [str(a['UnitSubObject']['m_PathID']) for a in events['FISTON']]
        objects = {g['pathId']: g for g in self.data['gameObjects']}
        self.assertEqual([objects[p]['activeSelf'] for p in ids], [False, True])
        self.assertEqual([objects[p]['name'] for p in ids], ['vader_leftfist_mesh', 'vader_lefthand_mesh'])
        for audit in self.data['glbAudit']:
            for pid in ids:
                binding = next(b for b in audit['bindings'] if b['sourcePathId'] == pid)
                self.assertEqual(binding['status'], 'EXACT')
                self.assertTrue(binding['nodes'][0]['hasMesh'])

    def test_pointer_identity_companions_are_lossless_for_web(self):
        for component in self.data['components']:
            for reference in component['referenceBindings']:
                self.assertIsInstance(reference['pathId'], str)
                if reference['fileId'] == 0 and reference['pathId'] != '0':
                    self.assertIn(int(reference['pathId']) & MASK, self.source.objects)

    def test_semantic_clip_events_are_bound_original_timestamps(self):
        clips = {c['animationName']: c for c in self.data['clips']}
        self.assertTrue(all(c['controllerPointerVerified'] for c in clips.values()))
        clip = clips['Limit Break 3']
        self.assertEqual(clip['clipName'], 'hmn_sbr_massacre_vader')
        original = self.source.objects[int(clip['clipPathId']) & MASK].read_typetree()
        self.assertEqual(clip['events'], original['m_Events'])
        events = [e for e in clip['events'] if e['functionName'] == 'PlaySubObjectAnimationEvent']
        self.assertEqual([e['data'] for e in events], ['FISTON', 'FISTOFF'])
        self.assertAlmostEqual(events[0]['time'], 0.3666666746)
        self.assertAlmostEqual(events[1]['time'], 1.8666666746)
        self.assertEqual(clips['Idle@1.0']['blendThreshold'], 1.0)


if __name__ == '__main__':
    unittest.main()
