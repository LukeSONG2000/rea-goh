import fs from 'node:fs';
import vm from 'node:vm';
import assert from 'node:assert/strict';
const object=(fields={})=>Object.assign({isNull:()=>false,equals(other){return other===this;}},fields);
let hook,watchdog,nativeMoves=0,watchdogCalls=0,manualMoves=0,completed=0,legal=true,started=true,reverted=0,now=1000;
const retained=new Set();
const nil={isNull:()=>true};
const target=id=>object({_unitDO:object({Id:id,'<Definition>k__BackingField':object({BaseId:id})})});
const targets=[target('ally-1'),target('ally-2')];let selected=targets[0],manual=nil,targetCalls=0;
const targetController=object({GetValidTargets:()=>({_size:targets.length,_items:targets}),GetCurrentTarget:()=>selected,get_ManualTarget:()=>manual,
 CalculateAndSetCurrentTarget:(unused,preferred)=>{targetCalls++;selected=preferred;return preferred;}});
const ability=(id,ally='')=>object({'<CooldownCounter>k__BackingField':0,get_IsAbilityReady:()=>true,'<AbilityDO>k__BackingField':object({get_Id:()=>id,get_AllyTargetingRuleId:()=>ally,get_AbilityType:()=>0})});
const specials=[ability('original-special'),ability('ally-special','original-ally-rule')];
const unit=object({_unitDO:object({Id:'enemy-1'}),_attackAbilityContext:ability('original-basic'),_limitBreakAbilityContexts:{_size:2,_items:specials},
 _targetingController:targetController,
 get_Owner:()=>1,CanPerformMove:()=>legal,PerformManualMove:()=>{manualMoves++;unit['<EnemyTarget>k__BackingField']=selected;return started;}});
let noCandidates=false,cancelCalls=0;
const selector=object({_completed:nil,_units:nil,get_CurrentlyPerformingUnitSelection:()=>!selector._completed.isNull(),CancelSelectUnit:()=>{cancelCalls++;selector._completed=nil;selector._units=nil;},HandleUnitTapped:id=>{unit['<AllyTarget>k__BackingField']=targets.find(t=>t._unitDO.Id===id);selector._completed=nil;selector._units=nil;}});
const fsm=object({'<TurnCount>k__BackingField':2,_previousNonIdleState:34,_nextState:1});
fsm.SelectAlly=()=>{selector._completed=noCandidates?nil:object();selector._units=noCandidates?nil:{_size:targets.length,_items:targets};unit['<AllyTarget>k__BackingField']=nil;};
const context=object({'<BattleManualUnitSelectController>k__BackingField':selector,get_CurrentUnit:()=>unit,get_BattleFsmController:()=>fsm});
const state=object(),stateController=object({'<CurrentState>k__BackingField':state});
const provider=object({_battleContext:context}),callback=object({Invoke:()=>completed++});
const sandbox={Process:{id:1},Date:{now:()=>now},SceneApi:{string:x=>x,array:x=>x},
 NativeFunction:function(a){return ()=>a==='DoMove'?nativeMoves++:watchdogCalls++;},NativeCallback:function(fn){return fn;},
 Interceptor:{replace(a,fn){if(a==='DoMove')hook=fn;else watchdog=fn;},flush(){},revert(){reverted++;}},
 RuntimeApi:{nil,string:x=>x,findClass:x=>x,classOf:x=>x,findMethod:(c,name)=>({name,readPointer:()=>name}),
  readRef:(p,k)=>p[k],readValue:(p,k)=>p[k],invoke:(m,p,args)=>p[m.name](...args),unbox:x=>x,
  retain(p){const h={p};retained.add(h);return h;},release(h){assert(retained.delete(h));},arg:{ref:x=>x,value:(t,x)=>x}},
 visualProbe:{fixture:{context,state:stateController},entered:state}};
vm.createContext(sandbox);
vm.runInContext(fs.readFileSync(new URL('./enemy_input_gate.js',import.meta.url),'utf8'),sandbox);
const gate=sandbox.installEnemyInputGate(context);
hook(object({_battleContext:object()}),unit,callback,null);
assert.equal(nativeMoves,1,'foreign context forwards to original');
hook(provider,unit,callback,null);
const token=gate.snapshot().token;
assert.equal(gate.snapshot().abilities.length,3);
specials[0].get_IsAbilityReady=()=>false;
assert.throws(()=>gate.chooseAbility({token,moveType:1,index:0}),/readiness/);
specials[0].get_IsAbilityReady=()=>true;
assert.throws(()=>gate.chooseAbility({token,moveType:1,index:2}),/absent/);

