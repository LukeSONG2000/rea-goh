'use strict';
// Metadata-only observer. No game methods or service endpoints are invoked.
const mod=Process.getModuleByName('GameAssembly.dll');
const api=(n,r,a)=>new NativeFunction(mod.getExportByName('il2cpp_'+n),r,a);
const domain=api('domain_get','pointer',[])();
const getAssemblies=api('domain_get_assemblies','pointer',['pointer','pointer']);
const imageOf=api('assembly_get_image','pointer',['pointer']);
const imageName=api('image_get_name','pointer',['pointer']);
const classCount=api('image_get_class_count','uint64',['pointer']);
const classAt=api('image_get_class','pointer',['pointer','uint64']);
const className=api('class_get_name','pointer',['pointer']);
const classNS=api('class_get_namespace','pointer',['pointer']);
const methods=api('class_get_methods','pointer',['pointer','pointer']);
const methodName=api('method_get_name','pointer',['pointer']);
const params=api('method_get_param_count','uint',['pointer']);
const fields=api('class_get_fields','pointer',['pointer','pointer']);
const fieldName=api('field_get_name','pointer',['pointer']);
const offset=api('field_get_offset','size_t',['pointer']);
const flags=api('field_get_flags','uint',['pointer']);
const selected=new Set(['BattleSimulationController','BattleContext','TransitionData','Injector','BattleInputProvider','BattleController','BattleDomainSimulationController','BattleData','GameController']);
const size=Memory.alloc(8); const assemblies=getAssemblies(domain,size);const rows=[];
for(let a=0;a<size.readU64().toNumber();a++){
 const image=imageOf(assemblies.add(a*8).readPointer()),name=imageName(image).readUtf8String();
 if(!/Assembly-CSharp/.test(name))continue;
 for(let n=0;n<classCount(image).toNumber();n++){
  const c=classAt(image,n),cn=className(c).readUtf8String(); if(!selected.has(cn))continue;
  const row={name:cn,namespace:classNS(c).readUtf8String(),methods:[],fields:[]};
  let it=Memory.alloc(8);it.writePointer(ptr(0));let p;
  while(!(p=methods(c,it)).isNull()){const fn=p.readPointer();row.methods.push({name:methodName(p).readUtf8String(),parameters:params(p),rva:fn.isNull()?null:fn.sub(mod.base).toString()});}
  it.writePointer(ptr(0));while(!(p=fields(c,it)).isNull())row.fields.push({name:fieldName(p).readUtf8String(),offset:offset(p).toNumber(),flags:flags(p)});
  rows.push(row);
 }
}
send({kind:'selected-runtime-metadata',pid:Process.id,classes:rows});
