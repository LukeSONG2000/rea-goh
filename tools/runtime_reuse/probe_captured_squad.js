'use strict';
// Standalone probe. Supply capturedBytes from a verified BattleData file.
// This converts captured enemy squads only; never substitutes them for the player.
(async()=>{
 const R=RuntimeApi,S=SceneApi,A=R.arg,sources=[];
 const result={kind:'protobuf-probe-complete',scope:'captured enemy squad original factory isolation',passed:false,encounters:[],sharedFields:[]};
 const list=p=>p.isNull()?[]:Array.from({length:R.readValue(p,'_size','i32')},(_,i)=>R.readRef(p,'_items').add(32+i*8).readPointer());
 try{await R.runOnMainThreadOnce(()=>{
  result.phase='convert-whole-battle-data';
  const gameData=R.findBound('GameDataDomainController'),before=R.readRef(gameData,'_pveUnitDefinitionMap');
  for(let i=0;i<2;i++)sources.push(convertCapturedBattle(capturedBytes,capturedMetadata));
  result.originalDefinitionMapRestored=R.readRef(gameData,'_pveUnitDefinitionMap').equals(before);
  if(!result.originalDefinitionMapRestored)throw Error('Definition map changed');
  const encounters=sources.map(s=>list(R.readRef(s.proto,'_battleEncounter')));
  for(let i=0;i<encounters[0].length;i++){
   result.phase='convert-encounter-'+i;
   const squads=sources.map((s,k)=>S.invoke('BattleDataDO','GetSquad',s.battleData,[A.ref(R.readRef(encounters[k][i],'EncounterId'))]));
   if(squads[0].equals(squads[1]))throw Error('Shared squad root');
   const cells=squads.map(s=>list(R.readRef(s,'_squadCells'))),rows=[];
   if(cells[0].length!==cells[1].length)throw Error('Cell count differs');
   for(let j=0;j<cells[0].length;j++){
    result.phase='inspect-encounter-'+i+'-cell-'+j;
    const pair=cells.map(c=>R.readRef(c[j],'Unit'));
    if(cells[0][j].equals(cells[1][j])||pair[0].equals(pair[1]))throw Error('Shared cell or unit');
    for(const field of ['_statCalculator','<AttackAbility>k__BackingField','<LeaderAbility>k__BackingField','<Relic>k__BackingField','_equipmentSlots','_statModEquipSet','_skills']){
     const refs=pair.map(p=>R.readRef(p,field));
     if(!refs[0].isNull()&&refs[0].equals(refs[1]))result.sharedFields.push({encounter:i,cell:j,field});
    }
    const level=R.readValue(pair[0],'<CurrentLevel>k__BackingField','i32');
    if(level!==R.readValue(pair[1],'<CurrentLevel>k__BackingField','i32'))throw Error('Level differs');
    // Only touch an isolated temporary object; verify the second conversion stays unchanged.
    R.writeValue(pair[0],'<CurrentLevel>k__BackingField','i32',level+1);
    if(R.readValue(pair[1],'<CurrentLevel>k__BackingField','i32')!==level)throw Error('Mutation escaped');
    R.writeValue(pair[0],'<CurrentLevel>k__BackingField','i32',level);
    rows.push({baseId:S.string(R.readRef(R.readRef(pair[1],'<Definition>k__BackingField'),'BaseId')),level});
   }
   result.encounters.push({id:S.string(R.readRef(encounters[0][i],'EncounterId')),units:rows});
  }
  result.passed=result.encounters.length>0&&result.sharedFields.length===0;
 });}catch(e){result.error=String(e);result.stack=e.stack;result.conversionPhase=globalThis.captureConversionPhase;}
 finally{for(const s of sources)s.release();R.releaseAll();send(result);}
})();
