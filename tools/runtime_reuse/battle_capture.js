'use strict';
// Observe one NATURAL server-response conversion, never invoke the response handler.
// Snapshot only Protos.BattleData; Player/InventoryDelta/auth response data are excluded.
globalThis.BattleCapture=(()=>{
 const R=RuntimeApi,S=SceneApi,depth=new Map(),hooks=[];
 let captured=false,errors=[];
 hooks.push(Interceptor.attach(R.module.base.add(0xF8F750),{
  onEnter(){this.t=Process.getCurrentThreadId();depth.set(this.t,(depth.get(this.t)||0)+1);},
  onLeave(){const n=(depth.get(this.t)||1)-1;if(n)depth.set(this.t,n);else depth.delete(this.t);}
 }));
 hooks.push(Interceptor.attach(R.module.base.add(0x1D0BFF0),{
  onEnter(args){
   if(captured||errors.length>=5||!depth.get(Process.getCurrentThreadId()))return;
   let clone=null;
   try{
    if(R.fullName(R.classOf(args[1]))!=='Protos.BattleData')throw Error('Unexpected factory input');
    const mission={};
    if(!args[2].isNull())for(const field of ['_campaignID','_mapId','_nodeId','_missionId'])mission[field]=S.string(R.readRef(args[2],field));
    if(!args[2].isNull())mission.difficulty=R.readValue(args[2],'_difficulty','u8');
    clone=cloneOriginalBattleProto(args[1]);
    const bytes=serializeOriginalBattleProto(clone.object);
    captured=true;
    send({kind:'original-battle-proto-captured',scope:'natural response factory input; BattleData only',mission,overrideRelicTier:args[3].toInt32()&255,bytes:bytes.byteLength},bytes);
   }catch(e){errors.push(String(e));send({kind:'original-battle-proto-capture-error',error:String(e)});}
   finally{if(clone)clone.release();}
  }
 }));
 return {status:()=>({captured,errors}),stop:()=>{for(const hook of hooks)hook.detach();}};
})();
