import assert from 'node:assert/strict';
import {validatePredictionTrace} from './prediction_validation.mjs';
const valid={predictionFailed:false,errors:[],dropped:0,rows:[{sequence:1}],predictions:[{sequence:1,matched:true,randomAfterMatched:true,originalRandomUnchanged:true}]};
assert.equal(validatePredictionTrace(valid),true);
for(const bad of [null,{}, {...valid,rows:[]},{...valid,predictions:[]},{...valid,dropped:1},{...valid,predictionFailed:true},{...valid,errors:['failure']}])assert.equal(validatePredictionTrace(bad),false);
for(const change of [{sequence:2},{matched:false},{randomAfterMatched:false},{originalRandomUnchanged:false},{error:'native failure'},{cleanupError:'dispose failed'}])assert.equal(validatePredictionTrace({...valid,predictions:[{...valid.predictions[0],...change}]}),false);
console.log('PASS prediction evidence requires complete matched rows, original RNG preservation, matching private final RNG and clean disposal');
