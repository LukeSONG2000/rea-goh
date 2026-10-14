import os, json, re, collections, sys, time
import UnityPy

CACHE = r'C:\Users\PPfavorite\AppData\LocalLow\Unity\Electronic Arts_SWGoH'
names = json.load(open(r'F:\PPfavorite\Work\goh\catalog\_bundle_names.json'))

def latest_data(name):
    root = os.path.join(CACHE, name)
    best = None; best_mt = -1
    for h in os.scandir(root):
        if not h.is_dir(): continue
        d = os.path.join(h.path, '__data')
        if os.path.isfile(d):
            mt = h.stat().st_mtime
            if mt > best_mt: best_mt = mt; best = d
    return best

def pick(regex, n, exclude=()):
    r = re.compile(regex); ex = tuple(exclude)
    cands = [k for k in names if r.match(k) and not k.startswith(ex)]
    cands.sort(key=lambda k: -names[k]['size'])
    step = max(1, len(cands)//n)
    return cands[::step][:n]

CATEGORIES = {
    'ship':        pick(r'ship_', 5, exclude=('shipunlock','shippvp')),
    'env':         pick(r'env_', 6),
    'cin_scene':   pick(r'.*_scene$', 6),
    'cin_other':   pick(r'cin_', 5),
    'pack':        pick(r'pack_', 6, exclude=('packopening','packs')),
    'ability':     pick(r'ability_', 6, exclude=('abilityui',)),
    'vfx':         pick(r'vfx_', 4),
    'objective':   pick(r'objective_', 4, exclude=('objectives',)),
    'territorymap':pick(r'territorymap_', 4),
    'battle':      pick(r'battle_', 4, exclude=('battleui',)),
    'background':  pick(r'background_', 3),
    'cantinapatron':pick(r'cantinapatron_', 3),
    'vanity':      pick(r'vanity_', 3),
    'newsletter':  pick(r'newsletter_', 3),
    'charui':      pick(r'charui_', 3),
    'ui':          pick(r'ui_', 3),
    'relic':       pick(r'relic_', 3),
    'equip':       pick(r'equip_', 3),
    'guild':       pick(r'guild_', 3),
    'conquest':    pick(r'conquest_', 3),
    'events':      pick(r'events_', 3),
    'shared_misc': [k for k in names if k.startswith('shared_')],
    'misc_odd':    ['malevolence_pre','pabu_sunset','shar_cantina_pre','emp_throne_room_dest_scene','fo_kylo_quarters_kijimi_scene','logo_splash_era08_newrepublic','era_characterupgradecelebrationui_view'] if True else [],
}

out = {}
order = ['ship','env','cin_scene','cin_other','pack','ability','vfx','objective','territorymap','battle','background','cantinapatron','vanity','newsletter','charui','ui','relic','equip','guild','conquest','events','shared_misc','misc_odd']
for cat in order:
    bundles = CATEGORIES.get(cat, [])
    out[cat] = []
    for b in bundles:
        p = latest_data(b)
        rec = {'bundle': b, 'size': names.get(b,{}).get('size')}
        if not p:
            rec['error'] = 'no __data'; out[cat].append(rec); continue
        try:
            env = UnityPy.load(p)
            types = collections.Counter()
            gos = []
            for o in env.objects:
                types[o.type.name] += 1
                if o.type.name == 'GameObject' and len(gos) < 20:
                    try: gos.append(o.read().m_Name)
                    except Exception: gos.append('<unreadable>')
            rec['types'] = dict(types.most_common(12))
            rec['names'] = gos
            # texture names if any
            tns = []
            for o in env.objects:
                if o.type.name == 'Texture2D' and len(tns) < 12:
                    try: tns.append(o.read().m_Name)
                    except Exception: pass
            rec['textures'] = tns
        except Exception as e:
            rec['error'] = f'{type(e).__name__}: {e}'
        out[cat].append(rec)
        print(f'[{cat}] {b}: {rec.get("types", rec.get("error"))}', flush=True)

json.dump(out, open(r'F:\PPfavorite\Work\goh\catalog\_samples.json','w'), indent=1, ensure_ascii=False)
print('DONE')
