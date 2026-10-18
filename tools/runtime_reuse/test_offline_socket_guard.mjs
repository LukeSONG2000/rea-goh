import fs from 'node:fs';
import vm from 'node:vm';
import assert from 'node:assert/strict';
const callbacks=new Map(),restored=[],delays=[],errors=[];let thread=10,failModule=false;
const ctx={Process:{getCurrentThreadId:()=>thread,getModuleByName:()=>({getExportByName:n=>n}),findModuleByAddress:()=>{if(failModule)throw Error('diagnostic unavailable');return {name:'transport.dll',base:100};}},NativeFunction:function(n){return n==='Sleep'?n=>delays.push(n):n=>errors.push(n);},NativeCallback:function(fn){return fn;},Interceptor:{replace:(n,fn)=>callbacks.set(n,fn),flush(){},revert:n=>restored.push(n)}};
vm.runInNewContext(fs.readFileSync(new URL('./offline_socket_guard.js',import.meta.url),'utf8'),ctx);
const guard=ctx.installOfflineSocketGuard();
for(const [name,fn] of callbacks){assert.equal(fn.call({returnAddress:{toString:()=>name,sub:()=>123}}),-1);}
assert.equal(guard.diagnostics().installedThreadCalls,6);
failModule=true;thread=999;
assert.equal(callbacks.get('send').call({returnAddress:{toString:()=> 'failed-diagnostic'}}),-1);
assert.equal(guard.diagnostics().diagnosticErrors,1);failModule=false;
for(let i=0;i<50;i++){thread=100+i;assert.equal(callbacks.get('send').call({}),-1);}
assert.equal(guard.diagnostics().callers.length,32);assert.equal(guard.diagnostics().untrackedCalls,24);
failModule=true;thread=999;assert.equal(callbacks.get('send').call({returnAddress:{toString:()=> 'new'}}),-1);
// Capacity-bound diagnostics do not affect the guard's rejection semantics.
assert.equal(errors.length,58);assert.ok(errors.every(e=>e===10050));assert.ok(delays.every(n=>n===5));
failModule=false;thread=10;
for(let i=0;i<20;i++)callbacks.get('send').call({returnAddress:{toString:()=> 'send'}});
assert.ok(delays.every(n=>n===5),'Never increase delay on installation/Unity thread');
// Reuse an existing background row even when the diagnostic map is full.
thread=100;for(let i=0;i<9;i++)callbacks.get('send').call({});
assert.equal(delays.at(-1),50);assert.equal(guard.diagnostics().backoffCalls,2);
guard.restore();guard.restore();assert.equal(restored.length,6);
console.log('PASS all six socket APIs reject, bounded caller metrics, main-thread accounting, idempotent restore');
