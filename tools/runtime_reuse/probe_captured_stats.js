'use strict';
// Standalone read-only comparison against original GameData currently loaded in client.
// Requires runtime_api, unity_scene_api, original_fixture, battle_proto, captured_battle.
(async()=>{
 const R=RuntimeApi,S=SceneApi,A=R.arg,call=OriginalFixtureTools.call;
 const result={kind:'protobuf-probe-complete',passed:false,rows:[]};let converted;
 const list=p=>Array.from({length:R.readValue(p,'_size','i32')},(_,i)=>R.readRef(p,'_items').add(32+i*8).readPointer());
 const stats=p=>Array.from({length:62},(_,i)=>R.unbox(call(p,'GetStat',[A.value('u8',i+1)]),'f32'));
 try{await R.runOnMainThreadOnce(()=>{
  const state=R.readRef(R.findBound('StateController'),'<CurrentState>k__BackingField');
  if(R.fullName(R.classOf(state))!=='HomeBaseGameState')throw Error('Idle Home required');
  const game=R.findBound('GameDataDomainController'),before=R.readRef(game,'_pveUnitDefinitionMap');
  // Read existing original definitions BEFORE any capture conversion.
  const ids=['PVE_OOMCOMMANDBATTLEDROID:ONBOARDING','PVE_OOMSECURITYBATTLEDROID:ONBOARDING','PVE_B1BATTLEDROID:ONBOARDING','PVE_DROIDEKA:ONBOARDING'];
  const original=new Map(ids.map(id=>{
   const d=call(game,'GetUnitDefinition',[A.ref(R.string(id))]);
   if(d.isNull())throw Error('Original definition missing '+id);
   return [id,stats(R.readRef(d,'StatDefinition'))];
  }));
  converted=convertCapturedBattle(capturedBytes,capturedMetadata);
  const squad=call(converted.battleData,'GetSquad',[A.ref(call(converted.battleData,'get_EncounterId'))]);
  for(const cell of list(R.readRef(squad,'_squadCells'))){
   const u=R.readRef(cell,'Unit'),d=R.readRef(u,'<Definition>k__BackingField'),id=S.string(R.readRef(d,'Id'));
   const ds=stats(R.readRef(d,'StatDefinition')),os=original.get(id);
   const us=stats(call(u,'get_StatDefinition'));
   const calculated=R.unbox(call(u,'GetCalculatedStatValue',[A.value('u8',1),A.value('bool',false),A.ref(R.nil),A.value('i32',0)]),'f32');
   result.rows.push({id,originalDefinitionStats:os,capturedDefinitionStats:ds,convertedUnitStats:us,calculatedHp:calculated,equal:!!os&&os.every((v,i)=>v===ds[i])});
  }
  result.mapRestored=R.readRef(game,'_pveUnitDefinitionMap').equals(before);
  result.passed=result.rows.length===5&&result.rows.every(r=>r.equal)&&result.mapRestored;
 });}catch(e){result.error=String(e);result.stack=e.stack;}
 finally{if(converted)converted.release();R.releaseAll();send(result);}
})();
