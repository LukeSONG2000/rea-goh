import assert from 'node:assert/strict';
import vm from 'node:vm';
import {readFileSync} from 'node:fs';
const source=readFileSync(new URL('./runtime_api.js',import.meta.url),'utf8');
const block=(a,b)=>source.slice(source.indexOf(a),source.indexOf(b,source.indexOf(a)));
const nil={isNull:()=>true,toString:()=> '0x0'};
const pointer=n=>({isNull:()=>false,toString:()=>String(n)});
const a=pointer(1),b=pointer(2),m1=pointer(11),m2=pointer(12),m3=pointer(13);
let enumerations=0,lookups=0;
const methods=new Map([[a,[m1,m2]],[b,[m3]]]);
const descriptors=new Map([[m1,{name:'Choose',type:'System.Int32'}],[m2,{name:'Choose',type:'System.String'}],[m3,{name:'Choose',type:'System.Int32'}]]);
let rows=[{name:'Unit+Nested',imageName:'one.dll',klass:a},{name:'Unit+Nested',imageName:'two.dll',klass:b}];
const native={
 class_get_methods:(k,it)=>{enumerations++;return methods.get(k)?.[it.index++]||nil;},
 method_get_name:m=>({readUtf8String:()=>descriptors.get(m).name}),
 method_get_param_count:()=>1,
 method_get_param:(m)=>m,
 type_get_name:m=>({readUtf8String:()=>descriptors.get(m).type}),
 class_get_field_from_name:(k,name)=>{lookups++;return name==='value'?(k===a?m1:m3):nil;},
 class_get_parent:()=>nil
};
const ctx=vm.createContext({P:8,nil,Memory:{alloc:()=>({index:0,writePointer(){}})},utf:x=>x,api:name=>native[name],allClasses:()=>rows,fullName:k=>k.toString()});
vm.runInContext('const classCache=new Map(),fieldCache=new Map(),methodCache=new Map();'+block('  function findClass(', '  function fieldType(')+block('  function findMethod(', '  const arg=')+';globalThis.f={findClass,field,findMethod};',ctx);
const f=ctx.f;
assert.throws(()=>f.findClass('Unit/Nested'),/uniquely/);
assert.equal(f.findClass('Unit/Nested','one.dll'),a);
assert.equal(f.findClass('Unit+Nested','two.dll'),b);
assert.throws(()=>f.findClass('Absent'),/matches=0/);
rows.push({name:'Absent',klass:a});assert.equal(f.findClass('Absent'),a,'failed lookups must not be cached');
assert.throws(()=>f.findMethod(a,'Choose',1),/matches=2/);
assert.equal(f.findMethod(a,'Choose',1,['System.Int32']),m1);
let calls=enumerations;
assert.equal(f.findMethod(a,'Choose',1,['System.Int32']),m1);assert.equal(enumerations,calls,'repeat successful lookup uses cache');
assert.equal(f.findMethod(a,'Choose',1,['System.String']),m2);
assert.equal(f.findMethod(b,'Choose',1,['System.Int32']),m3);
assert.throws(()=>f.findMethod(a,'Choose',0,['System.Int32']),/matches=0/);
assert.throws(()=>f.findMethod(a,'Choose',1),/matches=2/,'typed lookup must not mask ambiguity');
assert.equal(f.field(a,'value'),m1);calls=lookups;assert.equal(f.field(a,'value'),m1);assert.equal(lookups,calls);
assert.equal(f.field(b,'value'),m3);assert.throws(()=>f.field(a,'absent'),/absent/);
console.log('PASS metadata cache: class image/alias/ambiguity, class+signature method keys, repeated lookups, field scope, failures not cached.');

