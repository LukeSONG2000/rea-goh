import assert from 'node:assert/strict';
import fs from 'node:fs';
import vm from 'node:vm';

const source=fs.readFileSync(new URL('./battle_info_ui.js',import.meta.url),'utf8');
const pointer=id=>({isNull:()=>id===null,toString:()=>String(id),equals:other=>other.toString()===String(id)});
const nil=pointer(null),view=pointer('view'),target=pointer('target');
const first=pointer('first'),second=pointer('second');
const item={view,target,label:{text:''},unit:first};
const targets=new Map([['target',item]]),stats={target:'old',healthAnomalies:[]};
let state=pointer('state'),selected=nil,reads=0,layouts=0;
const ctx=vm.createContext({targets,stats,
 R:{findBound:()=>pointer('controller'),classOf:()=> 'BattleGameState',fullName:x=>x,
 readRef:(o,k)=>k==='<CurrentState>k__BackingField'?state:k==='_opponentTargetComponent'?target:k==='<SelectedOpponent>k__BackingField'?selected:pointer(k)},
 call:()=>view,active:()=>true,
 setText:(label,text)=>{label.text=text;},
 targetNumbers:item=>{assert.equal(item.unit.isNull(),false);reads++;return item.unit.toString();},
 layoutTarget:()=>{layouts++;},
 makeLabel:()=>assert.fail('Null or existing targets must not allocate labels')});
vm.runInContext(source.slice(source.indexOf(' function refreshCurrentTarget('),source.indexOf(' function render(popup,')),ctx);
ctx.renderTarget(view,first);assert.equal(item.label.text,'first');
ctx.renderTarget(view,nil);assert.equal(item.label.text,'');assert.equal(stats.target,null);
assert.equal(item.unit,nil);const before=reads;
ctx.refreshCurrentTarget();assert.equal(reads,before,'Timer must not resurrect old unit');
ctx.renderTarget(view,second);assert.equal(item.label.text,'second');assert.equal(stats.target,'second');
selected=first;
ctx.refreshCurrentTarget();assert.equal(item.label.text,'first','Native selection overrides stale callback cache');
selected=second;
ctx.refreshCurrentTarget();assert.equal(item.label.text,'second');
ctx.updateTargetItem(item,nil);assert.equal(item.label.text,'');assert.equal(stats.target,null);
targets.clear();stats.target='old';ctx.renderTarget(view,nil);assert.equal(stats.target,null);
state=nil;stats.target='old';ctx.refreshCurrentTarget();assert.equal(stats.target,null);
assert.ok(layouts>=2);
console.log('PASS target clear, no stale timer reads, reselection, missing label and null state');
