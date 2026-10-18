import assert from 'node:assert/strict';
import fs from 'node:fs';
import vm from 'node:vm';
let serial=0,failConstructor=false,sharedQueue=false;
const retained=new Set();
const pointer=fields=>Object.assign({id:++serial,isNull:()=>false,toString(){return String(this.id);},equals(p){return this===p;}},fields);
const queue=()=>pointer({_array:pointer({}),_size:0,_version:0,_head:0,_tail:0,items:[]});
const original=pointer({type:'pool',_initialInstanceCapacity:10,_pool:queue()});
const context={BattleUnitControllerListPool:original};
const R={readRef:(o,k)=>o[k],readValue:(o,k)=>o[k],classOf:o=>o.type,
 api:()=>type=>pointer({type}),retain:p=>{retained.add(p);return p;},release:p=>retained.delete(p),
 arg:{value:(t,v)=>v},findMethod:(type,name)=>name,
 invoke(name,p,args=[]){
  assert.notEqual(p,original,'Never call original pool');
  if(name==='.ctor'){
   if(failConstructor)throw Error('constructor failure');
   assert.deepEqual(Array.from(args),[0,10]);p._initialInstanceCapacity=args[1];p._pool=sharedQueue?original._pool:queue();return;
  }
  if(name==='GetInstance'){
   const q=p._pool,result=q.items.shift()??pointer({type:'list',Pool:p,_size:0});q._size=q.items.length;q._version++;return result;
  }
  assert.equal(name,'Dispose');const q=p.Pool._pool;p._size=0;q.items.push(p);q._size=q.items.length;q._version++;
 }};
const sandbox={RuntimeApi:R};
vm.runInNewContext(fs.readFileSync(new URL('./original_ai_trace.js',import.meta.url),'utf8'),sandbox);
const audit=sandbox.verifyOriginalAITargetPool(context);
assert.equal(audit.originalUnchanged,true);assert.equal(audit.reuseConfirmed,true);assert.equal(audit.empty,true);assert.equal(retained.size,0);
const pool=sandbox.createOriginalAITargetPool(context),a=pool.borrow(),b=pool.borrow();
a._size=2;assert.notEqual(a,b);assert.throws(()=>pool.giveBack(pointer({})),/not borrowed/);
pool.giveBack(a);assert.throws(()=>pool.giveBack(a),/not borrowed/);
pool.dispose();pool.dispose();assert.equal(retained.size,0);assert.equal(b._size,0);
assert.throws(()=>pool.borrow(),/disposed/);assert.throws(()=>pool.giveBack(b),/disposed/);
failConstructor=true;assert.throws(()=>sandbox.createOriginalAITargetPool(context),/constructor failure/);assert.equal(retained.size,0);
failConstructor=false;sharedQueue=true;assert.throws(()=>sandbox.createOriginalAITargetPool(context),/shared/);assert.equal(retained.size,0);
assert.equal(original._pool._version,0);assert.equal(original._pool._size,0);
console.log('PASS private target pool ownership, reuse, return cleanup, double-return rejection, constructor failure and shared-queue rejection');
sharedQueue=false;
const classes=new Map();R.classOf=o=>{if(!classes.has(o.type))classes.set(o.type,{name:o.type,equals(other){return other===this;}});return classes.get(o.type);};
R.fullName=c=>c.name;R.api=()=>klass=>pointer({type:klass.name});R.unbox=x=>x;
const battle=pointer({type:'BattleContext',BattleUnitControllerListPool:original});
const evaluator=pointer({type:'BattleTargetingRuleEvaluator',_battleContext:battle});
const controller=pointer({type:'BattleAbilityAIController',_battleContext:battle,_targetingRuleEvaluator:evaluator});
const nil={isNull:()=>true};
const tree=pointer({type:'AbilityDecisionTreeDO',_rootNode:nil});
const data=pointer({type:'GameDataDomainController',_abilityDecisionTreeDO:tree});
battle['<GameDataDomainController>k__BackingField']=data;controller._gameDataDC=data;
const names=['BattleRandom','PlayerAbilitySelectionRandom','OpponentAbilitySelectionRandom','PlayerTargetRandom','OpponentTargetRandom'];
names.forEach((n,i)=>battle['<'+n+'>k__BackingField']=pointer({type:'JavaRandom',_seed:BigInt(i+100)}));
const originals=new Set([original,battle,evaluator,controller,data,tree,...names.map(n=>battle['<'+n+'>k__BackingField'])]);
let writes=0,failWrite=Infinity;
R.writeRef=(p,k,v)=>{assert.ok(!originals.has(p),'Never write original object');if(++writes===failWrite)throw Error('write failure');p[k]=v;};
sandbox.SceneApi={invoke(type,name,p,args){
 if(name==='MemberwiseClone'){const q=pointer({});for(const [k,v] of Object.entries(p))if(!['id','isNull','toString','equals'].includes(k))q[k]=v;return q;}
 assert.equal(type,'JavaRandom');assert.equal(name,'nextInt');assert.ok(!originals.has(p));p._seed++;return Number(p._seed%BigInt(args[0]));
}};
const infrastructure=sandbox.verifyOriginalAIInfrastructure(controller);
assert.equal(infrastructure.privateBindings,true);assert.equal(infrastructure.originalUnchanged,true);assert.equal(infrastructure.readyForEvaluation,false);assert.equal(retained.size,0);
assert.equal(infrastructure.privateGraphRouting,true);
writes=0;failWrite=7;assert.throws(()=>sandbox.createOriginalAIInfrastructure(controller),/write failure/);assert.equal(retained.size,0,'Release all copies after partial binding failure');
failWrite=Infinity;evaluator._battleContext=pointer({type:'BattleContext'});
assert.throws(()=>sandbox.createOriginalAIInfrastructure(controller),/identity mismatch/);assert.equal(retained.size,0);
console.log('PASS private context/controller/evaluator bindings, original write exclusion, partial failure cleanup and foreign evaluator rejection');
