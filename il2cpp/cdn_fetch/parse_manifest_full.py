# Full SWGoH RawAssetManifest parser -> package inventory.
# Writes: il2cpp/cdn_fetch/manifest_packages.json  (all records)
#         il2cpp/cdn_fetch/manifest_stats.txt      (human summary)
#         il2cpp/cdn_fetch/ui_fetch_list.txt       (UI bundle names to download)
#         il2cpp/cdn_fetch/audio_wwpkg_list.txt    (audio banks, list only)
import struct, sys, re, json, os
from collections import Counter

BASE = os.path.dirname(os.path.abspath(__file__))
MANIFEST = os.path.join(BASE, "..", "bootstrap_probe", "manifest_100054_Windows_ETC.data")


def read_varint(b, i):
    val = 0; shift = 0
    while True:
        c = b[i]; i += 1
        val |= (c & 0x7F) << shift
        if not (c & 0x80):
            return val, i
        shift += 7


def parse_fields(b):
    i = 0; out = []
    n = len(b)
    while i < n:
        key, i = read_varint(b, i)
        fn, wt = key >> 3, key & 7
        if wt == 0:
            v, i = read_varint(b, i)
        elif wt == 2:
            ln, i = read_varint(b, i)
            v = b[i:i+ln]; i += ln
        elif wt == 5:
            v = b[i:i+4]; i += 4
        elif wt == 1:
            v = b[i:i+8]; i += 8
        else:
            raise ValueError(f"wt {wt} at {i}")
        out.append((fn, wt, v))
    return out


data = open(MANIFEST, "rb").read()
top = parse_fields(data)
hdr = {}
records = []
for fn, wt, v in top:
    if wt == 0:
        hdr[fn] = v
    elif wt == 2 and fn == 2:
        r = {}
        for f2, w2, v2 in parse_fields(v):
            if w2 == 0:
                r.setdefault(f2, v2)
            elif w2 == 2:
                r.setdefault(f2, []).append(v2)
        records.append(r)

pkgs = []
for r in records:
    name_f = r.get(1, [b""])
    if isinstance(name_f, list):
        name_f = name_f[0] if name_f else b""
    pkgs.append({
        "name": name_f.decode("utf-8", "replace"),
        "version": r.get(2, 0),
        "size": r.get(3, 0),
        "rank": r.get(6, 0),
        "deps": [d.decode("utf-8", "replace") for d in r.get(9, [])],
    })

total_bytes = sum(p["size"] for p in pkgs)
# Manifest names carry NO extension; URL extension comes from PackageType:
# audio_* -> WwiseAudio -> ".wwpkg", everything else -> UnityBundle -> ".bundle"
# (WebBundleAssetInfo..cctor / BOOTSTRAP_INVENTORY.md sec 2.3; live probe verified
#  audio_sharedui.wwpkg). 150 audio_* banks == the 150 .wwpkg count from the probe.
wwpkg = [p for p in pkgs if p["name"].startswith("audio_")]
bundles = [p for p in pkgs if not p["name"].startswith("audio_")]

# --- classification ------------------------------------------------------
UI_PAT = re.compile(r"^(ui_|homebase|contextual|commanderselectui|uiglobals|splash|textinputui)", re.I)
ui_pkgs = [p for p in pkgs if UI_PAT.match(p["name"]) and not p["name"].startswith("audio_")]
font_named = [p for p in pkgs if re.search(r"font", p["name"], re.I)]

with open(os.path.join(BASE, "manifest_packages.json"), "w", encoding="utf-8") as f:
    json.dump({"header": {k: v for k, v in hdr.items()}, "packages": pkgs}, f, indent=1)

lines = []
lines.append(f"header: {hdr}")
lines.append(f"total records: {len(pkgs)}")
lines.append(f"total bytes (sum field 3): {total_bytes:,} ({total_bytes/1e9:.2f} GB)")
lines.append(f"audio_* .wwpkg banks: {len(wwpkg)} totaling {sum(p['size'] for p in wwpkg):,} bytes")
lines.append(f"unity bundles (get .bundle at fetch): {len(bundles)} totaling {sum(p['size'] for p in bundles):,} bytes")
lines.append(f"UI-pattern packages (^(ui_|homebase|contextual|commanderselectui|uiglobals|splash|textinputui)): {len(ui_pkgs)} totaling {sum(p['size'] for p in ui_pkgs):,} bytes")
lines.append(f"packages with 'font' in name: {len(font_named)}: {[p['name'] for p in font_named]}")
lines.append("")

# name prefix census (first token before digits)
pref = Counter()
for p in pkgs:
    n = p["name"].removesuffix(".wwpkg")
    m = re.match(r"^([a-z_]+?[a-z])", n)
    pref[m.group(1) if m else n] += 1
lines.append("== name prefix census (top 60) ==")
for k, c in pref.most_common(60):
    lines.append(f"  {c:6d}  {k}")
lines.append("")

lines.append("== ALL ui-pattern packages (name, size, deps) ==")
for p in sorted(ui_pkgs, key=lambda x: -x["size"]):
    lines.append(f"  {p['size']:>10,}  {p['name']}  deps={','.join(p['deps']) if p['deps'] else '-'}")
lines.append("")

lines.append("== ALL .wwpkg audio banks (list only) ==")
for p in sorted(wwpkg, key=lambda x: -x["size"]):
    lines.append(f"  {p['size']:>10,}  {p['name']}")
lines.append("")

# shared containers (deps of everything)
dep_c = Counter()
for p in pkgs:
    for d in p["deps"]:
        dep_c[d] += 1
lines.append("== top dependencies (shared containers) ==")
for k, c in dep_c.most_common(15):
    lines.append(f"  {c:6d}  {k}")

with open(os.path.join(BASE, "manifest_stats.txt"), "w", encoding="utf-8") as f:
    f.write("\n".join(lines))

# UI fetch list: all non-wwpkg UI-pattern packages
with open(os.path.join(BASE, "ui_fetch_list.txt"), "w", encoding="utf-8") as f:
    for p in sorted(ui_pkgs, key=lambda x: x["name"]):
        f.write(f"{p['name']}\t{p['size']}\n")

with open(os.path.join(BASE, "audio_wwpkg_list.txt"), "w", encoding="utf-8") as f:
    for p in sorted(wwpkg, key=lambda x: x["name"]):
        f.write(f"{p['name']}\t{p['size']}\n")

print("\n".join(lines[:20]))
print(f"\nWROTE: manifest_packages.json ({len(pkgs)} pkgs), manifest_stats.txt, ui_fetch_list.txt, audio_wwpkg_list.txt")
