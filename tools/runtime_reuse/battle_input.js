'use strict';
// Original UI input, on the Unity main thread. No simulator rules or stat writes.
globalThis.BattleInputBridge=(()=>{
 const R=RuntimeApi,S=SceneApi,A=R.arg;
 let consumed=null;
 function inspect(){
  const state=R.readRef(R.findBound('StateController'),'<CurrentState>k__BackingField');
  if(R.fullName(R.classOf(state))!=='BattleGameState')throw Error('Battle input requires BattleGameState');
  const context=R.readRef(R.readRef(state,'_battleController'),'_battleContext');
  const fsm=S.invoke('BattleContext','get_BattleFsmController',context);
  const actor=S.invoke('BattleContext','get_CurrentUnit',context);
  const previous=R.readValue(fsm,'_previousNonIdleState','i32'),next=R.readValue(fsm,'_nextState','i32');
  const turn=R.readValue(fsm,'<TurnCount>k__BackingField','i32');
  const view=S.invoke('BattleContext','get_BattleView',context);
  const table=R.readRef(view,'_playerAbilitiesComponent');
  const visible=!!R.readValue(table,'_abilityButtonsVisible','bool');
  const owner=actor.isNull()?null:R.unbox(S.invoke('BattleUnitController','get_Owner',actor),'i32');
  const list=R.readRef(table,'_abilityButtons');
  const items=R.readRef(list,'_items'),count=R.readValue(list,'_size','i32');
  if(count<0||count>64||items.isNull()||count>items.add(24).readU64().toNumber())throw Error('Invalid original button list');
  const abilities=[];
  for(let index=0;index<count;index++){
   const button=items.add(32+index*8).readPointer();
   const render=R.readRef(button,'<RenderData>k__BackingField');
   if(render.isNull())continue;
   const ability=R.readRef(render,'<Ability>k__BackingField');if(ability.isNull())continue;
   const data=R.readRef(ability,'<AbilityDO>k__BackingField');
   const active=!!R.unbox(S.invoke('UnityEngine.GameObject','get_activeInHierarchy',S.gameObject(button)),'bool');
   abilities.push({index,id:S.string(S.invoke('AbilityDO','get_Id',data)),active,
    abilityType:R.unbox(S.invoke('AbilityDO','get_AbilityType',data),'u8'),
    buttonState:R.readValue(button,'<CurrentState>k__BackingField','i32'),
    cooldown:R.readValue(ability,'<CooldownCounter>k__BackingField','i32')});
  }
  const token=JSON.stringify([context.toString(),actor.toString(),turn,previous,next,visible,owner,abilities]);
  return {table,list,public:{token,turn,previous,next,actor:actor.toString(),abilities,
   awaitingAbility:previous===34&&next===1&&visible&&owner===0,scope:'current original primary ability table; ultimate and special unit selection pending'}};
 }
 function choose(request){
  if(!request||typeof request.token!=='string'||!Number.isInteger(request.index))throw Error('Expected token and integer index');
  const live=inspect(),info=live.public;
  if(request.token!==info.token)throw Error('Stale battle input snapshot');
  if(consumed===request.token)throw Error('This input token was already dispatched; inspect state before retrying');
  if(!info.awaitingAbility)throw Error('Original FSM is not awaiting ability input');
  const ability=info.abilities.find(a=>a.index===request.index);
  if(!ability||!ability.active||![0,1].includes(ability.buttonState)||[1,4].includes(ability.abilityType))throw Error('Original ability button unavailable');
  consumed=request.token;
  S.invoke('BattleAbilityTableComponent','HandleAbilityButtonClicked',live.table,[A.value('i32',ability.index),A.ref(live.list)]);
  return {inputDispatched:true,abilityId:ability.id,index:ability.index,turn:info.turn,
   invoked:'BattleAbilityTableComponent.HandleAbilityButtonClicked',resolution:'Original callback invoked; outcome requires subsequent observation'};
 }
 return {snapshot:()=>inspect().public,choose};
})();
