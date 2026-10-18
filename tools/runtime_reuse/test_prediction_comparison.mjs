import assert from 'node:assert/strict';
import {compareAiPrediction} from './compare_ai_prediction.mjs';
const row={sequence:1,result:{abilityId:'original'},randomAfter:{BattleRandom:'9007199254740993'}};
const complete={kind:'visual-probe-complete',error:null,serviceCalls:0,terminalObserved:true,cleanup:{home:true,guardRestored:true,gcReleased:true,definitionsRestored:true,errors:[]}};
const trace={kind:'original-ai-trace',errors:[],dropped:0,rows:[row],trees:[{root:0}],predictionFailed:false,predictions:[{sequence:1,matched:true,originalRandomUnchanged:true,randomAfterMatched:true,result:row.result,privateRandomAfter:row.randomAfter}]};
const evidence=()=>({messages:[{type:'send',payload:structuredClone(trace)},{type:'send',payload:structuredClone(complete)}]});
assert.equal(compareAiPrediction(evidence(),evidence()).valid,true);
for(const mutate of [e=>e.messages[0].payload.rows[0].sequence++,e=>e.messages[0].payload.predictions[0].result={abilityId:'wrong'},e=>e.messages[0].payload.predictions[0].privateRandomAfter={},e=>e.messages[1].payload.serviceCalls++,e=>e.messages[1].payload.cleanup.definitionsRestored=false,e=>e.messages[0].payload.errors.push('failure')]){
 const experiment=evidence();mutate(experiment);assert.equal(compareAiPrediction(evidence(),experiment).valid,false);
}
const duplicate=evidence();duplicate.messages.push(duplicate.messages[0]);assert.throws(()=>compareAiPrediction(evidence(),duplicate),/Expected one/);
console.log('PASS comparison rejects altered rows, prediction content/seed mismatches, service activity, failed cleanup and duplicate evidence');
