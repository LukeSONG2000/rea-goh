'use strict';
// Presentation only. No battle setters, input commands, account writes or RPCs.
(()=>{
 const R=RuntimeApi,S=SceneApi,A=R.arg,call=S.invoke,hooks=[],popups=new Map(),targets=new Map();
 const marker='\n[80DFFF]',stats={shows:0,updates:0,modeGateCalls:0,tutorialDisplays:0,errors:[],healthAnomalies:[],last:null,target:null};
 let stopped=false,refreshTimer=null,refreshPending=false;
 const list=l=>l.isNull()?[]:S.array(R.readRef(l,'_items')).slice(0,R.readValue(l,'_size','i32'));
 const active=o=>!!R.unbox(call('UnityEngine.GameObject','get_activeInHierarchy',S.gameObject(o)),'bool');
 const withString=(text,fn)=>{const string=R.api('string_new','pointer',['pointer'])(Memory.allocUtf8String(text)),handle=R.retain(string);try{return fn(string);}finally{R.release(handle);}};
 const setText=(label,text)=>withString(text,string=>call('UILabel','set_text',label,[A.ref(string)]));
 function safe(fn){try{return fn();}catch(e){const message=String(e);if(!stats.errors.includes(message)){stats.errors.push(message);send({kind:'battle-info-error',error:message,stack:e.stack});}}}
 function hook(type,name,n,callbacks){hooks.push(Interceptor.attach(R.findMethod(R.findClass(type),name,n).readPointer(),callbacks));}
 function setVector(t,name,values){const p=Memory.alloc(4*values.length);values.forEach((v,i)=>p.add(i*4).writeFloat(v));call('UnityEngine.Transform','set_'+name,t,[A.byRef(p)]);}
 function makeLabel(popup,options={}){
  const template=R.readRef(popup,'_ultimateChargeLabel');
  const clone=call('UnityEngine.Object','Instantiate',R.nil,[A.ref(S.gameObject(template)),A.ref(options.parent||S.parent(S.transform(template))),A.value('bool',false)],['UnityEngine.Object','UnityEngine.Transform','System.Boolean']);
  withString(options.name||'REA_AbsoluteHealth',name=>call('UnityEngine.Object','set_name',clone,[A.ref(name)]));
  const label=call('UnityEngine.GameObject','GetComponent',clone,[A.ref(S.type('UILabel'))],['System.Type']);
  for(const anchor of ['leftAnchor','rightAnchor','topAnchor','bottomAnchor']){const a=R.readRef(label,anchor);if(!a.isNull())R.writeRef(a,'target',R.nil);}
  call('UIWidget','set_pivot',label,[A.value('i32',options.pivot||0)]);
  call('UIWidget','set_width',label,[A.value('i32',options.width||450)]);
  call('UIWidget','set_height',label,[A.value('i32',110)]);
  call('UILabel','set_fontSize',label,[A.value('i32',options.font||20)]);
  call('UILabel','set_maxLineCount',label,[A.value('i32',4)]);
  setVector(S.transform(label),'localPosition',options.position||[-205,-75,0]);
  const color=Memory.alloc(16);[1,1,1,1].forEach((v,i)=>color.add(i*4).writeFloat(v));call('UIWidget','set_color',label,[A.byRef(color)]);
  call('UnityEngine.GameObject','SetActive',clone,[A.value('bool',true)]);
  return label;
 }
 function healthText(unit){
  const read=n=>R.unbox(call('BattleUnitController','get_'+n,unit),'i32');
  const values={};
  // Original CurrentShield caps the usable total at max(0, MaxShield).
  // A negative native ceiling can coexist with retained permanent shield;
  // expose it as a restriction, without rewriting either native reading.
  const pair=(label,a,b)=>{values[a]=read(a);values[b]=read(b);return label+' '+values[a]+' / '+(values[b]<0?'受限('+values[b]+')':values[b]);};
  const text=[pair('生命','CurrentHP','MaxHP'),pair('总护盾','CurrentShield','MaxShield'),pair('永久护盾','CurrentPermanentShield','MaxPermanentShield'),pair('临时护盾','CurrentBonusShield','MaxBonusShield')].join('\n');
  if(Object.values(values).some(v=>v<0)&&stats.healthAnomalies.length<16){
   const key=unit.toString()+':'+JSON.stringify(values);
   if(!stats.healthAnomalies.some(row=>row.key===key)){
    const row={key,values,observedAt:Date.now(),unitType:R.fullName(R.classOf(unit))};
    // Preserve native values. Do not clamp a negative stat or substitute a
    // previous target's maximum before the source of the anomaly is known.
    try{
     row.maxShieldStat=R.unbox(call('BattleUnitController','GetStatValue',unit,[A.value('u8',25)],['UnitStatEnum']),'f32');
     row.unitId=S.string(R.readRef(call('BattleUnitController','get_UnitDO',unit),'Id'));
    }catch(e){row.diagnosticError=String(e);}
    stats.healthAnomalies.push(row);send({kind:'battle-info-health-anomaly',...row});
   }
  }
  return text;
 }
 function layoutTarget(item){
  const name=R.readRef(item.target,'_nameLabel'),parent=S.parent(S.transform(name));
  const component=R.readRef(item.view,'_opponentAbilitiesComponent');
  const controls=[name];
  if(!component.isNull())for(const field of ['_abilityButtonGrid','_leaderAndUniqueIconsGrid']){
   const control=R.readRef(component,field);if(!control.isNull())controls.push(control);
  }
  const bounds=control=>{
   const box=call('NGUIMath','CalculateRelativeWidgetBounds',R.nil,[A.ref(parent),A.ref(S.transform(control))],['UnityEngine.Transform','UnityEngine.Transform']);
   const p=R.api('object_unbox','pointer',['pointer'])(box);
   return {left:p.readFloat()-p.add(12).readFloat(),right:p.readFloat()+p.add(12).readFloat()};
  };
  let left=null;
  const measured=[];
  for(const control of controls){
   const isActive=active(control);
   if(!isActive){measured.push({active:false});continue;}
   const measuredBounds=bounds(control);measured.push({active:true,...measuredBounds});
   const minX=measuredBounds.left;
   if(Number.isFinite(minX)&&(left===null||minX<left))left=minX;
  }
  if(left===null){stats.targetLayout={measured,unavailable:true};return;}
  const position=S.vector(S.transform(name),'localPosition');
  setVector(S.transform(item.label),'localPosition',[left-16,position[1],position[2]]);
  // A cloned label can carry scaling/geometry offsets. Correct its actual
  // right edge rather than treating the Transform origin as the edge.
  const actual=bounds(item.label),delta=left-16-actual.right;
  if(Number.isFinite(delta))setVector(S.transform(item.label),'localPosition',[left-16+delta,position[1],position[2]]);
  stats.targetLayout={controlsLeft:left,label:bounds(item.label),gap:16,measured};
 }
 function refreshCurrentTarget(){
  const state=R.readRef(R.findBound('StateController'),'<CurrentState>k__BackingField');
  if(state.isNull()||R.fullName(R.classOf(state))!=='BattleGameState'){stats.target=null;return;}
  const controller=R.readRef(state,'_battleController');if(controller.isNull())return;
  const context=R.readRef(controller,'_battleContext');if(context.isNull())return;
  const view=call('BattleContext','get_BattleView',context);
  if(view.isNull())return;
  const target=R.readRef(view,'_opponentTargetComponent'),item=targets.get(target.toString());
  if(!item||!item.view.equals(view)||!active(target)){stats.target=null;return;}
  if(item.unit.isNull()){setText(item.label,'');stats.target=null;return;}
  const text=healthText(item.unit);setText(item.label,text);stats.target=text;
  for(const row of stats.healthAnomalies)if(!row.deferred&&row.key.startsWith(item.unit.toString()+':')){
   row.deferred={observedAt:Date.now(),text};
   send({kind:'battle-info-health-deferred',key:row.key,...row.deferred});
  }
  layoutTarget(item);
 }
 function updateTargetItem(item,unit){
  item.unit=unit;
  if(unit.isNull()){setText(item.label,'');stats.target=null;return;}
  const text=healthText(unit);setText(item.label,text);stats.target=text;
 }
 function renderTarget(view,unit){
  const target=R.readRef(view,'_opponentTargetComponent');if(target.isNull())return;
  let item=targets.get(target.toString());
  if(unit.isNull()){if(item)updateTargetItem(item,unit);else stats.target=null;return;}
  if(!item){
   const name=R.readRef(target,'_nameLabel'),transform=S.transform(name),position=S.vector(transform,'localPosition');
   const label=makeLabel(R.readRef(view,'_effectDetailsPopup'),{parent:S.parent(transform),name:'REA_TargetAbsoluteHealth',pivot:2,width:350,font:18,position:[position[0],position[1],position[2]]});
   item={target,label,unit,view};targets.set(target.toString(),item);
  }
  layoutTarget(item);
  updateTargetItem(item,unit);
 }
 function render(popup,unit){
  if(unit.isNull())return;
  const key=popup.toString();let item=popups.get(key);
  if(!item){item={popup,unit,label:makeLabel(popup),rows:new Map(),table:S.transform(R.readRef(popup,'_effectsTable')),lastTime:0};item.tablePosition=S.vector(item.table,'localPosition');popups.set(key,item);}
  if(!item.scrollPanel){
   const scroll=R.readRef(popup,'_effectsScrollView');
   item.scrollPanel=call('UnityEngine.GameObject','GetComponent',S.gameObject(scroll),[A.ref(S.type('UIPanel'))],['System.Type']);
   item.topAnchor=R.readRef(item.scrollPanel,'topAnchor');
   item.topAbsolute=R.readValue(item.topAnchor,'absolute','i32');
   R.writeValue(item.topAnchor,'absolute','i32',item.topAbsolute-140);
   call('UIRect','ResetAndUpdateAnchors',item.scrollPanel);
  }
  item.unit=unit;
  const status=readBattleStatus(unit);
  const text=healthText(unit);
  setText(item.label,text);
  call('UnityEngine.GameObject','SetActive',S.gameObject(item.label),[A.value('bool',true)]);
  const displayed=[];
  for(const row of list(R.readRef(popup,'_effectItemsPool'))){
   if(row.isNull()||!active(row))continue;
   const sprite=R.readRef(row,'_battleEffectDetails'),label=R.readRef(row,'_battleEffectDescription');
   const icon=S.string(call('BattleEffectSprite','get_EffectSpriteName',sprite));
   const overlay=S.string(call('BattleEffectSprite','get_EffectOverlaySpriteName',sprite))||'';
   const matches=status.effects.filter(e=>e.iconSprite===icon&&(e.overlaySprite||'')===overlay);
   const base=S.string(call('UILabel','get_text',label)).split(marker)[0];
   let saved=item.rows.get(label.toString());
   if(!saved){
    const widget=call('UnityEngine.GameObject','GetComponent',S.gameObject(row),[A.ref(S.type('UIWidget'))],['System.Type']);
    saved={label,base,widget,overflow:R.unbox(call('UILabel','get_overflowMethod',label),'i32'),lines:R.unbox(call('UILabel','get_maxLineCount',label),'i32'),height:widget.isNull()?null:R.unbox(call('UIWidget','get_height',widget),'i32')};
    item.rows.set(label.toString(),saved);
   }else saved.base=base;
   call('UILabel','set_overflowMethod',label,[A.value('i32',3)]);
   call('UILabel','set_maxLineCount',label,[A.value('i32',0)]);
   const durations=[...new Set(matches.map(e=>e.durationText))];
   const suffix=durations.length?durations.join('；'):'未读取到计时（原显示标记）';
   setText(label,base+marker+suffix+'[-]');
   const size=R.api('object_unbox','pointer',['pointer'])(call('UILabel','get_printedSize',label));
   if(!saved.widget.isNull())call('UIWidget','set_height',saved.widget,[A.value('i32',Math.max(saved.height,Math.ceil(size.add(4).readFloat())+12))]);
   displayed.push({icon,suffix,instances:matches.length});
  }
  call('UITable','Reposition',R.readRef(popup,'_effectsTable'));
  item.lastTime=Date.now();stats.updates++;stats.last={health:text,effects:displayed};
 }
 function refreshOpponent(enabled=true){
  const state=R.readRef(R.findBound('StateController'),'<CurrentState>k__BackingField');
  if(R.fullName(R.classOf(state))!=='BattleGameState')return;
  const context=R.readRef(R.readRef(state,'_battleController'),'_battleContext');
  if(enabled===null){
   // Resolve only from the current battle. A previous battle's helper may
   // already have been collected when the UI host is stopped in the tavern.
   const helper=call('BattleContext','get_BattleOpponentAbilityInfoEnabledHelper',context);
   if(helper.isNull())return;
   enabled=R.unbox(call('BattleOpponentAbilityInfoEnabledController','IsEnabled',helper),'bool');
  }
  const view=call('BattleContext','get_BattleView',context);
  call('BattleBaseView','FilterOpponentAbilityDisplay',view,[A.value('bool',enabled)]);
 }
 function showEmptyPopup(view,unit,effects){
  const data=call('UnitListItemRendererData+Factory','Get',R.readRef(view,'_unitListItemRenderDataFactory'),[
   A.ref(call('BattleUnitController','get_UnitDO',unit)),A.ref(R.nil),A.value('i32',-1),A.value('bool',false),A.value('bool',false),A.value('i32',R.readValue(view,'_battleMode','i32'))]);
  const health=call('UnitHealthRenderDataFactory','Get',R.readRef(view,'_UnitHealthRenderDataFactory'),[A.ref(unit)],['IBattleUnit']);
  call('UnitListItemRendererData','set_HealthRenderData',data,[A.ref(health)]);
  call('UnitListItemRendererData','set_ShowHealthBar',data,[A.value('bool',true)]);
  const popup=R.readRef(view,'_effectDetailsPopup');
  call('BattleEffectsPopupView','Show',popup,[A.ref(unit),A.ref(data),A.ref(effects),A.ref(R.readRef(view,'_resourceLoader')),A.ref(R.readRef(view,'_localizationManager'))]);
  // Calls originating inside an interceptor may bypass nested interceptors.
  stats.shows++;render(popup,unit);
 }
 R.runOnMainThreadOnce(()=>{
  hook('BattleOpponentAbilityInfoEnabledController','IsEnabled',0,{onLeave(result){if(!stopped){result.replace(1);stats.modeGateCalls++;}}});
  hook('BattleBaseView','FilterOpponentAbilityDisplay',1,{onEnter(args){if(!stopped)args[1]=ptr(1);}});
  hook('BattleBaseView','SetupOpponentAbilities',4,{
   onEnter(args){this.view=args[0];this.values=[args[1],args[2],args[3],args[4]];},
   onLeave(){if(stopped)return;safe(()=>{
    if(R.unbox(call('BattleBaseView','get_IsInOnboarding',this.view),'bool')){
     const c=R.readRef(this.view,'_opponentAbilitiesComponent');if(c.isNull())return;
     call('BattleAbilityComponent','SetupAbilities',c,[...this.values.map(A.ref),A.ref(R.nil),A.ref(R.readRef(this.view,'_keyboardBindingDomainController')),A.value('i32',0),A.value('i32',0)]);
     call('BattleAbilityComponent','ToggleAbilityButtonsVisible',c,[A.value('bool',true)]);stats.tutorialDisplays++;
    }
    call('BattleBaseView','FilterOpponentAbilityDisplay',this.view,[A.value('bool',true)]);
    const target=R.readRef(this.view,'_opponentTargetComponent'),item=targets.get(target.toString());
    if(item)layoutTarget(item);
   });}
  });
  hook('BattleBaseView','SetOpponentUnitData',1,{onEnter(args){this.view=args[0];this.unit=args[1];},onLeave(){if(!stopped)safe(()=>renderTarget(this.view,this.unit));}});
  hook('BattleTargetComponent','SetHealth',1,{onEnter(args){this.target=args[0];this.unit=args[1];},onLeave(){if(!stopped)safe(()=>{const item=targets.get(this.target.toString());if(item)updateTargetItem(item,this.unit);});}});
  hook('BattleEffectsPopupView','Show',5,{onEnter(args){this.popup=args[0];this.unit=args[1];},onLeave(){if(!stopped)safe(()=>{stats.shows++;render(this.popup,this.unit);});}});
  hook('BattleBaseView','HandleEffectsLongPressed',3,{onEnter(args){this.view=args[0];this.unit=args[1];this.effects=args[2];this.updateOnly=args[3].toInt32()!==0;this.before=stats.shows;},onLeave(){if(!stopped&&!this.updateOnly&&stats.shows===this.before)safe(()=>showEmptyPopup(this.view,this.unit,this.effects));}});
  hook('BattleEffectsPopupView','UpdateHealth',2,{onEnter(args){this.popup=args[0];this.unit=args[1];},onLeave(){if(!stopped)safe(()=>render(this.popup,this.unit));}});
  hook('BattleEffectsPopupView','Update',0,{onEnter(args){this.popup=args[0];},onLeave(){if(stopped)return;const item=popups.get(this.popup.toString());if(item&&Date.now()-item.lastTime>300)safe(()=>{if(R.unbox(call('BattleEffectsPopupView','get_IsDisplaying',this.popup),'bool'))render(this.popup,item.unit);});}});
  safe(refreshOpponent);
  refreshTimer=setInterval(()=>{
   if(stopped||refreshPending)return;
   refreshPending=true;
   R.runOnMainThreadOnce(()=>{if(!stopped)safe(refreshCurrentTarget);})
    .catch(e=>{if(!stopped)safe(()=>{throw e;});}).finally(()=>{refreshPending=false;});
  },300);
  send({kind:'battle-info-installed',pid:Process.id,scope:'UI only',hooks:hooks.length});
 }).catch(error=>send({kind:'battle-info-error',error:String(error)}));
 globalThis.battleInfoStatus=()=>stats;
 globalThis.battleInfoTestPopup=showEmptyPopup;
 globalThis.battleInfoTestTarget=renderTarget;
 globalThis.stopBattleInfo=async()=>{
  stopped=true;if(refreshTimer!==null)clearInterval(refreshTimer);hooks.forEach(h=>h.detach());
  await R.runOnMainThreadOnce(()=>{
   const alive=new Set(S.all('BattleEffectsPopupView').map(p=>p.toString()));
   for(const item of popups.values())if(alive.has(item.popup.toString()))safe(()=>{
    call('UnityEngine.GameObject','SetActive',S.gameObject(item.label),[A.value('bool',false)]);
    for(const row of item.rows.values()){
     setText(row.label,row.base);
     call('UILabel','set_overflowMethod',row.label,[A.value('i32',row.overflow)]);
     call('UILabel','set_maxLineCount',row.label,[A.value('i32',row.lines)]);
     if(!row.widget.isNull())call('UIWidget','set_height',row.widget,[A.value('i32',row.height)]);
    }
    R.writeValue(item.topAnchor,'absolute','i32',item.topAbsolute);
    call('UIRect','ResetAndUpdateAnchors',item.scrollPanel);
    call('UITable','Reposition',R.readRef(item.popup,'_effectsTable'));
    setVector(item.table,'localPosition',item.tablePosition);
   });
   const liveTargets=new Set(S.all('BattleTargetComponent').map(p=>p.toString()));
   for(const item of targets.values())if(liveTargets.has(item.target.toString()))safe(()=>call('UnityEngine.GameObject','SetActive',S.gameObject(item.label),[A.value('bool',false)]));
   safe(()=>refreshOpponent(null));
  });return stats;
 };
 rpc.exports={status:()=>stats,stop:()=>globalThis.stopBattleInfo()};
})();
