'use strict';
// Standalone: load runtime_api, unity_scene_api, original_fixture, battle_proto, isolated_inventory.
// Supply schema-normalized inventoryBytes from a direct current Inventory capture.
// Checks only the listed fields; not certification of the entire account graph.
(async()=>{const R=RuntimeApi,S=SceneApi,A=R.arg,result={kind:'protobuf-probe-complete',passed:false};try{await R.runOnMainThreadOnce(()=>{
 const liveDC=R.findBound('InventoryDomainController'),live=R.readRef(liveDC,'<Inventory>k__BackingField');
 const proto=deserializeOriginalInventoryProto(inventoryBytes);const built=buildPrivateInventory(proto.object);
 result.originalDependenciesUnchanged=built.originalDependenciesUnchanged;result.liveInventoryUnchanged=R.readRef(liveDC,'<Inventory>k__BackingField').equals(live);
 const list=p=>Array.from({length:R.readValue(p,'_size','i32')},(_,i)=>R.readRef(p,'_items').add(32+i*8).readPointer());
 const units=i=>list(R.readRef(R.readRef(i,'_unitRoster'),'_units'));
 const original=units(live),copies=units(built.inventory),index=new Map(original.map(u=>[S.string(R.readRef(u,'Id')),u]));
 result.originalUnits=original.length;result.privateUnits=copies.length;result.shared=[];result.mismatches=[];
 for(const copy of copies){const id=S.string(R.readRef(copy,'Id')),o=index.get(id);if(!o){result.mismatches.push({reason:'missing live counterpart'});continue;}
 if(o.equals(copy))result.shared.push('UnitDO');
 for(const field of ['_statCalculator','<AttackAbility>k__BackingField','<LeaderAbility>k__BackingField','<Relic>k__BackingField','_equipmentSlots','_statModEquipSet','_skills']){const p=R.readRef(copy,field);if(!p.isNull()&&p.equals(R.readRef(o,field)))result.shared.push(field);}
 for(const [field,kind] of [['<CurrentLevel>k__BackingField','i32'],['<CurrentRarity>k__BackingField','u8'],['<CurrentTier>k__BackingField','u8'],['<CurrentEraLevel>k__BackingField','i32']])if(R.readValue(copy,field,kind)!==R.readValue(o,field,kind))result.mismatches.push({field});
 }
 result.passed=result.originalDependenciesUnchanged&&result.liveInventoryUnchanged&&original.length===copies.length&&!result.shared.length&&!result.mismatches.length;
 proto.release();
});}catch(e){result.error=String(e);result.stack=e.stack;if(e.managedException){const b=Memory.alloc(4096);R.api('format_exception','void',['pointer','pointer','int'])(e.managedException,b,4096);result.originalException=b.readUtf8String();}}finally{R.releaseAll();send(result);}})();
