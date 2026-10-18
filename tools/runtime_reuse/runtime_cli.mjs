import {submitCommand} from './runtime_commands.mjs';
import {acquireRuntimeSession} from './runtime_session_lock.mjs';
// JSON CLI. Explicit whitelisted navigation; observation never triggers actions.
import fs from 'node:fs';
import path from 'node:path';
import crypto from 'node:crypto';
import {fileURLToPath} from 'node:url';
import {createRequire} from 'node:module';
import {execFileSync} from 'node:child_process';
const args=process.argv.slice(2);
let battleRequest;
if(['calendar-select','calendar-claim','inbox-claim','inbox-select','prebattle-remove','prebattle-add','prebattle-support-select','events-mission-select','events-open','objectives-go','objectives-claim','inventory-unit','inventory-skill','inventory-skill-close','inventory-skill-level','inventory-skill-quote','campaign-mission','campaign-prebattle','campaign-enter','battle-ability','battle-target','battle-unit','battle-cancel','enemy-basic','enemy-resume','enemy-target','enemy-ability','enemy-unit','enemy-cancel'].includes(args[0])&&args.length===5&&args[3]==='--request'){
 if(fs.statSync(args[4]).size>4096)throw Error('Request file too large');
 battleRequest=JSON.parse(fs.readFileSync(args[4],'utf8').replace(/^\uFEFF/,''));args.splice(3,2);
}
if(args.length===1&&args[0]==='capabilities'){
 console.log(JSON.stringify({schemaVersion:1,operations:[
  ...['calendar-open','calendar-list','calendar-close','calendar-claim-status'].map(name=>({name,effect:'Original daily login calendar UI; close submits native queued claims',arguments:['--pid <PID>']})),
  {name:'calendar-select',effect:'Select an original calendar by current list ID; no claim',arguments:['--pid <PID>','--request <JSON file>'],request:{calendarId:'calendar-list.calendars[].id'}},
  {name:'calendar-claim',effect:'Original free daily claim once; no catch-up; verify claim-status after close',arguments:['--pid <PID>','--request <JSON file>'],request:{calendarId:'calendar-list.selected.id',rewardIndex:'calendar-list.selected.rewardIndex',lastClaimed:'calendar-list.selected.lastClaimed'}},
  {name:'objectives-go',effect:'original quest Go to event/challenge menu',arguments:['--pid <PID>','--request <JSON file>'],request:{questId:'original quest ID',tab:'current tab',subTab:'current subtab'}},
  {name:'events-mission-select',effect:'original mission selection into prebattle; no Battle button',arguments:['--pid <PID>','--request <JSON file>'],request:{eventId:'current event ID',missionId:'original mission ID'}},
  {name:'prebattle-roster',effect:'read original filtered roster and current empty slot',arguments:['--pid <PID>']},
  ...['prebattle-remove','prebattle-add'].map(name=>({name,effect:'original squad callback and slot readback; no Battle button',arguments:['--pid <PID>','--request <JSON file>'],request:{eventId:'current event, or null for ordinary campaign',missionId:'current mission',missionIdentity:'copy prebattle-roster.missionIdentity; required when eventId is null',cellIndex:'original slot index',unitId:'original unit ID'}})),
  {name:'prebattle-supports',effect:'read current original support choices',arguments:['--pid <PID>']},
  {name:'prebattle-support-select',effect:'original support selection into squad page; no Battle button',arguments:['--pid <PID>','--request <JSON file>'],request:{eventId:'current event ID, or null for ordinary campaign',missionId:'current mission ID',missionIdentity:'copy prebattle-supports.missionIdentity; required when eventId is null',playerId:'current support player ID'}},
  {name:'prebattle-back',effect:'original prebattle Back callback',arguments:['--pid <PID>']},
  {name:'events-missions',effect:'read original mission costs, progress and attempt/cooldown flags',arguments:['--pid <PID>']},
  {name:'events-list',effect:'read original active challenge list',arguments:['--pid <PID>']},
  {name:'events-open',effect:'original event selection to mission page; no battle input',arguments:['--pid <PID>','--request <JSON file>'],request:{eventId:'events-list.events[].eventId',instanceId:'events-list.events[].instanceId'}},
  {name:'events-missions-back',effect:'original mission page Back to challenge list',arguments:['--pid <PID>']},
  {name:'events-back',effect:'original events Back callback',arguments:['--pid <PID>']},
  {name:'objectives-claim-status',effect:'read correlated native reward callbacks; no claim retry',arguments:['--pid <PID>']},
  {name:'objectives-claim',effect:'original eligible quest reward callback; receipt is submission only',arguments:['--pid <PID>','--request <JSON file>'],request:{questId:'objectives-list.quests[].id',tab:'objectives-list.tab',subTab:'objectives-list.subTab'}},
  {name:'objectives-achievements',effect:'original tab navigation; no claim',arguments:['--pid <PID>']},
  {name:'objectives-daily',effect:'original tab navigation; no claim',arguments:['--pid <PID>']},
  {name:'objectives-open',effect:'original objectives open; no claims',arguments:['--pid <PID>']},
  {name:'objectives-list',effect:'original objectives list; no claims',arguments:['--pid <PID>']},
  {name:'objectives-home',effect:'original objectives home; no claims',arguments:['--pid <PID>']},
  {name:'inbox-open',effect:'original Home inbox navigation',arguments:['--pid <PID>']},
  {name:'inbox-list',effect:'read current filtered inbox; no claims or deletion',arguments:['--pid <PID>']},
  {name:'inbox-select',effect:'select one original inbox message; no claim or deletion',arguments:['--pid <PID>','--request <JSON file>'],request:{messageId:'inbox-list.messages[].id'}},
  {name:'inbox-claim',effect:'claim one selected native reward; never retry automatically',arguments:['--pid <PID>','--request <JSON file>'],request:{messageId:'selected original reward ID'}},
  {name:'inbox-claim-status',effect:'read original callback outcomes and currency/material balance deltas; unsupported rewards remain unconfirmed',arguments:['--pid <PID>']},
  {name:'inbox-home',effect:'original inbox back navigation',arguments:['--pid <PID>']},
  {name:'inventory-list',effect:'read current player-roster units in filtered collection; loaned marked',arguments:['--pid <PID>']},
  {name:'inventory-unit',effect:'original collection tile opens details; no upgrade',arguments:['--pid <PID>','--request <JSON file>'],request:{unitId:'inventory-list.units[].unitId'}},
  {name:'inventory-skill',effect:'open original skill details; no upgrade',arguments:['--pid <PID>','--request <JSON file>'],request:{unitId:'snapshot.detail.unitId',skillId:'snapshot.detail.skillDisplay[].skillId'}},
  {name:'inventory-skill-close',effect:'close original skill popup',arguments:['--pid <PID>','--request <JSON file>'],request:{unitId:'snapshot.detail.unitId'}},
  {name:'inventory-skill-level',effect:'preview an original skill level; no upgrade',arguments:['--pid <PID>','--request <JSON file>'],request:{unitId:'snapshot.detail.unitId',skillId:'snapshot.detail.skillPopup.skillId',tier:'snapshot.detail.skillPopup.levels[].tier'}},
  {name:'inventory-skill-quote',effect:'read selected original skill costs and conditions; no upgrade',arguments:['--pid <PID>','--request <JSON file>'],request:{unitId:'snapshot.detail.unitId',skillId:'snapshot.detail.skillPopup.skillId',tier:'snapshot.detail.skillPopup.selectedLevel.tier'}},
  {name:'inventory-back',effect:'original detail back callback',arguments:['--pid <PID>']},
  {name:'campaign-enter',effect:'open a visible unlocked campaign; no battle start',arguments:['--pid <PID>','--request <JSON file>'],request:{feature:'snapshot.campaignMenu[].feature'}},
  {name:'campaign-mission',effect:'read current map mission identity and selection availability',arguments:['--pid <PID>']},
  {name:'campaign-prebattle',effect:'original selected mission callback to support/squad selection; no combat start',arguments:['--pid <PID>','--request <JSON file>'],request:{missionIdentity:'copy campaign-mission.missionIdentity'}},
  {name:'enemy-basic',effect:'owned trial only: original basic attack',arguments:['--pid <PID>','--request <JSON file>'],request:{token:'snapshot.enemyInput.token'}},
  {name:'enemy-resume',effect:'owned trial only: resume original AI',arguments:['--pid <PID>','--request <JSON file>'],request:{token:'snapshot.enemyInput.token'}},
  {name:'enemy-target',effect:'owned trial only: original legal target selection',arguments:['--pid <PID>','--request <JSON file>'],request:{token:'snapshot.enemyInput.token',targetId:'snapshot.enemyInput.targets[].id'}},
  {name:'enemy-unit',effect:'confirm original ally candidate and execute pending enemy skill',arguments:['--pid <PID>','--request <JSON file>'],request:{token:'snapshot.enemyInput.token',targetId:'snapshot.enemyInput.selectionCandidates[].id'}},
  {name:'enemy-cancel',effect:'cancel enemy ally selection without spending the move',arguments:['--pid <PID>','--request <JSON file>'],request:{token:'snapshot.enemyInput.token'}},
  {name:'enemy-ability',effect:'owned trial only: original ability; may begin ally selection',arguments:['--pid <PID>','--request <JSON file>'],request:{token:'snapshot.enemyInput.token',moveType:'snapshot.enemyInput.abilities[].moveType',index:'snapshot.enemyInput.abilities[].index'}},
  {name:'snapshot',effect:'read',arguments:['--pid <SWGoH PID>']},
  {name:'watch',effect:'read',arguments:['--pid <SWGoH PID>']},
  {name:'battle-ability',effect:'original gameplay input',arguments:['--pid <SWGoH PID>','--request <JSON file>'],request:{token:'snapshot.input.token',index:'snapshot.input.abilities[].index'},result:'inputDispatched confirms callback only; observe subsequent snapshot for resolution'},
  {name:'battle-cancel',effect:'cancel original pending unit selection',arguments:['--pid <SWGoH PID>','--request <JSON file>'],request:{token:'snapshot.input.token'},result:'selectionCancelled confirms original cancellation'},
  {name:'battle-unit',effect:'complete original pending unit selection',arguments:['--pid <SWGoH PID>','--request <JSON file>'],request:{token:'snapshot.input.token',targetId:'snapshot.input.selectionCandidates[].id'},result:'selectionCompleted confirms original selection callback finished'},
  {name:'battle-target',effect:'original target selection',arguments:['--pid <SWGoH PID>','--request <JSON file>'],request:{token:'snapshot.input.token',targetId:'snapshot.input.targets[].id'},result:'targetConfirmed checks actual and manual target; repeated selection is a no-op'}
 ],navigation:['campaign-menu','campaign-home','inventory-home','inventory-characters','inventory-ships'],trial:{launcher:'start_captured_trial.ps1',aiPriorityOrder:'-AiPriorityOrder <JSON permutation file>; owned enemy, original linear chain only',scope:'captured snapshots; owned visual trial supports snapshot and battle inputs'},unavailable:['otherActions','uncapturedTrial','aiPrediction','enemyUltimateSkills']}));
}else if(args.length===3&&['calendar-select','calendar-open','calendar-list','calendar-claim','calendar-claim-status','calendar-close','prebattle-roster','prebattle-remove','prebattle-add','prebattle-supports','prebattle-support-select','prebattle-back','events-mission-select','events-missions','events-list','events-open','events-missions-back','events-back','objectives-go','objectives-claim-status','objectives-claim','objectives-achievements','objectives-daily','objectives-open','objectives-list','objectives-home','inbox-open','inbox-list','inbox-select','inbox-claim','inbox-claim-status','inbox-home','inventory-list','inventory-unit','inventory-skill','inventory-skill-close','inventory-skill-level','inventory-skill-quote','inventory-back','snapshot','watch','campaign-menu','campaign-mission','campaign-prebattle','campaign-enter','campaign-home','inventory-home','inventory-characters','inventory-ships','battle-ability','battle-target','battle-unit','battle-cancel','enemy-basic','enemy-resume','enemy-target','enemy-ability','enemy-unit','enemy-cancel'].includes(args[0])&&args[1]==='--pid'&&/^[1-9][0-9]*$/.test(args[2])){
 const pid=Number(args[2]);
 const processPath=execFileSync('powershell.exe',['-NoProfile','-Command',`(Get-Process -Id ${pid} -ErrorAction Stop).Path`],{encoding:'utf8'}).trim();
 if(processPath.toLowerCase()!=='e:\\games\\ea games\\swgoh\\swgoh.exe')throw Error('PID must be the supported original client');
 const hash=crypto.createHash('sha256').update(fs.readFileSync(path.join(path.dirname(processPath),'GameAssembly.dll'))).digest('hex');
 if(hash!=='12cc254525a58aedea122a2967d0476da05b98ef0c219dc18cfc0b7207538db6')throw Error('Unsupported original runtime revision');
 const npmRoot=execFileSync(process.env.ComSpec||'cmd.exe',['/d','/s','/c','npm root -g'],{encoding:'utf8'}).trim();
 const frida=createRequire(path.join(npmRoot,'rea-agents','package.json'))('frida');
 const directory=path.dirname(fileURLToPath(import.meta.url));
 const trialStatusPath=path.resolve(directory,`../../logs/high_reuse/trial-status-${pid}.json`);
 if(fs.existsSync(trialStatusPath)){
  const status=JSON.parse(fs.readFileSync(trialStatusPath,'utf8'));
  let alive=false;try{process.kill(status.hostPid,0);alive=true;}catch{}
  if(status.pid===pid&&!status.detached&&alive){
   const lock=JSON.parse(fs.readFileSync(path.resolve(directory,`../../logs/high_reuse/runtime-session-${pid}.lock`),'utf8'));
   if(lock.hostPid!==status.hostPid||lock.gamePid!==pid)throw Error('Trial ownership mismatch');
   if(!status.battleCli||status.needsAttention)throw Error('Trial input unavailable; inspect trial owner status');
   if(args[0]==='snapshot'){
    const cached=JSON.parse(fs.readFileSync(status.snapshotPath,'utf8'));
    if(cached.pid!==pid||cached.hostPid!==status.hostPid||Date.now()-Date.parse(cached.observedAt)>5000)throw Error('Trial snapshot stale; do not attach another host');
    console.log(JSON.stringify(cached,null,2));process.exit(0);
   }
   if(!['battle-ability','battle-target','battle-unit','battle-cancel','enemy-basic','enemy-resume','enemy-target','enemy-ability','enemy-unit','enemy-cancel'].includes(args[0]))throw Error('Only snapshot and battle inputs are available while trial owns runtime');
   const result=await submitCommand(path.dirname(trialStatusPath),{pid,hostPid:status.hostPid},args[0],battleRequest);
   console.log(JSON.stringify(result,null,2));process.exit(result.ok?0:1);
  }
 }
 if(args[0].startsWith('enemy-'))throw Error('Enemy control requires an active owned trial with -EnemyControl');
 const source=['runtime_api.js','unity_scene_api.js','battle_status_reader.js','battle_attributes_reader.js','battle_input.js','runtime_observer.js'].map(f=>fs.readFileSync(path.join(directory,f),'utf8')).join('\n');
 const statusPath=path.resolve(directory,'../../logs/high_reuse/battle-info-status.json');
 if(fs.existsSync(statusPath)){
  const status=JSON.parse(fs.readFileSync(statusPath,'utf8'));
  if(status.pid===pid&&!status.detached){
   process.kill(status.hostPid,0);
   if(args[0].startsWith('calendar-')||args[0].startsWith('prebattle-')||args[0].startsWith('events-')||args[0].startsWith('objectives-')||args[0].startsWith('inbox-')||args[0].startsWith('inventory-')||args[0].startsWith('campaign-')||['inventory-unit','inventory-skill','inventory-skill-close','inventory-skill-level','inventory-skill-quote','campaign-mission','campaign-prebattle','campaign-enter','battle-ability','battle-target','battle-unit','battle-cancel','enemy-basic','enemy-resume','enemy-target','enemy-ability','enemy-unit','enemy-cancel'].includes(args[0])){
    const result=await submitCommand(path.dirname(statusPath),{pid,hostPid:status.hostPid},args[0],battleRequest);
    console.log(JSON.stringify(result,null,2));process.exit(result.ok?0:1);
   }
   if(args[0]==='watch')throw Error('Shared display host already observes this client; use snapshot or overlay.');
   const cached=JSON.parse(fs.readFileSync(path.resolve(directory,'../../logs/high_reuse/runtime-snapshot.json'),'utf8'));
   if(cached.pid!==pid||Date.now()-Date.parse(cached.observedAt)>5000)throw Error('Owner snapshot stale; do not attach another host');
   console.log(JSON.stringify(cached,null,2));process.exit(0);
  }
 }
 if(['calendar-select','calendar-open','calendar-list','calendar-claim','calendar-claim-status','calendar-close','prebattle-roster','prebattle-remove','prebattle-add','prebattle-supports','prebattle-support-select','prebattle-back','events-mission-select','events-missions','events-list','events-open','events-missions-back','events-back','objectives-go','objectives-claim-status','objectives-claim','objectives-achievements','objectives-daily','objectives-open','objectives-list','objectives-home','inbox-open','inbox-list','inbox-select','inbox-claim','inbox-claim-status','inbox-home','inventory-list','inventory-unit','inventory-skill','inventory-skill-close','inventory-skill-level','inventory-skill-quote','inventory-back','campaign-menu','campaign-mission','campaign-prebattle','campaign-enter','campaign-home','inventory-characters','inventory-ships','battle-ability','battle-target','battle-unit','battle-cancel','enemy-basic','enemy-resume','enemy-target','enemy-ability','enemy-unit','enemy-cancel'].includes(args[0]))throw Error('Start the shared battle-info host for this operation.');
 const releaseOwnership=acquireRuntimeSession(pid);
 const session=await frida.attach(pid);
 try{
  const script=await session.createScript(source);
  script.message.connect(m=>{if(m.type==='error')console.error(JSON.stringify(m));});
  await script.load();
  if(args[0]==='inventory-home'){const invoked=await script.exports.inventoryhome();await new Promise(r=>setTimeout(r,1000));const after=await script.exports.snapshot();console.log(JSON.stringify({...invoked,transitionConfirmed:after.state==='HomeBaseGameState',afterState:after.state}));}
  else if(args[0]==='snapshot')console.log(JSON.stringify(await script.exports.snapshot(),null,2));
  else {
   const output=path.resolve(directory,'../../logs/high_reuse/runtime-snapshot.json');
   fs.mkdirSync(path.dirname(output),{recursive:true});
   const stopFile=path.resolve(directory,`../../logs/high_reuse/runtime-watch-${process.pid}.stop`);
   if(fs.existsSync(stopFile))fs.unlinkSync(stopFile);
   let stop=false;process.on('SIGINT',()=>{stop=true;});process.on('SIGTERM',()=>{stop=true;});
   session.detached.connect(()=>{stop=true;});
   while(!stop&&!fs.existsSync(stopFile)){
    try{
     const snapshot=await script.exports.snapshot();
     fs.writeFileSync(output,JSON.stringify({...snapshot,observedAt:new Date().toISOString(),pid}));
    }catch(e){fs.writeFileSync(output,JSON.stringify({state:'unavailable',observedAt:new Date().toISOString(),error:String(e),units:[]}));break;}
    await new Promise(resolve=>setTimeout(resolve,1000));
   }
  }
 }finally{await session.detach();releaseOwnership();}
}else{
 console.error('Usage: node tools/runtime_reuse/runtime_cli.mjs capabilities | snapshot|watch|inventory-home|inventory-characters|inventory-ships|inventory-list|inventory-back|campaign-menu|campaign-home --pid <SWGoH PID> | inventory-unit|campaign-enter|battle-ability|battle-target|battle-unit|battle-cancel --pid <PID> --request <JSON file>');process.exitCode=2;
}
