import assert from 'node:assert/strict';
import fs from 'node:fs';
import vm from 'node:vm';
const source=fs.readFileSync(new URL('./battle_info_ui.js',import.meta.url),'utf8');
const part=source.slice(source.indexOf(' function layoutTarget('),source.indexOf(' function refreshCurrentTarget('));
const obj=fields=>({fields,isNull:()=>false});
const name=obj({min:-187,max:65}),skills=obj({min:-260,max:58}),passives=obj({min:-310,max:-77});
const label=obj({x:0});
const item={target:obj({_nameLabel:name}),view:obj({_opponentAbilitiesComponent:obj({_abilityButtonGrid:skills,_leaderAndUniqueIconsGrid:passives})}),label};
const stats={};
const ctx=vm.createContext({stats,Number,Math,widgetBounds:()=>({y:10}),
 R:{nil:null,readRef:(o,k)=>o.fields[k],api:()=>x=>x},
 S:{parent:x=>x,transform:x=>x,vector:()=>[0,10,0]},A:{ref:x=>x},active:()=>true,
 setVector:(o,k,v)=>{o.fields.x=v[0];},
 call:(c,m,o,args)=>{const f=args[1].fields;
  const min=args[1]===label?f.x-350+27:f.min,max=args[1]===label?f.x+27:f.max;
  return {readFloat:()=>(min+max)/2,add:()=>({readFloat:()=>(max-min)/2})};}
});
vm.runInContext(part,ctx);ctx.layoutTarget(item);
assert.equal(stats.targetLayout.controlsLeft,-310);
assert.equal(stats.targetLayout.label.right,-326,'Correct scaled/offset label edge, not only transform origin');
skills.fields.min=-420;ctx.layoutTarget(item);
assert.equal(stats.targetLayout.label.right,-436,'Expanded ability grid must move numbers farther left');
console.log('Battle info actual-edge layout PASS');
