# -*- coding: utf-8 -*-
"""Offline battle sim -- faithful port of the client's turn core, decompiled
from GameAssembly.dll (Ghidra 12 + PyGhidra; dump/decompiled/*.c).

Every algorithmic choice below cites the decompiled function it came from.
The real game is client-authoritative deterministic: the server hands one
int seed at battle start and the client simulates everything locally with
five JavaRandom streams, all seeded with that same value (see SEEDING).

Status: turn order, crit, damage scaling = exact ports.  Stat aggregation
(GetCalculatedStatValue's modifier lists) and ability execution are still
skeletons -- they need the persistent-effect pipeline, not the turn core.
"""
from java_random import JavaRandom

# ---------------------------------------------------------------- constants
# GameConstants instance fields, values extracted from the ctor's machine
# code (mov dword ptr [reg+disp], imm32 -- see tools note in BATTLE_SIM_INVENTORY).
# The ctor only hardcodes these; the Min/PerLevel/Base* fields are data-driven
# (hydrated from gamedata at runtime, absent from the binary) -> sourced from
# the gamedata mirror, defaults here are placeholders.
UNIT_SPEED_FILL_MAX = 100.0     # GameConstants+0x24B4 (ctor imm 0x42C80000)
CRIT_RATING_MOD = 100.0         # +0x2308 attack / +0x22F0 ability (ctor imm)
CRIT_CHANCE_LEVEL_DELTA_MAX = 1.0   # +0x22F8 attack / +0x22E0 ability (ctor imm)
DAMAGE_MOD_LEVEL_DELTA_MAX = 1.0    # +0x2340 / era +0x2334 (ctor imm)
ARMOR_RATING_MOD = 100.0        # +0x22F4 UnitArmorRatingMod (ctor imm)
SUPPRESSION_RATING_MOD = 100.0  # +0x24E0 UnitSuppressionRatingMod (ctor imm)
MITIGATION_LEVEL_OFFSET = 0     # +0x1C34 ShipMitigationAttackerLevelOffset
                                # (data-driven; the same offset serves units --
                                # disasm selects armor/suppression by damageType)

# Data-driven GameConstants fields (not in the binary; from gamedata mirror):
CRIT_CHANCE_LEVEL_DELTA_PER_LEVEL = 0.0    # +0x2300 / ability +0x22E8
CRIT_CHANCE_LEVEL_DELTA_MIN = 0.0          # +0x22FC / ability +0x22E4
BASE_ATTACK_CRIT_CHANCE = 0.0              # +0x2314
BASE_ABILITY_CRIT_CHANCE = 0.0             # +0x230C
BASE_CRIT_DAMAGE = 0.0                     # +0x231C
DAMAGE_MOD_PER_LEVEL_DELTA = 0.0           # +0x2348
DAMAGE_MOD_LEVEL_DELTA_MIN = 0.0           # +0x2344
DAMAGE_MOD_PER_ERA_LEVEL_DELTA = 0.0       # +0x233C
DAMAGE_MOD_ERA_LEVEL_DELTA_MIN = 0.0       # +0x2338
DAMAGE_MOD_ERA_LEVEL_DELTA_MAX = 1.0       # +0x2334 (ctor imm, era twin of +0x2340)

# UnitStatEnum values used by the damage pipeline (dump/cs/il2cpp.cs:602463)
STAT_SPEED = 5
STAT_ARMOR = 8
STAT_SUPPRESSION = 9
STAT_ARMOR_PENETRATION = 10
STAT_SUPPRESSION_PENETRATION = 11
STAT_ATTACK_CRIT_RATING = 14
STAT_ABILITY_CRIT_RATING = 15
STAT_CRIT_DAMAGE = 16
STAT_CRIT_DAMAGE_NEGATE = 17
STAT_ATTACK_CRIT_NEGATE_RATING = 22
STAT_ABILITY_CRIT_NEGATE_RATING = 23
STAT_CRIT_PERCENT_ADDITIVE = 28
STAT_SHIELD_PENETRATION = 34   # fraction (clamped 0..1) of damage routed to shields
STAT_CRIT_NEGATE_PERCENT_ADDITIVE = 40
STAT_MASTERY = 61              # recursion root: mastery converts into sub-stats

