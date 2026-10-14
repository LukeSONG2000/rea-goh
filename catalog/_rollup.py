import json, re, collections

d = json.load(open(r'F:\PPfavorite\Work\goh\catalog\_bundle_names.json'))
samples = json.load(open(r'F:\PPfavorite\Work\goh\catalog\_samples.json'))

# (category, description, regex) - first match wins
RULES = [
    ('char',            '角色模型 prefab（主线已导出）', r'^char_(?!ui)'),
    ('charui',          '角色 UI 头像/图标单贴图', r'^(charui_|jg_)'),
    ('env',             '环境场景 prefab（地块/建筑/星球背景）', r'^env_'),
    ('scene',           '命名场景（竞技场/行星/房间）', r'_scene$'),
    ('npc',             '酒馆 NPC / 共享角色 prefab', r'^(cantinapatron_|shar_)'),
    ('ship',            '舰船模型 prefab', r'^(ship_|malevolence_pre)'),
    ('cinematic',       '过场演出（Timeline / 演出 prefab）', r'^(cin_|col_0|pas_)'),
    ('territorymap',    '领土战/大会战 3D 地图', r'^(territorymap_|tb3_|era_battle)'),
    ('battle',          '战斗布局/相机/出生点配置', r'^(battle_|node_|arena_|territory(?!map|ui|war|tournament))'),
    ('vfx',             '独立技能特效/战斗投射物 prefab', r'^(vfx_|fx_|blaster_|bantha_|ballista_|circular_glow)'),
    ('ability',         '技能特效/技能图标', r'^ability_'),
    ('ui_view',         'UI 视图 prefab（界面/弹窗）', r'(_view$|_subview$|atlas|^ui_)'),
    ('store_art',       '商店礼包/新闻报/活动横幅单贴图', r'^(pack|packs_|packopening_|bundleoffer|newsletter|neswletter|crystalpackart|mosdpack|purchase|sliceabundle)'),
    ('icon_art',        '图标/徽章/旗帜/头像单贴图', r'^(icon|logo|guild_|league_|challenge|consumable|chest|relic_|equip|vanity|crewportrait|commanderportrait|holoplanet|background|pabu|episode_|events|datacron|datacrons|era_|marquee|unitguide|help|reward|daily|inbox|contextual|rewards|skill|objective)'),
    ('map_ui',          '游戏模式界面（征服/竞技场/公会战/库存）', r'^(conquest|coliseum|territoryui|territorywar|territorybattle|territorytournament|guildraid|guildui|shippvp|championship|scavenger|modenhancement|charactermods|inventoryui|storeui|unitdetailsui|abilitydetailsui|abilityui|objectives|homebaseui|playerprofileui|settingsui|campaignui|matchmakingui|alliesui|prebattleui|battleui|datacronui|filtersort|unit_|loaned|returning|shipunlock|raidsith|raidrancor|guildraidmission|contextualmessageui|conquestpass|shippvp3v3|coliseumhome)'),
    ('shared',          '共享资源容器（网格/贴图/动画/着色器池）', r'^shared'),
]

def classify(name):
    for cat, desc, rx in RULES:
        if re.search(rx, name): return cat
    return 'other'

cats = collections.defaultdict(lambda: {'n':0,'size':0,'examples':[],'sample':None})
for k,v in d.items():
    c = classify(k)
    cats[c]['n'] += 1
    cats[c]['size'] += v['size']
    if len(cats[c]['examples']) < 8: cats[c]['examples'].append(k)

# attach samples
for cat, recs in samples.items():
    m = {'ship':'ship','env':'env','cin_scene':'scene','cin_other':'cinematic','pack':'store_art',
         'ability':'ability','vfx':'vfx','objective':'icon_art','territorymap':'territorymap',
         'battle':'battle','background':'icon_art','cantinapatron':'npc','vanity':'icon_art',
         'newsletter':'store_art','charui':'charui','ui':'ui_view','relic':'icon_art','equip':'icon_art',
         'guild':'icon_art','conquest':'map_ui','events':'icon_art','shared_misc':'shared','misc_odd':'other'}
    tgt = m.get(cat)
    if tgt and tgt in cats: cats[tgt]['sample'] = recs

other_ex = [k for k in d if classify(k)=='other']
out = {'total_bundles': len(d), 'total_bytes': sum(v['size'] for v in d.values()),
       'categories': {c: dict(cats[c]) for c in cats},
       'unclassified_examples': other_ex[:40], 'unclassified_count': len(other_ex)}
json.dump(out, open(r'F:\PPfavorite\Work\goh\catalog\other_content.json','w'), indent=1, ensure_ascii=False)
for c in sorted(cats, key=lambda c:-cats[c]['size']):
    print(f"{c:14s} {cats[c]['n']:6d} {cats[c]['size']/1e6:9.1f} MB")
print('unclassified:', len(other_ex), other_ex[:15])
