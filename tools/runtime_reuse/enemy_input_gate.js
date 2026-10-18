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
     supported:abilityType===0&&!allyTargetingRuleId};
   });
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
  const targetInfo=targeting();
  return {pending:true,token:pending.token,expiresAt:pending.expiresAt,actor:unitId(pending.unit),
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
   const saved=pending;
   const chosen=abilities().find(x=>x.moveType===moveType&&x.index===index);
   if(!chosen)throw Error('Enemy ability absent from original list');
   if(!chosen.supported)throw Error('Original ability requires ally selection or an unsupported ability type');
   if(!chosen.ready||!chosen.canPerform)throw Error('Original CanPerformMove/readiness rejected ability');
   const abilityId=chosen.id;
   const selectedTarget=unitId(call(R.readRef(saved.unit,'_targetingController'),'GetCurrentTarget'));
   // Same original move and completion delegate used by BattleInputProvider.
   // Target remains the original targeting controller's current target.
   pending=null;
   try{
    const started=R.unbox(call(saved.unit,'PerformManualMove',[A.value('i32',moveType),A.value('i32',index)]),'bool');
    if(!started){pending=saved;throw Error('Original manual move did not start');}
    call(saved.callback,'Invoke',[A.value('i32',moveType)]);
    const actualTarget=unitId(R.readRef(saved.unit,'<EnemyTarget>k__BackingField'));
    events.push({event:moveType===0?'manual-basic':'manual-special',token,abilityId,moveType,index,selectedTarget,actualTarget});
    return {started:true,token,abilityId,moveType,index,selectedTarget,actualTarget};
   }finally{if(pending!==saved)for(const handle of saved.retained)R.release(handle);}
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
