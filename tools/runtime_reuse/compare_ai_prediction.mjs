import fs from 'node:fs';
import path from 'node:path';
import {pathToFileURL} from 'node:url';
import {validatePredictionTrace} from './prediction_validation.mjs';

// Offline evidence comparison only. Never attaches to the game or evaluates AI.
export function compareAiPrediction(baseline,experiment){
 const payload=(evidence,kind)=>{
  const matches=(evidence?.messages||[]).filter(m=>m.type==='send'&&m.payload?.kind===kind);
  if(matches.length!==1)throw Error(`Expected one ${kind}, got ${matches.length}`);
  return matches[0].payload;
 };
 const original=payload(baseline,'original-ai-trace'),predicted=payload(experiment,'original-ai-trace');
 const completions=[baseline,experiment].map(e=>payload(e,'visual-probe-complete'));
 const clean=completions.every(c=>c.error===null&&c.serviceCalls===0&&c.terminalObserved===true&&c.cleanup?.home===true&&c.cleanup.guardRestored===true&&c.cleanup.gcReleased===true&&c.cleanup.definitionsRestored===true&&Array.isArray(c.cleanup.errors)&&!c.cleanup.errors.length);
 const baselineValid=Array.isArray(original.errors)&&!original.errors.length&&original.dropped===0&&Array.isArray(original.rows)&&original.rows.length>0;
 const rowsEqual=JSON.stringify(original.rows)===JSON.stringify(predicted.rows);
 const graphsEqual=JSON.stringify(original.trees)===JSON.stringify(predicted.trees);
 const predictionsValid=validatePredictionTrace(predicted);
 const resultDetailsEqual=predictionsValid&&predicted.predictions.every((p,i)=>JSON.stringify(p.result)===JSON.stringify(predicted.rows[i].result)&&JSON.stringify(p.privateRandomAfter)===JSON.stringify(predicted.rows[i].randomAfter));
 return {valid:clean&&baselineValid&&rowsEqual&&graphsEqual&&predictionsValid&&resultDetailsEqual,clean,baselineValid,rowsEqual,graphsEqual,predictionsValid,resultDetailsEqual,
  decisions:predicted.rows?.length??0,scope:'Exact recorded original rows, graph, prediction results and final seeds; does not prove full heap isolation or fixture identity'};
}

if(process.argv[1]&&import.meta.url===pathToFileURL(path.resolve(process.argv[1])).href){
 const [baseline,experiment]=process.argv.slice(2);
 if(!baseline||!experiment)throw Error('Expected baseline-evidence.json prediction-evidence.json');
 const read=p=>JSON.parse(fs.readFileSync(p,'utf8').replace(/^\uFEFF/,''));
 const result=compareAiPrediction(read(baseline),read(experiment));console.log(JSON.stringify(result,null,2));if(!result.valid)process.exitCode=1;
}
