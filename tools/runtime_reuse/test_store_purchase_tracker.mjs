import fs from 'node:fs';
import vm from 'node:vm';
import assert from 'node:assert/strict';
const ptr=o=>{const p={...o,isNull:()=>false};p.equals=x=>x===p;return p;};
const nil={isNull:()=>true};
const array=rows=>ptr({add:n=>n===24?{readU64:()=>({toNumber:()=>rows.length})}:{readPointer:()=>rows[(n-32)/8]}});
const list=rows=>ptr({_size:rows.length,_items:array(rows)});
const backing=o=>Object.fromEntries(Object.entries(o).map(([k,v])=>[`<${k}>k__BackingField`,v]));
function setup(readBalances=()=>[]){
 const callbacks=new Map(),retained=new Set();let flushes=0,detaches=0;
 const read=(o,n)=>{if(!(n in o))throw Error('Missing '+n);return o[n];};
 const R={readRef:read,readValue:read,findClass:x=>x,findMethod:(t,m)=>({readPointer:()=>t+'.'+m}),retain:p=>{const h={p};retained.add(h);return h;},release:h=>retained.delete(h)};
 const ctx={InboxClaimTracker:{readBalances},RuntimeApi:R,SceneApi:{string:x=>x},rpc:{exports:{}},Interceptor:{attach:(p,c)=>{callbacks.set(p,c);return {detach(){detaches++;callbacks.delete(p);}};},flush(){flushes++;}}};
 vm.runInNewContext(fs.readFileSync(new URL('./store_purchase_tracker.js',import.meta.url),'utf8'),ctx);
 return {tracker:ctx.StorePurchaseTracker,callbacks,retained,get flushes(){return flushes;},get detaches(){return detaches;}};
}
const owner=ptr({}),item=ptr({}),other=ptr({});
const bucket=ptr(backing({ID:'material',ItemType:7,Quantity:2}));
const result=id=>ptr(backing({ItemId:id,CreditedBucketItems:list([bucket])}));
const env=setup();env.tracker.prepare(owner,item,'A');
assert.equal(env.flushes,1);assert.equal(env.retained.size,2);
const hook=env.callbacks.get('BaseStoreController.OnItemPurchaseSuccessful');
for(const args of [[other,result('A'),item],[owner,result('A'),other],[owner,result('B'),item],[owner,nil,item]]){
 hook.onEnter(args);assert.equal(env.tracker.status().record.nativeSuccessObserved,false);
}
hook.onEnter([owner,result('A'),item]);
assert.equal(env.tracker.status().record.nativeSuccessObserved,true);
assert.equal(env.tracker.status().record.creditedBucketItems[0].quantity,2);
assert.equal(env.tracker.status().record.settlementConfirmed,false);
assert.equal(env.retained.size,0);
env.callbacks.get('FixedStoreController.ItemPurchaseSuccessful').onEnter([owner,result('A'),item]);
assert.equal(env.tracker.status().record.callbackSource,'BaseStoreController.OnItemPurchaseSuccessful');
assert.throws(()=>env.tracker.prepare(owner,item,'A'),/already/);
env.tracker.dispose();assert.equal(env.detaches,2);
const bad=setup();bad.tracker.prepare(owner,item,'A');
const invalid=result('A');invalid['<CreditedBucketItems>k__BackingField']._size=99;
bad.callbacks.get('FixedStoreController.ItemPurchaseSuccessful').onEnter([owner,invalid,item]);
assert.equal(bad.tracker.status().record.nativeSuccessObserved,true);
assert.match(bad.tracker.status().record.trackingError,/bounds/);
assert.equal(bad.tracker.status().record.creditedBucketItems,null);
assert.equal(bad.retained.size,0);bad.tracker.dispose();
const pending=setup();pending.tracker.prepare(owner,item,'A');pending.tracker.dispose();assert.equal(pending.retained.size,0);
console.log('PASS original callback correlation, foreign/null/ID mismatch refusal, duplicate observation, bounded rewards, error containment and handle cleanup');


let afterValue=11;
const totals=rows=>{const m=new Map();for(const r of rows)m.set(r.id,(m.get(r.id)||0)+r.quantity);return [...m].map(([id,expected])=>({id,itemType:7,expected,value:afterValue}));};
const delta=setup(totals),baseline={readConfirmed:true,materials:[{id:'material',itemType:7,value:10}]};
delta.tracker.prepare(owner,item,'A',baseline);
baseline.materials[0].value=999; // Snapshot must not alias caller's baseline.
const doubled=result('A');doubled['<CreditedBucketItems>k__BackingField']=list([bucket,bucket]);
delta.callbacks.get('BaseStoreController.OnItemPurchaseSuccessful').onEnter([owner,doubled,item]);
assert.equal(delta.tracker.status().record.materialRewardsReconciled,false);
assert.equal(delta.tracker.status().record.materialComparison[0].expected,4);
assert.equal(delta.tracker.status().record.materialComparison[0].before,10);
afterValue=15;assert.equal(delta.tracker.status().record.materialRewardsReconciled,false);
afterValue=14;assert.equal(delta.tracker.status().record.materialRewardsReconciled,true);
afterValue=1;assert.equal(delta.tracker.status().record.materialRewardsReconciled,true,'Verified evidence remains historical after later spending');
assert.equal(delta.tracker.status().record.settlementConfirmed,false);delta.tracker.dispose();
for(const missing of [[],[{id:'material',itemType:7,value:10},{id:'material',itemType:7,value:10}]]){
 const e=setup(totals);e.tracker.prepare(owner,item,'A',{readConfirmed:true,materials:missing});
 e.callbacks.get('BaseStoreController.OnItemPurchaseSuccessful').onEnter([owner,result('A'),item]);
 assert.equal(e.tracker.status().record.materialRewardsReconciled,false);e.tracker.dispose();
}
const mixed=setup(totals);mixed.tracker.prepare(owner,item,'A',{readConfirmed:true,materials:[]});
const unsupported=result('A');unsupported['<CreditedBucketItems>k__BackingField']=list([ptr(backing({ID:'unit',ItemType:1,Quantity:1}))]);
mixed.callbacks.get('BaseStoreController.OnItemPurchaseSuccessful').onEnter([owner,unsupported,item]);
assert.match(mixed.tracker.status().record.comparisonUnavailable,/Unsupported/);mixed.tracker.dispose();
console.log('PASS duplicate rewards aggregated, partial/excess rejected, exact delta latched, copied baseline, missing/ambiguous baseline and unsupported reward refused');
