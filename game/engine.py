# -*- coding: utf-8 -*-
"""Headless playable battle: wires the decompiled turn core
(il2cpp/offline/battle.py) to real gamedata units.

v0 rules (explicit stand-ins, each maps to a real binary concept):
  * every unit has ONE action: the basic attack.  ApplyToTargets
    (0x181364CA0) computes
        (context * skill_mult + flat) * ProcessSourceDamage(...)
    with ProcessSourceDamage = stat[effect.base_stat] * coeff + flat.
    v0: base_stat = calculated ATTACK_DAMAGE, coefficient 1.0, context
    factor neutral 1.0 (the stat-module context write is pending the
    ability pipeline) -- crit roll, level scaling, mitigation and the
    whole ported chain all run for real.
  * AI targets the lowest-current-HP living enemy (the game's own
    default tree ends in a Damage->lowest-HP pick before Default).
  * battle ends when a side has no living units (500-turn cap).
"""
import os
import sys

GAME = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(GAME)
for p in (ROOT, os.path.join(ROOT, "il2cpp", "offline")):
    if p not in sys.path:
        sys.path.insert(0, p)

from battle import BattleSimulation, BattleUnit  # noqa: E402
from game import data_units                       # noqa: E402

STAT_MAX_HEALTH = 1
STAT_ATTACK_DAMAGE = 6
STAT_SPEED = 5
LEVEL = 85


class Unit:
    """Build a BattleUnit from a data_units.spec with full g13 stats."""

    def __init__(self, def_id, side, slot):
        s = data_units.spec(def_id)
        stats = s["stats"]
        hp = max(stats.get(STAT_MAX_HEALTH, 1.0), 1.0)
        u = BattleUnit(def_id, side, stats.get(STAT_SPEED, 0.0),
                       hp=hp, level=LEVEL, unit_id=f"{def_id}#{slot}")
        u.stats = dict(stats)
        u.current_hp = hp
        u.max_hp = hp
        self.bu = u
        self.def_id = def_id
        meta = data_units.roster().get(def_id, {})
        self.en = meta.get("en") or def_id
        self.folder = meta.get("char") or ""

    def label(self):
        return f"{self.en}[{self.bu.unit_id}]"


def battle(team_a, team_b, seed):
    """team_a/team_b: lists of defIds.  Returns (events, winner, units)
    where units is client-facing metadata (initial hp/speed for bars)."""
    p1 = [Unit(d, "player", i) for i, d in enumerate(team_a)]
    p2 = [Unit(d, "opponent", i) for i, d in enumerate(team_b)]
    sim = BattleSimulation([u.bu for u in p1], [u.bu for u in p2], seed)
    by_id = {}
    for u in p1 + p2:
        by_id[u.bu.base_id] = u
        by_id[u.bu.unit_id] = u

    def actor_side(side):
        return [u.bu for u in (p1 if side == "player" else p2) if u.bu.alive]

    events = []
    winner = None
    for _ in range(500):
        if not actor_side("player"):
            winner = "opponent"
            break
        if not actor_side("opponent"):
            winner = "player"
            break
        sim.run_turn(choose_action=_ai)
        e = dict(sim.actors[-1])           # {turn, actor, action}
        act = e["action"] or {}
        e.update(act if isinstance(act, dict) else {"action": act})
        e["actor_name"] = by_id[e["actor"]].en
        tgt = act.get("target") if isinstance(act, dict) else None
        e["target_name"] = by_id[tgt].en if tgt in by_id else None
        events.append(e)

    if winner is None:
        winner = "draw"
    units = [{"unit_id": u.bu.unit_id, "def_id": u.def_id, "name": u.en,
              "side": u.bu.side, "hp": round(u.bu.max_hp, 1),
              "speed": u.bu.stats.get(STAT_SPEED, 0.0),
              "folder": u.folder}
             for u in p1 + p2]
    return events, winner, units


def _ai(sim, actor):
    """Basic attack the lowest-current-HP living enemy.

    ApplyToTargets (damage_apply_to_targets.c:167) multiplies TWO factors:
        (context_value * skill_mult + flat) * ProcessSourceDamage(...)
    where ProcessSourceDamage = stat[effectDO+0x21] * mult(+0x24) + flat(+0x28),
    level/relic scaled.  v0 basic attack: stat = ATTACK_DAMAGE, coefficient
    1.0, context factor neutral (1.0) -- the real context write (stat module
    -> EffectResults) is pending the ability pipeline.
    """
    foes = [u for u in sim.units
            if u.side != actor.side and u.alive]
    if not foes:
        return "basic"
    target = min(foes, key=lambda u: u.current_hp)
    atk = sim.get_calculated_stat_value(actor, STAT_ATTACK_DAMAGE)
    effect = {"mult": 1.0, "flat": 0.0, "variance": 0.0,
              "base_stat_value": atk, "source_mult": 1.0,
              "source_flat": 0.0}
    dmg = sim.apply_damage_pipeline(actor, target, 1.0, effect, True)
    target.current_hp = max(0.0, target.current_hp - dmg)
    return {"action": "basic", "target": target.unit_id,
            "damage": round(dmg, 1),
            "hp_after": round(target.current_hp, 1)}
