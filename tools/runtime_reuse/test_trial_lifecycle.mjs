import assert from 'node:assert/strict';
import {trialCleanupComplete} from './trial_lifecycle.mjs';
const rejection={ticks:0,cleanup:{attempted:false,loopStopped:false,hooksDetached:true,guardRestored:true,gcReleased:true,errors:[]}};
assert.equal(trialCleanupComplete(rejection,'headless'),true);
for(const change of [{attempted:true},{attempted:undefined},{hooksDetached:false},{guardRestored:false},{gcReleased:false},{errors:['shutdown failed']}]){
 assert.equal(trialCleanupComplete({...rejection,cleanup:{...rejection.cleanup,...change}},'headless'),false);
}
assert.equal(trialCleanupComplete({...rejection,ticks:1},'headless'),false);
assert.equal(trialCleanupComplete({...rejection,cleanup:{...rejection.cleanup,attempted:true,loopStopped:true}},'headless'),true);
assert.equal(trialCleanupComplete(rejection,'visual'),false);
assert.equal(trialCleanupComplete({...rejection,cleanup:{...rejection.cleanup,home:true}},'visual'),true);
console.log('PASS: clean pre-initialization rejection exits; initialized/unproven/failed cleanup retains ownership.');
