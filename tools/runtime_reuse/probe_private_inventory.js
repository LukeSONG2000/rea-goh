'use strict';
// Standalone: load runtime_api, unity_scene_api, original_fixture, battle_proto, isolated_inventory.
// Supply schema-normalized inventoryBytes from a direct current Inventory capture.
// Checks only the listed fields; not certification of the entire account graph.
(async()=>{const R=RuntimeApi,S=SceneApi,A=R.arg,result={kind:'protobuf-probe-complete',passed:false};try{await R.runOnMainThreadOnce(()=>{
 const state=R.readRef(R.findBound('StateController'),'<CurrentState>k__BackingField');
 if(R.fullName(R.classOf(state))!=='HomeBaseGameState')throw Error('Idle Home required for inventory audit');
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

 result.equipmentAudit={mods:0,stats:0,rolls:0,slots:0,crew:0};
 const ref=(p,f)=>R.readRef(p,f),eq=(a,b,label)=>{if(a!==b)result.mismatches.push({field:label});};
 function separate(a,b,label){if(a.isNull()||b.isNull()){eq(a.isNull(),b.isNull(),label+' presence');return false;}if(a.equals(b))result.shared.push(label);return true;}
 function scalars(a,b){
  for(let c=R.classOf(a);!c.isNull();c=R.api('class_get_parent','pointer',['pointer'])(c)){
   const it=Memory.alloc(8);it.writePointer(R.nil);for(;;){const f=R.api('class_get_fields','pointer',['pointer','pointer'])(c,it);if(f.isNull())break;if(R.api('field_get_flags','uint',['pointer'])(f)&0x10)continue;
    const name=R.api('field_get_name','pointer',['pointer'])(f).readUtf8String(),t=R.api('field_get_type','pointer',['pointer'])(f);let code=R.api('type_get_type','int',['pointer'])(t),k=R.api('class_from_type','pointer',['pointer'])(t);
    if(R.api('class_is_enum','bool',['pointer'])(k))code=R.api('type_get_type','int',['pointer'])(R.api('class_enum_basetype','pointer',['pointer'])(k));
    const kind={2:'bool',4:'i8',5:'u8',6:'i16',7:'u16',8:'i32',9:'u32',10:'i64',11:'u64',12:'f32',13:'f64'}[code];
    if(kind)eq(String(R.readValue(a,name,kind)),String(R.readValue(b,name,kind)),name);
    else if(code===14)eq(S.string(ref(a,name)),S.string(ref(b,name)),name);
   }
  }
 }
 function stat(a,b){if(!separate(a,b,'mod stat'))return;result.equipmentAudit.stats++;scalars(a,b);
 const ar=ref(a,'<Rolls>k__BackingField'),br=ref(b,'<Rolls>k__BackingField');if(!separate(ar,br,'mod roll list'))return;
 const aa=list(ar),bb=list(br);eq(aa.length,bb.length,'roll count');for(let i=0;i<Math.min(aa.length,bb.length);i++){if(separate(aa[i],bb[i],'mod roll')){scalars(aa[i],bb[i]);result.equipmentAudit.rolls++;}}
 }
 const visitedMods=new Set();
 function mod(a,b){if(!separate(a,b,'mod'))return;const key=String(a)+'/'+String(b);if(visitedMods.has(key))return;visitedMods.add(key);result.equipmentAudit.mods++;scalars(a,b);stat(ref(a,'<PrimaryStat>k__BackingField'),ref(b,'<PrimaryStat>k__BackingField'));
 const ar=ref(a,'<SecondaryStats>k__BackingField'),br=ref(b,'<SecondaryStats>k__BackingField');if(!separate(ar,br,'secondary array'))return;const aa=S.array(ar),bb=S.array(br);eq(aa.length,bb.length,'secondary count');for(let i=0;i<Math.min(aa.length,bb.length);i++)stat(aa[i],bb[i]);
 }
 const privateUnitPointers=new Set(copies.map(String));
 for(const copy of copies){const o=index.get(S.string(ref(copy,'Id')));if(!o)continue;
 const a=ref(o,'_statModEquipSet'),b=ref(copy,'_statModEquipSet');if(a.isNull()||b.isNull())continue;
 eq(R.fullName(R.classOf(a)),R.fullName(R.classOf(b)),'equipment set class');
 const aa=list(ref(a,'_equipmentSlots')),bb=list(ref(b,'_equipmentSlots'));eq(aa.length,bb.length,'slot count');
 for(let i=0;i<Math.min(aa.length,bb.length);i++){
  separate(aa[i],bb[i],'equipment slot');result.equipmentAudit.slots++;
  const x=ref(aa[i],'<EquippedItem>k__BackingField'),y=ref(bb[i],'<EquippedItem>k__BackingField');
  if(R.fullName(R.classOf(a))==='StatModEquipmentSetDO')mod(x,y);
  else if(!x.isNull()&&!y.isNull()){
   separate(x,y,'crew holder');const xu=ref(x,'<CrewMember>k__BackingField'),yu=ref(y,'<CrewMember>k__BackingField');
   if(!privateUnitPointers.has(String(yu)))result.shared.push('crew outside private roster');
   eq(S.string(ref(xu,'Id')),S.string(ref(yu,'Id')),'crew unit identity');result.equipmentAudit.crew++;
  }
 }
 }
 const liveMods=list(ref(live,'_statMods')),privateMods=list(ref(built.inventory,'_statMods')),modIndex=new Map(liveMods.map(m=>[S.string(ref(m,'<Id>k__BackingField')),m]));
 eq(liveMods.length,privateMods.length,'inventory mod count');for(const m of privateMods){const originalMod=modIndex.get(S.string(ref(m,'<Id>k__BackingField')));if(originalMod)mod(originalMod,m);else result.mismatches.push({field:'inventory mod missing'});}

 result.calculatedStats={units:0,comparisons:0,datacronContexts:0};
 const statMethod=R.findMethod(R.findClass('UnitDO'),'GetCalculatedStatValue',4,['UnitStatEnum','System.Boolean','DatacronDO','System.Int32']);
 const calculate=new NativeFunction(statMethod.readPointer(),'float',['pointer','uint8','bool','pointer','int','pointer']);
 for(const copy of copies){const o=index.get(S.string(ref(copy,'Id')));if(!o)continue;
 const originalContext=ref(o,'_equippedDatacron'),privateContext=ref(copy,'_equippedDatacron');
 const originalIndex=R.readValue(o,'_equippedDatacronActiveStatIndex','i32'),privateIndex=R.readValue(copy,'_equippedDatacronActiveStatIndex','i32');
 if(!originalContext.isNull()||!privateContext.isNull()){result.calculatedStats.datacronContexts++;continue;}
 // Pass each object's existing context back unchanged: the original getter writes it.
 for(let statId=1;statId<=62;statId++){
  const a=calculate(o,statId,0,originalContext,originalIndex,statMethod),b=calculate(copy,statId,0,privateContext,privateIndex,statMethod);
  result.calculatedStats.comparisons++;if(!Number.isFinite(a)||!Number.isFinite(b)||a!==b)result.mismatches.push({field:'calculated stat',statId,original:a,copy:b});
 }
 if(!ref(o,'_equippedDatacron').equals(originalContext)||R.readValue(o,'_equippedDatacronActiveStatIndex','i32')!==originalIndex)throw Error('Original stat context changed');
 result.calculatedStats.units++;
 }
 result.passed=result.originalDependenciesUnchanged&&result.liveInventoryUnchanged&&original.length>0&&original.length===copies.length&&result.calculatedStats.datacronContexts===0&&!result.shared.length&&!result.mismatches.length;
 proto.release();
});}catch(e){result.error=String(e);result.stack=e.stack;if(e.managedException){const b=Memory.alloc(4096);R.api('format_exception','void',['pointer','pointer','int'])(e.managedException,b,4096);result.originalException=b.readUtf8String();}}finally{R.releaseAll();send(result);}})();
