import assert from 'node:assert/strict';
import fs from 'node:fs';
import vm from 'node:vm';
const pointer=fields=>Object.assign({isNull:()=>false,equals(other){return this===other;}},fields);
const context=pointer({}),state=pointer({_battleController: {_battleContext:context}});
const controller={'<CurrentState>k__BackingField':state},calls=[];
const sandbox={rpc:{exports:{}},RuntimeApi:{runOnMainThreadOnce:fn=>fn(),readRef:(p,f)=>p[f]},
 visualProbe:{entered:state,fixture:{state:controller,context}},
 BattleInputBridge:Object.fromEntries(['choose','selectTarget','selectUnit','cancelSelection'].map(n=>[n,r=>{calls.push([n,r]);return {accepted:n};}]))};
vm.runInNewContext(fs.readFileSync(new URL('./trial_input.js',import.meta.url),'utf8'),sandbox);
const execute=sandbox.rpc.exports.trialcommand,request={token:'current'};
for(const [op,method] of [['battle-ability','choose'],['battle-target','selectTarget'],['battle-unit','selectUnit'],['battle-cancel','cancelSelection']]){
 assert.equal(execute(op,request).accepted,method);assert.equal(calls.at(-1)[1],request);
}
const count=calls.length;
assert.throws(()=>execute('campaign-home',request),/Only battle inputs/);
controller['<CurrentState>k__BackingField']=pointer({_battleController:{_battleContext:context}});
assert.throws(()=>execute('battle-ability',request),/not the owned trial/);
controller['<CurrentState>k__BackingField']=state;
state._battleController._battleContext=pointer({});
assert.throws(()=>execute('battle-ability',request),/context ownership mismatch/);
state._battleController._battleContext=context;
sandbox.visualProbe=null;
assert.throws(()=>execute('battle-ability',request),/not entered/);
assert.equal(calls.length,count);
console.log('PASS: existing bridge receives only four allowed inputs in the exact owned battle/context; rejected cases never dispatch.');
