# -*- coding: utf-8 -*-
"""Build catalog/character_names.json : prefab 名 -> defId + 角色名.

数据源(全部来自 swgoh-utils/gamedata @ main 的官方 gamedata 镜像):
  units.json / units_pve.json : 每行带 unitPrefab ("unit.char_jynerso_pre") + nameKey
  units_gas.json              : 无 unitPrefab, 但有 baseId + nameKey
  loc_en.json / loc_cn.json   : Loc_ENG_US.txt.json / Loc_CHS_CN.txt.json, nameKey -> 显示名
"""
import json, os, re, difflib

CAT = os.path.dirname(os.path.abspath(__file__))
GOH = os.path.dirname(CAT)


def load(f):
    d = json.load(open(os.path.join(CAT, f), encoding='utf-8'))
    return d['data'] if isinstance(d, dict) and 'data' in d else d


units_full = load('units.json') + load('units_pve.json')   # rows have unitPrefab
units_gas = load('units_gas.json')                          # rows lack unitPrefab
loc_en = json.load(open(os.path.join(CAT, 'loc_en.json'), encoding='utf-8'))
loc_cn = json.load(open(os.path.join(CAT, 'loc_cn.json'), encoding='utf-8'))

by_prefab = {}
for u in units_full:
    if isinstance(u, dict) and u.get('unitPrefab'):
        by_prefab.setdefault(u['unitPrefab'], u)

# baseId -> (nameKey, obtainable) from all sources (prefer units.json rows)
by_base = {}
for u in units_full + units_gas:
    if isinstance(u, dict) and u.get('baseId'):
        by_base.setdefault(u['baseId'], u)


def locn(d, key):
    if not key:
        return None
    return d.get(key) or d.get('[%s]' % key)


def slug(s):
    return re.sub(r'[^a-z0-9]', '', (s or '').lower())


# name-slug index from loc (unit name keys only)
name_slug = {}
for k, v in loc_en.items():
    kk = k.strip('[]')
    if kk.startswith('UNIT_') and kk.endswith('_NAME'):
        name_slug.setdefault(slug(v), kk[len('UNIT_'):-len('_NAME')])

raw_mods = json.loads(open(os.path.join(GOH, 'models.json'), 'rb').read().decode('utf-8'))
if isinstance(raw_mods, dict):
    raw_mods = raw_mods.get('data', [])
models = [m['id'] if isinstance(m, dict) else m for m in raw_mods]
model_names = {m['id']: (m.get('en'), m.get('zh')) for m in raw_mods if isinstance(m, dict)}


def core_variants(p):
    """yield (core, variant_tokens) candidates, longest core first"""
    toks = p[len('char_'):-len('_pre')].split('_')
    yield '_'.join(toks), []
    for i in range(len(toks) - 1, 0, -1):
        yield '_'.join(toks[:i]), toks[i:]
    for i in range(len(toks)):
        c = '_'.join(toks[:i] + toks[i+1:])
        if c:
            yield c, [toks[i]]


SRC = ('https://github.com/swgoh-utils/gamedata (官方 gamedata 镜像): '
       'units.json/unitPrefab + Loc_ENG_US.txt.json + Loc_CHS_CN.txt.json')

out, miss = {}, []
cnt = {'exact_prefab': 0, 'variant_prefab': 0, 'exact_baseId': 0,
       'tokenmove_baseId': 0, 'name_slug': 0}
