import fs from 'node:fs';
import vm from 'node:vm';
import assert from 'node:assert/strict';
let serial=0,handler,detached=0;
const nil={isNull:()=>true};
function ptr(fields={}){const id=++serial,p=Object.assign({isNull:()=>false,toString:()=>String(id)},fields);p.equals=other=>other===p;return p;}
const branch=(next,tag=null,basic=false)=>ptr({_nextNode:next,_choosenAbilityTag:tag,_resultTarget:0,'<SelectBasic>k__BackingField':basic});
const node=tag=>ptr({_decisionType:2,_decisionTarget:0,_decisionTargetExcludeSelf:false,_params:['0'],_paramModifier:0,'<Chance>k__BackingField':0,'<Negate>k__BackingField':false,_paramAsFloatCached:false,_paramAsFloat:0,_paramAsIntCached:false,_paramAsInt:0,'<SuccessResult>k__BackingField':branch(nil,tag),'<FailureResult>k__BackingField':nil});
const first=node('priority'),second=node('damage');first['<FailureResult>k__BackingField']=branch(second);second['<FailureResult>k__BackingField']=branch(nil,null,true);
const source=ptr({_rootNode:first,_id:'default','<ShouldEvaluateBasicAbility>k__BackingField':true});
const context=ptr({'<GameDataDomainController>k__BackingField':ptr({_abilityDecisionTreeDO:source})});
const R={nil,arg:{ref:p=>p,value:(t,v)=>v},readRef:(p,f)=>p[f],readValue:(p,f)=>p[f],writeRef:(p,f,v)=>p[f]=v,writeValue:(p,f,t,v)=>p[f]=v,
 findClass:n=>n,findMethod:(c,n)=>({name:n,readPointer:()=>n}),classOf:p=>p,unbox:x=>x,invoke:(m,p)=>p[m.name](),
 newObject(c,m,a){
  if(c==='AbilityDecisionResult')return branch(a[0]);
  if(c==='AbilityDecisionTreeDO')return ptr({_id:a[0],_rootNode:a[1],'<ShouldEvaluateBasicAbility>k__BackingField':a[2]});
  const n=node(null);Object.assign(n,{_decisionType:a[0],_decisionTarget:a[1],_params:a[2],_paramModifier:a[3],_decisionTargetExcludeSelf:a[4],'<Chance>k__BackingField':a[5],'<Negate>k__BackingField':a[6],'<SuccessResult>k__BackingField':a[7]});return n;
 }};
const sandbox={RuntimeApi:R,SceneApi:{string:x=>x,array:x=>x},Interceptor:{attach(a,h){handler=h;return {detach(){detached++;}};}}};
vm.createContext(sandbox);
for(const file of ['original_ai_trace.js','original_ai_priority.js'])vm.runInContext(fs.readFileSync(new URL(file,import.meta.url),'utf8'),sandbox);
for(const bad of [[0],[0,0],[0,2],[-1,0],[true,0]])assert.throws(()=>sandbox.installOriginalAIPriority(context,bad),/permutation/);
const original=JSON.stringify(sandbox.readOriginalAITree(source).graph);
const gate=sandbox.installOriginalAIPriority(context,[1,0]),copy=gate.treeFor(1);
assert.notEqual(copy,source);assert.notEqual(copy._rootNode,second);assert.equal(gate.treeFor(0),source);
const graph=sandbox.readOriginalAITree(copy).graph;assert.equal(graph.nodes[0].success.abilityTag,'damage');assert.equal(graph.nodes[1].success.abilityTag,'priority');assert.equal(graph.nodes[1].failure.selectBasic,true);
assert.equal(JSON.stringify(sandbox.readOriginalAITree(source).graph),original);
const controller=ptr({_battleContext:context});const ai=owner=>ptr({Self:ptr({get_Owner:()=>owner})});
let args=[controller,first,ai(0)];handler.onEnter(args);assert.equal(args[1],first);
args=[ptr({_battleContext:ptr()}),first,ai(1)];handler.onEnter(args);assert.equal(args[1],first);
args=[controller,second,ai(1)];handler.onEnter(args);assert.equal(args[1],second);
args=[controller,first,ai(1)];handler.onEnter(args);assert.equal(args[1],copy._rootNode);
assert.equal(gate.snapshot().substitutions,1);assert.equal(gate.snapshot().originalUnchanged,true);assert.equal(gate.snapshot().errors.length,0);
gate.stop();gate.stop();assert.equal(detached,1);
first['<SuccessResult>k__BackingField']._nextNode=second;
assert.throws(()=>sandbox.installOriginalAIPriority(context,[0,1]),/topology/);
console.log('PASS: exact private clone, neutral continuation order, terminal fallback, original unchanged and owned-enemy-only substitution.');
