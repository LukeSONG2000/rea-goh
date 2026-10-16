# SWGoH Client-Side Fleet Battle (P6 ship combat) — Inventory (2026-10-15)

Companion to `BATTLE_SIM_INVENTORY.md` (read that first — the character-battle
core documented there is *the same engine* that runs fleet battles). This file
maps only what is ship-specific. All line numbers refer to
`il2cpp/dump/cs/il2cpp.cs`; decompiled evidence in `il2cpp/decompiled/
fleet_ship_module.c`, `fleet_crew_stats.c`, `fleet_mitigation_listing.c`
(PyGhidra pass 2026-10-15, read-only open of `ghidra/proj/goh_assembly`).

## Headline conclusion

**There is no separate fleet-battle simulator.** The ship battle is the exact
same `BattleController`/`BattleFsmController`/`BattleUnitsController`/damage
pipeline as ground combat, parameterized by a one-byte
`CombatTypeEnum { CHARACTER=0, SHIP=1 }` (`Protos.CombatType` CHARACTER=1,
SHIP=2 — dump.cs 125354/125361). `BattleController._CombatType` (+0x120,
59430) is the only battle-wide switch; the decompiled turn-order and damage
artifacts (`turn_order.c`, `damage_apply_to_targets.c`, `apply_attack_damage.c`)
contain **zero** `CombatType`/`Ship` references. Everything fleet-flavored —
squad shape, commander, target lock, crew stats, mitigation tuning — is
data-driven (gamedata JSON + `GameConstants` ship fields), not new code paths.

## 1. Battle entry — same RPC, same FSM, `CombatType = SHIP` (GOAL 1)

- **No `FleetBattle*`/`SpaceBattle*` classes exist.** The only ship-named
  battle class is `BattleControllerCombatTypeShipModule` (59850, pair
  `...CharacterModule` 59808) implementing `IBattleControllerCombatTypeModule`:
  pure presentation + squad-size policy (audio key `SET_GS_COMBAT_SHIP_BATTLE`,
  layout/VFX prefabs, `SHIP_BATTLE_SPEED_KEY`). Every method RVA listed at
  59879–59895.
- **Squad composition** (decompiled → `fleet_ship_module.c`):
  - `ShipModule.GetMaxSquadSize` 0x1817EC050 → `GameConstants.
    DefaultShipMaxSquadSize` (+0xAB8, 235391), or `EntryRestrictionsDO+0x14`
    override. Characters use `DefaultCharacterMaxSquadSize` (+0xA18, 235370).
  - `ShipModule.GetMaxStartingSquadSize` 0x1817EC0D0 → +0xAB8 (or override).
  - `ShipModule.GetMaxDeployedBattleSquadSize` 0x1817EC200 →
    `DefaultShipMaxSquadSizeWithReinforcements` (+0xAC4, 235394).
  - `PreBattleControllerShipModule` (418051): `DefaultMaxUnitsAllowed`
    0x1819654D0 → +0xAB8, `DefaultMinUnitsAllowed` 0x1819654F0 →
    `DefaultShipMinSquadSize` (+0xABC, 235392; fleet variant +0xAC0 235393),
    `DefaultMaxReinforcementUnitsAllowed` 0x181965510 →
    `DefaultMaxReinforcementSize` (+0xA38, 235375). All config-sent values,
    not hardcoded — read them from a hydrated `GameConstants`.
  - `SquadUnitTypeEnum` (501756): `LEADER=0, SUPPORT=1, DEFAULT=2,
    COMMANDER=3, REINFORCEMENT=4`. The **capital ship occupies the COMMANDER
    cell**; `SquadDO.PrepareShipSquad` (0x181007EA0, decompiled) indexes
    commander/frigate cells 0..max-1 and pushes REINFORCEMENT cells after.
    `ShipModule.SetupOpponentSquadUnitTypes` 0x1817EC680 calls it per opponent
    squad. So a standard fleet = 1 capital ship (commander) + up to 4-5
    frigates, sizes from config.
