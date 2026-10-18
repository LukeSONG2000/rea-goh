'use strict';
// Read original visible catalog only. No item-button, refresh or purchase calls.
globalThis.StoreBridge=(()=>{
 const R=RuntimeApi,S=SceneApi,A=R.arg;
 const field=n=>`<${n}>k__BackingField`;
 const val=(o,n,t='i32')=>R.readValue(o,field(n),t);
 const str=(o,n)=>S.string(R.readRef(o,field(n)));
 function array(a){if(a.isNull())return [];const n=a.add(24).readU64().toNumber();if(n>4096)throw Error('Store array bounds');return Array.from({length:n},(_,i)=>a.add(32+i*8).readPointer());}
 function list(o){if(o.isNull())throw Error('Store list not ready');const n=R.readValue(o,'_size','i32'),all=array(R.readRef(o,'_items'));if(n<0||n>all.length)throw Error('Store list bounds');return all.slice(0,n);}
 const current=()=>R.readRef(R.findBound('StateController'),'<CurrentState>k__BackingField');
 const name=o=>R.fullName(R.classOf(o));
 function live(){
  const state=current();if(name(state)!=='StoreState')throw Error('Store state unavailable');
  const c=R.readRef(state,'_fixedStoreController'),v=R.readRef(c,'_storePackAccordionView');
  if(v.isNull()||!R.readValue(c,'_tabInteractionReady','bool')||R.readValue(c,'_showingSubview','bool')||!R.unbox(S.invoke('UnityEngine.GameObject','get_activeInHierarchy',S.gameObject(v)),'bool'))throw Error('Store catalog unavailable or busy');
  return {c,v};
 }
 function offer(o,formatter){
  if(o.isNull())throw Error('Null store offer');
  const productId=S.string(R.readRef(o,'_productId')),webProductId=S.string(R.readRef(o,'_webProductId'));
  const currencyType=R.readValue(o,'_currencyType','i32');
  let formattedPrice=null,priceSource='unavailable',priceError=null;
  try{
   const cached=currencyType!==4||!!R.unbox(S.invoke('StoreOfferPriceFormatter','CanFormatOffer',formatter,[A.ref(o)],['OfferDO']),'bool');
   formattedPrice=S.string(S.invoke('StoreOfferPriceFormatter','Format',formatter,[A.ref(o)],['OfferDO']));
   if(formattedPrice)priceSource=currencyType===4?(cached?'cached-commerce-product':'original-fallback-unconfirmed'):'original-currency-formatter';
  }catch(e){priceError=String(e);}
  return {formattedPrice,priceSource,priceConfirmed:priceSource==='cached-commerce-product'||priceSource==='original-currency-formatter',priceError,currencyType,rawPrice:R.readValue(o,'_price','i32'),productId,webProductId,costKey:S.string(R.readRef(o,'_costKey')),requiresProductPricing:!!(productId||webProductId)};
 }
 function tabs(v){
  return list(R.readRef(R.readRef(v,'_storeAccordion'),'_panels')).map((panel,index)=>{
   const button=R.readRef(panel,'_panelButton'),d=R.readRef(button,field('RenderData')),context=R.readRef(d,'ActionContext');
   if(context.isNull()||name(context)!=='StoreLocation')throw Error('Unsupported store tab context');
   return {button,index,tab:R.unbox(context,'i32'),label:S.string(R.readRef(d,'Label')),locked:R.readValue(d,'Locked','bool')};
  });
 }
 function status(){
  const {v}=live(),formatter=R.readRef(v,'_storeOfferPriceFormatter'),items=list(R.readRef(v,'_storeItemData')),render=list(R.readRef(v,'_storeItemRenderData')),seen=new Set();
  const rows=render.map(d=>{
   const index=val(d,'ItemIndex');if(index<0||index>=items.length||seen.has(index))throw Error('Store item mapping invalid');seen.add(index);
   const item=items[index];if(item.isNull())throw Error('Null store item');
   return {itemIndex:index,id:str(item,'Id'),nameKey:str(d,'NameKey'),quantity:val(d,'Quantity'),possiblePurchases:val(d,'PossiblePurchases'),maxPurchaseLimit:val(d,'MaxPurchaseLimit'),maxPurchasePerTransaction:val(item,'MaxPurchasePerTransaction'),locked:val(d,'IsLocked','bool'),freeOffer:val(d,'IsFreeOffer','bool'),showPrice:val(d,'ShowPrice','bool'),overrideDisplayPrice:str(d,'OverrideDisplayPrice'),offers:array(R.readRef(d,field('Offers'))).map(o=>offer(o,formatter))};
  });
  return {readConfirmed:true,storeTab:R.readValue(v,'_currentStoreTab','i32'),items:rows,tabs:tabs(v).map(({button,...row})=>row),purchaseAuthorized:false,note:'Original offer formatting; fallback prices remain unconfirmed. Per-item overrideDisplayPrice may override offer text; this is not a checkout authorization. PossiblePurchases is the original eligibility count, not physical stock.'};
 }
 function execute(op,p={}){
  if(op==='store-list')return status();
  if(op==='store-open'){
   const state=current();if(name(state)!=='HomeBaseGameState')throw Error('Open store from Home');
   S.invoke('HomeBaseController','OpenStore',R.readRef(state,'_homeBaseController'),[A.value('i32',0)],['StoreLocation']);
   return {navigationDispatched:true,expectedState:'StoreState'};
  }
  if(op==='store-tab'){
   if(!Number.isInteger(p.tab))throw Error('Explicit store tab required');
   const {v}=live(),matches=tabs(v).filter(t=>t.tab===p.tab);
   if(matches.length!==1||matches[0].locked)throw Error('Store tab missing, ambiguous or locked');
   const unchanged=R.readValue(v,'_currentStoreTab','i32')===p.tab;
   if(!unchanged)S.invoke('AccordionPanelItem','HandleButtonClick',matches[0].button);
   return {navigationDispatched:!unchanged,alreadySelected:unchanged,expectedTab:p.tab};
  }
  if(op==='store-back'){const {v}=live();S.invoke('HudBaseView','OnBackClick',v);return {navigationDispatched:true,expectedState:'HomeBaseGameState'};}
  throw Error('Unsupported store operation');
 }
 return {execute,status};
})();
rpc.exports.storecommand=(op,p)=>RuntimeApi.runOnMainThreadOnce(()=>StoreBridge.execute(op,p));
