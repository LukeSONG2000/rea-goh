# -*- coding: utf-8 -*-
"""Bake StreamingAssets/abilities.json for the Unity battle sim (STEP 4).

Walks the SAME decoded chain as game/abilities.py v1 (the web engine's real
ability pipeline):

    units.json[baseId].skillReference[].skillId
      -> skill.json[id].abilityReference
        -> ability.json[id]  (abilityType 7/10 ACTIVATED, cooldown,
                              tier[-1].effectReference = highest tier)
          -> effect.json[id] (type 1 DAMAGE / type 2 RECOVER, /10000 decimals)

into one compact record per unit:

    {"def_id": "JYNERSO", "abilities": [
        {"id": "basicability_jynerso", "skill_id": "basicskill_JYNERSO",
         "name": "加紧进攻", "kind": "basic", "cooldown": 0,
         "damageMultiplier": 1.467, "flatDamage": 0.0, "damageType": 2,
         "healAmount": 0.0, "chance": 100.0, "side": 3, "select": 2}]}

Aggregation mirrors engine.py _act(): DAMAGE effects contribute
mult+flat through the damage pipeline (summed), RECOVER effects heal
(mult+flat) * ABILITY_POWER (summed); non-executable effect types
(buff/dispel/summon) are simply not baked (they were reported "skipped"
on the web side, never invented).

Names resolve through loc_cn.json (the Unity UI language), falling back to
loc_en.json and finally a prettified id.

Run:  PYTHONIOENCODING=utf-8 py tools/build_abilities_data.py
Out:  unity/SWGoH-Restored/Assets/StreamingAssets/abilities.json
"""
import json
import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
GAMEDATA = os.path.join(ROOT, "catalog", "gamedata")
OUT = os.path.join(ROOT, "unity", "SWGoH-Restored", "Assets",
                   "StreamingAssets", "abilities.json")

DEC = 10000.0
TYPE_DAMAGE, TYPE_RECOVER = 1, 2
ACTIVATED_TYPES = (7, 10)          # ACTIVATED / ACTIVATED_ALLY_SELECT


def load(fname):
    with open(os.path.join(GAMEDATA, fname), encoding="utf-8") as f:
        return json.load(f)["data"]


def dec(v):
    if v is None:
        return 0.0
    try:
        return float(v) / DEC
    except (TypeError, ValueError):
        return 0.0


def pretty(ability_id):
    tail = re.sub(r"_u\d+$", "", ability_id.split("_", 2)[-1])
    return tail.replace("_", " ").strip().title() or ability_id


def main():
    units = json.load(open(os.path.join(ROOT, "catalog", "units.json"),
                           encoding="utf-8"))
    skills = {r["id"]: r for r in load("skill.json") if isinstance(r, dict)}
    abilities = {r["id"]: r for r in load("ability.json") if isinstance(r, dict)}
    effects = {r["id"]: r for r in load("effect.json") if isinstance(r, dict)}
    try:
        loc_cn = json.load(open(os.path.join(ROOT, "catalog", "loc_cn.json"),
                                encoding="utf-8"))
    except OSError:
        loc_cn = {}
    try:
        loc_en = json.load(open(os.path.join(ROOT, "catalog", "loc_en.json"),
                                encoding="utf-8"))
    except OSError:
        loc_en = {}

    def name_of(ability_id, arow):
        nk = arow.get("nameKey") or ""
        for loc in (loc_cn, loc_en):
            n = loc.get(nk)
            if n and n != "PLACEHOLDER":
                return n
        return pretty(ability_id)

    out = []
    stats = {"units": 0, "abilities": 0, "skipped_units": 0}
    for u in units:
        base_id = u.get("baseId")
        if not base_id:
            continue
        recs = []
        for ref in u.get("skillReference") or []:
            sid = ref.get("skillId")
            kind = (sid or "").split("_", 1)[0]
            if kind not in ("basicskill", "specialskill"):
                continue                      # leader/unique = passive auras
            srow = skills.get(sid)
            if not srow:
                continue
            aid = srow.get("abilityReference")
            arow = abilities.get(aid) if isinstance(aid, str) else None
            if not arow:
                continue
            if arow.get("abilityType") not in ACTIVATED_TYPES:
                continue
            tiers = arow.get("tier") or []
            refs = (tiers[-1].get("effectReference") if tiers else None) \
                or arow.get("effectReference") or []
            dmg_mult = dmg_flat = heal = 0.0
            dmg_type, chance = 2, 100.0
            dmg_side = dmg_select = 3
            heal_side = heal_select = 2
            dmg_seen = heal_seen = False
            for r in refs:
                er = effects.get(r.get("id")) if isinstance(r, dict) else None
                if not er:
                    continue
                etype = er.get("type", 0)
                if etype not in (TYPE_DAMAGE, TYPE_RECOVER):
                    continue                  # buff/dispel/summon: inert in v1
                tgt = (er.get("target") or [{}])[0]
                ch = float(er.get("chance", 100) or 100)
                chance = min(chance, ch)
                if etype == TYPE_DAMAGE:
                    dmg_mult += dec(er.get("multiplierAmountDecimal"))
                    dmg_flat += dec(er.get("additiveAmountDecimal"))
                    if not dmg_seen:
                        dmg_type = er.get("damageType", 2)
                        dmg_side = tgt.get("battleSide", 3)
                        dmg_select = tgt.get("unitSelect", 2)
                        dmg_seen = True
                else:
                    heal += dec(er.get("multiplierAmountDecimal")) \
                          + dec(er.get("additiveAmountDecimal"))
                    if not heal_seen:
                        heal_side = tgt.get("battleSide", 2)
                        heal_select = tgt.get("unitSelect", 4)
                        heal_seen = True
            rec = {
                "id": aid,
                "skill_id": sid,
                "name": name_of(aid, arow),
                "kind": "basic" if kind == "basicskill" else "special",
                "cooldown": float(arow.get("cooldown", 0) or 0),
                "damageMultiplier": round(dmg_mult, 5),
                "flatDamage": round(dmg_flat, 5),
                "damageType": dmg_type if dmg_type else 2,
                "healAmount": round(heal, 5),
                "chance": chance,
                "dmgSide": dmg_side,
                "dmgSelect": dmg_select,
                "healSide": heal_side,
                "healSelect": heal_select,
            }
            recs.append(rec)
        if not recs:
            stats["skipped_units"] += 1
            continue
        out.append({"def_id": base_id, "abilities": recs})
        stats["units"] += 1
        stats["abilities"] += len(recs)

    os.makedirs(os.path.dirname(OUT), exist_ok=True)
    with open(OUT, "w", encoding="utf-8") as f:
        json.dump(out, f, ensure_ascii=False, separators=(",", ":"))
    print(f"baked {stats['units']} units / {stats['abilities']} abilities "
          f"({stats['skipped_units']} units without an activatable chain) -> {OUT}")


if __name__ == "__main__":
    sys.exit(main())
