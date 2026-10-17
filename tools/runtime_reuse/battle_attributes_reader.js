'use strict';
// Main-thread, read-only original getters. No hooks, setters or gameplay objects.
// Metadata: il2cpp/dump/cs/il2cpp.cs:72012,72408,72916,503471,595223,602463.
// GetStatValue body: logs/effect_phase2/unit_stat_value.c (includes stat limits).
(() => {
 const R=globalThis.RuntimeApi,S=globalThis.SceneApi;
 const names={1:'MAX_HEALTH',2:'STRENGTH',3:'AGILITY',4:'INTELLIGENCE',5:'SPEED',6:'ATTACK_DAMAGE',7:'ABILITY_POWER',8:'ARMOR',9:'SUPPRESSION',10:'ARMOR_PENETRATION',11:'SUPPRESSION_PENETRATION',12:'DODGE_RATING',13:'DEFLECTION_RATING',14:'ATTACK_CRITICAL_RATING',15:'ABILITY_CRITICAL_RATING',16:'CRITICAL_DAMAGE',18:'ACCURACY',19:'RESISTANCE',24:'HEALTH_STEAL',25:'MAX_SHIELD',28:'ATTACK_CRITICAL_PERCENT_ADDITIVE',29:'ABILITY_CRITICAL_PERCENT_ADDITIVE',61:'MASTERY'};
 const defaultStats=Object.keys(names).map(Number);
 const delta=(current,initial)=>Number.isFinite(current)&&Number.isFinite(initial)?current-initial:null;
 function readBattleAttributes(unit,options={}){
  if(!unit||unit.isNull())throw Error('Original battle unit required');
  const ids=options.statIds===undefined?defaultStats:options.statIds;
  if(!Array.isArray(ids)||ids.some(id=>!Number.isInteger(id)||id<1||id>62))throw Error('UnitStatEnum ids must be integers 1..62');
  const warnings=[];
  function read(label,fn){try {const value=fn();if(typeof value==='number'&&!Number.isFinite(value))throw Error('nonfinite original result');return value;}catch(e){warnings.push(label+': '+String(e.message||e));return null;}}
  function number(type,method,obj,args=[],types=[],kind='f32'){
   return R.unbox(S.invoke(type,method,obj,args,types),kind);
  }
  const scalar=name=>read(name,()=>number('BattleUnitController','get_'+name,unit,[],[],'i32'));
  const result={health:{current:scalar('CurrentHP'),max:scalar('MaxHP')},
   protection:{current:scalar('CurrentShield'),max:scalar('MaxShield'),
    permanent:{current:scalar('CurrentPermanentShield'),max:scalar('MaxPermanentShield')},
    bonus:{current:scalar('CurrentBonusShield'),max:scalar('MaxBonusShield')}},stats:[],warnings};
   const calculator=read('StatCalculator',()=>R.readRef(unit,'_statCalculator'));
  for(const id of [...new Set([...ids,61])]){
   const args=[R.arg.value('u8',id)],types=['UnitStatEnum'];
   const initial=read('initial '+id,()=>number('BattleUnitController','GetInitialUnitCalculatedStat',unit,args,types));
   const current=read('current '+id,()=>number('BattleUnitController','GetStatValue',unit,args,types));
   // Do not invoke UnitDO.GetCalculatedStatValue: even displayOnly=true writes
   // equipped datacron context without restoring it (native 1011440 -> 10114C0).
   const dtoDisplayOnly=null;
   const hasMasteryContribution=read('mastery flag '+id,()=>number('BattleUnitController','HasMasteryContribution',unit,args,types,'bool')!==0);
   // Invoke native calculation; do not derive contribution from mastery rating.
   // FDFF40 reads primary-stat/role table coefficient, not the applied contribution.
   // It may lazily cache the GameData controller; it does not change battle stats.
   const masteryCoefficient=(!calculator||calculator.isNull()||hasMasteryContribution!==true)?null:
    read('mastery contribution '+id,()=>number('StatCalculator','GetMasteryContribution',calculator,
     [...args,R.arg.ref(unit)],['UnitStatEnum','IStatSource']));
   result.stats.push({id,name:names[id]||'UnitStatEnum_'+id,initial,current,netBattleDelta:delta(current,initial),dtoDisplayOnly,
    hasMasteryContribution,masteryCoefficient,masteryContributionDelta:null});
  }
  const mastery=result.stats.find(s=>s.id===61);
  result.mastery={initial:mastery.initial,current:mastery.current,netBattleDelta:mastery.netBattleDelta,
   attributedStatChanges:null,reason:'No original baseline mastery-contribution snapshot; total stat delta is not mastery-only delta.'};
  result.contract={bonusShield:'Original CurrentBonusShield/MaxBonusShield getters; not inferred from total minus permanent.',
   dtoDisplayOnly:'Unavailable: original display getter mutates datacron context and is deliberately not invoked.',
   netBattleDelta:'Difference of original current and initial readings; includes all changes, not additive-effect sum.',
   masteryCoefficient:'Native FDFF40 primary-stat/role lookup coefficient; not current contribution or attributed change.',
   units:'Raw original stat units. Ratings are not converted to percentages.'};
  return result;
 }
 globalThis.readBattleAttributes=readBattleAttributes;
 globalThis.BattleAttributesReader={readBattleAttributes,delta,defaultStats,names};
})();