- **Server payload is the generic one** — there is no ship variant of
  `BattleStartResponse` (69793) / `Protos.BattleData` (61777: `battle_seed`,
  `targeting_seed`, `BattleEncounter[]`, `units`, `granted_ability`). The
  fleet flavor rides inside `Protos.Squad`/`SquadCell` (499565/499605):
  - `SquadCell.squad_unit_type` (proto 12) = `SquadUnitType
    { UNITTYPELEADER..UNITTYPECOMMANDER, UNITTYPEREINFORCEMENT }` (501746)
    flags the capital ship;
  - `SquadCell.crew_battle_stat` (proto 13, `List<UnitBattleStat>`,
    +0x40) — **server-sent crew stat blocks** (opponent crew data), sibling
    of `unit_battle_stat` (proto 6);
  - `Protos.Unit._crew` (`List<CrewMember>`, 0x118 at 595394) — player-side
    crew roster (`CrewMember`: `unit_id`, `slot`, `skill_reference`,
    `skillless_crew_ability_id`, 150343);
  - `Protos.BattleUnitState.is_commander` (proto 14, 73008) and
    `BattleUnitStateDO.IsCommander` (+0x41, 73063) persist it across
    encounters.
- **Commander identity is gamedata, not code**:
  `BattleUnitController.get_IsCommander` (0x181BBA230, decompiled) = `UnitDO.
  Definition.UnitClass (+0xC4) == 1`. `UnitClassEnum` (594526):
  `UNITCLASS_STANDARD=0, UNITCLASS_COMMANDER=1`; proto `UnitClass` (594519):
  `6=STANDARD, 7=COMMANDER` (matches units.json: 427 ships combatType=2 of
  which 70 unitClass=7 capital ships). `UnitDefinitionDO.CombatType` +0x121,
  read by `UnitDO.get_CombatType` 0x18100F5A0 / `BattleUnitController.
  get_CombatType` 0x181BBA320 (both decompiled — one-byte loads).
- Flow parity: fleet arena uses the same `BattleService` (69128) RPCs; PVE
  module even has `LoadShipBattleView` (60046, 0x1817EDE30). Fleet PVP shells:
  `ShipMatchmakingController` 482596, `ShipPvpNavigationController` 483249,
  `ShipPvpLeaderboardController` 482918.

## 2. Turn / speed system — the identical tick-fill engine (GOAL 2)

- One shared `BattleStates` FSM (64759–64860: `Attack_ProcessUnitSpeed`=30,
  `Attack_StartUnitTurn`=34, `Attack_ProcessCooldowns`=38, ...) — there is no
  ship state machine, no `ProcessShipsSpeed` (grep: 0 hits).
- `BattleUnitsController.ProcessUnitsSpeed` (73550, 0x181CFFB50) and the whole
  tick/candidate-window/priority machinery documented in
  `BATTLE_SIM_INVENTORY.md` §3 run unchanged for ships; `turn_order.c` has no
  combat-type branch. Turn meter is still `BattleUnitController.SpeedFill`
  (+0x208) / `SpeedFillWithOverflow` (+0x20C), zeroed by the actor at
  `TriggerTurnStarted` (0x18193B880, re-decompiled: `SetSpeedFill(0)` then
  `+0x20C = 0` before `StartPlayerUnitMove`).
- Ship speed = `UNITSTATSPEED` (5) from the same `StatCalculator`; ship
  stattables in gamedata carry stat id 5 (see §7). The formation movement /
  speed bands on the fleet screen are presentation only: `ShipUnitPresentation`
  (483500), `ShipAttackMotionAction` (481083), `ShipIdle` (482145),
  `ShipHitComponent` (482133), `ShipDestructionComponent` (481197), plus
  `SetVisualTurnOrder` (73555) which the sim ignores.
- Ability cooldowns: no "CooldownWaves" concept anywhere in the binary (grep
  0 hits) or gamedata. `AbilityCooldownType` proto (12775):
  `ABILITYCOOLDOWNTURNBASED=1` / `ABILITYCOOLDOWNSPEEDBASED=2` (client enum
  12782 `TURN_BASED/SPEED_BASED/UNDEFINED`). ability.json: 6635 abilities
  turn-based vs **97** speed-based — and those 97 are Territory-War strategic
  abilities (`strategicability_*`, `*_platoonability_*`), raid grantables and
  one capital ability (`specialability_capitalprofundity03`), *not* a fleet
  system. Ship cooldowns are ordinary turn-based cooldowns that decay with
  speed ticks inside `ProcessUnitsSpeed` (cooldowns −= base·ticks, clamped —
  `turn_order.c`), which produces the wave-like readiness cycling.

