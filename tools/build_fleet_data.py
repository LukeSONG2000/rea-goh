# -*- coding: utf-8 -*-
"""Bake StreamingAssets/fleet.json for the P6 fleet data pass (STEP 5).

DATA-LAYER ONLY -- no fleet simulator here.  The future fleet sim reuses the
existing BattleSim with a CombatType parameter (the binary's fleet battle is
the same turn core; P6 recon found no separate simulator, see PROGRESS.md),
so this file stages what that sim needs:

  * ships        : every combatType==2 unit (catalog/units.json) with its
                   crew slots (crew[].unitId/slot/skillIds), primary stat,
                   CN display name and the count of activatable abilities
                   already baked into StreamingAssets/abilities.json
  * crew tables  : catalog/fleet_crew_tables.json stattable_<UNIT>_<NSTAR>
                   per-star crew contribution rows, filtered to the crew
                   units actually referenced by ships
  * constants    : ship/mitigation GameConstants recovered from the live
                   runtime scan (catalog/game_constants_runtime.json,
                   instance-scan values):

                   ShipMitigationAttackerLevelOffset        = 60
                   UnitMaxMitigation                        = 0.9
                   BattleShipDefaultTimeScale               = 1.5
                   GalacticPowerPerShipAbilitySimulatedCrewScalar  = 8.2
                   GalacticPowerPerShipReinforcementAbilitySimulatedCrewScalar = 2.3
                   GalacticPowerPerShipLevelSimulatedCrewScalar    = 3.5

Run:  PYTHONIOENCODING=utf-8 py tools/build_fleet_data.py
Out:  unity/SWGoH-Restored/Assets/StreamingAssets/fleet.json
"""
import json
import os
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OUT = os.path.join(ROOT, "unity", "SWGoH-Restored", "Assets",
                   "StreamingAssets", "fleet.json")

COMBAT_TYPE_SHIP = 2


def main():
    units = json.load(open(os.path.join(ROOT, "catalog", "units.json"),
                           encoding="utf-8"))
    crew_tables = json.load(open(os.path.join(ROOT, "catalog",
                                              "fleet_crew_tables.json"),
                                 encoding="utf-8"))
    constants_doc = json.load(open(os.path.join(ROOT, "catalog",
                                                "game_constants_runtime.json"),
                                   encoding="utf-8"))
    try:
        abilities = {u["def_id"] for u in json.load(
            open(os.path.join(os.path.dirname(OUT), "abilities.json"),
                 encoding="utf-8"))}
    except OSError:
        abilities = set()

    # locate named runtime constants wherever the scanner nested them
    found = {}

    def scan(obj):
        if isinstance(obj, dict):
            nm = obj.get("name")
            if isinstance(nm, str) and nm in _WANT and obj.get("value") is not None:
                found.setdefault(nm, obj["value"])
            for v in obj.values():
                scan(v)
        elif isinstance(obj, list):
            for x in obj:
                scan(x)

    _WANT = {
        "ShipMitigationAttackerLevelOffset": 60,
        "UnitMaxMitigation": 0.9,
        "BattleShipDefaultTimeScale": 1.5,
        "GalacticPowerPerShipAbilitySimulatedCrewScalar": 8.2,
        "GalacticPowerPerShipReinforcementAbilitySimulatedCrewScalar": 2.3,
        "GalacticPowerPerShipLevelSimulatedCrewScalar": 3.5,
        "UnitSpeedFillMax": 1000.0,
    }
    scan(constants_doc)
    constants = {k: found.get(k, dflt) for k, dflt in _WANT.items()}

    try:
        loc_cn = json.load(open(os.path.join(ROOT, "catalog", "loc_cn.json"),
                                encoding="utf-8"))
    except OSError:
        loc_cn = {}

    ships = []
    needed_crew = set()
    for u in units:
        if u.get("combatType") != COMBAT_TYPE_SHIP:
            continue
        crew = []
        for c in u.get("crew") or []:
            cid = c.get("unitId")
            if cid:
                needed_crew.add(cid)
            crew.append({
                "unitId": cid,
                "slot": c.get("slot", 0),
                "skills": [r.get("skillId")
                           for r in (c.get("skillReference") or [])
                           if r.get("skillId")],
            })
        base_id = u.get("baseId")
        nm = loc_cn.get(u.get("nameKey") or "")
        ships.append({
            "baseId": base_id,
            "name": nm if nm and nm != "PLACEHOLDER" else base_id,
            "primaryUnitStat": u.get("primaryUnitStat", 0),
            "crew": crew,
            "abilityCount": (1 if base_id in abilities else 0) if abilities else None,
        })

    crew_stat_tables = {}
    for cid in sorted(needed_crew):
        rows = {k: v for k, v in crew_tables.items()
                if k.startswith(f"stattable_{cid}_")}
        if rows:
            crew_stat_tables[cid] = rows

    doc = {
        "_meta": {
            "generated_by": "tools/build_fleet_data.py",
            "purpose": "P6 fleet DATA LAYER (no simulator) -- the fleet sim "
                       "reuses BattleSim with a CombatType parameter; ability "
                       "chains live in abilities.json keyed by the same def_id",
            "sources": [
                "catalog/units.json (combatType==2 ships + crew slots)",
                "catalog/fleet_crew_tables.json (stattable_<UNIT>_<NSTAR> crew rows)",
                "catalog/game_constants_runtime.json (live instance-scan values)",
                "catalog/loc_cn.json (ship display names)",
            ],
        },
        "constants": constants,
        "shipCount": len(ships),
        "ships": ships,
        "crewStatTables": crew_stat_tables,
    }

    os.makedirs(os.path.dirname(OUT), exist_ok=True)
    with open(OUT, "w", encoding="utf-8") as f:
        json.dump(doc, f, ensure_ascii=False, separators=(",", ":"))
    print(f"baked {len(ships)} ships, {len(crew_stat_tables)} crew stat table "
          f"sets, constants {constants} -> {OUT}")


if __name__ == "__main__":
    sys.exit(main())
