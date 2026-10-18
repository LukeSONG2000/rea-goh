import assert from 'node:assert/strict';
import fs from 'node:fs';
import vm from 'node:vm';

// Execute the actual UI module with no popup objects. A stale helper must
// never be consulted after returning home; stopping in battle must restore
// the current battle's original mode gate after detaching the override.
const source=fs.readFileSync(new URL('./battle_info_ui.js',import.meta.url),'utf8');
async function scenario(inBattle,originalEnabled){
 const nil={isNull:()=>true},helper={isNull:()=>false},hooks=new Map(),calls=[];
 const state={type:inBattle?'BattleGameState':'HomeBaseGameState',isNull:()=>false};let current=state;
 const controller={},context={};let detached=0;
 const R={arg:{value:(t,v)=>v,ref:v=>v},nil,
  runOnMainThreadOnce:async fn=>fn(),findBound:()=>({}),
  readRef:(o,k)=>k==='<CurrentState>k__BackingField'?current:k==='_battleController'?controller:k==='_battleContext'?context:assert.fail(k),
  fullName:x=>x,classOf:o=>o.type,unbox:x=>x,
  findClass:x=>x,findMethod:(c,m)=>({readPointer:()=>c+'.'+m})};
 const S={all:()=>[],invoke:(c,m,o,args)=>{
  calls.push({c,m,o,args});
  if(m==='get_BattleView')return {};
  if(m==='get_BattleOpponentAbilityInfoEnabledHelper'){assert.equal(o,context);return helper;}
  if(m==='IsEnabled'){assert.equal(o,helper);assert.equal(detached,9);return originalEnabled;}
  if(m==='FilterOpponentAbilityDisplay')return;
  assert.fail(c+'.'+m);
 }};
 let timerCallback=null,timerCleared=false;
 const ctx=vm.createContext({RuntimeApi:R,SceneApi:S,rpc:{exports:{}},send:()=>{},
  setInterval(fn,ms){assert.equal(ms,300);timerCallback=fn;return 123;},
  clearInterval(id){assert.equal(id,123);timerCleared=true;},
  Interceptor:{attach(address,callbacks){hooks.set(address,callbacks);return {detach(){detached++;}};}}});
 vm.runInContext(source,ctx);await Promise.resolve();calls.length=0;
 current=nil;timerCallback();await Promise.resolve();await Promise.resolve();
 assert.equal(calls.length,0,'Transient null state must not touch native objects');current=state;
 const result=await ctx.stopBattleInfo();
 assert.equal(result.errors.length,0);assert.equal(detached,9);
 assert.equal(timerCleared,true);const countAfterStop=calls.length;
 timerCallback();await Promise.resolve();assert.equal(calls.length,countAfterStop,'Queued timer after stop must not touch native objects');
 if(!inBattle)assert.equal(calls.length,0,'Home cleanup must not dereference a previous battle');
 else assert.equal(calls.at(-1).args[0],originalEnabled,'Restore actual mode gate, including false');
}
await scenario(false,false);
await scenario(true,false);
await scenario(true,true);
console.log('Battle info cleanup lifecycle PASS (home, battle disabled, battle enabled)');