## 3. Targeting — one engine; commander vs frigates is data (GOAL 3)

- Capital-ship abilities and frigate attacks use the same
  `BattleUnitAbilityController`/`AbilityContext` executor and the same
  `BattleTargetingController` + `BattleTargetingRuleEvaluator` (70891/70971).
  No combat-type targeting code exists.
- **首标锁定 / Target Lock is a data-driven debuff effect**, not a status
  class: effect `basicability_tieadvanced_targetlock` (effect.json) carries
  `descriptiveTag: ["target_lock", "featcounter_target_lock",
  "ai_target_lock", "clearable_debuff", "countable_debuff"]`, params
  `FORCE_DEBUFF, UNRESISTABLE`, and expiration conditions. 334 `targetlock*`
  tag references in effect.json.
- Targeting rules query the tag: `battleTargetingRule.json` ids
  `target_random_targetlocked_enemy`, `target_lowest_targetlocked_enemy`,
  `target_random_nontargetlocked_enemy`, `target_lowest_nontargetlocked_enemy`,
  `target_random_targetlocked_nonhealer_enemy` — each is just
  `activeEffectTagCriteria: [{tag:"target_lock"}, {tag:"ignore_me",
  exclude:true}]` + unitClass/battleSide filters (unitClass 6 = ships only).
  Referenced from ability.json (`target_random_targetlocked_enemy` ×26 etc.).
- AI side: the shared decision tree has a `TargetLock` node whose default
  position is listed in BATTLE_SIM §AI; the node's check compares against
  `GameConstants.AbilityAiTargetLockEffectTag` (readonly string +0x88,
  234977 — config-sent; builder `GetTargetLockCheck` 0x180FEF2A0 at 13142).
  Bonus damage / crit-down vs locked ships are ordinary effect modules
  (`targetlock_ability_crit_debuff` etc.).
- "Capital-ship abilities" are plain `AbilityDO`s on the cap ship unit
  (basic + 2-3 specials + unique, standard `cooldown` fields — e.g. Chimaera:
  basicability cooldown 0, specials 3/4/4, unique 0). Crew members *unlock*
  ship specials: `units.json` crew entries carry `skillReference` (e.g.
  GRANDADMIRALTHRAWN slot 0 → `specialskill_CAPITALCHIMAERA03`), resolved
  through `skill.json` → `ability_reference`.

## 4. RNG streams — unchanged (GOAL 4)

Same five `BattleContext` `JavaRandom` streams (+0x158..0x178) seeded from the
single `BattleStartResponse.BattleData.BattleSeed` (proto +0x10) in
`BattleFsmController._SendBattleStartRequest_b__0` (0x181A5EFB0 — shared FSM,
no combat-type branch; see `battle_start_seed.c`). `targeting_seed` (proto +0x28)
exists in the payload but is unused by this client, exactly as in ground
battles. No ship-specific seed or stream anywhere.

## 5. Crew linkage — crew members are the ship's "equipment" (GOAL 5)

No `CapitalShip*` stat class; ships are `UnitDO : IStatSource` (594948) whose
`_equipmentSlots` (+0x78) hold `CrewMemberDO`/`CrewSlotDO` instead of gear:

- `UnitDO.Factory.EquipCrewMembers(unit, allUnits)` (0x181018C30, decompiled)
  builds a `CrewSetDO : EquipmentSetDO<CrewMemberDO>` (150438;
  `_contributionPerStat` StatProgressionDO +0x18, `_statModifierScalar` float
  +0x20) from `UnitDefinitionDO.CrewMemberSlots`; `CrewSlotDO` (150466 region)
  caches `_crewRating` per member on equip. `SetupCrewlessShip` (0x181019060)
  is the empty-crew fallback.
