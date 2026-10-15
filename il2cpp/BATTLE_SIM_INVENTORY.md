# SWGoH Client-Side Battle Simulation — Inventory (2026-10-15)

## DECODE STATUS (Ghidra pass, 2026-10-15 — decompiled sources in `dump/decompiled/*.c`, port in `offline/battle.py`)

Corrections to the guesses below, now confirmed from the binary:

- **Seeding**: all FIVE `BattleContext` JavaRandom streams (+0x158..0x178) are constructed from the
  **same int** (`BattleStartResponse`+0x18) inside `BattleFsmController._SendBattleStartRequest_b__0`
  (0x181A5EFB0 → `battle_start_seed.c`). There is **no separate targeting seed stream** — the proto
  field exists but the client we ship seeds everything from one value. `JavaRandom..ctor` (0x180CAC420)
  = `seed = (s ^ 0x5DEECE66D) & (2^48-1)`.
- **`UnitSpeedFillMax` = 100.0** (not 1000). GameConstants..ctor (0x1811EBF50) hardcodes only some
  fields — extract via `C7 8x disp32 imm32` scan of the ctor's machine code. Hardcoded: SpeedFillMax
  100.0, Attack/AbilityCritRatingMod 100.0, ArmorRatingMod 100.0, SuppressionRatingMod 100.0,
  crit/damage level-delta MAXes 1.0, crit-negate-rating divisors 2400.0. The Min/PerLevel/Base* fields
  are data-driven (hydrated from gamedata; NOT in binary or local assets).
- **Turn order** (`ProcessUnitsSpeed` 0x181CFFB50 → `turn_order.c`): ticks_i = (100−fill)/speed for
  eligible units; candidate window |ticks−ticks_min| < max(maxspan·1e-6, ~8e-6); `_turnPriorityUnits`
  walked backward (non-candidates evicted), survivors scored by overflow+speed·ticks_min; group-max by
  speed; ties → `BattleRandom.nextInt(n)`. Everyone then += speed·ticks (fill AND overflow), ability
  cooldowns −= base·ticks clamp [0,max]. **The actor zeroes BOTH meter fields when its turn starts**
  (`BattleFsmController_TriggerTurnStarted` 0x18193B880) — no overflow carry into the next round.
- **Crit** (`ProcessCritChance` 0x181365B20): chance = (rating−negateRating)/100 + pctAdditive-diff
  (capped 1.0) + clamp(levelDiff·perLevel, Min, Max) + base; roll = `nextInt(100) < chance·100`.
  Crit damage (`ProcessCritDamage`): stat16 + UnitBaseCritDamage, floor 1.0.
- **Damage scaling** (`ProcessSourceDamage` 0x1813658C0): physical ×(relicDelta+1)·(1+clamp(levelDiff·
  perLvl)); ability ×(1+clamp(eraDiff·perEra)).
- **Mitigation** (`GetMitigationPercentFromRating` 0x180FDFDF0 — C decompile mangled, recovered from
  raw listing): `pct = rating / (rating + (attacker_level + LevelOffset(+0x1C34)) * RatingMod) * 100`,
  RatingMod = ArmorRatingMod/SuppressionRatingMod by damage type.
- **ApplyAttackDamage** (0x181BC50B0 → `apply_attack_damage.c`) is the HP-application + event layer
  (death records, ConditionTriggered events, stat-42 turn-meter-gain roll → random valid target).
- **Full damage pipeline** (`DamageModule.ApplyToTargets` 0x181364CA0 → `damage_apply_to_targets.c`):
  per target `dmg = casterPower(virtual +0x1f8)·effectData.mult(+0x2C) [+ flat(+0x24)] ·
  ProcessSourceDamage · (crit ? ProcessCritDamage : 1) · GetModifyEffectAmount(stat 28) ·
  ProcessMitigation`; variance roll = `BattleRandom.nextInt(dmg·variance(+0x30)·2 | 1) − dmg·variance`;
  shield split via stat **34 = SHIELD_PENETRATION** (clamp 0..1, fraction of damage routed through
  shields); effect flag `+0xf0 & 1` = cannot reduce below 1 HP; then ApplyAttackDamage.
  Ported as `BattleSimulation.apply_damage_pipeline` (determinism-verified).
