import assert from 'node:assert/strict';
import fs from 'node:fs';
import vm from 'node:vm';
let serial=0;
const nil={isNull:()=>true,equals:o=>o===nil};
const object=(type,fields={})=>({type,fields,id:++serial,isNull:()=>false,equals(o){return o===this;},toString(){return String(this.id);}});
const list=rows=>object('List',{_size:rows.length,_items:{isNull:()=>false,equals(o){return o===this;},add:offset=>offset===24?{readU64:()=>({toNumber:()=>rows.length})}:{readPointer:()=>rows[(offset-32)/8]}}});
const unit=id=>object('UnitDO',{Id:id,'<Definition>k__BackingField':object('Definition',{BaseId:id})});
const cell=(index,u,type=2)=>object('SquadCellDO',{CellIndex:index,SquadUnitType:type,Unit:u,BattleUnitState:nil,Selectable:true,IsMandatory:false,IsOverkillable:false,DeathRequiredForEncounter:true,IsNonActivatedSquadUnit:false,OverrideAllStatsMultiplier:0});
const squad=rows=>object('SquadDO',{Datacron:nil,_squadCells:list(rows),_squadAbilities:list([])});
const rows=l=>Array.from({length:l.fields._size},(_,i)=>l.fields._items.add(32+i*8).readPointer());
let copyMode='normal';
const R={readRef:(o,k)=>o.fields[k]??nil,readValue:(o,k)=>o.fields[k],writeRef:(o,k,v)=>{o.fields[k]=v;},classOf:o=>o.type,fullName:x=>x,findClass:x=>x,findMethod:()=>'.ctor',arg:{ref:x=>x},newObject:(type,ctor,[source])=>{
 if(type==='UnitDO')return copyMode==='shared-unit'?source:object(type,{...source.fields});
 if(copyMode==='shared-squad')return source;
 const original=rows(source.fields._squadCells);
 return squad(copyMode==='shared-cell'?original:original.map(c=>object(c.type,{...c.fields})));
}};
const context=vm.createContext({RuntimeApi:R,SceneApi:{string:x=>x===nil?null:x}});
vm.runInContext(fs.readFileSync(new URL('./selected_squad.js',import.meta.url),'utf8'),context);
const api=context.SelectedSquadTools;
const support=unit('borrowed'),source=squad([cell(0,unit('leader'),0),cell(1,nil),cell(5,support,1)]);
const copy=api.copy(source);
assert.equal(copy.copiedUnits,2);assert.equal(copy.selection.length,3);
assert.equal(copy.selection[2].SquadUnitType,1);assert.equal(copy.selection[2].CellIndex,5);
const copied=rows(copy.player.fields._squadCells);
copied[2].fields.Unit.fields.Id='private-change';
assert.equal(support.fields.Id,'borrowed','Private UnitDO mutation must not touch source');
copied[0].fields.CellIndex=20;
assert.equal(rows(source.fields._squadCells)[0].fields.CellIndex,0);
for(const mode of ['shared-unit','shared-squad','shared-cell']){
 copyMode=mode;assert.throws(()=>api.copy(source),/not copied/);
}
copyMode='normal';
const repeated=squad([cell(0,support,0),cell(5,support,1)]),repeatedCopy=api.copy(repeated);
const repeatedCells=rows(repeatedCopy.player.fields._squadCells);
assert.equal(repeatedCells[0].fields.Unit,repeatedCells[1].fields.Unit,'Preserve original aliasing within the private copy');
assert.notEqual(repeatedCells[0].fields.Unit,support);
const persisted=squad([cell(0,unit('u'),0)]);rows(persisted.fields._squadCells)[0].fields.BattleUnitState=object('BattleUnitStateDO');
assert.throws(()=>api.copy(persisted),/Persisted/);
const datacron=squad([cell(0,unit('u'),0)]);datacron.fields.Datacron=object('DatacronDO');
assert.throws(()=>api.copy(datacron),/Datacron/);
assert.throws(()=>api.copy(squad([])),/Empty/);
R.nil=nil;R.retain=()=>1;
context.SceneApi.invoke=(type,method,source,args)=>{
 if(method==='MemberwiseClone')return object(source.type,{...source.fields});
 if(method==='set_SupportUnit'){source.fields['<SupportUnit>k__BackingField']=args[0];return nil;}
 throw Error('Unexpected invocation '+method);
};
const ally=object('BattleSupportDO',{'<SupportUnit>k__BackingField':support,_playerLevel:null,_isAlly:false,_supportRewardQuantity:0});
const supportSquad=api.copy(squad([cell(5,support,1)])).player;
const allyCopy=api.copySupport(ally,supportSquad);
assert.notEqual(allyCopy,ally);
assert.equal(allyCopy.fields._playerLevel,null);assert.equal(allyCopy.fields._isAlly,false);assert.equal(allyCopy.fields._supportRewardQuantity,0);
assert.equal(allyCopy.fields['<SupportUnit>k__BackingField'],rows(supportSquad.fields._squadCells)[0].fields.Unit);
assert.equal(ally.fields['<SupportUnit>k__BackingField'],support);
assert.equal(api.copySupport(nil,supportSquad),nil);
assert.throws(()=>api.copySupport(ally,squad([cell(0,support,0)])),/exactly one/);
assert.throws(()=>api.copySupport(ally,squad([cell(5,support,1)])),/live unit/);
const duplicateSupport=api.copy(squad([cell(5,support,1),cell(6,support,1)])).player;
assert.throws(()=>api.copySupport(ally,duplicateSupport),/exactly one/);
const mission={CampaignID:'EVENTS',MapID:'CHALLENGES',NodeID:'XP',MissionID:'DIFF06',Difficulty:1};
const captured={_campaignID:'EVENTS',_mapId:'CHALLENGES',_nodeId:'XP',_missionId:'DIFF06',difficulty:1};
const selected={missionIdentity:mission,player:supportSquad,battleSupport:allyCopy,eventId:'challenge_XP',instanceId:'instance',eventScreenMode:2};
const transition=object('BattleGameState+TransitionData');let writes=0;
R.string=x=>x;R.arg.value=(_,x)=>x;
const invoke=context.SceneApi.invoke;
context.SceneApi.invoke=(type,method,source,args)=>{
 if(type==='BattleGameState+TransitionData'){writes++;source.fields[method.slice(4)]=args[0];return nil;}
 return invoke(type,method,source,args);
};
for(const key of Object.keys(captured)){
 const mismatch={...captured,[key]:'wrong'};
 assert.throws(()=>api.applyToTransition(transition,selected,mismatch),/does not match/);
 assert.equal(writes,0,'Mission mismatch must reject before any transition writes');
}
const bound=object('BattleGameState+TransitionData',{'<BattleContext>k__BackingField':object('Context')});
assert.throws(()=>api.applyToTransition(bound,selected,captured),/Unbound/);
assert.equal(writes,0);
assert.equal(api.applyToTransition(transition,selected,captured).missionMatched,true);
assert.equal(writes,5);assert.equal(transition.fields.BattleSupport,allyCopy);
assert.equal(transition.fields.PlayerBattleSquad,supportSquad);
assert.equal(transition.fields.EventScreenMode,2);
console.log('Selected squad copy PASS: native-copy contract, support/empty slots, object isolation, alias preservation, unsupported-state rejection');
