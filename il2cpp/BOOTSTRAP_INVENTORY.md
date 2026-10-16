# SWGoH bootstrap CDN — content download system (reconstructed)

Date: 2026-10-16. Target: PC client 0.40.7 (BuildId 4490, changelist 2102664),
`E:\Games\EA Games\SWGoH\GameAssembly.dll` (Unity 2022.3.72f1, IL2CPP, metadata v39).
All hosts below are the public, unauthenticated CDNs every client downloads from.
The RPC/API host (`sw-live.cgswprod.com`) is out of scope and was never contacted;
it appears here only as the literal content of the public `env-list.ini`.

## 1. The two CDN hosts

| host | role | evidence |
|---|---|---|
| `https://swgoh-bootstrap.akamaized.net/` | bootstrap: `env-list.ini` (env -> rpc endpoint map) | string literal `0x18582C458` (Client.ENV_LIST_DOMAIN, dump.cs:116705) |
| `https://eaassets-a.akamaihd.net/assetssw.capitalgames.com/` | content: Unity asset bundles, Wwise audio, manifest | runtime config `assetPath` (captured in `catalog/gamedata/meta.json`); the S3 bucket `assetssw.capitalgames.com` (shipped `ClientInfo.txt` key `AssetS3Bucket`) is fronted by eaassets-a |

## 2. URL construction — decompiled evidence

All RVAs from `il2cpp/dump/cs/il2cpp.cs`; decompiles in
`il2cpp/dump/decompiled/{webbundle_urls,bootstrap_chain,bootstrap_manifest_url,content_download}.c`.
String-literal slots (`DAT_…`) resolved via the `stringLiterals` map in `il2cpp/dump/il2cpp.json`.

### 2.1 bootstrap env-list fetch

`Client.GetS3Endpoint` @ `0x180C84450` (name is legacy) and its inlined twin
`BootStrapLoader.LoadBootStrap` @ `0x1811BC070`:

```
url = "https://swgoh-bootstrap.akamaized.net/"
      + Uri.EscapeDataString(Client._communicationKey /* +0x158 */)
      + "/" + "env-list.ini"
```

`Client.ClientInfo.txt` ships `CommunicationKey=` (empty) and `Environment=PROD`,
so the live URL is `https://swgoh-bootstrap.akamaized.net//env-list.ini`
(double slash) — verified 200. `env-list.ini` content (55 B):

```ini
[Environments]
PROD = https://sw-live.cgswprod.com/rpc
```

`BootStrapLoader.PopulateDomain` @ `0x1811BC700` then reads the selected
environment's ini keys `endpoint` (`0x185774158`) and `contentNamespace`
(`0x185774108`) into `Client._endpoint` (+0x38) / `Client._contentNamespace`
(+0x40). A local `<persistentData>/Bootstrap` ini can override.

### 2.2 content root

`WebBundleAssetInfo` (static `FullAssetPath`, static-fields offset 0x100):

- `WebBundleAssetInfo.SetFullAssetPath(assetPackageRoot, subPath, addlDevS3SubPath)`
  @ `0x180C7DAF0`:
  - `assetPackageRoot.Contains("PROD")` (literal `0x18582CA48`) →
    `FullAssetPath = JoinPaths(assetPackageRoot, subPath)`
  - else (dev) → `JoinPaths(assetPackageRoot, addlDevS3SubPath, subPath)`
- `WebBundleAssetInfo.GetFullAssetPath(rootPath, int assetVersion, Platform, TexFormat, …)`
  @ `0x180C7DBE0` = `JoinPaths(rootPath, version.ToString(), platform.ToString(), texFormat.ToString())`
- runtime values (server config `assetPath` / `assetVersion` / `assetSubpath`,
  captured in `catalog/gamedata/meta.json`):
  - `assetPath` = `https://eaassets-a.akamaihd.net/assetssw.capitalgames.com/PROD/`
  - `assetVersion` = `100054`
  - `assetSubpath` = `100054/Windows/ETC` (Platform segment is the
    runtime-config string `Windows`; TexFormat `ETC` — matches shipped
    `ClientInfo.txt` `TexFormat=ETC`, `AssetMode=WEB_ASSETS`)

### 2.3 manifest and package URLs

- `WebBundleAssetInfo.GetAssetManifestURL()` @ `0x180C7D970` =
  `JoinPaths(FullAssetPath, ASSET_WEB_MANIFEST_FILENAME /* "manifest.data", 0x18582C928 */)`
  (confirmed by caller `WebBundleAssetManifestLoader.LoadManifest` @ `0x18113DCA0`,
  which then hands the URL to `DownloadManifest` @ `0x18113DEE0` → BestHTTP GET)
- `WebBundleAssetInfo.GetPackageURL(name, PackageType)` @ `0x180C7DA30` and
  `WebBundleAssetManifestRecord.GetPackageURL` @ `0x180C81C00` =
  `JoinPaths(FullAssetPath, name + PACKAGE_EXTENSION[type])`,
  where `PACKAGE_EXTENSION = { ".bundle" /* UnityBundle */, ".wwpkg" /* WwiseAudio */ }`
  (`WebBundleAssetInfo..cctor` @ `0x180C7DD70`)

### Final pattern

```
https://eaassets-a.akamaihd.net/assetssw.capitalgames.com/PROD/<assetVersion>/<Platform>/<TexFormat>/manifest.data
https://eaassets-a.akamaihd.net/assetssw.capitalgames.com/PROD/<assetVersion>/<Platform>/<TexFormat>/<pkg>.bundle     # Unity bundles (UI prefabs, fonts, shaders, …)
https://eaassets-a.akamaihd.net/assetssw.capitalgames.com/PROD/<assetVersion>/<Platform>/<TexFormat>/<pkg>.wwpkg      # Wwise audio banks
```

