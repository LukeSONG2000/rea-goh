'use strict';
// Main-thread observation only. No UI navigation, input or service calls.
globalThis.readRuntimeSnapshot=()=>RuntimeApi.runOnMainThreadOnce(()=>{
 const R=RuntimeApi,S=SceneApi;
 const state=R.readRef(R.findBound('StateController'),'<CurrentState>k__BackingField');
 const stateName=R.fullName(R.classOf(state));
 const result={schemaVersion:1,state:stateName,units:[],scope:'observed current client; no gameplay input'};
 if(stateName!=='BattleGameState')return result;
 const context=R.readRef(R.readRef(state,'_battleController'),'_battleContext');
 const seen=new Set();
 for(const reticle of S.all('BattleMessageReticle')){
  const unit=S.invoke('BattleMessageReticle','get_TargetUnit',reticle);
  if(unit.isNull()||seen.has(unit.toString())||!R.readRef(unit,'_battleContext').equals(context))continue;
  if(!R.unbox(S.invoke('UnityEngine.GameObject','get_activeInHierarchy',S.gameObject(reticle)),'bool'))continue;
  seen.add(unit.toString());
  result.units.push({sessionKey:unit.toString(),attributes:readBattleAttributes(unit),status:readBattleStatus(unit)});
 }
 return result;
});
rpc.exports={snapshot:()=>readRuntimeSnapshot()};
