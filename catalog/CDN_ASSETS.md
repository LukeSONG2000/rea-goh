# SWGoH CDN asset acquisition — manifest, UI prefab corpus, original fonts

Date: 2026-10-16. Source: the public content CDN only
(`https://eaassets-a.akamaihd.net/assetssw.capitalgames.com/PROD/100054/Windows/ETC/`,
per `il2cpp/BOOTSTRAP_INVENTORY.md`). No auth, no RPC/API hosts touched.
Fetch tooling + inventory in `il2cpp/cdn_fetch/` (this document summarizes it).

## 1. Manifest (`manifest.data`, protobuf RawAssetManifest)

Live file: `il2cpp/bootstrap_probe/manifest_100054_Windows_ETC.data` (760,766 B).
Parser: `il2cpp/cdn_fetch/parse_manifest_full.py` ->
`manifest_packages.json` (all 11,604 records: name, version, size, rank, deps),
`manifest_stats.txt`, `ui_fetch_list.txt`, `audio_wwpkg_list.txt`.

| field | value |
|---|---|
| header | version=100054, timestamp=1791918396, revision=24462 |
| total packages | **11,604** |
| total size (sum of record field 3) | **1,460,991,375 B (1.46 GB)** |
| `audio_*` banks (fetched as `.wwpkg`) | **150** totaling 152,817,003 B |
| Unity bundles (fetched as `.bundle`) | 11,454 totaling 1,308,174,372 B |
| packages with "font" in the name | **0** (fonts are embedded in view bundles — see §3) |

Manifest names carry no extension; the URL extension comes from PackageType
(`audio_*` -> `.wwpkg`, everything else -> `.bundle`). Largest packages:
character/ability/particle bundles; largest UI bundle is
`ui_galacticbundlecelebration_view` (302,788 B).

## 2. UI prefab corpus — fetched (task 2)

Fetch set = every non-audio package matching the UI patterns
(`ui_*`, `homebase*`, `contextual*`, `commanderselectui`, `uiglobals`,
`splash`, `textinputui`, any `ui_` substring — which adds `inventoryui_*`,
`unitdetailsui_*`, `battleui_*`, `guildui_*`, `alliesui_*`, `abilityui_*`,
`jg_charui_*`, … — plus `*_view`) **plus the transitive dependency closure**
(shared_resourcecontainer / shared_shadercontainer / shared_uicontainer,
icon/pack atlases, etc.).

| metric | value |
|---|---|
| packages in fetch set | **1,911** (list: `il2cpp/cdn_fetch/fetch_list_full.txt`) |
| bytes on disk (all size-verified == manifest field 3) | **63,093,286 B (63.1 MB)** |
| fetch result | 1,909 downloaded + 2 pre-seeded probes, **0 failures** (one TLS reset, retried once per the 5x30s loop) |
| location | `il2cpp/cdn_fetch/bundles/<name>.bundle` |

Tools: `il2cpp/cdn_fetch/fetch_ui_bundles.py` (curl, sequential, retry 5x30s,
403 = stop item — never triggered), `verify_and_extract.py` (UnityPy census ->
`bundle_verification.json`, `verification_summary.json`).

## 3. Fonts — original extraction (task 3)

**Result: 3 real font binaries extracted to `catalog/fonts_original/`** —
validated sfnt structure (table directory + `head`), family names read from the
`name` table:

| file | bytes | PS name / family | carrier bundle |
|---|---|---|---|
| `FuturaCom-Bold_68c23ce1.ttf` | 124,004 | FuturaCom-Bold / "Futura Com Book" (Bold) | `shared_resourcecontainer` |
| `TradeGothicLTCom-BdCn20_f150e600.ttf` | 143,024 | TradeGothicLTCom-BdCn20 / "Trade Gothic LT Com Bold Condensed No. 20" | `shared_resourcecontainer` |
| `ShareTechMono-Regular_9b2f3858.ttf` | 42,754 | ShareTechMono-Regular / "Share Tech Mono" | `settingsui_view` |

`FuturaCom-Bold` is the game's `futura_bold` (the NGUI UIFont prefab name);
it ships as a complete TTF inside `shared_resourcecontainer`, **not** inside
`textinputui_view` — the view only embeds the UIFont prefab.

Font-object census across all 1,911 bundles: **68 Font objects** — 65 are the
built-in `EmptyFont` stub (0 bytes of font data; fallback list `Roboto,
DroidSansFallback, San Francisco, Helvetica Neue, Arial`), 3 carry the real
TTFs above.

NGUI font-prefab structure (decodable even though MonoBehaviours are
typetree-stripped; offsets derived from `il2cpp/dump/cs/il2cpp.cs`): every view
embeds the standard 6-UIFont set (`standard_header_font_ref`,
`standard_system_font_ref`, `standard_body_font_ref`, `futura_bold`,
`trade_gothic`, `empty_font`) — 1,501 UIFont components in 481 bundles. All are
dynamic-mode; the `standard_*_font_ref` prefabs delegate via
`UIFont.mReplacement` to a base UIFont whose `mDynamicFont` points at the
`EmptyFont` Unity Font stub (e.g. `empty_font`: dynamicFont size 36, style
Normal). `empty_font`-backed text therefore renders via OS font fallback at
runtime; only the three TTFs above are real embedded faces.

