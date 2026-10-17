// Offline fixtures verify reader contracts, not gameplay formula equivalence.
import assert from 'node:assert/strict';
import vm from 'node:vm';
import {readFileSync} from 'node:fs';
const unit={isNull:()=>false},dto={isNull:()=>false},calculator={isNull:()=>false};
const calls=[];
const context=vm.createContext({RuntimeApi:{
 arg:{value:(kind,value)=>({kind,value}),ref:value=>({kind:'ref',value})},
 readRef:(obj,name)=>{assert.equal(obj,unit);assert.ok(['_unitDO','_statCalculator'].includes(name));return name==='_unitDO'?dto:calculator;},
 unbox:(box,kind)=>{assert.equal(box.kind,kind);return box.value;}
},SceneApi:{invoke:(type,method,obj,args,types)=>{
 calls.push({type,method,obj,args,types});
 const box=(value,kind='f32')=>({value,kind});
 const protection={CurrentHP:700,MaxHP:1000,CurrentShield:150,MaxShield:400,CurrentPermanentShield:50,MaxPermanentShield:300,CurrentBonusShield:100,MaxBonusShield:100};
 if(method.startsWith('get_'))return box(protection[method.slice(4)],'i32');
 assert.equal(args[0].kind,'u8');assert.equal(types[0],'UnitStatEnum');
 const id=args[0].value;
 if(method==='GetInitialUnitCalculatedStat')return box(id===61?20:100);
 if(method==='GetStatValue'){assert.equal(types.length,1);return box(id===61?35:140);}
 if(method==='GetCalculatedStatValue')throw Error('Forbidden mutating getter');
 if(method==='HasMasteryContribution')return box(id===6?1:0,'bool');
 if(method==='GetMasteryContribution'){assert.equal(obj,calculator);assert.equal(args[1].value,unit);assert.equal(types[1],'IStatSource');return box(12.5);}
 throw Error('Unapproved invocation '+method);
}}});
vm.runInContext(readFileSync(new URL('./battle_attributes_reader.js',import.meta.url),'utf8'),context);
assert.equal(calls.length,0,'Loading reader must not invoke native methods');
const result=context.readBattleAttributes(unit,{statIds:[6,6]});
assert.equal(result.stats.length,2,'Deduplicate requested stats and include mastery');
assert.equal(result.protection.bonus.current,100);
assert.equal(result.protection.permanent.max,300);
assert.equal(result.stats[0].netBattleDelta,40);
assert.equal(result.stats[0].masteryCoefficient,12.5);
assert.equal(result.stats[0].masteryContributionDelta,null);
assert.equal(result.mastery.netBattleDelta,15);
assert.equal(result.mastery.attributedStatChanges,null);
assert.equal(result.warnings.length,0);
assert.equal(context.BattleAttributesReader.delta(null,10),null);
const before=calls.length;
assert.throws(()=>context.readBattleAttributes(unit,{statIds:[256]}),/UnitStatEnum/);
assert.equal(calls.length,before,'Reject invalid enum before native calls');
console.log('PASS: original getter arguments, byte enum overload, independent protection pools, net vs mastery attribution, no calls on load.');

assert.ok(calls.every(c=>c.method!=='GetCalculatedStatValue'),'Never invoke mutating display getter');
assert.equal(result.stats[0].dtoDisplayOnly,null);
