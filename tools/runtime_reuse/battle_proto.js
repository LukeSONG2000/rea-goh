'use strict';
// Original protobuf cloning only. This does not clone BattleDataDO or enter battles.
globalThis.cloneOriginalBattleProto=source=>{
 const R=RuntimeApi,S=SceneApi;
 if(source.isNull()||R.fullName(R.classOf(source))!=='Protos.BattleData')throw Error('Expected original Protos.BattleData');
 const copy=S.invoke('ProtoBuf.Serializer+NonGeneric','DeepClone',R.nil,[R.arg.ref(source)]);
 if(copy.isNull()||copy.equals(source)||R.fullName(R.classOf(copy))!=='Protos.BattleData')throw Error('Original protobuf clone failed');
 const handle=R.retain(copy);
 return {object:copy,release:()=>R.release(handle)};
};
