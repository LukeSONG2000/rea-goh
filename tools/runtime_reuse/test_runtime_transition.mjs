import assert from 'node:assert/strict';
import fs from 'node:fs';
import vm from 'node:vm';
const sandbox={rpc:{exports:{}},SceneApi:{},RuntimeApi:{
 runOnMainThreadOnce:fn=>fn(),findBound:()=>({}),readRef:()=>({isNull:()=>true}),
 fullName:()=>{throw Error('Null transition must not dereference metadata');}}};
vm.runInNewContext(fs.readFileSync(new URL('./runtime_observer.js',import.meta.url),'utf8'),sandbox);
const snapshot=await sandbox.rpc.exports.snapshot();
assert.equal(snapshot.state,'Transitioning');assert.equal(snapshot.units.length,0);
assert.equal(snapshot.input,undefined);
console.log('PASS: transient null original state yields an explicit non-actionable snapshot.');
