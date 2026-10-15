# Harvest ORIGINAL UI sprite assets from the raw cache bundles.
#
# SWGoH UI uses NGUI: atlases are serialized as a Material + Texture2D plus a
# MonoBehaviour (UIAtlas) whose mSprites[] array carries every sprite's exact
# rect (x/y = TOP-LEFT origin, verified by visual crop probe).  Newer Unity
# Sprite objects also exist in some bundles and are extracted via sprite.image.
#
# Outputs
#   extracted_ui/atlas_sprites/<bundle>/<sprite>.png   one PNG per sprite rect
#   extracted_ui/atlas_icons/<texname>.png             tex.ability*/tex.abilityui* icons
#   extracted_ui/atlas_sprites/index.json              name -> {atlas, rect, border, pathID, file}
#
# Scope: the 6 atlas bundles + every bundle whose name contains 'ui' (except
# charui_* portraits, already extracted) + ability_*/abilityui_* (skill icons).
import glob
import json
import os
import re
import sys
import time

import UnityPy
from PIL import Image

ROOT = r"F:\PPfavorite\Work\goh"
RAW = os.path.join(ROOT, "raw_bundles")
OUT = os.path.join(ROOT, "extracted_ui", "atlas_sprites")
ICONS = os.path.join(ROOT, "extracted_ui", "atlas_icons")

ATLAS_BUNDLES = [
    "championship_atlas",
    "conquestui_atlas",
    "contextual_atlas",
    "territoryui_view_rgba_atlas",
    "territoryui_view_standard_atlas",
    "unit_details_atlas",
]


def scope_bundles():
    names = sorted(os.listdir(RAW))
    scan = set(ATLAS_BUNDLES)
    icons = set()
    for n in names:
        if n.startswith("charui_"):
            continue
        low = n.lower()
        if "ui" in low or "inventory" in low or "collection" in low or "atlas" in low:
            scan.add(n)
        if low.startswith("ability") or low.startswith("abilityui"):
            icons.add(n)
    return sorted(scan), sorted(icons)


def bundle_files(name):
    return sorted(glob.glob(os.path.join(RAW, name, "*", "__data")))


def texture_of_material(env, objs, mat_ptr):
    pid = mat_ptr.get("m_PathID", 0) if isinstance(mat_ptr, dict) else 0
    if not pid:
        return None
    mo = objs.get(pid)
    if mo is None or mo.type.name != "Material":
        return None
    try:
        mt = mo.read_typetree()
    except Exception:
        return None
    for _, texenv in mt.get("m_SavedProperties", {}).get("m_TexEnvs", []):
        tex_ptr = texenv.get("m_Texture")
        if isinstance(tex_ptr, dict) and tex_ptr.get("m_PathID"):
            to = objs.get(tex_ptr["m_PathID"])
            if to is not None and to.type.name == "Texture2D":
                try:
                    return to.read().image
                except Exception:
                    return None
    return None


