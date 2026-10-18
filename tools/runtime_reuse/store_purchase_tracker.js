'use strict';
// Observe original success callbacks only; never invoke a purchase or callback.
globalThis.StorePurchaseTracker=(()=>{
 const R=RuntimeApi,S=SceneApi,hooks=[];let record=null,owner=null,item=null,handles=[],installed=false;
 const ref=(o,n)=>R.readRef(o,`<${n}>k__BackingField`);
 const text=(o,n)=>S.string(ref(o,n));
 function release(){for(const h of handles)R.release(h);handles=[];owner=null;item=null;}
 function buckets(result){
  const l=ref(result,'CreditedBucketItems');if(l.isNull())return null;
  const n=R.readValue(l,'_size','i32'),a=R.readRef(l,'_items');
  if(n<0||n>4096||(a.isNull()?n!==0:n>a.add(24).readU64().toNumber()))throw Error('Purchase reward bounds');
  return Array.from({length:n},(_,i)=>{
   const b=a.add(32+i*8).readPointer();if(b.isNull())throw Error('Null purchase reward');
   return {id:text(b,'ID'),itemType:R.readValue(b,'<ItemType>k__BackingField','u8'),quantity:R.readValue(b,'<Quantity>k__BackingField','i32')};
  });
 }
 function observe(args,source){
  if(!record||record.nativeSuccessObserved||!owner||!args[0].equals(owner)||!args[2].equals(item))return;
  try{
   if(args[1].isNull()||text(args[1],'ItemId')!==record.itemId)return;
   record.nativeSuccessObserved=true;record.callbackSource=source;record.observedAt=Date.now();
   record.creditedBucketItems=buckets(args[1]);
  }catch(e){record.trackingError=String(e);}
  finally{if(record.nativeSuccessObserved)release();}
 }
 function install(){
  if(installed)return;
  try{
   for(const [t,m] of [['BaseStoreController','OnItemPurchaseSuccessful'],['FixedStoreController','ItemPurchaseSuccessful']]){
    const address=R.findMethod(R.findClass(t),m,3).readPointer();
    hooks.push(Interceptor.attach(address,{onEnter(args){observe(args,t+'.'+m);}}));
   }
   Interceptor.flush();installed=true;
  }catch(e){for(const h of hooks.splice(0))h.detach();throw e;}
 }
 function prepare(c,i,id,baseline=null){
  if(record)throw Error('Native store observer already has a submission');
  install();
  try{handles.push(R.retain(c));handles.push(R.retain(i));}catch(e){release();throw e;}
  owner=c;item=i;record={itemId:id,materialBefore:baseline?JSON.parse(JSON.stringify(baseline)):null,nativeSuccessObserved:false,creditedBucketItems:null,trackingError:null,materialRewardsReconciled:false,settlementConfirmed:false};
 }
 function reconcile(){
  if(!record?.nativeSuccessObserved||record.trackingError||record.materialRewardsReconciled)return;
  const rewards=record.creditedBucketItems,before=record.materialBefore;
  if(!before?.readConfirmed||!Array.isArray(before.materials)||!Array.isArray(rewards)||!rewards.length){record.comparisonUnavailable='Missing original rewards or pre-claim baseline';return;}
  if(rewards.some(r=>r.itemType!==7||!r.id||!Number.isSafeInteger(r.quantity)||r.quantity<=0)){record.comparisonUnavailable='Unsupported or invalid reward type/quantity';return;}
  try{
   const after=InboxClaimTracker.readBalances(rewards);
   record.materialComparison=after.map(a=>{
    const matches=before.materials.filter(b=>b.id===a.id&&b.itemType===7);
    const value=matches.length===1&&Number.isSafeInteger(matches[0].value)&&matches[0].value>0?matches[0].value:null;
    return {id:a.id,itemType:a.itemType,before:value,after:a.value,expected:a.expected,delta:value!==null&&Number.isSafeInteger(a.value)?a.value-value:null};
   });
   record.materialRewardsReconciled=after.length===new Set(rewards.map(r=>r.id)).size&&record.materialComparison.every(r=>r.itemType===7&&Number.isSafeInteger(r.expected)&&r.expected>0&&r.delta===r.expected);
   record.comparisonUnavailable=null;
   if(record.materialRewardsReconciled)record.materialReconciledAt=Date.now();
  }catch(e){record.comparisonUnavailable=String(e);}
 }
 function status(){reconcile();return {installed,record:record?{...record}:null,scope:'Native success and credited material delta only; not proof of other reward categories, transaction ID, or authorization to retry'};}
 function dispose(){for(const h of hooks.splice(0))h.detach();installed=false;release();return status();}
 return {install,prepare,status,dispose};
})();
rpc.exports.storeclaimdispose=()=>RuntimeApi.runOnMainThreadOnce(()=>StorePurchaseTracker.dispose());

