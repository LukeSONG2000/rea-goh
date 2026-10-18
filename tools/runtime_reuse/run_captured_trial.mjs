import fs from 'node:fs';
import path from 'node:path';
import crypto from 'node:crypto';
import {createRequire} from 'node:module';
import {execFileSync} from 'node:child_process';
import {acquireRuntimeSession} from './runtime_session_lock.mjs';
import {processCommands} from './runtime_commands.mjs';
import {fileURLToPath} from 'node:url';
import {trialCleanupComplete} from './trial_lifecycle.mjs';
const [pidText,manifestPath,output]=process.argv.slice(2),pid=Number(pidText);
if(!Number.isSafeInteger(pid)||pid<=0||!manifestPath||!output)throw Error('Expected PID manifest.json evidence.json');
if(fs.existsSync(output))throw Error('Evidence already exists; choose a new output path');
const manifest=JSON.parse(fs.readFileSync(manifestPath,'utf8').replace(/^\uFEFF/,''));
if(manifest.schemaVersion!==1||!['visual','headless'].includes(manifest.mode)||!Number.isInteger(manifest.durationSeconds)||manifest.durationSeconds<15||manifest.durationSeconds>600)throw Error('Invalid trial manifest');
const source=fs.readFileSync(manifest.source),digest=crypto.createHash('sha256').update(source).digest('hex');
if(digest!==manifest.sourceSha256)throw Error('Prepared source integrity mismatch');
const expected='12cc254525a58aedea122a2967d0476da05b98ef0c219dc18cfc0b7207538db6';
if(manifest.originalAssemblySha256!==expected)throw Error('Unsupported original revision');
// Resolve actual process image; never trust a manifest to nominate the target binary.
const processImage=execFileSync('powershell.exe',['-NoProfile','-Command',`(Get-Process -Id ${pid} -ErrorAction Stop).Path`],{encoding:'utf8'}).trim();
if(processImage.toLowerCase()!=='e:\\games\\ea games\\swgoh\\swgoh.exe')throw Error('Unexpected target image');
const assembly=path.join(path.dirname(processImage),'GameAssembly.dll');
if(crypto.createHash('sha256').update(fs.readFileSync(assembly)).digest('hex')!==expected)throw Error('Original assembly changed');
const unlock=acquireRuntimeSession(pid);
const directory=path.resolve(path.dirname(fileURLToPath(import.meta.url)),'../../logs/high_reuse');
const statusPath=path.join(directory,`trial-status-${pid}.json`),snapshotPath=path.join(directory,`trial-snapshot-${pid}.json`);
const owner={pid,hostPid:process.pid};
const saveStatus=(extra={})=>{
 const temp=statusPath+'.tmp';fs.writeFileSync(temp,JSON.stringify({...owner,kind:'captured-trial',mode:manifest.mode,battleCli:manifest.battleCli===true,snapshotPath,detached:false,...extra}));fs.renameSync(temp,statusPath);
};
let session,script,timer,complete=null;
const messages=[];
const persist=(extra={})=>fs.writeFileSync(output,JSON.stringify({pid,manifest:manifestPath,messages,...extra},null,2));
try{
 const npmRoot=execFileSync(process.env.ComSpec||'cmd.exe',['/d','/s','/c','npm root -g'],{encoding:'utf8'}).trim();
 const require=createRequire(path.join(npmRoot,'rea-agents','package.json'));
 session=await require('frida').attach(pid);
 script=await session.createScript(source.toString('utf8'));
 const done=new Promise(resolve=>{
  script.message.connect(message=>{
   messages.push(message);persist();
   const kind=manifest.mode==='visual'?'visual-probe-complete':'simulation-probe-complete';
   if(message.type==='send'&&message.payload?.kind===kind){complete=message.payload;resolve();}
  });
 });
 await script.load();
 saveStatus();
 // If native execution hangs, keep the owner/guard alive instead of silently
 // unloading a script while its original battle still holds private objects.
 timer=setInterval(()=>{
  persist({needsAttention:true,reason:'No cleanup completion yet; runtime ownership retained'});
  process.stderr.write('Trial cleanup has not completed; owner retained. Inspect evidence and client before intervention.\n');
 },(manifest.durationSeconds+60)*1000);
 if(manifest.mode==='visual'&&manifest.battleCli===true){
  while(!complete){
   await processCommands(directory,owner,async(operation,payload)=>{
    if(complete||!['battle-ability','battle-target','battle-unit','battle-cancel','enemy-basic','enemy-resume','enemy-target'].includes(operation))throw Error('Operation unavailable in owned trial');
    return await script.exports.trialcommand(operation,payload);
   });
   if(complete)break;
   try{
    const snapshot=await script.exports.snapshot();
    if(manifest.enemyControl)snapshot.enemyInput=await script.exports.enemysnapshot();
    const temp=snapshotPath+'.tmp';fs.writeFileSync(temp,JSON.stringify({...snapshot,...owner,trial:true,observedAt:new Date().toISOString()}));fs.renameSync(temp,snapshotPath);
   }catch(e){saveStatus({snapshotError:String(e)});}
   await new Promise(resolve=>setTimeout(resolve,250));
  }
 }else await done;
 clearInterval(timer);
 const cleaned=trialCleanupComplete(complete,manifest.mode);
 if(!cleaned){
  persist({needsAttention:true,reason:'Cleanup incomplete; ownership and script retained'});
  saveStatus({needsAttention:true,battleCli:false});
  process.stderr.write('Cleanup incomplete; retaining runtime session. Do not start another host.\n');
  await new Promise(()=>{setInterval(()=>{},60000);});
 }
 await new Promise(resolve=>setTimeout(resolve,150));
 await session.detach();session=null;
 persist({detached:true});
 saveStatus({detached:true,battleCli:false});
 const valid=complete.error===null&&complete.serviceCalls===0&&
   (manifest.mode==='headless'?complete.terminal:
    complete.originalUiObserved&&(!manifest.auto||(complete.clicked&&complete.terminalObserved)));
 if(!valid)process.exitCode=1;
 process.stdout.write(JSON.stringify({mode:manifest.mode,valid,terminal:complete.terminal??complete.terminalObserved,cleanup:complete.cleanup,evidence:output})+'\n');
}finally{
 clearInterval(timer);
 if(session)await session.detach();
 saveStatus({detached:true,battleCli:false});
 unlock();
}

