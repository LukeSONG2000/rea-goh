export function trialCleanupComplete(result,mode){
 const c=result?.cleanup;
 if(!c?.guardRestored||!c.gcReleased||!Array.isArray(c.errors)||c.errors.length)return false;
 if(mode==='visual')return c.home===true;
 if(mode!=='headless'||c.hooksDetached!==true)return false;
 // A builder rejection never started a simulation. Require both explicit
 // initialization-cleanup absence and zero ticks, rather than assuming it.
 return c.loopStopped===true||(c.attempted===false&&result.ticks===0);
}