for p in models:
    rec = None
    # T1/T2: unitPrefab exact or variant-stripped
    for c, v in core_variants(p):
        k = 'unit.char_%s_pre' % c
        if k in by_prefab:
            u = by_prefab[k]
            rec = {'defId': u['baseId'],
                   'match': 'exact' if not v else 'derived',
                   'detail': None if not v else 'core=%s variant=%s' % (c, '+'.join(v)),
                   'obtainable': bool(u.get('obtainable'))}
            cnt['exact_prefab' if not v else 'variant_prefab'] += 1
            break
    # T3: exact baseId (units_gas-only units, e.g. 4-LOM, Captain Rex)
    if rec is None:
        for c, v in core_variants(p):
            if not v and c.upper() in by_base:
                u = by_base[c.upper()]
                rec = {'defId': c.upper(), 'match': 'exact_baseId', 'detail': 'units_gas',
                       'obtainable': bool(u.get('obtainable'))}
                cnt['exact_baseId'] += 1
                break
    # T4: move trailing token to front (AHSOKATANO_GL -> GLAHSOKATANO)
    if rec is None:
        toks = p[len('char_'):-len('_pre')].split('_')
        for i in range(1, len(toks)):
            head, rest = toks[i:], toks[:i]
            cand = ''.join(head) + ''.join(rest)
            if cand.upper() in by_base:
                u = by_base[cand.upper()]
                rec = {'defId': cand.upper(), 'match': 'derived_baseId',
                       'detail': 'token move %s' % '+'.join(toks),
                       'obtainable': bool(u.get('obtainable'))}
                cnt['tokenmove_baseId'] += 1
                break
    # T5: match by english display-name slug
    if rec is None:
        n = p[len('char_'):-len('_pre')]
        hit = name_slug.get(slug(n.replace('_', '')))
        if hit and hit in by_base:
            u = by_base[hit]
            rec = {'defId': hit, 'match': 'derived_name',
                   'detail': 'loc name slug', 'obtainable': bool(u.get('obtainable'))}
            cnt['name_slug'] += 1
    if rec is None:
        miss.append(p)
        continue
    u = by_base.get(rec['defId'], {})
    nk = u.get('nameKey')
    ne, nz = locn(loc_en, nk), locn(loc_cn, nk)
    rec_out = {'defId': rec['defId'],
               'name_en': ne,
               'name_zh': nz,
               'match': rec['match'],
               'detail': rec['detail'],
               'obtainable': rec['obtainable'],
               'source': SRC}
    if ne is None or nz is None:
        # loc 快照缺这些 key —— 用 models.json 里人工维护的 en/zh 兜底并明确标注
        me, mz = model_names.get(p, (None, None))
        if me and not ne:
            rec_out['name_en'] = me
        if mz and not nz:
            rec_out['name_zh'] = mz
        if rec_out['name_en'] or rec_out['name_zh']:
            rec_out['source'] = SRC + '; 缺失名来自 models.json(人工维护, 未经验证)'
            rec_out['match'] = rec_out['match'] + '+name_from_models'
    out[p] = rec_out

# 未在 gamedata 中命中的 prefab: 仅给 models.json 里的名称(若有), defId 置空并标注
for p in miss:
    me, mz = model_names.get(p, (None, None))
    out[p] = {'defId': None,
              'name_en': me,
              'name_zh': mz,
              'match': 'unmatched_in_gamedata',
              'detail': '官方 units 表(镜像)中无 unitPrefab/baseId 对应项',
              'obtainable': None,
              'source': 'models.json (人工维护, 未经验证)' if (me or mz) else None}

json.dump(out, open(os.path.join(CAT, 'character_names.json'), 'w', encoding='utf-8'),
          ensure_ascii=False, indent=1)
json.dump(miss, open(os.path.join(CAT, '_unmatched.json'), 'w'), indent=1)
print('total', len(models), 'matched', len(out), 'unmatched', len(miss))
print(cnt)
named = sum(1 for v in out.values() if v['name_en'])
print('final named:', named)
named = sum(1 for v in out.values() if v['name_en'])
zh = sum(1 for v in out.values() if v['name_zh'])
print('name_en ok:', named, ' name_zh ok:', zh)
for s in ['char_jynerso_pre', 'char_4lom_pre', 'char_ahsokatano_gl_pre',
          'char_captainrex_pre', 'char_acklay_pre']:
    print(s, '->', json.dumps(out.get(s), ensure_ascii=False))
