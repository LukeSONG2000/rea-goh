import assert from 'node:assert/strict';
import fs from 'node:fs';
import path from 'node:path';
import crypto from 'node:crypto';
import {processCommands} from './runtime_commands.mjs';
const root=fs.mkdtempSync(path.resolve('logs/high_reuse/command-test-'));
const box=path.join(root,'commands');fs.mkdirSync(box);
let calls=0;const execute=async()=>{calls++;return {transitionConfirmed:true};};
const owner={pid:123,hostPid:456},now=()=>10000;
async function test(extra){
 const id=crypto.randomUUID();fs.writeFileSync(path.join(box,id+'.request.json'),JSON.stringify({id,...owner,operation:'inventory-home',expiresAt:11000,...extra}));
 await processCommands(root,owner,execute,now);return JSON.parse(fs.readFileSync(path.join(box,id+'.result.json')));
}
try{
 assert.equal((await test({})).ok,true);assert.equal(calls,1);
 for(const invalid of [{pid:999},{hostPid:999},{expiresAt:9999},{expiresAt:Infinity},{operation:'purchase'},{operation:'eval'},{operation:'inventory-unit'},{operation:'inventory-unit',payload:{unitId:''}},{operation:'inventory-unit',payload:{unitId:42}},{operation:'inventory-unit',payload:{unitId:'a'.repeat(257)}},{operation:'campaign-enter'},{operation:'campaign-enter',payload:{feature:'19'}},{operation:'campaign-enter',payload:{feature:-1}},{operation:'campaign-enter',payload:{feature:256}},{operation:'campaign-enter',payload:{feature:1.5}},{operation:'battle-ability'},{operation:'battle-ability',payload:{token:'x',index:-1}},{operation:'battle-ability',payload:{token:'x',index:1.5}}]){
  const result=await test(invalid);assert.equal(result.ok,false);assert.equal(result.dispatched,false);
 }
 assert.equal(calls,1,'Rejected commands never dispatch');
 for(const payload of [null,{}, {messageId:''},{messageId:42},{messageId:'a'.repeat(257)}]){
  assert.equal((await test({operation:'inbox-select',payload})).dispatched,false);
 }
 assert.equal(calls,1,'Malformed inbox requests must not invoke selection');
 for(const operation of ['prebattle-remove','prebattle-add'])for(const payload of [null,{}, {eventId:'e',missionId:'m',unitId:'u',cellIndex:-1},{eventId:'e',missionId:'m',unitId:'u',cellIndex:1.5}]){
  assert.equal((await test({operation,payload})).dispatched,false);
 }
 for(const payload of [null,{}, {eventId:'e',missionId:'m',playerId:''},{eventId:'e',missionId:'m',playerId:42}]){
  const result=await test({operation:'prebattle-support-select',payload});assert.equal(result.dispatched,false);
 }
 const campaignPayload={eventId:null,missionId:'Mi01',playerId:'p',missionIdentity:{CampaignID:'C01L',MapID:'M01',NodeID:'N01',MissionID:'Mi01',Difficulty:1}};
 assert.equal((await test({operation:'prebattle-support-select',payload:campaignPayload})).ok,true);
 for(const missionIdentity of [undefined,{}, {...campaignPayload.missionIdentity,MapID:''},{...campaignPayload.missionIdentity,Difficulty:256},{...campaignPayload.missionIdentity,MissionID:'Mi02'}]){
  assert.equal((await test({operation:'prebattle-support-select',payload:{...campaignPayload,missionIdentity}})).dispatched,false);
  for(const operation of ['prebattle-remove','prebattle-add'])assert.equal((await test({operation,payload:{...campaignPayload,unitId:'u',cellIndex:1,missionIdentity}})).dispatched,false);
 }
 for(const payload of [null,{}, {questId:'q',tab:1,subTab:0},{questId:'',tab:2,subTab:1},{questId:'q',tab:3,subTab:1}]){
  const result=await test({operation:'objectives-claim',payload});assert.equal(result.dispatched,false);
 }
 for(const payload of [{token:'x',moveType:2,index:0},{token:'x',moveType:1,index:-1},{token:'x',moveType:1,index:64},{token:'x',moveType:0,index:1},{token:'x',moveType:1,index:1.5}]){
  const result=await test({operation:'enemy-ability',payload});assert.equal(result.ok,false);assert.equal(result.dispatched,false);
 }
 for(const operation of ['enemy-basic','enemy-resume','enemy-cancel']){
  for(const payload of [null,{}, {token:42},{token:''}]){
   const result=await test({operation,payload});assert.equal(result.ok,false);assert.equal(result.dispatched,false);
  }
  const id=crypto.randomUUID();
  fs.writeFileSync(path.join(box,id+'.request.json'),JSON.stringify({id,...owner,operation,payload:{token:'enemy-token'},expiresAt:11000}));
  await processCommands(root,owner,async()=>operation==='enemy-basic'?{started:true}:operation==='enemy-cancel'?{selectionCancelled:true}:{resumed:true},now);
  assert.equal(JSON.parse(fs.readFileSync(path.join(box,id+'.result.json'))).ok,true);
 }
 for(const payload of [null,{}, {token:42},{token:''}]){
  const result=await test({operation:'battle-cancel',payload});assert.equal(result.ok,false);assert.equal(result.dispatched,false);
 }
 for(const payload of [null,{token:'x',targetId:''},{token:'x',targetId:42},{token:42,targetId:'unit'}]){
  const result=await test({operation:'battle-target',payload});assert.equal(result.ok,false);assert.equal(result.dispatched,false);
  const selection=await test({operation:'battle-unit',payload});assert.equal(selection.ok,false);assert.equal(selection.dispatched,false);
  const ally=await test({operation:'enemy-unit',payload});assert.equal(ally.ok,false);assert.equal(ally.dispatched,false);
  const enemy=await test({operation:'enemy-target',payload});assert.equal(enemy.ok,false);assert.equal(enemy.dispatched,false);
 }
 for(const operation of ['inventory-skill','inventory-skill-close','inventory-skill-level','inventory-skill-quote'])for(const payload of [null,{}, {unitId:''},{unitId:42}]){
  const result=await test({operation,payload});assert.equal(result.ok,false);assert.equal(result.dispatched,false);
 }
 for(const skillId of [undefined,'',42,'x'.repeat(257)]){
  const result=await test({operation:'inventory-skill',payload:{unitId:'unit',skillId}});assert.equal(result.dispatched,false);
 }
 for(const tier of [undefined,-2,128,1.5,'6']){
  const result=await test({operation:'inventory-skill-level',payload:{unitId:'unit',skillId:'skill',tier}});assert.equal(result.dispatched,false);
  const quote=await test({operation:'inventory-skill-quote',payload:{unitId:'unit',skillId:'skill',tier}});assert.equal(quote.dispatched,false);
 }
 for(const [operation,payload,result] of [
  ['objectives-open',undefined,{transitionConfirmed:true}],
  ['objectives-go',{questId:'q',tab:2,subTab:1},{transitionConfirmed:true}],
  ['events-back',undefined,{transitionConfirmed:true}],
  ['events-list',undefined,{readConfirmed:true,events:[]}],
  ['events-missions',undefined,{readConfirmed:true,missions:[]}],
  ['events-mission-select',{eventId:'event',missionId:'mission'},{transitionConfirmed:true}],
  ['prebattle-back',undefined,{transitionConfirmed:true}],
  ['prebattle-supports',undefined,{readConfirmed:true,supports:[]}],
  ['prebattle-roster',undefined,{readConfirmed:true,roster:[]}],
  ['prebattle-remove',{eventId:'e',missionId:'m',unitId:'u',cellIndex:1},{transitionConfirmed:true}],
  ['prebattle-add',{eventId:'e',missionId:'m',unitId:'u',cellIndex:1},{transitionConfirmed:true}],
  ['prebattle-remove',{...campaignPayload,unitId:'u',cellIndex:1},{transitionConfirmed:true}],
  ['prebattle-add',{...campaignPayload,unitId:'u',cellIndex:1},{transitionConfirmed:true}],
  ['prebattle-support-select',{eventId:'e',missionId:'m',playerId:'p'},{transitionConfirmed:true}],
  ['events-open',{eventId:'event',instanceId:'instance'},{transitionConfirmed:true}],
  ['events-missions-back',undefined,{transitionConfirmed:true}],
  ['objectives-claim-status',undefined,{readConfirmed:true,claims:[]}],
  ['objectives-claim',{questId:'q',tab:2,subTab:1},{claimDispatched:true,rewardConfirmed:false}],
  ['objectives-achievements',undefined,{transitionConfirmed:true}],
  ['objectives-daily',undefined,{transitionConfirmed:true}],
  ['objectives-home',undefined,{transitionConfirmed:true}],
  ['objectives-list',undefined,{readConfirmed:true,quests:[]}],
  ['inbox-open',undefined,{transitionConfirmed:true}],
  ['inbox-home',undefined,{transitionConfirmed:true}],
  ['inbox-list',undefined,{readConfirmed:true,messages:[]}],
  ['inventory-skill-quote',{unitId:'unit',skillId:'skill',tier:0},{readConfirmed:true}],
  ['inventory-skill-level',{unitId:'unit',skillId:'skill',tier:-1},{transitionConfirmed:true}],
  ['inventory-skill',{unitId:'unit',skillId:'skill'},{transitionConfirmed:true}],
  ['inventory-skill-close',{unitId:'unit'},{transitionConfirmed:true}],
  ['inventory-list',undefined,{readConfirmed:true,units:[]}],
  ['inventory-unit',{unitId:'native-unit-id'},{transitionConfirmed:true}],
  ['inventory-back',undefined,{transitionConfirmed:true}],
  ['enemy-ability',{token:'t',moveType:1,index:1},{selectionStarted:true,started:false}],
  ['enemy-basic',{token:'t'},{selectionEmpty:true,started:false}],
  ['enemy-unit',{token:'t',targetId:'ally'},{started:true,allyTarget:'ally'}]]){
  const id=crypto.randomUUID();fs.writeFileSync(path.join(box,id+'.request.json'),JSON.stringify({id,...owner,operation,payload,expiresAt:11000}));
  await processCommands(root,owner,async()=>result,now);
  assert.equal(JSON.parse(fs.readFileSync(path.join(box,id+'.result.json'))).ok,true);
 }
 await processCommands(root,owner,execute,now);assert.equal(calls,2,'Completed requests are not replayed');
 const completed=fs.readdirSync(box).find(f=>JSON.parse(fs.readFileSync(path.join(box,f))).ok);
 const completedId=completed.replace('.result.json','');
 fs.writeFileSync(path.join(box,completedId+'.request.json'),JSON.stringify({id:completedId,...owner,operation:'inventory-home',expiresAt:11000}));
 await processCommands(root,owner,execute,now);assert.equal(calls,2,'Duplicate completed IDs must not dispatch again');
 const pendingId=crypto.randomUUID();
 fs.writeFileSync(path.join(box,pendingId+'.request.json'),JSON.stringify({id:pendingId,...owner,operation:'inventory-home',expiresAt:11000}));
 await processCommands(root,owner,async()=>({transitionConfirmed:false}),now);
 const pending=JSON.parse(fs.readFileSync(path.join(box,pendingId+'.result.json')));
 assert.equal(pending.ok,false);assert.equal(pending.dispatched,true);
 const id=crypto.randomUUID();fs.writeFileSync(path.join(box,id+'.running.json'),'{}');
 await processCommands(root,owner,execute,now);assert.equal(calls,2,'Uncertain running commands are never retried');
 console.log('PASS: valid dispatch, wrong owner/expired/unknown operations rejected, no replay or retry.');
}finally{
 for(const file of fs.readdirSync(box))fs.unlinkSync(path.join(box,file));fs.rmdirSync(box);fs.rmdirSync(root);
}
