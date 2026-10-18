// Observation failures must not terminate the display host or leave a fresh-
// looking copy of old actionable input. Never retry a gameplay command here.
export async function readSnapshotOrUnavailable(read,pid){
 try{return {...await read(),pid,observedAt:new Date().toISOString()};}
 catch(error){return {schemaVersion:1,state:'Unavailable',units:[],battleReady:false,
  input:{unavailable:true},error:String(error),pid,observedAt:new Date().toISOString(),
  scope:'Snapshot read failed; no gameplay input; next poll may recover'};}
}