- **casterPower slot identified**: the virtual at klass+0x1f8 is `GetContextValue(BattleEffectContext)`
  (base impl 0x18136A570 → `get_context_value.c`; the module's only other virtual is ApplyToTargets
  at the adjacent invoke slot). It SUMS the unit's float results accumulated by preceding effect
  modules into `BattleEffectContext.EffectResults` (context+0x18,
  `List<PooledDictionary<BattleUnitController,float>>`). So damage input = context-accumulated
  values × effect mult/flat — not a direct stat read.
- **GetModifyEffectAmount decoded** (`BattlePersistentEffectsController`, private 0x181A73980 →
  `get_modify_effect_amount_priv.c`, public wrapper 0x181A73880, EffectParamEnum 45=INCOMING
  46=OUTGOING): per matching persistent effect, modifier amounts aggregate **per source-key —
  positives MAX, negatives MIN** (strongest wins); totals = Σ survivors; final =
  `(1 + outgoing) × (1 + incoming)`, clamp ≥ 0 (static multiplier at .rdata 0x18529EF3C = 1.0,
  read from the dll). `ProcessDamageModification` (0x181365FC0) just forwards to
  `BattleStatController.GetModifyEffectAmount(target, effect, modifyType=28, 0xFF)`.
  Ported as `BattleSimulation.modify_effect_amount`.

Source: `il2cpp/dump/cs/il2cpp.cs` (34.5MB stub from Il2CppInspectorRedux; regenerated via
`Il2CppInspector.Redux.CLI.exe process GameAssembly.dll global-metadata.dat -o dump -s -m`).
All line numbers refer to that file. Nearly every method carries an `// 0x...-0x...` RVA
comment, so each class listed here is directly loadable into a decompiler.

## Headline conclusion

**The client runs the entire battle deterministically and locally.** The server's only
simulation inputs live in `Protos.BattleData` (line 61777): a `battle_seed` (int),
`targeting_seed`, per-encounter squads (`BattleEncounter` → `Squad` + `BattleUnitState`),
opponent `UnitDef`s and granted abilities. The client seeds `JavaRandom` (line 317578 — a
Java 48-bit LCG, `next(bits)`/`nextInt(n)` only) streams from that seed, plays both sides
(AI decision trees + player input), computes all damage/effects, logs every event into a
typed record tree (`BattleRecordController`, line 68501), and submits the result — outcome,
final `BattleState`, rounds, and the **full `BattleRecord[]` log** — in `BattleFinishRequest`
(line 64380). There is no separate `BattleReplay`/`Playback` class: the record stream *is*
the replay artifact, and headless re-simulation already exists (`HeadlessMode`/
`SimulatorMode` in `BattleSimulationController`, line 69454; `BattleControllerSimulationModule`,
line 60657).

## 1. Battle simulation core

