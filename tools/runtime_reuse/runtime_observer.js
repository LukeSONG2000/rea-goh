'use strict';
// Snapshot is read-only; separate explicit RPC exports perform original UI navigation.
globalThis.readRuntimeSnapshot=()=>RuntimeApi.runOnMainThreadOnce(()=>{
 const R=RuntimeApi,S=SceneApi;
 const state=R.readRef(R.findBound('StateController'),'<CurrentState>k__BackingField');
 if(state.isNull())return {schemaVersion:1,state:'Transitioning',units:[],scope:'original state transition in progress; no gameplay input'};
 const stateName=R.fullName(R.classOf(state));
 const result={schemaVersion:1,state:stateName,units:[],scope:'observed current client; no gameplay input'};
 if(stateName==='HomeBaseGameState')result.campaignMenu=readCampaignMenu();
 if(stateName==='MasterInventoryGameState'){
  const controller=R.readRef(state,'_masterInventoryController'),data=R.readRef(controller,'_transitionData');
  result.inventory={type:R.readValue(data,'<Type>k__BackingField','i32'),navigation:R.readValue(data,'<NavType>k__BackingField','i32')};
 }
 if(stateName!=='BattleGameState')return result;
 try{result.input=BattleInputBridge.snapshot();}catch(e){result.input={unavailable:true,error:String(e)};}
 const context=R.readRef(R.readRef(state,'_battleController'),'_battleContext');
 const seen=new Set();
 for(const reticle of S.all('BattleMessageReticle')){
  const unit=S.invoke('BattleMessageReticle','get_TargetUnit',reticle);
  if(unit.isNull()||seen.has(unit.toString())||!R.readRef(unit,'_battleContext').equals(context))continue;
  if(!R.unbox(S.invoke('UnityEngine.GameObject','get_activeInHierarchy',S.gameObject(reticle)),'bool'))continue;
  seen.add(unit.toString());
  result.units.push({sessionKey:unit.toString(),baseId:S.string(R.readRef(R.readRef(R.readRef(unit,'_unitDO'),'<Definition>k__BackingField'),'BaseId')),attributes:readBattleAttributes(unit),status:readBattleStatus(unit)});
 }
 return result;
});
globalThis.inventoryHome=()=>RuntimeApi.runOnMainThreadOnce(()=>{
 const R=RuntimeApi,S=SceneApi,state=R.readRef(R.findBound('StateController'),'<CurrentState>k__BackingField');
 if(R.fullName(R.classOf(state))!=='MasterInventoryGameState')throw Error('inventory-home requires the inventory screen');
 const views=S.all('MasterInventoryView').filter(v=>R.unbox(S.invoke('UnityEngine.GameObject','get_activeInHierarchy',S.gameObject(v)),'bool'));
 if(views.length!==1)throw Error('Inventory view must resolve uniquely');
 S.invoke('MasterInventoryView','OnHomeButtonClick',views[0]);
 return {invoked:'MasterInventoryView.OnHomeButtonClick',effect:'original UI navigation',transitionConfirmed:false};
});
globalThis.openInventory=kind=>RuntimeApi.runOnMainThreadOnce(()=>{
 if(!['characters','ships'].includes(kind))throw Error('Unsupported inventory kind');
 const R=RuntimeApi,S=SceneApi,A=R.arg,controller=R.findBound('StateController');
 const state=R.readRef(controller,'<CurrentState>k__BackingField');
 if(R.fullName(R.classOf(state))!=='HomeBaseGameState')throw Error('Opening inventory requires HomeBaseGameState');
 const klass=R.findClass('MasterInventoryGameState+TransitionData'),data=R.api('object_new','pointer',['pointer'])(klass),handle=R.retain(data);
 try{
 S.invoke('MasterInventoryGameState+TransitionData','.ctor',data);
 for(const [name,value] of [['Type',kind==='characters'?1:2],['NavType',kind==='characters'?8:10]])S.invoke('MasterInventoryGameState+TransitionData','set_'+name,data,[A.value('i32',value)]);
 S.invoke('MasterInventoryGameState+TransitionData','set_IsPlayerRoster',data,[A.value('bool',true)]);
 S.invoke('MasterInventoryGameState+TransitionData','set_UseConditionalCachedTransitionData',data,[A.value('bool',false)]);
 S.invoke('StateController','EnterState',controller,[A.ref(S.type('MasterInventoryGameState')),A.ref(data),A.value('bool',false)],['System.Type','IStateTransitionData','System.Boolean']);
 return {invoked:'StateController.EnterState',effect:'original inventory navigation',kind,transitionConfirmed:false};
 }finally{R.release(handle);}
});
rpc.exports={...rpc.exports,snapshot:()=>readRuntimeSnapshot(),inventoryhome:()=>inventoryHome(),openinventory:kind=>openInventory(kind),battleability:request=>RuntimeApi.runOnMainThreadOnce(()=>BattleInputBridge.choose(request))};
rpc.exports.battletarget=request=>RuntimeApi.runOnMainThreadOnce(()=>BattleInputBridge.selectTarget(request));

