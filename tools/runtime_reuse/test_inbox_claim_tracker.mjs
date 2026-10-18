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
// Inventory comparisons exercise the production tracker with original getter signatures.
const balances=new Map([['2:PREMIUM',100],['7:ship-xp-mat-four-star',20]]),calls=[];
Object.assign(ctx.RuntimeApi,{nil,findBound:()=>owner,string:x=>x,unbox:x=>x,arg:{ref:x=>x,value:(_,x)=>x}});
Object.assign(ctx.SceneApi,{type:x=>x,invoke:(type,method,instance,args,signature)=>{
 calls.push({type,method,args,signature});
 if(method==='Parse')return args[1];
 if(method==='GetCurrencyQuantity')return balances.get('2:'+args[0]);
 if(method==='GetCraftingMaterialQuantity')return balances.get('7:'+args[0]);
 throw Error('Unexpected method '+method);
}});
const rewards=[{itemType:2,id:'PREMIUM',quantity:5},{itemType:7,id:'ship-xp-mat-four-star',quantity:2},{itemType:7,id:'ship-xp-mat-four-star',quantity:2}];
const before=t.readBalances(rewards);assert.equal(before.length,2);assert.equal(before[1].expected,4);
assert.equal(calls.find(c=>c.method==='GetCraftingMaterialQuantity').signature[0],'System.String');
t.submit({messageId:'mixed',rewards,balanceBefore:before},owner,()=>{});
balances.set('2:PREMIUM',105);
let mixed=()=>t.status().claims.find(r=>r.messageId==='mixed');
assert.equal(mixed().rewardBalanceReconciled,false,'Partial currency delivery is not full mixed delivery');
balances.set('7:ship-xp-mat-four-star',24);
assert.equal(mixed().rewardBalanceReconciled,true);assert.equal(mixed().settlementEvidence,'native-inventory-delta');
assert.equal(mixed().nativeCallbackObserved,undefined,'Inventory evidence does not fabricate a callback');
const unsupported=[...rewards,{itemType:1,id:'PREMIUM',quantity:1}];
const unknown=t.readBalances(unsupported);assert.equal(unknown.length,3,'Same ID with different type stays distinct');
t.submit({messageId:'unknown',rewards:unsupported,balanceBefore:unknown},owner,()=>{});
balances.set('2:PREMIUM',110);balances.set('7:ship-xp-mat-four-star',28);
const unknownStatus=t.status().claims.find(r=>r.messageId==='unknown');
assert.equal(unknownStatus.rewardBalanceReconciled,false);assert.equal(unknownStatus.balanceComparison[2].delta,null);
assert.throws(()=>t.readBalances([{itemType:7,id:'bad',quantity:-1}]),/Invalid/);
assert.throws(()=>t.readBalances([{itemType:7,id:'bad',quantity:Number.MAX_SAFE_INTEGER},{itemType:7,id:'bad',quantity:1}]),/overflow/);
const invoke=ctx.SceneApi.invoke;ctx.SceneApi.invoke=()=>{throw Error('Native read failed');};
assert.equal(t.readBalances(rewards)[0].value,null);ctx.SceneApi.invoke=invoke;
t.dispose();assert.equal(hooks.size,0);
console.log('Inbox tracker PASS: exact ID/owner/closure, callbacks, pending cleanup, no fabricated balance confirmation');
