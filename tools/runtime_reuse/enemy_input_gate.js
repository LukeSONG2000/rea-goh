'use strict';
// Owned isolated trials only. Defers the original input-provider callback;
// does not change teams, skills, stats, targets, or decision-tree definitions.
globalThis.installEnemyInputGate=context=>{
 const R=RuntimeApi,S=SceneApi,A=R.arg;
 const method=R.findMethod(R.findClass('BattleInputProvider'),'DoMove',2);
 const address=method.readPointer();
 const original=new NativeFunction(address,'void',['pointer','pointer','pointer','pointer']);
 const events=[],errors=[];let pending=null,serial=0,stopped=false;
 const call=(p,n,args=[])=>R.invoke(R.findMethod(R.classOf(p),n,args.length),p,args);
 const actor=()=>call(context,'get_CurrentUnit');
 const fsm=()=>call(context,'get_BattleFsmController');
 function snapshot(){
  if(!pending)return {pending:false};
  return {pending:true,token:pending.token,actor:S.string(R.readRef(R.readRef(pending.unit,'_unitDO'),'Id')),
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
    pending={provider,unit,callback,info,retained,token:'enemy:'+Process.id+':'+(++serial)};
    defer=true;events.push({event:'held',...snapshot()});
   }
  }catch(e){errors.push(String(e));}
  if(!defer)original(provider,unit,callback,info);
 },'void',['pointer','pointer','pointer','pointer']);
 Interceptor.replace(address,replacement);Interceptor.flush();
 return {snapshot,events,errors,
  resume(token){
   validate();if(token!==pending.token)throw Error('Stale enemy input token');
   const saved=pending;pending=null;
   try{original(saved.provider,saved.unit,saved.callback,saved.info);events.push({event:'resumed-original',token});}
   finally{for(const handle of saved.retained)R.release(handle);}
   return {resumed:true,token};
  },
  chooseBasic(token){
   validate();if(token!==pending.token)throw Error('Stale enemy input token');
   const saved=pending;
   if(!R.unbox(call(saved.unit,'CanPerformMove',[A.value('i32',0),A.value('i32',0),A.value('bool',false)]),'bool'))
    throw Error('Original CanPerformMove rejected basic attack');
   const ability=R.readRef(saved.unit,'_attackAbilityContext');
   const abilityId=S.string(call(R.readRef(ability,'<AbilityDO>k__BackingField'),'get_Id'));
   // Same original move and completion delegate used by BattleInputProvider.
   // Target remains the original targeting controller's current target.
   pending=null;
   try{
    const started=R.unbox(call(saved.unit,'PerformManualMove',[A.value('i32',0),A.value('i32',0)]),'bool');
    if(!started){pending=saved;throw Error('Original manual move did not start');}
    call(saved.callback,'Invoke',[A.value('i32',0)]);
    events.push({event:'manual-basic',token,abilityId});
    return {started:true,token,abilityId};
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
   Interceptor.revert(address);Interceptor.flush();stopped=true;
   if(pending){for(const handle of pending.retained)R.release(handle);pending=null;}
  }
 };
};
