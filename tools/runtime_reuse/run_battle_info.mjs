import {acquireRuntimeSession} from './runtime_session_lock.mjs';
import fs from 'node:fs';
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
 script.message.connect(m=>{messages.push(m);if(messages.length>100)messages.shift();save();});
 await script.load();
 while(!detached&&!stop&&!fs.existsSync(stopPath)){
  await new Promise(resolve=>setTimeout(resolve,1000));
  if(!detached){last=await script.exports.status();save();
   const snapshot=await script.exports.snapshot();
   fs.writeFileSync(path.join(directory,'runtime-snapshot.json'),JSON.stringify({...snapshot,pid:Number(pid),observedAt:new Date().toISOString()}));}
 }
 if(!detached){last=await script.exports.stop();save();}
}finally{
 if(!detached)await session.detach();detached=true;save();releaseOwnership();
}
