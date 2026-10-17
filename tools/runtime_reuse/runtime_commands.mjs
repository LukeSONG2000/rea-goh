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
   if(!['inventory-home','inventory-characters','inventory-ships'].includes(r.operation))throw Error('Operation unavailable; not authorized by this interface');
   // Claim happens before invocation. A crash leaves .running, never an auto retry.
   response.dispatched=true;response.result=await execute(r.operation);
   response.ok=response.result?.transitionConfirmed===true;
   if(!response.ok)response.error='Navigation dispatched but destination not confirmed; inspect current state before retrying.';
  }catch(e){response.error=String(e);}
  const result=path.join(mailbox,id+'.result.json'),temp=result+'.tmp';
  fs.writeFileSync(temp,JSON.stringify(response));fs.renameSync(temp,result);
  fs.unlinkSync(claimed);
 }
}
export async function submitCommand(directory,owner,operation){
 const mailbox=path.join(directory,'commands');fs.mkdirSync(mailbox,{recursive:true});
 const id=crypto.randomUUID(),request=path.join(mailbox,id+'.request.json');
 fs.writeFileSync(request+'.tmp',JSON.stringify({id,...owner,operation,expiresAt:Date.now()+10000}));fs.renameSync(request+'.tmp',request);
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
