'use strict';
// Reuse the original UI bridge, but only for this probe's exact owned battle.
rpc.exports.trialcommand=(operation,request)=>RuntimeApi.runOnMainThreadOnce(()=>{
 const R=RuntimeApi,probe=globalThis.visualProbe;
 if(!probe?.entered||probe.entered.isNull())throw Error('Trial battle not entered');
 const state=R.readRef(probe.fixture.state,'<CurrentState>k__BackingField');
 if(!state.equals(probe.entered))throw Error('Current state is not the owned trial');
 const context=R.readRef(R.readRef(state,'_battleController'),'_battleContext');
 if(!context.equals(probe.fixture.context))throw Error('Trial context ownership mismatch');
 const actions={'battle-ability':BattleInputBridge.choose,'battle-target':BattleInputBridge.selectTarget,
  'battle-unit':BattleInputBridge.selectUnit,'battle-cancel':BattleInputBridge.cancelSelection};
 if(!Object.hasOwn(actions,operation))throw Error('Only battle inputs are available in a trial');
 return actions[operation](request);
});