# SEEDING: BattleFsmController._SendBattleStartRequest_b__0 (0x181A5EFB0,
# decompiled/battle_start_seed.c) constructs FIVE JavaRandom instances from
# the same int at BattleStartResponse+0x18 and stores them into
# BattleContext+0x158..0x178.  There is no separate targeting seed.
#
# JavaRandom ctor (0x180CAC420, inlined everywhere):
#     seed = (s ^ 0x5DEECE66D) & (2**48 - 1)
# nextInt(bound): Java semantics -- seed*0x5DEECE66D+0xB mod 2^48, next31 =
# seed>>17, nextInt = next31 % bound with the int32-overflow rejection loop.


def _clamp(v, lo, hi):
    if v < lo:
        return lo
    if v > hi:
        return hi
    return v


class BattleUnit:
    """Runtime slice of BattleUnitController (dump/cs/il2cpp.cs:72012).

    Field offsets referenced below are the controller's: +0x98 unitDO,
    +0xC0 currentHP, +0x1E8 hasFled, +0x208 speedFill,
    +0x20C speedFillWithOverflow, +0x100/+0x110 limitBreak/unique contexts.
    """

    def __init__(self, base_id, side, speed, hp=1.0, level=1, era=0, unit_id=None):
        self.base_id = base_id
        self.unit_id = unit_id or base_id
        self.side = side               # 'player' | 'opponent'
        self.level = level
        self.era = era
        self.stats = {STAT_SPEED: float(speed)}
        self.current_hp = float(hp)
        self.has_fled = False
        self.health_state = 0          # +0xD8: 1 = dead (logging branch only)
        self.speed_fill = 0.0          # +0x208
        self.speed_fill_overflow = 0.0  # +0x20C
        # (unit_id, base_cooldown, max_cooldown, cd_remaining) triples stand in
        # for the limit-break/unique ability contexts (+0x100/+0x110)
        self.cooldowns = []

    @property
    def alive(self):
        return (not self.has_fled) and self.current_hp > 0

    def get_stat(self, stat):
        """GetStatValue stand-in: base stats only (modifier lists pending)."""
        return self.stats.get(stat, 0.0)

    # modifier sources: BattleUnit has none of the real IStatModifierSource
    # machinery yet; these hooks exist so get_calculated_stat_value below
    # mirrors the binary's exact pass structure when the effect pipeline lands.
    def additives_for_stat(self, stat):
        """StatCalculator.GetAdditivesForStat (0x180FE0480) stand-in."""
        return []

    def percent_additives_for_stat(self, stat):
        """StatCalculator.GetPercentAdditivesForStat (0x180FE0280) stand-in."""
        return []

    def mastery_contribution(self, stat):
        """StatCalculator.GetMasteryContribution (0x180FDFF40) stand-in:
        GameDataDomainController.GetUnitMasteryTable (0x181BE74A0) lookup
        keyed by stat-source identity -> conversion table call."""
        return 0.0

    @property
    def speed(self):
        s = self.get_stat(STAT_SPEED)
        return 0.0 if s < 0.0 else s   # speed clamped at 0 in every use site

    def __repr__(self):
        return f"<{self.side}:{self.base_id} fill={self.speed_fill:.1f}>"


class BattleContext:
    """BattleContext (dump/cs/il2cpp.cs:58888): the five RNG streams at
    +0x158..+0x178.  All five are constructed from the same battle seed
    (decompiled/battle_start_seed.c) -- there is no separate targeting seed."""

    def __init__(self, battle_seed):
        self.battle_random = JavaRandom(battle_seed)              # +0x158
        self.player_ability_random = JavaRandom(battle_seed)      # +0x160
        self.opponent_ability_random = JavaRandom(battle_seed)    # +0x168
        self.player_target_random = JavaRandom(battle_seed)       # +0x170
        self.opponent_target_random = JavaRandom(battle_seed)     # +0x178


