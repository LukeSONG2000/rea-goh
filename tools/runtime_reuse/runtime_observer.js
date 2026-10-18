'use strict';
// Snapshot is read-only; separate explicit RPC exports perform original UI navigation.

// Inventory reads are explicit requests: do not enumerate the full roster on
// every battle-info tick. Only the currently open detail is polled.
globalThis.InventoryBridge=(()=>{
 const R=RuntimeApi,S=SceneApi;
 const current=()=>R.readRef(R.findBound('StateController'),'<CurrentState>k__BackingField');
 function list(value){
  if(value.isNull())return [];
  const items=R.readRef(value,'_items'),n=R.readValue(value,'_size','i32');
  if(n<0||n>4096||(items.isNull()?n!==0:n>items.add(24).readU64().toNumber()))throw Error('Inventory list bounds invalid');
  return Array.from({length:n},(_,i)=>items.add(32+i*8).readPointer());
 }
 function unitInfo(unit){
  const definition=R.readRef(unit,'<Definition>k__BackingField');
  return {unitId:S.string(R.readRef(unit,'Id')),baseId:S.string(R.readRef(definition,'BaseId')),loaned:!!R.unbox(S.invoke('UnitDO','get_IsLoanedUnit',unit),'bool'),
   level:R.readValue(unit,'<CurrentLevel>k__BackingField','i32'),rarity:R.readValue(unit,'<CurrentRarity>k__BackingField','u8'),tier:R.readValue(unit,'<CurrentTier>k__BackingField','u8')};
 }
 function skills(unit){
  const definition=R.readRef(unit,'<Definition>k__BackingField'),entries=[];
  for(const skill of list(R.readRef(definition,'_skillDefinitions')))entries.push({skill,source:'unit',crewMemberId:null});
  for(const slot of list(R.readRef(definition,'_crewMemberSlots'))){
   const crewMemberId=S.string(R.readRef(slot,'<CrewMemberId>k__BackingField'));
   for(const skill of list(R.readRef(slot,'<CrewSkillDef>k__BackingField')))entries.push({skill,source:'crew',crewMemberId});
  }
  if(entries.length>256)throw Error('Inventory skill definition bound exceeded');
  return entries.map(({skill,source,crewMemberId})=>{
   const id=R.readRef(skill,'Id'),args=[R.arg.ref(id)];
   const present=!!R.unbox(S.invoke('UnitDO','HasSkill',unit,args,['System.String']),'bool');
   // GetSkillTier is an indexer and throws for absent skills. Never guess zero.
   const currentTier=present?R.unbox(S.invoke('UnitDO','GetSkillTier',unit,args,['System.String']),'i32'):null;
   const reference=R.readRef(skill,'AbilityReferenceId');
   const ability=S.invoke('UnitDO','GetAbility',unit,[R.arg.ref(reference)],['System.String']);
   return {skillId:S.string(id),abilityReferenceId:S.string(reference),
    skillDefinitionNameKey:S.string(R.readRef(skill,'NameKey')),
    abilityNameKey:ability.isNull()?null:S.string(S.invoke('AbilityDO','get_NameKey',ability)),
    abilityType:ability.isNull()?null:R.unbox(S.invoke('AbilityDO','get_AbilityType',ability),'u8'),
    baseCooldown:ability.isNull()?null:R.readValue(ability,'CooldownMax','i16'),source,crewMemberId,present,currentTier,
    tierCount:R.unbox(S.invoke('SkillDefinitionDO','GetTotalTiers',skill),'i32'),
    requiredUnitTier:R.readValue(skill,'RequiredTier','u8'),requiredRelicTier:R.readValue(skill,'RequiredRelicTier','u8'),
    skillType:R.readValue(skill,'SkillType','i32'),omicronMode:R.readValue(skill,'OmicronMode','i32'),
    definitionIsZeta:R.readValue(skill,'IsZeta','bool')};
  });
 }
 function inventory(){
  const state=current();if(state.isNull()||R.fullName(R.classOf(state))!=='MasterInventoryGameState')throw Error('Inventory command requires the collection screen');
  const master=R.readRef(state,'_masterInventoryController'),data=R.readRef(master,'_transitionData');
  if(!R.readRef(data,'<ReadOnlyRoster>k__BackingField').isNull())throw Error('Inventory interface requires the player collection');
  const kind=R.readValue(data,'<Type>k__BackingField','i32');
  if(![1,2].includes(kind))throw Error('Select characters or ships first');
  const controller=R.readRef(master,kind===1?'_characterInventoryController':'_shipInventoryController');
  const view=R.readRef(controller,kind===1?'_inventoryView':'_shipInventoryView');
  if(view.isNull())throw Error('Inventory view has not been initialized');
  if(!R.unbox(S.invoke('UnityEngine.GameObject','get_activeInHierarchy',S.gameObject(view)),'bool'))throw Error('Inventory view is not ready');
  return {kind,controller,rows:list(R.readRef(view,'_filterData'))};
 }
 function abilityOwner(controller,type){
  if(type==='ShipDetailsState')return controller;
  const panels=R.readRef(controller,'_unitDetailsInfoPanelsController');
  return panels.isNull()?R.nil:R.readRef(panels,'_unitDetailsAbilityController');
 }
 function displayRows(controller,type){
   let rows=null;
   if(type==='CharacterDetailsState'){
    const panels=R.readRef(controller,'_unitDetailsInfoPanelsController');
    const abilities=panels.isNull()?R.nil:R.readRef(panels,'_unitDetailsAbilityController');
    if(!abilities.isNull())rows=list(R.readRef(abilities,'_abilityViewData')).map(data=>({data,container:'character'}));
   }else{
    const viewData=R.readRef(controller,'_currentData');
    if(!viewData.isNull()){
     rows=[];
     const append=(group,container,crewSlot=null)=>{if(!group.isNull())for(const data of list(R.readRef(group,'<AbilityData>k__BackingField')))rows.push({data,container,crewSlot});};
     append(R.readRef(viewData,'<ShipAbilityData>k__BackingField'),'ship');
     append(R.readRef(viewData,'<AdvancedAbilityData>k__BackingField'),'advanced');
     list(R.readRef(viewData,'<CrewAbilityData>k__BackingField')).forEach((group,index)=>append(group,'crew',index));
    }
   }
   if(rows!==null&&rows.length>256)throw Error('Skill display bound exceeded');
   return rows;
 }
 function skillMaterials(data){
  const recipe=R.readRef(data,'<RecipeData>k__BackingField');
  if(recipe.isNull())return null;
  const count=R.readValue(recipe,'_size','i32');
  if(count<0||count>128)throw Error('Skill recipe bound exceeded');
  const rows=[];
  // List<KeyValuePair<...>> contains value types: use the original indexer to box
  // each pair instead of interpreting the array as a list of object pointers.
  const get=R.findMethod(R.classOf(recipe),'get_Item',1);
  for(let i=0;i<count;i++){
   const pair=R.invoke(get,recipe,[R.arg.value('i32',i)]),tile=R.readRef(pair,'value');
   if(tile.isNull())throw Error('Null skill recipe tile');
   const itemType=R.readValue(pair,'key','u8'),id=R.readRef(tile,'ID');
   const row={itemType,renderType:R.fullName(R.classOf(tile)),
    id:S.string(id),label:S.string(R.readRef(tile,'<LabelString>k__BackingField')),
    quantityRaw:R.readValue(tile,'<Quantity>k__BackingField','i32'),ownedQuantity:null};
   // ItemTypeEnum.MATERIAL. Other recipe item types require their own inventory getter.
   if(itemType===7)try{
    row.ownedQuantity=R.unbox(S.invoke('InventoryDomainController','GetCraftingMaterialQuantity',R.findBound('InventoryDomainController'),[R.arg.ref(id)],['System.String']),'i32');
   }catch(error){row.inventoryError=String(error);}
   rows.push(row);
  }
  return rows;
 }
 function skillLevel(data){
  if(data.isNull())return null;
  const text=name=>S.string(R.readRef(data,'<'+name+'>k__BackingField'));
  const value=(name,kind='i32')=>R.readValue(data,'<'+name+'>k__BackingField',kind);
  const result={tier:value('AbilityTier'),levelText:text('AbilityLevel'),typeText:text('AbilityLevelType'),
   description:text('AbilityDescription'),requirements:text('AbilityUpgradeRequirements'),
   cooldown:value('Cooldown'),hideCooldown:!!value('HideCooldown','bool'),
   lockedByConditions:!!value('LevelLockedByConditions','bool'),upgradeStatus:value('UpgradeStatus'),
   upgradeAvailable:!!value('UpgradeAvailable','bool'),
   currencyCost:value('RecipeCost'),currencyType:value('RecipeCurrencyType'),
   materials:skillMaterials(data),costComplete:false,currencyOwned:null};
  try{
   result.currencyOwned=R.unbox(S.invoke('InventoryDomainController','GetCurrencyQuantity',R.findBound('InventoryDomainController'),[R.arg.value('i32',result.currencyType)],['CurrencyTypeEnum']),'i32');
  }catch(error){result.currencyError=String(error);}
  return result;
 }
 function skillPopup(controller,type,raw=false){
  const owner=abilityOwner(controller,type);if(owner.isNull())return null;
  for(const [field,viewType] of [['_skillsOverlayView','UnitDetailsSkillOverlayView'],['_abilityDetailsView','AbilityDetailsView']]){
   const view=R.readRef(owner,field);if(view.isNull()||!R.unbox(S.invoke('UnityEngine.GameObject','get_activeInHierarchy',S.gameObject(view)),'bool'))continue;
   if(raw)return {view,viewType};
   if(viewType==='UnitDetailsSkillOverlayView'){
    const data=R.readRef(view,'_data');
    if(data.isNull())return {loading:true};
    const levels=list(R.readRef(data,'<AbilityLevelData>k__BackingField'));
    if(levels.length>128)throw Error('Skill level bound exceeded');
    return {kind:'levels',skillId:S.string(R.readRef(data,'<SkillId>k__BackingField')),abilityId:S.string(R.readRef(data,'<AbilityId>k__BackingField')),name:S.string(R.readRef(data,'<AbilityName>k__BackingField')),
     allowUpgrade:!!R.readValue(data,'<AllowUpgrade>k__BackingField','bool'),
     selectedLevel:skillLevel(R.readRef(view,'_selectedLevelData')),levels:levels.map(skillLevel)};
   }
   const label=R.readRef(view,'_abilityName');
   return {kind:'single',name:S.string(S.invoke('UILabel','get_text',label))};
  }
  return null;
 }
 function detail(state){
  const type=R.fullName(R.classOf(state));if(!['CharacterDetailsState','ShipDetailsState'].includes(type))return null;
  const controller=R.readRef(state,'_controller');if(controller.isNull())return {loading:true};
  const data=R.readRef(controller,'_transitionData');
  if(data.isNull())return {loading:true};
  const unit=R.readRef(data,type==='CharacterDetailsState'?'<Unit>k__BackingField':'Unit');
  if(unit.isNull())return {loading:true};
  const result={kind:type==='CharacterDetailsState'?'characters':'ships',...unitInfo(unit)};
  try{result.skills=skills(unit);}catch(error){result.skillsError=String(error);}
  try{
   const rows=displayRows(controller,type);
   result.skillDisplay=rows===null?null:rows.map(({data,container,crewSlot=null})=>({
    container,crewSlot,
    skillId:S.string(R.readRef(data,'<SkillId>k__BackingField')),
    abilityId:S.string(R.readRef(data,'<AbilityId>k__BackingField')),
    name:S.string(R.readRef(data,'<NameText>k__BackingField')),
    levelText:S.string(R.readRef(data,'<LevelText>k__BackingField')),
    currentTier:R.readValue(data,'<CurrentTierForSkill>k__BackingField','i32'),
    requiredSkillTier:R.readValue(data,'<RequiredSkillTier>k__BackingField','i32'),
    displayLevel:R.unbox(S.invoke('UnitDetailsAbilityViewData','get_CurrentTierDisplayValueForSkill',data),'i32'),
    maxed:!!R.readValue(data,'<SkillForAbilityMaxed>k__BackingField','bool'),
    locked:!!R.readValue(data,'<Locked>k__BackingField','bool')
   }));
  }catch(error){result.skillDisplayError=String(error);}
  try{result.skillPopup=skillPopup(controller,type);}catch(error){result.skillPopupError=String(error);}
  return result;
 }
 function execute(operation,request){
  if(['inventory-skill','inventory-skill-close','inventory-skill-level','inventory-skill-quote'].includes(operation)){
   const state=current(),type=R.fullName(R.classOf(state));
   if(!['CharacterDetailsState','ShipDetailsState'].includes(type))throw Error('Skill command requires a unit detail page');
   const controller=R.readRef(state,'_controller'),info=detail(state);
   if(info.loading||!request||request.unitId!==info.unitId)throw Error('Skill command unit changed or missing unitId');
   if(operation==='inventory-skill-quote'){
    const panel=info.skillPopup,selected=panel?.selectedLevel;
    if(info.skillPopupError||panel?.kind!=='levels'||panel.skillId!==request.skillId||!selected||selected.tier!==request.tier)throw Error('Quote requires the current skill and selected tier');
    const costs=selected.materials;
    const complete=Array.isArray(costs)&&costs.every(m=>m.itemType===7&&typeof m.id==='string'&&m.id.length>0&&Number.isInteger(m.quantityRaw)&&m.quantityRaw>=0&&Number.isInteger(m.ownedQuantity)&&m.ownedQuantity>=0&&!m.inventoryError)&&Number.isInteger(selected.currencyCost)&&selected.currencyCost>=0&&Number.isInteger(selected.currencyOwned)&&selected.currencyOwned>=0&&!selected.currencyError&&(selected.currencyCost===0||selected.currencyType>0);
    return {readConfirmed:true,unitId:info.unitId,skillId:panel.skillId,abilityId:panel.abilityId,tier:selected.tier,levelText:selected.levelText,
     currency:{type:selected.currencyType,required:selected.currencyCost,owned:selected.currencyOwned,error:selected.currencyError||null},
     materials:costs,displayCostsComplete:complete,
     originalConditions:{allowUpgrade:panel.allowUpgrade,upgradeAvailable:selected.upgradeAvailable,upgradeStatus:selected.upgradeStatus,lockedByConditions:selected.lockedByConditions,requirements:selected.requirements},
     executionSupported:false,scope:'Current original UI cost snapshot; not a reservation, purchase authorization or server transaction quote'};
   }
   const popup=skillPopup(controller,type,true);
   if(operation==='inventory-skill-level'){
    if(!popup||popup.viewType!=='UnitDetailsSkillOverlayView')throw Error('Level selection requires the original multi-level popup');
    if(!Number.isInteger(request.tier)||request.tier< -1||request.tier>127)throw Error('Invalid skill tier');
    const data=R.readRef(popup.view,'_data');
    if(data.isNull()||S.string(R.readRef(data,'<SkillId>k__BackingField'))!==request.skillId)throw Error('Skill popup changed');
    const levels=list(R.readRef(data,'<AbilityLevelData>k__BackingField'));
    if(levels.length>128)throw Error('Skill level bound exceeded');
    const matches=levels.filter(level=>R.readValue(level,'<AbilityTier>k__BackingField','i32')===request.tier);
    if(matches.length!==1)throw Error('Choose one tier from the original popup levels');
    S.invoke('UnitDetailsSkillOverlayView','AbilityLevelButtonClicked',popup.view,[R.arg.ref(matches[0])]);
    return {expectedState:type,unitId:info.unitId,skillId:request.skillId,selectedTier:request.tier};
   }
   if(operation==='inventory-skill-close'){
    if(!popup)throw Error('No active original skill popup');
    S.invoke(popup.viewType,'OnBackClick',popup.view);
    return {expectedState:type,unitId:info.unitId,closing:true};
   }
   if(popup)throw Error('Close the current skill popup first');
   if(typeof request.skillId!=='string'||!request.skillId.length||request.skillId.length>256)throw Error('Invalid skillId');
   const matches=(displayRows(controller,type)||[]).filter(({data})=>S.string(R.readRef(data,'<SkillId>k__BackingField'))===request.skillId);
   if(matches.length!==1)throw Error('Choose one skill from the current original detail');
   const data=matches[0].data,owner=abilityOwner(controller,type);
   if(type==='CharacterDetailsState')S.invoke('CharacterDetailsAbilityController','AbilityClicked',owner,[R.arg.ref(R.readRef(data,'<AbilityId>k__BackingField'))],['System.String']);
   else S.invoke('ShipDetailsController','AbilityClicked',owner,[R.arg.ref(data)]);
   return {expectedState:type,unitId:info.unitId,skillId:request.skillId,name:S.string(R.readRef(data,'<NameText>k__BackingField'))};
  }
  if(operation==='inventory-back'){
   const state=current(),type=R.fullName(R.classOf(state));
   if(!['CharacterDetailsState','ShipDetailsState'].includes(type))throw Error('Back requires a character or ship detail page');
   const controller=R.readRef(state,'_controller');
   S.invoke(type==='CharacterDetailsState'?'CharacterDetailsController':'FeatureController','OnBackButtonClicked',controller);
   return {expectedState:'MasterInventoryGameState',effect:'original detail back callback'};
  }
  if(!['inventory-list','inventory-unit'].includes(operation))throw Error('Unsupported inventory action');
  const live=inventory();
  const domain=R.findBound('InventoryDomainController'),loadedInventory=R.readRef(domain,'<Inventory>k__BackingField');
  const roster=R.readRef(loadedInventory,'_unitRoster');
  const ownedIds=new Set(list(R.readRef(roster,'_units')).map(unit=>S.string(R.readRef(unit,'Id'))));
  const rows=live.rows.filter(row=>!row.isNull()).map(row=>({row,unit:R.readRef(row,'<Unit>k__BackingField')}))
   .filter(r=>!r.unit.isNull()&&ownedIds.has(S.string(R.readRef(r.unit,'Id'))));
  if(operation==='inventory-list')return {readConfirmed:true,kind:live.kind===1?'characters':'ships',units:rows.filter(r=>!r.unit.isNull()).map(r=>unitInfo(r.unit)),scope:'current player-roster units in the original filtered list; loaned marked; no configuration changes'};
  if(typeof request?.unitId!=='string'||!request.unitId.length||request.unitId.length>256)throw Error('Expected owned inventory unitId');
  const chosen=rows.filter(r=>!r.unit.isNull()&&S.string(R.readRef(r.unit,'Id'))===request.unitId);
  if(chosen.length!==1)throw Error('Choose one owned unit from the current filtered inventory list');
  S.invoke(live.kind===1?'CharacterInventoryController':'ShipInventoryController',live.kind===1?'HandleOnUnitTileClicked':'OnShipInventoryTileClicked',live.controller,[R.arg.ref(chosen[0].row)]);
  return {unitId:request.unitId,expectedState:live.kind===1?'CharacterDetailsState':'ShipDetailsState',effect:'original inventory tile callback'};
 }
 return {execute,detail,readiness(){try{inventory();return {ready:true};}catch(e){return {ready:false,reason:String(e)};}}};
})();

