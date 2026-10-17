using System;
using System.Globalization;
using System.Linq;

namespace SwgohRestored.Battle
{
    public partial class BattleSim
    {
        static float OriginalFlatAmount(RawEffect effect) => (float)(double.Parse(effect.additiveAmountDecimal ?? "0", CultureInfo.InvariantCulture)/10000.0);
        internal static readonly System.Collections.Generic.Dictionary<string,int> OriginalModifierStats =
            new System.Collections.Generic.Dictionary<string,int> {
                {"SPEED",5}, {"ATTACK_DAMAGE",6}, {"ABILITY_POWER",7},
                {"CRITICAL_DAMAGE",16}, {"ATTACK_CRITICAL_PERCENT_ADDITIVE",28},
                {"ABILITY_CRITICAL_PERCENT_ADDITIVE",29}, {"TAUNT",62} };
        static int OriginalModifierStat(RawEffect effect) => OriginalModifierStats[
            (effect.param ?? Array.Empty<string>()).Single(p => OriginalModifierStats.ContainsKey(p))];
        static int OriginalPersistentType(RawEffect effect) => effect.type == 4 ? 1 : effect.type == 9 ? 2 : effect.type == 8 ? 4 : effect.type == 6 ? 6 : int.MaxValue;

        float OriginalDispel(BattleUnit source,BattleUnit host,RawEffect effect)
        {
            if (!DamageFlag(effect,"UNRESISTABLE") && OriginalResisted(source,host))
            { OriginalStatusLog(source,host,effect,"resist");return 0; }
            int maximum=(int)OriginalFlatAmount(effect);
            // ClearEffects 0x181A6D400 snapshots candidates before callbacks.
            // CompareTo 0x180FFB210 sorts runtime type, then stat (0 last).
            var selected=host.PersistentEffects.OrderBy(s=>OriginalPersistentType(s.Definition))
                .ThenBy(s=>s.Definition.type==8?OriginalModifierStat(s.Definition):int.MaxValue)
                .Where(s=>(s.Definition.descriptiveTag??Array.Empty<RawTag>()).Any(tag=>
                    (effect.effectedTag??Array.Empty<RawTag>()).Any(wanted=>wanted.tag==tag.tag)))
                .Take(Math.Max(0,maximum)).ToArray();
            foreach(var state in selected) OriginalRemoveState(host,state,source,true);
            return selected.Length;
        }

        void OriginalRemoveState(BattleUnit host,OriginalPersistentEffect state,BattleUnit source,bool dispelled)
        {
            if(!host.PersistentEffects.Remove(state))return;
            OriginalStatusLog(state.Source,host,state.Definition,"status_remove",state.InstanceId);
            // HandleEffectRemoved: decrement tags before instance callbacks;
            // whole-tag callbacks only when the removed tag's count reaches 0.
            foreach(var tag in state.Definition.descriptiveTag??Array.Empty<RawTag>())
            {
                if(tag.exclude)continue;
                bool last=!host.HasEffectTag(tag.tag);
                if(dispelled)
                {
                    DispatchOriginalConditions(host,85,tag.tag);
                    if(source!=null)DispatchOriginalConditions(source,84,tag.tag);
                }
                DispatchOriginalConditions(host,51,tag.tag);
                if(last && dispelled)
                {
                    DispatchOriginalConditions(host,86,tag.tag);
                    if(source!=null)DispatchOriginalConditions(source,83,tag.tag);
                }
                if(last)DispatchOriginalConditions(host,29,tag.tag);
            }
            // Team-relative tag callback families remain validator-gated.
        }

        float OriginalRecoverSpeed(BattleUnit source,BattleUnit host,RawEffect effect)
        {
            // Recovery.ApplyToTarget 0x181371C10: positive SPEED_FILL is a buff.
            // Recovery modifiers and immunity remain validator-gated.
            if(!OriginalIsBuff(effect) && !DamageFlag(effect,"UNRESISTABLE") && OriginalResisted(source,host))
            { OriginalStatusLog(source,host,effect,"resist");return 0; }
            float maximum=GameConstants.UnitSpeedFillMax;
            float requested=OriginalFlatAmount(effect)+maximum*(effect.multiplierAmountDecimal/10000f);
            float before=(float)host.SpeedFill;
            float after=Math.Max(0,Math.Min(maximum,before+requested));
            host.SpeedFill=after;
            host.SpeedFillOverflow=after>=maximum?(float)host.SpeedFillOverflow+Math.Max(0,requested):after;
            if(DamageFlag(effect,"PRIORITY"))
            {
                _turnPriority.RemoveAll(u=>u==host);
                _turnPriority.Add(host);
            }
            Log.Add(new BattleEvent {Turn=Round,Actor=source.BaseId,ActorUnitId=source.UnitId,
                ActorSide=source.Side==0?"player":"opponent",Target=host.BaseId,TargetUnitId=host.UnitId,
                Kind="turn_meter",EffectId=effect.id,Amount=after-before});
            return after-before;
        }

        float OriginalRecoverHealth(BattleUnit source, BattleUnit host, RawEffect effect)
        {
            // Recovery.ApplyToTarget 0x181371C10; helper 0x18000CB30 is nearest
            // ties-to-even. ApplyHeal 0x181BC6060 declines dead recipients.
            if (!host.Alive) return 0;
            bool shield = DamageFlag(effect,"SHIELD");
            int maximum = (int)(shield ? host.MaxShield : host.MaxHp);
            float amount = OriginalFlatAmount(effect) + maximum * (effect.multiplierAmountDecimal / 10000f);
            int requested = (int)Math.Round(amount, MidpointRounding.ToEven);
            double before = shield ? host.CurrentShield : host.CurrentHp;
            int after = Math.Max(0,Math.Min(maximum,Math.Max(0,Math.Min(maximum,(int)before))+requested));
            if (shield) host.CurrentShield = after; else host.CurrentHp = after;
            double actual = after-before;
            Log.Add(new BattleEvent {Turn=Round,Actor=source.BaseId,ActorUnitId=source.UnitId,
                ActorSide=source.Side==0?"player":"opponent",Target=host.BaseId,TargetUnitId=host.UnitId,
                Kind="heal",EffectId=effect.id,Amount=actual,Heal=shield?0:actual,ShieldHeal=shield?actual:0,
                Note=(shield?"shield":"health")+" recovery requested="+requested+" actual="+actual});
            return requested;
        }
    }
}
