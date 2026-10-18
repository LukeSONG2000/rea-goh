import assert from 'node:assert/strict';
import {compareStoreWallet as compare} from './store_receipt.mjs';
const wallet=[{id:'PREMIUM',itemType:2,value:100},{id:'SOCIAL',itemType:2,value:300}];
const claim={id:'request-A',pid:123,itemId:'bronze',quantity:1,expectedCost:0};
const original={id:claim.id,dispatched:true,result:{submission:{itemId:'bronze',quantity:1,expectedCost:0,walletBefore:wallet}}};
const current={walletAfter:structuredClone(wallet)};
assert.equal(compare(claim,original,current,123).unchanged,true);
current.walletAfter[1].value=50;
assert.equal(compare(claim,original,current,123).unchanged,false);
assert.equal(compare(claim,original,current,123).rows[1].delta,-250);
assert.equal(compare(claim,original,current,456).reason,'client-process-changed');
assert.equal(compare(claim,{...original,id:'request-B'},current,123).confirmed,false);
for(const mutate of [s=>delete s.walletBefore,s=>s.walletBefore.pop(),s=>s.walletBefore[0].value=null,s=>s.walletBefore[0].value=-1,s=>s.walletBefore[0].id='SOCIAL',s=>s.itemId='other',s=>s.quantity=2]){
 const bad=structuredClone(original);mutate(bad.result.submission);
 assert.equal(compare(claim,bad,current,123).unchanged,null);
}
assert.equal(compare(claim,original,{walletAfter:[]},123).confirmed,false);
assert.equal('settlementConfirmed' in compare(claim,original,current,123),false);
console.log('PASS durable wallet match, changed balance, client/command/item mismatch, missing/duplicate/invalid baseline, no settlement assertion');
