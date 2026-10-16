// BattleSim -- faithful C# port of the client's decoded turn core.
// Source of truth: il2cpp/offline/battle.py (which cites, per method, the
// Ghidra-decompiled GameAssembly.dll function it came from).  Constants were
// extracted from GameConstants..ctor machine code; RNG from JavaRandom..ctor.
using System;
using System.Collections.Generic;
using System.Linq;

namespace SwgohRestored.Battle
{
    // UnitStatEnum (dump/cs/il2cpp.cs:602463) -- values used by the pipeline.
    public static class Stat
    {
        public const int Speed = 5;
        public const int Armor = 8;
        public const int Suppression = 9;
        public const int ArmorPenetration = 10;
        public const int SuppressionPenetration = 11;
        public const int AttackCritRating = 14;
        public const int AbilityCritRating = 15;
        public const int CritDamage = 16;
        public const int AttackCritNegateRating = 22;
        public const int AbilityCritNegateRating = 23;
        public const int CritPercentAdditive = 28;
        public const int MaxShield = 25;
        public const int ShieldPenetration = 34;   // 0..1 fraction routed to shields
        public const int CritNegatePercentAdditive = 40;
        public const int Mastery = 61;
    }

    // GameConstants values -- LIVE RUNTIME TRUTH read from the client process
    // (catalog/game_constants_runtime.json, klass-anchored instance
    // 0x2DCD1A5F000, 2026-10-16).  Ctor hardcodes are superseded by the
    // server-hydrated config values below.
    public static class GameConstants
    {
        public static float UnitSpeedFillMax = 1000.0f;     // +0x24B4 (ctor 100.0f)
        public static float CritRatingMod = 2400.0f;        // +0x2308 / +0x22F0
        public static float CritChanceLevelDeltaMax = 0.15f;// +0x22F8 / +0x22E0
        public static float DamageModLevelDeltaMax = 0.0f;  // +0x2340 (config-set zero)
        public static float ArmorRatingMod = 7.5f;          // +0x22F4
        public static float SuppressionRatingMod = 7.5f;    // +0x24E0
        public static int MitigationLevelOffset = 60;       // +0x1C34

        // data-driven -- live-hydrated from the server config at login
        public static float CritChanceLevelDeltaPerLevel = 0.01f; // +0x2300
        public static float CritChanceLevelDeltaMin = 0.0f;       // +0x22FC
        public static float BaseAttackCritChance = 0.1f;          // +0x2314
        public static float BaseAbilityCritChance = 0.1f;         // +0x230C
        public static float BaseCritDamage = 1.5f;                // +0x231C
        public static float DamageModPerLevelDelta = 0.0f;        // +0x2348
        public static float DamageModLevelDeltaMin = 0.0f;        // +0x2344
        public static float DamageModPerEraLevelDelta = 0.075f;   // +0x233C
        public static float DamageModEraLevelDeltaMin = -0.75f;   // +0x2338
        public static float DamageModEraLevelDeltaMax = 0.75f;    // +0x2334
    }

    /// Runtime slice of BattleUnitController (+0xC0 hp, +0x208 speedFill,
    /// +0x20C speedFillWithOverflow).
    public class BattleUnit
    {
        public string BaseId;
        public string UnitId;
        public int Side;                // 0 player, 1 opponent
        public int Level = 85;
        public int Era;
        public Dictionary<int, double> Stats = new Dictionary<int, double>();
        public double CurrentHp = 1.0;
        public double MaxHp = 1.0;
        public bool HasFled;
        public double SpeedFill;           // +0x208
        public double SpeedFillOverflow;   // +0x20C
        public bool IsAttackRole = true;   // basic = physical (vs ability)

        public bool Alive => !HasFled && CurrentHp > 0;
        public double GetStat(int s) => Stats.TryGetValue(s, out var v) ? v : 0.0;
        public double Speed => GetStat(Stat.Speed) < 0 ? 0 : GetStat(Stat.Speed);
    }

