'use strict';
// Supply capturedBytes as a Uint8Array from a verified local BattleData file.
// No battle launch, account mutation, or response callback invocation.
(async()=>{
 const result={kind:'protobuf-probe-complete',scope:'captured natural BattleData file; original serializer roundtrip only',passed:false};
 let decoded=null;
 try{await RuntimeApi.runOnMainThreadOnce(()=>{
  const R=RuntimeApi;
  decoded=deserializeOriginalBattleProto(capturedBytes);
  const bytes=new Uint8Array(serializeOriginalBattleProto(decoded.object));
  if(bytes.length!==capturedBytes.length||bytes.some((v,i)=>v!==capturedBytes[i]))throw Error('Serialized bytes differ');
  const encounters=R.readRef(decoded.object,'_battleEncounter');
  result.encounters=encounters.isNull()?0:R.readValue(encounters,'_size','i32');
  result.encounterIndex=R.readValue(decoded.object,'EncounterIndex','i32');
  result.bytes=bytes.length;result.byteIdentical=true;result.passed=true;
 });}catch(e){result.error=String(e);}
 finally{if(decoded)decoded.release();RuntimeApi.releaseAll();send(result);}
})();