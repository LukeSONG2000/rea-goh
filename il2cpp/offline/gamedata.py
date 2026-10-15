# -*- coding: utf-8 -*-
"""Offline gamedata store — the python equivalent of the client's
`GameDataDomainController` (dump line 237545), fed entirely from local files
so battle simulation never touches a server:

  catalog/gamedata/ability.json        -> Protos.Ability        (6,732)
  catalog/gamedata/effect.json         -> Protos.Effect         (63,035)
  catalog/gamedata/skill.json          -> SkillDefinition
  catalog/gamedata/targetingSet.json   -> Protos.TargetingSet
  catalog/gamedata/battleTargetingRule.json
  catalog/gamedata/table.json          -> Protos.Table (188)
  catalog/gamedata/statProgression.json (7,423)
  catalog/gamedata/enums.json          -> enum id-name maps
  catalog/units*.json                  -> UnitDef rows (unitPrefab/baseId)

Numbers arrive as string-encoded decimals ("20000" = 0.02 at 1e6 scale);
`dec()` converts to float.  `swgoh-utils/gamedata` mirror, fetched 2026-10-15.
"""
import json
import os

_CATALOG = os.path.join(os.path.dirname(os.path.dirname(
    os.path.dirname(os.path.abspath(__file__)))), "catalog")


def dec(s):
    """String-encoded fixed-point decimal (1e6 scale) -> float."""
    if s is None or s == "":
        return 0.0
    return float(s) / 1e6 if isinstance(s, str) else float(s)


def _load(path):
    d = json.load(open(path, encoding="utf-8"))
    return d.get("data", d) if isinstance(d, dict) else d


class GameData:
    def __init__(self, catalog=_CATALOG):
        gd = os.path.join(catalog, "gamedata")

        def item(name):
            p = os.path.join(gd, name)
            return _load(p) if os.path.exists(p) else []

        self.abilities = {a["id"]: a for a in item("ability.json")}
        self.effects = {e["id"]: e for e in item("effect.json")}
        self.skills = {s["id"]: s for s in item("skill.json")}
        self.targeting_sets = {t["id"]: t for t in item("targetingSet.json")}
        self.targeting_rules = {t["id"]: t
                                for t in item("battleTargetingRule.json")}
        self.tables = {t["id"]: t for t in item("table.json")}
        self.stat_progressions = {s["id"]: s for s in item("statProgression.json")}
        self.enums = item("enums.json")

        self.units = []
        self.units_by_prefab = {}
        self.units_by_base = {}
        for f in ("units.json", "units_pve.json", "units_gas.json"):
            for u in _load(os.path.join(catalog, f)):
                if not isinstance(u, dict) or not u.get("baseId"):
                    continue
                self.units.append(u)
                if u.get("unitPrefab"):
                    self.units_by_prefab.setdefault(u["unitPrefab"], u)
                self.units_by_base.setdefault(u["baseId"], u)

    # -- GameDataDomainController-style accessors --------------------------
    def get_unit_definition(self, base_id):
        return self.units_by_base.get(base_id)

    def get_unit_by_prefab(self, prefab):
        return self.units_by_prefab.get(prefab)

    def get_ability_definition(self, ability_id):
        return self.abilities.get(ability_id)

    def get_battle_effect_definition(self, effect_id):
        return self.effects.get(effect_id)

    def get_stat_progression(self, prog_id):
        return self.stat_progressions.get(prog_id)

    def get_table(self, table_id):
        return self.tables.get(table_id)

    def get_targeting_set(self, ts_id):
        return self.targeting_sets.get(ts_id)

    def enum_name(self, enum_type, value):
        """enum int -> name from enums.json (e.g. ('ConditionTypeEnum', 3))."""
        m = self.enums.get(enum_type)
        if not m:
            return str(value)
        for name, val in m.items():
            if val == value:
                return name.split("_", 1)[-1] if name.count("_") else name
        return str(value)

    def unit_skills(self, unit):
        """[{skillId, requiredTier...}] from a UnitDef row."""
        return unit.get("skillReference", [])

    def unit_stat_progression_ids(self, unit):
        """statProgression row ids across the unit's tiers (unit_tier[].statProgressionId)."""
        return [t.get("statProgressionId") for t in unit.get("unitTier", [])
                if t.get("statProgressionId")]
