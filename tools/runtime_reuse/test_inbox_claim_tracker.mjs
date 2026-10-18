import assert from 'node:assert/strict';
import fs from 'node:fs';
import vm from 'node:vm';
const text=fs.readFileSync(new URL('./runtime_observer.js',import.meta.url),'utf8');
const source=text.slice(text.indexOf('globalThis.InboxClaimTracker='),text.indexOf('// Observe only closures captured'));
let seq=0;const nil={isNull:()=>true};
const obj=(type,fields={})=>({type,fields,id:++seq,isNull:()=>false,equals(o){return o===this;},toString(){return String(this.id);}});
const hooks=new Map(),pendingHooks=new Map(),roots=new Set();let flushes=0;
const ctx=vm.createContext({RuntimeApi:{readRef:(o,k)=>o.fields[k]??nil,classOf:o=>o.type,fullName:x=>x,findClass:x=>x,
 findMethod:(t,m)=>({readPointer:()=>m}),retain:o=>{roots.add(o);return o;},release:o=>roots.delete(o)},SceneApi:{string:x=>x},
 Interceptor:{attach:(p,h)=>{pendingHooks.set(p,h);return {detach:()=>{hooks.delete(p);pendingHooks.delete(p);}};},flush:()=>{for(const [p,h] of pendingHooks)hooks.set(p,h);pendingHooks.clear();flushes++;}}});
vm.runInContext(source,ctx);const t=ctx.InboxClaimTracker;t.install();
assert.equal(flushes,1);assert.equal(hooks.size,3,'Observers must be active before first immediate native dispatch');
const owner=obj('InboxController'),other=obj('InboxController');
const closure=(who,id)=>obj('InboxController+<>c__DisplayClass49_0',{'<>4__this':who,messageDO:obj('Message',{'<Id>k__BackingField':id})});
function entry(id,c){const delegate=obj('Action',{m_target:c});hooks.get('UpdateInbox').onEnter([nil,id,{toInt32:()=>2},nil,nil,delegate,delegate,nil]);}
function finish(c,ok){const h=hooks.get(ok?'<UpdateInbox>b__0':'<UpdateInbox>b__1'),call={};h.onEnter.call(call,[c]);h.onLeave.call(call);}
const own=closure(owner,'m'),foreign=closure(other,'m');entry('m',own);assert.equal(roots.size,0);
t.submit({messageId:'m'},owner,()=>{entry('m',foreign);entry('wrong',own);assert.equal(roots.size,0);entry('m',own);});
assert.equal(roots.size,1);finish(foreign,true);assert.equal(t.status().claims[0].outcome,'pending');
finish(own,true);assert.equal(t.status().claims[0].outcome,'native-success-callback');assert.equal(roots.size,0);
assert.equal(t.status().claims[0].rewardBalanceReconciled,false);
const failed=closure(owner,'f');t.submit({messageId:'f'},owner,()=>entry('f',failed));finish(failed,false);
assert.equal(t.status().claims[1].outcome,'native-failure-callback');
const delayed=closure(owner,'later');t.submit({messageId:'later'},owner,()=>{});entry('later',delayed);finish(delayed,true);
assert.equal(t.status().claims[2].outcome,'native-success-callback','Deferred exact transaction is correlated without active dispatch');
const pending=closure(owner,'p');t.submit({messageId:'p'},owner,()=>entry('p',pending));
const callbackOnly=closure(owner,'async');t.submit({messageId:'async'},owner,()=>{});
finish(closure(other,'async'),true);assert.equal(t.status().claims[4].outcome,'pending');
finish(callbackOnly,true);assert.equal(t.status().claims[4].outcome,'native-success-callback');
assert.equal(t.status().claims[4].nativeTransactionObserved,false,'Callback does not fabricate service-entry observation');
assert.equal(t.status().claims[4].nativeCallbackObserved,true);
assert.equal(t.dispose().claims[3].outcome,'pending');assert.equal(roots.size,0);assert.equal(hooks.size,0);
console.log('Inbox tracker PASS: exact ID/owner/closure, callbacks, pending cleanup, no fabricated balance confirmation');
