import {processCommands} from './runtime_commands.mjs';
import {readSnapshotOrUnavailable} from './runtime_poll.mjs';
import {acquireRuntimeSession} from './runtime_session_lock.mjs';
import fs from 'node:fs';
import crypto from 'node:crypto';
import path from 'node:path';
import {createRequire} from 'node:module';
import {execFileSync} from 'node:child_process';
const [pid,source,directory]=process.argv.slice(2);
const root=execFileSync(process.env.ComSpec||'cmd.exe',['/d','/s','/c','npm root -g'],{encoding:'utf8'}).trim();
const frida=createRequire(path.join(root,'rea-agents','package.json'))('frida');
const statusPath=path.join(directory,'battle-info-status.json'),stopPath=path.join(directory,'battle-info.stop');
const messages=[];let detached=false,stop=false,script,last=null;
process.on('SIGINT',()=>{stop=true;});process.on('SIGTERM',()=>{stop=true;});
const releaseOwnership=acquireRuntimeSession(Number(pid));
const session=await frida.attach(Number(pid));
session.detached.connect(()=>{detached=true;});
const save=()=>fs.writeFileSync(statusPath,JSON.stringify({pid:Number(pid),hostPid:process.pid,detached,stop,status:last,messages},null,2));
try{
 script=await session.createScript(fs.readFileSync(source,'utf8'));
 script.message.connect((m,data)=>{
  if(m.type==='send'&&m.payload?.kind==='original-battle-proto-captured'){
   try{
    if(!data||data.length!==m.payload.bytes||data.length>16*1024*1024)throw Error('Invalid battle capture size');
    const capturePath=path.join(directory,`battle-proto-${pid}-${process.pid}.pb`);
    fs.writeFileSync(capturePath,data,{flag:'wx'});
    const metadata={...m.payload,path:capturePath,sha256:crypto.createHash('sha256').update(data).digest('hex')};
    fs.writeFileSync(capturePath+'.json',JSON.stringify(metadata,null,2),{flag:'wx'});
    m={type:'send',payload:metadata};
   }catch(e){m={type:'send',payload:{kind:'battle-proto-save-error',error:String(e)}};}
  }
  messages.push(m);if(messages.length>100)messages.shift();save();
 });
 await script.load();
 while(!detached&&!stop&&!fs.existsSync(stopPath)){
  await new Promise(resolve=>setTimeout(resolve,1000));
  if(!detached){
   await processCommands(directory,{pid:Number(pid),hostPid:process.pid},async (operation,payload)=>{
    if(['prebattle-roster','prebattle-remove','prebattle-add','prebattle-supports','prebattle-support-select','prebattle-back','events-mission-select','events-missions','events-list','events-open','events-missions-back'].includes(operation)){
     const result=['prebattle-roster','prebattle-remove','prebattle-add'].includes(operation)?await script.exports.prebattleedit(operation,payload):operation.startsWith('prebattle-support')?await script.exports.prebattlesupport(operation,payload):operation==='prebattle-back'?await script.exports.prebattleback():await script.exports.eventscommand(operation,payload);
     if(['prebattle-roster','prebattle-supports','events-list','events-missions'].includes(operation))return result;
     let after,confirmed=false;
     for(let i=0;i<30;i++){
      await new Promise(r=>setTimeout(r,250));after=await script.exports.snapshot();
      confirmed=operation==='prebattle-back'?after.state===result.expectedState:['prebattle-remove','prebattle-add','events-mission-select','prebattle-support-select'].includes(operation)?after.state===result.expectedState&&(after.prebattle?.ready||(operation==='events-mission-select'&&after.prebattle?.supportSelectionRequired))&&(result.expectedCellIndex===undefined||after.prebattle.squadCells?.some(c=>c.cellIndex===result.expectedCellIndex&&(c.unit?.unitId??null)===result.expectedCellUnitId))&&after.prebattle.eventId===result.eventId&&after.prebattle.missionId===result.missionId&&(!result.expectedMissionIdentity||['CampaignID','MapID','NodeID','MissionID','Difficulty'].every(k=>after.prebattle.missionIdentity?.[k]===result.expectedMissionIdentity[k]))&&(operation!=='prebattle-support-select'||after.prebattle.selectedSupport?.playerId===result.expectedSupportPlayerId):after.state===result.expectedState&&(result.listExpected?after.events?.listActive:after.events?.missionsActive&&after.events?.eventId===result.eventId);
      if(confirmed)break;
     }
     return {...result,transitionConfirmed:!!confirmed,afterState:after.state,events:after.events,prebattle:after.prebattle};
    }
    if(operation==='events-back'||operation.startsWith('objectives-')){
     const result=await script.exports.objectivescommand(operation,payload);
     if(['objectives-claim-status','objectives-list','objectives-claim'].includes(operation))return result;
     let after,confirmed=false;
     for(let i=0;i<30;i++){
      await new Promise(r=>setTimeout(r,250));after=await script.exports.snapshot();
      confirmed=operation==='events-back'?(after.state!==result.fromState&&after.state!=='Transitioning'):after.state===result.expectedState&&(['objectives-home','objectives-go','events-back'].includes(operation)||after.objectives?.ready===true)&&(result.tab===undefined||(after.objectives?.tab===result.tab&&after.objectives?.subTab===result.subTab));
      if(confirmed)break;
     }
     return {...result,transitionConfirmed:confirmed,afterState:after.state};
    }
    if(operation.startsWith('store-')){
     const result=await script.exports.storecommand(operation,payload);
     if(operation==='store-list')return result;
     let after=null,lastError=null;
     for(let i=0;i<30;i++){
      await new Promise(r=>setTimeout(r,250));
      try{after=operation==='store-back'?await script.exports.snapshot():await script.exports.storecommand('store-list');if(operation==='store-back'?after.state===result.expectedState:after.readConfirmed&&(operation!=='store-tab'||after.storeTab===result.expectedTab))break;}catch(e){lastError=String(e);}
     }
     return {...result,transitionConfirmed:operation==='store-back'?after?.state===result.expectedState:after?.readConfirmed===true&&(operation!=='store-tab'||after.storeTab===result.expectedTab),catalog:after,error:after?null:lastError};
    }
    if(operation.startsWith('calendar-')){
     const result=await script.exports.calendarcommand(operation,payload);
     if(!['calendar-open','calendar-close','calendar-select'].includes(operation))return result;
     let confirmed=false;
     for(let i=0;i<30;i++){
      await new Promise(r=>setTimeout(r,250));
      const visible=await script.exports.calendarvisibility();
      if(operation==='calendar-select'){
       const after=await script.exports.calendarcommand('calendar-list',{});
       confirmed=visible.showing&&visible.active&&after.selected?.id===result.expectedCalendarId;
      }else confirmed=operation==='calendar-open'?(visible.showing&&visible.active):(!visible.showing&&!visible.active);
      if(confirmed)break;
     }
     return {...result,transitionConfirmed:confirmed};
    }
    if(operation.startsWith('inbox-')){
     const result=await script.exports.inboxcommand(operation,payload);
     if(['inbox-list','inbox-claim','inbox-claim-status'].includes(operation))return result;
     let after,confirmed=false;
     for(let i=0;i<30;i++){
      await new Promise(r=>setTimeout(r,250));after=await script.exports.snapshot();
      confirmed=after.state===result.expectedState&&(operation==='inbox-home'||after.inbox?.ready===true)&&(operation!=='inbox-select'||after.inbox?.selectedMessageId===result.expectedMessageId);
      if(confirmed)break;
     }
     return {...result,transitionConfirmed:confirmed,afterState:after.state};
    }
    if(['inventory-list','inventory-unit','inventory-skill','inventory-skill-close','inventory-skill-level','inventory-skill-quote','inventory-back'].includes(operation)){
     const result=await script.exports.inventorycommand(operation,payload);
     if(['inventory-list','inventory-skill-quote'].includes(operation))return result;
     let after,confirmed=false;
     for(let i=0;i<20;i++){
      await new Promise(r=>setTimeout(r,250));after=await script.exports.snapshot();
      confirmed=['inventory-skill','inventory-skill-close','inventory-skill-level','inventory-skill-quote'].includes(operation)?after.state===result.expectedState&&after.detail?.unitId===result.unitId&&!after.detail?.skillPopupError&&(result.closing?after.detail?.skillPopup===null:after.detail?.skillPopup?.skillId===result.skillId||(after.detail?.skillPopup?.kind==='single'&&after.detail?.skillPopup?.name===result.name)):after.state===result.expectedState&&(operation==='inventory-unit'?after.detail?.unitId===result.unitId:after.inventory?.ready===true);
      if(operation==='inventory-skill-level')confirmed=confirmed&&after.detail?.skillPopup?.selectedLevel?.tier===result.selectedTier;
      if(confirmed)break;
     }
     return {...result,transitionConfirmed:confirmed,afterState:after.state,detail:after.detail};
    }
    if(operation==='battle-ability')return await script.exports.battleability(payload);
    if(operation==='battle-cancel')return await script.exports.battlecancel(payload);
    if(operation==='battle-unit')return await script.exports.battleunit(payload);
    if(operation==='battle-target')return await script.exports.battletarget(payload);
    if(['campaign-sweep-rewards','campaign-sweep-rewards-close','campaign-sweep-confirm','campaign-sweep-receipt','campaign-sweep-count','campaign-sweep-preview','campaign-sweep-status','campaign-sweep-close','campaign-mission','campaign-prebattle'].includes(operation)){
     const result=await script.exports.campaignmission(operation,payload);
     if(['campaign-sweep-rewards','campaign-sweep-confirm','campaign-sweep-receipt','campaign-mission','campaign-sweep-status'].includes(operation))return result;
     if(operation==='campaign-sweep-rewards-close'){
      let after=null;for(let i=0;i<30;i++){await new Promise(r=>setTimeout(r,250));after=await script.exports.campaignmission('campaign-sweep-rewards',{});if(!after.visible)break;}
      return {...result,transitionConfirmed:after?.visible===false};
     }
     if(['campaign-sweep-count','campaign-sweep-preview','campaign-sweep-close'].includes(operation)){
      let preview=null,confirmed=false;
      for(let i=0;i<30;i++){
       await new Promise(r=>setTimeout(r,250));preview=await script.exports.campaignmission('campaign-sweep-status',{});
       confirmed=operation==='campaign-sweep-close'?!preview.visible:preview.visible&&(operation!=='campaign-sweep-count'||preview.count===result.expectedCount)&&['CampaignID','MapID','NodeID','MissionID','Difficulty'].every(k=>preview.missionIdentity?.[k]===result.expectedMissionIdentity[k]);
       if(confirmed)break;
      }
      return {...result,transitionConfirmed:confirmed,preview};
     }
     let after,confirmed=false;
     for(let i=0;i<30;i++){
      await new Promise(r=>setTimeout(r,250));after=await script.exports.snapshot();
      confirmed=after.state===result.expectedState&&(after.prebattle?.ready||after.prebattle?.supportSelectionRequired)&&['CampaignID','MapID','NodeID','MissionID','Difficulty'].every(k=>after.prebattle.missionIdentity?.[k]===result.expectedMissionIdentity[k]);
      if(confirmed)break;
     }
     return {...result,transitionConfirmed:!!confirmed,afterState:after.state,prebattle:after.prebattle};
    }
    if(operation.startsWith('campaign-')){
     const result=await script.exports.campaignnavigate(operation,payload);let after,confirmed=false;
     for(let i=0;i<10;i++){
      await new Promise(r=>setTimeout(r,300));after=await script.exports.snapshot();
      confirmed=operation==='campaign-menu'?after.state==='HomeBaseGameState'&&after.campaignMenu?.length>0:after.state===(operation==='campaign-home'?'HomeBaseGameState':'CampaignGameState');
      if(confirmed)break;
     }
     return {...result,transitionConfirmed:!!confirmed,afterState:after.state,campaignMenu:after.campaignMenu};
    }
    const home=operation==='inventory-home';
    const result=home?await script.exports.inventoryhome():await script.exports.openinventory(operation==='inventory-characters'?'characters':'ships');
    let after,confirmed=false;
    for(let i=0;i<30;i++){
     await new Promise(r=>setTimeout(r,300));after=await script.exports.snapshot();
     confirmed=home?after.state==='HomeBaseGameState':after.state==='MasterInventoryGameState'&&after.inventory?.type===(operation==='inventory-characters'?1:2)&&after.inventory?.ready===true;
     if(confirmed)break;
    }
    return {...result,transitionConfirmed:confirmed,afterState:after.state,inventory:after.inventory};
   });
   last=await script.exports.status();save();
   const snapshot=await readSnapshotOrUnavailable(()=>script.exports.snapshot(),Number(pid));
   fs.writeFileSync(path.join(directory,'runtime-snapshot.json'),JSON.stringify({...snapshot,hostPid:process.pid}));}
 }
 if(!detached){last=await script.exports.stop();save();}
}finally{
 if(!detached&&script){
  try{const claims=await script.exports.inboxclaimdispose();fs.writeFileSync(path.join(directory,`inbox-claims-${process.pid}.json`),JSON.stringify(claims,null,2));}
  catch(error){messages.push({kind:'inbox-claim-dispose-error',error:String(error)});}
  try{const claims=await script.exports.questclaimdispose();fs.writeFileSync(path.join(directory,`quest-claims-${process.pid}.json`),JSON.stringify(claims,null,2));}
  catch(error){messages.push({kind:'quest-claim-dispose-error',error:String(error)});}
 }
 if(!detached)await session.detach();detached=true;save();releaseOwnership();
}
