import assert from 'node:assert/strict';
import fs from 'node:fs';
import vm from 'node:vm';
let serial=0,failConstructor=false,sharedQueue=false;
const retained=new Set();
const pointer=fields=>Object.assign({id:++serial,isNull:()=>false,toString(){return String(this.id);},equals(p){return this===p;}},fields);
const queue=()=>pointer({_array:pointer({}),_size:0,_version:0,_head:0,_tail:0,items:[]});
const original=pointer({type:'pool',_initialInstanceCapacity:10,_pool:queue()});
const context={BattleUnitControllerListPool:original};
const R={readRef:(o,k)=>o[k],readValue:(o,k)=>o[k],classOf:o=>o.type,
 api:()=>type=>pointer({type}),retain:p=>{retained.add(p);return p;},release:p=>retained.delete(p),
 arg:{value:(t,v)=>v},findMethod:(type,name)=>name,
 invoke(name,p,args=[]){
  assert.notEqual(p,original,'Never call original pool');
  if(name==='.ctor'){
   if(failConstructor)throw Error('constructor failure');
   assert.deepEqual(Array.from(args),[0,10]);p._initialInstanceCapacity=args[1];p._pool=sharedQueue?original._pool:queue();return;
  }
  if(name==='GetInstance'){
   const q=p._pool,result=q.items.shift()??pointer({type:'list',Pool:p,_size:0});q._size=q.items.length;q._version++;return result;
  }
  assert.equal(name,'Dispose');const q=p.Pool._pool;p._size=0;q.items.push(p);q._size=q.items.length;q._version++;
 }};
const sandbox={RuntimeApi:R};
vm.runInNewContext(fs.readFileSync(new URL('./original_ai_trace.js',import.meta.url),'utf8'),sandbox);
const audit=sandbox.verifyOriginalAITargetPool(context);
assert.equal(audit.originalUnchanged,true);assert.equal(audit.reuseConfirmed,true);assert.equal(audit.empty,true);assert.equal(retained.size,0);
const pool=sandbox.createOriginalAITargetPool(context),a=pool.borrow(),b=pool.borrow();
a._size=2;assert.notEqual(a,b);assert.throws(()=>pool.giveBack(pointer({})),/not borrowed/);
pool.giveBack(a);assert.throws(()=>pool.giveBack(a),/not borrowed/);
pool.dispose();pool.dispose();assert.equal(retained.size,0);assert.equal(b._size,0);
assert.throws(()=>pool.borrow(),/disposed/);assert.throws(()=>pool.giveBack(b),/disposed/);
failConstructor=true;assert.throws(()=>sandbox.createOriginalAITargetPool(context),/constructor failure/);assert.equal(retained.size,0);
failConstructor=false;sharedQueue=true;assert.throws(()=>sandbox.createOriginalAITargetPool(context),/shared/);assert.equal(retained.size,0);
assert.equal(original._pool._version,0);assert.equal(original._pool._size,0);
console.log('PASS private target pool ownership, reuse, return cleanup, double-return rejection, constructor failure and shared-queue rejection');
