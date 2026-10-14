# -*- coding: utf-8 -*-
"""classify_models.py — 给 models.json 的 747 个模型目录建立 英雄/非英雄 分类 + 阵营标签。

数据源（全部本地，不访问网络）:
  catalog/units.json           2,107 行 = 301 个可玩单位 × 7 星级（id 形如 VADER:SEVEN_STAR，含 unitPrefab）
  catalog/units_pve.json       6,500 行 PVE/敌方单位（id 形如 PVE_XXX:MODE_ELITE，含 unitPrefab，
                               少量带 '.' 的子 prefab 如 char_aat_pre.phase01）
  catalog/units_gas.json       {version, data}: 2,877 行 = 411 个 baseId × 7 星级，无 unitPrefab 的完整单位总表
                               （units.json 的 baseId 集合是它的真子集；gas 独有 110 个 = 较新/未进 prefab 表的单位）
  catalog/character_names.json 747 条 模型id -> defId/英文名/中文名（build_names.py 的产物，match 字段标注置信度）
  models.json                  747 个模型 {id, en, zh}

输出:
  catalog/model_classification.json   {_meta, models: {model_id: {...}}}
  catalog/MODEL_CLASSES.md            人类可读报告

类别体系（与任务约定一致）:
  hero          在 units.json 中通过 unitPrefab 精确对应，或通过官方 defId 关联到可玩单位
                （units.json 的 prefab 变体 / units_gas.json 独有单位）—— hero_match 字段区分判定方式
  pve_variant   仅出现在 units_pve.json（记录该 PVE 单位的标签），或英雄的 PVE/剧情用变体
  environment / cinematic / ship_assets / other
                按 env_*/cin_*/ship_* 前缀归类 —— models.json 全部是 char_* 前缀，实际为 0（见报告说明）
  unclassified  三张单位表都找不到（附 guess 直觉标注）

判定优先级（第一个命中即停，路径记录在 match 字段）:
  1. unitPrefab 精确命中 units.json                        -> hero (prefab_exact)
  2. unitPrefab 精确命中 units_pve.json（'.' 子prefab已归一）-> pve_variant (prefab_pve)
  3. defId 命中 units.json：id 带 PVE 标记 -> pve_variant (defId+hero_link)；否则 -> hero (defId_variant)
  4. defId 命中 units_pve.json                             -> pve_variant (defId_pve)
  5. defId 命中 units_gas.json 独有单位（obtainable=True）  -> hero (defId_gas)
  6. 核心 token 递减/反向前缀模糊匹配（变体词白名单校验）    -> hero/pve_variant (fuzzy_*)
  7. 社区缩写别名兜底                                       -> hero (alias)
  8. 都找不到                                               -> unclassified

召唤(summon)结论（先查证后标注，详见 MD 报告）:
  - units.json 无任何 summoned_unit 标记，summonStatTable 全空；hermityoda/BB8 是普通可玩单位。
  - 召唤物只在 units_pve.json：categoryId 含 `summoned_unit`（52 行）或 id 含 *SUMMON*。
  - `SOE_summoner` 是"召唤者"标记（可玩侧仅 ARCTROOPER501ST、GEONOSIANBROODALPHA）。
  - 因此不设 summon 类别，命中时输出 is_summon / is_summoner 字段。

用法:  PYTHONIOENCODING=utf-8 py tools/classify_models.py
"""
import json
import os
import re
import sys
from collections import Counter, OrderedDict

GOH = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
CAT = os.path.join(GOH, 'catalog')
OUT_JSON = os.path.join(CAT, 'model_classification.json')
OUT_MD = os.path.join(CAT, 'MODEL_CLASSES.md')

PVE_ID_MARKERS = ('_pve', '_raid', '_nokill', '_enemy', '_boss', '_noshield')
CIN_TOKENS = ('cin', 'cinematic', 'cinematics')
ALIGNMENTS = ('alignment_light', 'alignment_dark', 'alignment_neutral', 'alignment_seedy')
COMBAT_TYPE = {1: 'CHARACTER', 2: 'SHIP'}

# 变体词白名单：模糊匹配时允许剥掉的 token / 允许残留的后缀（防止乱挂到无关单位上）
VARWORD = (r'(?:speederbike|holo|cin(?:ematic|ematics)?|pve|raid|nokill|noshield|enemy|boss|'
           r'remastered|orig|gl|s3|ot|tfa|tros|dsv|variant|var0?\d|tier0?\d|dlc\d+|echoes|'
           r'jediknight|jedimaster|gray|grey|scrapper|commander|event|pre)')

