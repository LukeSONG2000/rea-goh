using System;
using System.Linq;

namespace SwgohRestored.Battle
{
    public sealed class OriginalPassiveReadiness
    {
        public string UnitId, AbilityId, Kind, Reason;
        public bool Enabled, Supported;
    }

    public partial class BattleSim
    {
        bool _originalPassivesInitialized;
        public bool OriginalPassivesInitialized => _originalPassivesInitialized;

        string OriginalPassiveReason(UnitAbility ability) =>
            ability.Kind!="uniqueskill" && ability.Kind!="leaderskill"
                ? "PENDING-ORIGINAL: passive activation kind "+ability.Kind
                : OriginalUnsupportedReason(ability);

        public OriginalPassiveReadiness[] GetOriginalPassiveReadiness() => _units.OrderBy(u=>u.Side)
            .SelectMany(u=>u.Passives.Select(a=> {
                string reason=OriginalPassiveReason(a);
                return new OriginalPassiveReadiness {UnitId=u.UnitId,AbilityId=a.Id,Kind=a.Kind,
                    Enabled=a.Kind!="leaderskill" || u.IsLeader,Reason=reason,Supported=reason==null};
            })).ToArray();

        // Bridge for the restored single-encounter battle. Native ApplyUniqueAbilities
        // 0x181BBF410 uses SelectUnitsForAbility + ApplyEffects, without manual cast,
        // cooldown or PERFORMMOVE. Player and opponent unique phases precede SOE.
        // Validation is complete before the first RNG draw or persistent mutation.
        public bool TryInitializeOriginalPassives(out string reason, bool allowUnsupportedForTraining = false)
        {
            if (_originalPassivesInitialized) { reason=null; return true; }
            if (PendingActor != null || Round != 0) { reason="Passives must initialize before the first turn."; return false; }
            var entries=_units.OrderBy(u=>u.Side).SelectMany(u=>u.Passives
                .Where(a=>a.Kind!="leaderskill" || u.IsLeader).Select(a=>(unit:u,ability:a))).ToArray();
            var accepted=new System.Collections.Generic.List<(BattleUnit unit,UnitAbility ability)>();
            var pending=new System.Collections.Generic.List<(BattleUnit unit,UnitAbility ability,string reason)>();
            foreach(var entry in entries)
            {
                reason=OriginalPassiveReason(entry.ability);
                if(reason==null)reason=ValidateOriginalParticipants(entry.ability.OriginalGraph,
                    entry.ability.OriginalReferences,entry.unit,entry.unit);
                if(reason!=null)
                {
                    if(!allowUnsupportedForTraining){reason=entry.ability.Id+": "+reason;return false;}
                    pending.Add((entry.unit,entry.ability,reason));
                }
                else accepted.Add(entry);
            }
            foreach(var entry in pending)
                Log.Add(new BattleEvent {Actor=entry.unit.BaseId,ActorUnitId=entry.unit.UnitId,
                    Kind="pending_original",AbilityId=entry.ability.Id,
                    Note="Explicit training fixture: passive not activated. "+entry.reason});
            foreach(var entry in accepted)
                ExecuteOriginalAbility(entry.unit,entry.ability,entry.unit,true);
            _originalPassivesInitialized=true;
            foreach(var unit in _units)OriginalConditionEvent(unit,7);
            reason=null;return true;
        }
    }
}
