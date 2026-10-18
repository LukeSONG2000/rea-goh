import fs from 'node:fs';
import vm from 'node:vm';
import assert from 'node:assert/strict';
const object=(fields={})=>Object.assign({isNull:()=>false,equals(other){return other===this;}},fields);
let hook,watchdog,nativeMoves=0,watchdogCalls=0,manualMoves=0,completed=0,legal=true,started=true,reverted=0,now=1000;
const retained=new Set();
const unit=object({_unitDO:object({Id:'enemy-1'}),_attackAbilityContext:object({'<AbilityDO>k__BackingField':object({get_Id:()=> 'original-basic'})}),
 get_Owner:()=>1,CanPerformMove:()=>legal,PerformManualMove:()=>{manualMoves++;return started;}});
const fsm=object({'<TurnCount>k__BackingField':2,_previousNonIdleState:34,_nextState:1});
const context=object({get_CurrentUnit:()=>unit,get_BattleFsmController:()=>fsm});
const state=object(),stateController=object({'<CurrentState>k__BackingField':state});
const provider=object({_battleContext:context}),callback=object({Invoke:()=>completed++});
const sandbox={Process:{id:1},Date:{now:()=>now},SceneApi:{string:x=>x},
 NativeFunction:function(a){return ()=>a==='DoMove'?nativeMoves++:watchdogCalls++;},NativeCallback:function(fn){return fn;},
 Interceptor:{replace(a,fn){if(a==='DoMove')hook=fn;else watchdog=fn;},flush(){},revert(){reverted++;}},
 RuntimeApi:{findClass:x=>x,classOf:x=>x,findMethod:(c,name)=>({name,readPointer:()=>name}),
  readRef:(p,k)=>p[k],readValue:(p,k)=>p[k],invoke:(m,p,args)=>p[m.name](...args),unbox:x=>x,
  retain(p){const h={p};retained.add(h);return h;},release(h){assert(retained.delete(h));},arg:{value:(t,x)=>x}},
 visualProbe:{fixture:{context,state:stateController},entered:state}};
vm.createContext(sandbox);
vm.runInContext(fs.readFileSync(new URL('./enemy_input_gate.js',import.meta.url),'utf8'),sandbox);
const gate=sandbox.installEnemyInputGate(context);
hook(object({_battleContext:object()}),unit,callback,null);
assert.equal(nativeMoves,1,'foreign context forwards to original');
hook(provider,unit,callback,null);
const token=gate.snapshot().token;
watchdog(fsm,null);assert.equal(watchdogCalls,0);assert.equal(gate.watchdogDeferrals,1);
watchdog(object(),null);assert.equal(watchdogCalls,1);
assert.equal(retained.size,3);assert.equal(nativeMoves,1);
assert.throws(()=>gate.chooseBasic('stale'),/Stale/);assert.equal(manualMoves,0);
now=11000;assert.throws(()=>gate.chooseBasic(token),/expired/);assert.equal(manualMoves,0);now=1000;
legal=false;assert.throws(()=>gate.chooseBasic(token),/CanPerformMove/);assert.equal(manualMoves,0);
legal=true;started=false;assert.throws(()=>gate.chooseBasic(token),/did not start/);
assert.equal(gate.snapshot().pending,true);assert.equal(retained.size,3);
stateController['<CurrentState>k__BackingField']=object();
assert.throws(()=>gate.resume(token),/ownership changed/);
stateController['<CurrentState>k__BackingField']=state;
started=true;assert.equal(gate.chooseBasic(token).started,true);
assert.equal(completed,1);assert.equal(retained.size,0);assert.equal(gate.snapshot().pending,false);
assert.throws(()=>gate.chooseBasic(token),/No owned/);
hook(provider,unit,callback,null);hook(provider,unit,callback,null);
assert.equal(nativeMoves,1,'duplicate must not dispatch a second move');assert.equal(retained.size,3);
assert.throws(()=>gate.stop(),/Exit owned/);assert.equal(reverted,0);
stateController['<CurrentState>k__BackingField']=object();gate.stop();gate.stop();
assert.equal(reverted,2);assert.equal(retained.size,0);
console.log('PASS: foreign context, stale token, native legality/rejection, ownership, completion, duplicate and cleanup gates.');
