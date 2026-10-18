import assert from 'node:assert/strict';
import fs from 'node:fs';
import vm from 'node:vm';
import {readSnapshotOrUnavailable} from './runtime_poll.mjs';
const source=fs.readFileSync(new URL('./runtime_observer.js',import.meta.url),'utf8');
const part=source.slice(source.indexOf('globalThis.readRuntimeSnapshot='),source.indexOf('globalThis.inventoryHome='));
const nil={isNull:()=>true};
const object=fields=>({fields,isNull:()=>false});
const handles=object({_items:nil,_size:0}),view=object({_unitReticles:handles}),context=object({}),controller=object({_battleContext:context}),state=object({_battleController:controller});
let currentView=view;
const sandbox={RuntimeApi:{runOnMainThreadOnce:fn=>fn(),findBound:()=>object({}),
 readRef:(o,k)=>{assert.notEqual(o,nil,'No null dereference');return k==='<CurrentState>k__BackingField'?state:o.fields[k];},
 readValue:(o,k)=>o.fields[k],classOf:()=> 'BattleGameState',fullName:x=>x},
 SceneApi:{invoke:()=>currentView},BattleInputBridge:{snapshot:()=>({awaitingAbility:true,token:'must-not-leak-during-loading'})}};
vm.runInNewContext(part,sandbox);
for(const stage of ['controller','context','view','reticles']){
 const owner=stage==='controller'?state:stage==='context'?controller:view;
 const field=stage==='controller'?'_battleController':stage==='context'?'_battleContext':'_unitReticles';
 const previous=owner.fields[field];
 if(stage==='view')currentView=nil;else owner.fields[field]=nil;
 const result=await sandbox.readRuntimeSnapshot();
 assert.equal(result.loadingStage,stage);assert.equal(result.battleReady,false);
 assert.equal(result.input.unavailable,true);assert.equal(result.input.token,undefined);
 if(stage==='view')currentView=view;else owner.fields[field]=previous;
}
assert.equal((await sandbox.readRuntimeSnapshot()).battleReady,true);
let reads=0;
const read=async()=>{if(++reads===1)throw Error('Transient native read');return {state:'HomeBaseGameState',units:[]};};
const failed=await readSnapshotOrUnavailable(read,123);
assert.equal(failed.state,'Unavailable');assert.equal(failed.input.unavailable,true);assert.equal(failed.units.length,0);
const recovered=await readSnapshotOrUnavailable(read,123);
assert.equal(recovered.state,'HomeBaseGameState');assert.equal(recovered.error,undefined);assert.equal(reads,2);
console.log('Runtime loading stages and polling recovery PASS');
