import fs from 'node:fs';
import vm from 'node:vm';
import assert from 'node:assert/strict';
function setup(){
 const ptr=(fields={})=>Object.assign({isNull:()=>false,equals(o){return this===o;}},fields);
 const nil={isNull:()=>true};let claims=0,balance=100;
 const array=rows=>({isNull:()=>false,add(offset){return offset===24?{readU64:()=>({toNumber:()=>rows.length})}:{readPointer:()=>rows[(offset-32)/8]};}});
 const list=rows=>ptr({_size:rows.length,_items:array(rows)});
 const reward=ptr({get_ID:'PREMIUM',get_ItemType:2,get_Quantity:50});
 const source=ptr({get_DailyReward:list([reward]),get_Claimed:false,get_LastClaimed:'123'});
 const row=ptr({Id:'OCTOBER',CurrentRewardIndex:0,DailyLoginTitle:'October',RewardClaimed:false});
 const button=ptr({_buttonTarget:ptr({enabled:true})});
 const view=ptr({_calendarList:list([row]),get_ActiveRenderData:row,_claimButton:button,_catchUpIsFree:false,ClaimButtonClicked(){claims++;row.RewardClaimed=true;},OnBackClick(){view.active=false;balance+=50;}});
 const player=ptr({GetDailyLoginWithId:source});
 const controller=ptr({_calendarView:view,_showing:true,_showingInfo:false,_bulkClaiming:false,_playerDC:player});
 const R={arg:{ref:x=>x,value:(type,x)=>x},findBound:()=>controller,readRef:(o,n)=>o[n],readValue:(o,n)=>o[n],classOf:o=>o,findMethod:(c,n)=>n,invoke:(m,o,a)=>typeof o[m]==='function'?o[m](...a):o[m],unbox:x=>x,string:x=>x,nil};
 const S={gameObject:o=>o,string:x=>x,invoke:(t,n,o)=>n==='get_activeInHierarchy'?o.active!==false:n==='get_isEnabled'?o.enabled:undefined};
 const balances=items=>{const totals=new Map();for(const r of items){const key=JSON.stringify([r.itemType,r.id]);const v=totals.get(key)||{id:r.id,itemType:r.itemType,value:r.itemType===2?balance:null,expected:0};v.expected+=r.quantity;totals.set(key,v);}return [...totals.values()];};
 const context={RuntimeApi:R,SceneApi:S,InboxClaimTracker:{readBalances:balances},rpc:{exports:{}}};
 vm.runInNewContext(fs.readFileSync(new URL('./calendar_bridge.js',import.meta.url),'utf8'),context);
 return {bridge:context.CalendarBridge,view,row,source,controller,button,list,ptr,reward,setBalance:v=>balance=v,claims:()=>claims,payload:{calendarId:'OCTOBER',rewardIndex:0,lastClaimed:'123'}};
}
for(const mutate of [x=>x.payload.rewardIndex=1,x=>x.payload.lastClaimed='122',x=>x.view._catchUpIsFree=true,x=>x.button._buttonTarget.enabled=false,x=>x.row.RewardClaimed=true,x=>x.source.get_Claimed=true,x=>x.view.active=false,x=>x.controller._bulkClaiming=true]){
 const x=setup();mutate(x);assert.throws(()=>x.bridge.execute('calendar-claim',x.payload));assert.equal(x.claims(),0);
}
const x=setup();assert.equal(x.bridge.execute('calendar-list').selected.rewards[0].quantity,50);
assert.equal(x.bridge.execute('calendar-claim',x.payload).claimDispatched,true);
assert.equal(x.bridge.execute('calendar-claim-status').claims[0].rewardBalanceReconciled,false);
x.row.RewardClaimed=false;assert.throws(()=>x.bridge.execute('calendar-claim',x.payload),/Already submitted/);
x.bridge.execute('calendar-close');assert.equal(x.bridge.execute('calendar-claim-status').claims[0].rewardBalanceReconciled,true);assert.equal(x.claims(),1);
console.log('PASS calendar identity, original eligibility, catch-up rejection, one submission, deferred settlement');
const y=setup();let selectedIndex=null;
const second=y.ptr({...y.row,Id:'SECOND'});
y.view._calendarList=y.list([y.row,second]);
y.view.ShowCalendar=index=>{selectedIndex=index;y.view.get_ActiveRenderData=[y.row,second][index];};
assert.equal(y.bridge.execute('calendar-select',{calendarId:'OCTOBER'}).alreadySelected,true);assert.equal(selectedIndex,null);
assert.equal(y.bridge.execute('calendar-select',{calendarId:'SECOND'}).expectedCalendarId,'SECOND');assert.equal(selectedIndex,1);
assert.equal(y.bridge.status().selected.id,'SECOND');assert.equal(y.claims(),0);
assert.throws(()=>y.bridge.execute('calendar-select',{calendarId:'MISSING'}),/missing/);
y.view._calendarList=y.list([second,second]);assert.throws(()=>y.bridge.execute('calendar-select',{calendarId:'SECOND'}),/ambiguous/);
console.log('PASS two-calendar original index dispatch, no-op, missing/ambiguous rejection, no reward claim');
function twoClaims({secondBaseline=100,secondType=2}={}){
 const z=setup(),other=z.ptr({...z.row,Id:'SECOND',RewardClaimed:false});
 z.view._calendarList=z.list([z.row,other]);z.bridge.execute('calendar-claim',z.payload);
 z.setBalance(secondBaseline);z.reward.get_ItemType=secondType;z.view.get_ActiveRenderData=other;
 z.bridge.execute('calendar-claim',{...z.payload,calendarId:'SECOND'});return z;
}
const batch=twoClaims();batch.setBalance(150);
assert.ok(batch.bridge.execute('calendar-claim-status').claims.every(r=>!r.rewardBalanceReconciled),'Partial combined credit must not confirm either receipt');
batch.setBalance(200);const complete=batch.bridge.execute('calendar-claim-status').claims;
assert.ok(complete.every(r=>r.rewardBalanceReconciled&&r.settlementEvidence==='native-inventory-batch-delta'));
assert.equal(complete[0].batchBalanceComparison[0].expected,100);assert.equal(complete[0].batchMembers.length,2);
batch.setBalance(5);assert.ok(batch.bridge.execute('calendar-claim-status').claims.every(r=>r.rewardBalanceReconciled),'Later spending must not erase observed settlement');
const inconsistent=twoClaims({secondBaseline:150});inconsistent.setBalance(200);
assert.ok(inconsistent.bridge.execute('calendar-claim-status').claims.every(r=>!r.rewardBalanceReconciled&&!r.baselineConsistent));
const unsupported=twoClaims({secondType:99});unsupported.setBalance(150);
assert.ok(unsupported.bridge.execute('calendar-claim-status').claims.every(r=>!r.rewardBalanceReconciled));
const extra=twoClaims();extra.setBalance(201);assert.ok(extra.bridge.execute('calendar-claim-status').claims.every(r=>!r.rewardBalanceReconciled));
console.log('PASS aggregate credit, partial/extra credit, inconsistent baselines, unsupported rewards, latched evidence');
