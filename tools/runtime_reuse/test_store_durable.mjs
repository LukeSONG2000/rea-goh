import assert from 'node:assert/strict';
import fs from 'node:fs';
import path from 'node:path';
import crypto from 'node:crypto';
import {processCommands} from './runtime_commands.mjs';
const payload={itemId:'premium-pack-silver',quantity:1,expectedCost:0};
async function command(root,hostPid,operation,body,execute){
 const id=crypto.randomUUID(),owner={pid:123,hostPid},box=path.join(root,'commands');fs.mkdirSync(box,{recursive:true});
 fs.writeFileSync(path.join(box,id+'.request.json'),JSON.stringify({id,...owner,operation,payload:body,expiresAt:11000}));
 await processCommands(root,owner,execute,()=>10000);
 return JSON.parse(fs.readFileSync(path.join(box,id+'.result.json')));
}
for(const failure of [false,true]){
 const root=fs.mkdtempSync(path.resolve('logs/high_reuse/store-durable-test-'));
 const claimPath=path.join(root,'store-free.pending.json');let calls=0;
 const execute=async()=>{
  calls++;
  assert.equal(JSON.parse(fs.readFileSync(claimPath)).quantity,1,'Durable record exists before native dispatch');
  if(failure)throw Error('uncertain native outcome');
  return {claimDispatched:true,settlementConfirmed:false};
 };
 for(const bad of [{...payload,itemId:null},{...payload,quantity:2},{...payload,expectedCost:1}]){
  assert.equal((await command(root,1,'store-claim-free',bad,execute)).dispatched,false);
  assert.equal(fs.existsSync(claimPath),false);
 }
 const first=await command(root,1,'store-claim-free',payload,execute);
 assert.equal(first.dispatched,true);assert.equal(first.ok,!failure);
 const original=fs.readFileSync(claimPath,'utf8');
 // New owner and new request ID must not replay even if the native call threw.
 const retry=await command(root,2,'store-claim-free',payload,execute);
 assert.equal(retry.dispatched,false);assert.match(retry.error,/requires reconciliation/);
 assert.equal(calls,1);assert.equal(fs.readFileSync(claimPath,'utf8'),original);
 const receipt=await command(root,2,'store-claim-status',{},async()=>({readConfirmed:true,receipt:null,settlementConfirmed:false}));
 assert.equal(receipt.ok,true);assert.equal(receipt.result.retryBlocked,true);
 assert.equal(receipt.result.originalCommandResult.id,first.id);
 assert.equal(receipt.result.settlementConfirmed,false);
}
// A truncated record from an interrupted write must fail closed too.
const interrupted=fs.mkdtempSync(path.resolve('logs/high_reuse/store-durable-interrupted-'));
fs.writeFileSync(path.join(interrupted,'store-free.pending.json'),'{');
assert.equal((await command(interrupted,3,'store-claim-free',payload,async()=>{throw Error('must not execute');})).dispatched,false);
console.log('PASS durable pre-dispatch claim, malformed input, success/exception restart refusal, original receipt, truncated record refusal');
