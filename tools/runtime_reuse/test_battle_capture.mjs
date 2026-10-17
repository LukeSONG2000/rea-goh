import assert from 'node:assert/strict';
import fs from 'node:fs';
import vm from 'node:vm';
const source=fs.readFileSync(new URL('./battle_capture.js',import.meta.url),'utf8');
function fixture(fail=false){
 const hooks=new Map(),messages=[];let thread=1,clones=0,releases=0,detaches=0;
 const context={RuntimeApi:{module:{base:{add:n=>n}},classOf:x=>x,fullName:()=> 'Protos.BattleData'},SceneApi:{},
  Process:{getCurrentThreadId:()=>thread},Interceptor:{attach:(rva,h)=>{hooks.set(rva,h);return {detach(){detaches++;}};}},
  cloneOriginalBattleProto:()=>{clones++;return {object:{},release(){releases++;}};},
  serializeOriginalBattleProto:()=>{if(fail)throw Error('serialization failed');return new ArrayBuffer(3);},send:(meta,bytes)=>messages.push({meta,bytes})};
 vm.runInNewContext(source,context);
 const args=[null,{}, {isNull:()=>true},{toInt32:()=>0}];
 return {enter(){const call={};hooks.get(0xF8F750).onEnter.call(call);return call;},leave(call){hooks.get(0xF8F750).onLeave.call(call);},factory(){hooks.get(0x1D0BFF0).onEnter(args);},setThread:n=>thread=n,
  counts:()=>({clones,releases,detaches}),messages,status:()=>context.BattleCapture.status(),stop:()=>context.BattleCapture.stop()};
}
const f=fixture();f.factory();assert.equal(f.counts().clones,0,'Unrelated factory calls excluded');
const outer=f.enter(),inner=f.enter();f.leave(inner);
f.setThread(2);f.factory();assert.equal(f.counts().clones,0,'Other thread excluded');
f.setThread(1);f.factory();f.factory();f.leave(outer);f.factory();
assert.equal(f.messages.length,1);assert.equal(f.messages[0].bytes.byteLength,3);
assert.deepEqual(f.counts(),{clones:1,releases:1,detaches:0});
f.stop();assert.equal(f.counts().detaches,2);
const bad=fixture(true),call=bad.enter();for(let i=0;i<20;i++)bad.factory();bad.leave(call);
assert.equal(bad.messages.length,5,'Repeated failure is bounded');
assert.equal(bad.counts().releases,5,'Clones released even on serialization failure');
assert.equal(bad.status().captured,false);
console.log('PASS capture callback scope, thread isolation, one-shot, cleanup and failure bound (mock runtime)');
