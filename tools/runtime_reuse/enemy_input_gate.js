'use strict';
// Owned isolated trials only. Defers the original input-provider callback;
// does not change teams, skills, stats, or decision-tree definitions.
globalThis.installEnemyInputGate=context=>{
 const R=RuntimeApi,S=SceneApi,A=R.arg;
 const method=R.findMethod(R.findClass('BattleInputProvider'),'DoMove',2);
 const address=method.readPointer();
 const original=new NativeFunction(address,'void',['pointer','pointer','pointer','pointer']);
 const events=[],errors=[];let pending=null,serial=0,stopped=false,watchdogDeferrals=0;
 const call=(p,n,args=[])=>R.invoke(R.findMethod(R.classOf(p),n,args.length),p,args);
 const actor=()=>call(context,'get_CurrentUnit');
 const fsm=()=>call(context,'get_BattleFsmController');
 const unitId=p=>p.isNull()?null:S.string(R.readRef(R.readRef(p,'_unitDO'),'Id'));
 function abilities(){
  const list=R.readRef(pending.unit,'_limitBreakAbilityContexts');
  const n=R.readValue(list,'_size','i32'),items=S.array(R.readRef(list,'_items'));
  if(n<0||n>64||n>items.length)throw Error('Original special ability list bounds');
  return [{ability:R.readRef(pending.unit,'_attackAbilityContext'),moveType:0,index:0},
   ...items.slice(0,n).map((ability,index)=>({ability,moveType:1,index}))].filter(x=>!x.ability.isNull()).map(x=>{
    const data=R.readRef(x.ability,'<AbilityDO>k__BackingField');
    const allyTargetingRuleId=S.string(call(data,'get_AllyTargetingRuleId'));
    const abilityType=R.unbox(call(data,'get_AbilityType'),'u8');
    return {...x,id:S.string(call(data,'get_Id')),allyTargetingRuleId,abilityType,
     cooldown:R.readValue(x.ability,'<CooldownCounter>k__BackingField','i32'),
     ready:!!R.unbox(call(x.ability,'get_IsAbilityReady'),'bool'),
     canPerform:!!R.unbox(call(pending.unit,'CanPerformMove',[A.value('i32',x.moveType),A.value('i32',x.index),A.value('bool',false)]),'bool'),
     requiresAllySelection:!!allyTargetingRuleId,
     supported:abilityType===0||(abilityType===3&&!!allyTargetingRuleId)};
   });
 }
 function selection(){
  if(!pending?.choice)return {selectingUnit:false,selectionCandidates:[]};
  const controller=R.readRef(context,'<BattleManualUnitSelectController>k__BackingField');
  const active=!!R.unbox(call(controller,'get_CurrentlyPerformingUnitSelection'),'bool');
  if(!active)return {controller,selectingUnit:false,selectionCandidates:[]};
  if(pending.choice.callback&&!R.readRef(controller,'_completed').equals(pending.choice.callback))throw Error('Enemy selection ownership changed');
  const list=R.readRef(controller,'_units');
  const n=R.readValue(list,'_size','i32'),items=S.array(R.readRef(list,'_items'));
  if(n<0||n>64||n>items.length)throw Error('Original ally selection list bounds');
  return {controller,selectingUnit:true,selectionCandidates:items.slice(0,n).map(p=>({id:unitId(p),baseId:S.string(R.readRef(R.readRef(R.readRef(p,'_unitDO'),'<Definition>k__BackingField'),'BaseId'))}))};
 }
 function clearSelection(){
  if(!pending.choice)return;
  const live=selection();
  if(live.selectingUnit){
   call(live.controller,'CancelSelectUnit');
   if(R.unbox(call(live.controller,'get_CurrentlyPerformingUnitSelection'),'bool'))throw Error('Original ally cancellation not confirmed');
  }
  pending.choice=null;
 }
 function freshToken(){pending.token='enemy:'+Process.id+':'+(++serial);}
 function dispatch(chosen){
  const saved=pending,{token}=saved,{moveType,index,id:abilityId}=chosen;
  const selectedTarget=unitId(call(R.readRef(saved.unit,'_targetingController'),'GetCurrentTarget'));
  pending=null;
  try{
   const started=R.unbox(call(saved.unit,'PerformManualMove',[A.value('i32',moveType),A.value('i32',index)]),'bool');
   if(!started){pending=saved;throw Error('Original manual move did not start');}
   call(saved.callback,'Invoke',[A.value('i32',moveType)]);
   const actualTarget=unitId(R.readRef(saved.unit,'<EnemyTarget>k__BackingField'));
   const allyTarget=chosen.requiresAllySelection?unitId(R.readRef(saved.unit,'<AllyTarget>k__BackingField')):null;
   const result={started:true,token,abilityId,moveType,index,selectedTarget,actualTarget,allyTarget};
   events.push({event:moveType===0?'manual-basic':'manual-special',...result});return result;
  }finally{if(pending!==saved)for(const handle of saved.retained)R.release(handle);}
 }
 function targeting(){
  const controller=R.readRef(pending.unit,'_targetingController');
  const list=call(controller,'GetValidTargets',[A.ref(R.nil)]);
  const n=R.readValue(list,'_size','i32'),items=S.array(R.readRef(list,'_items'));
  if(n<0||n>64||n>items.length)throw Error('Original enemy target list bounds');
  return {controller,units:items.slice(0,n),targets:items.slice(0,n).map(p=>({id:unitId(p),baseId:S.string(R.readRef(R.readRef(R.readRef(p,'_unitDO'),'<Definition>k__BackingField'),'BaseId'))})),
   selectedTarget:unitId(call(controller,'GetCurrentTarget')),manualTarget:unitId(call(controller,'get_ManualTarget'))};
 }
 function snapshot(){
  if(!pending)return {pending:false};
  const targetInfo=targeting(),ally=selection();
  return {pending:true,token:pending.token,expiresAt:pending.expiresAt,actor:unitId(pending.unit),
   selectingUnit:ally.selectingUnit,selectionCandidates:ally.selectionCandidates,
   selectedAbility:pending.choice?{moveType:pending.choice.moveType,index:pending.choice.index,id:pending.choice.id}:null,
   abilities:abilities().map(({ability,...info})=>info),
   targets:targetInfo.targets,selectedTarget:targetInfo.selectedTarget,manualTarget:targetInfo.manualTarget,
   owner:R.unbox(call(pending.unit,'get_Owner'),'i32'),
   turn:R.readValue(fsm(),'<TurnCount>k__BackingField','i32'),
   previous:R.readValue(fsm(),'_previousNonIdleState','i32'),next:R.readValue(fsm(),'_nextState','i32')};
 }
 function validate(){
  const probe=globalThis.visualProbe;
  if(stopped||!pending||!probe?.entered||!probe.fixture.context.equals(context))throw Error('No owned enemy input');
  const state=R.readRef(probe.fixture.state,'<CurrentState>k__BackingField');
  if(!state.equals(probe.entered)||!actor().equals(pending.unit))throw Error('Enemy input ownership changed');
 }
 const replacement=new NativeCallback((provider,unit,callback,info)=>{
  let defer=false;
  try{
   if(!stopped&&R.readRef(provider,'_battleContext').equals(context)&&actor().equals(unit)&&
      R.unbox(call(unit,'get_Owner'),'i32')===1){
    if(pending){defer=true;throw Error('Duplicate pending enemy input');}
    const retained=[provider,unit,callback].map(p=>R.retain(p));
    pending={provider,unit,callback,info,retained,expiresAt:Date.now()+10000,token:'enemy:'+Process.id+':'+(++serial)};
    defer=true;events.push({event:'held',...snapshot()});
   }
  }catch(e){errors.push(String(e));}
  if(!defer)original(provider,unit,callback,info);
 },'void',['pointer','pointer','pointer','pointer']);
 const watchdogAddress=R.findMethod(R.findClass('BattleFsmController'),'BattleLockDetection',0).readPointer();
 const originalWatchdog=new NativeFunction(watchdogAddress,'void',['pointer','pointer']);
 const watchdogReplacement=new NativeCallback((controller,info)=>{
  // Exclude deliberate input waiting from the original lock timer. Keep its
  // normal behavior for every other context and while moves are resolving.
  if(!stopped&&pending&&controller.equals(fsm())&&actor().equals(pending.unit)){
   watchdogDeferrals++;return;
  }
  originalWatchdog(controller,info);
 },'void',['pointer','pointer']);
 try{Interceptor.replace(address,replacement);Interceptor.replace(watchdogAddress,watchdogReplacement);Interceptor.flush();}
 catch(e){Interceptor.revert(address);Interceptor.revert(watchdogAddress);throw e;}
 return {snapshot,events,errors,get watchdogDeferrals(){return watchdogDeferrals;},
  selectTarget(request){
   validate();
   if(!request||request.token!==pending.token)throw Error('Stale enemy input token');
   if(Date.now()>=pending.expiresAt)throw Error('Enemy input expired; original AI will resume');
   if(pending.choice)throw Error('Finish or cancel enemy ally selection first');
   const live=targeting();
   if(typeof request.targetId!=='string'||!live.targets.some(t=>t.id===request.targetId))throw Error('Target is not in original enemy legal target list');
   if(live.selectedTarget===request.targetId)
    return {targetConfirmed:true,alreadySelected:true,inputDispatched:false,targetId:request.targetId};
   // The player UI selection route failed on the tested enemy module. Use the
   // original AI target resolver with a legal preferred target instead.
   const preferred=live.units.find(p=>unitId(p)===request.targetId);
   call(live.controller,'CalculateAndSetCurrentTarget',[A.ref(R.nil),A.ref(preferred)]);
   const selectedTarget=unitId(call(live.controller,'GetCurrentTarget'));
   const manualTarget=unitId(call(live.controller,'get_ManualTarget'));
   events.push({event:'target-selected',token:pending.token,requested:request.targetId,selectedTarget,manualTarget});
   return {targetConfirmed:selectedTarget===request.targetId,inputDispatched:true,selectedTarget,manualTarget};
  },
  resume(token){
   validate();if(token!==pending.token)throw Error('Stale enemy input token');
   clearSelection();
   const saved=pending;pending=null;
   try{original(saved.provider,saved.unit,saved.callback,saved.info);events.push({event:'resumed-original',token});}
   finally{for(const handle of saved.retained)R.release(handle);}
   return {resumed:true,token};
  },
  chooseBasic(token){return this.chooseAbility({token,moveType:0,index:0});},
  chooseAbility(request){
   const {token,moveType,index}=request||{};
   validate();if(token!==pending.token)throw Error('Stale enemy input token');
   if(Date.now()>=pending.expiresAt)throw Error('Enemy input expired; original AI will resume');
   if(![0,1].includes(moveType)||!Number.isInteger(index)||index<0||index>63||(moveType===0&&index!==0))throw Error('Invalid enemy ability selector');
   if(pending.choice)throw Error('Finish or cancel enemy ally selection first');
   const chosen=abilities().find(x=>x.moveType===moveType&&x.index===index);
   if(!chosen)throw Error('Enemy ability absent from original list');
   if(!chosen.supported)throw Error('Original ability requires ally selection or an unsupported ability type');
   if(!chosen.ready||!chosen.canPerform)throw Error('Original CanPerformMove/readiness rejected ability');
   if(chosen.requiresAllySelection){
    const controller=R.readRef(context,'<BattleManualUnitSelectController>k__BackingField');
    if(R.unbox(call(controller,'get_CurrentlyPerformingUnitSelection'),'bool'))throw Error('Another original unit selection is active');
    pending.choice={moveType,index,id:chosen.id};
    // Native closure writes AllyTarget. A null continuation leaves input held
    // until enemy-unit explicitly confirms and dispatches the move.
    freshToken();
    call(fsm(),'SelectAlly',[A.ref(chosen.ability),A.ref(R.nil)]);
    const active=!!R.unbox(call(controller,'get_CurrentlyPerformingUnitSelection'),'bool');
    if(active)pending.choice.callback=R.readRef(controller,'_completed');
    const live=selection();
    if(!active)pending.choice=null; // Native empty-list path already completed with null.
    const result={started:false,selectionStarted:active,selectionEmpty:!active,token:pending.token,abilityId:chosen.id,candidates:live.selectionCandidates};
    events.push({event:'ally-selection',...result});return result;
   }
   return dispatch(chosen);
  },
  selectUnit(request){
   validate();if(request?.token!==pending.token)throw Error('Stale enemy input token');
   if(Date.now()>=pending.expiresAt)throw Error('Enemy input expired; original AI will resume');
   if(!pending.choice)throw Error('No pending enemy ally selection');
   const live=selection();
   if(!live.selectingUnit||!live.selectionCandidates.some(t=>t.id===request.targetId))throw Error('Unit is not in original enemy ally candidates');
   const chosen=abilities().find(x=>x.moveType===pending.choice.moveType&&x.index===pending.choice.index&&x.id===pending.choice.id);
   if(!chosen?.ready||!chosen.canPerform)throw Error('Original CanPerformMove/readiness rejected ability');
   call(live.controller,'HandleUnitTapped',[A.ref(R.string(request.targetId))]);
   const actual=unitId(R.readRef(pending.unit,'<AllyTarget>k__BackingField'));
   if(R.unbox(call(live.controller,'get_CurrentlyPerformingUnitSelection'),'bool')||actual!==request.targetId)throw Error('Original ally selection not confirmed');
   events.push({event:'ally-selected',token:pending.token,abilityId:chosen.id,actual});
   pending.choice=null;return dispatch(chosen);
  },
  cancelSelection(token){
   validate();if(token!==pending.token)throw Error('Stale enemy input token');
   if(Date.now()>=pending.expiresAt)throw Error('Enemy input expired; original AI will resume');
   if(!pending.choice)throw Error('No pending enemy ally selection');
   clearSelection();freshToken();
   events.push({event:'ally-selection-cancelled',token:pending.token});
   return {selectionCancelled:true,token:pending.token};
  },
  stop(){
   if(stopped)return;
   // The owner must first exit the private battle; never drop a live callback.
   if(pending){
    const probe=globalThis.visualProbe;
    const state=R.readRef(probe.fixture.state,'<CurrentState>k__BackingField');
    if(state.equals(probe.entered))throw Error('Exit owned battle before discarding pending input');
   }
   Interceptor.revert(address);Interceptor.revert(watchdogAddress);Interceptor.flush();stopped=true;
   if(pending){for(const handle of pending.retained)R.release(handle);pending=null;}
  }
 };
};