Live example (all values from the runtime config snapshot):
`.../PROD/100054/Windows/ETC/manifest.data`

## 3. Probe results (public CDN, no auth, plain curl)

| URL | code | note |
|---|---|---|
| `swgoh-bootstrap.akamaized.net/env-list.ini` | **200** (55 B) | also `//env-list.ini` → 200 |
| `swgoh-bootstrap.akamaized.net/prod/env-list.ini` | 400 | Akamai path whitelist (only env-list.ini shape allowed) |
| `eaassets-a.akamaihd.net/assetssw.capitalgames.com/PACK_PROBABILITIES/P29DYA/pack_probabilities.pdf` | **200** (278 KB) | control: server-config URL, proves the bucket fronting |
| `eaassets-a.akamaihd.net/assetssw.capitalgames.com/PROD/100054/Windows/ETC/manifest.data` | **200** (760,766 B) | protobuf RawAssetManifest |
| `.../PROD/100054/Windows/ETC/ui_ally_tile.bundle` | **200** (24,543 B) | == manifest size field |
| `.../PROD/100054/Windows/ETC/textinputui_view.bundle` | **200** (26,441 B) | == manifest size field (26441) |
| `.../PROD/100054/Windows/ETC/audio_sharedui.wwpkg` | **200** (3,302,071 B, HEAD) | Wwise bank |
| wrong version/platform/tex guesses (`2102664`, `4490`, `1`, `DXT`, `StandaloneWindows64`, …) | 403 | S3 origin returns 403 for missing keys — blind version guessing does not work; the current `assetVersion` must come from a config snapshot (or the local game config), older versions stay on the CDN |

Manifest (`manifest_100054_Windows_ETC.data`): header `version=100054,
timestamp=1791918396, revision=24462`; **11,604 packages, ~1.39 GB total**,
150 `audio_*` `.wwpkg` banks, no font-specific or localization packages.

## 4. What became fetchable

- **UI prefabs** — yes. Fetched + UnityPy-verified:
  `il2cpp/bootstrap_probe/ui_ally_tile.bundle` (314 objects: GameObjects
  "Pending Label", "Four Star Rarity", "Pip (12)", UIWidget/UIPanel MonoScripts…).
- **Fonts** — yes, embedded in UI view bundles (no dedicated package):
  `il2cpp/bootstrap_probe/textinputui_view.bundle` (272 objects) contains
  `futura_bold` — the game's UI font asset.
- **Audio** — yes, 150 `.wwpkg` Wwise banks (e.g. `audio_sharedui.wwpkg`).
- **Gamedata** — the *asset* manifest is fetched; the actual gamedata content
  is segmented content whose per-segment URLs come from the RPC layer
  (`MetadataService.GetSegmentedContentDetails`, `latestSegmentedContentVersion`),
  which is out of bounds. Localization bundles are likewise not in this
  manifest (separate localization loader).

## 5. Acquisition recipe

1. Get the current `<assetVersion>` (int) + `<Platform>` (`Windows` on PC) +
   `<TexFormat>` (`ETC` on PC per shipped ClientInfo; runtime config confirms).
   These come from the server `assetPath/assetVersion/assetSubpath` config —
   on this machine a snapshot already exists:
   `catalog/gamedata/meta.json` (`assetVersion=100054`, `assetSubpath=100054/Windows/ETC`).
   Old versions remain downloadable; wrong ones return 403 (indistinguishable
   from missing), so don't guess — use a snapshot or bump from a known good value.
2. `GET https://eaassets-a.akamaihd.net/assetssw.capitalgames.com/PROD/<assetVersion>/<Platform>/<TexFormat>/manifest.data`
   → protobuf (`RawAssetManifest`). Parser: `il2cpp/bootstrap_probe/parse_manifest.py`.
   Record fields: `1`=name, `2`=version(ulong), `3`=size, `6`=rank,
   `9`=repeated dependency names (e.g. `shared_resourcecontainer`,
   `shared_shadercontainer`, `shared_uicontainer`).
3. For each needed package (plus its dependencies):
   `GET .../PROD/<assetVersion>/<Platform>/<TexFormat>/<name>.bundle`
   (Unity) or `.wwpkg` (Wwise). Size check: HTTP body length == record field 3.
4. Open Unity packages with UnityPy directly (uncompressed LZ4/LZHC bundles;
   no encryption — verified).
5. Bootstrap env discovery (only needed to re-derive the RPC endpoint, not for
   content): `GET https://swgoh-bootstrap.akamaized.net/env-list.ini`.

## 6. Deliverables

- `il2cpp/bootstrap_probe/manifest_100054_Windows_ETC.data` — live asset manifest (11,604 pkgs)
- `il2cpp/bootstrap_probe/ui_ally_tile.bundle` — UI prefab original (UnityPy-verified)
- `il2cpp/bootstrap_probe/textinputui_view.bundle` — font carrier original (`futura_bold`)
- `il2cpp/bootstrap_probe/env-list.ini` — bootstrap env map
- `il2cpp/bootstrap_probe/parse_manifest.py` — manifest parser
- `il2cpp/dump/decompiled/webbundle_urls.c` — URL builders decompiled
- `il2cpp/dump/decompiled/bootstrap_chain.c`, `bootstrap_manifest_url.c`, `content_download.c`
- `il2cpp/dump/xref_url_builders.py`, `scan_callers.py`, `ghidra_manifest_url.py` — tooling