# 社区绰号/缩写别名（内嵌策展表：参考 LukeSONG2000/goh-skill lib/names.py 的检索思路；
# 其绰号数据 names.json 被上游 .gitignore 无法程序化获取。运行时校验目标 baseId 存在才启用）
NICKNAME_ALIASES = {
    # —— 玩家社区常用缩写（策展；上游 names.json 被 .gitignore 无法拉取）——
    'jml': 'GRANDMASTERLUKE',          # Jedi Master Luke
    'jmluke': 'GRANDMASTERLUKE',
    'jmk': 'JEDIMASTERKENOBI',         # Jedi Master Kenobi
    'jkl': 'JEDIKNIGHTLUKE',           # Jedi Knight Luke
    'gas': 'GENERALSKYWALKER',         # General Anakin Skywalker
    'glrey': 'GLREY',                  # GL Rey
    'glreyskywalker': 'GLREY',
    'see': 'PALPATINEEMPERORETERNAL',  # Sith Eternal Emperor
    'lv': 'LORDVADER',                 # Lord Vader
    'cls': 'COMMANDERLUKESKYWALKER',   # Commander Luke Skywalker
    # —— 模型 id 与 defId 命名不一致的官方修正（目标均验证存在于 units/units_gas）——
    'merrin': 'MERRIN',                # char_nightsistermerrin_pre -> MERRIN
    'nightsistermerrin': 'MERRIN',
    'leiagl': 'GLLEIA',                # char_leia_gl_* -> GLLEIA
    'leia_gl': 'GLLEIA',
    'darthrey': 'DARKREY',             # char_darthrey_pre -> DARKREY
    'r5d4': 'R5D4',                    # char_astromech_r5d4_pre -> R5D4
    'commanderappo': 'APPO',           # char_commanderappo_pre -> APPO
    'chirpa': 'CHIEFCHIRPA',           # char_chirpa_pre -> CHIEFCHIRPA
    'luke_trooper': 'STORMTROOPERLUKE',  # char_luke_trooper_pre（伪装暴风兵的卢克）
    'batcher': 'BATCHERS3',            # char_batcher_pre -> BATCHERS3
    'obiwan_jedimaster': 'JEDIMASTERKENOBI',  # char_obiwan_jedimaster* -> JMK
    'hondo_gl': 'GLHONDO',             # char_hondo_gl_* -> GLHONDO
}


def load(name):
    d = json.load(open(os.path.join(CAT, name), encoding='utf-8'))
    if isinstance(d, dict) and isinstance(d.get('data'), list):
        d = d['data']
    return d


def strip_unit_prefix(p):
    return p[5:] if p.startswith('unit.') else p


def base_of_prefab(p):
    """'char_aat_pre.phase01' -> 'char_aat_pre'（PVE 子 prefab 按 '.' 截断）"""
    return p.split('.', 1)[0]


def model_core(model_id):
    """char_han_speederbike_pre -> han_speederbike ; char_damage -> damage"""
    c = model_id
    if c.startswith('char_'):
        c = c[len('char_'):]
    if c.endswith('_pre'):
        c = c[:-4]
    return c


def alnum(s):
    return re.sub(r'[^a-z0-9]', '', (s or '').lower())


def pve_markers(model_id):
    """按 token 边界匹配（避免 _boss 误伤 bossnass 这类名字）"""
    toks = set(model_core(model_id).split('_'))
    return [m[1:] for m in PVE_ID_MARKERS if m[1:] in toks]


def variant_flags(model_id):
    """从模型 id 提取描述性变体标记（不影响主分类）"""
    c = model_core(model_id)
    flags = []
    for t in c.split('_'):
        if t == 'speederbike':
            flags.append('speederbike_variant')
        elif t in CIN_TOKENS:
            flags.append('cinematic_variant')
        elif t == 'holo':
            flags.append('holo_variant')
        elif t == 'pve':
            flags.append('pve_used_variant')
        elif t == 'raid':
            flags.append('raid_variant')
        elif t == 'nokill':
            flags.append('nokill_variant')
        elif t == 'boss':
            flags.append('boss_variant')
        elif t == 'noshield':
            flags.append('noshield_variant')
        elif t == 'remastered':
            flags.append('remaster_variant')
        elif t == 'orig':
            flags.append('original_model_variant')
        elif t == 'variant' or re.fullmatch(r'var0?\d', t):
            flags.append('skin_variant')
    return sorted(set(flags))


PROP_KEYWORDS = {
    'crate': 'prop（补给箱）', 'damage': 'prop/VFX', 'starmap': 'prop（星图）',
    'shieldgenerator': 'prop（护盾发生器）', 'endorbag': 'prop（袋子）',
    'cash': 'prop（现金/测试道具）',
    'tauntaun': 'creature（汤汤坐骑）', 'bantha': 'creature（班萨兽）',
    'kraytdragon': 'creature（克雷特龙）',
    'atst': 'vehicle（AT-ST 步行机）', 'stap': 'vehicle（STAP 单兵飞行平台）',
    'aat': 'vehicle（AAT 坦克）',
}
GUESS_CHAR_KEYWORDS = (
    'luke', 'vader', 'leia', 'obiwan', 'kenobi', 'jedi', 'sith', 'barriss',
    'merrin', 'qimir', 'grogu', 'rex', 'appo', 'ithano', 'chamsyndulla',
    'batcher', 'k3p0', 'r5d4', 'astromech', 'fortuna', 'embo', 'chirpa',
    'kneesaa', 'jarjar', 'amidala', 'gungan', 'naboo', 'pyke', 'nightsister',
    'trooper', 'clone', 'droid', 'commando', 'partisan', 'ghost', 'rey', 'dryax',
    'zeffo', 'jotaz', 'haxion', 'morgan', 'nighttrooper', 'herasyndulla',
    'quiggold', 'mse6', 'silvo', 'brutus', 'drogan', 'tarpals', 'bossnass',
    'boomadier', 'phalanx', 'vanth', 'cinta', 'jocastanu', 'gray', 'sunfac',
    'cx', 'kylo', 'lando', 'traya', 'tusken', 'b1',
)


def guess_for_unclassified(model_id, name_en):
    c = model_core(model_id)
    for k, v in PROP_KEYWORDS.items():
        if k in c:
            return v
    blob = (c + ' ' + (name_en or '')).lower()
    if any(k in blob for k in GUESS_CHAR_KEYWORDS):
        return 'character model（gamedata 无对应单位，疑为剧情/活动/未实装用模型）'
    return None


