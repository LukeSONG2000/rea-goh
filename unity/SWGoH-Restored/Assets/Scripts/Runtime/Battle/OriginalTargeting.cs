using System;
using System.Collections.Generic;
using System.Linq;

namespace SwgohRestored.Battle
{
    public partial class BattleSim
    {
        // Wire enum values, recovered from TargetingRuleFactory/ApplyFilter.
        // Unit lists retain battle insertion order until the module's explicit
        // SquadPositionPriority sort (after its per-target chance rolls).
        IEnumerable<BattleUnit> OriginalTargets(RawEffect effect, BattleUnit owner, BattleUnit selected)
        {
            var seen = new HashSet<BattleUnit>();
            foreach (var rule in effect.target)
            {
                IEnumerable<BattleUnit> pool = rule.unitSelect == 4 ? new[] { owner }
                    : rule.unitSelect == 2 ? new[] { selected } : _units;
                foreach (var target in pool)
                {
                    if (target == null || !target.Alive || seen.Contains(target)) continue;
                    if ((rule.battleSide == 2 && target.Side != owner.Side) ||
                        (rule.battleSide == 3 && target.Side == owner.Side)) continue;
                    if ((rule.excludeSelf && target == owner) ||
                        (rule.excludeSelectedTarget && target == selected)) continue;
                    if (!CategoriesMatch(target, rule.category, false) ||
                        !CategoriesMatch(target, rule.requiredCategory, true)) continue;
                    if (!TagsMatch(target, rule.activeEffectTagCriteria, false) ||
                        !TagsMatch(target, rule.requiredActiveEffectTagCriteria, true)) continue;
                    seen.Add(target);
                    yield return target;
                }
            }
        }

        static bool TagsMatch(BattleUnit target, RawTag[] tags, bool required)
        {
            if (tags == null) return true;
            if (tags.Any(t => t.exclude && target.HasEffectTag(t.tag))) return false;
            var positives = tags.Where(t => !t.exclude).ToArray();
            return positives.Length == 0 || (required
                ? positives.All(t => target.HasEffectTag(t.tag))
                : positives.Any(t => target.HasEffectTag(t.tag)));
        }
    }
}
