'use strict';
// Observe original calls only. Never invokes the decision evaluator or random stream.
globalThis.installOriginalAITrace=context=>{
 const R=RuntimeApi,S=SceneApi,A=R.arg,rows=[],errors=[],hooks=[],active=new Map();let serial=0,dropped=0;
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
   active.set(this.thread,this.row);rows.push(this.row);
  });},
  onLeave(value){if(!this.row)return;inspect(()=>{
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
   const node=args[1];row.nodes.push({type:R.readValue(node,'_decisionType','i32'),target:R.readValue(node,'_decisionTarget','i32'),parameters:S.array(ref(node,'_params')).map(text),modifier:R.readValue(node,'_paramModifier','i32'),chance:R.readValue(node,'<Chance>k__BackingField','f32'),negate:R.readValue(node,'<Negate>k__BackingField','bool')});
  });}
 });
 hook('ProcessResult',4,['AbilityDecisionResult','System.Collections.Generic.List<BattleUnitController>','BattleAbilityAIContext','BattlePreferredTargetsParams'],{
  onEnter(args){inspect(()=>{
   if(!owns(args[0]))return;const row=active.get(Process.getCurrentThreadId());if(!row||row.results.length>=128||args[1].isNull())return;
   row.results.push({abilityTag:text(ref(args[1],'_choosenAbilityTag')),selectBasic:R.readValue(args[1],'<SelectBasic>k__BackingField','bool'),target:R.readValue(args[1],'_resultTarget','i32')});
  });}
 });
 let stopped=false;
 return {snapshot:()=>({kind:'original-ai-trace',scope:'observed original decisions; not a pre-turn prediction',rows,errors,dropped}),stop(){if(stopped)return;stopped=true;for(const h of hooks)h.detach();}};
};
