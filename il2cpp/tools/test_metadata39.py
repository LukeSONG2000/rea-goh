"""Validation script for the v39 global-metadata parser.

Run:  py tools/test_metadata39.py
"""
import os
import re
import sys
import unicodedata

# Windows consoles often default to a legacy codepage (e.g. GBK); force UTF-8
sys.stdout.reconfigure(encoding='utf-8', errors='replace')

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from metadata39 import Metadata39, SECTION_NAMES

METADATA_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                             '..', 'data', 'global-metadata.dat')


def printable_ratio(s: str) -> float:
    if not s:
        return 1.0
    good = sum(1 for ch in s
               if ch.isprintable() or unicodedata.category(ch).startswith('Z'))
    return good / len(s)


def main():
    m = Metadata39(METADATA_PATH, validate=True)
    print(f'global-metadata.dat: {m.file_size:,} bytes, '
          f'magic=0x{m.sanity:08X}, version={m.version}')
    assert m.validate(strict=True)
    print('validate(): ALL CONSISTENCY CHECKS PASSED\n')

    # ------------------------------------------------------------------
    print('=' * 78)
    print('HEADER SECTION LAYOUT (31 sections x (offset, size, count))')
    print('=' * 78)
    print(f'{"section":<42}{"offset":>12}{"size":>12}{"count":>10}  elem')
    for name in SECTION_NAMES:
        s = m.header[name]
        elem = f'{s.size / s.count:.2f}' if s.count else '-'
        print(f'{name:<42}{s.offset:>12,}{s.size:>12,}{s.count:>10,}  {elem}')
    print(f'header size: {m.header_size} bytes; sections end at '
          f'{m.header[SECTION_NAMES[-1]].offset + m.header[SECTION_NAMES[-1]].size:,} '
          f'(file size {m.file_size:,})')

    # ------------------------------------------------------------------
    print()
    print('=' * 78)
    print('FIRST 20 IDENTIFIERS FROM STRING BLOB')
    print('=' * 78)
    ids = m.identifiers
    for i, s in enumerate(ids[:20]):
        print(f'  [{i}] {s!r}')

    # ------------------------------------------------------------------
    print()
    print('=' * 78)
    print('FIRST 30 TYPE DEFINITIONS')
    print('=' * 78)
    for td in m.typedefs[:30]:
        print(f'  [{td.index:>2}] {td.full_name:<45} '
              f'methods={td.method_count:<4} fields={td.field_count:<3} '
              f'token=0x{td.token:08X}')

    # ------------------------------------------------------------------
    print()
    print('=' * 78)
    print('TYPE COUNTS')
    print('=' * 78)
    print(f'  type definitions : {len(m.typedefs):>8,}')
    print(f'  methods          : {len(m.methods):>8,}')
    print(f'  fields           : {len(m.fields):>8,}')
    print(f'  parameters       : {len(m.parameters):>8,}')
    print(f'  properties       : {len(m.properties):>8,}')
    print(f'  events           : {len(m.events):>8,}')
    print(f'  string literals  : {len(m.literals):>8,}')
    print(f'  identifier strings: {len(ids):>7,}')
    print(f'  images/assemblies : {len(m.images):>7,}')

    # ------------------------------------------------------------------
    print()
    print('=' * 78)
    print('IDENTIFIER PRINTABILITY CHECK')
    print('=' * 78)
    sample = ids[::max(1, len(ids) // 5000)]
    ratios = [printable_ratio(s) for s in sample]
    good = sum(1 for r in ratios if r >= 0.95)
    print(f'  sampled {len(sample)} identifiers: {good} ({good / len(sample):.2%}) '
          f'are >=95% printable')
    bad = [s for s, r in zip(sample, ratios) if r < 0.95][:5]
    if bad:
        print(f'  examples below threshold: {bad!r}')

    # ------------------------------------------------------------------
    print()
    print('=' * 78)
    print('SEED SEARCH: types matching Unit/Definition/Request/Response/Battle')
    print('=' * 78)
    pats = ['Unit', 'Definition', 'Request', 'Response', 'Battle']
    for pat in pats:
        rx = re.compile(pat, re.IGNORECASE)
        hits = [td for td in m.typedefs if rx.search(td.name)]
        print(f'\n  --- "{pat}": {len(hits)} matches, first 40:')
        for td in hits[:40]:
            print(f'    {td.full_name}')

    # ------------------------------------------------------------------
    print()
    print('=' * 78)
    print('SPOT CHECKS')
    print('=' * 78)
    td = m.typedef_by_full_name('AbilityDetailsView')
    if td:
        print(f'  AbilityDetailsView: {td.method_count} methods, {td.field_count} fields')
        for md in m.methods_of(td)[:6]:
            ps = ', '.join(p.name for p in m.parameters_of(md))
            print(f'    .{md.name}({ps})  token=0x{md.token:08X}')
        for fd in m.fields_of(td)[:6]:
            print(f'    field {fd.name}  token=0x{fd.token:08X}')

    lit_hits = [x for x in m.literals if 'CONNECTED TO SERVER' in x][:1]
    print(f'  literal sample: {lit_hits!r}')

    asm = m.assemblies[0]
    print(f'  assembly[0]: {asm.name} {asm.version}')
    print(f'  image[0]: {m.images[0].name}  typeCount={m.images[0].type_count}')
    names = [a.name for a in m.assemblies]
    print(f'  assemblies: {names[:10]} ...')

    print('\nDONE - all acceptance checks executed.')


if __name__ == '__main__':
    main()
