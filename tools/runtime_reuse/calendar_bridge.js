'use strict';
// Original calendar UI only. Catch-up/purchase routes are deliberately excluded.
globalThis.CalendarBridge=(()=>{
 const R=RuntimeApi,S=SceneApi,A=R.arg,records=[];
 const call=(o,n,args=[])=>R.invoke(R.findMethod(R.classOf(o),n,args.length),o,args);
 const active=o=>!o.isNull()&&!!R.unbox(S.invoke('UnityEngine.GameObject','get_activeInHierarchy',S.gameObject(o)),'bool');
 function list(o){if(o.isNull())return [];const n=R.readValue(o,'_size','i32'),a=R.readRef(o,'_items');if(n<0||n>4096||(a.isNull()?n!==0:n>a.add(24).readU64().toNumber()))throw Error('Calendar list bounds');return Array.from({length:n},(_,i)=>a.add(32+8*i).readPointer());}
 function controller(){return R.findBound('CalendarController');}
 function live(){const c=controller(),v=R.readRef(c,'_calendarView');if(!R.readValue(c,'_showing','bool')||!active(v)||R.readValue(c,'_showingInfo','bool')||R.readValue(c,'_bulkClaiming','bool'))throw Error('Calendar view unavailable or busy');return {c,v};}
 function daily(c,id){return call(R.readRef(c,'_playerDC'),'GetDailyLoginWithId',[A.ref(R.string(id))]);}
 function row(c,d){
  const id=S.string(R.readRef(d,'Id')),index=R.readValue(d,'CurrentRewardIndex','i32'),source=daily(c,id);
  const rewards=source.isNull()?[]:list(call(source,'get_DailyReward'));
  const reward=index>=0&&index<rewards.length?rewards[index]:null;
  return {id,rewardIndex:index,title:S.string(R.readRef(d,'DailyLoginTitle')),claimed:!!R.readValue(d,'RewardClaimed','bool'),sourceClaimed:source.isNull()?null:!!R.unbox(call(source,'get_Claimed'),'bool'),lastClaimed:source.isNull()?null:String(R.unbox(call(source,'get_LastClaimed'),'i64')),rewards:reward?[{id:S.string(call(reward,'get_ID')),itemType:R.unbox(call(reward,'get_ItemType'),'u8'),quantity:R.unbox(call(reward,'get_Quantity'),'i32')}]:[]};
 }
 function status(){
  const {c,v}=live(),selected=call(v,'get_ActiveRenderData'),rows=list(R.readRef(v,'_calendarList'));
  if(selected.isNull()||!rows.some(d=>d.equals(selected)))throw Error('Calendar selection ownership mismatch');
  const button=R.readRef(v,'_claimButton'),target=R.readRef(button,'_buttonTarget');
  return {readConfirmed:true,calendars:rows.map(d=>({...row(c,d),selected:d.equals(selected)})),selected:row(c,selected),claimEnabled:active(button)&&!target.isNull()&&!!R.unbox(S.invoke('UIButton','get_isEnabled',target),'bool'),catchUpRoute:!!R.readValue(v,'_catchUpIsFree','bool')};
 }
 function claimStatus(){
  return {readConfirmed:true,claims:records.map(r=>{
   if(r.rewardBalanceReconciled)return {...r};
   const after=InboxClaimTracker.readBalances(r.rewards);
   const exact=r.before.length>0&&r.before.every(b=>{const a=after.find(x=>x.id===b.id&&x.itemType===b.itemType);return Number.isSafeInteger(b.value)&&Number.isSafeInteger(a?.value)&&a.value-b.value===b.expected;});
   const result={...r,after,rewardBalanceReconciled:exact,settlementEvidence:exact?'native-inventory-delta':'unconfirmed'};
   if(exact)Object.assign(r,result);
   return result;
  })};
 }
 function execute(op,p={}){
  if(op==='calendar-claim-status')return claimStatus();
  if(op==='calendar-list')return status();
  if(op==='calendar-open'){
   const state=R.readRef(R.findBound('StateController'),'<CurrentState>k__BackingField');
   if(R.fullName(R.classOf(state))!=='HomeBaseGameState')throw Error('Open calendar from Home');
   const c=controller(),v=R.readRef(c,'_calendarView');
   if(!active(v))S.invoke('CalendarController','ShowCalendar',c,[A.ref(R.nil)],['NguiView']);
   return {navigationDispatched:true};
  }
  const {c,v}=live();
  if(op==='calendar-close'){call(v,'OnBackClick');return {navigationDispatched:true};}
  const info=status();
  if(typeof p.calendarId!=='string'||p.calendarId!==info.selected.id||!Number.isInteger(p.rewardIndex)||p.rewardIndex!==info.selected.rewardIndex||p.lastClaimed!==info.selected.lastClaimed)throw Error('Calendar reward identity changed');
  if(op!=='calendar-claim')throw Error('Unsupported calendar operation');
  const selected=info.selected;
  if(!info.claimEnabled||info.catchUpRoute||selected.claimed||selected.sourceClaimed!==false||!selected.rewards.length)throw Error('Original free daily claim unavailable');
  if(records.some(r=>r.calendarId===p.calendarId&&r.rewardIndex===p.rewardIndex&&r.lastClaimed===p.lastClaimed))throw Error('Already submitted; inspect status, do not retry');
  const r={calendarId:p.calendarId,rewardIndex:p.rewardIndex,lastClaimed:p.lastClaimed,rewards:selected.rewards,before:InboxClaimTracker.readBalances(selected.rewards),claimDispatched:false};
  records.push(r); // Retain uncertain submissions, even when native invocation throws.
  call(v,'ClaimButtonClicked');r.claimDispatched=true;
  return {claimDispatched:true,receipt:{...r},settlementConfirmed:false,note:'Original calendar may defer settlement until close; read calendar-claim-status after closing.'};
 }
 function visibility(){const c=controller();return {showing:!!R.readValue(c,'_showing','bool'),active:active(R.readRef(c,'_calendarView'))};}
 return {execute,status,visibility};
})();
rpc.exports.calendarcommand=(op,payload)=>RuntimeApi.runOnMainThreadOnce(()=>CalendarBridge.execute(op,payload));
rpc.exports.calendarvisibility=()=>RuntimeApi.runOnMainThreadOnce(()=>CalendarBridge.visibility());
