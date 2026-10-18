'use strict';
// Observe original decisions only. Never invokes an extra decision evaluator or
// advances an original random stream. The copy audit advances private clones.
// Snapshot the original graph without evaluating conditions or advancing RNG.
globalThis.readOriginalAIRandomState=context=>{
 const result={};
 for(const name of ['BattleRandom','PlayerAbilitySelectionRandom','OpponentAbilitySelectionRandom','PlayerTargetRandom','OpponentTargetRandom']){
  const random=RuntimeApi.readRef(context,'<'+name+'>k__BackingField');
  result[name]=random.isNull()?null:String(RuntimeApi.readValue(random,'_seed','i64'));
 }
 return result;
};

globalThis.copyOriginalAIRandomStreams=context=>{
 const R=RuntimeApi,S=SceneApi,handles=[],streams={},copies=new Map();let disposed=false;
 const names=['BattleRandom','PlayerAbilitySelectionRandom','OpponentAbilitySelectionRandom','PlayerTargetRandom','OpponentTargetRandom'];
 function dispose(){if(disposed)return;disposed=true;for(const handle of handles)R.release(handle);}
 function requireOpen(){if(disposed)throw Error('Random copy already disposed');}
 try{
  for(const name of names){
   const source=R.readRef(context,'<'+name+'>k__BackingField');
   if(source.isNull()||R.fullName(R.classOf(source))!=='JavaRandom')throw Error('Original JavaRandom stream required: '+name);
   let copy=copies.get(source.toString());
   if(!copy){
    copy=S.invoke('System.Object','MemberwiseClone',source);
    if(copy.isNull()||copy.equals(source)||R.fullName(R.classOf(copy))!=='JavaRandom')throw Error('Independent JavaRandom copy required');
    handles.push(R.retain(copy));
    if(String(R.readValue(copy,'_seed','i64'))!==String(R.readValue(source,'_seed','i64')))throw Error('Copied random seed changed');
    copies.set(source.toString(),copy);
   }
   streams[name]=copy;
  }
 }catch(error){dispose();throw error;}
 return {dispose,state(){requireOpen();return Object.fromEntries(names.map(n=>[n,String(R.readValue(streams[n],'_seed','i64'))]));},
  nextInt(name,bound){requireOpen();if(!Object.hasOwn(streams,name)||!Number.isInteger(bound)||bound<1||bound>2147483647)throw Error('Invalid private random draw');return R.unbox(S.invoke('JavaRandom','nextInt',streams[name],[R.arg.value('i32',bound)]),'i32');}};
};

globalThis.verifyOriginalAIRandomCopies=context=>{
 const before=readOriginalAIRandomState(context);let a,b;
 try{
  a=copyOriginalAIRandomStreams(context);b=copyOriginalAIRandomStreams(context);
  const rows=Object.keys(before).map(name=>{
   const first=[1,7,100,2147483647].map(bound=>a.nextInt(name,bound));
   const second=[1,7,100,2147483647].map(bound=>b.nextInt(name,bound));
   return {name,first,second,equal:JSON.stringify(first)===JSON.stringify(second)};
  });
  const after=readOriginalAIRandomState(context);
  const unchanged=JSON.stringify(before)===JSON.stringify(after);
  if(!unchanged||rows.some(r=>!r.equal))throw Error('Private random copy verification failed');
  return {before,after,originalUnchanged:unchanged,rows,scope:'Private RNG copies only; not an isolated AI evaluator'};
 }finally{a?.dispose();b?.dispose();}
};
globalThis.readOriginalAITree=tree=>{
 const R=RuntimeApi,S=SceneApi,indices=new Map(),nodes=[];
 const ref=(p,f)=>R.readRef(p,f),value=(p,f,t)=>R.readValue(p,f,t);
 function branch(result){
  if(result.isNull())return null;
  return {next:visit(ref(result,'_nextNode')),abilityTag:S.string(ref(result,'_choosenAbilityTag')),
   target:value(result,'_resultTarget','i32'),selectBasic:value(result,'<SelectBasic>k__BackingField','bool')};
 }
 function visit(node){
  if(node.isNull())return null;
  const key=node.toString();if(indices.has(key))return indices.get(key);
  if(nodes.length>=512)throw Error('Original AI graph exceeds 512 nodes');
  const index=nodes.length;indices.set(key,index);nodes.push(null);
  const parameters=S.array(ref(node,'_params'));
  if(parameters.length>64)throw Error('Original AI node exceeds 64 parameters');
  nodes[index]={index,type:value(node,'_decisionType','i32'),target:value(node,'_decisionTarget','i32'),
   excludeSelf:value(node,'_decisionTargetExcludeSelf','bool'),parameters:parameters.map(p=>S.string(p)),
   modifier:value(node,'_paramModifier','i32'),chance:value(node,'<Chance>k__BackingField','f32'),
   negate:value(node,'<Negate>k__BackingField','bool'),
   success:branch(ref(node,'<SuccessResult>k__BackingField')),failure:branch(ref(node,'<FailureResult>k__BackingField'))};
  return index;
 }
 if(tree.isNull())throw Error('Original AI tree absent');
 const root=visit(ref(tree,'_rootNode'));
 return {graph:{id:S.string(ref(tree,'_id')),evaluateBasic:value(tree,'<ShouldEvaluateBasicAbility>k__BackingField','bool'),root,nodes},
  indexOf:node=>indices.get(node.toString())??null};
};

