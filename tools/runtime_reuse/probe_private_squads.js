'use strict';
// Standalone main-thread audit, after private inventory dependencies and inventoryBytes.
// Does not start a battle or change the live inventory. Unsupported references are reported.
(async()=>{
 const R=RuntimeApi,S=SceneApi,result={kind:'protobuf-probe-complete',passed:false,squads:[],issues:[]};
 let source;
 const list=p=>p.isNull()?[]:Array.from({length:R.readValue(p,'_size','i32')},(_,i)=>R.readRef(p,'_items').add(32+i*8).readPointer());
 const ref=(p,f)=>R.readRef(p,f);
 try{await R.runOnMainThreadOnce(()=>{
  const state=ref(R.findBound('StateController'),'<CurrentState>k__BackingField');
  if(R.fullName(R.classOf(state))!=='HomeBaseGameState')throw Error('Idle Home required');
  const dc=R.findBound('InventoryDomainController'),live=ref(dc,'<Inventory>k__BackingField');
  source=deserializeOriginalInventoryProto(inventoryBytes);
  const built=buildPrivateInventory(source.object),roster=ref(built.inventory,'_unitRoster'),original=ref(live,'_unitRoster');
  const privateUnits=new Set(list(ref(roster,'_units')).map(String));
  if(!privateUnits.size)throw Error('Empty private roster');
  for(const field of ['_characterSquads','_shipSquads']){
   const copies=list(ref(roster,field)),originals=list(ref(original,field));
   if(copies.length!==originals.length)result.issues.push(field+' count differs');
   const byType=new Map(originals.map(p=>[R.readValue(p,'SquadType','u8'),p]));
   for(const squad of copies){
    const type=R.readValue(squad,'SquadType','u8'),o=byType.get(type);
    const row={category:field,type,cells:[],datacron:!ref(squad,'Datacron').isNull(),abilities:list(ref(squad,'_squadAbilities')).length};
    result.squads.push(row);
    if(!o){result.issues.push(field+'/'+type+' missing live counterpart');continue;}
    if(squad.equals(o))result.issues.push('Shared squad');
    for(const f of ['SquadType','TargetingTacticType'])if(R.readValue(squad,f,'u8')!==R.readValue(o,f,'u8'))result.issues.push('Squad field differs: '+f);
    if(row.datacron)result.issues.push(field+'/'+type+' datacron requires deep audit');
    if(row.abilities)result.issues.push(field+'/'+type+' squad abilities require deep audit');
    const cells=list(ref(squad,'_squadCells')),ocells=list(ref(o,'_squadCells'));
    if(cells.length!==ocells.length)result.issues.push('Cell count differs');
    for(let i=0;i<cells.length;i++){
     const c=cells[i],oc=ocells[i],u=ref(c,'Unit');
     const cell={index:R.readValue(c,'CellIndex','i32'),role:R.readValue(c,'SquadUnitType','u8'),empty:u.isNull(),privateRoster:u.isNull()||privateUnits.has(String(u)),state:!ref(c,'BattleUnitState').isNull()};
     if(!u.isNull())cell.baseId=S.string(ref(ref(u,'<Definition>k__BackingField'),'BaseId'));
     row.cells.push(cell);
     if(!cell.privateRoster)result.issues.push(field+'/'+type+' unit outside private roster');
     if(cell.state)result.issues.push(field+'/'+type+' battle state requires deep audit');
     if(!oc){result.issues.push('Missing original cell');continue;}
     if(c.equals(oc))result.issues.push('Shared cell');
     const ou=ref(oc,'Unit');
     if(u.isNull()!==ou.isNull()||(!u.isNull()&&S.string(ref(u,'Id'))!==S.string(ref(ou,'Id'))))result.issues.push('Cell unit differs');
     for(const [f,k] of [['CellIndex','i32'],['SquadUnitType','u8'],['Selectable','bool'],['IsOverkillable','bool'],['DeathRequiredForEncounter','bool'],['IsMandatory','bool'],['IsNonActivatedSquadUnit','bool'],['OverrideAllStatsMultiplier','f32']])if(R.readValue(c,f,k)!==R.readValue(oc,f,k))result.issues.push('Cell field differs: '+f);
    }
   }
  }
  result.liveInventoryUnchanged=ref(dc,'<Inventory>k__BackingField').equals(live);
  result.passed=result.squads.length>0&&result.issues.length===0&&result.liveInventoryUnchanged&&built.originalDependenciesUnchanged;
 });}catch(e){result.error=String(e);result.stack=e.stack;}
 finally{if(source)source.release();R.releaseAll();send(result);}
})();
