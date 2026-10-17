using System;
using System.Collections.Generic;
using System.Linq;

namespace SwgohRestored.Battle
{
    public partial class BattleSim
    {
        long _nextEffectInstance;
        static bool Any<T>(T[] values) => values != null && values.Length != 0;

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
            // AbilityContext.IsAbilityReady / PlayerManualAbilitySelected do not
            // use AbilityDO.TriggerConditions as manual activation probability.
            if (!string.IsNullOrEmpty(raw.blockingEffectId)) return "PENDING-ORIGINAL: ability blocking";
            if (!Any(ability.OriginalReferences)) return "PENDING-ORIGINAL: empty effect chain";
            return ValidateOriginalReferences(ability.OriginalGraph, ability.OriginalReferences, new HashSet<string>());
        }

        static string ValidateOriginalReferences(OriginalEffectGraph graph, RawEffectReference[] references, HashSet<string> path)
        {
            foreach (var reference in references ?? Array.Empty<RawEffectReference>())
            {
                if (Any(reference.contextIndex) || reference.maxBonusMove > 1)
                    return "PENDING-ORIGINAL: reference context/bonus move";
                if (!graph.Effects.TryGetValue(reference.id, out var e)) return "Missing effect " + reference.id;
                if (!path.Add(e.id)) return "PENDING-ORIGINAL: cyclic effect graph";
                var reason = ValidateOriginalEffect(e);
                if (reason == null) reason = ValidateOriginalReferences(graph, e.effectReference, path);
                path.Remove(e.id);
                if (reason != null) return e.id + ": " + reason;
            }
            return null;
        }

        static string ValidateOriginalEffect(RawEffect e)
        {
            if (e.type != 1 && e.type != 4 && e.type != 8 && e.type != 9)
                return "PENDING-ORIGINAL: effect type " + e.type;
            if (e.applyType < 1 || e.applyType > 3 || e.chance < 0 || e.chance > 100)
                return "PENDING-ORIGINAL: application values";
            if (Any(e.effectDefinition) || Any(e.summonEffect) || Any(e.infiniteCycleEffect) ||
                Any(e.contextIndex) || Any(e.effectedTag) || e.contextMultiplierDecimal != 0 || e.maxBonusMoves > 1 ||
                e.triggerOnApplication || e.expireOnApplication || e.applyEffectEnabled ||
                !string.IsNullOrEmpty(e.blackboardName) || !string.IsNullOrEmpty(e.blackboardCompareName) ||
                !string.IsNullOrEmpty(e.replaceTableId) || !string.IsNullOrEmpty(e.applyEffectLabel))
                return "PENDING-ORIGINAL: context/application semantics";
            if (e.type == 1)
            {
                var reason = ValidateOriginalDamage(e);
                if (reason != null) return reason;
                if (Any(e.expirationCondition) || Any(e.triggerCondition) || Any(e.effectReference))
                    return "PENDING-ORIGINAL: persistent damage module";
            }
            else
            {
                var allowed = e.type == 8 ? new[] { "SPEED", "FORCE_BUFF", "FORCE_DEBUFF", "UNRESISTABLE" }
                    : new[] { "FORCE_BUFF", "FORCE_DEBUFF", "UNRESISTABLE" };
                if ((e.param ?? Array.Empty<string>()).Any(p => !allowed.Contains(p)))
                    return "PENDING-ORIGINAL: persistent flags/stat";
                if (e.type == 8 && !(e.param ?? Array.Empty<string>()).Contains("SPEED"))
                    return "PENDING-ORIGINAL: stat modifier";
                if (e.resultVarianceDecimal != 0 ||
                    (!string.IsNullOrEmpty(e.additiveAmountDecimal) && e.additiveAmountDecimal != "0") ||
                    (e.type != 8 && e.multiplierAmountDecimal != 0))
                    return "PENDING-ORIGINAL: persistent amount";
                if (e.type != 9 && (Any(e.triggerCondition) || Any(e.effectReference)))
                    return "PENDING-ORIGINAL: triggered module type";
                foreach (var c in e.triggerCondition ?? Array.Empty<RawCondition>())
                    if (c.conditionType != 25 && c.conditionType != 28)
                        return "PENDING-ORIGINAL: trigger condition " + c.conditionType;
                foreach (var c in e.expirationCondition ?? Array.Empty<RawCondition>())
                    if (c.conditionType != 49 && c.conditionType != 32 && c.conditionType != 21 &&
                        c.conditionType != 24 && c.conditionType != 25)
                        return "PENDING-ORIGINAL: expiration condition " + c.conditionType;
            }
            if (!Any(e.target)) return "PENDING-ORIGINAL: implicit target";
            foreach (var t in e.target)
            {
                if ((t.unitSelect != 1 && t.unitSelect != 2 && t.unitSelect != 4) ||
                    (t.battleSide != 2 && t.battleSide != 3) || t.healthState != 2 ||
                    t.battleDeploymentState != 2 || Any(t.unitClass) || Any(t.forceAlignment) ||
                    Any(t.statValue) || !string.IsNullOrEmpty(t.id)) return "PENDING-ORIGINAL: target rule";
                foreach (var c in new[] { t.category, t.requiredCategory })
                    if (c != null && (c.exclude || Any(c.categoryId))) return "PENDING-ORIGINAL: obsolete category fields";
            }
            return null;
        }

