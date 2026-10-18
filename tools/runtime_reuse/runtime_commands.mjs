// Local, single-owner command mailbox. Whitelist only; no arbitrary native RPC.
import fs from 'node:fs';
import path from 'node:path';
import crypto from 'node:crypto';
const uuid=/^[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}$/;
export async function processCommands(directory,owner,execute,now=Date.now){
 const mailbox=path.join(directory,'commands');fs.mkdirSync(mailbox,{recursive:true});
 for(const name of fs.readdirSync(mailbox).filter(n=>n.endsWith('.request.json')).slice(0,16)){
  const id=name.slice(0,-13);if(!uuid.test(id))continue;
  const requestPath=path.join(mailbox,name),claimed=path.join(mailbox,id+'.running.json');
  try{fs.renameSync(requestPath,claimed);}catch{continue;}
  // A caller may resubmit an already completed ID. Keep its original receipt.
  if(fs.existsSync(path.join(mailbox,id+'.result.json'))){fs.unlinkSync(claimed);continue;}
  let response={id,ok:false,dispatched:false};
  try{
   if(fs.statSync(claimed).size>4096)throw Error('Command too large');
   const r=JSON.parse(fs.readFileSync(claimed,'utf8'));
   if(r.id!==id||r.pid!==owner.pid||r.hostPid!==owner.hostPid)throw Error('Command owner mismatch');
   if(!Number.isFinite(r.expiresAt)||r.expiresAt<now()||r.expiresAt>now()+30000)throw Error('Command expired or invalid deadline');
   if(!['store-open','store-list','store-back','calendar-select','calendar-open','calendar-list','calendar-claim','calendar-claim-status','calendar-close','prebattle-roster','prebattle-remove','prebattle-add','prebattle-supports','prebattle-support-select','prebattle-back','events-mission-select','events-missions','events-list','events-open','events-missions-back','events-back','objectives-go','objectives-claim-status','objectives-claim','objectives-achievements','objectives-daily','objectives-open','objectives-list','objectives-home','inbox-open','inbox-list','inbox-select','inbox-claim','inbox-claim-status','inbox-home','inventory-list','inventory-unit','inventory-skill','inventory-skill-close','inventory-skill-level','inventory-skill-quote','inventory-back','campaign-menu','campaign-mission','campaign-prebattle','campaign-enter','campaign-home','inventory-home','inventory-characters','inventory-ships','battle-ability','battle-target','battle-unit','battle-cancel','enemy-basic','enemy-resume','enemy-target','enemy-ability','enemy-unit','enemy-cancel'].includes(r.operation))throw Error('Operation unavailable; not authorized by this interface');
   if(r.operation==='calendar-select'&&(typeof r.payload?.calendarId!=='string'||!r.payload.calendarId.length||r.payload.calendarId.length>256))throw Error('Invalid calendar identity');
   if(r.operation==='calendar-claim'&&(typeof r.payload?.calendarId!=='string'||!r.payload.calendarId.length||r.payload.calendarId.length>256||!Number.isInteger(r.payload.rewardIndex)||r.payload.rewardIndex<0||typeof r.payload.lastClaimed!=='string'||!/^\d{1,20}$/.test(r.payload.lastClaimed)))throw Error('Invalid calendar reward identity');
   if(['inbox-select','inbox-claim'].includes(r.operation)&&(typeof r.payload?.messageId!=='string'||!r.payload.messageId.length||r.payload.messageId.length>256))throw Error('Invalid inbox message identity');
   if(r.operation==='campaign-prebattle'){
    const m=r.payload?.missionIdentity;
    if(!m||['CampaignID','MapID','NodeID','MissionID'].some(k=>typeof m[k]!=='string'||!m[k].length||m[k].length>256)||!Number.isInteger(m.Difficulty)||m.Difficulty<0||m.Difficulty>255)throw Error('Invalid full campaign mission identity');
   }
   if(['prebattle-remove','prebattle-add'].includes(r.operation)&&(!Number.isInteger(r.payload?.cellIndex)||r.payload.cellIndex<0||r.payload.cellIndex>63))throw Error('Invalid squad edit identity');
   if(['prebattle-support-select','prebattle-remove','prebattle-add'].includes(r.operation)){
    const p=r.payload;
    if(!p||['missionId',r.operation==='prebattle-support-select'?'playerId':'unitId'].some(k=>typeof p[k]!=='string'||!p[k].length||p[k].length>256)||(p.eventId!==null&&(typeof p.eventId!=='string'||!p.eventId.length||p.eventId.length>256)))throw Error('Invalid prebattle identity');
    if(p.eventId===null||p.missionIdentity!==undefined){
     const m=p.missionIdentity;
     if(!m||['CampaignID','MapID','NodeID','MissionID'].some(k=>typeof m[k]!=='string'||!m[k].length||m[k].length>256)||!Number.isInteger(m.Difficulty)||m.Difficulty<0||m.Difficulty>255||m.MissionID!==p.missionId)throw Error('Invalid full campaign mission identity');
    }
   }
   if(r.operation==='events-mission-select'&&(['eventId','missionId'].some(k=>typeof r.payload?.[k]!=='string'||!r.payload[k].length||r.payload[k].length>256)))throw Error('Invalid mission identity');
   if(r.operation==='events-open'&&(typeof r.payload?.eventId!=='string'||!r.payload.eventId.length||r.payload.eventId.length>256||typeof r.payload.instanceId!=='string'||r.payload.instanceId.length>256))throw Error('Invalid event identity');
   if(['objectives-claim','objectives-go'].includes(r.operation)&&(typeof r.payload?.questId!=='string'||!r.payload.questId.length||r.payload.questId.length>256||!((r.payload.tab===2&&r.payload.subTab===1)||(r.payload.tab===3&&r.payload.subTab===0))))throw Error('Invalid objectives claim request');
   if(['inventory-skill','inventory-skill-close','inventory-skill-level','inventory-skill-quote'].includes(r.operation)&&(typeof r.payload?.unitId!=='string'||!r.payload.unitId.length||r.payload.unitId.length>256))throw Error('Invalid skill unit request');
   if(['inventory-skill','inventory-skill-level','inventory-skill-quote'].includes(r.operation)&&(typeof r.payload?.skillId!=='string'||!r.payload.skillId.length||r.payload.skillId.length>256))throw Error('Invalid skill request');
   if(['inventory-skill-level','inventory-skill-quote'].includes(r.operation)&&(!Number.isInteger(r.payload?.tier)||r.payload.tier< -1||r.payload.tier>127))throw Error('Invalid skill tier');
   if(r.operation==='inventory-unit'&&(typeof r.payload?.unitId!=='string'||!r.payload.unitId.length||r.payload.unitId.length>256))throw Error('Invalid inventory unit request');
   if(r.operation.startsWith('enemy-')&&(!r.payload||typeof r.payload.token!=='string'||!r.payload.token.length||r.payload.token.length>3000))throw Error('Invalid enemy input request');
   if(r.operation==='enemy-ability'&&(![0,1].includes(r.payload.moveType)||!Number.isInteger(r.payload.index)||r.payload.index<0||r.payload.index>63||(r.payload.moveType===0&&r.payload.index!==0)))throw Error('Invalid enemy ability selector');
   if(r.operation==='campaign-enter'&&(!Number.isInteger(r.payload?.feature)||r.payload.feature<0||r.payload.feature>255))throw Error('Invalid campaign feature');
   if(['battle-target','battle-unit','enemy-target','enemy-unit'].includes(r.operation)&&(!r.payload||typeof r.payload.token!=='string'||r.payload.token.length>3000||typeof r.payload.targetId!=='string'||!r.payload.targetId.length||r.payload.targetId.length>256))throw Error('Invalid battle target request');
   if(r.operation==='battle-ability'&&(!r.payload||typeof r.payload.token!=='string'||r.payload.token.length>3000||!Number.isInteger(r.payload.index)||r.payload.index<0||r.payload.index>63))throw Error('Invalid battle ability request');
   if(r.operation==='battle-cancel'&&(!r.payload||typeof r.payload.token!=='string'||!r.payload.token.length||r.payload.token.length>3000))throw Error('Invalid cancellation request');
   // Claim happens before invocation. A crash leaves .running, never an auto retry.
   response.dispatched=true;response.result=await execute(r.operation,r.payload);
   response.ok=['calendar-claim','objectives-claim','inbox-claim'].includes(r.operation)?response.result?.claimDispatched===true:['store-list','calendar-list','calendar-claim-status','campaign-mission','inbox-claim-status','prebattle-roster','prebattle-supports','events-missions','events-list','objectives-claim-status','objectives-list','inbox-list','inventory-list','inventory-skill-quote'].includes(r.operation)?response.result?.readConfirmed===true:['enemy-basic','enemy-ability'].includes(r.operation)?(response.result?.started===true||response.result?.selectionStarted===true||response.result?.selectionEmpty===true):r.operation==='enemy-unit'?response.result?.started===true:r.operation==='enemy-cancel'?response.result?.selectionCancelled===true:r.operation==='enemy-resume'?response.result?.resumed===true:r.operation==='battle-cancel'?response.result?.selectionCancelled===true:r.operation==='battle-unit'?response.result?.selectionCompleted===true:['battle-target','enemy-target'].includes(r.operation)?response.result?.targetConfirmed===true:r.operation==='battle-ability'?response.result?.inputDispatched===true:response.result?.transitionConfirmed===true;
   if(!response.ok)response.error='Operation dispatched but not confirmed; inspect current state before retrying.';
  }catch(e){response.error=String(e);}
  const result=path.join(mailbox,id+'.result.json'),temp=result+'.tmp';
  fs.writeFileSync(temp,JSON.stringify(response));fs.renameSync(temp,result);
  fs.unlinkSync(claimed);
 }
}
export async function submitCommand(directory,owner,operation,payload){
 const mailbox=path.join(directory,'commands');fs.mkdirSync(mailbox,{recursive:true});
 const id=crypto.randomUUID(),request=path.join(mailbox,id+'.request.json');
 fs.writeFileSync(request+'.tmp',JSON.stringify({id,...owner,operation,payload,expiresAt:Date.now()+10000}));fs.renameSync(request+'.tmp',request);
 const result=path.join(mailbox,id+'.result.json');
 const end=Date.now()+25000;
 while(Date.now()<end){
  if(fs.existsSync(result))return JSON.parse(fs.readFileSync(result,'utf8'));
  await new Promise(resolve=>setTimeout(resolve,100));
 }
 // Remove only an unclaimed request; an in-flight command may have executed.
 try{fs.unlinkSync(request);}catch{}
 throw Error(`Command ${id} timed out; execution unknown. Inspect its result; do not blindly retry.`);
}