globalThis.readRuntimeSnapshot=()=>RuntimeApi.runOnMainThreadOnce(()=>{
 const R=RuntimeApi,S=SceneApi;
 const state=R.readRef(R.findBound('StateController'),'<CurrentState>k__BackingField');
 if(state.isNull())return {schemaVersion:1,state:'Transitioning',units:[],scope:'original state transition in progress; no gameplay input'};
 const stateName=R.fullName(R.classOf(state));
 const result={schemaVersion:1,state:stateName,units:[],scope:'observed current client; no gameplay input'};
 if(stateName==='HomeBaseGameState')result.campaignMenu=readCampaignMenu();
 if(stateName==='MasterInventoryGameState'){
  const controller=R.readRef(state,'_masterInventoryController'),data=R.readRef(controller,'_transitionData');
  result.inventory={type:R.readValue(data,'<Type>k__BackingField','i32'),navigation:R.readValue(data,'<NavType>k__BackingField','i32'),...InventoryBridge.readiness()};
 }
 if(['CharacterDetailsState','ShipDetailsState'].includes(stateName))result.detail=InventoryBridge.detail(state);
 if(stateName!=='BattleGameState')return result;
 try{result.input=BattleInputBridge.snapshot();}catch(e){result.input={unavailable:true,error:String(e)};}
 const context=R.readRef(R.readRef(state,'_battleController'),'_battleContext');
 const seen=new Set();
 // Read this battle view's original handles, not every loaded Unity object.
 const view=S.invoke('BattleContext','get_BattleView',context);
 const handles=R.readRef(view,'_unitReticles');
 const items=R.readRef(handles,'_items'),count=R.readValue(handles,'_size','i32');
 if(count<0||count>256||(items.isNull()?count!==0:count>items.add(24).readU64().toNumber()))throw Error('Battle reticle handle list bounds invalid');
 for(let index=0;index<count;index++){
  const handle=items.add(32+index*8).readPointer();if(handle.isNull())continue;
  const reticle=R.readRef(handle,'Reticle');if(reticle.isNull())continue;
  const unit=S.invoke('BattleMessageReticle','get_TargetUnit',reticle);
  if(unit.isNull()||seen.has(unit.toString())||!R.readRef(unit,'_battleContext').equals(context))continue;
  if(!R.unbox(S.invoke('UnityEngine.GameObject','get_activeInHierarchy',S.gameObject(reticle)),'bool'))continue;
  seen.add(unit.toString());
  result.units.push({sessionKey:unit.toString(),baseId:S.string(R.readRef(R.readRef(R.readRef(unit,'_unitDO'),'<Definition>k__BackingField'),'BaseId')),attributes:readBattleAttributes(unit),status:readBattleStatus(unit)});
 }
 return result;
});
globalThis.inventoryHome=()=>RuntimeApi.runOnMainThreadOnce(()=>{
 const R=RuntimeApi,S=SceneApi,state=R.readRef(R.findBound('StateController'),'<CurrentState>k__BackingField');
 if(R.fullName(R.classOf(state))!=='MasterInventoryGameState')throw Error('inventory-home requires the inventory screen');
 const views=S.all('MasterInventoryView').filter(v=>R.unbox(S.invoke('UnityEngine.GameObject','get_activeInHierarchy',S.gameObject(v)),'bool'));
 if(views.length!==1)throw Error('Inventory view must resolve uniquely');
 S.invoke('MasterInventoryView','OnHomeButtonClick',views[0]);
 return {invoked:'MasterInventoryView.OnHomeButtonClick',effect:'original UI navigation',transitionConfirmed:false};
});
globalThis.openInventory=kind=>RuntimeApi.runOnMainThreadOnce(()=>{
 if(!['characters','ships'].includes(kind))throw Error('Unsupported inventory kind');
 const R=RuntimeApi,S=SceneApi,A=R.arg,controller=R.findBound('StateController');
 const state=R.readRef(controller,'<CurrentState>k__BackingField');
 if(R.fullName(R.classOf(state))!=='HomeBaseGameState')throw Error('Opening inventory requires HomeBaseGameState');
 const klass=R.findClass('MasterInventoryGameState+TransitionData'),data=R.api('object_new','pointer',['pointer'])(klass),handle=R.retain(data);
 try{
 S.invoke('MasterInventoryGameState+TransitionData','.ctor',data);
 for(const [name,value] of [['Type',kind==='characters'?1:2],['NavType',kind==='characters'?8:10]])S.invoke('MasterInventoryGameState+TransitionData','set_'+name,data,[A.value('i32',value)]);
 S.invoke('MasterInventoryGameState+TransitionData','set_IsPlayerRoster',data,[A.value('bool',true)]);
 S.invoke('MasterInventoryGameState+TransitionData','set_UseConditionalCachedTransitionData',data,[A.value('bool',false)]);
 // Original HomeBaseController uses asyncEntry=true: this runs the original
 // state preparation (including preset tabs) before creating the inventory UI.
 S.invoke('StateController','EnterState',controller,[A.ref(S.type('MasterInventoryGameState')),A.ref(data),A.value('bool',true)],['System.Type','IStateTransitionData','System.Boolean']);
 return {invoked:'StateController.EnterState',effect:'original inventory navigation',kind,transitionConfirmed:false};
 }finally{R.release(handle);}
});
rpc.exports={...rpc.exports,snapshot:()=>readRuntimeSnapshot(),inventoryhome:()=>inventoryHome(),openinventory:kind=>openInventory(kind),battleability:request=>RuntimeApi.runOnMainThreadOnce(()=>BattleInputBridge.choose(request))};
rpc.exports.battletarget=request=>RuntimeApi.runOnMainThreadOnce(()=>BattleInputBridge.selectTarget(request));