globalThis.installOriginalAITrace=context=>{
 const R=RuntimeApi,S=SceneApi,A=R.arg,rows=[],errors=[],hooks=[],active=new Map(),trees=new Map();let serial=0,dropped=0;
 let randomCopyAudit=null,auditAttempted=false;
 const ref=(p,f)=>R.readRef(p,f),text=p=>S.string(p);
 const owns=p=>!p.isNull()&&ref(p,'_battleContext').equals(context);
 const call=(p,n,args=[])=>R.invoke(R.findMethod(R.classOf(p),n,args.length),p,args);
 function unit(p){if(p.isNull())return null;const u=ref(p,'_unitDO');return {baseId:text(ref(ref(u,'<Definition>k__BackingField'),'BaseId')),owner:R.unbox(call(p,'get_Owner'),'i32')};}
 function targets(p){if(p.isNull())return [];const n=R.readValue(p,'_size','i32'),items=ref(p,'_items');if(n<0||n>64)throw Error('AI target list bounds');return Array.from({length:n},(_,i)=>unit(items.add(32+i*8).readPointer()));}
 function inspect(fn){try{return fn();}catch(e){if(errors.length<10)errors.push(String(e));return null;}}
 function hook(name,count,types,handlers){try{hooks.push(Interceptor.attach(R.findMethod(R.findClass('BattleAbilityAIController'),name,count,types).readPointer(),handlers));}catch(e){for(const h of hooks)h.detach();throw e;}}
 hook('GetAbilityAIResult',1,['BattleAbilityAIContext'],{
  onEnter(args){inspect(()=>{
   if(!owns(args[0]))return;
   if(rows.length>=128){dropped++;return;}
   this.thread=Process.getCurrentThreadId();this.previous=active.get(this.thread);
   this.row={sequence:++serial,actor:unit(ref(args[1],'Self')),chance:R.readValue(args[1],'Chance','i32'),nodes:[],results:[]};
   this.row.randomBefore=readOriginalAIRandomState(context);
   // Streams are initialized by original battle entry, not fixture allocation.
   // Audit at the first real evaluator entry, before its original body runs.
   if(!auditAttempted){auditAttempted=true;randomCopyAudit=inspect(()=>verifyOriginalAIRandomCopies(context));}
   active.set(this.thread,this.row);rows.push(this.row);
   // Native 16A2EC0 reads context.GameDataDomainController._abilityDecisionTreeDO
   // directly, bypassing the GetAbilityDecisionTree call site.
   const tree=globalThis.trialAIPriority?.treeFor(this.row.actor.owner)||ref(ref(context,'<GameDataDomainController>k__BackingField'),'_abilityDecisionTreeDO');
   const key=tree.toString();
   if(!trees.has(key)){
    if(trees.size>=16)throw Error('Too many original AI trees');
    trees.set(key,readOriginalAITree(tree));
   }
   const record=trees.get(key);this.row.treeId=record.graph.id;this.row.treeGraph=Array.from(trees.keys()).indexOf(key);
   this.row.evaluateBasic=record.graph.evaluateBasic;

  });},
  onLeave(value){if(!this.row)return;inspect(()=>{
   this.row.randomAfter=readOriginalAIRandomState(context);
   this.row.randomChanged=Object.keys(this.row.randomBefore).filter(k=>this.row.randomBefore[k]!==this.row.randomAfter[k]);
   if(value.isNull()){this.row.result=null;return;}
   const ability=ref(value,'Ability');
   this.row.result={abilityId:ability.isNull()?null:text(call(ref(ability,'<AbilityDO>k__BackingField'),'get_Id')),enemyTargets:targets(ref(value,'EnemyTargets')),allyTargets:targets(ref(value,'AllyTargets'))};
  });if(this.previous)active.set(this.thread,this.previous);else active.delete(this.thread);}
 });
 hook('GetAbilityDecisionTree',1,['BattleAbilityAIContext'],{
  onEnter(args){inspect(()=>{this.row=owns(args[0])?active.get(Process.getCurrentThreadId()):null;});},
  onLeave(value){if(this.row&&!value.isNull())inspect(()=>{this.row.treeId=text(ref(value,'_id'));this.row.evaluateBasic=R.readValue(value,'<ShouldEvaluateBasicAbility>k__BackingField','bool');});}
 });
 hook('GetAbilityAIResult',2,['AbilityDecisionNodeDO','BattleAbilityAIContext'],{
  onEnter(args){inspect(()=>{
   if(!owns(args[0]))return;const row=active.get(Process.getCurrentThreadId());if(!row||row.nodes.length>=128||args[1].isNull())return;
   const node=args[1],record=Array.from(trees.values())[row.treeGraph];row.nodes.push({nodeIndex:record?.indexOf(node)??null,type:R.readValue(node,'_decisionType','i32'),target:R.readValue(node,'_decisionTarget','i32'),parameters:S.array(ref(node,'_params')).map(text),modifier:R.readValue(node,'_paramModifier','i32'),chance:R.readValue(node,'<Chance>k__BackingField','f32'),negate:R.readValue(node,'<Negate>k__BackingField','bool')});
  });}
 });
 hook('ProcessResult',4,['AbilityDecisionResult','System.Collections.Generic.List<BattleUnitController>','BattleAbilityAIContext','BattlePreferredTargetsParams'],{
  onEnter(args){inspect(()=>{
   if(!owns(args[0]))return;const row=active.get(Process.getCurrentThreadId());if(!row||row.results.length>=128||args[1].isNull())return;
   row.results.push({abilityTag:text(ref(args[1],'_choosenAbilityTag')),selectBasic:R.readValue(args[1],'<SelectBasic>k__BackingField','bool'),target:R.readValue(args[1],'_resultTarget','i32')});
  });}
 });
 let stopped=false;
 return {snapshot:()=>({kind:'original-ai-trace',scope:'observed original decisions; not a pre-turn prediction',rows,trees:Array.from(trees.values(),record=>record.graph),errors,dropped,randomCopyAudit}),stop(){if(stopped)return;stopped=true;for(const h of hooks)h.detach();}};
};
