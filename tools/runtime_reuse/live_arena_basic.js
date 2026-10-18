'use strict';
// Explicitly armed from original PVP prebattle for ONE real match. No network,
// result, timer, stats, passive effects or original files are changed.
globalThis.installLiveArenaBasic=()=>{
 const R=RuntimeApi,S=SceneApi,A=R.arg;
 const stateController=R.findBound('StateController');
 const current=()=>R.readRef(stateController,'<CurrentState>k__BackingField');
 const initial=current();
 if(R.fullName(R.classOf(initial))!=='PvpPreBattleState')throw Error('Arm only from original PVP prebattle');
 const stats={armed:true,entered:false,finished:false,enemyBasics:0,playerOriginalInputs:0,fallbacks:0,errors:[],moves:[]};
 let entered=null,context=null,stopped=false,attempted=false;
 const hooks=[];
 const call=(p,n,args=[])=>R.invoke(R.findMethod(R.classOf(p),n,args.length),p,args);
 const address=R.findMethod(R.findClass('BattleInputProvider'),'DoMove',2).readPointer();
 const original=new NativeFunction(address,'void',['pointer','pointer','pointer','pointer']);
 function report(kind){send({kind,...stats});}
 function stop(){if(stopped)return;stopped=true;stats.armed=false;Interceptor.revert(address);for(const h of hooks)h.detach();Interceptor.flush();report('live-arena-basic-stopped');}
 const replacement=new NativeCallback((provider,unit,callback,info)=>{
  let started=false;
  try{
   if(stopped||!entered||!current().equals(entered))return original(provider,unit,callback,info);
   const controller=R.readRef(entered,'_battleController');
   if(controller.isNull())throw Error('Battle controller unavailable');
   const live=R.readRef(controller,'_battleContext');
   if(live.isNull()||!R.readRef(provider,'_battleContext').equals(live))return original(provider,unit,callback,info);
   if(context&&!context.equals(live))throw Error('Arena context changed');
   context=live;
   if(!call(context,'get_CurrentUnit').equals(unit))return original(provider,unit,callback,info);
   const owner=R.unbox(call(unit,'get_Owner'),'i32');
   if(owner!==1){stats.playerOriginalInputs++;return original(provider,unit,callback,info);}
   const ability=R.readRef(unit,'_attackAbilityContext');
   if(ability.isNull()||!R.unbox(call(ability,'get_IsAbilityReady'),'bool')||!R.unbox(call(unit,'CanPerformMove',[A.value('i32',0),A.value('i32',0),A.value('bool',false)]),'bool'))throw Error('Original basic attack unavailable');
   const data=R.readRef(ability,'<AbilityDO>k__BackingField');
   if(R.unbox(call(data,'get_AbilityType'),'u8')!==0||S.string(call(data,'get_AllyTargetingRuleId')))throw Error('Basic requires unsupported selection');
   const abilityId=S.string(call(data,'get_Id'));
   started=!!R.unbox(call(unit,'PerformManualMove',[A.value('i32',0),A.value('i32',0)]),'bool');
   if(!started)throw Error('Original basic did not start');
   call(callback,'Invoke',[A.value('i32',0)]);
   const turn=R.readValue(call(context,'get_BattleFsmController'),'<TurnCount>k__BackingField','i32');
   stats.enemyBasics++;stats.moves.push({turn,abilityId,moveType:0,index:0});
   report('live-arena-basic-move');
  }catch(e){
   stats.errors.push(String(e));
   // Never retry a move that already started. If rejected before dispatch,
   // preserve the original match and record that basic-only verification failed.
   if(!started){stats.fallbacks++;original(provider,unit,callback,info);}
   report('live-arena-basic-error');
  }
 },'void',['pointer','pointer','pointer','pointer']);
 try{
  hooks.push(Interceptor.attach(R.findMethod(R.findClass('BattleGameState'),'SC_Enter',2).readPointer(),{onEnter(args){
   if(stopped||attempted)return;attempted=true;
   try{
    if(R.fullName(R.classOf(args[1]))!=='BattleGameState+TransitionData'||R.readValue(args[1],'<BattleTypeEnum>k__BackingField','u8')!==0||R.readValue(args[1],'<CombatTypeEnum>k__BackingField','u8')!==0)throw Error('Next battle is not original squad PVP');
    entered=args[0];stats.entered=true;report('live-arena-basic-entered');
   }catch(e){stats.errors.push(String(e));stop();}
  }}));
  hooks.push(Interceptor.attach(R.findMethod(R.findClass('BattleGameState'),'SC_Exit',2).readPointer(),{onEnter(args){
   if(entered&&args[0].equals(entered)){stats.finished=true;stop();}
  }}));
  Interceptor.replace(address,replacement);Interceptor.flush();report('live-arena-basic-armed');
 }catch(e){stop();throw e;}
 return {status:()=>stats,stop};
};
