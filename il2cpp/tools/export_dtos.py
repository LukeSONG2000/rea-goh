"""Export the game protocol DTO inventory from global-metadata.dat (v39).

The wire-protocol classes live in the `Protos` (and `Rpc`) namespaces.
For every type we record its fields (names only -- field *types* live in the
binary Il2CppType table inside GameAssembly.dll, not in metadata) and method
names. Output:

  il2cpp/dto_inventory.json   machine-readable
  il2cpp/PROTOCOL_DTOS.md     human-readable summary

Usage: py il2cpp/tools/export_dtos.py
"""
import json, os, sys, collections

sys.path.insert(0, os.path.dirname(__file__))
from metadata39 import Metadata39

META = r'E:/Games/EA Games/SWGoH/SWGoH_Data/il2cpp_data/Metadata/global-metadata.dat'
HERE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))  # il2cpp/
NAMESPACES = ('Protos', 'Rpc')

m = Metadata39(META, validate=False)

# map declaring typedef index -> nested types happen via nestedTypes section;
# a top-level scan with declaring_type==-1 plus its nested range covers all.
out = []
for td in m.typedefs:
    if td.namespace not in NAMESPACES:
        continue
    entry = {
        'namespace': td.namespace,
        'name': td.name,
        'flags': td.flags,
        'is_enum': td.is_enum,
        'is_valuetype': td.is_value_type,
        'fields': [f.name for f in m.fields_of(td)],
        'methods': [mm.name for mm in m.methods_of(td)],
        'nested': [n.name for n in m.nested_types_of(td)] if td.nested_type_count else [],
    }
    out.append(entry)

out.sort(key=lambda e: (e['namespace'], e['name']))
json_path = os.path.join(HERE, 'dto_inventory.json')
with open(json_path, 'w', encoding='utf-8') as f:
    json.dump(out, f, ensure_ascii=False, indent=1)

# ---- markdown summary -----------------------------------------------------
by_kind = collections.Counter()
for e in out:
    n = e['name']
    if e['is_enum']:
        by_kind['enum'] += 1
    elif 'Request' in n:
        by_kind['Request'] += 1
    elif 'Response' in n:
        by_kind['Response'] += 1
    elif 'Command' in n:
        by_kind['Command'] += 1
    elif 'Event' in n:
        by_kind['Event'] += 1
    else:
        by_kind['other'] += 1

lines = ['# SWGoH 客户端协议 DTO 清单', '',
         f'来源：global-metadata.dat v39（`metadata39.py` 解析，GameAssembly.dll 版本 0.40.7）。',
         f'范围：命名空间 `{"`、`".join(NAMESPACES)}`，共 **{len(out)}** 个类型。', '',
         '字段仅记录**名称**（字段类型在 GameAssembly.dll 的 Il2CppType 表里，需 dump.cs 阶段才有）。', '',
         '| 分类 | 数量 |', '|---|---|']
for k, v in by_kind.most_common():
    lines.append(f'| {k} | {v} |')

lines += ['', '## 类型明细', '',
          '| 类型 | 字段数 | 字段 |', '|---|---|---|']
for e in out:
    kind = 'enum ' if e['is_enum'] else ''
    flds = ', '.join(e['fields'][:24])
    if len(e['fields']) > 24:
        flds += f', … (+{len(e["fields"]) - 24})'
    if not flds:
        flds = '—'
    lines.append(f'| `{e["namespace"]}.{kind}{e["name"]}` | {len(e["fields"])} | {flds} |')

md_path = os.path.join(HERE, 'PROTOCOL_DTOS.md')
with open(md_path, 'w', encoding='utf-8') as f:
    f.write('\n'.join(lines) + '\n')

print(f'types exported : {len(out)}')
for k, v in by_kind.most_common():
    print(f'  {k:10s}: {v}')
print('json ->', json_path)
print('md   ->', md_path)
