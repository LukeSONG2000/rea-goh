// Use REA's installed Frida provider, detaching only after probe cleanup reports.
import fs from 'node:fs';
import path from 'node:path';
import {createRequire} from 'node:module';
import {execFileSync} from 'node:child_process';
const [pid,source,output,mode]=process.argv.slice(2);
if(!pid||!source||!output||!['visual','headless'].includes(mode))throw Error('Expected PID source output mode');
const npmRoot=execFileSync(process.env.ComSpec||'cmd.exe',['/d','/s','/c','npm root -g'],{encoding:'utf8'}).trim();
const require=createRequire(path.join(npmRoot,'rea-agents','package.json'));
const frida=require('frida');
const messages=[];
const session=await frida.attach(Number(pid));
let script,timer;
try{
 script=await session.createScript(fs.readFileSync(source,'utf8'));
 const completion=new Promise((resolve,reject)=>{
  timer=setTimeout(()=>reject(Error('Probe exceeded 180 seconds; inspect retained evidence and original client state')),180000);
  script.message.connect(message=>{
   messages.push(message);
   fs.writeFileSync(output,JSON.stringify({provider:'REA-installed Frida',pid:Number(pid),messages},null,2));
   if(message.type==='send'&&message.payload?.kind===(mode==='visual'?'visual-probe-complete':'simulation-probe-complete'))resolve();
   if(message.type==='error')reject(Error(message.description));
  });
 });
 await script.load();
 await completion;
 // Let the completion callback return before unloading its script.
 await new Promise(resolve=>setTimeout(resolve,150));
}finally{
 clearTimeout(timer);
 await session.detach();
 fs.writeFileSync(output,JSON.stringify({provider:'REA-installed Frida',pid:Number(pid),messages,detached:true},null,2));
}