        static bool CategoriesMatch(BattleUnit target, RawCategory category, bool required)
        {
            if (category?.category == null) return true;
            var positives = category.category.Where(c => !c.exclude).ToArray();
            if (category.category.Any(c => c.exclude && target.Categories.Contains(c.categoryId))) return false;
            return positives.Length == 0 || (required ? positives.All(c => target.Categories.Contains(c.categoryId))
                : positives.Any(c => target.Categories.Contains(c.categoryId)));
        }

        string OriginalCommandReason(BattleUnit actor, UnitAbility ability, BattleUnit selected)
        {
            var reason = OriginalUnsupportedReason(ability);
            return reason ?? ValidateOriginalParticipants(ability.OriginalGraph, ability.OriginalReferences, actor, selected);
        }

        string ValidateOriginalParticipants(OriginalEffectGraph graph, RawEffectReference[] references, BattleUnit owner, BattleUnit selected)
        {
            foreach (var reference in references ?? Array.Empty<RawEffectReference>())
            {
                var e = graph.Effects[reference.id];
                foreach (var target in OriginalTargets(e, owner, selected))
                {
                    string reason = e.type == 1 ? ValidateOriginalDamage(owner, target, e)
                        : ValidateOriginalParticipants(graph, e.effectReference, target, target);
                    if (reason != null) return reason;
                }
            }
            return null;
        }

        void ExecuteOriginalAbility(BattleUnit actor, UnitAbility ability, BattleUnit selected)
        {
            var affected = new List<BattleUnit>();
            foreach (var reference in ability.OriginalReferences)
                foreach (var target in OriginalTargets(ability.OriginalGraph.Effects[reference.id], actor, selected))
                    if (!affected.Contains(target)) affected.Add(target);
            var evasion = new Dictionary<BattleUnit, bool[]>();
            foreach (var target in affected)
            {
                var values = new bool[4];
                for (int apply = 1; apply <= 3; apply++) values[apply] = OriginalEvaded(actor, target, apply);
                evasion[target] = values;
            }
            ExecuteOriginalReferences(ability.OriginalGraph, ability.OriginalReferences, actor, selected,
                ability.Id, ability.Name, evasion);
            if (ability.Cooldown > 0) actor.CooldownLeft[ability.Id] = ability.Cooldown;
        }

        void ExecuteOriginalReferences(OriginalEffectGraph graph, RawEffectReference[] references, BattleUnit owner,
            BattleUnit selected, string abilityId, string abilityName, Dictionary<BattleUnit, bool[]> evasion = null)
        {
            foreach (var reference in references ?? Array.Empty<RawEffectReference>())
            {
                var effect = graph.Effects[reference.id];
                var targets = OriginalTargets(effect, owner, selected).Where(t => evasion == null ||
                    (evasion.TryGetValue(t, out var values) && !values[effect.applyType])).ToList();
                // Module rolls every valid target's chance BEFORE any target's
                // damage/crit/variance/resistance, then sorts by original priority.
                var succeeded = new HashSet<BattleUnit>();
                foreach (var target in targets)
                    if (_ctx.BattleRandom.NextInt(100) < effect.chance) succeeded.Add(target);
                targets.Sort((a, b) => a.SquadPositionPriority.CompareTo(b.SquadPositionPriority));
                foreach (var target in targets)
                {
                    if (!target.Alive || !succeeded.Contains(target)) continue;
                    if (effect.type == 1)
                    {
                        var damage = OriginalDamage(owner, target, effect);
                        damage.AbilityId = abilityId; damage.AbilityName = abilityName; damage.EffectId = effect.id;
                        if (!target.Alive) OriginalConditionEvent(target, 21);
                    }
                    else
                    {
                        bool isBuff = OriginalIsBuff(effect);
                        if (!isBuff && !(effect.param ?? Array.Empty<string>()).Contains("UNRESISTABLE") && OriginalResisted(owner, target))
                        { OriginalStatusLog(owner, target, effect, "resist"); continue; }
                        var state = new OriginalPersistentEffect { InstanceId = ++_nextEffectInstance,
                            Definition = effect, Source = owner, Host = target, Graph = graph,
                            AbilityId = abilityId, AbilityName = abilityName,
                            IsBuff = isBuff, IsDebuff = OriginalIsDebuff(effect),
                            Amount = effect.type == 8 ? (float)target.GetInitialStat(5) * (effect.multiplierAmountDecimal / 10000f) : 0,
                            StackingLine = string.IsNullOrEmpty(effect.stackingLineOverride) ? "ability-stacking-line"
                                : effect.stackingLineOverride == "distinct-stacking-line" ? "instance:" + _nextEffectInstance
                                : effect.stackingLineOverride };
                        foreach (var c in effect.triggerCondition ?? Array.Empty<RawCondition>())
                            state.Triggers.Add(new OriginalConditionState { Definition = c });
                        foreach (var c in effect.expirationCondition ?? Array.Empty<RawCondition>())
                            state.Expiration.Add(new OriginalConditionState { Definition = c });
                        var newTags = (effect.descriptiveTag ?? Array.Empty<RawTag>()).Where(t => !t.exclude)
                            .Select(t => t.tag).Distinct().Where(t => !target.HasEffectTag(t)).ToArray();
                        target.PersistentEffects.Add(state);
                        OriginalStatusLog(owner, target, effect, "status_add", state.InstanceId);
                        foreach (var tag in newTags) DispatchOriginalConditions(target, 28, tag);
                    }
                }
            }
        }

