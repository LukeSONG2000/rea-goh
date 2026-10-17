"""Data provenance regressions: run py -m unittest discover -s tools/tests -p test_effect_export.py."""
import copy
import importlib.util
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
spec = importlib.util.spec_from_file_location('effect_export', ROOT / 'tools/build_abilities_data.py')
export = importlib.util.module_from_spec(spec)
spec.loader.exec_module(export)


class EffectExportTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.raw_abilities = export.load('ability.json')
        cls.raw_effects = export.load('effect.json')
        cls.raw_units = export.read_json(ROOT / 'catalog/units.json')
        cls.skills = export.load('skill.json')
        cls.menus = export.read_json(export.OUT)
        cls.manifest = export.read_json(export.GRAPH_OUT)
        cls.graph = {'schemaVersion': cls.manifest['schemaVersion'],
                     'abilities': cls.manifest['abilities'],
                     'effects': []}
        for name in cls.manifest['effectFiles']:
            cls.graph['effects'].extend(export.read_json(export.OUT.with_name(name))['effects'])
        cls.units = {r['def_id']: r for r in cls.menus}
        cls.effects = {r['id']: r for r in cls.graph['effects']}
        cls.abilities = {r['id']: r for r in cls.raw_abilities}

    def test_entire_original_graph_is_lossless(self):
        self.assertEqual(self.graph['schemaVersion'], 2)
        self.assertEqual(self.graph['effects'], self.raw_effects)
        self.assertEqual(self.graph['abilities'], self.raw_abilities)
        for name in self.manifest['effectFiles']:
            self.assertLess(export.OUT.with_name(name).stat().st_size, 40 * 1024 * 1024)
        zero_chance = [e for e in self.raw_effects if e.get('chance') == 0]
        for effect in zero_chance:
            self.assertEqual(self.effects[effect['id']]['chance'], 0)

    def test_zero_chance_is_not_promoted_to_certain(self):
        # This catalog currently has no zero-chance rows; protect the original
        # exporter bug with an explicit input instead of inventing source data.
        effect = {'id': 'never_applies', 'chance': 0, 'type': 4,
                  'expirationCondition': [{'conditionType': 49, 'conditionValue': '1'}]}
        _, graph = export.build_exports([], [], [], [effect])
        serialized = __import__('json').loads(__import__('json').dumps(graph))
        self.assertEqual(serialized['effects'], [effect])
        self.assertEqual(serialized['effects'][0]['chance'], 0)

    def test_vader_preserves_four_ordered_branches(self):
        ability = next(a for a in self.units['VADER']['abilities']
                       if a['id'] == 'basicability_darthvader')
        ids = ['basicability_vader_damage_jedi_u05', 'basicability_vader_block_jedi_u03',
               'basicability_vader_damage_u05', 'basicability_vader_block_u03']
        self.assertEqual([r['id'] for r in ability['effectReference']], ids)
        original = self.abilities[ability['id']]
        self.assertEqual(ability['selectedTier'], len(original['tier']) - 1)
        self.assertEqual(ability['effectReference'], original['tier'][-1]['effectReference'])
        for effect_id in (ids[0], ids[2]):
            effect = self.effects[effect_id]
            self.assertEqual(effect['multiplierAmountDecimal'], 32410)
            self.assertEqual(effect['resultVarianceDecimal'], 500)
        for index, chance in ((1, 100), (3, 80)):
            effect = self.effects[ids[index]]
            self.assertEqual(effect['chance'], chance)
            self.assertEqual(effect['type'], 4)
            self.assertEqual(effect['expirationCondition'], [
                {'conditionType': 49, 'conditionValue': '1', 'priority': 0},
                {'conditionType': 21, 'conditionValue': '1', 'priority': 0},
                {'conditionType': 24, 'conditionValue': '', 'priority': 0}])
        positive = self.effects[ids[0]]['target'][0]['category']['category']
        negative = self.effects[ids[2]]['target'][0]['category']['category']
        self.assertEqual([c['categoryId'] for c in positive],
                         ['profession_jedi', 'affiliation_rebels'])
        self.assertTrue(all(not c['exclude'] for c in positive))
        self.assertTrue(all(c['exclude'] for c in negative))

    def test_all_menus_are_original_refs_without_aggregation(self):
        self.assertEqual(len(self.units), len(self.menus))
        forbidden = {'damageMultiplier', 'flatDamage', 'healAmount', 'chance',
                     'damageType', 'dmgSide', 'dmgSelect', 'healSide', 'healSelect'}
        for unit in self.menus:
            source = next(u for u in self.raw_units if u['baseId'] == unit['def_id'])
            self.assertEqual(unit['categoryId'], source['categoryId'])
            self.assertEqual(unit['squadPositionPriority'], source['squadPositionPriority'])
            for ability in unit['abilities']:
                self.assertFalse(forbidden.intersection(ability))
                raw = self.abilities[ability['id']]
                tier = ability['selectedTier']
                selected = raw['tier'][tier] if tier >= 0 else raw
                self.assertEqual(ability['effectReference'], selected['effectReference'])
                self.assertEqual(ability['abilityType'], raw['abilityType'])

    def test_two_other_heroes_follow_identical_pipeline(self):
        for hero in ('JYNERSO', 'GRANDMOFFTARKIN'):
            source = [u for u in self.raw_units if u['baseId'] == hero]
            menus, _ = export.build_exports(source, self.skills, self.raw_abilities,
                                            self.raw_effects)
            self.assertEqual(menus[0]['abilities'][0]['effectReference'],
                             self.units[hero]['abilities'][0]['effectReference'])
            self.assertEqual(menus[0]['categoryId'], self.units[hero]['categoryId'])
            self.assertEqual(len(menus), 1)

    def test_variant_disagreement_fails_loudly(self):
        source = copy.deepcopy(next(u for u in self.raw_units if u['baseId'] == 'VADER'))
        changed = copy.deepcopy(source)
        changed['categoryId'].append('test_disagreement')
        with self.assertRaisesRegex(ValueError, 'variants disagree'):
            export.build_exports([source, changed], self.skills, self.raw_abilities,
                                 self.raw_effects)

    def test_empty_tier_is_not_replaced_by_base_effects(self):
        unit = {'baseId': 'SYNTHETIC', 'categoryId': [],
                'skillReference': [{'skillId': 'basicskill_SYNTHETIC'}]}
        skill = {'id': 'basicskill_SYNTHETIC', 'abilityReference': 'basic_test'}
        ability = {'id': 'basic_test', 'abilityType': 7,
                   'effectReference': [{'id': 'base_effect'}],
                   'tier': [{'effectReference': []}]}
        menus, _ = export.build_exports([unit], [skill], [ability], [])
        self.assertEqual(menus[0]['abilities'][0]['effectReference'], [])
        self.assertEqual(menus[0]['abilities'][0]['selectedTier'], 0)

    def test_variant_squad_priority_disagreement_fails_loudly(self):
        source = copy.deepcopy(next(u for u in self.raw_units if u['baseId'] == 'VADER'))
        changed = copy.deepcopy(source)
        changed['squadPositionPriority'] += 1
        with self.assertRaisesRegex(ValueError, 'variants disagree'):
            export.build_exports([source, changed], self.skills, self.raw_abilities,
                                 self.raw_effects)


if __name__ == '__main__':
    unittest.main()