| Class | Line | Role |
|---|---|---|
| `BattleContext` | 58888 | Battle-scoped service locator: every controller plus **5 `JavaRandom` streams** — `BattleRandom` (0x158), `PlayerAbilitySelectionRandom`/`OpponentAbilitySelectionRandom` (0x160/0x168), `PlayerTargetRandom`/`OpponentTargetRandom` (0x170/0x178). The determinism core. |
| `BattleController : FeatureController` | 59430 | Top-level battle feature controller; dispatches to per-battle-type modules. |
| `BattleControllerPveModule` 60306, `PvpModule` 60494, `SimulationModule` 60657, `GuildRaidModule` 60088, `TerritoryWarModule` 61180, `TerritoryBattleModule` 60752, `TerritoryTournamentModule` 60969, `WarModule` 61371, `ColiseumModule` 59609, `ConquestModule` 59898 | | `IBattleControllerBattleTypeModule` strategy objects (squad sources, encounter count, targeting tactic, `AllowChanceMechanics`, health/shield recovery per encounter). SimulationModule wraps another module with `headlessMode` + `skipEndBattleView`. |
| `BattleDomainController` 62201 / `BattleDomainSimulationController` 62409 | | Domain layer: `BattleStateDO`, `BattleDataDO`, `List<IBattleRecordDO> BattleRecords`, loot; `StartBattle`/`FinishBattle` plumbing. |
| `BattleSimulationController` | 69454 | "Simulator mode" controller — `HeadlessMode`, `SimulatorMode`, `UnitStates`, `SaveBattleOutcome`. Proof the codebase already runs battles without presentation. |
| `BattleFsmController` | 64628 | The battle loop state machine. `BattleStates` enum (64759–64860) is the definitive turn-loop spec: `BattleStart_SendBattleStartRequest`, `Attack_StartRound`, `Attack_ProcessUnitSpeed`, `Attack_StartUnitTurn`, `Attack_ProcessCounterAttacks`, `Attack_UnitDiedFromDOTS`, `Attack_ProcessCooldowns`, `Attack_EndRound`, `EncounterEnd_*`, `BattleEnd_Common_PVP_SendBattleFinishRequest` (76). |
| `BattleUnitsController` | 73318 | Unit roster manager + **turn-order engine** (§3). |
| `BattleUnitController` | 72012 | Per-unit runtime: `CurrentHP`/`MaxHP`, shield stack, `SpeedFill`/`SpeedFillWithOverflow` (turn meter), `AbilityContext`s (attack/leader/limitBreak/unique/ultimate/granted), `StatCalculator`, pending effects, cooldowns, counters, summons. |
| `BattleUnitAbilityController` | 71600 | Per-ability executor: `AbilityDO` + `AbilityContext`, effect controller modules, `_evadingUnits`, activation/apply pipeline. |
| `BattleRecordController` | 68501 | The battle event logger — `LogAbilityRecord`, `LogEffectRecord`, `LogDamageRecord` (0x181BA6CC0), `LogRecoveryRecord`, `LogEffectAppliedRecord`, `LogEffectExpiredRecord`, `LogConditionRecord`, `LogRoundIncrementedRecord`, `LogOpeningMoveRecord`, `LogSummonRecord`, `LogPlayerManual*Record`. Builds the parent/child record tree. |
| `BattleRecordDO` 68565 + subclasses | 56288, 58756, 62573, 63369, 64148, 64164, 68612, … | Game-logic record DTOs (`Timestamp`, `RecordIndex`, `ParentRecord`). |
| `BattleTargetingController` 70891, `BattleTargetingRuleEvaluator` 70971, `BattleTargetingSetDO` 71035 | | Targeting-rule engine (`Evaluate(BattleEffectTargetDO…)` 0x181BB0200), tag/health/stat-range checks. Fully client-evaluable. |
| `BattleBlackboardController` 57542, `BattleConditionPriorityController` 58682, `BattleStatController` 69824 | | Cross-unit value store; condition priority ordering; battle criteria stats. |

Protocol shells (no logic): `Protos.BattleState` 69949, `Protos.BattleEncounter` 64120,
`Protos.BattleUnitState` 72974, `Protos.BattleRecordType` 68590 (17 record types = the DO
subclasses 1:1).

## 2. Damage / ability math — fully client-side

- **`BattleEffectControllerDamageModule`** (line 62718) — the damage formula, all RVAs:
  `ProcessSourceDamage` 0x1813658C0, `ProcessCritChance` 0x181365B20,
  `ProcessCritDamage` 0x181365F10, `ProcessDamageModification` 0x181365FC0,
  `ProcessMitigation` 0x181366000.
- **`StatCalculator`** (503419): `GetCalculatedStatValue` (0x180FDE5C0),
  `GetCoreStatContribution`, `GetStatPercentageFromRating` (rating→percent),
  `GetMitigationPercentFromRating` (0x180FDFDF0), `GetMasteryContribution`.
  Pure functions over `IStatSource` + tables + `GameConstants`.