Extraction note: UnityPy 1.25.4 misparses the modern Font typetree
(`m_FontData` surfaces as an int list), so the TTFs were recovered by scanning
the Font objects' raw serialized bytes for sfnt magics and validating the table
directory, then trimming to true size (`il2cpp/cdn_fetch/extract_fonts2.py`,
inventory in `font_extract_inventory.json`).

## 4. UGUI vs NGUI — the verdict (task 2 verification)

The shipped game's UI is **NGUI**, not uGUI. Numbers across the whole fetched
corpus (1,911 bundles):

| measure | count |
|---|---|
| GameObjects | 92,159 |
| `Transform` components | **92,152** |
| `RectTransform` components | **7** (0.0076%), in 5 bundles |
| NGUI MonoBehaviours (UIWidget/UIPanel/UISprite/…) | **96,568** |
| UnityEngine.UI (uGUI) MonoBehaviours | **0** |
| TextMeshPro | **0** |
| bundles with any UI prefab | 505 (the rest are atlases, icons, particles) |

The 7 RectTransforms are isolated nodes in newer content, not a UI system:
5x GO "SpecialMission" (`territorybattleui_stats_view` x2, `..._phase_listitem`,
`..._stats_phase_subview`, `..._stats_zone_subview`), "Scrim" + "Disabled
Target Needed To Function" (`datacrons_upgradepresentation_view`) — each of
those bundles is still 90%+ NGUI widgets.

Top MonoBehaviour classes in the corpus: UISprite 41,562 · UILocalizedLabel
18,136 · UIWidget 12,196 · UICollapseInactive 5,949 · UIButton 3,591 ·
UITexture 3,004 · UIFont 1,501 · UIPanel 1,049 · NGUIAtlas 1,032.

Consequence: the shipped UI rebuild reference is the NGUI widget tree
(Transform hierarchy + UIWidget width/height/depth/anchors), not
RectTransform/anchorsMin/anchorsMax — replacing REFERENCE-MEASURED with
ORIGINAL-EXTRACTED below.

## 5. Original UI layout dumps (task 4)

`catalog/ui_prefab_layouts/<pkg>.json` — one file per screen, full hierarchy:
per node name/active/layer/localPos/localRot/localScale + component classes,
and for NGUI components (decoded from the typetree-stripped MonoBehaviours at
offsets derived from the il2cpp dump; parser `il2cpp/cdn_fetch/dump_prefab_layouts.py`):

- UIWidget/UILabel/UISprite/UITexture/UIStretchChildren: width, height, depth,
  color, and the 4 NGUI anchors (relative, absolute, target PPtr)
- UILabel/UILocalizedLabel: text, fontSize, alignment, overflow
- UISprite: spriteName
- UIPanel: alpha, depth, clipRange, sortingOrder
- UIFont: mode (dynamic/bitmap), dynamicFont PPtr, fontSize, replacement chain

| file | nodes | nodes with NGUI data | label texts | sprite names |
|---|---|---|---|---|
| `homebaseui_view.json` | 875 | 753 | 212 | 402 |
| `homebaseui_v2_view.json` | 866 | 737 | 212 | 386 |
| `inventoryui_masterinventoryview.json` | 2,231 | 1,875 | 396 | 1,189 |
| `unitdetailsui_characterview.json` | 658 | 531 | 145 | 284 |
| `battleui_view.json` | 1,094 | 869 | 180 | 475 |
| `textinputui_view.json` | 65 | 57 | 13 | 22 |

Example (textinputui_view): `ui.textinputui_view -> UI Root (UIPanel) ->
Window 760x400 d1 -> TopBar 760x72 d3 (sprite `panel_button_title`) ->
TitleLabel 728x40 d4 …`, `standard_close_button` 72x72 with `X icon` 72x72.

Remaining acquisition list:
- **Audio (list only, not fetched)**: 150 `audio_*` `.wwpkg` Wwise banks,
  152,817,003 B total — full name+size list in
  `il2cpp/cdn_fetch/audio_wwpkg_list.txt`; URL shape
  `.../PROD/100054/Windows/ETC/<name>.wwpkg` (HEAD-verified with
  `audio_sharedui.wwpkg` in the bootstrap probe). Largest:
  `audio_sharedvfx` 24.2 MB, `audio_env_shared` 10.6 MB, `audio_env_coliseum`
  7.7 MB.
- The other ~9.5k manifest packages (1.31 GB: `ch_*` character prefabs,
  `ab_*` abilities, `pa_*` particles, `ev_*` events, battle encounter layouts,
  …) remain on the CDN and can be fetched with the same recipe as needed.
