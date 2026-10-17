'use strict';
// Original UI input, on the Unity main thread. No simulator rules or stat writes.
globalThis.BattleInputBridge=(()=>{
 const R=RuntimeApi,S=SceneApi,A=R.arg;
 let consumed=null,signature=null,serial=0,token=null;
 const epoch=Process.id+':'+Date.now();
 const unitId=unit=>unit.isNull()?null:S.string(R.readRef(R.readRef(unit,'_unitDO'),'Id'));
 function unitList(list){
  if(list.isNull())return [];
  const items=R.readRef(list,'_items'),n=R.readValue(list,'_size','i32');
  if(n<0||n>64||items.isNull()||n>items.add(24).readU64().toNumber())throw Error('Invalid original target list');
  return Array.from({length:n},(_,i)=>items.add(32+i*8).readPointer());
 }
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
    ready:!!R.unbox(S.invoke('AbilityContext','get_IsAbilityReady',ability),'bool'),
    abilityType:R.unbox(S.invoke('AbilityDO','get_AbilityType',data),'u8'),
    buttonState:R.readValue(button,'<CurrentState>k__BackingField','i32'),
    cooldown:R.readValue(ability,'<CooldownCounter>k__BackingField','i32')});
  }
  const selection=R.readRef(context,'<BattleManualUnitSelectController>k__BackingField');
  const selectingUnit=!selection.isNull()&&!!R.unbox(S.invoke('BattleManualUnitSelectController','get_CurrentlyPerformingUnitSelection',selection),'bool');
  const selectionCandidates=selectingUnit?unitList(R.readRef(selection,'_units')).map(unit=>({id:unitId(unit),baseId:S.string(R.readRef(R.readRef(R.readRef(unit,'_unitDO'),'<Definition>k__BackingField'),'BaseId'))})):[];
  const awaitingAbility=previous===34&&next===1&&visible&&owner===0&&!selectingUnit;
  const targeting=actor.isNull()?R.nil:R.readRef(actor,'_targetingController');
  let targets=[],selectedTarget=null,manualTarget=null;
  if(awaitingAbility&&!targeting.isNull()){
   targets=unitList(S.invoke('BattleTargetingController','GetValidTargets',targeting,[A.ref(R.nil)])).map(unit=>({id:unitId(unit),baseId:S.string(R.readRef(R.readRef(R.readRef(unit,'_unitDO'),'<Definition>k__BackingField'),'BaseId'))}));
   selectedTarget=unitId(S.invoke('BattleTargetingController','GetCurrentTarget',targeting));
   manualTarget=unitId(S.invoke('BattleTargetingController','get_ManualTarget',targeting));
  }
  const currentSignature=JSON.stringify([context.toString(),actor.toString(),turn,previous,next,visible,owner,abilities,targets,selectedTarget,manualTarget,selectingUnit,selectionCandidates]);
  if(currentSignature!==signature){signature=currentSignature;token=epoch+':'+(++serial);}
  return {table,list,targeting,selection,public:{token,turn,previous,next,actor:actor.toString(),abilities,targets,selectedTarget,manualTarget,selectingUnit,selectionCandidates,
   awaitingAbility,scope:'original primary ability table, enemy targets and pending unit selection; ultimate UI pending'}};
 }
 function choose(request){
  if(!request||typeof request.token!=='string'||!Number.isInteger(request.index))throw Error('Expected token and integer index');
  const live=inspect(),info=live.public;
  if(request.token!==info.token)throw Error('Stale battle input snapshot');
  if(consumed===request.token)throw Error('This input token was already dispatched; inspect state before retrying');
  if(!info.awaitingAbility)throw Error('Original FSM is not awaiting ability input');
  const ability=info.abilities.find(a=>a.index===request.index);
  if(!ability||!ability.active||!ability.ready||![0,1].includes(ability.buttonState)||[1,4].includes(ability.abilityType))throw Error('Original ability button unavailable');
  consumed=request.token;
  S.invoke('BattleAbilityTableComponent','HandleAbilityButtonClicked',live.table,[A.value('i32',ability.index),A.ref(live.list)]);
  return {inputDispatched:true,abilityId:ability.id,index:ability.index,turn:info.turn,
   invoked:'BattleAbilityTableComponent.HandleAbilityButtonClicked',resolution:'Original callback invoked; outcome requires subsequent observation'};
 }
 function selectTarget(request){
  if(!request||typeof request.token!=='string'||typeof request.targetId!=='string')throw Error('Expected token and targetId');
  const live=inspect(),info=live.public;
  if(request.token!==info.token)throw Error('Stale battle input snapshot');
  if(!info.awaitingAbility||consumed===request.token)throw Error('Original battle is not accepting target input');
  if(!info.targets.some(t=>t.id===request.targetId))throw Error('Target is not in original legal target list');
  // Native SelectManualTarget toggles an already locked target off.
  if(info.manualTarget===request.targetId&&info.selectedTarget===request.targetId)return {targetConfirmed:true,inputDispatched:false,alreadySelected:true,targetId:request.targetId};
  S.invoke('BattleTargetingController','SelectManualTarget',live.targeting,[A.ref(R.string(request.targetId))]);
  const actual=unitId(S.invoke('BattleTargetingController','GetCurrentTarget',live.targeting));
  const manual=unitId(S.invoke('BattleTargetingController','get_ManualTarget',live.targeting));
  return {inputDispatched:true,targetConfirmed:actual===request.targetId&&manual===request.targetId,targetId:request.targetId,actualTarget:actual,manualTarget:manual,invoked:'BattleTargetingController.SelectManualTarget'};
 }
 function selectUnit(request){
  if(!request||typeof request.token!=='string'||typeof request.targetId!=='string')throw Error('Expected token and targetId');
  const live=inspect(),info=live.public;
  if(request.token!==info.token)throw Error('Stale battle input snapshot');
  if(!info.selectingUnit||consumed===request.token)throw Error('No pending original unit selection');
  // Invalid native IDs finish the selection with null. Never forward them.
  if(!info.selectionCandidates.some(t=>t.id===request.targetId))throw Error('Unit is not in original selection candidates');
  consumed=request.token;
  S.invoke('BattleManualUnitSelectController','HandleUnitTapped',live.selection,[A.ref(R.string(request.targetId))]);
  return {inputDispatched:true,selectionCompleted:!R.unbox(S.invoke('BattleManualUnitSelectController','get_CurrentlyPerformingUnitSelection',live.selection),'bool'),targetId:request.targetId,invoked:'BattleManualUnitSelectController.HandleUnitTapped'};
 }
 function cancelSelection(request){
  if(!request||typeof request.token!=='string')throw Error('Expected token');
  const live=inspect(),info=live.public;
  if(request.token!==info.token)throw Error('Stale battle input snapshot');
  if(!info.selectingUnit||consumed===request.token)throw Error('No pending original unit selection');
  consumed=request.token;
  S.invoke('BattleManualUnitSelectController','CancelSelectUnit',live.selection);
  return {inputDispatched:true,selectionCancelled:!R.unbox(S.invoke('BattleManualUnitSelectController','get_CurrentlyPerformingUnitSelection',live.selection),'bool'),invoked:'BattleManualUnitSelectController.CancelSelectUnit'};
 }
 return {snapshot:()=>inspect().public,choose,selectTarget,selectUnit,cancelSelection};
})();
