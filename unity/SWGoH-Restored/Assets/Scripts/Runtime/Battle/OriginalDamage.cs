using System;
using System.Globalization;
using System.Linq;

namespace SwgohRestored.Battle
{
    public partial class BattleSim
    {
        // Body evidence: damage_module.c (0x1813658C0/B20/F10),
        // damage_apply_to_targets.c (0x181364CA0), and corrected float return
        // logs/effect_recovery/damage_mitigation_float.c (0x181366000).
        // Constants: catalog/game_constants_runtime.json, 2026-10-16 snapshot.
        // This slice excludes context-driven amounts and damage-modifying
        // persistent effects; the graph must reject those before execution.
        static bool DamageFlag(RawEffect e, string flag) => e.param != null && e.param.Contains(flag);

        public static string ValidateOriginalDamage(RawEffect effect)
        {
            if (effect == null || effect.type != 1) return "Expected protobuf DAMAGE effect.";
            if (effect.damageType < 1 || effect.damageType > 3) return "Unknown damage type.";
            if (effect.contextMultiplierDecimal != 0) return "Context-driven damage is not implemented.";
            if (effect.resultVarianceDecimal < 0) return "Negative variance is unsupported.";
            if (!double.TryParse(effect.additiveAmountDecimal ?? "0", NumberStyles.Float,
                    CultureInfo.InvariantCulture, out var amount) || double.IsNaN(amount) || double.IsInfinity(amount))
                return "Invalid additive amount.";
            int sources = 0;
            foreach (string flag in effect.param ?? Array.Empty<string>())
            {
                switch (flag)
                {
                    case "ATTACK_DAMAGE": case "ABILITY_POWER": case "MAX_HEALTH":
                    case "DAMAGE_HEALTH_PERCENTAGE": sources++; break;
                    case "NO_KILL": case "UNRESISTABLE": case "NO_HEALTH_STEAL": break;
                    // These flags suppress side effects which this supported slice
                    // does not produce. Preserve them in the raw graph.
                    case "NO_COUNTER_ATTACK":
                    case "SUPPRESS_VISUALS": case "HIDE_VFX": break;
                    default: return "Unsupported damage parameter: " + flag;
                }
            }
            return sources == 1 ? null : "Damage requires one supported source stat/health-percent parameter.";
        }

        public BattleEvent OriginalDamage(BattleUnit attacker, BattleUnit target, RawEffect effect)
        {
            string reason = ValidateOriginalDamage(attacker, target, effect);
            if (reason != null) throw new NotSupportedException(reason);

            float mult = effect.multiplierAmountDecimal / 10000f;
            float flat = (float)(double.Parse(effect.additiveAmountDecimal ?? "0", CultureInfo.InvariantCulture) / 10000.0);
            float damage;
            if (DamageFlag(effect, "DAMAGE_HEALTH_PERCENTAGE"))
            {
                // ApplyToTargets replaces ProcessSourceDamage with target max HP.
                damage = (int)target.MaxHp * mult;
            }
            else
            {
                int source = DamageFlag(effect, "ATTACK_DAMAGE") ? 6
                    : DamageFlag(effect, "ABILITY_POWER") ? 7 : 1;
                damage = (float)attacker.GetStat(source) * mult + flat;
                float delta = attacker.IsEraUnit
                    ? Math.Max(-0.75f, Math.Min(0.75f, (attacker.Era - target.Era) * 0.075f))
                    : 0f; // live ordinary level modifier is zero; equal relic tiers => zero.
                damage *= 1f + delta;
            }

            bool critical = false;
            if (effect.damageType != 1) // UNMITIGABLE: ProcessCritChance returns before RNG.
            {
                bool physical = effect.damageType == 2;
                float rating = (float)attacker.GetStat(physical ? 14 : 15)
                    - (float)target.GetStat(physical ? 22 : 23);
                float additive = (float)attacker.GetStat(physical ? 28 : 29)
                    - (float)target.GetStat(physical ? 40 : 41);
                float chance = Math.Min(1f, rating / 2400f + additive);
                chance += Math.Max(0f, Math.Min(0.15f, (attacker.Level - target.Level) * 0.01f));
                chance += 0.1f;
                critical = _ctx.BattleRandom.NextInt(100) < chance * 100f;
                if (critical) damage *= Math.Max(1f, (float)attacker.GetStat(16) + 1.5f);
                damage *= OriginalMitigationMultiplier(attacker, target, physical);
            }
            // 0x18136524C: xorps xmm1,xmm1; movss xmm0,[rcx+30];
            // call Approximately(variance, 0). Raw positive decimal variance
            // is at least .0001, so only zero is approximately zero here.
            if (damage > 0f && effect.resultVarianceDecimal != 0)
            {
                int variance = (int)(damage * (effect.resultVarianceDecimal / 10000f));
                damage += _ctx.BattleRandom.NextInt(variance * 2 | 1) - variance;
            }
            damage = Math.Max(0f, damage);
            float penetration = Math.Max(0f, Math.Min(1f, (float)attacker.GetStat(34)));
            float shield = Math.Min(Math.Max(0f, (float)target.CurrentShield), damage - damage * penetration);
            float hp = damage - shield;
            if (DamageFlag(effect, "NO_KILL")) hp = Math.Min(hp, Math.Max(0f, (float)target.CurrentHp - 1f));
            int hpDamage = (int)Math.Max(0f, hp);
            int shieldDamage = (int)Math.Max(0f, shield);
            target.CurrentShield = Math.Max(0, target.CurrentShield - shieldDamage);
            target.CurrentHp = Math.Max(0, target.CurrentHp - hpDamage);
            var ev = new BattleEvent {
                Turn = Round, Actor = attacker.BaseId, ActorUnitId = attacker.UnitId,
                ActorSide = attacker.Side == 0 ? "player" : "opponent", Target = target.BaseId,
                TargetUnitId = target.UnitId, Kind = critical ? "crit" : "damage",
                Amount = hpDamage + shieldDamage, HpDamage = hpDamage, ShieldDamage = shieldDamage,
                Note = effect.id
            };
            Log.Add(ev);
            if (!target.Alive) Log.Add(new BattleEvent {
                Turn = Round, Actor = attacker.BaseId, ActorUnitId = attacker.UnitId,
                ActorSide = ev.ActorSide, Target = target.BaseId, TargetUnitId = target.UnitId, Kind = "death"
            });
            OriginalHealthSteal(attacker, effect, damage);
            return ev;
        }

