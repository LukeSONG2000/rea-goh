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
 const context={RuntimeApi:R,SceneApi:S,InboxClaimTracker:{readBalances:()=>[{id:'PREMIUM',itemType:2,value:balance,expected:50}]},rpc:{exports:{}}};
 vm.runInNewContext(fs.readFileSync(new URL('./calendar_bridge.js',import.meta.url),'utf8'),context);
 return {bridge:context.CalendarBridge,view,row,source,controller,button,list,ptr,claims:()=>claims,payload:{calendarId:'OCTOBER',rewardIndex:0,lastClaimed:'123'}};
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
