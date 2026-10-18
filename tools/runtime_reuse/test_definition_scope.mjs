import assert from 'node:assert/strict';
import fs from 'node:fs';
import vm from 'node:vm';
const pointer=name=>({name,isNull:()=>name==='nil',equals:other=>name===other.name});
const nil=pointer('nil'),old=pointer('old'),captured=pointer('captured'),foreign=pointer('foreign'),game=pointer('game');
for(const previous of [nil,old]){
 let current=previous;const retained=[];
 const context={RuntimeApi:{readRef:()=>current,writeRef:(_,key,value)=>{assert.equal(key,'_pveUnitDefinitionMap');current=value;},retain:x=>retained.push(x)}};
 vm.createContext(context);vm.runInContext(fs.readFileSync(new URL('./definition_scope.js',import.meta.url),'utf8'),context);
 assert.throws(()=>context.activateCapturedDefinitions(game,nil),/requires original/);
 const restore=context.activateCapturedDefinitions(game,captured);
 assert.equal(current,captured);assert.ok(retained.includes(captured));
 if(previous===old)assert.ok(retained.includes(old));
 current=foreign;assert.throws(restore,/another owner/);assert.equal(current,foreign);
 current=captured;restore();assert.equal(current,previous);
 restore();assert.equal(current,previous);
}
console.log('Definition scope PASS (null previous, retention, ownership, restore, idempotence)');
