# -*- coding: utf-8 -*-
"""Headless playable battle: wires the decompiled turn core
(il2cpp/offline/battle.py) to real gamedata units AND the real skill chain
(game/abilities.py: unitData skills -> skill.abilityReference ->
ability.tier[-1].effectReference -> effect.json rows).

v1 rules (explicit stand-ins, each maps to a real binary concept):
  * ability selection mirrors the decoded default AI decision tree
    (AbilityDecisionTreeProvider order, BATTLE_SIM_INVENTORY.md):
    team-heal when someone is hurt -> damage special -> basic attack.
  * damage effects run the FULL decoded pipeline
    (DamageModule.ApplyToTargets 0x181364CA0): caster_power * mult + flat,
    ProcessSourceDamage level scaling, crit roll, mitigation, variance --
    with caster_power neutral 1.0 (the context-chaining EffectResults
    accumulator is pending the persistent-effect pipeline).
  * damage type routes the decoded branches: ATTACK_DAMAGE(2) -> physical
    (armor mitigation, attack crit), ABILITY_POWER(3) -> suppression path,
    UNMITIGABLE(1) -> suppression stats, no mitigation roll.
  * RECOVER effects heal (mult+flat) * caster ABILITY_POWER; context-driven
    heals (mult=flat=0) are logged as skipped, not invented.
  * percent chance rolls use the seeded BattleRandom (nextInt(100) < chance),
    matching ConditionStatus.RerollChance; cooldowns tick on the owner's
    turn start (per-unit ProcessCooldowns stand-in).
  * non-executed effect types (dispel/buff/summon...) are reported in the
    event under "skipped" -- visible via /api/battle, never silent.
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
from game import abilities, data_units           # noqa: E402

STAT_MAX_HEALTH = 1
STAT_ATTACK_DAMAGE = 6
STAT_ABILITY_POWER = 7
STAT_SPEED = 5
LEVEL = 85
HEAL_URGE = 0.55          # ally HP fraction below which healers prioritize


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
        self.abilities = abilities.unit_abilities(s.get("skills"))
        self.cooldowns = {a["id"]: 0 for a in self.abilities}
        meta = data_units.roster().get(def_id, {})
        self.en = meta.get("en") or def_id
        self.folder = meta.get("char") or ""

    def ability(self, ability_id):
        for a in self.abilities:
            if a["id"] == ability_id:
                return a
        return None

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
        sim.run_turn(choose_action=lambda s, a: _act(s, a, by_id))
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
              "folder": u.folder,
              "abilities": [{"id": a["id"], "name": a["name"],
                             "kind": a["kind"], "cooldown": a["cooldown"]}
                            for a in u.abilities]}
             for u in p1 + p2]
    return events, winner, units


# ---------------------------------------------------------------------------
# ability execution (BattleUnitAbilityController activation pipeline)

def _pick_targets(sim, actor, eff, by_id, primary):
    """Target expansion for one effect record (the first TargetSelector)."""
    living = lambda side: [u for u in sim.units
                           if u.side == side and u.alive]
    side = actor.side
    foes_side = "opponent" if side == "player" else "player"
    if eff["side"] == abilities.SIDE_ALLY:
        pool = living(side)
    elif eff["side"] == abilities.SIDE_ENEMY:
        pool = living(foes_side)
    else:
        pool = living(side) if eff["select"] == abilities.SEL_SELF else []
    if eff["select"] == abilities.SEL_SELF:
        return [actor] if actor.alive else []
    if eff["select"] == abilities.SEL_ALL:
        return pool
    if eff["select"] == abilities.SEL_TARGET:
        if eff["side"] == abilities.SIDE_ENEMY:
            primary_u = by_id[primary].bu if primary in by_id else None
            if primary_u is not None and primary_u.alive:
                return [primary_u]
        return pool[:1]
    return pool[:1]


def _run_effect(sim, actor, eff, targets, is_attack_flag=None):
    """One effect row against expanded targets.  Returns (value, applied)."""
    if eff["chance"] < 100.0:
        roll = sim.ctx.battle_random.next_int(100)
        if roll >= eff["chance"]:
            return 0.0, False
    etype = eff["type"]
    if etype == abilities.TYPE_DAMAGE:
        dmg_type = eff["damage_type"]
        # ATTACK_DAMAGE(2) -> physical/armor branch; ABILITY_POWER(3) and
        # UNMITIGABLE(1) -> suppression branch (the pipeline's mitigation
        # bypass for unmitigable damage lands with ApplyAttackDamage).
        is_attack = (dmg_type == abilities.DAMAGE_ATTACK)
        base_stat = STAT_ATTACK_DAMAGE if is_attack else STAT_ABILITY_POWER
        effect = {"mult": eff["mult"], "flat": eff["flat"],
                  "variance": eff["variance"],
                  "base_stat_value": sim.get_calculated_stat_value(
                      actor, base_stat),
                  "source_mult": 1.0, "source_flat": 0.0}
        total = 0.0
        for t in targets:
            dmg = sim.apply_damage_pipeline(actor, t, 1.0, effect,
                                            is_attack)
            t.current_hp = max(0.0, t.current_hp - dmg)
            total += dmg
        return total, True
    if etype == abilities.TYPE_RECOVER:
        if eff["mult"] <= 0.0 and eff["flat"] <= 0.0:
            return 0.0, None            # context-driven: not executable yet
        power = sim.get_calculated_stat_value(actor, STAT_ABILITY_POWER)
        heal = (eff["mult"] + eff["flat"]) * power
        total = 0.0
        for t in targets:
            t.current_hp = min(t.max_hp, t.current_hp + heal)
            total += heal
        return total, True
    return 0.0, None                    # buff/dispel/summon: reported inert


def _act(sim, actor, by_id):
    """Decoded-default-tree-lite ability selection + full effect pipeline."""
    u = by_id[actor.unit_id]
    for aid in u.cooldowns:                      # owner-turn cooldown tick
        u.cooldowns[aid] = max(0, u.cooldowns[aid] - 1)

    allies = [x for x in sim.units if x.side == actor.side and x.alive]
    foes = [x for x in sim.units if x.side != actor.side and x.alive]
    if not foes:
        return {"action": "basic", "target": None, "damage": 0.0}

    primary = min(foes, key=lambda x: x.current_hp)      # Damage node target
    primary_id = primary.unit_id
    hurt = [x for x in allies
            if x.current_hp < HEAL_URGE * x.max_hp]

    def ready(kind, has_type=None):
        for a in u.abilities:
            if a["kind"] != kind or u.cooldowns[a["id"]] > 0:
                continue
            if has_type is None or any(e["type"] == has_type
                                       for e in a["effects"]):
                return a
        return None

    # 1. TeamHealAlly / SelfHeal branch
    ability = None
    if hurt:
        ability = ready("special", abilities.TYPE_RECOVER)
    # 2. KillLowHPEnemy / Damage branch
    if ability is None:
        ability = ready("special", abilities.TYPE_DAMAGE)
    # 3. Default branch: basic attack (with its own rider effects)
    if ability is None:
        ability = ready("basic")
    if ability is None:
        return _legacy_basic(sim, actor, foes, primary, by_id)

    target0 = primary_id
    if ability["kind"] == "special" and not any(
            e["type"] == abilities.TYPE_DAMAGE and
            e["side"] == abilities.SIDE_ENEMY
            for e in ability["effects"]):
        target0 = min(hurt or allies,
                      key=lambda x: x.current_hp / x.max_hp).unit_id

    damage = heal = 0.0
    targets_hit, skipped = [], []
    for eff in ability["effects"]:
        tgts = _pick_targets(sim, actor, eff, by_id, target0)
        value, applied = _run_effect(sim, actor, eff, tgts)
        if applied is None:
            skipped.append({"id": eff["id"], "type": eff["type"]})
        elif eff["type"] == abilities.TYPE_DAMAGE:
            damage += value
            targets_hit += [t.unit_id for t in tgts]
        elif eff["type"] == abilities.TYPE_RECOVER:
            heal += value
            targets_hit += [t.unit_id for t in tgts]
    if u.cooldowns[ability["id"]] is not None:
        u.cooldowns[ability["id"]] = int(ability["cooldown"])
    return {"action": ability["kind"], "ability": ability["name"],
            "ability_id": ability["id"], "target": target0,
            "targets": sorted(set(targets_hit)), "damage": round(damage, 1),
            "heal": round(heal, 1), "skipped": skipped}


def _legacy_basic(sim, actor, foes, primary, by_id):
    """v0 fixed formula kept for units with no gamedata ability chain."""
    atk = sim.get_calculated_stat_value(actor, STAT_ATTACK_DAMAGE)
    effect = {"mult": 1.0, "flat": 0.0, "variance": 0.0,
              "base_stat_value": atk, "source_mult": 1.0,
              "source_flat": 0.0}
    dmg = sim.apply_damage_pipeline(actor, primary, 1.0, effect, True)
    primary.current_hp = max(0.0, primary.current_hp - dmg)
    return {"action": "basic", "ability": "Attack", "ability_id": None,
            "target": primary.unit_id, "targets": [primary.unit_id],
            "damage": round(dmg, 1), "heal": 0.0, "skipped": []}