    /// One damage effect DO (mult +0x2C, flat +0x24, variance +0x30, source
    /// stat/mult/flat from ProcessSourceDamage's inputs).
    public class DamageEffect
    {
        public double Mult = 1.0;
        public double Flat = 0.0;
        public double Variance = 0.0;
        public double BaseStatValue;   // stat value fed to ProcessSourceDamage
        public double SourceMult = 1.0;
        public double SourceFlat;
        public bool IsAttack = true;   // 1=physical, 2=ability
        public bool KeepTargetAtOneHp; // effect flag +0xf0 & 1
    }

    /// BattleContext (+0x158..+0x178): five JavaRandom streams, all seeded with
    /// the single battle seed (BattleFsmController._SendBattleStartRequest_b__0
    /// 0x181A5EFB0 -- decompiled/battle_start_seed.c).
    public class BattleContext
    {
        public JavaRandom BattleRandom;
        public JavaRandom PlayerAbilityRandom;
        public JavaRandom OpponentAbilityRandom;
        public JavaRandom PlayerTargetRandom;
        public JavaRandom OpponentTargetRandom;

        public BattleContext(long battleSeed)
        {
            BattleRandom = new JavaRandom(battleSeed);
            PlayerAbilityRandom = new JavaRandom(battleSeed);
            OpponentAbilityRandom = new JavaRandom(battleSeed);
            PlayerTargetRandom = new JavaRandom(battleSeed);
            OpponentTargetRandom = new JavaRandom(battleSeed);
        }
    }

    public class BattleEvent
    {
        public int Turn;
        public string Actor;         // baseId
        public string ActorSide;
        public string Kind;          // "turn" | "damage" | "crit" | "death"
        public string Target;
        public double Amount;
        public string Note;
    }

    /// BattleUnitsController turn loop + DamageModule pipeline (headless port).
    public class BattleSim
    {
        readonly BattleContext _ctx;
        readonly List<BattleUnit> _units;
        readonly List<BattleUnit> _turnPriority = new();   // +0xB8
        public List<BattleEvent> Log = new();
        public int Round;

        public BattleSim(IEnumerable<BattleUnit> player, IEnumerable<BattleUnit> opponent, long seed)
        {
            _ctx = new BattleContext(seed);
            _units = player.Concat(opponent).ToList();
        }

        public IReadOnlyList<BattleUnit> Units => _units;

        static double Clamp(double v, double lo, double hi) => v < lo ? lo : v > hi ? hi : v;

        // -- DetermineTicksToNextAttack (0x181D006F0) ------------------------
        (double ticksMin, List<(BattleUnit u, double t)> entries) DetermineTicks()
        {
            var entries = new List<(BattleUnit, double)>();
            double ticksMin = double.PositiveInfinity;
            foreach (var u in _units)
            {
                if (u.Speed <= 0 || u.HasFled || u.CurrentHp <= 0) continue;
                double t = Math.Max(GameConstants.UnitSpeedFillMax - u.SpeedFill, 0) / u.Speed;
                entries.Add((u, t));
                if (t < ticksMin) ticksMin = t;
            }
            return (ticksMin, entries);
        }

        // -- PopulateNextUnitCandidateList (0x181D00EE0) ---------------------
        (List<BattleUnit> all, List<BattleUnit> prio) PopulateCandidates(
            List<(BattleUnit u, double t)> entries, double ticksMin)
        {
            if (double.IsPositiveInfinity(ticksMin)) return (new(), new());
            double Window(double t) => Math.Max(Math.Max(Math.Abs(t), Math.Abs(ticksMin)) * 1e-6, 8e-6);
            var allList = entries.Where(e => Math.Abs(e.t - ticksMin) < Window(e.t))
                                 .Select(e => e.u).ToList();

            var prio = new List<BattleUnit>();
            double best = -1;
            for (int i = _turnPriority.Count - 1; i >= 0; i--)   // backward walk
            {
                var u = _turnPriority[i];
                if (!allList.Contains(u)) { _turnPriority.RemoveAt(i); continue; }
                double score = u.SpeedFillOverflow + u.Speed * ticksMin;
                if (score > best) { prio.Clear(); prio.Add(u); best = score; }
                else if (score == best) prio.Add(u);
            }
            return (allList, prio);
        }

