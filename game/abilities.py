# -*- coding: utf-8 -*-
"""Gamedata ability loader: wires catalog/gamedata (skill/ability/effect.json,
the 1:1 mirror of Protos.GameDataResponse lists -- see
il2cpp/BATTLE_SIM_INVENTORY.md section 5) into executable ability records
for game/engine.py.

Chain (all string ids, per the decoded referencing model):

    unitData[def_id].skills[]                (gameData.json, skill ids)
      -> skill.json[id].abilityReference     (plain string ability id)
        -> ability.json[id]                 cooldown / abilityType / tier[]
          -> tier[-1].effectReference[]     highest unlocked tier
            -> effect.json[id]              type / multipliers / targets

Enums (catalog/gamedata/enums.json, == the binary's enums):
    EffectType      1=DAMAGE 2=RECOVER 5=DISPELL 8=MODIFY_STAT 9=APPLY_EFFECT ...
    EffectDamageType 1=UNMITIGABLE 2=ATTACK_DAMAGE 3=ABILITY_POWER
    EffectTargetBattleSide  2=ALLY_SIDE 3=ENEMY_SIDE
    EffectTargetUnitSelect  1=SELECT_ALL 2=SELECTED_TARGET 4=SELECT_SELF
    UnitHealthState         2=ALIVE
Decimal fields are fixed-point /10000 (multiplier 17450 -> 1.745).
"""
import json
import os
import re

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
GAMEDATA = os.path.join(ROOT, "catalog", "gamedata")
LOC_EN = os.path.join(ROOT, "catalog", "loc_en.json")

DEC = 10000.0

# EffectType subset the sim executes; the rest are logged as inert.
TYPE_DAMAGE = 1
TYPE_RECOVER = 2

# EffectDamageType -> (engine stat, is_attack flag for the decoded pipeline)
DAMAGE_UNMITIGABLE, DAMAGE_ATTACK, DAMAGE_ABILITY = 1, 2, 3

# battleSide / unitSelect / healthState
SIDE_ALLY, SIDE_ENEMY = 2, 3
SEL_ALL, SEL_TARGET, SEL_SELF = 1, 2, 4
HEALTH_ALIVE = 2

_CACHED = {}


def _load(fname):
    key = fname
    if key not in _CACHED:
        rows = json.load(open(os.path.join(GAMEDATA, fname),
                              encoding="utf-8"))["data"]
        _CACHED[key] = {r["id"]: r for r in rows if isinstance(r, dict)}
    return _CACHED[key]


def _dec(v):
    """Fixed-point decimal -> float ('0' strings and ints both occur)."""
    if v is None:
        return 0.0
    try:
        return float(v) / DEC
    except (TypeError, ValueError):
        return 0.0


def _pretty(ability_id):
    tail = re.sub(r"_u\d+$", "", ability_id.split("_", 2)[-1])
    return tail.replace("_", " ").strip().title() or ability_id


def ability_name(ability_id):
    """nameKey -> English name via loc_en.json (lazy; falls back to a
    prettified id when the mirror ships a placeholder)."""
    ability = _load("ability.json").get(ability_id, {})
    nk = ability.get("nameKey") or ""
    global _LOC
    if _LOC is None:
        try:
            _LOC = json.load(open(LOC_EN, encoding="utf-8"))
        except OSError:
            _LOC = {}
    name = _LOC.get(nk)
    if not name or name == "PLACEHOLDER":
        name = _pretty(ability_id)
    return name


_LOC = None


def _effect_record(eid):
    e = _load("effect.json").get(eid)
    if not e:
        return None
    tgt = (e.get("target") or [{}])[0]
    return {
        "id": eid,
        "type": e.get("type", 0),
        "mult": _dec(e.get("multiplierAmountDecimal")),
        "flat": _dec(e.get("additiveAmountDecimal")),
        "variance": _dec(e.get("resultVarianceDecimal")),
        "chance": float(e.get("chance", 100) or 0),
        "damage_type": e.get("damageType", 0),
        "side": tgt.get("battleSide", 0),
        "select": tgt.get("unitSelect", 0),
        "health": tgt.get("healthState", 0),
    }


def unit_abilities(skill_ids):
    """Executable abilities for one unit, in skillReference order.

    Returns a list of:
        {id, skill_id, kind, name, cooldown, effects: [effect records]}
    kind: basic | special (leader/unique passives are out of the v1 sim:
    they are stat auras, no activation, decoded as AbilityType 8).
    """
    skills = _load("skill.json")
    abilities = _load("ability.json")
    out = []
    for sid in skill_ids or []:
        srow = skills.get(sid)
        if not srow:
            continue
        kind = sid.split("_", 1)[0]
        if kind not in ("basicskill", "specialskill"):
            continue                      # leader/unique = passive aura
        aid = srow.get("abilityReference")
        if not isinstance(aid, str):
            continue
        arow = abilities.get(aid)
        if not arow:
            continue
        if arow.get("abilityType") not in (7, 10):   # ACTIVATED[_ALLY_SELECT]
            continue
        tiers = arow.get("tier") or []
        refs = (tiers[-1].get("effectReference") if tiers else None) \
            or arow.get("effectReference") or []
        effects = [r for r in (_effect_record(x.get("id")) for x in refs
                               if isinstance(x, dict)) if r]
        out.append({
            "id": aid,
            "skill_id": sid,
            "kind": "basic" if kind == "basicskill" else "special",
            "name": ability_name(aid),
            "cooldown": float(arow.get("cooldown", 0) or 0),
            "effects": effects,
        })
    return out
