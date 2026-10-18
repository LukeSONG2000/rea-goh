import fs from 'node:fs';
import vm from 'node:vm';
import assert from 'node:assert/strict';
let serial=0;
const nil={isNull:()=>true};
const object=fields=>Object.assign({isNull:()=>false,toString:()=>String(++serial)},fields);
function pointer(fields){const key=String(++serial);return Object.assign(object(fields),{toString:()=>key});}
const result=(next=nil)=>pointer({_nextNode:next,_choosenAbilityTag:'original-tag',_resultTarget:2,'<SelectBasic>k__BackingField':false});
const node=()=>pointer({_decisionType:5,_decisionTarget:1,_decisionTargetExcludeSelf:true,_params:['param'],_paramModifier:2,'<Chance>k__BackingField':0.25,'<Negate>k__BackingField':true,'<SuccessResult>k__BackingField':nil,'<FailureResult>k__BackingField':nil});
const first=node(),second=node();
first['<SuccessResult>k__BackingField']=result(second);
first['<FailureResult>k__BackingField']=result(second);
second['<FailureResult>k__BackingField']=result(first);
const tree=pointer({_rootNode:first,_id:'original-tree','<ShouldEvaluateBasicAbility>k__BackingField':true});
const sandbox={RuntimeApi:{readRef:(p,f)=>p[f],readValue:(p,f)=>p[f]},SceneApi:{string:p=>p,array:p=>p}};
vm.createContext(sandbox);
vm.runInContext(fs.readFileSync(new URL('./original_ai_trace.js',import.meta.url),'utf8'),sandbox);
const snapshot=sandbox.readOriginalAITree(tree),g=snapshot.graph;
assert.equal(g.id,'original-tree');assert.equal(g.evaluateBasic,true);assert.equal(g.nodes.length,2);
assert.equal(g.nodes[0].success.next,1);assert.equal(g.nodes[0].failure.next,1);assert.equal(g.nodes[1].failure.next,0);
assert.equal(g.nodes[0].success.abilityTag,'original-tag');assert.equal(g.nodes[0].excludeSelf,true);
assert.equal(snapshot.indexOf(second),1);assert.equal(snapshot.indexOf(pointer({})),null);
assert.equal(first['<SuccessResult>k__BackingField']._nextNode,second,'No graph mutation');
first._params=Array(65).fill('x');assert.throws(()=>sandbox.readOriginalAITree(tree),/64 parameters/);first._params=[];
let current=first;for(let i=0;i<512;i++){const next=node();current['<SuccessResult>k__BackingField']=result(next);current=next;}
assert.throws(()=>sandbox.readOriginalAITree(tree),/512 nodes/);
assert.throws(()=>sandbox.readOriginalAITree(nil),/absent/);
console.log('PASS: original graph branches, shared nodes, cycles, fields, bounds, and no mutation.');
