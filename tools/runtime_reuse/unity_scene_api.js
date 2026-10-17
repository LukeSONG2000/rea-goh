'use strict';
globalThis.SceneApi=(()=>{
 const R=RuntimeApi,A=R.arg;
 const methods=new Map();
 const invoke=(type,name,obj,args=[],types)=>{const key=type+'.'+name+':'+args.length+':'+JSON.stringify(types);if(!methods.has(key))methods.set(key,R.findMethod(R.findClass(type),name,args.length,types));return R.invoke(methods.get(key),obj,args);};
 const string=o=>o.isNull()?null:o.add(20).readUtf16String(o.add(16).readS32());
 const type=name=>R.api('type_get_object','pointer',['pointer'])(R.api('class_get_type','pointer',['pointer'])(R.findClass(name)));
 const array=o=>{if(o.isNull())return [];const n=Number(R.api('array_length','size_t',['pointer'])(o));if(n>100000)throw Error('Array too large');return Array.from({length:n},(_,i)=>o.add(32+i*8).readPointer());};
 const all=name=>array(invoke('UnityEngine.Resources','FindObjectsOfTypeAll',R.nil,[A.ref(type(name))],['System.Type']));
 const name=o=>string(invoke('UnityEngine.Object','get_name',o));
 const transform=o=>invoke('UnityEngine.Component','get_transform',o);
 const gameObject=o=>invoke('UnityEngine.Component','get_gameObject',o);
 const parent=t=>invoke('UnityEngine.Transform','get_parent',t);
 function path(t){const parts=[];for(let i=0;!t.isNull()&&i<60;i++,t=parent(t))parts.unshift(name(t));return parts.join('/');}
 const vector=(t,key)=>{const b=invoke('UnityEngine.Transform','get_'+key,t),p=R.api('object_unbox','pointer',['pointer'])(b);return [p.readFloat(),p.add(4).readFloat(),p.add(8).readFloat()];};
 return {invoke,string,type,array,all,name,transform,gameObject,parent,path,vector};
})();
