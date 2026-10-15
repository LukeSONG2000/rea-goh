# -*- coding: utf-8 -*-
"""Offline battle skeleton mirroring the client's own architecture
(dump/il2cpp.cs; see il2cpp/BATTLE_SIM_INVENTORY.md for line numbers).

Design contract: the real game is client-authoritative deterministic -- the
server hands over `battle_seed` + `targeting_seed` + squads and the client
simulates everything locally with 5 separated JavaRandom streams
(`BattleContext` 0x158-0x178).  This module reproduces that shape so results
can later be validated against a real captured `BattleRecord[]` log.

Slots marked TODO-DECOMPILE need the method bodies from Ghidra (RVAs noted);
the surrounding plumbing is already real.
"""
from java_random import JavaRandom

# GameConstants (dump 236527) -- value pending decompile of the ctor;
# community consensus is 1000.
UNIT_SPEED_FILL_MAX = 1000.0        # TODO-DECOMPILE GameConstants ctor


class BattleUnit:
    """Runtime slice of BattleUnitController (dump 72012) that turn order
    needs: identity, side, effective speed, turn meter."""

    def __init__(self, base_id, side, speed, unit_id=None):
        self.base_id = base_id
        self.unit_id = unit_id or base_id
        self.side = side               # 'player' | 'opponent'
        self.speed = float(speed)
        self.speed_fill = 0.0          # SetSpeedFill / SpeedFillWithOverflow
        self.alive = True
        # TODO-DECOMPILE: hp/shield/abilities/cooldowns/persistent effects

    def __repr__(self):
        return f"<{self.side}:{self.base_id} fill={self.speed_fill:.0f}>"


class BattleContext:
    """Battle-scoped services; the five RNG streams are the determinism core."""

    def __init__(self, battle_seed, targeting_seed):
        self.battle_random = JavaRandom(battle_seed)
        self.player_ability_random = JavaRandom(battle_seed)   # TODO-DECOMPILE ctor seeding
        self.opponent_ability_random = JavaRandom(battle_seed)
        self.player_target_random = JavaRandom(targeting_seed)
        self.opponent_target_random = JavaRandom(targeting_seed)


class BattleSimulation:
    """BattleFsmController + BattleUnitsController turn loop (headless)."""

    def __init__(self, player_units, opponent_units, battle_seed, targeting_seed):
        self.ctx = BattleContext(battle_seed, targeting_seed)
        self.units = list(player_units) + list(opponent_units)
        self.round = 0
        self.actors = []               # BattleRecord[] equivalent: event log

    # -- BattleUnitsController.ProcessUnitsSpeed (0x181CFFB50) -------------
    def process_units_speed(self):
        """Pick the next actor by turn meter.

        Client algorithm (bodies TODO-DECOMPILE): compute per-unit ticks to
        fill, advance every meter by that tick count, candidates = units at
        cap, tie-break via SortTurnOrder (0x181CFF960) then roster order.
        The loop below is the community-documented equivalent, good enough
        to drive an idle/replay sim until the bodies land.
        """
        alive = [u for u in self.units if u.alive]
        while True:
            capped = [u for u in alive if u.speed_fill >= UNIT_SPEED_FILL_MAX]
            if capped:
                return max(capped, key=lambda u: (u.speed_fill, u.speed))
            ticks = min((UNIT_SPEED_FILL_MAX - u.speed_fill) / u.speed
                        for u in alive)
            for u in alive:
                u.speed_fill += u.speed * ticks
        # TODO-DECOMPILE: GiveUnitTurnPriority overrides, overflow carry

    def run_round(self, choose_action=None):
        """One FSM pass: Attack_StartRound .. Attack_EndRound (dump 64759)."""
        self.round += 1
        actor = self.process_units_speed()
        actor.speed_fill -= UNIT_SPEED_FILL_MAX   # spend the meter
        action = choose_action(self, actor) if choose_action else "basic"
        self.actors.append({"round": self.round, "actor": actor.base_id,
                            "action": action})
        return actor
