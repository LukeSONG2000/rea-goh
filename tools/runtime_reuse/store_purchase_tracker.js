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
 function prepare(c,i,id){
  if(record)throw Error('Native store observer already has a submission');
  install();
  try{handles.push(R.retain(c));handles.push(R.retain(i));}catch(e){release();throw e;}
  owner=c;item=i;record={itemId:id,nativeSuccessObserved:false,creditedBucketItems:null,trackingError:null,settlementConfirmed:false};
 }
 function status(){return {installed,record:record?{...record}:null,scope:'Same controller and item object plus result item ID; observes native success, not an inventory delta or transaction ID'};}
 function dispose(){for(const h of hooks.splice(0))h.detach();installed=false;release();return status();}
 return {install,prepare,status,dispose};
})();
rpc.exports.storeclaimdispose=()=>RuntimeApi.runOnMainThreadOnce(()=>StorePurchaseTracker.dispose());

