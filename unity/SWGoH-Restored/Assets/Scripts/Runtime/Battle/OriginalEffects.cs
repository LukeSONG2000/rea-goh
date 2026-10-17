using System;
using System.Collections.Generic;
using System.Linq;

namespace SwgohRestored.Battle
{
    public partial class BattleSim
    {
        static bool Any<T>(T[] values) => values != null && values.Length != 0;

        // Capability checks are deliberately separate from eligibility. Unsupported
        // graphs cannot consume a turn, RNG or a subset of their original effects.
        public string OriginalUnsupportedReason(UnitAbility ability)
        {
            if (ability.OriginalGraph == null) return "PENDING-ORIGINAL: missing effect graph";
            var raw = ability.OriginalGraph.Abilities[ability.Id];
            if (raw.cooldownType != 1) return "PENDING-ORIGINAL: cooldown type";
            if (ability.OriginalTier >= 0)
            {
                if (raw.tier == null || ability.OriginalTier >= raw.tier.Length)
                    return "PENDING-ORIGINAL: ability tier";
                var tier = raw.tier[ability.OriginalTier];
                if (!string.IsNullOrEmpty(tier.blockingEffectId) ||
                    (tier.cooldownMaxOverride != 0 && tier.cooldownMaxOverride != ability.Cooldown))
                    return "PENDING-ORIGINAL: tier blocking/cooldown override";
            }
            if (Any(raw.triggerCondition) || !string.IsNullOrEmpty(raw.blockingEffectId))
                return "PENDING-ORIGINAL: ability conditions";
            if (!Any(ability.OriginalReferences)) return "PENDING-ORIGINAL: empty effect chain";
            foreach (var reference in ability.OriginalReferences)
            {
                if (Any(reference.contextIndex) || reference.maxBonusMove > 1)
                    return "PENDING-ORIGINAL: reference context/bonus move";
                if (!ability.OriginalGraph.Effects.TryGetValue(reference.id, out var e))
                    return "PENDING-ORIGINAL: missing effect " + reference.id;
                var reason = ValidateOriginalEffect(e);
                if (reason != null) return e.id + ": " + reason;
            }
            return null;
        }

        static string ValidateOriginalEffect(RawEffect e)
        {
            if (e.type != 1 && e.type != 4) return "PENDING-ORIGINAL: effect type " + e.type;
            if (e.applyType < 1 || e.applyType > 3 || e.chance < 0 || e.chance > 100)
                return "PENDING-ORIGINAL: application values";
            if (Any(e.triggerCondition) || Any(e.effectReference) || Any(e.effectDefinition) ||
                Any(e.summonEffect) || Any(e.infiniteCycleEffect) || Any(e.contextIndex) ||
                Any(e.effectedTag) || e.contextMultiplierDecimal != 0 || e.maxBonusMoves > 1 ||
                e.triggerOnApplication || e.expireOnApplication || e.applyEffectEnabled ||
                !string.IsNullOrEmpty(e.blackboardName) || !string.IsNullOrEmpty(e.blackboardCompareName) ||
                !string.IsNullOrEmpty(e.replaceTableId) || !string.IsNullOrEmpty(e.applyEffectLabel))
                return "PENDING-ORIGINAL: nested/context/trigger semantics";
            if (e.type == 1)
            {
                var reason = ValidateOriginalDamage(e);
                if (reason != null) return reason;
                if (Any(e.expirationCondition)) return "PENDING-ORIGINAL: persistent damage";
            }
            else
            {
                if ((e.param ?? Array.Empty<string>()).Any(p => p != "UNRESISTABLE" && p != "FORCE_DEBUFF"))
                    return "PENDING-ORIGINAL: persistent flags";
                if (e.multiplierAmountDecimal != 0 || e.resultVarianceDecimal != 0 ||
                    (!string.IsNullOrEmpty(e.additiveAmountDecimal) && e.additiveAmountDecimal != "0"))
                    return "PENDING-ORIGINAL: persistent amount";
                foreach (var c in e.expirationCondition ?? Array.Empty<RawCondition>())
                    if (c.conditionType != 49 && c.conditionType != 32 && c.conditionType != 21 && c.conditionType != 24)
                        return "PENDING-ORIGINAL: expiration condition " + c.conditionType;
            }
            if (!Any(e.target)) return "PENDING-ORIGINAL: implicit target";
            foreach (var t in e.target)
            {
                if (t.unitSelect != 2 || t.battleSide != 3 || t.healthState != 2 ||
                    t.battleDeploymentState != 2 || Any(t.unitClass) || Any(t.forceAlignment) ||
                    Any(t.statValue) || Any(t.activeEffectTagCriteria) || Any(t.requiredActiveEffectTagCriteria) ||
                    !string.IsNullOrEmpty(t.id)) return "PENDING-ORIGINAL: target rule";
                foreach (var c in new[] { t.category, t.requiredCategory })
                    if (c != null && (c.exclude || Any(c.categoryId)))
                        return "PENDING-ORIGINAL: obsolete category fields";
            }
            return null;
        }