        // Damage.ApplyToTargets 0x181364CA0: stat24 * final pre-cap float damage,
        // including shield/overkill, truncated once. ApplyHeal 0x181BC6060 rejects
        // dead recipients; UpdateHealth 0x181BC6900 clamps integer HP to max HP.
        // Immunity effects and healing-trigger passives remain outside this slice.
        // Do not substitute descriptiveTag for native IsImmuneToEffect(tag).
        void OriginalHealthSteal(BattleUnit source, RawEffect effect, float damage)
        {
            if (DamageFlag(effect, "NO_HEALTH_STEAL") || !source.Alive) return;
            float recovery = (float)source.GetStat(24) * damage;
            if (recovery <= 0f) return;
            int requested = (int)recovery;
            double before = source.CurrentHp;
            int maximum = (int)source.MaxHp;
            int current = Math.Max(0, Math.Min(maximum, (int)before));
            source.CurrentHp = Math.Max(0, Math.Min(maximum, current + requested));
            double actual = source.CurrentHp - before;
            Log.Add(new BattleEvent {
                Turn = Round, Actor = source.BaseId, ActorUnitId = source.UnitId,
                ActorSide = source.Side == 0 ? "player" : "opponent",
                Target = source.BaseId, TargetUnitId = source.UnitId, Kind = "heal",
                Amount = actual, Heal = actual, EffectId = effect.id,
                Note = effect.id + ":health_steal requested=" + requested.ToString(CultureInfo.InvariantCulture)
                    + " actual=" + actual.ToString(CultureInfo.InvariantCulture)
            });
        }

        public static string ValidateOriginalDamage(BattleUnit attacker, BattleUnit target, RawEffect effect)
        {
            string reason = ValidateOriginalDamage(effect);
            if (reason != null) return reason;
            if (attacker == null || target == null || !attacker.Alive || !target.Alive)
                return "Damage requires living source and target.";
            if (attacker.CombatType != 1)
                return "Ship mitigation constants are not connected to this slice.";
            if (!attacker.IsEraUnit && attacker.RelicTier != target.RelicTier)
                return "Unequal relic tiers require the original battle relic modifier table.";
            if (!DamageFlag(effect, "NO_HEALTH_STEAL") && attacker.GetStat(24) > 0
                && attacker.PersistentEffects.Any(state => state.Definition.type == 6))
                return "PENDING-ORIGINAL: health-steal immunity tag and immunity matching.";
            return null;
        }

        static float OriginalMitigationMultiplier(BattleUnit attacker, BattleUnit target, bool physical)
        {
            float rating = (float)target.GetStat(physical ? 8 : 9);
            float penetration = (float)attacker.GetStat(physical ? 10 : 11)
                + (float)attacker.GetStat(56) * rating;
            float effective = Math.Max(0f, rating - Math.Max(0f, penetration));
            return 1f - Math.Min(0.9f, effective / (attacker.Level * 7.5f + effective));
        }

        // 0x18184D3F0. Graph caches this per target/applyType for one activation
        // (RollTargetEvasion 0x18184C590); caller owns paralysis/category bypasses.
        public bool OriginalEvaded(BattleUnit attacker, BattleUnit target, int protobufApplyType, string[] flags = null)
        {
            if (protobufApplyType == 1 || attacker.Side == target.Side || !target.Alive) return false;
            if (protobufApplyType != 2 && protobufApplyType != 3)
                throw new NotSupportedException("Unknown protobuf apply type.");
            bool physical = protobufApplyType == 2;
            float rating = (float)target.GetStat(physical ? 12 : 13) - (float)attacker.GetStat(physical ? 20 : 21);
            float additive = (float)target.GetStat(physical ? 26 : 27) - (float)attacker.GetStat(physical ? 38 : 39);
            // All live dodge/deflection level-delta constants are zero.
            float chance = rating / 1200f + additive + 0.02f;
            return _ctx.BattleRandom.NextInt(100) < chance * 100f;
        }

        // 0x181BCAC50; graph handles UNRESISTABLE before calling (0x18136B460).
        public bool OriginalResisted(BattleUnit attacker, BattleUnit target)
        {
            if (attacker.Side == target.Side) return false;
            float level = Math.Max(0f, Math.Min(0.3f, (target.Level - attacker.Level) * 0.05f));
            float chance = Math.Max(0f, (float)target.GetStat(19) - (float)attacker.GetStat(18) + level) + 0.15f;
            return _ctx.BattleRandom.NextInt(100) < chance * 100f;
        }
    }
}