def tags_of(cat_ids):
    cat_ids = cat_ids or []
    aligns = [a[len('alignment_'):] for a in cat_ids if a in ALIGNMENTS]
    return {
        'alignment': aligns[0] if aligns else None,
        'affiliations': sorted({c[len('affiliation_'):] for c in cat_ids
                                if c.startswith('affiliation_') and not c.endswith('_visible')}),
        'roles': sorted({c[len('role_'):] for c in cat_ids if c.startswith('role_')}),
        'professions': sorted({c[len('profession_'):] for c in cat_ids if c.startswith('profession_')}),
        'species': sorted({c[len('species_'):] for c in cat_ids if c.startswith('species_')}),
    }


def summon_flags(unit, unit_id=None):
    cats = unit.get('categoryId') or []
    f = {}
    if 'summoned_unit' in cats or (unit_id and 'SUMMON' in unit_id):
        f['is_summon'] = True
    if 'SOE_summoner' in cats:
        f['is_summoner'] = True
    return f


def unit_record(unit, source, defid=None):
    uid = unit.get('id') or ''
    rec = {
        'defId': defid or unit.get('baseId'),
        'unit_id_sample': uid.split(':')[0],
        'combatType': COMBAT_TYPE.get(unit.get('combatType'), unit.get('combatType')),
        'obtainable': unit.get('obtainable'),
        'source_table': source,
    }
    rec.update(tags_of(unit.get('categoryId')))
    rec.update(summon_flags(unit, uid))
    return rec