- **Stat math** (`GetCalculatedStatValue` pipeline unchanged): ship stats =
  base ship stats + `CrewSetDO.GetStatModifier(stat)` = Σ_slots `crewRating ×
  scalar × contributionPerStat[stat]`. Evidence: `UnitDO.GetEquippedStatContribution`
  (0x181011C90, decompiled → `fleet_crew_stats.c`) skips the gear bonus for
  `Definition.CombatType != CHARACTER` and adds the crew contribution
  `cachedCrewRating (+0xF0) × simulatedCrewStatModifierScalar (+0x100) ×
  StatDefinitionDO.GetStat(_simulatedCrewContributionPerStat +0xF8, stat)`;
  `GetSimulatedCrewStatContribution` (0x181012060) is the same formula.
  Gate: `UnitDefinitionDO.Crewless` getter (0x18101A900, 595615) — crewless
  ships use a simulated crew rating.
- **Crew rating source**: `CrewRatingCalculator` (150393) `GetCrewRating`
  (0x180FD62A0, decompiled) sums data-driven tables —
  `crewRatingPerUnitRarityTable`, `crewRatingPerUnitLevelTable`,
  `crewRatingPerGearTierTable`, `crewRatingPerAbilityLevelTable`,
  `crewRatingPerModRarityLevelTierTable`, `crewRatingPerRelicTierTable` +
  `crewlessShipCrewRatingModifierPerAbility` (fields +0x18..+0x50).
- **Gamedata wiring**: `units.json` ship rows carry `crew` (per-rarity list:
  e.g. 7★ Chimaera crew = [GRANDADMIRALTHRAWN slot 0]) and
  `crewContributionTableId` (e.g. `stattable_CAPITALCHIMAERA`), resolved in
  `statProgression.json` — 2772 `stattable_*` rows; the Chimaera table lists
  per-crew-rating-point contributions for stats {1,5,6,7,8,9,10,11,12,13,14,
  15,16,17,18,27,28,29,37,38,39,40} (proto ids incl. SPEED=5).
  For real battle replays the server normally ships `SquadCell.crew_battle_stat`,
  so offline replay can often skip recomputing crew stats entirely.

## 6. Damage pipeline — shared modules, ship-mode constants (GOAL 6)

- `BattleEffectControllerDamageModule` (62718) and the whole
  ApplyToTargets → ProcessSourceDamage → crit → mitigation →
  ApplyAttackDamage chain are reused verbatim; `damage_apply_to_targets.c` /
  `apply_attack_damage.c` contain no combat-type branches.
- **There are no `SHIP_*` entries in `UnitStatEnum`** (602463, 62 entries
  UNDEFINED..TAUNT — listed in BATTLE_SIM §2) nor in `Protos.UnitStat`
  (602397). Ships use the same stats: the Chimaera stattable and effect data
  target MAX_HEALTH/ARMOR/SUPPRESSION/SHIELD/… identical ids. Goal premise
  corrected: ship-specific tuning lives in **GameConstants**, not the enum.
- **Ship-mode mitigation variants confirmed** from the raw listing
  (`fleet_mitigation_listing.c`; the C decompile of this method stays mangled):
  `StatCalculator.GetMitigationPercentFromRating` (0x180FDFDF0-0x180FDFF40)
  branches on the statSource's ship-ness (per-target bool) and damage type:
  - damageType physical → `GameConstants.ShipArmorRatingMod` (+0x1C28,
    236190) vs `UnitArmorRatingMod` (+0x22F4, 236447);
  - damageType special → `GameConstants.ShipSuppressionRatingMod` (+0x1C64,
    236203) vs `UnitSuppressionRatingMod` (+0x24E0, 236538);
  - level offset: ships only — `ShipMitigationAttackerLevelOffset` (+0x1C34,
    236194); characters add 0.
  - Formula (registers fully traced): `pct = rating / (rating +
    (attacker_level + shipLevelOffset) × RatingMod) × 100.0`
    (×100.0 literal at .rdata 0x18529F324, same as character pass).
  This **corrects** BATTLE_SIM's recovery, which read +0x1C34 as a generic
  "LevelOffset" — it is the ship-only offset; character mitigation has none.
- Nearby ship feature flags: `GameConstants.ShipFeatureEnabled` (+0x1C2C),
  `Ship5v5ProfileLayoutEnabled` (+0x1C2D) (236191-236192) — config-sent, same
  ConfigEntry mechanism as the rest of GameConstants.

