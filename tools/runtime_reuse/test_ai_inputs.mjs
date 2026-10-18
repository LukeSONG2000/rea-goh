import fs from 'node:fs';
import vm from 'node:vm';
import assert from 'node:assert/strict';
let id=0,cloneCount=0,failAt=Infinity;
const retained=new Set(),nil={isNull:()=>true,toString:()=> '0'};
const pointer=fields=>Object.assign({id:++id,isNull:()=>false,equals(p){return p===this;},toString(){return String(this.id);}},fields);
const array=items=>pointer({items});
const unit=pointer({}),ability=pointer({});
const targets=pointer({_items:array([unit,nil]),_size:1,_version:3});
const skills=pointer({_items:array([ability]),_size:1,_version:8});
const source=pointer({type:'BattleAbilityAIContext',EnemyTargetOptions:targets,Enemies:targets,Allies:nil,SpecialAbilities:skills,Self:unit,PrimaryAbility:ability,Chance:37});
const originals=new Set([source,targets,targets._items,skills,skills._items]);
const sandbox={RuntimeApi:{readRef:(p,f)=>p[f],readValue:(p,f)=>p[f],classOf:p=>p.type,fullName:x=>x,
 retain:p=>{retained.add(p);return p;},release:p=>retained.delete(p),writeRef:(p,f,v)=>{assert.ok(!originals.has(p));p[f]=v;}},
 SceneApi:{array:p=>p.items,invoke(type,name,p){
  if(++cloneCount===failAt)throw Error('allocation failed');
  if(name==='Clone'){assert.equal(type,'System.Array');return array([...p.items]);}
  assert.equal(name,'MemberwiseClone');const q=pointer({});for(const [k,v] of Object.entries(p))if(!['id','equals','isNull','toString'].includes(k))q[k]=v;return q;
 }}};
vm.runInNewContext(fs.readFileSync(new URL('./original_ai_trace.js',import.meta.url),'utf8'),sandbox);
const audit=sandbox.verifyOriginalAIInputs(source);assert.equal(audit.independent,true);assert.equal(audit.contentsEqual,true);assert.equal(audit.lists,2);assert.equal(audit.arrays,2);assert.equal(retained.size,0);
const copy=sandbox.copyOriginalAIInputs(source),c=copy.context;
assert.equal(c.EnemyTargetOptions,c.Enemies);assert.equal(c.Self,unit);assert.equal(c.PrimaryAbility,ability);assert.equal(c.Chance,37);
c.Enemies._items.items[0]=nil;c.Enemies._size=0;assert.equal(targets._items.items[0],unit);assert.equal(targets._size,1);
copy.dispose();copy.dispose();assert.equal(retained.size,0);
const separateList=pointer({_items:targets._items,_size:1,_version:1});source.Allies=separateList;originals.add(separateList);
const aliased=sandbox.copyOriginalAIInputs(source);assert.notEqual(aliased.context.Allies,aliased.context.Enemies);assert.equal(aliased.context.Allies._items,aliased.context.Enemies._items);aliased.dispose();
cloneCount=0;failAt=3;assert.throws(()=>sandbox.copyOriginalAIInputs(source),/allocation/);assert.equal(retained.size,0);
failAt=Infinity;targets._size=513;assert.throws(()=>sandbox.copyOriginalAIInputs(source),/bounds/);assert.equal(retained.size,0);
assert.throws(()=>sandbox.copyOriginalAIInputs(nil),/required/);
console.log('PASS private list/array identity, source mutation isolation, aliases, nullable list, scalar and element preservation, bounds and failure cleanup');