def main():
    units = load('units.json')
    pve = load('units_pve.json')
    gas = load('units_gas.json')
    models = json.load(open(os.path.join(GOH, 'models.json'), encoding='utf-8'))
    if isinstance(models, dict):
        models = models.get('data', [])
    char_names = load('character_names.json')

    # ---------- 索引 ----------
    hero_by_pref, hero_by_base = {}, {}
    for x in units:
        p = base_of_prefab(strip_unit_prefix(x['unitPrefab']))
        if p not in hero_by_pref or x.get('rarity', 0) > hero_by_pref[p].get('rarity', 0):
            hero_by_pref[p] = x
        if x['baseId'] not in hero_by_base or x.get('rarity', 0) > hero_by_base[x['baseId']].get('rarity', 0):
            hero_by_base[x['baseId']] = x
    gas_by_base = {}
    for x in gas:
        gas_by_base.setdefault(x['baseId'], x)
    def _pve_summon_rank(x):
        """带召唤标记的行优先作为该 baseId 的代表行（保证 is_summon 标志不因索引顺序丢失）"""
        cats = x.get('categoryId') or []
        return ('summoned_unit' in cats or 'SUMMON' in x.get('id', ''),)

    pve_by_pref, pve_by_id = {}, {}
    for x in pve:
        p = base_of_prefab(strip_unit_prefix(x['unitPrefab']))
        if p not in pve_by_pref or _pve_summon_rank(x) > _pve_summon_rank(pve_by_pref[p]):
            pve_by_pref[p] = x
        k = x['id'].split(':')[0]
        if k not in pve_by_id or _pve_summon_rank(x) > _pve_summon_rank(pve_by_id[k]):
            pve_by_id[k] = x

    hero_prefab_cores = {model_core(p) for p in hero_by_pref}
    pve_prefab_cores = {model_core(p) for p in pve_by_pref}
    hero_prefab_alnum = {alnum(c): c for c in hero_prefab_cores}   # ewok_chief -> ewokchief
    pve_prefab_alnum = {alnum(c): c for c in pve_prefab_cores}
    hero_base_alnum = {b: alnum(b) for b in hero_by_base}
    gas_base_alnum = {b: alnum(b) for b in gas_by_base}
    pve_id_alnum = {b: alnum(b) for b in pve_by_id}

    aliases = {}
    for k, v in NICKNAME_ALIASES.items():
        if v and (v in hero_by_base or v in gas_by_base):
            aliases[alnum(k)] = v          # 键按纯字母数字归一（leia_gl -> leiagl）

    results = OrderedDict()
    stats = Counter()

    def finish(mid, mrow, category, match, evidence, unit=None, source=None,
               hero_link=None, note=None):
        cnames = char_names.get(mid, {})
        entry = {
            'category': category,
            'match': match,
            'match_evidence': evidence,
            'name_en': cnames.get('name_en') or mrow.get('en'),
            'name_zh': cnames.get('name_zh') or mrow.get('zh'),
            'defId_model_map': cnames.get('defId'),
            'cn_map_confidence': cnames.get('match'),
            'variant_flags': variant_flags(mid),
        }
        if hero_link:
            entry['hero_link'] = hero_link
        if unit is not None:
            ur = unit_record(unit, source)
            ur['name_en'] = entry['name_en']
            entry['unit'] = ur
        if note:
            entry['note'] = note
        results[mid] = entry
        stats[category] += 1
        stats['match:' + match] += 1

    for mrow in models:
        mid = mrow['id']
        core = model_core(mid)
        marks = pve_markers(mid)

        # 1. hero prefab 精确
        if mid in hero_by_pref:
            finish(mid, mrow, 'hero', 'prefab_exact', 'unitPrefab=unit.%s（units.json）' % mid,
                   unit=hero_by_pref[mid], source='units.json')
            continue
        # 2. pve prefab 精确
        if mid in pve_by_pref:
            finish(mid, mrow, 'pve_variant', 'prefab_pve',
                   'unitPrefab=unit.%s（units_pve.json，非可玩单位）' % mid,
                   unit=pve_by_pref[mid], source='units_pve.json')
            continue

        # 3-5. defId 路径
        defid = char_names.get(mid, {}).get('defId')
        if defid:
            if defid in hero_by_base:
                if marks:
                    finish(mid, mrow, 'pve_variant', 'defId+hero_link',
                           'defId=%s（units.json 可玩单位）但模型 id 带 PVE 标记 %s' % (defid, marks),
                           unit=pve_by_id.get(defid, hero_by_base[defid]),
                           source='units.json/units_pve.json', hero_link=defid,
                           note='可玩单位 %s 的 PVE/剧情用变体模型' % defid)
                else:
                    finish(mid, mrow, 'hero', 'defId_variant',
                           'defId=%s（units.json）；模型 id 与该单位 prefab（%s）不同，为变体/备用模型'
                           % (defid, strip_unit_prefix(hero_by_base[defid]['unitPrefab'])),
                           unit=hero_by_base[defid], source='units.json', hero_link=defid)
                continue
            if defid in pve_by_id:
                hl = defid if defid in hero_by_base else None
                finish(mid, mrow, 'pve_variant', 'defId_pve',
                       'defId=%s 仅出现在 units_pve.json' % defid,
                       unit=pve_by_id[defid], source='units_pve.json', hero_link=hl)
                continue
            if defid in gas_by_base:
                g = gas_by_base[defid]
                if marks:  # gas 独有英雄的 PVE 用变体
                    finish(mid, mrow, 'pve_variant', 'defId_gas+hero_link',
                           'defId=%s（units_gas 可玩单位）但模型 id 带 PVE 标记 %s' % (defid, marks),
                           unit=pve_by_id.get(defid, g), source='units_gas.json/units_pve.json',
                           hero_link=defid,
                           note='units_gas 独有英雄 %s 的 PVE/剧情用变体模型' % defid)
                    continue
                finish(mid, mrow, 'hero', 'defId_gas',
                       'defId=%s 仅在 units_gas.json 完整单位表（obtainable=%s，该表无 unitPrefab 行）'
                       % (defid, g.get('obtainable')),
                       unit=g, source='units_gas.json', hero_link=defid,
                       note='units_gas 独有单位（units.json 尚无该单位的 prefab 行）')
                continue

        # 6. 模糊：token 递减 + 反向前缀（全部要求命中变体词白名单）
        #    元组: (category, match, evidence, defId, unit)
        toks = core.split('_')
        fuzzy = None
        for i in range(len(toks) - 1, 0, -1):
            dropped = toks[i:]
            if not any(re.fullmatch(VARWORD, d) for d in dropped):
                continue
            base_core = '_'.join(toks[:i])
            if base_core in hero_prefab_cores:
                fuzzy = ('hero', 'fuzzy_prefab',
                         '去掉尾部变体 token %s 后命中英雄 prefab 核心 char_%s_pre' % (dropped, base_core),
                         None, hero_by_pref.get('char_%s_pre' % base_core))
                break
            if base_core in pve_prefab_cores:
                fuzzy = ('pve_variant', 'fuzzy_prefab_pve',
                         '去掉尾部变体 token %s 后命中 PVE prefab 核心 char_%s_pre' % (dropped, base_core),
                         None, pve_by_pref.get('char_%s_pre' % base_core))
                break
        if not fuzzy:
            ca = alnum(core)
            m = re.match(r'^(.*?)(' + VARWORD + r')+$', ca)
            ca_base = m.group(1) if m else ca
            # 6a. prefab 核心的纯字母数字一致（模型 id 拼写差异，如 ewokchief vs ewok_chief）
            if ca_base in hero_prefab_alnum:
                _p = hero_prefab_alnum[ca_base]
                fuzzy = ('hero', 'fuzzy_prefab',
                         '核心名 %s 与英雄 prefab 核心 char_%s_pre 仅拼写不同' % (ca, _p),
                         None, hero_by_pref.get('char_%s_pre' % _p))
            elif ca_base in pve_prefab_alnum:
                _p = pve_prefab_alnum[ca_base]
                fuzzy = ('pve_variant', 'fuzzy_prefab_pve',
                         '核心名 %s 与 PVE prefab 核心 char_%s_pre 仅拼写不同' % (ca, _p),
                         None, pve_by_pref.get('char_%s_pre' % _p))
        if not fuzzy:
            ca = alnum(core)
            m = re.match(r'^(.*?)(' + VARWORD + r')+$', ca)
            ca_base = m.group(1) if m else ca
            if len(ca_base) >= 6:
                hit = next((b for b, a in hero_base_alnum.items() if a == ca_base), None) \
                    or next((b for b, a in gas_base_alnum.items() if a == ca_base), None)
                if hit:
                    fuzzy = ('hero', 'fuzzy_baseid',
                             '核心名 %s（去变体后缀）命中 baseId %s' % (ca, hit), hit,
                             hero_by_base.get(hit) or gas_by_base.get(hit))
                else:
                    def _var_suffix(s):
                        return re.fullmatch(r'(?:' + VARWORD + r')+', s) is not None
                    hit = next((b for b, a in hero_base_alnum.items()
                                if len(a) >= 6 and ca_base.startswith(a) and _var_suffix(ca_base[len(a):])), None)
                    if hit:
                        fuzzy = ('hero', 'fuzzy_baseid',
                                 '核心名 %s（= 英雄 baseId %s + 变体后缀 %s）' % (ca, hit, ca_base[len(alnum(hit)):]),
                                 hit, hero_by_base.get(hit))
                    else:
                        hit = next((b for b, a in gas_base_alnum.items()
                                    if len(a) >= 6 and ca_base.startswith(a) and _var_suffix(ca_base[len(a):])), None)
                        if hit:
                            fuzzy = ('hero', 'fuzzy_baseid_gas',
                                     '核心名 %s（去变体后缀）命中 units_gas baseId %s' % (ca, hit), hit,
                                     gas_by_base.get(hit))
                if not fuzzy:
                    def _pve_ok(item):
                        b, a = item
                        if len(a) < 5:
                            return False
                        if a == ca_base:
                            return True
                        return ca_base.startswith(a) and _var_suffix(ca_base[len(a):])
                    hit = next((b for b, a in pve_id_alnum.items() if _pve_ok((b, a))), None)
                    if hit:
                        fuzzy = ('pve_variant', 'fuzzy_pve_id',
                                 '核心名 %s（去变体后缀）命中 PVE 单位 %s' % (ca, hit), hit,
                                 pve_by_id.get(hit))
        if fuzzy:
            cat, match, ev, fdef, funit = fuzzy
            if cat == 'hero' and not fdef and funit is not None:
                fdef = funit.get('baseId')  # prefab 命中的英雄没有 defId 时，从单位行取
            if cat == 'hero' and marks:  # 模糊命中的是英雄，但模型 id 带 PVE 标记 -> PVE 变体
                finish(mid, mrow, 'pve_variant', 'fuzzy+hero_link', ev, unit=funit,
                       source='units.json/units_gas.json', hero_link=fdef,
                       note='模糊命中英雄 %s，但模型 id 带 PVE 标记 %s —— PVE/剧情用变体' % (fdef, marks))
            else:
                finish(mid, mrow, cat, match, ev, unit=funit,
                       source='units.json/units_gas.json/units_pve.json',
                       hero_link=fdef if cat == 'hero' else None,
                       note='模糊匹配（置信度低于精确/defId 路径，可复核）')
            continue

        # 7. 别名兜底（整名 / 去变体后缀名 / 右侧 token 递减 / 单 token；键均已 alnum 归一）
        _ca = alnum(core)
        _m = re.match(r'^(.*?)(' + VARWORD + r')+$', _ca)
        alias_hit = aliases.get(_ca) \
            or aliases.get(_m.group(1) if _m else _ca)
        if not alias_hit:
            for i in range(len(toks) - 1, 0, -1):
                alias_hit = aliases.get(alnum('_'.join(toks[:i])))
                if alias_hit:
                    break
        if not alias_hit:
            alias_hit = next((aliases[alnum(t)] for t in toks if alnum(t) in aliases), None)
        if alias_hit:
            src = hero_by_base if alias_hit in hero_by_base else gas_by_base
            finish(mid, mrow, 'hero', 'alias', '社区缩写别名 %s -> %s' % (alnum(core), alias_hit),
                   unit=src[alias_hit], source='units.json/units_gas.json', hero_link=alias_hit,
                   note='通过内嵌社区绰号/缩写表匹配')
            continue

        # 8. unclassified
        finish(mid, mrow, 'unclassified', 'none',
               'units.json / units_pve.json / units_gas.json 均无对应单位')

    for mid, e in results.items():
        if e['category'] == 'unclassified':
            e['guess'] = guess_for_unclassified(mid, e['name_en'])

    # ---------- 自检 ----------
    errs = []
    if len(results) != 747:
        errs.append('总数 %d != 747' % len(results))
    cat_sum = sum(stats[c] for c in ('hero', 'pve_variant', 'environment', 'cinematic',
                                     'ship_assets', 'other', 'unclassified'))
    if cat_sum != 747:
        errs.append('各类之和 %d != 747' % cat_sum)

    def check(mid, want_align, want_affils, want_cat='hero'):
        e = results.get(mid)
        if not e:
            errs.append('抽查缺失: %s' % mid)
            return
        if e['category'] != want_cat:
            errs.append('%s category=%s 期望 %s' % (mid, e['category'], want_cat))
            return
        u = e.get('unit') or {}
        if u.get('alignment') != want_align:
            errs.append('%s alignment=%s 期望 %s' % (mid, u.get('alignment'), want_align))
        for wa in want_affils:
            if wa not in (u.get('affiliations') or []):
                errs.append('%s affiliations=%s 缺 %s' % (mid, u.get('affiliations'), wa))

    # 注：官方数据无 affiliation_sith，维达的西斯标记是 affiliation_sithlord + profession_sith
    check('char_vader_pre', 'dark', ['empire', 'sithlord'])
    check('char_luke_ep4_pre', 'light', ['rebels'])
    for bb in ('char_hunter_pre', 'char_wrecker_pre', 'char_tech_pre', 'char_bb_echo_pre'):
        check(bb, 'light', ['badbatch'])

    n_char_units = len({x['baseId'] for x in units if x.get('combatType') == 1})
    n_ship_units = len({x['baseId'] for x in units if x.get('combatType') == 2})
    hero_prefabs_from_units = {base_of_prefab(strip_unit_prefix(x['unitPrefab']))
                               for x in units if x.get('combatType') == 1}
    hero_prefabs_from_models = {m for m, e in results.items() if e['match'] == 'prefab_exact'}
    if hero_prefabs_from_units != hero_prefabs_from_models:
        errs.append('prefab_exact 集合与 units.json CHARACTER prefab 集合不一致: '
                    '仅units=%s 仅models=%s' % (sorted(hero_prefabs_from_units - hero_prefabs_from_models),
                                                sorted(hero_prefabs_from_models - hero_prefabs_from_units)))
    if stats['match:prefab_exact'] != n_char_units:
        errs.append('prefab_exact=%s != units.json CHARACTER 单位数 %d' % (stats['match:prefab_exact'], n_char_units))

    if errs:
        print('SELF-CHECK FAILED:')
        for e in errs:
            print('  -', e)
        sys.exit(1)

    # ---------- 写 JSON ----------
    out = {
        '_meta': {
            'generated_by': 'tools/classify_models.py',
            'total_models': len(results),
            'category_counts': {c: stats[c] for c in ('hero', 'pve_variant', 'environment',
                                                      'cinematic', 'ship_assets', 'other', 'unclassified')},
            'hero_match_counts': {k: stats['match:' + k] for k in
                                  ('prefab_exact', 'defId_variant', 'defId_gas', 'fuzzy_prefab',
                                   'fuzzy_baseid', 'fuzzy_baseid_gas', 'alias') if stats['match:' + k]},
            'pve_match_counts': {k: stats['match:' + k] for k in
                                 ('prefab_pve', 'defId+hero_link', 'defId_pve', 'fuzzy_prefab_pve',
                                  'fuzzy_pve_id') if stats['match:' + k]},
            'units_json_unique_units': len(hero_by_base),
            'units_json_character_units': n_char_units,
            'units_json_ship_units': n_ship_units,
            'units_gas_unique_baseids': len(gas_by_base),
            'match_priority': ['prefab_exact(hero)', 'prefab_pve', 'defId->units.json',
                               'defId->units_pve.json', 'defId->units_gas.json',
                               'fuzzy(token/prefix)', 'alias', 'unclassified'],
            'summon_conclusion': '可玩单位表无召唤物；召唤物仅在 units_pve.json（summoned_unit / *SUMMON*），'
                                 'SOE_summoner 是召唤者标记；本表以 is_summon/is_summoner 字段输出',
            'sources': ['models.json', 'catalog/units.json', 'catalog/units_pve.json',
                        'catalog/units_gas.json', 'catalog/character_names.json'],
        },
        'models': results,
    }
    with open(OUT_JSON, 'w', encoding='utf-8') as f:
        json.dump(out, f, ensure_ascii=False, indent=1)

    write_md(results, stats, hero_prefabs_from_models, n_char_units, n_ship_units)
    print('OK: %d 个模型全部分类' % len(results))
    print('  JSON -> %s' % os.path.relpath(OUT_JSON, GOH))
    print('  MD   -> %s' % os.path.relpath(OUT_MD, GOH))
    for c in ('hero', 'pve_variant', 'environment', 'cinematic', 'ship_assets', 'other', 'unclassified'):
        print('  %-14s %4d' % (c, stats[c]))
    print('自检通过: 总数=747、抽查(vader=dark/empire/sith, luke=light/rebels, badbatch)通过、'
          'hero prefab_exact=%d == units.json CHARACTER 单位数=%d（一一对应）'
          % (stats['match:prefab_exact'], n_char_units))