        // -- ProcessUnitsSpeed (0x181CFFB50) ---------------------------------
        public BattleUnit ProcessUnitsSpeed()
        {
            var (ticksMin, entries) = DetermineTicks();
            if (entries.Count == 0) return null;
            var (allList, prio) = PopulateCandidates(entries, ticksMin);

            BattleUnit chosen = null;
            if (prio.Count > 1)
            {
                double top = prio.Max(u => u.Speed);
                var group = prio.Where(u => u.Speed == top).ToList();
                chosen = group.Count == 1 ? group[0]
                       : group[_ctx.BattleRandom.NextInt(group.Count)];
            }
            else if (prio.Count == 1) chosen = prio[0];

            if (chosen == null)
            {
                var scores = allList.Select(u => (u, s: u.SpeedFillOverflow + u.Speed * ticksMin)).ToList();
                double best = scores.Max(x => x.s);
                var group = scores.Where(x => x.s == best).Select(x => x.u).ToList();
                if (group.Count > 1)
                {
                    double top = group.Max(u => u.Speed);
                    var g2 = group.Where(u => u.Speed == top).ToList();
                    chosen = g2.Count == 1 ? g2[0] : g2[_ctx.BattleRandom.NextInt(g2.Count)];
                }
                else chosen = group[0];
            }
            AdvanceSpeedFill(ticksMin);
            return chosen;
        }

        // -- AdvanceUnitSpeedFill (0x181D013B0); actor resets in BeginTurn ---
        void AdvanceSpeedFill(double ticks)
        {
            foreach (var u in _units)
                if (u.Alive) { u.SpeedFill += u.Speed * ticks; u.SpeedFillOverflow += u.Speed * ticks; }
        }

        // -- TriggerTurnStarted (0x18193B880): both meters zero, no carry ---
        public void BeginTurn(BattleUnit u) { u.SpeedFill = 0; u.SpeedFillOverflow = 0; }

        // -- ProcessCritChance (0x181365B20) ---------------------------------
        public bool ProcessCritChance(BattleUnit atk, BattleUnit tgt, bool isAttack)
        {
            int rating = isAttack ? Stat.AttackCritRating : Stat.AbilityCritRating;
            int negate = isAttack ? Stat.AttackCritNegateRating : Stat.AbilityCritNegateRating;
            double chance = (atk.GetStat(rating) - tgt.GetStat(negate)) / GameConstants.CritRatingMod;
            chance += atk.GetStat(Stat.CritPercentAdditive) - tgt.GetStat(Stat.CritNegatePercentAdditive);
            if (chance >= 1.0) chance = 1.0;
            chance += Clamp((atk.Level - tgt.Level) * GameConstants.CritChanceLevelDeltaPerLevel,
                            GameConstants.CritChanceLevelDeltaMin, GameConstants.CritChanceLevelDeltaMax);
            chance += isAttack ? GameConstants.BaseAttackCritChance : GameConstants.BaseAbilityCritChance;
            return _ctx.BattleRandom.NextInt(100) < chance * 100.0;
        }

        // -- ProcessCritDamage (0x181365F10): stat16 + BaseCritDamage, min 1 -
        public double ProcessCritDamage(BattleUnit atk) =>
            Math.Max(atk.GetStat(Stat.CritDamage) + GameConstants.BaseCritDamage, 1.0);

        // -- ProcessSourceDamage (0x1813658C0) -------------------------------
        public double ProcessSourceDamage(BattleUnit atk, BattleUnit tgt,
                                          double baseStat, double mult, double flat, bool isAttack)
        {
            double dmg = baseStat * mult + flat;
            if (isAttack)
            {
                double lvl = Clamp((atk.Level - tgt.Level) * GameConstants.DamageModPerLevelDelta,
                                   GameConstants.DamageModLevelDeltaMin, GameConstants.DamageModLevelDeltaMax);
                return dmg * 1.0 * (lvl + 1.0);   // relicDelta + 1.0 (gear pipeline pending)
            }
            double era = Clamp((atk.Era - tgt.Era) * GameConstants.DamageModPerEraLevelDelta,
                               GameConstants.DamageModEraLevelDeltaMin, GameConstants.DamageModEraLevelDeltaMax);
            return dmg * (era + 1.0);
        }

