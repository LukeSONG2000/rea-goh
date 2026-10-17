'use strict';
// PENDING-ORIGINAL: experimental standalone probe dependency, NOT a working trial API.
// Native conversion currently reports system error; do not load in the normal host.
// Main-thread only. Convert the WHOLE original payload so encounter-local definitions,
// overrides and state use the original factory ordering. Does not start a battle.
globalThis.convertCapturedBattle=(bytes,metadata)=>{
 const R=RuntimeApi,S=SceneApi,A=R.arg,handles=[];
 const mission=metadata?.mission;
 if(!mission||!['_campaignID','_mapId','_nodeId','_missionId'].every(k=>typeof mission[k]==='string'&&mission[k]))throw Error('Captured mission identity required');
 for(const value of [mission.difficulty,metadata.overrideRelicTier])if(!Number.isInteger(value)||value<0||value>255)throw Error('Captured enum must be a byte');
 globalThis.captureConversionPhase='deserialize';
 const source=deserializeOriginalBattleProto(bytes);
 const own=p=>{handles.push(R.retain(p));return p;};
 let released=false;
 const release=()=>{if(released)return;released=true;for(const h of handles.splice(0))R.release(h);source.release();};
 let gameData=null,oldMap=null,mapHandle=null,restored=false;
 try{
  globalThis.captureConversionPhase='resolve-factory';
  const factory=OriginalFixtureTools.inject('BattleDataDO+Factory');
  for(const name of ['_gameDataDC','_unitDOFactory','_unitDefinitionFactory','_battleStatDOFactory','_bucketItemDOFactory','_statTranslator','_grantedAbilityFactory','_statFactory','_battleUnitStateFactory','_datacronFactory'])
   if(R.readRef(factory,name).isNull())throw Error('Factory dependency is null: '+name);
  gameData=R.readRef(factory,'_gameDataDC');oldMap=R.readRef(gameData,'_pveUnitDefinitionMap');
  if(!oldMap.isNull())mapHandle=R.retain(oldMap);
  globalThis.captureConversionPhase='mission-identity';
  const identity=own(R.api('object_new','pointer',['pointer'])(R.findClass('CampaignElementIdentifierDO')));
  S.invoke('CampaignElementIdentifierDO','.ctor',identity);
  for(const [property,key] of [['CampaignID','_campaignID'],['MapID','_mapId'],['NodeID','_nodeId'],['MissionID','_missionId']])
   S.invoke('CampaignElementIdentifierDO','set_'+property,identity,[A.ref(R.string(mission[key]))]);
  S.invoke('CampaignElementIdentifierDO','set_Difficulty',identity,[A.value('u8',mission.difficulty)]);
  const method=R.findMethod(R.classOf(factory),'Get',3,['Protos.BattleData','CampaignElementIdentifierDO','RelicTierEnum']);
  if(!method.readPointer().equals(R.module.base.add(0x1D0BFF0)))throw Error('Unexpected BattleData factory revision');
  globalThis.captureConversionPhase='original-factory-get';
  // Diagnostic alternative to runtime_invoke; both currently fail. Hook silence
  // alone does not establish where the native failure happens.
  const get=new NativeFunction(method.readPointer(),'pointer',['pointer','pointer','pointer','uint8','pointer']);
  globalThis.captureConversionPhase='native-function-created';
  const returned=get(factory,source.object,identity,metadata.overrideRelicTier,method);
  globalThis.captureConversionPhase='native-function-returned';
  const data=own(returned);
  globalThis.captureConversionPhase='converted';
  return {battleData:data,missionId:identity,proto:source.object,release};
 }catch(e){release();throw e;}
 finally{
  try{if(gameData&&oldMap){R.writeRef(gameData,'_pveUnitDefinitionMap',oldMap);restored=R.readRef(gameData,'_pveUnitDefinitionMap').equals(oldMap);}}
  finally{if(mapHandle!==null)R.release(mapHandle);}
  if(gameData&&!restored)throw Error('Original PVE definition map restoration failed');
 }
};
