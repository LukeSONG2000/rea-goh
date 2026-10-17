'use strict';
// Temporary process-local Winsock egress guard; no OS firewall or install changes.
globalThis.installOfflineSocketGuard=()=>{
 const ws=Process.getModuleByName('ws2_32.dll'),entries=[],counts={};
 const setError=new NativeFunction(ws.getExportByName('WSASetLastError'),'void',['int']);
 const pause=new NativeFunction(Process.getModuleByName('kernel32.dll').getExportByName('Sleep'),'void',['uint']);
 const signatures={send:['pointer','pointer','int','int'],sendto:['pointer','pointer','int','int','pointer','int'],
  WSASend:['pointer','pointer','uint','pointer','uint','pointer','pointer'],
  WSASendTo:['pointer','pointer','uint','pointer','uint','pointer','int','pointer','pointer'],
  connect:['pointer','pointer','int'],WSAConnect:['pointer','pointer','int','pointer','pointer','pointer','pointer']};
 try{
  for(const [name,args] of Object.entries(signatures)){
   const address=ws.getExportByName(name);counts[name]=0;
   const replacement=new NativeCallback(()=>{counts[name]++;pause(5);setError(10050);return -1;},'int',args);
   Interceptor.replace(address,replacement);entries.push({address,replacement});
  }
  Interceptor.flush();
 }catch(e){for(const e of entries)Interceptor.revert(e.address);throw e;}
 return {counts,restore(){
  const errors=[];
  for(const entry of entries){
   if(entry.restored)continue;
   try{Interceptor.revert(entry.address);entry.restored=true;}catch(error){errors.push(String(error));}
  }
  Interceptor.flush();
  if(errors.length)throw Error('Socket restoration failed: '+errors.join('; '));
 }};
};
