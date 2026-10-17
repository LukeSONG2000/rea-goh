using System;
using System.Collections.Generic;
using System.Linq;

namespace SwgohRestored.Battle
{
    // Wire values are Protos enums, NOT the numerically different runtime enums.
    // Files retain the full original records. These DTOs expose the fields used
    // by this interpreter; unsupported gameplay fields are rejected, never summed.
    [Serializable] public class RawTag { public string tag; public bool exclude; }
    [Serializable] public class RawCategoryEntry { public string categoryId; public bool exclude; }
    [Serializable] public class RawCategory
    {
        public string[] categoryId;
        public RawCategoryEntry[] category;
        public bool exclude;
    }
    [Serializable] public class RawCondition { public int conditionType, priority; public string conditionValue; }
    [Serializable] public class RawUnknown { }
    [Serializable] public class RawTarget
    {
        public int unitSelect, battleSide, healthState, battleDeploymentState;
        public int[] unitClass, forceAlignment;
        public RawUnknown[] statValue;
        public RawTag[] activeEffectTagCriteria, requiredActiveEffectTagCriteria;
        public RawCategory category, requiredCategory;
        public string id;
        public bool excludeSelf, excludeSelectedTarget;
    }
    [Serializable] public class RawEffectReference
    {
        public string id;
        public int[] contextIndex;
        public int maxBonusMove;
    }
    [Serializable] public class RawEffect
    {
        public string id;
        public int type, chance, applyType, damageType;
        public string[] param;
        public RawTarget[] target;
        public RawTag[] descriptiveTag, effectedTag;
        public RawCondition[] triggerCondition, expirationCondition;
        public RawEffectReference[] effectReference;
        public RawEffect[] effectDefinition, summonEffect;
        public string[] infiniteCycleEffect;
        public int[] contextIndex;
        public int multiplierAmountDecimal, contextMultiplierDecimal, resultVarianceDecimal;
        public string additiveAmountDecimal;
        public int maxBonusMoves;
        public bool triggerOnApplication, expireOnApplication, applyEffectEnabled;
        public string blackboardName, blackboardCompareName, replaceTableId, applyEffectLabel;
        public string persistentIcon, persistentLocKey, persistentIconOverlay, stackingLineOverride;
        public int persistentIconPriority;
        public bool persistentIconGroupable;
    }
    [Serializable] public class RawAbilityTier
    {
        public RawEffectReference[] effectReference;
        public string blockingEffectId;
        public int cooldownMaxOverride;
    }
    [Serializable] public class RawAbility
    {
        public string id, blockingEffectId;
        public int abilityType, cooldown, cooldownType;
        public RawCondition[] triggerCondition;
        public RawEffectReference[] effectReference;
        public RawAbilityTier[] tier;
    }
    [Serializable] public class RawEffectManifest
    {
        public int schemaVersion;
        public string[] effectFiles;
        public RawAbility[] abilities;
    }
    [Serializable] public class RawEffectShard { public RawEffect[] effects; }
    [Serializable] public class OriginalAbilityMenu
    {
        public string id, skill_id, name, kind;
        public int cooldown, abilityType, selectedTier;
        public RawEffectReference[] effectReference;
    }
    [Serializable] public class OriginalUnitMenu
    {
        public string def_id;
        public string[] categoryId;
        public OriginalAbilityMenu[] abilities;
    }

    public sealed class OriginalEffectGraph
    {
        public readonly Dictionary<string, RawEffect> Effects;
        public readonly Dictionary<string, RawAbility> Abilities;
        public OriginalEffectGraph(IEnumerable<RawEffect> effects, IEnumerable<RawAbility> abilities)
        {
            Effects = effects.ToDictionary(e => e.id, StringComparer.Ordinal);
            Abilities = abilities.ToDictionary(a => a.id, StringComparer.Ordinal);
        }
        public UnitAbility Bind(OriginalAbilityMenu menu)
        {
            if (!Abilities.ContainsKey(menu.id)) throw new InvalidOperationException("Missing original ability " + menu.id);
            return new UnitAbility { Id = menu.id, SkillId = menu.skill_id, Name = menu.name,
                Kind = menu.kind, Cooldown = menu.cooldown, OriginalGraph = this,
                OriginalReferences = menu.effectReference, OriginalTier = menu.selectedTier };
        }
    }

    public sealed class OriginalPersistentEffect
    {
        public RawEffect Definition;
        public BattleUnit Source;
        public readonly List<OriginalConditionState> Expiration = new();
    }
    public sealed class OriginalConditionState
    {
        public RawCondition Definition;
        public int MatchedCount;
        public bool SawOwnTurnStart;
    }
}
