"""Report only explicitly emitted probe results, never infer success from exit code."""
import json
import sys
from pathlib import Path
data=json.loads(Path(sys.argv[1]).read_text(encoding='utf-8-sig'))
rows=[]
def visit(obj):
    if isinstance(obj,dict):
        if isinstance(obj.get('payload'),dict): rows.append(obj['payload'])
        for value in obj.values():visit(value)
    elif isinstance(obj,list):
        for value in obj:visit(value)
visit(data)
mode=sys.argv[2]
kind='simulation-probe-complete' if mode=='headless' else 'visual-probe-complete'
results=[r for r in rows if r.get('kind')==kind]
if not results:
    print('FAIL: no completed probe result; inspect evidence for initialization or cleanup errors.')
    raise SystemExit(1)
result=results[-1]
print(json.dumps(result,ensure_ascii=False,indent=2))
ok=result.get('verifiedVictory') if mode=='headless' else result.get('verifiedVisualBattle')
raise SystemExit(0 if ok else 1)
