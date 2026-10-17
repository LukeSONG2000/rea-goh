'use strict';
const R=RuntimeApi,A=R.arg;
R.runOnMainThreadOnce(()=>{
 const state=R.findBound('StateController'),current=R.readRef(state,'<CurrentState>k__BackingField');
 if(!current.isNull()){
  const name=R.fullName(R.classOf(current));
  if(name!=='CampaignGameState'){send({kind:'home-return-not-needed',state:name});return;}
  R.invoke(R.findMethod(R.classOf(state),'ExitState',3),state,[A.ref(current),A.value('bool',true),A.ref(R.nil)]);
 }
 const type=R.api('type_get_object','pointer',['pointer'])(R.api('class_get_type','pointer',['pointer'])(R.findClass('HomeBaseGameState')));
 R.invoke(R.findMethod(R.classOf(state),'EnterState',3,['System.Type','IStateTransitionData','System.Boolean']),state,[A.ref(type),A.ref(R.nil),A.value('bool',false)]);
 send({kind:'returned-home',state:R.fullName(R.classOf(R.readRef(state,'<CurrentState>k__BackingField')))});
}).catch(e=>send({kind:'home-return-error',error:String(e)}));