rpc.exports.battleunit=request=>RuntimeApi.runOnMainThreadOnce(()=>BattleInputBridge.selectUnit(request));

rpc.exports.battlecancel=request=>RuntimeApi.runOnMainThreadOnce(()=>BattleInputBridge.cancelSelection(request));

globalThis.readCampaignMenu=()=>{
 const R=RuntimeApi,S=SceneApi;
 return S.all('CampaignsPopupSelectListItem').filter(p=>R.unbox(S.invoke('UnityEngine.GameObject','get_activeInHierarchy',S.gameObject(p)),'bool')).map(p=>{
  const d=R.readRef(p,'_renderData');
  return {feature:R.readValue(d,'<Feature>k__BackingField','u8'),locked:!!R.readValue(d,'<IsLocked>k__BackingField','bool'),title:S.string(R.readRef(d,'<FeatureTitle>k__BackingField'))};
 });
};
rpc.exports.campaignnavigate=(operation,payload)=>RuntimeApi.runOnMainThreadOnce(()=>{
 const R=RuntimeApi,S=SceneApi,state=R.readRef(R.findBound('StateController'),'<CurrentState>k__BackingField');
 const stateName=R.fullName(R.classOf(state));
 const visible=type=>S.all(type).filter(p=>R.unbox(S.invoke('UnityEngine.GameObject','get_activeInHierarchy',S.gameObject(p)),'bool'));
 if(operation==='campaign-home'){
  if(stateName!=='CampaignGameState')throw Error('campaign-home requires campaign page');
  const views=visible('CampaignMissionsView');if(views.length!==1)throw Error('Campaign view must resolve uniquely');
  S.invoke('HudBaseView','OnHomeButtonClick',views[0]);
 }else{
  if(stateName!=='HomeBaseGameState')throw Error('Campaign menu navigation requires Home');
  if(operation==='campaign-menu'){
   if(readCampaignMenu().length)return {noOp:true,invoked:null};
   const props=visible('SelectableCantinaV2Prop').filter(p=>R.readValue(p,'_prop','i32')===9);
   if(props.length!==1)throw Error('Campaign prop must resolve uniquely');
   S.invoke('HomeBaseController','OnCampaignsClicked',R.readRef(state,'_homeBaseController'),[R.arg.value('i32',R.readValue(props[0],'_propState','i32'))]);
  }else if(operation==='campaign-enter'){
   if(!Number.isInteger(payload?.feature))throw Error('Campaign feature required');
   const rows=visible('CampaignsPopupSelectListItem').filter(p=>R.readValue(R.readRef(p,'_renderData'),'<Feature>k__BackingField','u8')===payload.feature);
   if(rows.length!==1||R.readValue(R.readRef(rows[0],'_renderData'),'<IsLocked>k__BackingField','bool'))throw Error('Choose a visible unlocked campaign row');
   S.invoke('CampaignsPopupSelectListItem','HandleEnterClicked',rows[0]);
  }else throw Error('Unsupported campaign operation');
 }
 return {invoked:operation,effect:'original menu navigation; no battle start or sweep'};
});
