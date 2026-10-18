import {processCommands} from './runtime_commands.mjs';
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
    if(['prebattle-back','events-mission-select','events-missions','events-list','events-open','events-missions-back'].includes(operation)){
     const result=operation==='prebattle-back'?await script.exports.prebattleback():await script.exports.eventscommand(operation,payload);
     if(['events-list','events-missions'].includes(operation))return result;
     let after,confirmed=false;
     for(let i=0;i<30;i++){
      await new Promise(r=>setTimeout(r,250));after=await script.exports.snapshot();
      confirmed=operation==='prebattle-back'?after.state===result.expectedState:operation==='events-mission-select'?after.state===result.expectedState&&after.prebattle?.ready&&after.prebattle.eventId===result.eventId&&after.prebattle.missionId===result.missionId:after.state===result.expectedState&&(result.listExpected?after.events?.listActive:after.events?.missionsActive&&after.events?.eventId===result.eventId);
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
    if(operation.startsWith('inbox-')){
     const result=await script.exports.inboxcommand(operation);
     if(operation==='inbox-list')return result;
     let after,confirmed=false;
     for(let i=0;i<30;i++){
      await new Promise(r=>setTimeout(r,250));after=await script.exports.snapshot();
      confirmed=after.state===result.expectedState&&(operation==='inbox-home'||after.inbox?.ready===true);
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
   const snapshot=await script.exports.snapshot();
   fs.writeFileSync(path.join(directory,'runtime-snapshot.json'),JSON.stringify({...snapshot,pid:Number(pid),observedAt:new Date().toISOString()}));}
 }
 if(!detached){last=await script.exports.stop();save();}
}finally{
 if(!detached&&script){
  try{const claims=await script.exports.questclaimdispose();fs.writeFileSync(path.join(directory,`quest-claims-${process.pid}.json`),JSON.stringify(claims,null,2));}
  catch(error){messages.push({kind:'quest-claim-dispose-error',error:String(error)});}
 }
 if(!detached)await session.detach();detached=true;save();releaseOwnership();
}
