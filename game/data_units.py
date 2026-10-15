# -*- coding: utf-8 -*-
"""Unit data layer for the playable game.

gamedata's unitData stats are keyed by the PROTOBUF enum `UnitStat`
(dump/cs/il2cpp.cs:602397) while the battle engine runs on the runtime
`UnitStatEnum` (:602463).  The two share names 1..16 but diverge above:
proto 28 = MAX_SHIELD vs runtime 28 = ATTACK_CRITICAL_PERCENT_ADDITIVE.
This module derives the proto->runtime mapping by parsing both enums out
of dump.cs (no hand-maintained table to rot).

Raw stat values are scaled by 1e8 for every stat class (verified against
known level-85 values: ADMIRALACKBAR g13 -> HP 19930, speed 139).

G13 (gear tier 13) is the v0 gear level: every roster unit has it.
"""
import json
import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OFFLINE = os.path.join(ROOT, "il2cpp", "offline")
for p in (ROOT, OFFLINE):
    if p not in sys.path:
        sys.path.insert(0, p)

DUMP_CS = os.path.join(ROOT, "il2cpp", "dump", "cs", "il2cpp.cs")
GAMEDATA = os.path.join(ROOT, "catalog", "gamedata", "gameData.json")
ROSTER = os.path.join(ROOT, "game_roster.json")

GEAR = "13"
SCALE = 1e8


def _parse_enum(src, decl):
    """'public enum X // ... \\n{ NAME = 1, ... }' -> {name: value} for the
    first enum whose declaration line matches `decl` (regex; the dump puts
    a TypeDefIndex comment between decl and brace)."""
    m = re.search(decl + r"[^\n]*\n\s*\{([^}]*)\}", src)
    out = {}
    for name, val in re.findall(r"(\w+)\s*=\s*(\d+)", m.group(1)):
        out[name] = int(val)
    return out


def proto_to_runtime():
    """{proto_value: runtime_value} for UnitStat -> UnitStatEnum, matched by
    normalized enum member name (UNITSTATATTACKDAMAGE == ATTACK_DAMAGE).
    Falls back to game/stat_map.json (precomputed from the same parse)
    when dump.cs is unavailable -- e.g. on the deploy server."""
    if not os.path.exists(DUMP_CS):
        return {int(k): v for k, v in json.load(
            open(os.path.join(os.path.dirname(os.path.abspath(__file__)),
                              "stat_map.json"))).items()}
    src = open(DUMP_CS, encoding="utf-8", errors="replace").read()
    proto = _parse_enum(src, r"public enum UnitStat\s")
    runtime = _parse_enum(src, r"public enum UnitStatEnum\s")
    rt_by_norm = {k.replace("_", ""): v for k, v in runtime.items()}
    out = {}
    for k, v in proto.items():
        norm = k[8:] if k.startswith("UNITSTAT") else k   # strip prefix
        if norm in rt_by_norm:
            out[k] = rt_by_norm[norm]
    missing = set(proto) - set(out)
    if missing:
        raise SystemExit(f"unmapped proto stats: {sorted(missing)}")
    # gameData keys stats by proto enum VALUE -> map value -> runtime value
    return {v: out[k] for k, v in proto.items()}


_PROTO_MAP = None


def _map():
    global _PROTO_MAP
    if _PROTO_MAP is None:
        _PROTO_MAP = proto_to_runtime()
    return _PROTO_MAP


def roster():
    """defId -> {'char': bundle folder, 'en': english name} (model-backed)."""
    return json.load(open(ROSTER, encoding="utf-8"))


def g13_stats(def_id):
    """G13 gear stats keyed by RUNTIME UnitStatEnum, scaled /1e8."""
    data = json.load(open(GAMEDATA, encoding="utf-8"))["data"]["unitData"]
    entry = data[def_id]["gearLvl"][GEAR]["stats"]
    pm = _map()
    return {pm[int(k)]: v / SCALE for k, v in entry.items()}


def skills(def_id):
    data = json.load(open(GAMEDATA, encoding="utf-8"))["data"]["unitData"]
    return [s["id"] for s in data[def_id].get("skills", [])]


def spec(def_id):
    """Everything the engine needs to instantiate one combatant."""
    return {"def_id": def_id, "stats": g13_stats(def_id), "skills": skills(def_id)}
