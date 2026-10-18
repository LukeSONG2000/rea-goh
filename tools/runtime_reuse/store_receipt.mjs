// Compare only a saved pre-dispatch wallet with a current read. This does not
// confirm reward settlement or authorize clearing the durable retry guard.
export function compareStoreWallet(claim,original,current,pid){
 const unknown=reason=>({confirmed:false,unchanged:null,reason,rows:[],scope:'Wallet comparison only; no reward settlement or retry authorization'});
 if(claim?.pid!==pid)return unknown('client-process-changed');
 if(original?.id!==claim?.id||original?.dispatched!==true)return unknown('original-command-unavailable-or-mismatched');
 const s=original.result?.submission;
 if(!s)return unknown('saved-submission-missing');
 if(s.itemId!==claim.itemId||s.quantity!==claim.quantity||s.expectedCost!==claim.expectedCost||claim.quantity!==1||claim.expectedCost!==0)return unknown('submission-identity-mismatch');
 const validate=rows=>Array.isArray(rows)&&rows.length===2&&['PREMIUM','SOCIAL'].every(id=>rows.filter(r=>r?.id===id&&r.itemType===2&&Number.isSafeInteger(r.value)&&r.value>=0).length===1);
 if(!validate(s.walletBefore))return unknown('pre-claim-wallet-unavailable');
 if(!validate(current?.walletAfter))return unknown('current-wallet-unavailable');
 const rows=s.walletBefore.map(b=>{const after=current.walletAfter.find(a=>a.id===b.id).value;return {id:b.id,itemType:2,before:b.value,after,delta:after-b.value};});
 return {confirmed:true,unchanged:rows.every(r=>r.delta===0),reason:null,sourceCommandId:claim.id,rows,scope:'Exact wallet snapshots; intervening activity can change balances. No reward settlement or retry authorization'};
}