        void OriginalStatusLog(BattleUnit source, BattleUnit target, RawEffect effect, string kind, long instance = 0)
        {
            Log.Add(new BattleEvent { Turn = Round, Actor = source.BaseId, ActorUnitId = source.UnitId,
                ActorSide = source.Side == 0 ? "player" : "opponent", Target = target.BaseId,
                TargetUnitId = target.UnitId, Kind = kind, EffectId = effect.id, EffectInstanceId = instance,
                EffectType = effect.type, EffectIcon = effect.persistentIcon, Note = effect.persistentLocKey });
        }

        static bool OriginalIsBuff(RawEffect effect)
        {
            var flags = effect.param ?? Array.Empty<string>();
            if (flags.Contains("FORCE_BUFF")) return true;
            if (flags.Contains("FORCE_DEBUFF")) return false;
            // Supported MODIFY_STAT records have zero context/flat. Definition
            // classification precedes target amount calculation (0x180FF5C50).
            return effect.type == 8 && effect.multiplierAmountDecimal > 0;
        }

        static bool OriginalIsDebuff(RawEffect effect)
        {
            var flags = effect.param ?? Array.Empty<string>();
            if (flags.Contains("FORCE_BUFF")) return false;
            if (flags.Contains("FORCE_DEBUFF")) return true;
            return effect.type == 4 || (effect.type == 8 && !OriginalIsBuff(effect));
        }

        void OriginalConditionEvent(BattleUnit unit, int eventType) =>
            DispatchOriginalConditions(unit, eventType == -1 ? 25 : eventType, null);

        static bool MatchOriginalCondition(OriginalConditionState state, int eventType, string tag)
        {
            var definition = state.Definition;
            bool match = definition.conditionType == eventType;
            if (definition.conditionType == 49)
            {
                if (eventType == 25) state.SawOwnTurnStart = true;
                match = eventType == 32 && state.SawOwnTurnStart;
                if (match) state.SawOwnTurnStart = false;
            }
            if (definition.conditionType == 28 && !string.IsNullOrEmpty(definition.conditionValue) &&
                !int.TryParse(definition.conditionValue, out _)) match &= definition.conditionValue == tag;
            if (!match || state.Reported) return false;
            if (!int.TryParse(definition.conditionValue, out int threshold)) threshold = 1;
            if (++state.MatchedCount < threshold) return false;
            state.Reported = true;
            return true;
        }

        void DispatchOriginalConditions(BattleUnit unit, int eventType, string tag)
        {
            // A unit has two shared trackers, trigger registered before expiry.
            // Collect every state's condition before firing the tracker's stable,
            // descending priority queue. Do not interleave per-state expirations.
            var triggered = unit.PersistentEffects.SelectMany(state => state.Triggers.Select(condition =>
                (state, condition))).Where(pair => MatchOriginalCondition(pair.condition, eventType, tag))
                .OrderByDescending(pair => pair.condition.Definition.priority).ToArray();
            foreach (var pair in triggered)
            {
                if (!unit.Alive || !unit.PersistentEffects.Contains(pair.state)) continue;
                var state = pair.state;
                ExecuteOriginalReferences(state.Graph, state.Definition.effectReference, unit, unit,
                    state.AbilityId, state.AbilityName);
                pair.condition.Reported = false; pair.condition.MatchedCount = 0;
            }
            var expired = unit.PersistentEffects.SelectMany(state => state.Expiration.Select(condition =>
                (state, condition))).Where(pair => MatchOriginalCondition(pair.condition, eventType, tag))
                .OrderByDescending(pair => pair.condition.Definition.priority).ToArray();
            foreach (var pair in expired)
            {
                if (!unit.PersistentEffects.Remove(pair.state)) continue;
                OriginalStatusLog(pair.state.Source, unit, pair.state.Definition, "status_remove", pair.state.InstanceId);
            }
        }
    }
}
