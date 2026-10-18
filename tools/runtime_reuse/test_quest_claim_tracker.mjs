import assert from 'node:assert/strict';
import vm from 'node:vm';
import fs from 'node:fs';
const source=fs.readFileSync(new URL('./runtime_observer.js',import.meta.url),'utf8');
const part=source.slice(source.indexOf('globalThis.QuestClaimTracker='),source.indexOf('globalThis.ObjectivesBridge='));
const nil={isNull:()=>true};let serial=0;
const obj=(type,fields={})=>{const id=String(++serial);return {type,fields,isNull:()=>false,toString:()=>id,equals(other){return other===this;}};};
const list=id=>obj('List',{_size:1,_items:{isNull:()=>false,add:n=>n===24?{readU64:()=>({toNumber:()=>1})}:{readPointer:()=>id}}});
const hooks=new Map(),retained=new Set();let detaches=0;
const R={readRef:(o,k)=>o.fields[k]??nil,readValue:(o,k)=>o.fields[k],fullName:x=>x,classOf:o=>o.type,
 findClass:x=>x,findMethod:(c,m,n)=>({readPointer:()=>m}),retain:o=>{retained.add(o);return o;},release:o=>retained.delete(o)};
const ctx=vm.createContext({RuntimeApi:R,SceneApi:{string:x=>x},Interceptor:{attach(address,callbacks){hooks.set(address,callbacks);return {detach(){hooks.delete(address);detaches++;}};}}});
vm.runInContext(part,ctx);const tracker=ctx.QuestClaimTracker;
assert.equal(tracker.status().claims.length,0);assert.equal(hooks.size,3);
const owner=obj('Controller'),foreignOwner=obj('Controller');
const closure=who=>obj('ObjectivesQuestSubController+<>c__DisplayClass17_0',{'<>4__this':who});
const delegate=target=>obj('Delegate',{m_target:target});
function entry(id,target,failTarget=target){hooks.get('ClaimQuestsAndHeadlines').onEnter([obj('Episode'),list(id),nil,delegate(target),delegate(failTarget)]);}
function outcome(target,success=true){const h=hooks.get(success?'<CallClaimQuestsAndHeadlines>b__0':'<CallClaimQuestsAndHeadlines>b__1'),call={};h.onEnter.call(call,[target]);h.onLeave.call(call);}
const outside=closure(owner);entry('q',outside);assert.equal(retained.size,0,'unowned user calls ignored');
const own=closure(owner),foreign=closure(foreignOwner);
tracker.submit({questId:'q',tab:2,subTab:1},owner,()=>{entry('q',foreign);entry('other',own);assert.equal(retained.size,0);entry('q',own);});
assert.equal(tracker.status().claims[0].nativeTransactionObserved,true);assert.equal(retained.size,1);
outcome(foreign);assert.equal(tracker.status().claims[0].outcome,'pending');
outcome(own);assert.equal(tracker.status().claims[0].outcome,'native-success-callback');assert.equal(retained.size,0);assert.equal(tracker.status().claims[0].rewardConfirmed,false);
outcome(own,false);assert.equal(tracker.status().claims[0].outcome,'native-success-callback','late duplicate cannot change result');
const failed=closure(owner);tracker.submit({questId:'failed'},owner,()=>entry('failed',failed));outcome(failed,false);assert.equal(tracker.status().claims[1].outcome,'native-failure-callback');
assert.throws(()=>tracker.submit({questId:'uncertain'},owner,()=>{throw Error('native error');}),/native error/);assert.match(tracker.status().claims[2].invocationError,/native error/);
const pending=closure(owner);tracker.submit({questId:'pending'},owner,()=>entry('pending',pending));assert.equal(retained.size,1);
const final=tracker.dispose();assert.equal(final.trackingStopped,true);assert.equal(final.claims[3].outcome,'pending');assert.equal(retained.size,0);assert.equal(detaches,3);
console.log('PASS quest claim tracker: exact closure/owner/id correlation, native outcomes, no fabricated reward confirmation, roots released.');