specials[0]['<AbilityDO>k__BackingField'].get_AbilityType=()=>1;
assert.equal(gate.snapshot().abilities[1].supported,false);
assert.throws(()=>gate.chooseAbility({token,moveType:1,index:0}),/unsupported ability type/);
specials[0]['<AbilityDO>k__BackingField'].get_AbilityType=()=>0;
assert.throws(()=>gate.chooseAbility({token,moveType:2,index:0}),/Invalid/);
assert.equal(gate.snapshot().targets.length,2);
assert.throws(()=>gate.selectTarget({token,targetId:'illegal'}),/legal target list/);assert.equal(targetCalls,0);
assert.throws(()=>gate.selectTarget({token:'stale',targetId:'ally-2'}),/Stale/);assert.equal(targetCalls,0);
assert.equal(gate.selectTarget({token,targetId:'ally-2'}).targetConfirmed,true);
assert.equal(gate.selectTarget({token,targetId:'ally-2'}).alreadySelected,true);assert.equal(targetCalls,1);
assert.equal(gate.snapshot().selectedTarget,'ally-2');
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
hook(provider,unit,callback,null);
const specialResult=gate.chooseAbility({token:gate.snapshot().token,moveType:1,index:0});
assert.equal(specialResult.abilityId,'original-special');assert.equal(specialResult.moveType,1);assert.equal(completed,2);
// Selection rotates tokens; invalid IDs cannot consume the pending move.
hook(provider,unit,callback,null);
let before=gate.snapshot().token;
let selectionResult=gate.chooseAbility({token:before,moveType:1,index:1});
assert.equal(selectionResult.selectionStarted,true);assert.notEqual(selectionResult.token,before);
const ownedCallback=selector._completed;selector._completed=object();
assert.throws(()=>gate.selectUnit({token:selectionResult.token,targetId:'ally-1'}),/ownership changed/);selector._completed=ownedCallback;
assert.throws(()=>gate.selectUnit({token:before,targetId:'ally-1'}),/Stale/);
assert.throws(()=>gate.selectUnit({token:selectionResult.token,targetId:'invalid'}),/candidates/);
assert.throws(()=>gate.chooseBasic(selectionResult.token),/Finish or cancel/);
assert.equal(gate.cancelSelection(selectionResult.token).selectionCancelled,true);assert.equal(cancelCalls,1);
assert.equal(gate.snapshot().selectingUnit,false);
noCandidates=true;
assert.equal(gate.chooseAbility({token:gate.snapshot().token,moveType:1,index:1}).selectionEmpty,true);
assert.equal(gate.snapshot().selectedAbility,null);noCandidates=false;
selectionResult=gate.chooseAbility({token:gate.snapshot().token,moveType:1,index:1});
const allyResult=gate.selectUnit({token:selectionResult.token,targetId:'ally-2'});
assert.equal(allyResult.allyTarget,'ally-2');assert.equal(allyResult.started,true);assert.equal(completed,3);
hook(provider,unit,callback,null);
selectionResult=gate.chooseAbility({token:gate.snapshot().token,moveType:1,index:1});
now=12000;assert.throws(()=>gate.selectUnit({token:selectionResult.token,targetId:'ally-2'}),/expired/);
assert.equal(gate.resume(selectionResult.token).resumed,true);assert.equal(cancelCalls,2);assert.equal(nativeMoves,2);now=1000;
sandbox.visualProbe.fixture.data={'<BattleTypeEnum>k__BackingField':0,'<CombatTypeEnum>k__BackingField':0};
const policyToken=gate.snapshot().policy.token;
assert.equal(gate.snapshot().policy.available,true);
sandbox.visualProbe.fixture.data['<BattleTypeEnum>k__BackingField']=1;
assert.equal(gate.snapshot().policy.available,false);
assert.throws(()=>gate.configurePolicy({token:policyToken,enabled:true,allowedTargets:['ally-1']}),/squad arena/);
sandbox.visualProbe.fixture.data['<BattleTypeEnum>k__BackingField']=0;
assert.throws(()=>gate.configurePolicy({token:'old',enabled:true,allowedTargets:['ally-1']}),/Stale/);
assert.throws(()=>gate.configurePolicy({token:policyToken,enabled:true,allowedTargets:[]}),/at least/);
gate.configurePolicy({token:policyToken,enabled:true,allowedTargets:['missing']});
hook(provider,unit,callback,null);
const movesBefore=manualMoves;
assert.equal(gate.tickPolicy(),true);assert.equal(manualMoves,movesBefore);assert.ok(gate.snapshot().policy.blocked);
gate.configurePolicy({token:policyToken,enabled:true,allowedTargets:['ally-1','ally-2']});
assert.equal(gate.tickPolicy(),true);assert.equal(manualMoves,movesBefore+1);assert.equal(gate.snapshot().pending,false);
assert.ok(['ally-1','ally-2'].includes(gate.events.at(-1).actualTarget));
gate.configurePolicy({token:policyToken,enabled:false,allowedTargets:[]});
hook(provider,unit,callback,null);gate.tickPolicy();assert.equal(nativeMoves,3);
hook(provider,unit,callback,null);hook(provider,unit,callback,null);
assert.equal(nativeMoves,3,'duplicate must not dispatch a second move');assert.equal(retained.size,3);
assert.throws(()=>gate.stop(),/Exit owned/);assert.equal(reverted,0);
stateController['<CurrentState>k__BackingField']=object();gate.stop();gate.stop();
assert.equal(reverted,2);assert.equal(retained.size,0);
console.log('PASS: foreign context, stale token, native legality/rejection, ownership, completion, duplicate and cleanup gates.');