        // -- GetMitigationPercentFromRating (0x180FDFDF0, from disassembly) --
        double MitigationPercent(double rating, int attackerLevel, bool isAttack)
        {
            double mod = isAttack ? GameConstants.ArmorRatingMod : GameConstants.SuppressionRatingMod;
            double denom = rating + (attackerLevel + GameConstants.MitigationLevelOffset) * mod;
            return rating / denom * 100.0;
        }

        // -- DamageModule.ApplyToTargets (0x181364CA0) -----------------------
        public double ApplyDamage(BattleUnit atk, BattleUnit tgt, double casterPower, DamageEffect e)
        {
            double dmg = casterPower * e.Mult + e.Flat;
            dmg *= ProcessSourceDamage(atk, tgt, e.BaseStatValue, e.SourceMult, e.SourceFlat, e.IsAttack);
            bool crit = ProcessCritChance(atk, tgt, e.IsAttack);
            if (crit) dmg *= ProcessCritDamage(atk);
            double rating = e.IsAttack ? tgt.GetStat(Stat.Armor) : tgt.GetStat(Stat.Suppression);
            dmg *= 1.0 - MitigationPercent(rating, atk.Level, e.IsAttack) / 100.0;
            if (e.Variance > 0)
            {
                int v = (int)(dmg * e.Variance);
                dmg += _ctx.BattleRandom.NextInt(v * 2 | 1) - v;
            }
            if (dmg < 0) dmg = 0;

            // shield split: stat 34 (0..1 fraction), then HP; keep-alive flag
            double frac = Clamp(tgt.GetStat(Stat.ShieldPenetration), 0, 1);
            double shieldPart = dmg * frac;
            double hpPart = dmg - shieldPart;
            if (e.KeepTargetAtOneHp && tgt.CurrentHp - hpPart < 1.0)
                hpPart = Math.Max(tgt.CurrentHp - 1.0, 0);
            tgt.CurrentHp -= hpPart;

            Log.Add(new BattleEvent { Turn = Round, Actor = atk.BaseId, ActorSide = atk.Side == 0 ? "player" : "opponent",
                                      Kind = crit ? "crit" : "damage", Target = tgt.BaseId, Amount = dmg });
            if (tgt.CurrentHp <= 0)
                Log.Add(new BattleEvent { Turn = Round, Actor = atk.BaseId, ActorSide = atk.Side == 0 ? "player" : "opponent",
                                          Kind = "death", Target = tgt.BaseId });
            return dmg;
        }

        public void GiveTurnPriority(BattleUnit u) => _turnPriority.Add(u);

        public bool CheckEnd(out int winnerSide)
        {
            winnerSide = -1;
            bool p = _units.Any(u => u.Side == 0 && u.Alive);
            bool o = _units.Any(u => u.Side == 1 && u.Alive);
            if (p && o) return false;
            winnerSide = p ? 0 : 1;
            return true;
        }

        /// Deterministic AI: lowest-HP living enemy, basic attack through the
        /// full decoded pipeline.  (Real AbilityDecisionTree is data-driven --
        /// the 17-node default tree is mapped in BATTLE_SIM_INVENTORY.md.)
        public BattleUnit RunTurn(BattleUnit actor)
        {
            BeginTurn(actor);
            Round++;
            var foes = _units.Where(u => u.Side != actor.Side && u.Alive)
                             .OrderBy(u => u.CurrentHp).ToList();
            if (foes.Count == 0) return actor;
            var target = foes[0];
            double power = actor.GetStat(actor.IsAttackRole ? 6 : 7);   // ATK / ABILITY_POWER
            // v0 basic attack: damage = ATK via the casterPower factor; the
            // ProcessSourceDamage factor is neutralized (mult 0 + flat 1)
            // until ability effect-DOs are wired in
            var eff = new DamageEffect { Mult = 1.0, Variance = 0.0, IsAttack = actor.IsAttackRole,
                                         SourceMult = 0.0, SourceFlat = 1.0 };
            ApplyDamage(actor, target, power, eff);
            Log.Add(new BattleEvent { Turn = Round, Actor = actor.BaseId, ActorSide = actor.Side == 0 ? "player" : "opponent",
                                      Kind = "turn", Target = target.BaseId });
            return actor;
        }
    }
}
