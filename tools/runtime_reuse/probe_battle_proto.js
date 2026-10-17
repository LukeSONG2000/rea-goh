'use strict';
// Scratch schema test, never a real mission payload and never used to launch battle.
(async()=>{
 const R=RuntimeApi,S=SceneApi,A=R.arg;let cloned=null;
 const result={kind:'protobuf-probe-complete',scope:'scratch original protobuf objects; no account or battle data',passed:false};
 try{
  await R.runOnMainThreadOnce(()=>{
   const make=name=>R.newObject(R.findClass(name));
   const add=(list,item)=>R.invoke(R.findMethod(R.classOf(list),'Add',1),list,[A.ref(item)]);
   const at=(list,i)=>R.readRef(list,'_items').add(32+8*i).readPointer();
   const source=make('Protos.BattleData');
   R.writeValue(source,'BattleSeed','i32',12345);R.writeValue(source,'TargetingSeed','i32',67890);R.writeValue(source,'EncounterIndex','i32',1);
   const encounters=S.invoke('Protos.BattleData','get_BattleEncounter',source),originalStates=[];
   for(let i=0;i<2;i++){
    const encounter=make('Protos.BattleEncounter'),state=make('Protos.BattleUnitState');
    R.writeRef(encounter,'EncounterId',R.string('scratch-'+i));
    R.writeRef(state,'UnitId',R.string('scratch-unit-'+i));
    R.writeValue(state,'CurrentHealth','i64',123456+i);R.writeValue(state,'MaxHealth','i64',999999);
    R.writeValue(state,'CurrentShield','i64',654321+i);
    add(S.invoke('Protos.BattleEncounter','get_UnitState',encounter),state);add(encounters,encounter);originalStates.push(state);
   }
   cloned=cloneOriginalBattleProto(source);const copy=cloned.object;
   const copyEncounters=R.readRef(copy,'_battleEncounter');
   if(copyEncounters.equals(encounters)||R.readValue(copyEncounters,'_size','i32')!==2)throw Error('Encounter list not cloned');
   for(let i=0;i<2;i++){
    const ce=at(copyEncounters,i),oe=at(encounters,i),cs=at(R.readRef(ce,'_unitState'),0);
    if(ce.equals(oe)||R.readRef(ce,'_unitState').equals(R.readRef(oe,'_unitState'))||cs.equals(originalStates[i]))throw Error('Shared mutable child');
    if(S.string(R.readRef(ce,'EncounterId'))!=='scratch-'+i||R.readValue(cs,'CurrentHealth','i64').toString()!==String(123456+i)||R.readValue(cs,'CurrentShield','i64').toString()!==String(654321+i))throw Error('Child values not preserved');
    R.writeValue(cs,'CurrentHealth','i64',1);
    if(R.readValue(originalStates[i],'CurrentHealth','i64').toString()!==String(123456+i))throw Error('Clone mutation escaped');
   }
   for(const [field,value] of [['BattleSeed',12345],['TargetingSeed',67890],['EncounterIndex',1]])if(R.readValue(copy,field,'i32')!==value)throw Error('Scalar not preserved '+field);
   result.passed=true;result.encounters=2;result.independentNestedState=true;
  });
 }catch(e){result.error=String(e);}
 finally{if(cloned)cloned.release();R.releaseAll();send(result);}
})();
