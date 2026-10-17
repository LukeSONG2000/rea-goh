"""Annotate recovered native bodies from existing metadata; no runtime or network access."""
import json,re
from pathlib import Path
root=Path(__file__).resolve().parents[2]
d=json.loads((root/'il2cpp/dump/il2cpp.json').read_text(encoding='utf-8'))['addressMap']
lookup={}
for group in ('methodDefinitions','constructedGenericMethods','typeInfoPointers','typeRefPointers','methodInfoPointers'):
 for row in d[group]:
  key=int(row['virtualAddress'],16)
  value=row.get('group','')+' '+row.get('dotNetType',row.get('dotNetSignature',row.get('name','')))
  lookup.setdefault(key,[]).append(value.strip())
out=root/'logs/high_reuse/native'
for p in out.glob('*.c'):
 text=p.read_text(encoding='utf-8')
 lines=[]
 for line in text.splitlines():
  if not line.strip():continue
  vals=[]
  for v in re.findall(r'(?:FUN_|0x|Ram0000000)(18[0-9a-fA-F]{7})',line):
   names=lookup.get(int(v,16),[])
   if names:vals.extend(names[:3])
  lines.append(line+(' // META: '+' | '.join(dict.fromkeys(vals)) if vals else ''))
 p.with_suffix('.annotated.txt').write_text('\n'.join(lines),encoding='utf-8')
print('Annotated',len(list(out.glob('*.c'))),'native bodies')
