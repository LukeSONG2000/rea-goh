'use strict';
// Concatenate runtime_api.js + unity_scene_api.js. Uses scratch structs only.
// Codec acceptance evidence: byte roundtrip alone cannot establish default fidelity.
(async()=>{const R=RuntimeApi,S=SceneApi,A=R.arg,result={kind:'protobuf-probe-complete',rows:[]};try{await R.runOnMainThreadOnce(()=>{
const model=R.newObject(R.findClass('GameSerializer'));
for(const name of ['Protos.Stat','Protos.BucketItem']){
 const c=R.findClass(name),size=R.api('class_value_size','int',['pointer','pointer'])(c,Memory.alloc(4)),v=Memory.alloc(size),ctor=R.findMethod(c,'.ctor',1);
 new NativeFunction(ctor.readPointer(),'void',['pointer','int','pointer'])(v,0,ctor);
 const boxed=R.api('value_box','pointer',['pointer','pointer'])(c,v);R.retain(boxed);
 const generic=S.invoke('ProtoBuf.Serializer+NonGeneric','DeepClone',R.nil,[A.ref(boxed)]);R.retain(generic);
 const compiled=S.invoke('ProtoBuf.Meta.TypeModel','DeepClone',model,[A.ref(boxed)]);R.retain(compiled);
 const inspect=o=>{const p=R.api('object_unbox','pointer',['pointer'])(o);return name==='Protos.Stat'?{id:p.readS32(),ui:String(p.add(24).readS64())}:{id:S.string(p.readPointer()),rarity:p.add(20).readS32()};};
 result.rows.push({name,original:inspect(boxed),generic:inspect(generic),compiled:inspect(compiled)});
}
});}catch(e){result.error=String(e);}finally{R.releaseAll();send(result);}})();