- **`GameConstants`** (234952) — all tuning constants as `public readonly` fields:
  `UnitMaxMitigation` (236518), `UnitArmorRatingMod` (236447), `UnitBaseCritDamage`
  (236457), `UnitBaseDodgeChance`/`UnitBaseDeflectionChance` + negate-rating chances
  (236458–236461), crit-chance per-level deltas (236442–236455),
  `UnitStatArmorPerAgility/PerStrength` (236529), `UnitSpeedFillMax` (236527),
  `ResetSpeedFillBetweenEncounters` (236157). The numeric skeleton of the whole sim.
- **Stat model**: `Protos.UnitStat` enum (602397) — 61 stats incl. ARMOR/SUPPRESSION +
  penetrations, DODGE/DEFLECTION ratings, ATTACK/ABILITY CRITICAL ratings,
  ACCURACY/RESISTANCE, HEALTHSTEAL, MASTERY. `StatDefinitionDO` 503538, `StatDO` 503479,
  `StatDef` proto 503520.
- **Effect magnitude math**: `Protos.Effect` (190473) carries `multiplier_amount_decimal`,
  `context_multiplier_decimal`, `additive_amount_decimal`, `result_variance_decimal`,
  `chance`, `apply_type`, `damage_type`; surfaced by
  `BattleEffectDO.MultiplierAmount/AdditiveAmount/ContextMultiplier/ResultVariance/
  GetStatEffectAmount()` (63171–63244). Percent rolls use the seeded `JavaRandom`
  (`BattleConditionTracker.ConditionStatus.RerollChance(JavaRandom)` 58819).

Verdict: **no server round-trip anywhere in the damage path** — everything is a pure
function of unit stats + gamedata + seeded RNG.

## 3. Turn / order system

All inside `BattleUnitsController` (73318), driven by FSM state `Attack_ProcessUnitSpeed`:
`ProcessUnitsSpeed(List<BattleUnitController>) → BattleUnitController` (73550,
RVA 0x181CFFB50), `DetermineTicksToNextAttack` (73551), `PopulateNextUnitCandidateList`
(73553), `AdvanceUnitSpeedFill(float)` (73554), `SortTurnOrder` (73549),
`SetVisualTurnOrder` (73555 — presentation only), `GiveUnitTurnPriority` (73548).
Turn meter = `BattleUnitController.SpeedFill` / `SpeedFillWithOverflow` (72191–72196,
setter `SetSpeedFill` 72544), capped by `GameConstants.UnitSpeedFillMax`. Speed stat from
`StatCalculator.GetCalculatedStatValue(UNITSTATSPEED…)`. Round/turn counting:
`BattleFsmController.TurnCount/PlayerTurnCount/OpponentTurnCount` (64703–64705),
`ProcessEncounterRecovery` (73556).

## 4. Status effects (buffs/debuffs)

- **`BattleEffectDO`** (63129) — runtime effect instance (`IsPersistentBuff/Debuff`):
  `BattleEffectDefinitionDO Definition`, `Creator`, `ContextIndexes` (chained effect
  results), `ApplyEffects` (sub-effect chain), plus all definition data (Chance, string
  `Id`, `EffectType`, `ApplyType`, `DamageType`, multiplier/additive/context-multiplier/
  variance, `Targets`, `TriggerConditions`/`ExpirationConditions`, `DescriptiveTags`,
  stacking-line strings, summon params, `Params: ulong` bitset).
- **`BattleEffectDefinitionDO`** (63248) — static definition built from `Protos.Effect`
  by `GameDataDomainController`'s factory.
- **`BattlePersistentEffectsController`** (67503) — per-unit buff/debuff container:
  `_persistentEffects`, `_appliedBuffEffects`/`_appliedDebuffEffects`,
  `EffectTagAdded/Removed/Dispelled` events, trigger + expiration `BattleConditionTracker`s,
  and the five stat-modifying effect types (`_modifyDamageEffectType`,
  `_modifyRecoverHealth/Shield/SpeedFill/CooldownEffectType`).