class BattleSimulation:
    """BattleUnitsController turn loop (headless port of ProcessUnitsSpeed)."""

    def __init__(self, player_units, opponent_units, battle_seed):
        self.ctx = BattleContext(battle_seed)
        self.units = list(player_units) + list(opponent_units)
        self.turn_priority = []        # BattleUnitsController+0xB8
        self.round = 0
        self.actors = []               # BattleRecord[] equivalent: event log

    # -- DetermineTicksToNextAttack (0x181D006F0) --------------------------
    def _determine_ticks(self):
        """ticks_i = max(0, UnitSpeedFillMax - speedFill) / max(speed, 0)
        for every eligible unit (speed > 0, not fled, hp > 0);
        returns (ticks_min, [(unit, ticks), ...])."""
        entries = []
        ticks_min = float("inf")
        for u in self.units:
            if u.speed <= 0.0 or u.has_fled or u.current_hp <= 0:
                continue
            ticks = max(UNIT_SPEED_FILL_MAX - u.speed_fill, 0.0) / u.speed
            entries.append((u, ticks))
            if ticks < ticks_min:
                ticks_min = ticks
        return ticks_min, entries

    # -- PopulateNextUnitCandidateList (0x181D00EE0) -----------------------
    def _populate_candidates(self, entries, ticks_min):
        """all_list = units whose |ticks - ticks_min| < window, where
        window = max(max(|ticks|, |ticks_min|) * 1e-6, static * 8.0).
        The static (class static field, cctor-set) wasn't recoverable from
        the binary; it is a float-epsilon scale -- 1e-6 reproduces the
        intended "simultaneous fill" semantics.  Then the turn-priority list
        is walked BACKWARD: units not in all_list are dropped; survivors
        score speedFillWithOverflow + speed * ticks_min and the best (ties
        grouped) become the priority candidates."""
        if ticks_min == float("inf"):
            return [], []

        def window(t):
            return max(max(abs(t), abs(ticks_min)) * 1e-6, 8e-6)

        all_list = [u for u, t in entries if abs(t - ticks_min) < window(t)]

        priority_candidates = []
        best_score = -1.0
        for u in reversed(self.turn_priority):
            if u not in all_list:            # backward walk, evict non-candidates
                self.turn_priority.remove(u)  # FUN_1836f61e0 RemoveAt
                continue
            score = u.speed_fill_overflow + u.speed * ticks_min
            if score > best_score:
                priority_candidates = [u]
                best_score = score
            elif score == best_score:
                priority_candidates.append(u)
        return all_list, priority_candidates

    # -- ProcessUnitsSpeed (0x181CFFB50) -----------------------------------
    def process_units_speed(self):
        """Pick the next actor.  Priority path first (max speed among
        priority candidates, ties -> BattleRandom.nextInt), else the general
        path over all window candidates: max score = speedFillWithOverflow +
        speed*ticks_min, ties -> max speed, ties -> BattleRandom.nextInt.
        Then AdvanceUnitSpeedFill(ticks_min) and return the actor."""
        ticks_min, entries = self._determine_ticks()
        if not entries:
            return None
        all_list, prio = self._populate_candidates(entries, ticks_min)

        chosen = None
        if len(prio) > 1:
            top = max(u.speed for u in prio)
            group = [u for u in prio if u.speed == top]
            chosen = group[0] if len(group) == 1 else \
                group[self.ctx.battle_random.next_int(len(group))]
        elif len(prio) == 1:
            chosen = prio[0]

        if chosen is None:
            scores = [(u, u.speed_fill_overflow + u.speed * ticks_min)
                      for u in all_list]
            best = max(s for _, s in scores)
            group = [u for u, s in scores if s == best]
            if len(group) > 1:
                top = max(u.speed for u in group)
                g2 = [u for u in group if u.speed == top]
                chosen = g2[0] if len(g2) == 1 else \
                    g2[self.ctx.battle_random.next_int(len(g2))]
            else:
                chosen = group[0]

        self._advance_unit_speed_fill(ticks_min)
        return chosen

    # -- AdvanceUnitSpeedFill (0x181D013B0) --------------------------------
    def _advance_unit_speed_fill(self, ticks):
        """Every eligible unit: speedFill += speed*ticks and
        speedFillWithOverflow += speed*ticks; limit-break/unique ability
        cooldowns tick down by (base_cooldown + modifyCooldownAmount)*ticks,
        clamped to [0, max_cooldown].  The ACTOR is zeroed separately when
        its turn starts (BattleFsmController_TriggerTurnStarted 0x18193B880:
        SetSpeedFill(0); speedFillWithOverflow = 0) -- no overflow carry."""
        for u in self.units:
            if not u.alive:
                continue
            u.speed_fill += u.speed * ticks
            u.speed_fill_overflow += u.speed * ticks
            for cd in u.cooldowns:
                _, base_cd, max_cd, rem = cd
                rem -= int(round((base_cd + 0.0) * ticks))   # modify amount: effect pipeline pending
                cd[3] = _clamp(rem, 0, max_cd)

    # -- BattleFsmController_TriggerTurnStarted ----------------------------
    def begin_turn(self, unit):
        """The acting unit's meter resets to zero in BOTH fields."""
        unit.speed_fill = 0.0
        unit.speed_fill_overflow = 0.0

    # -- damage module (BattleEffectControllerDamageModule) ----------------
    def process_crit_chance(self, attacker, target, is_attack):
        """ProcessCritChance (0x181365B20), effect damage-type 1=attack/2=ability:
        chance = (atkRating - tgtNegateRating)/CritRatingMod
                 + (atkPctAdd - tgtNegatePctAdd)          [clamped <= 1.0]
                 + clamp(levelDiff * PerLevelDelta, Min, Max)
                 + BaseCritChance
        roll = BattleRandom.nextInt(100) < chance * 100.0"""
        if is_attack:
            rating, negate = (STAT_ATTACK_CRIT_RATING,
                              STAT_ATTACK_CRIT_NEGATE_RATING)
            pct, pct_neg = (STAT_CRIT_PERCENT_ADDITIVE,
                            STAT_CRIT_NEGATE_PERCENT_ADDITIVE)
            rating_mod, per_level = CRIT_RATING_MOD, CRIT_CHANCE_LEVEL_DELTA_PER_LEVEL
            d_min, d_max = CRIT_CHANCE_LEVEL_DELTA_MIN, CRIT_CHANCE_LEVEL_DELTA_MAX
            base = BASE_ATTACK_CRIT_CHANCE
        else:
            rating, negate = (STAT_ABILITY_CRIT_RATING,
                              STAT_ABILITY_CRIT_NEGATE_RATING)
            pct, pct_neg = (STAT_CRIT_PERCENT_ADDITIVE,
                            STAT_CRIT_NEGATE_PERCENT_ADDITIVE)
            rating_mod, per_level = CRIT_RATING_MOD, CRIT_CHANCE_LEVEL_DELTA_PER_LEVEL
            d_min, d_max = CRIT_CHANCE_LEVEL_DELTA_MIN, CRIT_CHANCE_LEVEL_DELTA_MAX
            base = BASE_ABILITY_CRIT_CHANCE
        chance = (attacker.get_stat(rating) - target.get_stat(negate)) / rating_mod
        chance += attacker.get_stat(pct) - target.get_stat(pct_neg)
        if chance >= 1.0:                      # decompiled: capped at 1.0
            chance = 1.0
        level_term = _clamp(
            (attacker.level - target.level) * per_level, d_min, d_max)
        total = chance + level_term + base
        return self.ctx.battle_random.next_int(100) < total * 100.0

    def process_crit_damage(self, target):
        """ProcessCritDamage (0x181365F10):
        multiplier = getStat(CRITICAL_DAMAGE) + UnitBaseCritDamage, min 1.0"""
        return max(target.get_stat(STAT_CRIT_DAMAGE) + BASE_CRIT_DAMAGE, 1.0)

    def process_source_damage(self, attacker, target, base, mult, flat,
                              is_attack):
        """ProcessSourceDamage (0x1813658C0):
        base_dmg = baseStat * mult + flat
        physical: * (relicDelta + 1.0)
                  * (clamp(levelDiff * PerLevelDelta, Min, Max) + 1.0)
        ability:  * (clamp(eraDiff * PerEraLevelDelta, EraMin, EraMax) + 1.0)
        (relicDelta: GetRelicLevelDeltaMod -- gear pipeline pending.)"""
        dmg = base * mult + flat
        if is_attack:
            lvl_term = _clamp((attacker.level - target.level)
                              * DAMAGE_MOD_PER_LEVEL_DELTA,
                              DAMAGE_MOD_LEVEL_DELTA_MIN,
                              DAMAGE_MOD_LEVEL_DELTA_MAX)
            return dmg * (1.0 + 0.0) * (lvl_term + 1.0)   # relicDelta + 1.0
        era_term = _clamp((attacker.era - target.era)
                          * DAMAGE_MOD_PER_ERA_LEVEL_DELTA,
                          DAMAGE_MOD_ERA_LEVEL_DELTA_MIN,
                          DAMAGE_MOD_ERA_LEVEL_DELTA_MAX)
        return dmg * (era_term + 1.0)

    def process_mitigation_stats(self, target, attacker, is_attack):
        """ProcessMitigation (0x181366000) stat selection:
        physical -> target ARMOR vs attacker ARMOR_PENETRATION
        ability  -> target SUPPRESSION vs attacker SUPPRESSION_PENETRATION
        plus target's DEFENSE_PENETRATION_TARGET_PERCENT_ADDITIVE (stat 56)."""
        if is_attack:
            return (target.get_stat(STAT_ARMOR),
                    attacker.get_stat(STAT_ARMOR_PENETRATION))
        return (target.get_stat(STAT_SUPPRESSION),
                attacker.get_stat(STAT_SUPPRESSION_PENETRATION))

    @staticmethod
    def mitigation_percent(rating, attacker_level, is_attack):
        """GetMitigationPercentFromRating (0x180FDFDF0, from disassembly --
        the C decompile mangled the float return):
        pct = rating / (rating + (attacker_level + LevelOffset) * RatingMod) * 100
        RatingMod: physical UnitArmorRatingMod(+0x22F4)=100, ability
        UnitSuppressionRatingMod(+0x24E0)=100; ship-mode variants
        (+0x1C28/+0x1C64) also 100.  LevelOffset (+0x1C34) is data-driven."""
        mod = ARMOR_RATING_MOD if is_attack else SUPPRESSION_RATING_MOD
        denom = rating + (attacker_level + MITIGATION_LEVEL_OFFSET) * mod
        return rating / denom * 100.0

    def apply_damage_pipeline(self, attacker, target, caster_power, effect,
                              is_attack):
        """DamageModule.ApplyToTargets (0x181364CA0 -> damage_apply_to_targets.c),
        the full chain per target:

            dmg = caster_power * effect.mult(+0x2C) [+ effect.flat(+0x24)]
            dmg *= ProcessSourceDamage(attacker, target)     # level/era scaling
            if crit (roll):  dmg *= ProcessCritDamage(attacker)
            dmg *= GetModifyEffectAmount(attacker, target, stat=28)
            dmg *= ProcessMitigation(attacker, target)       # 1 - mitigation%
            # variance roll on BattleRandom: nextInt(v*2|1) - v, v = dmg * effect.variance(+0x30)
            # shield: stat 34 SHIELD_PENETRATION (clamped 0..1) splits hp/shield parts
            # effect flag +0xf0 & 1: cannot reduce below 1 HP
            # then ApplyAttackDamage(target, -hp_part, -shield_part, ...)
        (caster_power = DamageModule.GetContextValue(context), virtual slot
        klass+0x1f8 -> base impl 0x18136A570: SUM of this unit's float results
        recorded by preceding effect modules in the ability's
        BattleEffectContext.EffectResults (List<PooledDictionary<Unit,float>>
        at context+0x18).  effect = the ability's damage effect DO.)"""
        dmg = caster_power * effect.get("mult", 1.0) + effect.get("flat", 0.0)
        dmg *= self.process_source_damage(
            attacker, target, effect.get("base_stat_value", 0.0),
            effect.get("source_mult", 1.0), effect.get("source_flat", 0.0),
            is_attack)
        if self.process_crit_chance(attacker, target, is_attack):
            dmg *= self.process_crit_damage(attacker)
        mit = self.process_mitigation_stats(target, attacker, is_attack)
        dmg *= 1.0 - self.mitigation_percent(mit[0], attacker.level, is_attack) / 100.0
        variance = effect.get("variance", 0.0)
        if variance > 0.0:
            v = int(dmg * variance)
            dmg += self.ctx.battle_random.next_int(v * 2 | 1) - v
        return max(dmg, 0.0)

    @staticmethod
    def modify_effect_amount(outgoing_mods, incoming_mods):
        """PersistentEffectsController.GetModifyEffectAmount
        (private 0x181A73980 -> get_modify_effect_amount_priv.c, public wrapper
        0x181A73880, static multiplier read from .rdata 0x18529EF3C = 1.0):

        per matching persistent effect, its modifier amounts aggregate per
        source-key -- positives as MAX, negatives as MIN (strongest wins;
        weaker same-key modifiers are skipped); totals sum across effects;
        final = (1 + outgoing_total) * (1 + incoming_total), clamped >= 0.
        EffectParamEnum 45=INCOMING 46=OUTGOING.  Modifiers excluded by the
        incoming effect's NO_MODIFIER-style params are dropped first.
        Each arg: iterable of (source_key, amount)."""
        def aggregate(mods):
            pos, neg = {}, {}
            for key, amount in mods:
                if amount > 0.0:
                    if key not in pos or amount > pos[key]:
                        pos[key] = amount
                elif amount < 0.0:
                    if key not in neg or amount < neg[key]:
                        neg[key] = amount
            return sum(pos.values()) + sum(neg.values())
        total = (aggregate(outgoing_mods) + 1.0) * (aggregate(incoming_mods) + 1.0)
        return max(total, 0.0)

    # -- stat aggregation (StatCalculator.GetCalculatedStatValue) -----------
    # Stats the final clamp/truncate pass keeps fractional (switch default
    # truncates everything else to int): 16, 18-19, 24-45, 53-62.
    _FLOAT_STATS = ({16} | set(range(18, 20)) | {24}
                    | set(range(24, 46)) | set(range(53, 63)))
    _MIN_ONE_STATS = (1, 6, 7)   # MAX_HEALTH, ATTACK_DAMAGE, ABILITY_POWER

    def get_calculated_stat_value(self, unit, stat, initial=0.0):
        """StatCalculator.GetCalculatedStatValue (0x180FDE5C0 ->
        stat_calculated_value.c).  Five passes over the modifier lists:

            v = base(unit, stat) + initial
            v += Σ slot4(m, stat)                    # GetAdditivesForStat (0x180FE0480)
            if stat != MASTERY(61):
                v += calc_stat(MASTERY) * mastery_contribution(stat)   # 0x180FDFF40
            p += Σ (slot4 + slot5)(m, stat)          # GetPercentAdditivesForStat (0x180FE0280)
            v = unit.slot5(stat, exclude, skip) + (1 + p) * v          # statSource's own
            v += Σ slot5(m, stat, exclude, skip)     # GetAdditivesForStat again, aware form
            # clamps: MAX_HEALTH/ATTACK/ABILITY_POWER floor 1.0 (+inf/nan guard);
            # MAX_SHIELD(25) floor 0; then int-truncate unless stat in _FLOAT_STATS
        (slot4 = plain modifier accessor, slot5 = exclusion-aware accessor --
        both on the IStatModifierSource interface; the modifier lists are
        stand-ins on BattleUnit until the persistent-effect pipeline lands.)"""
        v = unit.get_stat(stat) + initial
        for m in unit.additives_for_stat(stat):
            v += m.get("flat", 0.0)
        if stat != STAT_MASTERY:
            v += self.get_calculated_stat_value(unit, STAT_MASTERY) \
                * unit.mastery_contribution(stat)
        p = 0.0
        for m in unit.percent_additives_for_stat(stat):
            p += m.get("pct", 0.0) + m.get("pct_aware", 0.0)
        v = 0.0 + (1.0 + p) * v   # statSource slot5 term: placeholder (no source yet)
        for m in unit.additives_for_stat(stat):
            v += m.get("flat_aware", 0.0)
        if stat in self._MIN_ONE_STATS and (v < 1.0 or v != v or v == float("inf")):
            v = 1.0
        elif stat == 25 and v < 0.0:              # MAX_SHIELD
            v = 0.0
        if stat not in self._FLOAT_STATS:
            v = float(int(v))
        return v

    # -- turn priority (GiveUnitTurnPriority 0x181CFF800) ------------------
    def give_turn_priority(self, unit):
        self.turn_priority.append(unit)

    # -- loop --------------------------------------------------------------
    def run_turn(self, choose_action=None):
        """One FSM pass: pick actor, zero its meter, act."""
        actor = self.process_units_speed()
        if actor is None:
            return None
        self.begin_turn(actor)
        action = choose_action(self, actor) if choose_action else "basic"
        self.round += 1
        self.actors.append({"turn": self.round, "actor": actor.base_id,
                            "action": action})
        return actor