def main():
    os.makedirs(OUT, exist_ok=True)
    os.makedirs(ICONS, exist_ok=True)
    scan, icon_bundles = scope_bundles()
    print(f"[harvest] scan={len(scan)} bundles, icon bundles={len(icon_bundles)}")
    index = {"sprites": {}, "icons": {}, "stats": {}}
    stats = {"bundles_scanned": 0, "ngui_atlases": 0, "ngui_sprites": 0,
             "unity_sprites": 0, "icons": 0, "decode_failures": 0, "dupe_skips": 0}
    t0 = time.time()

    def process(bundle, want_sprites=True, want_icons=False):
        files = bundle_files(bundle)
        if not files:
            return
        try:
            env = UnityPy.load(*files)
        except Exception as e:
            stats["decode_failures"] += 1
            print(f"[harvest] load fail {bundle}: {e}")
            return
        stats["bundles_scanned"] += 1
        objs = {o.path_id: o for o in env.objects}
        # NGUI atlases
        if want_sprites:
            for o in env.objects:
                if o.type.name != "MonoBehaviour":
                    continue
                try:
                    tree = o.read_typetree()
                except Exception:
                    continue
                if not isinstance(tree, dict) or "mSprites" not in tree:
                    continue
                atlas_name = tree.get("m_Name") or bundle
                tex = texture_of_material(env, objs, tree.get("material"))
                if tex is None:
                    # fallback: first Texture2D in env
                    for cand in objs.values():
                        if cand.type.name == "Texture2D":
                            try:
                                tex = cand.read().image
                                break
                            except Exception:
                                pass
                if tex is None:
                    stats["decode_failures"] += 1
                    continue
                stats["ngui_atlases"] += 1
                for sp in tree["mSprites"]:
                    name = sp.get("name")
                    x, y = int(sp.get("x", 0)), int(sp.get("y", 0))
                    w, h = int(sp.get("width", 0)), int(sp.get("height", 0))
                    if not name or w <= 0 or h <= 0:
                        continue
                    key = f"{bundle}/{name}"
                    if key in index["sprites"]:
                        stats["dupe_skips"] += 1
                        continue
                    try:
                        crop = tex.crop((x, y, x + w, y + h))
                    except Exception:
                        stats["decode_failures"] += 1
                        continue
                    d = os.path.join(OUT, bundle)
                    os.makedirs(d, exist_ok=True)
                    fp = os.path.join(d, name + ".png")
                    crop.save(fp)
                    index["sprites"][key] = {
                        "sprite": name, "atlas": bundle,
                        "rect": [x, y, w, h],
                        "border": [sp.get("borderLeft", 0), sp.get("borderRight", 0),
                                   sp.get("borderTop", 0), sp.get("borderBottom", 0)],
                        "pathID": o.path_id,
                        "file": os.path.relpath(fp, ROOT).replace("\\", "/"),
                    }
                    stats["ngui_sprites"] += 1
        # Unity Sprite objects
        if want_sprites:
            for o in env.objects:
                if o.type.name != "Sprite":
                    continue
                try:
                    rd = o.read()
                    name = rd.m_Name
                    rect = rd.m_Rect
                    img = rd.image
                except Exception:
                    stats["decode_failures"] += 1
                    continue
                if not name:
                    continue
                key = f"{bundle}/{name}"
                if key in index["sprites"]:
                    stats["dupe_skips"] += 1
                    continue
                d = os.path.join(OUT, bundle)
                os.makedirs(d, exist_ok=True)
                fp = os.path.join(d, name + ".png")
                try:
                    img.save(fp)
                except Exception:
                    stats["decode_failures"] += 1
                    continue
                index["sprites"][key] = {
                    "sprite": name, "atlas": bundle,
                    "rect": [rect.x, rect.y, rect.width, rect.height],
                    "border": [0, 0, 0, 0],
                    "pathID": o.path_id,
                    "file": os.path.relpath(fp, ROOT).replace("\\", "/"),
                }
                stats["unity_sprites"] += 1
        # ability icons: Texture2D named tex.ability*
        if want_icons:
            for o in env.objects:
                if o.type.name != "Texture2D":
                    continue
                try:
                    rd = o.read()
                except Exception:
                    continue
                nm = rd.m_Name or ""
                if not (nm.startswith("tex.ability") or nm.startswith("tex.abilityui")):
                    continue
                if nm in index["icons"]:
                    continue
                try:
                    img = rd.image
                    img.save(os.path.join(ICONS, nm + ".png"))
                except Exception:
                    stats["decode_failures"] += 1
                    continue
                index["icons"][nm] = {
                    "bundle": bundle,
                    "size": [rd.m_Width, rd.m_Height],
                    "pathID": o.path_id,
                }
                stats["icons"] += 1

    for i, b in enumerate(scan):
        process(b, want_sprites=True, want_icons=False)
        if i % 50 == 0:
            print(f"[harvest] {i}/{len(scan)} scanned, {stats['ngui_sprites']} ngui sprites, "
                  f"{stats['unity_sprites']} unity sprites, {time.time()-t0:.0f}s")
    for i, b in enumerate(icon_bundles):
        process(b, want_sprites=False, want_icons=True)
        if i % 200 == 0:
            print(f"[harvest] icons {i}/{len(icon_bundles)}, {stats['icons']} icons, {time.time()-t0:.0f}s")

    index["stats"] = stats
    with open(os.path.join(OUT, "index.json"), "w", encoding="utf-8") as f:
        json.dump(index, f, ensure_ascii=False, indent=1)
    print(f"[harvest] DONE {stats} in {time.time()-t0:.0f}s")


if __name__ == "__main__":
    main()
