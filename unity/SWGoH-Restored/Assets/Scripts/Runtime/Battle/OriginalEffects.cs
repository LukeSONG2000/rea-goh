using System;
using System.Collections.Generic;
using System.Linq;

namespace SwgohRestored.Battle
{
    public partial class BattleSim
    {
        long _nextEffectInstance;
        readonly Dictionary<BattleUnit,BattleUnit> _originalSelectedTargets = new Dictionary<BattleUnit,BattleUnit>();
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
                if (reference.maxBonusMove > 1)
                    return "PENDING-ORIGINAL: reference bonus move";
                if (!graph.Effects.TryGetValue(reference.id, out var e)) return "Missing effect " + reference.id;
                var indices = Any(reference.contextIndex) ? reference.contextIndex : e.contextIndex;
                bool conditional = DamageFlag(e, "CONTEXTUAL_EXECUTE") || DamageFlag(e, "CONTEXTUAL_SKIP");
                if (Any(indices) || conditional || (e.type == 8 && e.contextMultiplierDecimal != 0))
                {
                    if ((!conditional && e.type != 1 && e.type != 8) || (Any(reference.contextIndex) && Any(e.contextIndex) && !reference.contextIndex.SequenceEqual(e.contextIndex)))
                        return "PENDING-ORIGINAL: non-damage/overridden context";
                    int current = Array.IndexOf(references, reference);
                    foreach (int index in Any(indices) ? indices : new[] { current - 1 })
                        if (index < 0 || index >= current || !graph.Effects.TryGetValue(references[index].id, out var producer) || producer.type != 11)
                            return "PENDING-ORIGINAL: context producer must be a preceding query-count slot";
                }
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
            if (e.type != 1 && e.type != 2 && e.type != 4 && e.type != 5 && e.type != 6 && e.type != 8 && e.type != 9 && e.type != 11)
                return "PENDING-ORIGINAL: effect type " + e.type;
            if (e.applyType < 1 || e.applyType > 3 || e.chance < 0 || e.chance > 100)
                return "PENDING-ORIGINAL: application values";
            if (!double.TryParse(e.additiveAmountDecimal ?? "0", System.Globalization.NumberStyles.Float,
                    System.Globalization.CultureInfo.InvariantCulture, out double parsedFlat) ||
                double.IsNaN(parsedFlat) || double.IsInfinity(parsedFlat))
                return "PENDING-ORIGINAL: invalid additive amount";
            if (e.type == 5 && parsedFlat % 10000 != 0)
                return "PENDING-ORIGINAL: fractional dispel-count rounding";
            if (Any(e.effectDefinition) || Any(e.summonEffect) || Any(e.infiniteCycleEffect) ||
                (e.type != 1 && e.type != 8 && Any(e.contextIndex) && !DamageFlag(e, "CONTEXTUAL_EXECUTE") && !DamageFlag(e, "CONTEXTUAL_SKIP")) || (e.type != 11 && e.type != 5 && e.type != 6 && Any(e.effectedTag)) ||
                (e.type != 1 && e.type != 8 && e.contextMultiplierDecimal != 0) || e.maxBonusMoves > 1 ||
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
            else if (e.type == 2 || e.type == 5)
            {
                var allowed = e.type == 2 ? new[] { "SPEED_FILL", "HEALTH", "SHIELD", "PRIORITY", "UNRESISTABLE", "CONTEXTUAL_EXECUTE", "CONTEXTUAL_SKIP" }
                    : new[] { "UNRESISTABLE", "CONTEXTUAL_EXECUTE", "CONTEXTUAL_SKIP" };
                if ((e.param ?? Array.Empty<string>()).Any(p => !allowed.Contains(p)) ||
                    (e.type == 2 && (e.param ?? Array.Empty<string>()).Count(p => p == "SPEED_FILL" || p == "HEALTH" || p == "SHIELD") != 1) ||
                    (e.type == 2 && DamageFlag(e,"PRIORITY") && !DamageFlag(e,"SPEED_FILL")) ||
                    (e.effectedTag ?? Array.Empty<RawTag>()).Any(t => t.exclude) ||
                    Any(e.triggerCondition) || Any(e.expirationCondition) || Any(e.effectReference) ||
                    e.resultVarianceDecimal != 0 || e.multiplierAmountDecimal < 0 ||
                    !double.TryParse(e.additiveAmountDecimal ?? "0", System.Globalization.NumberStyles.Float,
                        System.Globalization.CultureInfo.InvariantCulture, out double flat) || flat < 0 || double.IsInfinity(flat) || double.IsNaN(flat))
                    return "PENDING-ORIGINAL: recovery/dispel options";
            }
            else if (e.type == 11)
            {
                if ((e.param ?? Array.Empty<string>()).Any(p => p != "COUNT_ACTIVE_EFFECTS") ||
                    (e.effectedTag ?? Array.Empty<RawTag>()).Any(t => t.exclude) ||
                    Any(e.expirationCondition) || Any(e.triggerCondition) || Any(e.effectReference) ||
                    e.multiplierAmountDecimal != 0 || e.resultVarianceDecimal != 0 ||
                    (!string.IsNullOrEmpty(e.additiveAmountDecimal) && e.additiveAmountDecimal != "0"))
                    return "PENDING-ORIGINAL: query-count options";
            }
            else
            {
                var allowed = e.type == 8 ? OriginalModifierStats.Keys.Concat(new[] { "FORCE_BUFF", "FORCE_DEBUFF", "UNRESISTABLE" }).ToArray()
                    : new[] { "FORCE_BUFF", "FORCE_DEBUFF", "UNRESISTABLE" };
                if ((e.param ?? Array.Empty<string>()).Any(p => !allowed.Contains(p)))
                    return "PENDING-ORIGINAL: persistent flags/stat";
                if (e.type == 6 && (e.effectedTag ?? Array.Empty<RawTag>()).Any(t => t.exclude))
                    return "PENDING-ORIGINAL: negative immunity tag";
                if (e.type == 8 && (e.param ?? Array.Empty<string>()).Count(p => OriginalModifierStats.ContainsKey(p)) != 1)
                    return "PENDING-ORIGINAL: stat modifier";
                if (e.type == 8 && DamageFlag(e,"TAUNT") &&
                    (e.multiplierAmountDecimal != 0 || e.contextMultiplierDecimal != 0 || parsedFlat % 10000 != 0))
                    return "PENDING-ORIGINAL: fractional/context taunt rank";
                if (e.resultVarianceDecimal != 0 ||
                    (e.type != 8 && !string.IsNullOrEmpty(e.additiveAmountDecimal) && e.additiveAmountDecimal != "0") ||
                    (e.type != 8 && e.multiplierAmountDecimal != 0))
                    return "PENDING-ORIGINAL: persistent amount";
                if (e.type != 9 && (Any(e.triggerCondition) || Any(e.effectReference)))
                    return "PENDING-ORIGINAL: triggered module type";
                foreach (var c in e.triggerCondition ?? Array.Empty<RawCondition>())
                    if (c.conditionType != 25 && c.conditionType != 28 && c.conditionType != 17 && c.conditionType != 21 &&
                        c.conditionType != 7 && c.conditionType != 35 && c.conditionType != 51 && c.conditionType != 74)
                        return "PENDING-ORIGINAL: trigger condition " + c.conditionType;
                foreach (var c in e.expirationCondition ?? Array.Empty<RawCondition>())
                    if (c.conditionType != 49 && c.conditionType != 32 && c.conditionType != 21 &&
                        c.conditionType != 24 && c.conditionType != 25 && c.conditionType != 10 && c.conditionType != 2)
                        return "PENDING-ORIGINAL: expiration condition " + c.conditionType;
            }
            if (!Any(e.target)) return "PENDING-ORIGINAL: implicit target";
            foreach (var t in e.target)
            {
                if ((t.unitSelect != 1 && t.unitSelect != 2 && t.unitSelect != 4) ||
                    (t.battleSide != 2 && t.battleSide != 3) || (t.healthState != 1 && t.healthState != 2) ||
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

        void ExecuteOriginalAbility(BattleUnit actor, UnitAbility ability, BattleUnit selected, bool passive = false)
        {
            if (!passive)
            {
                _originalSelectedTargets[actor] = selected;
                // PerformMove 0x181BC3F20 sets selected targets, reports STARTMOVE,
                // then SelectUnitsForAbility rolls evasion. No cooldown here yet.
                OriginalConditionEvent(actor,35);
            }
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
                // Observe already sampled cache entries for applicable modules;
                // presentation must not reroll evasion or silently lose targets.
                for(int apply=2;apply<=3;apply++)
                    if(values[apply] && ability.OriginalReferences.Any(r=>
                        ability.OriginalGraph.Effects[r.id].applyType==apply &&
                        OriginalTargets(ability.OriginalGraph.Effects[r.id],actor,selected).Contains(target)))
                        Log.Add(new BattleEvent { Turn=Round,Actor=actor.BaseId,ActorUnitId=actor.UnitId,
                            ActorSide=actor.Side==0?"player":"opponent",Target=target.BaseId,TargetUnitId=target.UnitId,
                            Kind="evade",AbilityId=ability.Id,AbilityName=ability.Name,Note="cached applyType="+apply });
            }
            ExecuteOriginalReferences(ability.OriginalGraph, ability.OriginalReferences, actor, selected,
                ability.Id, ability.Name, evasion);
            // Native AttackFinished 0x181BC4F50 reports PERFORMMOVE after all
            // effect modules, so the temporary crit modifier affects this hit.
            if (!passive)
            {
                OriginalConditionEvent(actor, 10);
                if (ability.Cooldown > 0) actor.CooldownLeft[ability.Id] = ability.Cooldown;
            }
        }

        void ExecuteOriginalReferences(OriginalEffectGraph graph, RawEffectReference[] references, BattleUnit owner,
            BattleUnit selected, string abilityId, string abilityName, Dictionary<BattleUnit, bool[]> evasion = null)
        {
            var context = new OriginalEffectContext();
            foreach (var reference in references ?? Array.Empty<RawEffectReference>())
            {
                var effect = graph.Effects[reference.id];
                var indices = Any(reference.contextIndex) ? reference.contextIndex : effect.contextIndex;
                float contextValue = context.Sum(indices);
                // ApplyToValidTargets 0x181368C10 checks the whole module before
                // targeting/RNG, appending an empty slot on a skipped module.
                // Only query-count inputs are admitted, hence these values are
                // integers and Approximately(value,0) is exactly value == 0.
                if ((DamageFlag(effect, "CONTEXTUAL_SKIP") && contextValue > 0) ||
                    (DamageFlag(effect, "CONTEXTUAL_EXECUTE") && contextValue == 0))
                {
                    context.AddResult(new Dictionary<BattleUnit, float>());
                    continue;
                }
                var targets = OriginalTargets(effect, owner, selected).Where(t => evasion == null ||
                    (evasion.TryGetValue(t, out var values) && !values[effect.applyType])).ToList();
                // Module rolls every valid target's chance BEFORE any target's
                // damage/crit/variance/resistance, then sorts by original priority.
                var succeeded = new HashSet<BattleUnit>();
                foreach (var target in targets)
                    if (_ctx.BattleRandom.NextInt(100) < effect.chance) succeeded.Add(target);
                targets.Sort((a, b) => a.SquadPositionPriority.CompareTo(b.SquadPositionPriority));
                var result = targets.ToDictionary(t => t, t => 0f);
                foreach (var target in targets)
                {
                    if (!succeeded.Contains(target)) continue;
                    if (effect.type == 1)
                    {
                        var damage = OriginalDamage(owner, target, effect, contextValue);
                        damage.AbilityId = abilityId; damage.AbilityName = abilityName; damage.EffectId = effect.id;
                        if (!target.Alive)
                        {
                            OriginalConditionEvent(owner, 17); // native NotifyKilled: killer KILL, not ENEMYKILLED
                            OriginalConditionEvent(target, 21);
                        }
                    }
                    else if (effect.type == 11)
                    {
                        // QueryCount 0x18136F1B0 counts each applied instance once,
                        // irrespective of how many descriptive tags match.
                        result[target] = (effect.param ?? Array.Empty<string>()).Contains("COUNT_ACTIVE_EFFECTS")
                            ? target.PersistentEffects.Count(state => (state.Definition.descriptiveTag ?? Array.Empty<RawTag>())
                                .Any(tag => (effect.effectedTag ?? Array.Empty<RawTag>()).Any(wanted => wanted.tag == tag.tag)))
                            : 1;
                    }
                    else if (effect.type == 5) result[target] = OriginalDispel(owner, target, effect);
                    else if (effect.type == 2) result[target] = DamageFlag(effect,"SPEED_FILL")
                        ? OriginalRecoverSpeed(owner, target, effect) : OriginalRecoverHealth(owner, target, effect);
                    else
                    {
                        bool isBuff = OriginalIsBuff(effect);
                        if (!isBuff && !(effect.param ?? Array.Empty<string>()).Contains("UNRESISTABLE") && OriginalResisted(owner, target))
                        { OriginalStatusLog(owner, target, effect, "resist"); continue; }
                        // Persistent.ApplyToTargets 0x18136CE90 forms the context
                        // delta first. Zero flat/mult/context creates no state.
                        float contextDelta = contextValue * (effect.contextMultiplierDecimal / 10000f);
                        if (effect.type == 8 && effect.multiplierAmountDecimal == 0 &&
                            OriginalFlatAmount(effect) == 0 && contextDelta == 0) continue;
                        var state = new OriginalPersistentEffect { InstanceId = ++_nextEffectInstance,
                            Definition = effect, Source = owner, Host = target, Graph = graph,
                            AbilityId = abilityId, AbilityName = abilityName,
                            IsBuff = isBuff, IsDebuff = OriginalIsDebuff(effect),
                            Amount = effect.type == 8 ? (float)target.GetInitialStat(OriginalModifierStat(effect)) * (effect.multiplierAmountDecimal / 10000f)
                                + OriginalFlatAmount(effect) + contextDelta : 0,
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
                // Only query-count producers are currently admitted by validation.
                // Other modules still occupy their original result slot.
                context.AddResult(result);
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
            // Native runtime IMMUNITY=6 is always a buff (0x180FF5C50).
            if (effect.type == 6) return true;
            if (effect.type == 8 && flags.Contains("TAUNT")) return true;
            if (effect.type == 2 && !flags.Contains("SPEED_FILL")) return true;
            // Definition classification precedes target amount calculation
            // (0x180FF5C50); use the first nonzero multiplier/context/flat field.
            if (effect.type != 8 && effect.type != 2) return false;
            if (effect.multiplierAmountDecimal != 0) return effect.multiplierAmountDecimal > 0;
            if (effect.contextMultiplierDecimal != 0) return effect.contextMultiplierDecimal > 0;
            return OriginalFlatAmount(effect) > 0;
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
            if ((definition.conditionType == 28 || definition.conditionType == 29 || definition.conditionType == 51 ||
                definition.conditionType >= 83 && definition.conditionType <= 86) && !string.IsNullOrEmpty(definition.conditionValue) &&
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
                // A death trigger belongs to the dying host and must run before
                // its death-expiry tracker; its child target rules decide life.
                if (!unit.PersistentEffects.Contains(pair.state)) continue;
                var state = pair.state;
                ExecuteOriginalReferences(state.Graph, state.Definition.effectReference, unit,
                    _originalSelectedTargets.TryGetValue(unit,out var selected) ? selected : unit,
                    state.AbilityId, state.AbilityName);
                pair.condition.Reported = false; pair.condition.MatchedCount = 0;
            }
            var expired = unit.PersistentEffects.SelectMany(state => state.Expiration.Select(condition =>
                (state, condition))).Where(pair => MatchOriginalCondition(pair.condition, eventType, tag))
                .OrderByDescending(pair => pair.condition.Definition.priority).ToArray();
            foreach (var pair in expired)
            {
                OriginalRemoveState(unit, pair.state, null, false);
            }
        }
    }
}
