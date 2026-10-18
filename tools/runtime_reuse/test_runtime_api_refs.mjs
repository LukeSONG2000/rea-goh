// Offline contract regression; never loads Frida or attaches to a process.
// Unity 6000.3.17f1 libil2cpp/vm/Field.cpp:128,204 call SetValueRaw(...,false);
// reference cases at :289 store value itself, primitives dereference value.
// Run: node tools/runtime_reuse/test_runtime_api_refs.mjs
import assert from 'node:assert/strict';
import {readFileSync} from 'node:fs';
import vm from 'node:vm';

const source=readFileSync(new URL('./runtime_api.js',import.meta.url),'utf8');
function harness(code=source){
  let address=0x1000;
  class Pointer {
    constructor(value,bytes){this.value=value;this.bytes=bytes;}
    toString(){return '0x'+this.value.toString(16);}
    isNull(){return this.value===0;}
    writePointer(value){this.storedPointer=value;return this;}
    readPointer(){return this.storedPointer;}
    readUtf8String(){return this.text;}
    writeS32(value){this.bytes.writeInt32LE(value);return this;}
    readS32(){return this.bytes.readInt32LE();}
    writeU8(value){this.bytes.writeUInt8(value);return this;}
    readU8(){return this.bytes.readUInt8();}
    writeFloat(value){this.bytes.writeFloatLE(value);return this;}
    readFloat(){return this.bytes.readFloatLE();}
  }
  const nil=new Pointer(0),ptr=n=>n===0?nil:new Pointer(n);
  const allocate=size=>new Pointer(address+=0x100,Buffer.alloc(size));
  const klass=ptr(1),owner=ptr(2),target=ptr(3),calls=[],fields=new Map();
  const field=(name,kind,flags=0)=>{const f=ptr(address+=0x100);f.kind=kind;f.flags=flags;fields.set(name,f);};
  for(const [name,kind] of [['ref',0x12],['string',0x0e],['array',0x1d],['genericRef',0x15],['integer',8],['boolean',2],['float',12]]){
    field(name,kind);field('static_'+name,kind,0x10);
  }
  const native={
    object_get_class:object=>{assert.equal(object,owner);return klass;},
    class_get_field_from_name:(c,name)=>{assert.equal(c,klass);return fields.get(name.readUtf8String())||nil;},
    field_get_type:f=>f,
    type_get_type:t=>t.kind,
    field_get_flags:f=>f.flags,
    field_get_value:(object,f,buffer)=>{assert.equal(object,owner);buffer.writeS32(f.current);},
    class_from_type:t=>t,
    class_is_enum:()=>false,
    class_is_valuetype:()=>false,
    field_set_value:(object,f,value)=>{assert.equal(object,owner);calls.push({static:false,f,value});},
    field_static_set_value:(f,value)=>calls.push({static:true,f,value})
  };
  const context=vm.createContext({
    Process:{pointerSize:8,getModuleByName:name=>{assert.equal(name,'GameAssembly.dll');return {getExportByName:name=>name};}},
    ptr,
    Memory:{alloc:allocate,allocUtf8String:text=>{const p=allocate(text.length+1);p.text=text;return p;}},
    NativeFunction:function(name){const fn=native[name.replace(/^il2cpp_/,'')];assert.ok(fn,'Unexpected native call '+name);return fn;}
  });
  vm.runInContext(code,context,{filename:'runtime_api.js'});
  assert.equal(calls.length,0,'Library load must not write fields');
  return {api:context.RuntimeApi,klass,owner,target,nil,calls,fields,Pointer};
}

const h=harness();
// Stable field metadata must not turn live values into a stale snapshot.
h.fields.get('integer').current=100;
assert.equal(h.api.readValue(h.owner,'integer','i32'),100);
h.fields.get('integer').current=27;
assert.equal(h.api.readValue(h.owner,'integer','i32'),27);
assert.throws(()=>h.api.readValue(h.owner,'integer','f32'),/primitive mismatch/);
assert.equal(h.api.readValue(h.owner,'integer','i32'),27);

let assertions=0;
for(const isStatic of [false,true]){
  const owner=isStatic?null:h.owner,klass=isStatic?h.klass:undefined;
  for(const name of ['ref','string','array','genericRef']){
    const fieldName=(isStatic?'static_':'')+name;
    for(const input of [h.target,h.nil,null,undefined]){
      h.api.writeRef(owner,fieldName,input,klass);
      const call=h.calls.at(-1);
      assert.equal(call.static,isStatic);
      assert.equal(call.f,h.fields.get(fieldName));
      assert.equal(call.value,input||h.nil,'Reference setter must receive object pointer itself, including null');
      assertions++;
    }
  }
  for(const [name,kind,value,read,size] of [['integer','i32',-123456,'readS32',4],['boolean','bool',true,'readU8',1],['boolean','bool',false,'readU8',1],['float','f32',1.25,'readFloat',4]]){
    const fieldName=(isStatic?'static_':'')+name;
    h.api.writeValue(owner,fieldName,kind,value,klass);
    const call=h.calls.at(-1);
    assert.equal(call.static,isStatic);
    assert.equal(call.f,h.fields.get(fieldName));
    assert.ok(call.value instanceof h.Pointer,'Primitive setter needs an address');
    assert.equal(call.value.bytes.length,size);
    assert.equal(call.value[read](),kind==='bool'?Number(value):value);
    assertions++;
  }
  const before=h.calls.length;
  assert.throws(()=>h.api.writeRef(owner,(isStatic?'static_':'')+'integer',h.target,klass),/Not reference field/);
  assert.throws(()=>h.api.writeValue(owner,(isStatic?'static_':'')+'ref','i32',42,klass),/managed reference/);
  assert.equal(h.calls.length,before,'Rejected type mismatches must not write');
}

// Prove this regression detects the former pointer-to-pointer bug, independently
// of the mocked setter: the legacy argument is not the managed object address.
assert.ok(source.includes('setField(object,f,value || nil);'),'Update mutation anchor if helper changes');
const legacy=harness(source.replace('setField(object,f,value || nil);',
  'const buffer=Memory.alloc(P);buffer.writePointer(value || nil);setField(object,f,buffer);'));
legacy.api.writeRef(legacy.owner,'ref',legacy.target);
assert.notEqual(legacy.calls[0].value,legacy.target);
assert.equal(legacy.calls[0].value.readPointer(),legacy.target);
console.log(`PASS: ${assertions} instance/static reference/null and primitive cases; type guards; former bug detected. No process attached.`);
