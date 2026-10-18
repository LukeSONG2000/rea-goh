import assert from 'node:assert/strict';
import fs from 'node:fs';
import vm from 'node:vm';
const source=fs.readFileSync(new URL('./battle_info_ui.js',import.meta.url),'utf8');
const part=source.slice(source.indexOf(' function healthText('),source.indexOf(' function layoutTarget('));
let diagnosticThrows=false,reads=0,shieldMax=100;
const stats={healthAnomalies:[]},messages=[];
const ctx=vm.createContext({stats,send:m=>messages.push(m),
 R:{unbox:x=>x,fullName:x=>x,classOf:()=> 'BattleUnitController',readRef:()=> 'unit-id'},
 S:{string:x=>x},A:{value:(t,v)=>v},
 call:(type,method)=>{
  if(method==='GetStatValue'){if(diagnosticThrows)throw Error('diagnostic unavailable');return shieldMax;}
  if(method==='get_UnitDO')return {};
  reads++;return method.includes('Max')&&method.includes('Shield')?shieldMax:100;
 }});
vm.runInContext(part,ctx);
const unit={toString:()=> 'unit'};
ctx.healthText(unit);assert.equal(reads,8);assert.equal(messages.length,0);
shieldMax=-1;
assert.match(ctx.healthText(unit),/永久护盾 100 \/ -1/,'Native negative value must not be fabricated as zero');
assert.equal(messages.length,1);assert.equal(messages[0].maxShieldStat,-1);
ctx.healthText(unit);assert.equal(messages.length,1,'Repeated identical reads must not flood IPC');
diagnosticThrows=true;shieldMax=-2;
assert.match(ctx.healthText(unit),/100 \/ -2/,'A diagnostic failure must not prevent text updates');
assert.match(messages.at(-1).diagnosticError,/diagnostic unavailable/);
for(let i=3;i<40;i++){shieldMax=-i;ctx.healthText(unit);}
assert.equal(stats.healthAnomalies.length,16,'Diagnostic history is bounded');
console.log('Battle info health diagnostics PASS');
