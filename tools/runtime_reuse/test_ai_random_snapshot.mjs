import assert from 'node:assert/strict';
import fs from 'node:fs';
import vm from 'node:vm';
const names=['BattleRandom','PlayerAbilitySelectionRandom','OpponentAbilitySelectionRandom','PlayerTargetRandom','OpponentTargetRandom'];
const context={},reads=[];
names.forEach((name,i)=>context['<'+name+'>k__BackingField']={isNull:()=>false,seed:9007199254740993n+BigInt(i)});
const sandbox={RuntimeApi:{readRef:(o,f)=>{reads.push(f);return o[f];},readValue:(o,f,type)=>{assert.equal(f,'_seed');assert.equal(type,'i64');return o.seed;}}};
vm.runInNewContext(fs.readFileSync(new URL('./original_ai_trace.js',import.meta.url),'utf8'),sandbox);
const before=sandbox.readOriginalAIRandomState(context);
assert.equal(before.BattleRandom,'9007199254740993');assert.equal(reads.length,5);
assert.equal(context['<BattleRandom>k__BackingField'].seed,9007199254740993n);
context['<OpponentTargetRandom>k__BackingField'].seed=123n;
const after=sandbox.readOriginalAIRandomState(context);
assert.deepEqual(Object.keys(before).filter(k=>before[k]!==after[k]),['OpponentTargetRandom']);
context['<PlayerTargetRandom>k__BackingField']={isNull:()=>true};assert.equal(sandbox.readOriginalAIRandomState(context).PlayerTargetRandom,null);
console.log('PASS exact int64 seeds, five streams, null stream, no evaluator/RNG/native calls or writes');
let serial=0;const retained=new Set();let failAt=Infinity,clones=0;
function pointer(seed){return {id:++serial,seed,type:'JavaRandom',isNull:()=>false,toString(){return String(this.id);},equals(p){return this===p;}};}
names.forEach((n,i)=>context['<'+n+'>k__BackingField']=pointer(BigInt(i+100)));
Object.assign(sandbox.RuntimeApi,{classOf:o=>o.type,fullName:x=>x,arg:{value:(t,x)=>x},unbox:x=>x,retain:o=>{retained.add(o);return o;},release:o=>retained.delete(o)});
sandbox.SceneApi={invoke(type,method,object,args){
 if(method==='MemberwiseClone'){if(++clones===failAt)throw Error('clone allocation failed');return pointer(object.seed);}
 assert.equal(type,'JavaRandom');assert.equal(method,'nextInt');assert.ok(retained.has(object));
 // Stub native mutation to check ownership/lifecycle, not the RNG algorithm.
 object.seed+=1n;return Number(object.seed%BigInt(args[0]));
}};
const audit=sandbox.verifyOriginalAIRandomCopies(context);assert.equal(audit.originalUnchanged,true);assert.ok(audit.rows.every(r=>r.equal));assert.equal(retained.size,0);
const copies=sandbox.copyOriginalAIRandomStreams(context);assert.equal(retained.size,5);
assert.throws(()=>copies.nextInt('not-a-stream',100));assert.throws(()=>copies.nextInt(names[0],0));assert.throws(()=>copies.nextInt(names[0],1.5));
copies.dispose();copies.dispose();assert.equal(retained.size,0);assert.throws(()=>copies.state(),/disposed/);assert.throws(()=>copies.nextInt(names[0],10),/disposed/);
clones=0;failAt=3;assert.throws(()=>sandbox.copyOriginalAIRandomStreams(context),/allocation/);assert.equal(retained.size,0,'Partial failure releases prior clones');
failAt=Infinity;context['<OpponentTargetRandom>k__BackingField']=context['<PlayerTargetRandom>k__BackingField'];
const aliased=sandbox.copyOriginalAIRandomStreams(context);assert.equal(retained.size,4);aliased.nextInt('PlayerTargetRandom',100);assert.equal(aliased.state().PlayerTargetRandom,aliased.state().OpponentTargetRandom);aliased.dispose();assert.equal(retained.size,0);
console.log('PASS private clone ownership, original seed preservation, aliasing, draw guards, disposal and failure cleanup');
