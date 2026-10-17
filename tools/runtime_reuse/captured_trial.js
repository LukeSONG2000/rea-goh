'use strict';
// Experimental standalone builder. Fresh inventory per trial, original saved squad,
// original captured mission and simulation context. No fabricated opponent/skill rules.
// Selected squad with Datacron, external Era units or battle state is not yet audited.
globalThis.createCapturedTrial=options=>{
 const R=RuntimeApi,A=R.arg,call=OriginalFixtureTools.call;
 const list=p=>p.isNull()?[]:Array.from({length:R.readValue(p,'_size','i32')},(_,i)=>R.readRef(p,'_items').add(32+i*8).readPointer());
 const state=R.findBound('StateController'),current=R.readRef(state,'<CurrentState>k__BackingField');
 if(R.fullName(R.classOf(current))!=='HomeBaseGameState')throw Error('Idle Home required');
 if(!R.readRef(R.findBound('BattleDomainController'),'<CurrentBattleState>k__BackingField').isNull())throw Error('Live battle exists');
 if(options.flags!==7||!Number.isInteger(options.squadType))throw Error('Explicit headless flags 7 and saved squad type required');
 const source=deserializeOriginalInventoryProto(options.inventoryBytes),built=buildPrivateInventory(source.object);
 const roster=R.readRef(built.inventory,'_unitRoster'),units=new Set(list(R.readRef(roster,'_units')).map(String));
 const matches=list(R.readRef(roster,'_characterSquads')).filter(s=>R.readValue(s,'SquadType','u8')===options.squadType);
 if(matches.length!==1)throw Error('Saved squad selection is missing or ambiguous');
 const player=matches[0],cells=list(R.readRef(player,'_squadCells'));
 if(!cells.length)throw Error('Empty saved squad');
 if(!R.readRef(player,'Datacron').isNull())throw Error('Selected Datacron requires isolation audit');
 if(list(R.readRef(player,'_squadAbilities')).length)throw Error('Selected squad abilities require isolation audit');
 for(const c of cells){const u=R.readRef(c,'Unit');
  if(!u.isNull()&&!units.has(String(u)))throw Error('Selected unit outside private roster');
  if(!R.readRef(c,'BattleUnitState').isNull())throw Error('Selected persisted battle state requires audit');
 }
 const converted=convertCapturedBattle(options.capturedBytes,options.capturedMetadata),battleData=converted.battleData,missionId=converted.missionId;
 const encounter=call(battleData,'get_EncounterId'),opponent=call(battleData,'GetSquad',[A.ref(encounter)]);
 if(opponent.isNull())throw Error('Captured initial encounter squad missing');
 const mission=call(R.findBound('CampaignDomainController'),'GetMission',[A.ref(missionId)]);
 if(mission.isNull())throw Error('Original mission unavailable');
 const data=R.newObject(R.findClass('BattleGameState+TransitionData'));
 call(data,'set_CampaignMissionIdentifier',[A.ref(missionId)]);
 call(data,'set_BattleTypeEnum',[A.value('u8',1)]);
 call(data,'set_CombatTypeEnum',[A.value('u8',0)]);
 call(data,'set_PlayerBattleSquad',[A.ref(player)]);
 call(data,'set_OpponentBattleSquad',[A.ref(opponent)]);
 const context=call(OriginalFixtureTools.inject('BattleContext+Factory'),'Get',[A.ref(data),A.value('i32',7)]);
 call(data,'set_BattleContext',[A.ref(context)]);
 const domain=call(context,'get_BattleDomainController');
 if(R.fullName(R.classOf(domain))!=='BattleDomainSimulationController')throw Error('Expected original simulation domain');
 const simulation=R.newObject(R.findClass('BattleSimulationController'));
 call(simulation,'set_HeadlessMode',[A.value('bool',true)]);
 call(context,'set_BattleController',[A.ref(simulation)]);
 call(battleData,'set_PlayerSquad',[A.ref(player)]);
 call(domain,'set_BattleData',[A.ref(battleData)]);
 return {data,context,domain,battleData,player,opponent,state,simulation,missionId,mission,options,source,built,converted};
};