rpc.exports.battleunit=request=>RuntimeApi.runOnMainThreadOnce(()=>BattleInputBridge.selectUnit(request));

rpc.exports.battlecancel=request=>RuntimeApi.runOnMainThreadOnce(()=>BattleInputBridge.cancelSelection(request));

globalThis.readCampaignMenu=()=>{
 const R=RuntimeApi,S=SceneApi;
 return S.all('CampaignsPopupSelectListItem').filter(p=>R.unbox(S.invoke('UnityEngine.GameObject','get_activeInHierarchy',S.gameObject(p)),'bool')).map(p=>{
  const d=R.readRef(p,'_renderData');
  return {feature:R.readValue(d,'<Feature>k__BackingField','u8'),locked:!!R.readValue(d,'<IsLocked>k__BackingField','bool'),title:S.string(R.readRef(d,'<FeatureTitle>k__BackingField'))};
 });
};
rpc.exports.campaignnavigate=(operation,payload)=>RuntimeApi.runOnMainThreadOnce(()=>{
 const R=RuntimeApi,S=SceneApi,state=R.readRef(R.findBound('StateController'),'<CurrentState>k__BackingField');
 const stateName=R.fullName(R.classOf(state));
 const visible=type=>S.all(type).filter(p=>R.unbox(S.invoke('UnityEngine.GameObject','get_activeInHierarchy',S.gameObject(p)),'bool'));
 if(operation==='campaign-home'){
  if(stateName!=='CampaignGameState')throw Error('campaign-home requires campaign page');
  const views=visible('CampaignMissionsView');if(views.length!==1)throw Error('Campaign view must resolve uniquely');
  S.invoke('HudBaseView','OnHomeButtonClick',views[0]);
 }else{
  if(stateName!=='HomeBaseGameState')throw Error('Campaign menu navigation requires Home');
  if(operation==='campaign-menu'){
   if(readCampaignMenu().length)return {noOp:true,invoked:null};
   const props=visible('SelectableCantinaV2Prop').filter(p=>R.readValue(p,'_prop','i32')===9);
   if(props.length!==1)throw Error('Campaign prop must resolve uniquely');
   S.invoke('HomeBaseController','OnCampaignsClicked',R.readRef(state,'_homeBaseController'),[R.arg.value('i32',R.readValue(props[0],'_propState','i32'))]);
  }else if(operation==='campaign-enter'){
   if(!Number.isInteger(payload?.feature))throw Error('Campaign feature required');
   const rows=visible('CampaignsPopupSelectListItem').filter(p=>R.readValue(R.readRef(p,'_renderData'),'<Feature>k__BackingField','u8')===payload.feature);
   if(rows.length!==1||R.readValue(R.readRef(rows[0],'_renderData'),'<IsLocked>k__BackingField','bool'))throw Error('Choose a visible unlocked campaign row');
   S.invoke('CampaignsPopupSelectListItem','HandleEnterClicked',rows[0]);
  }else throw Error('Unsupported campaign operation');
 }
 return {invoked:operation,effect:'original menu navigation; no battle start or sweep'};
});

rpc.exports.inventorycommand=(operation,request)=>RuntimeApi.runOnMainThreadOnce(()=>InventoryBridge.execute(operation,request));
