import assert from 'node:assert/strict';
import fs from 'node:fs';
import vm from 'node:vm';
const source=fs.readFileSync(new URL('./battle_info_ui.js',import.meta.url),'utf8');
const ctx=vm.createContext({});
vm.runInContext(source.slice(source.indexOf(' function timedPrefix('),source.indexOf(' function layoutTarget(')),ctx);
assert.equal(ctx.timedPrefix([{conditions:[]}]),'');
assert.equal(ctx.timedPrefix([{conditions:[{kind:'non-turn-condition',remaining:null}]}]),'');
assert.equal(ctx.timedPrefix([{conditions:[{kind:'event-count',remaining:2}]}]),'[FFFFFF](2)[-] ');
assert.equal(ctx.timedPrefix([{conditions:[{kind:'event-count',remaining:2},{kind:'event-count',remaining:4}]}]),'[FFFFFF](2/4)[-] ');
assert.equal(ctx.timedPrefix([{conditions:[{kind:'event-count',remaining:0}]}]),'[FFFFFF](0)[-] ');
const values={CurrentHP:23,MaxHP:100,CurrentPermanentShield:12,MaxPermanentShield:90,CurrentBonusShield:45,MaxBonusShield:60};
const colors=vm.createContext({R:{unbox:x=>x},A:{value:(t,x)=>x},
 call:(type,method)=>method==='GetHealthColor'?'00FF00':values[method.slice(4)],
 nativeColor:x=>x,barColor:x=>x});
vm.runInContext(source.slice(source.indexOf(' function numericHealth('),source.indexOf(' function targetNumbers(')),colors);
assert.equal(colors.numericHealth({}, {health:'FF0000',shield:'AACCFF',bonus:'CC00FF'},{}),
 '[FF0000]23[-]/[00FF00]100[-]\n[CC00FF]45/60[-]    [AACCFF]12/90[-]');
assert.equal(colors.numericHealth({}, {health:'FF0000',shield:'AACCFF',bonus:'CC00FF'},{},true),
 '[CC00FF]45/60[-]    [AACCFF]12/90[-]\n[FF0000]23[-]/[00FF00]100[-]');
console.log('PASS compact timed prefix: unknown omitted, finite zero and distinct counters preserved');
