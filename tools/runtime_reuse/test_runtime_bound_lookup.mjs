import assert from 'node:assert/strict';
import vm from 'node:vm';
import {readFileSync} from 'node:fs';
const source=readFileSync(new URL('./runtime_api.js',import.meta.url),'utf8');
const block=source.slice(source.indexOf('  function findBound('),source.indexOf('  const mainThreadJobs='));
const ptr=n=>({isNull:()=>n===0,toString:()=>String(n)}),nil=ptr(0);
const klass=ptr(1),injector=ptr(2),binder=ptr(3),dictionary=ptr(4),binding=ptr(5);
let instance=ptr(6),found=true,rows=[],invocations=0,fallbacks=0;
const ctx=vm.createContext({P:8,nil,findClass:()=>klass,required:p=>p,classOf:()=>klass,
 readRef:(object,name)=>name==='_instance'?(object===nil?injector:instance):name==='_binder'?binder:dictionary,
 api:()=>()=>klass,Memory:{alloc:()=>({p:nil,writePointer(p){this.p=p;},readPointer(){return this.p;}})},
 findMethod:()=>klass,arg:{ref:value=>({value}),byRef:value=>({value})},
 invoke:(method,obj,args)=>{assert.equal(obj,dictionary);invocations++;args[1].value.writePointer(binding);return found;},unbox:x=>x,
 bindings:()=>{fallbacks++;return rows;}
});
vm.runInContext(block+';globalThis.resolve=findBound;',ctx);
assert.equal(ctx.resolve('StateController'),instance);instance=ptr(7);
assert.equal(ctx.resolve('StateController'),instance,'replacement binding instance must be read live');
assert.equal(invocations,2);assert.equal(fallbacks,0);
found=false;rows=[{boundType:'IState',instanceType:'StateController',instance,binding}];
assert.equal(ctx.resolve('StateController'),instance);assert.equal(fallbacks,1);
rows.push({boundType:'IState2',instanceType:'StateController',instance:ptr(8),binding});
assert.throws(()=>ctx.resolve('StateController'),/matches=2/);
rows=[];assert.throws(()=>ctx.resolve('StateController'),/matches=0/);
console.log('PASS native binding lookup: dictionary TryGetValue, live replacement instance, interface fallback and ambiguity rejection.');