        // TargetingRule.CanBeApplied 0x180FF8C20: positive category OR,
        // excluded categories none; required positives AND. Not AND for both.
        static bool CategoriesMatch(BattleUnit target, RawCategory category, bool required)
        {
            if (category?.category == null) return true;
            var positives = category.category.Where(c => !c.exclude).ToArray();
            if (category.category.Any(c => c.exclude && target.Categories.Contains(c.categoryId))) return false;
            return positives.Length == 0 || (required
                ? positives.All(c => target.Categories.Contains(c.categoryId))
                : positives.Any(c => target.Categories.Contains(c.categoryId)));
        }

        static bool OriginalTargetMatches(RawEffect e, BattleUnit actor, BattleUnit target)
        {
            return target.Alive && target.Side != actor.Side && e.target.Any(t =>
                !t.excludeSelectedTarget && (!t.excludeSelf || target != actor) &&
                CategoriesMatch(target, t.category, false) && CategoriesMatch(target, t.requiredCategory, true));
        }

        string OriginalCommandReason(BattleUnit actor, UnitAbility ability, BattleUnit target)
        {
            var reason = OriginalUnsupportedReason(ability);
            if (reason != null) return reason;
            foreach (var reference in ability.OriginalReferences)
            {
                var e = ability.OriginalGraph.Effects[reference.id];
                if (e.type == 1 && OriginalTargetMatches(e, actor, target))
                {
                    reason = ValidateOriginalDamage(actor, target, e);
                    if (reason != null) return reason;
                }
            }
            return null;
        }

        void ExecuteOriginalAbility(BattleUnit actor, UnitAbility ability, BattleUnit target)
        {
            // AbilityController.RollTargetEvasion caches all apply types once per
            // target; separate damage/debuff nodes reuse the result, never reroll.
            var evaded = new bool[4];
            for (int apply = 1; apply <= 3; apply++) evaded[apply] = OriginalEvaded(actor, target, apply);
            foreach (var reference in ability.OriginalReferences)
            {
                var e = ability.OriginalGraph.Effects[reference.id];
                if (!OriginalTargetMatches(e, actor, target) || evaded[e.applyType]) continue;
                // FailedChanceRoll 0x18136A110 consumes even chance 100 and 0.
                if (_ctx.BattleRandom.NextInt(100) >= e.chance) continue;
                if (e.type == 1)
                {
                    var damage = OriginalDamage(actor, target, e);
                    damage.AbilityId = ability.Id;
                    damage.AbilityName = ability.Name;
                    damage.EffectId = e.id;
                    if (!target.Alive) OriginalConditionEvent(target, 21);
                }
                else
                {
                    if (!(e.param ?? Array.Empty<string>()).Contains("UNRESISTABLE") && OriginalResisted(actor, target))
                    { OriginalStatusLog(actor, target, e, "resist"); continue; }
                    var state = new OriginalPersistentEffect { Definition = e, Source = actor };
                    foreach (var condition in e.expirationCondition ?? Array.Empty<RawCondition>())
                        state.Expiration.Add(new OriginalConditionState { Definition = condition });
                    target.PersistentEffects.Add(state);
                    OriginalStatusLog(actor, target, e, "status_add");
                }
            }
            if (ability.Cooldown > 0) actor.CooldownLeft[ability.Id] = ability.Cooldown;
        }

        void OriginalStatusLog(BattleUnit source, BattleUnit target, RawEffect effect, string kind)
        {
            Log.Add(new BattleEvent { Turn = Round, Actor = source.BaseId, ActorUnitId = source.UnitId,
                ActorSide = source.Side == 0 ? "player" : "opponent", Target = target.BaseId,
                TargetUnitId = target.UnitId, Kind = kind, EffectId = effect.id, Note = effect.persistentLocKey });
        }

        // Event IDs here are protobuf IDs. Start uses an internal sentinel; the
        // wire FULLTURNCOMPLETE=49 maps to runtime 54, never runtime enum 49.
        void OriginalConditionEvent(BattleUnit unit, int eventType)
        {
            foreach (var state in unit.PersistentEffects.ToArray())
            {
                bool expired = false;
                foreach (var c in state.Expiration)
                {
                    bool match = c.Definition.conditionType == eventType;
                    if (c.Definition.conditionType == 49)
                    {
                        if (eventType == -1) c.SawOwnTurnStart = true;
                        match = eventType == 32 && c.SawOwnTurnStart;
                        if (match) c.SawOwnTurnStart = false;
                    }
                    if (!match) continue;
                    int threshold;
                    if (!int.TryParse(c.Definition.conditionValue, out threshold)) threshold = 1;
                    if (++c.MatchedCount >= threshold) expired = true;
                }
                if (!expired) continue;
                unit.PersistentEffects.Remove(state);
                OriginalStatusLog(state.Source, unit, state.Definition, "status_remove");
            }
        }
    }
}
