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
 for(const invalid of [{pid:999},{hostPid:999},{expiresAt:9999},{expiresAt:Infinity},{operation:'purchase'},{operation:'eval'},{operation:'campaign-enter'},{operation:'campaign-enter',payload:{feature:'19'}},{operation:'campaign-enter',payload:{feature:-1}},{operation:'campaign-enter',payload:{feature:256}},{operation:'campaign-enter',payload:{feature:1.5}},{operation:'battle-ability'},{operation:'battle-ability',payload:{token:'x',index:-1}},{operation:'battle-ability',payload:{token:'x',index:1.5}}]){
  const result=await test(invalid);assert.equal(result.ok,false);assert.equal(result.dispatched,false);
 }
 assert.equal(calls,1,'Rejected commands never dispatch');
 for(const operation of ['enemy-basic','enemy-resume']){
  for(const payload of [null,{}, {token:42},{token:''}]){
   const result=await test({operation,payload});assert.equal(result.ok,false);assert.equal(result.dispatched,false);
  }
  const id=crypto.randomUUID();
  fs.writeFileSync(path.join(box,id+'.request.json'),JSON.stringify({id,...owner,operation,payload:{token:'enemy-token'},expiresAt:11000}));
  await processCommands(root,owner,async()=>operation==='enemy-basic'?{started:true}:{resumed:true},now);
  assert.equal(JSON.parse(fs.readFileSync(path.join(box,id+'.result.json'))).ok,true);
 }
 for(const payload of [null,{}, {token:42},{token:''}]){
  const result=await test({operation:'battle-cancel',payload});assert.equal(result.ok,false);assert.equal(result.dispatched,false);
 }
 for(const payload of [null,{token:'x',targetId:''},{token:'x',targetId:42},{token:42,targetId:'unit'}]){
  const result=await test({operation:'battle-target',payload});assert.equal(result.ok,false);assert.equal(result.dispatched,false);
  const selection=await test({operation:'battle-unit',payload});assert.equal(selection.ok,false);assert.equal(selection.dispatched,false);
 }
 await processCommands(root,owner,execute,now);assert.equal(calls,1,'Completed requests are not replayed');
 const completed=fs.readdirSync(box).find(f=>JSON.parse(fs.readFileSync(path.join(box,f))).ok);
 const completedId=completed.replace('.result.json','');
 fs.writeFileSync(path.join(box,completedId+'.request.json'),JSON.stringify({id:completedId,...owner,operation:'inventory-home',expiresAt:11000}));
 await processCommands(root,owner,execute,now);assert.equal(calls,1,'Duplicate completed IDs must not dispatch again');
 const pendingId=crypto.randomUUID();
 fs.writeFileSync(path.join(box,pendingId+'.request.json'),JSON.stringify({id:pendingId,...owner,operation:'inventory-home',expiresAt:11000}));
 await processCommands(root,owner,async()=>({transitionConfirmed:false}),now);
 const pending=JSON.parse(fs.readFileSync(path.join(box,pendingId+'.result.json')));
 assert.equal(pending.ok,false);assert.equal(pending.dispatched,true);
 const id=crypto.randomUUID();fs.writeFileSync(path.join(box,id+'.running.json'),'{}');
 await processCommands(root,owner,execute,now);assert.equal(calls,1,'Uncertain running commands are never retried');
 console.log('PASS: valid dispatch, wrong owner/expired/unknown operations rejected, no replay or retry.');
}finally{
 for(const file of fs.readdirSync(box))fs.unlinkSync(path.join(box,file));fs.rmdirSync(box);fs.rmdirSync(root);
}
