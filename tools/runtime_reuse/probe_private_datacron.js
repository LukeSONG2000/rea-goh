'use strict';
// Standalone read-only audit of saved squad Datacron combat objects. No equip/reroll API.
(async()=>{
 const R=RuntimeApi,S=SceneApi,A=R.arg,call=OriginalFixtureTools.call;
 const result={kind:'protobuf-probe-complete',passed:false,rows:[],shared:[],mismatches:[],definitionReferences:[]};let source;
 const ref=(p,f)=>R.readRef(p,f);
 const list=p=>p.isNull()?[]:Array.from({length:R.readValue(p,'_size','i32')},(_,i)=>ref(p,'_items').add(32+i*8).readPointer());
 const equal=(a,b,label)=>{if(a!==b)result.mismatches.push(label);};
 const separate=(a,b,label)=>{if(a.isNull()||b.isNull()){equal(a.isNull(),b.isNull(),label+' presence');return false;}if(a.equals(b))result.shared.push(label);return true;};
 function strings(a,b,label){if(!separate(a,b,label))return;const aa=list(a).map(S.string),bb=list(b).map(S.string);equal(JSON.stringify(aa),JSON.stringify(bb),label+' values');}
 function scalars(a,b,fields){for(const [f,k] of fields)equal(String(R.readValue(a,f,k)),String(R.readValue(b,f,k)),f);}
 function scalarFields(a,b){
  const it=Memory.alloc(8);it.writePointer(R.nil);
  for(;;){const f=R.api('class_get_fields','pointer',['pointer','pointer'])(R.classOf(a),it);if(f.isNull())break;
   if(R.api('field_get_flags','uint',['pointer'])(f)&0x10)continue;
   const name=R.api('field_get_name','pointer',['pointer'])(f).readUtf8String(),t=R.api('field_get_type','pointer',['pointer'])(f),c=R.api('class_from_type','pointer',['pointer'])(t);
   let code=R.api('type_get_type','int',['pointer'])(t);
   if(R.api('class_is_enum','bool',['pointer'])(c))code=R.api('type_get_type','int',['pointer'])(R.api('class_enum_basetype','pointer',['pointer'])(c));
   const kind={2:'bool',4:'i8',5:'u8',6:'i16',7:'u16',8:'i32',9:'u32',10:'i64',11:'u64',12:'f32',13:'f64'}[code];
   if(kind)scalars(a,b,[[name,kind]]);
   else if(code===14)equal(S.string(ref(a,name)),S.string(ref(b,name)),name);
  }
 }
 try{await R.runOnMainThreadOnce(()=>{
  const state=ref(R.findBound('StateController'),'<CurrentState>k__BackingField');
  if(R.fullName(R.classOf(state))!=='HomeBaseGameState')throw Error('Idle Home required');
  const dc=R.findBound('InventoryDomainController'),live=ref(dc,'<Inventory>k__BackingField');
  source=deserializeOriginalInventoryProto(inventoryBytes);const built=buildPrivateInventory(source.object);
  const squads=i=>list(ref(ref(i,'_unitRoster'),'_characterSquads'));
  const originals=new Map(squads(live).map(s=>[R.readValue(s,'SquadType','u8'),s]));
  for(const squad of squads(built.inventory)){
   const d=ref(squad,'Datacron');if(d.isNull())continue;
   const type=R.readValue(squad,'SquadType','u8'),oSquad=originals.get(type),o=ref(oSquad,'Datacron');
   if(!separate(o,d,'DatacronDO'))continue;
   const row={squadType:type,affixes:0,modifierComparisons:0,activeComparisons:0,mechanics:[]};result.rows.push(row);
   equal(S.string(ref(o,'<Id>k__BackingField')),S.string(ref(d,'<Id>k__BackingField')),'Datacron identity');
   scalars(o,d,[['<RerollIndex>k__BackingField','i32'],['<Locked>k__BackingField','bool'],['<RerollCount>k__BackingField','i32'],['_focused','bool']]);
   strings(ref(o,'<Tags>k__BackingField'),ref(d,'<Tags>k__BackingField'),'Datacron tags');
   for(const f of ['<SetDefinition>k__BackingField','<TemplateDefinition>k__BackingField']){
    if(!ref(o,f).equals(ref(d,f)))result.mismatches.push('Definition identity '+f);
    result.definitionReferences.push(f);
   }
   for(const field of ['<Affixes>k__BackingField','<RerollOptions>k__BackingField']){
    const aa=ref(o,field),bb=ref(d,field);if(!separate(aa,bb,field))continue;
    const count=R.unbox(call(aa,'get_Count'),'i32');equal(count,R.unbox(call(bb,'get_Count'),'i32'),field+' count');
    for(let i=0;i<count;i++){
     const a=call(aa,'get_Item',[A.value('i32',i)]),b=call(bb,'get_Item',[A.value('i32',i)]);R.retain(a);R.retain(b);
     row.affixes++;
     scalars(a,b,[['RequiredUnitTier','u8'],['RequiredRelicTier','u8'],['Stat','u8'],['StatValue','f32'],['Scope','i32']]);
     for(const f of ['ScopeIcon','TargetRule'])equal(S.string(ref(a,f)),S.string(ref(b,f)),f);
     strings(ref(a,'Tags'),ref(b,'Tags'),'affix tags');
     const ga=ref(a,'GrantedAbility'),gb=ref(b,'GrantedAbility');
     if(separate(ga,gb,'GrantedAbilityDO')){
      scalarFields(ga,gb);
      const ad=ref(ga,'<Ability>k__BackingField'),bd=ref(gb,'<Ability>k__BackingField');
      if(separate(ad,bd,'AbilityDO'))scalarFields(ad,bd);
      equal(ref(ad,'_definition').equals(ref(bd,'_definition')),true,'ability definition identity');
      equal(ref(ga,'<TargetRule>k__BackingField').equals(ref(gb,'<TargetRule>k__BackingField')),true,'target definition identity');
      equal(ref(ga,'<UnlockRequirement>k__BackingField').isNull(),true,'unexpected original unlock requirement');
      equal(ref(gb,'<UnlockRequirement>k__BackingField').isNull(),true,'unexpected private unlock requirement');
      const za=ref(ga,'<ZonesToApplyTo>k__BackingField'),zb=ref(gb,'<ZonesToApplyTo>k__BackingField');
      equal(za.isNull(),zb.isNull(),'zone presence');
      if(!za.isNull()){equal(R.unbox(call(za,'get_Count'),'i32'),0,'original nonempty zones');equal(R.unbox(call(zb,'get_Count'),'i32'),0,'private nonempty zones');}
      row.mechanics.push({index:i,abilityId:S.string(call(ad,'get_Id')),abilityObjectShared:ad.equals(bd),abilityDefinitionShared:ref(ad,'_definition').equals(ref(bd,'_definition')),targetDefinitionShared:ref(ga,'<TargetRule>k__BackingField').equals(ref(gb,'<TargetRule>k__BackingField'))});
     }
    }
    if(field==='<Affixes>k__BackingField'){
     for(let index=-1;index<count;index++)for(let stat=1;stat<=62;stat++){
      const args=[A.value('u8',stat),A.value('i32',index)];
      equal(R.unbox(call(o,'GetStatModifier',args),'f32'),R.unbox(call(d,'GetStatModifier',args),'f32'),'native stat modifier');row.modifierComparisons++;
     }
     for(let i=0;i<count;i++){
      equal(R.unbox(call(o,'IsAffixActiveAt',[A.ref(oSquad),A.value('i32',i)]),'bool'),R.unbox(call(d,'IsAffixActiveAt',[A.ref(squad),A.value('i32',i)]),'bool'),'native affix eligibility');row.activeComparisons++;
     }
    }
   }
  }
  result.liveInventoryUnchanged=ref(dc,'<Inventory>k__BackingField').equals(live);
  result.passed=result.rows.length>0&&!result.shared.length&&!result.mismatches.length&&result.liveInventoryUnchanged;
 });}catch(e){result.error=String(e);result.stack=e.stack;}
 finally{if(source)source.release();R.releaseAll();send(result);}
})();