- **`BattleConditionDO`** (58580) — `ConditionTypeEnum` + **string** `ConditionValue` +
  priority; evaluated by **`BattleConditionTracker`** (58782, `ConditionStatus.IsMatch`
  0x1817DC710, `RerollChance(JavaRandom)`); priority ordering in
  `BattleConditionPriorityController` (58682).
- **Effect executors**: ~35 `BattleEffectController*Module` subclasses (62616–63110) —
  Damage, Recovery, Dispell, Revive, Summon, BonusMove, CopyEffect, ReplaceEffect,
  Blackboard* (compare/limit/loop/modify/query), ScriptComponent, … — dispatched by
  `BattleEffectControllerModuleFactory` (62837) keyed on `EffectType`.
  `BattleEffectContext` (62595) is the invocation-scoped result accumulator (per-target
  float results feeding `contextIndex` chaining).
- **Referencing model**: everything is **string ids + tags** (effect id, descriptive/effect
  tags, targeting-rule ids, stacking lines), plus int params — directly matching the
  swgoh-utils/gamedata mirror JSON, not opaque ints.

## 5. Gamedata ingestion

- **`Protos.GameDataResponse`** (238461) — the master payload: `List<Effect>`,
  `List<Ability>`, `List<SkillDefinition>`, `List<UnitDef>`, `List<Table>`,
  `List<StatProgression>`, `List<TargetingSet>`, `List<AbilityDecisionTree>`,
  `List<Protos.Category>`, `List<DatacronSet>`, `List<Campaign>`, … **This is** the
  structure the swgoh-utils/gamedata JSON mirror dumps: `ability.json`≈`Protos.Ability`
  (12506; `cooldown`, `tier[]` w/ `AbilityTier`, `effect_reference[]`, `ai_params`,
  `blocking_effect_id`), `skill.json`≈`SkillDefinition` (488587; `is_zeta`,
  `omicron_mode`, `ability_reference`), `units.json`≈`UnitDef` (595292; `base_stat:
  StatDef`, `stat_progression_id`, `unit_tier[]`, embedded
  `basic_attack`/`leader_ability`/`limit_break`/`unique_ability`), `effect.json`≈
  `Protos.Effect` (190473), plus targetingset/table. Request side: `GameDataRequest`
  (238431) + `GameDataSegment` (238742).
- **`GameDataLoader`** (238276) — fetch/cache. **`GameData`** (237221) — translation hub
  (runs every `XTranslator`/`XDO.Factory` over the proto lists).
- **`GameDataDomainController`** (237545) — the queryable store the sim reads:
  `GetUnitDefinition(id)` (0x181BE3680), `GetAbilityDefinition(id)`,
  `GetBattleEffectDefinition(id)`, `GetStatProgression(id)`, `GetTable(id)`,
  `GetTargetingSet(id)`, `GetUnitDefinitionsWithCategory(…)`,
  `GetUnitDefinitionsWithBaseId(…)` — ~40 accessors with RVAs.
- Runtime DOs: `AbilityDO` 12795, `AbilityDefinitionDO` 13211, `UnitDefinitionDO` 595526,
  `UnitDO` 594948, `SkillDefinitionDO` 488627, `AbilityDecisionTreeDO` 13104 /
  `AbilityDecisionTreeTranslator` 13155 / `AbilityDecisionTreeProvider` 13122.

## 6. Server boundary (RPC)

- **`BattleService : ServiceBase`** (69128) — the two battle RPCs with RVAs:
  `SendBattleStartRequest` (handler 0x180F8F750), `SendBattleFinishRequest(…)` (69339,
  RVA 0x180F8D7C0). Request id `SendBattleFinishRequest = 23` in the request-type enum
  (64559).
- **Flow**: `BattleStartRequest` (69738) → `BattleStartResponse` (69793:
  `Protos.BattleData`) → **local deterministic sim** → `BattleFinishRequest` (64380:
  `battle_round[]`, `Protos.BattleState battle_state`, `battle_outcome`, **`battle_record[]`**,
  `battle_stats: BattleCriteriaStats`, `submit_results`) → `BattleFinishResponse` (64432).
  FSM state `BattleEnd_Common_PVP_SendBattleFinishRequest` confirms PvP uses the identical
  submit-records path (client-simulated there too).
