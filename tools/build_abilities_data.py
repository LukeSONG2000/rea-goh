# -*- coding: utf-8 -*-
"""Export original references and a lossless battle effect graph.

abilities.json is a unique-unit menu index selecting the highest available tier
for the offline demo, not account progress. battle_effects.json preserves EVERY
source ability/effect row (effects in manifest-listed shards), including passives, nested references, conditions,
original enum values and fixed-point numbers. Exporting a row does not claim
that the runtime implements it. No effect aggregation belongs in this exporter.
"""
import json
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
GAMEDATA = ROOT / 'catalog' / 'gamedata'
OUT = ROOT / 'unity/SWGoH-Restored/Assets/StreamingAssets/abilities.json'
GRAPH_OUT = OUT.with_name('battle_effects.json')
ACTIVATED_TYPES = (7, 10)
EFFECTS_PER_FILE = 10000


def read_json(path):
    with open(path, encoding='utf-8') as stream:
        return json.load(stream)


def load(fname):
    return read_json(GAMEDATA / fname)['data']


def pretty(ability_id):
    tail = re.sub(r'_u\d+$', '', ability_id.split('_', 2)[-1])
    return tail.replace('_', ' ').strip().title() or ability_id


def index_unique(rows, label):
    indexed = {}
    for row in rows:
        key = row['id']
        if key in indexed:
            raise ValueError(f'Duplicate {label} id: {key}')
        indexed[key] = row
    return indexed


def build_exports(units, skill_rows, ability_rows, effect_rows, loc_cn=None, loc_en=None):
    """Return menu index and source graph; never alter input rows."""
    skills = index_unique(skill_rows, 'skill')
    abilities = index_unique(ability_rows, 'ability')
    index_unique(effect_rows, 'effect')
    loc_cn, loc_en = loc_cn or {}, loc_en or {}
    output, unit_signatures = {}, {}
    for unit in units:
        base_id = unit.get('baseId')
        if not base_id:
            continue
        # Fail on disagreement instead of choosing an arbitrary rarity variant.
        signature = (unit.get('categoryId', []), unit.get('skillReference', []),
                     unit.get('squadPositionPriority', 0))
        if base_id in unit_signatures:
            if unit_signatures[base_id] != signature:
                raise ValueError(f'Unit variants disagree on categories/skills/squad priority: {base_id}')
            continue
        unit_signatures[base_id] = signature
        records, passives = [], []
        for reference in unit.get('skillReference') or []:
            sid = reference.get('skillId', '')
            kind = sid.split('_', 1)[0]
            if sid not in skills:
                raise ValueError(f'Missing skill {sid} for {base_id}')
            aid = skills[sid].get('abilityReference')
            if not isinstance(aid, str) or aid not in abilities:
                raise ValueError(f'Missing ability {aid!r} for {sid}')
            ability = abilities[aid]
            activated = kind in ('basicskill', 'specialskill') and ability.get('abilityType') in ACTIVATED_TYPES
            tiers = ability.get('tier') or []
            selected_tier = len(tiers) - 1
            selected = tiers[-1] if tiers else ability
            # Explicitly empty tier references stay empty, with no fallback.
            refs = selected.get('effectReference', ability.get('effectReference', []))
            names = [loc.get(ability.get('nameKey', '')) for loc in (loc_cn, loc_en)]
            name = next((n for n in names if n and n != 'PLACEHOLDER'), pretty(aid))
            (records if activated else passives).append({
                'id': aid, 'skill_id': sid, 'name': name,
                'kind': ('basic' if kind == 'basicskill' else 'special') if activated else kind,
                'cooldown': ability.get('cooldown', 0),
                'abilityType': ability['abilityType'],
                'selectedTier': selected_tier, 'effectReference': refs,
            })
        if records or passives:
            output[base_id] = {'def_id': base_id, 'categoryId': unit.get('categoryId', []),
                               'squadPositionPriority': unit.get('squadPositionPriority', 0),
                               'abilities': records, 'passives': passives}
    graph = {'schemaVersion': 2, 'effects': effect_rows, 'abilities': ability_rows}
    return list(output.values()), graph


def main():
    units = read_json(ROOT / 'catalog/units.json')
    localization = []
    for language in ('cn', 'en'):
        path = ROOT / f'catalog/loc_{language}.json'
        localization.append(read_json(path) if path.exists() else {})
    menus, graph = build_exports(units, load('skill.json'), load('ability.json'),
                                load('effect.json'), *localization)
    OUT.parent.mkdir(parents=True, exist_ok=True)
    effect_files = []
    outputs = [(OUT, menus)]
    for offset in range(0, len(graph['effects']), EFFECTS_PER_FILE):
        filename = f'battle_effects_{offset // EFFECTS_PER_FILE:02d}.json'
        effect_files.append(filename)
        outputs.append((OUT.with_name(filename),
                        {'effects': graph['effects'][offset:offset + EFFECTS_PER_FILE]}))
    manifest = {'schemaVersion': graph['schemaVersion'],
                'effectFiles': effect_files, 'abilities': graph['abilities']}
    # Manifest is written last; readers use only its named shards.
    outputs.append((GRAPH_OUT, manifest))
    for path, data in outputs:
        encoded = json.dumps(data, ensure_ascii=False, separators=(',', ':'))
        if not path.exists() or path.read_text(encoding='utf-8') != encoded:
            path.write_text(encoded, encoding='utf-8')
    print(f'Exported {len(menus)} unique units / '
          f"{sum(len(u['abilities']) for u in menus)} menu abilities; "
          f"preserved {len(graph['abilities'])} original abilities / "
          f"{len(graph['effects'])} original effects (schema 2, highest-tier demo).")


if __name__ == '__main__':
    main()
