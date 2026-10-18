export function validatePredictionTrace(trace){
 if(!trace||trace.predictionFailed!==false||!Array.isArray(trace.errors)||trace.errors.length||trace.dropped!==0)return false;
 const rows=trace.rows,predictions=trace.predictions;
 if(!Array.isArray(rows)||!rows.length||!Array.isArray(predictions)||predictions.length!==rows.length)return false;
 return predictions.every((p,i)=>p.sequence===rows[i].sequence&&p.matched===true&&p.randomAfterMatched===true&&p.originalRandomUnchanged===true&&!p.error&&!p.cleanupError);
}
