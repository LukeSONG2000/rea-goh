'use strict';
// Read-only live object inventory. Never serialize account fields or credentials.
const m=Process.getModuleByName('GameAssembly.dll');
const api=(n,r,a)=>new NativeFunction(m.getExportByName('il2cpp_'+n),r,a);
const name=api('class_get_name','pointer',['pointer']);
const klass=api('object_get_class','pointer',['pointer']);
const fname=api('field_get_name','pointer',['pointer']);
const fields=api('class_get_fields','pointer',['pointer','pointer']);
const off=api('field_get_offset','size_t',['pointer']);
const getfield=api('class_get_field_from_name','pointer',['pointer','pointer']);
const cn=p=>p.isNull()?null:name(klass(p)).readUtf8String();
const field=(c,n)=>getfield(c,Memory.allocUtf8String(n));
const ref=(o,n)=>o.add(off(field(klass(o),n)).toNumber()).readPointer();
const count=Memory.alloc(8);
const aa=api('domain_get_assemblies','pointer',['pointer','pointer'])(api('domain_get','pointer',[])(),count);
let ic=ptr(0);
for(let i=0;i<count.readU64().toNumber();i++){
 const im=api('assembly_get_image','pointer',['pointer'])(aa.add(i*8).readPointer());
 const candidate=api('class_from_name','pointer',['pointer','pointer','pointer'])(im,Memory.allocUtf8String(''),Memory.allocUtf8String('Injector'));
 if(!candidate.isNull())ic=candidate;
}
if(ic.isNull())throw Error('Injector class absent');
const slot=Memory.alloc(8);api('field_static_get_value','void',['pointer','pointer'])(field(ic,'_instance'),slot);
const injector=slot.readPointer();if(injector.isNull())throw Error('Injector not initialized');
const binder=ref(injector,'_binder'),dict=ref(binder,'_typeMap'),entries=ref(dict,'_entries');
const ec=api('class_get_element_class','pointer',['pointer'])(klass(entries));
const stride=api('class_value_size','int',['pointer','pointer'])(ec,Memory.alloc(4));
// Field offsets on value types include the 16-byte boxed header.
const vo=off(field(ec,'value')).toNumber()-16;
const n=entries.add(24).readU64().toNumber(),rows=[];
for(let i=0;i<n;i++){
 const binding=entries.add(32+i*stride+vo).readPointer();if(binding.isNull())continue;
 const instance=ref(binding,'_instance');if(instance.isNull())continue;
 const type=cn(instance);const row={type};
 if(/StateController|BattleFsm|CampaignDomain|InventoryUnit|BattleContext|BattleDomain|UnitDomain/.test(type)){
  row.refs=[];const it=Memory.alloc(8);it.writePointer(ptr(0));let f;
  while(!(f=fields(klass(instance),it)).isNull()){
   const nm=fname(f).readUtf8String();
   if(/state|current|mission|squad|unit|context/i.test(nm)){
    const ft=api('field_get_type','pointer',['pointer'])(f);
    const kind=api('type_get_type','int',['pointer'])(ft);
    if([0x12,0x15,0x1c,0x1d].includes(kind))try{row.refs.push({field:nm,type:cn(instance.add(off(f).toNumber()).readPointer())});}catch(e){row.refs.push({field:nm,error:String(e)});}
   }
  }
 }
 rows.push(row);
}
send({kind:'binding-inventory',pid:Process.id,binder:cn(binder),entryStride:stride,instances:rows});