def write_md(results, stats, hero_prefab_set, n_char_units, n_ship_units):
    L = []
    A = L.append
    A('# 模型分类报告（models.json → 英雄 / 非英雄）')
    A('')
    A('> 生成: `tools/classify_models.py` ｜ 数据（全本地）: `models.json` + `catalog/units.json` + '
      '`catalog/units_pve.json` + `catalog/units_gas.json` + `catalog/character_names.json`')
    A('')
    A('## 一、总览')
    A('')
    A('| 类别 | 数量 | 说明 |')
    A('|---|---|---|')
    A('| hero | %d | 可玩单位：units.json 精确 prefab / defId 变体 / units_gas 独有单位 |' % stats['hero'])
    A('| pve_variant | %d | 仅出现在 units_pve.json 的 PVE/敌方单位，或英雄的 PVE/剧情变体 |' % stats['pve_variant'])
    A('| environment | %d | models.json 无 env_* 前缀（见下） |' % stats['environment'])
    A('| cinematic | %d | models.json 无 cin_* 前缀（见下） |' % stats['cinematic'])
    A('| ship_assets | %d | models.json 无 ship_* 前缀（见下） |' % stats['ship_assets'])
    A('| other | %d | 无（非单位资产留在 unclassified 并给 guess） |' % stats['other'])
    A('| unclassified | %d | 三张单位表都找不到（附 guess 直觉标注） |' % stats['unclassified'])
    A('| **合计** | **%d** |  |' % sum(stats[c] for c in ('hero', 'pve_variant', 'environment',
                                                        'cinematic', 'ship_assets', 'other', 'unclassified')))
    A('')
    A('**关于 env_*/cin_*/ship_* 前缀**：`models.json` 的 747 个 id **全部**以 `char_` 开头'
      '（740 个以 `_pre` 结尾），不存在 env_*/cin_*/ship_* 前缀，故这三个类别为 0。'
      '环境/飞船/过场内容在 `catalog/other_content.json`（env 269、ship 92、cinematic 284 个 bundle）里，'
      '不在本模型清单中。少量非角色资产（`char_crate_a3`、`char_damage`、`char_starmap_pre`、'
      '`char_shieldgenerator_pre` 等）留在 unclassified，用 `guess` 字段标注直觉判断。')
    A('')
    A('## 二、hero 判定方式分布')
    A('')
    A('| hero_match | 数量 | 含义 |')
    A('|---|---|---|')
    hm = [('prefab_exact', '模型 id 与 units.json 某可玩单位 unitPrefab 完全一致（去 unit. 前缀）'),
          ('defId_variant', 'defId 对应 units.json 可玩单位但模型 id ≠ 其 prefab —— 该英雄的变体/备用/皮肤模型（speederbike、holo、remastered 等），带 hero_link'),
          ('defId_gas', 'defId 仅在 units_gas.json 完整单位表（obtainable=True；gas 无 unitPrefab，是 units.json 超集，多 110 个较新单位）'),
          ('fuzzy_prefab', '去尾部变体 token 后命中英雄 prefab 核心（低置信）'),
          ('fuzzy_baseid', '去变体后缀后命中英雄 baseId（低置信）'),
          ('fuzzy_baseid_gas', '同上，目标在 units_gas（低置信）'),
          ('alias', '社区绰号/缩写别名命中（低置信）')]
    for k, desc in hm:
        if stats['match:' + k]:
            A('| %s | %d | %s |' % (k, stats['match:' + k], desc))
    A('')
    A('## 三、pve_variant 判定方式分布')
    A('')
    pm = [('prefab_pve', '模型 id 与 units_pve.json 单位 unitPrefab 一致（含 char_xxx_pre.yyy 子 prefab 归一）'),
          ('defId+hero_link', 'defId 是可玩英雄但模型 id 带 _pve/_raid/_nokill/_enemy/_boss/_noshield —— 英雄的 PVE 用变体（附 hero_link）'),
          ('defId_gas+hero_link', 'defId 是 units_gas 独有英雄但模型 id 带 PVE 标记（附 hero_link）'),
          ('defId_pve', 'defId 仅存在于 units_pve.json'),
          ('fuzzy+hero_link', '模糊命中英雄但模型 id 带 PVE 标记（低置信，附 hero_link）'),
          ('fuzzy_prefab_pve', '去尾 token 后命中 PVE prefab 核心（低置信）'),
          ('fuzzy_pve_id', '去变体后缀后命中 PVE 单位 id（低置信）')]
    A('| pve_match | 数量 | 含义 |')
    A('|---|---|---|')
    for k, desc in pm:
        if stats['match:' + k]:
            A('| %s | %d | %s |' % (k, stats['match:' + k], desc))
    A('')
    A('## 四、hero 阵营分布')
    A('')
    align_c, affil_c, role_c = Counter(), Counter(), Counter()
    for e in results.values():
        if e['category'] == 'hero' and e.get('unit'):
            align_c[e['unit']['alignment']] += 1
            for a in e['unit']['affiliations']:
                affil_c[a] += 1
            for r in e['unit']['roles']:
                role_c[r] += 1
    A('| alignment | 数量 |')
    A('|---|---|')
    for a, n in align_c.most_common():
        A('| %s | %d |' % (a or '（无标签）', n))
    A('')
    A('Top 派系（affiliation_*，一个英雄可属多个）：')
    A('')
    for a, n in affil_c.most_common(15):
        A('- %s: %d' % (a, n))
    A('')
    A('定位分布（role_*）：' + '、'.join('%s %d' % (r, n) for r, n in role_c.most_common()))
    A('')
    A('## 五、判定规则（优先级从高到低）')
    A('')
    A('1. **prefab_exact**：`model_id == strip(unitPrefab)`（`unit.char_vader_pre` → `char_vader_pre`）；'
      'PVE 子 prefab（`char_aat_pre.phase01`）先按 `.` 截断。')
    A('2. **prefab_pve**：同上命中 units_pve.json 且不在 units.json。')
    A('3. **defId 路径**：`character_names.json` 的官方 defId（源自 gamedata 镜像，`cn_map_confidence` 保留其 match 置信度）：'
      '命中 units.json → hero；若模型 id 带 `_pve/_raid/_nokill/_enemy/_boss/_noshield` → 该英雄的 pve_variant（附 hero_link）；'
      '仅命中 units_pve.json → pve_variant；仅在 units_gas.json → hero（见数据坑 #2）。')
    A('4. **模糊**：核心名从右侧剥变体 token（speederbike/holo/cin/gl/s3/remastered/jediknight/…，白名单校验），'
      '或"baseId 前缀 + 变体后缀"反向前缀匹配。所有模糊命中带 `note`（低置信，可复核）。')
    A('5. **别名兜底**：内嵌社区缩写表（JML/JMK/GAS/SEE/LV 等，运行时校验目标存在才启用）。'
      '参考了 LukeSONG2000/goh-skill `lib/names.py` 的检索逻辑；其绰号数据 `names.json` 被上游 .gitignore 无法拉取，'
      '故为策展内嵌版。')
    A('6. 未命中 → **unclassified**（`guess` 字段：prop/载具/生物/角色模型 直觉猜测）。')
    A('')
    A('## 六、召唤（summon）结论 —— 先查证，不硬标')
    A('')
    A('- `units.json`（可玩 301 单位×7 星）：**没有任何** `summoned_unit` 类别；`summonStatTable` 字段 2,107 行**全空**。'
      '"可玩单位"与"召唤物"在官方数据里是分开的，模型目录中不存在"可玩召唤物"。')
    A('- 任务猜测的 `hermityoda`/`BB8`：都是 units.json 的**普通可玩单位**（HERMITYODA、BB8，obtainable=True、'
      'any_obtainable、有 alignment 标签），**不是**召唤物的标记方式。')
    A('- 召唤物的真实标记在 `units_pve.json`：`categoryId` 含 **`summoned_unit`**（52 行：blaster turret、'
      'Geonosian Brute、Vulture Droid、VCX100/YT2400/YV666 舰船、TIE Scythe、Conquest 补给箱 OBJ_CRATE_01 等），'
      '以及 id 含 `*SUMMON*` 的标量行（`PVE_VULTUREDROID:SUMMON_VULTURE`、`...:MALEVOLENCE_SUMMON_VULTURE` 等）。')
    A('- `SOE_summoner` 标记的是**召唤者**：可玩侧仅 2 个（ARCTROOPER501ST、GEONOSIANBROODALPHA，各×7 星）。')
    A('- 因此不设 summon 类别；命中单位带上述标记时，JSON 输出 `is_summon`/`is_summoner` 字段。'
      '本表实测：%d 个模型命中 is_summon（均为 PVE 侧召唤物变体：%s），%d 个命中 is_summoner（%s）——'
      '后者正是可玩侧仅有的两个 SOE_summoner 单位，与官方数据完全吻合。'
      % (sum(1 for e in results.values() if (e.get('unit') or {}).get('is_summon')),
         '、'.join(m for m, e in results.items() if (e.get('unit') or {}).get('is_summon')) or '无',
         sum(1 for e in results.values() if (e.get('unit') or {}).get('is_summoner')),
         '、'.join(m for m, e in results.items() if (e.get('unit') or {}).get('is_summoner')) or '无'))
    A('')
    A('## 七、自检')
    A('')
    A('- hero + pve_variant + environment + cinematic + ship_assets + other + unclassified = 747 ✅')
    A('- 抽查：`char_vader_pre` = dark / [empire, sithlord]（官方无 `affiliation_sith` 标签，西斯派系叫 '
      '`sithlord`，另有 `profession_sith`）✅ ｜ `char_luke_ep4_pre`（Luke Skywalker）= light / [rebels] ✅ ｜ '
      'Bad Batch（Hunter/Wrecker/Tech/Echo）= `affiliation_badbatch` ✅')
    A('- **hero 对应关系**：`prefab_exact` 的 %d 个模型与 units.json 的 **%d 个 CHARACTER 单位**（combatType=1，'
      '全部 any_obtainable）**一一对应**（prefab 集合双向无差集）✅。另有 %d 个 SHIP 单位（`unit.ship_*`）'
      '不在 char_* 模型清单中；hero 其余数量来自 defId 变体与 units_gas 独有单位（见第二节）。'
      % (len(hero_prefab_set), n_char_units, n_ship_units))
    A('')
    A('## 八、数据坑')
    A('')
    A('1. **units.json 每单位 7 行**：2,107 行 = 301 单位 × 7 星级（id 形如 `VADER:SEVEN_STAR`），统计必须按 baseId 去重。')
    A('2. **units_gas.json 两层嵌套 `{version, data}` 且无 unitPrefab**，只能按 baseId/defId 关联；'
      '它是 units.json 的**真超集**（411 vs 301 个 baseId），多出的 110 个（4LOM、Zuckuss、Kix、Maz Kanata、'
      'Jaxxon、Baylan Skoll、Shin Hati、Captain Enoch、GL Hondo、GL Leia、GL Ahsoka 等，多为 2024-25 新内容）'
      '在 units_pve.json 中**零重叠**，判 hero 的依据是官方 `any_obtainable` + obtainable=True。'
      '风险：无法 100% 排除个别是未实装数据；注意 Qimir、CX-2 等 Ahsoka/残次品角色连 gas 表都没有，只能留在 unclassified。')
    A('3. **PVE 子 prefab 带点号**：`char_aat_pre.phase01`、`char_rancor_pre.rancor_pit`、`char_veh_atst_pre.atst_hoth` 等，'
      '不截断会漏掉 AAT/Rancor/AT-ST 等大型载具与 Raid Boss 模型。')
    A('4. **character_names.json 有 104 个模型无 defId**（match=unmatched_in_gamedata），且约 90 个 defId 来自 token 剥离推导'
      '（match=derived_*）；只依赖它会传播误差 —— 本工具把它降为"路径之一"并原样输出 `cn_map_confidence`。')
    A('5. **defId 双重命中**：很多 defId 同时在 units.json 和 units_pve.json（英雄也会在 PVE 出怪），'
      '需靠模型 id 的 `_pve/_raid/_enemy…` 标记分辨本体还是敌方变体。')
    A('6. units_pve.json 同一 baseId 有十几行难度变体（`PVE_XXX:MODE_ELITE`），索引前必须去重。')
    A('7. 模型 id 本身的坑：`char_crate_a3`、`char_damage`、`char_traya_saberenemy` 不带 `_pre` 后缀；'
      '`char_maul_hatefueled_pre_holo`、`char_thrawnadmiral_pre_holo` 的后缀在 `_pre` 之后。')
    A('')
    A('## 九、样例清单')
    A('')
    A('### hero · prefab_exact 抽样 20')
    A('')
    A('```')
    n = 0
    for mid, e in results.items():
        if e['match'] == 'prefab_exact':
            u = e['unit']
            A('%-34s %-22s %-7s %s' % (mid, e['defId_model_map'] or u['defId'], u['alignment'],
                                       ','.join(u['affiliations'][:3])))
            n += 1
            if n >= 20:
                break
    A('```')
    A('')
    A('### hero · defId_variant 抽样 15（英雄的变体/备用模型）')
    A('')
    A('```')
    n = 0
    for mid, e in results.items():
        if e['match'] == 'defId_variant':
            A('%-42s -> %s' % (mid, e['hero_link']))
            n += 1
            if n >= 15:
                break
    A('```')
    A('')
    A('### hero · defId_gas 全列表（units_gas 独有单位）')
    A('')
    A('```')
    for mid, e in results.items():
        if e['match'] == 'defId_gas':
            A('%-42s -> %-24s %-7s' % (mid, e['unit']['defId'], e['unit']['alignment']))
    A('```')
    A('')
    A('### pve_variant 抽样 20')
    A('')
    A('```')
    n = 0
    for mid, e in results.items():
        if e['category'] == 'pve_variant':
            u = e.get('unit') or {}
            A('%-44s %-28s %-7s' % (mid, (e.get('hero_link') or u.get('defId') or u.get('unit_id_sample') or '?'),
                                    u.get('alignment')))
            n += 1
            if n >= 20:
                break
    A('```')
    A('')
    A('### unclassified 全列表（附 guess）')
    A('')
    A('```')
    for mid, e in results.items():
        if e['category'] == 'unclassified':
            A('%-46s %-36s %s' % (mid, e.get('name_en') or '?', e.get('guess') or '-'))
    A('```')
    A('')
    with open(OUT_MD, 'w', encoding='utf-8') as f:
        f.write('\n'.join(L))


if __name__ == '__main__':
    main()