- **`Protos.BattleData`** (61777) = the replay input tuple: `battle_seed`,
  `targeting_seed`, `BattleEncounter[]` (64120), `units` (opponent `UnitDef`s),
  `granted_ability`.
- **`Protos.BattleRecord`** (68437) = one replay event: `record_type`, `timestamp`,
  `source_unit_id`, `target_unit_ids[]`, `ability_id`, `effect_id`, `damage_amount`
  (string-encoded decimal), `recovery_type`/`recovery_amount`, `is_crit`,
  `auto_battle_on`, `encounter_index`, `excess_*`, `ally_unit_id`, `record_index`.
- **RNG**: `JavaRandom` (317578) — 48-bit LCG, exactly Java's `Random.next`/`nextInt`;
  trivially reimplementable.
- **Missing piece**: no "watch someone else's battle" client exists; replaying another
  player's battle means re-simulating from their `BattleData` (seed+squads) — which the
  headless simulation module demonstrates is a supported mode.

## Offline replay feasibility

**Exists client-side (all with RVAs for decompilation):** turn order + speed fill, full
battle FSM, damage/crit/mitigation math, effect/condition engine (~35 modules), targeting
rules, blackboards, AI for both sides (decision tree + ability AI + targeting tactics incl.
seeded `BattleRandomTargetingTactic` 68396), deterministic RNG, gamedata schema (mirror JSON
maps 1:1), a record/playback wire format, and an existing headless simulation mode.

**To reconstruct:** method **bodies** (recover via Ghidra/IDA at the RVAs — effort
concentrated in `ProcessUnitsSpeed`, `ProcessSourceDamage`, `ProcessCritChance`, effect-module
apply methods, `ConditionStatus.IsMatch`); `ConditionTypeEnum`/`EffectParamEnum`/`EffectTag`
per-case semantics; equipment/mod/datacron stat flows through `StatCalculator`; encounter
initial states for mid-battle resume and multi-encounter carryover.

**Assessment: high feasibility.** An offline replayer needs (a) gamedata JSON loader,
(b) `JavaRandom`, (c) the ~10 core classes below, (d) `BattleData` (seed + squads) as
input. `BattleRecord[]` output gives a ready-made validation oracle against real captures.

## Top 10 targets to study next (dump line numbers)

1. `BattleEffectControllerDamageModule` — 62718 (RVAs 0x181364CA0-0x181366360)
2. `BattleUnitsController` — 73318 (`ProcessUnitsSpeed` 73550 → `AdvanceUnitSpeedFill` 73554)
3. `BattleRecordController` + `Protos.BattleRecord`/`BattleRecordType` — 68501 / 68437 / 68589
4. `BattleUnitController` — 72012
5. `BattlePersistentEffectsController` — 67503, with `BattleEffectDO` 63129 / `BattleEffectDefinitionDO` 63248 / `BattleConditionTracker` 58782
6. `StatCalculator` — 503419 + `GameConstants` — 234952 (constants 236157–236530)
7. `BattleFsmController` — 64628 (`BattleStates` 64759-64860)
8. `Protos.GameDataResponse` — 238461 + `GameDataDomainController` — 237545 + `GameDataLoader` — 238276
9. `AbilityDecisionTreeProvider`/`Translator` — 13122/13155 + `BattleAbilityAIController` — 55636 + decision modules 56034–56273
10. `BattleService` — 69128 + `Protos.BattleData` 61777 / `BattleStartResponse` 69793 / `BattleFinishRequest` 64380

Secondary: `BattleContext` 58888 (RNG stream layout), `BattleTargetingRuleEvaluator` 70971,
`BattleControllerSimulationModule` 60657 (headless precedent), `Protos.Effect` 190473,
`Protos.UnitDef` 595292, `JavaRandom` 317578.
