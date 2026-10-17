// JSON CLI, backed by REA-installed Frida. Observation only; no hidden actions.
import fs from 'node:fs';
import path from 'node:path';
import crypto from 'node:crypto';
import {fileURLToPath} from 'node:url';
import {createRequire} from 'node:module';
import {execFileSync} from 'node:child_process';
const args=process.argv.slice(2);
if(args.length===1&&args[0]==='capabilities'){
 console.log(JSON.stringify({schemaVersion:1,operations:[{name:'snapshot',effect:'read',arguments:['--pid <SWGoH PID>']}],unavailable:['actions','trial','aiPrediction','controlOpponent']}));
}else if(args.length===3&&args[0]==='snapshot'&&args[1]==='--pid'&&/^[1-9][0-9]*$/.test(args[2])){
 const pid=Number(args[2]);
 const processPath=execFileSync('powershell.exe',['-NoProfile','-Command',`(Get-Process -Id ${pid} -ErrorAction Stop).Path`],{encoding:'utf8'}).trim();
 if(processPath.toLowerCase()!=='e:\\games\\ea games\\swgoh\\swgoh.exe')throw Error('PID must be the supported original client');
 const hash=crypto.createHash('sha256').update(fs.readFileSync(path.join(path.dirname(processPath),'GameAssembly.dll'))).digest('hex');
 if(hash!=='12cc254525a58aedea122a2967d0476da05b98ef0c219dc18cfc0b7207538db6')throw Error('Unsupported original runtime revision');
 const npmRoot=execFileSync(process.env.ComSpec||'cmd.exe',['/d','/s','/c','npm root -g'],{encoding:'utf8'}).trim();
 const frida=createRequire(path.join(npmRoot,'rea-agents','package.json'))('frida');
 const directory=path.dirname(fileURLToPath(import.meta.url));
 const source=['runtime_api.js','unity_scene_api.js','battle_status_reader.js','battle_attributes_reader.js','runtime_observer.js'].map(f=>fs.readFileSync(path.join(directory,f),'utf8')).join('\n');
 const session=await frida.attach(pid);
 try{
  const script=await session.createScript(source);
  script.message.connect(m=>{if(m.type==='error')console.error(JSON.stringify(m));});
  await script.load();
  const result=await script.exports.snapshot();
  console.log(JSON.stringify(result,null,2));
 }finally{await session.detach();}
}else{
 console.error('Usage: node tools/runtime_reuse/runtime_cli.mjs capabilities | snapshot --pid <SWGoH PID>');process.exitCode=2;
}