## 7. Gamedata files defining the fleet battle (GOAL 7)

| File (catalog/) | Fleet content |
|---|---|
| `units.json` | 427 ships (`combatType:2`); `unitClass` 6=ship / 7=capital; `crew[]` (unitId, slot, skillReference, skilllessCrewAbilityId); `crewContributionTableId` → `stattable_<ID>`; `actionCountMin/Max`; `capitalUnlockKey`; `commandCost` |
| `gamedata/statProgression.json` | 2772 `stattable_*` crew-contribution tables (per-crew-rating-point per-stat values) + ship base-stat tables |
| `gamedata/ability.json` | ship abilities, standard `cooldown` + `cooldownType` (1=TURN_BASED, 2=SPEED_BASED — speed-based is TW/raid-only); `aiParams`; targeting rule refs incl. `target_*_targetlocked_*` |
| `gamedata/skill.json` | crew skills → `ability_reference` (unlock ship specials per crew tier) |
| `gamedata/effect.json` | target-lock effects (tags `target_lock`, `ai_target_lock`; params `FORCE_DEBUFF`, `UNRESISTABLE`) |
| `gamedata/battleTargetingRule.json` | `target_random/lowest_targetlocked_enemy`, `*_nontargetlocked_*`, `target_random_targetlocked_nonhealer_enemy` (unitClass 6 filters) |
| `gamedata/targetingSet.json` | fleet uses the same `default_pve`/`default_pvp` sets |
| `gamedata/cooldown.json` | meta-cooldowns only: `PVP_FLEET_BATTLE_COOLDOWN`, `PVP_SQUAD_BATTLE_COOLDOWN` (arena refresh costs) — nothing about in-battle cooldowns |
| `gamedata/enums.json` | `unitClass`, `combatType`, `squadUnitType` translations |

No `CooldownWaves` key exists in any file; in-battle cooldown decay is the
speed-tick rule in `ProcessUnitsSpeed`.

## Offline fleet replay feasibility

**Verdict: HIGH — same as (and through) the character-battle replayer.** A
fleet battle replay needs *zero* new simulation code beyond
BATTLE_SIM_INVENTORY's list: identical FSM, tick-fill turn engine, damage
pipeline, effect/condition modules, targeting rules, AI decision trees, and
RNG seeding. Fleet-specific reconstruction is data work, not reversing:

1. **Input tuple** is the same `BattleData` (seed + squads); fleet squads add
   `squad_unit_type=COMMANDER` and server-sent `crew_battle_stat`/`Unit._crew`.
2. **Ship stats**: if captures include `unit_battle_stat`, stats are free;
   otherwise recompute = base progression + `crewRating × scalar ×
   stattable_<id>` (all tables local in statProgression.json/units.json;
   `CrewRatingCalculator`'s rating tables are GameConstants/config-sent —
   capture at login like the other config entries).
3. **Target lock** = ordinary effect with tag `target_lock` + existing
   targeting rules — already supported by the effect/condition engine.
4. Ship-mode mitigation constants are GameConstants fields (+0x1C28/+0x1C34/
   +0x1C64 vs +0x22F4/+0x24E0) — one config capture covers both modes.

Residual unknowns are shared with ground battles (config-sent GameConstants
values, per-case ConditionTypeEnum semantics), not fleet-specific.

## Top fleet targets still open

1. `BattleAbilityAIController.GetAbilityAIResult` per-unit fleet behavior —
   same tree walker 0x1816A2EC0/0x1816A30C0; only tree *ids* differ per squad.
2. `BattleReinforcementController` (68643) vs `BattleReinforcementHeadlessController`
   (68834) — reinforcement FSM for TW fleets (headless variant already exists).
3. `BattleUnitShieldController` — ships lean on shields heavily; shield stack
   logic is shared but ship-tuned via stats 25/29/34.
4. `PreBattleControllerShipModule.BuildInitialSquad` (0x181966BD0) — squad
   preset → COMMANDER cell assembly for captures of squad setup.
5. `UnitDO.GetFullUnitStatusHashCode` (0x181015590) — server-visible unit
   state hash, useful to validate offline stat reconstruction.
