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
   return {itemIndex:index,id:str(item,'Id'),itemType:val(item,'ItemType'),nameKey:str(d,'NameKey'),quantity:val(d,'Quantity'),possiblePurchases:val(d,'PossiblePurchases'),maxPurchaseLimit:val(d,'MaxPurchaseLimit'),maxPurchasePerTransaction:val(item,'MaxPurchasePerTransaction'),locked:val(d,'IsLocked','bool'),freeOffer:val(d,'IsFreeOffer','bool'),showPrice:val(d,'ShowPrice','bool'),overrideDisplayPrice:str(d,'OverrideDisplayPrice'),offers:array(R.readRef(d,field('Offers'))).map(o=>offer(o,formatter))};
  });
  return {readConfirmed:true,storeTab:R.readValue(v,'_currentStoreTab','i32'),items:rows,tabs:tabs(v).map(({button,...row})=>row),purchaseAuthorized:false,note:'Original offer formatting; fallback prices remain unconfirmed. Per-item overrideDisplayPrice may override offer text; this is not a checkout authorization. PossiblePurchases is the original eligibility count, not physical stock.'};
 }
 function details(){
  const state=current();if(name(state)!=='StoreState')throw Error('Store state unavailable');
  const c=R.readRef(state,'_fixedStoreController'),v=R.readRef(c,'_storePackDetailsView');
  if(v.isNull()||!R.unbox(S.invoke('UnityEngine.GameObject','get_activeInHierarchy',S.gameObject(v)),'bool'))throw Error('Store details not visible');
  const item=R.readRef(v,'_storeItem');if(item.isNull())throw Error('Store detail item missing');
  return {c,v,item};
 }
 const active=o=>!o.isNull()&&!!R.unbox(S.invoke('UnityEngine.GameObject','get_activeInHierarchy',S.gameObject(o)),'bool');
 function labelStatus(o){return o.isNull()?null:{active:active(o),text:S.string(S.invoke('UILabel','get_text',o))};}
 function buttonStatus(v,slot,fieldName){
  const b=R.readRef(v,fieldName);if(b.isNull())return {slot,present:false};
  const target=R.readRef(b,'_buttonTarget'),labels=R.readRef(b,'_labels');
  return {slot,present:true,active:active(b),targetActive:active(target),enabled:target.isNull()?null:!!R.unbox(S.invoke('UIButton','get_isEnabled',target),'bool'),state:R.readValue(b,'_currentState','i32'),hasEnoughForCost:R.readValue(b,'_hasEnoughForCost','bool'),labels:labels.isNull()?[]:list(labels).map(labelStatus),cost:labelStatus(R.readRef(b,'_costLabel')),originalCost:labelStatus(R.readRef(b,'_originalCostDiscountLabel')),discountPercent:labelStatus(R.readRef(b,'_discountPercentLabel')),discountCost:labelStatus(R.readRef(b,'_discountCostLabel'))};
 }
 function detailStatus(){
  const {v,item}=details(),formatter=R.readRef(v,'_storeOfferPriceFormatter');
  const label=n=>S.string(S.invoke('UILabel','get_text',R.readRef(v,n)));
  const currentOffer=R.readRef(item,field('Offer'));
  const buttons=[['single','_singlePurchaseButton'],['singleCentered','_singlePurchaseButtonCentered'],['multiple','_multiPurchaseButton'],['ok','_OKButtonCentered']].map(([slot,f])=>buttonStatus(v,slot,f));
  return {readConfirmed:true,id:str(item,'Id'),title:label('_packTitle'),description:label('_descriptionLabel'),fromTab:R.readValue(v,'_fromLocation','i32'),maxPurchaseAmount:R.readValue(v,'_maxPurchaseAmount','i32'),freeButtonShown:R.readValue(v,'_showFreeBuyButton','bool'),nativeFreePurchase:!!R.unbox(S.invoke('StorePackDetailsView','IsFreePurchase',v),'bool'),giftState:R.readValue(v,'_giftState','i32'),giftable:val(item,'Giftable','bool'),showBuyConfirmation:val(item,'ShowBuyConfirmation','bool'),currentOffer:currentOffer.isNull()?null:offer(currentOffer,formatter),buttons,offers:list(R.readRef(item,field('AllOffers'))).map(o=>offer(o,formatter)),scope:'Original detail display and raw button state; hidden text is not a payable quote, enabled does not authorize purchase'};
 }
 function packRewards(){
  const state=current();if(name(state)!=='PackOpeningGameState')return {readConfirmed:true,visible:false,rewards:null,settlementConfirmed:false};
  const c=R.readRef(state,'_packOpeningController'),context=R.readRef(c,'_packOpeningContext');
  if(context.isNull())throw Error('Pack context not ready');
  const v=R.readRef(context,'PackOpeningView');
  if(!active(v))return {readConfirmed:true,visible:false,rewards:null,settlementConfirmed:false};
  const item=R.readRef(context,'StoreItem'),data=R.readRef(context,'PackOpeningViewData');
  const rewards=list(R.readRef(data,'_allRewardTiles')).map(tile=>{
   if(tile.isNull())throw Error('Null pack reward');
   const renderType=name(tile);
   return {id:S.string(R.readRef(tile,'ID')),renderType,itemType:R.unbox(S.invoke(renderType,'get_ItemType',tile),'u8'),quantity:val(tile,'Quantity')};
  });
  return {readConfirmed:true,visible:true,itemId:str(item,'Id'),phase:R.readValue(c,'_currentState','i32'),allTilesRevealed:R.readValue(c,'_allTilesRevealed','bool'),rewards,finishButton:buttonStatus(v,'finish','_finishButton'),settlementConfirmed:false,scope:'Original pack display; item ID alone is not transaction correlation or inventory delta proof'};
 }
 let submission=null;
 function execute(op,p={}){
  if(op==='store-rewards')return packRewards();
  if(op==='store-claim-status')return {readConfirmed:true,submission,settlementConfirmed:false};
  if(op==='store-claim-free'){
   if(submission)throw Error('Prior store submission requires reconciliation');
   if(typeof p.itemId!=='string'||!p.itemId.length||p.itemId.length>256||p.quantity!==1||p.expectedCost!==0)throw Error('Explicit item, quantity one and zero cost required');
   const {v,item}=details(),info=detailStatus();
   if(info.id!==p.itemId||val(item,'ItemType')!==0||info.giftable||info.giftState!==0||!info.nativeFreePurchase||!info.freeButtonShown||info.maxPurchaseAmount<1)throw Error('Current standard free non-gift pack required');
   // Limit this first execution route to the audited in-game ally-point pack.
   if(!info.currentOffer||info.currentOffer.currencyType!==5||info.currentOffer.requiresProductPricing)throw Error('Unsupported free store currency route');
   const visible=info.buttons.filter(b=>b.present&&b.active&&b.targetActive&&['single','singleCentered'].includes(b.slot));
   if(visible.length!==1||!visible[0].enabled)throw Error('Unique active enabled single-purchase button required');
   submission={itemId:p.itemId,quantity:1,expectedCost:0,submittedAt:Date.now(),nativeReturned:false,error:null};
   try{S.invoke('StorePackDetailsView','PurchasePackClicked',v);submission.nativeReturned=true;}
   catch(e){submission.error=String(e);throw e;}
   return {claimDispatched:true,submission,settlementConfirmed:false};
  }
  if(op==='store-list')return status();
  if(op==='store-details')return detailStatus();
  if(op==='store-details-back'){const {c}=details();S.invoke('FixedStoreController','OnBackClickedStoreDetailsView',c);return {navigationDispatched:true};}
  if(op==='store-item'){
   if(typeof p.itemId!=='string'||!p.itemId.length||p.itemId.length>256)throw Error('Explicit store item ID required');
   const {c,v}=live(),info=status(),rows=info.items.filter(r=>r.id===p.itemId);
   if(rows.length!==1||rows[0].locked||rows[0].itemType!==0)throw Error('Unique unlocked standard pack required; other detail flows pending');
   if(R.readValue(R.readRef(c,'_transitionData'),'PurchaseItemOnTransition','bool'))throw Error('Store transition has pending purchase');
   const item=list(R.readRef(v,'_storeItemData'))[rows[0].itemIndex];
   S.invoke('FixedStoreController','OnShowDetailsClicked',c,[A.ref(item)],['StoreItemDO']);
   return {navigationDispatched:true,expectedItemId:p.itemId};
  }
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


