# -*- coding: utf-8 -*-
"""Extract the game's UI assets (portraits, atlases, ability icons, fonts,
HUD) from the raw bundle cache into extracted_ui/ + index.json."""
import json
import os
import pathlib
import sys

import UnityPy

ROOT = pathlib.Path(__file__).resolve().parent.parent
CACHE = ROOT / "raw_bundles"
OUT = ROOT / "extracted_ui"


def newest_data(name):
    d = CACHE / name
    if not d.is_dir():
        return None
    sub = sorted((s for s in d.iterdir() if s.is_dir()),
                 key=lambda s: s.stat().st_mtime)
    if not sub:
        return None
    f = sub[-1] / "__data"
    return f if f.exists() else None


def save_textures(bundle_name, dest_dir, max_tex=None):
    """Extract every decodable Texture2D from one bundle. Returns (saved, fails)."""
    f = newest_data(bundle_name)
    if not f:
        return 0, 0
    dest_dir.mkdir(parents=True, exist_ok=True)
    saved = fails = 0
    try:
        env = UnityPy.load(str(f))
    except Exception:
        return 0, 1
    n = 0
    for obj in env.objects:
        if obj.type.name != "Texture2D":
            continue
        n += 1
        if max_tex and n > max_tex:
            break
        try:
            tex = obj.read()
            img = tex.image
            name = "".join(c if c not in '\\/:*?"<>|' else "_"
                           for c in (tex.m_Name or bundle_name))
            img.save(dest_dir / f"{name}.png")
            saved += 1
        except Exception:
            fails += 1
    return saved, fails


def main():
    stats = {}
    index = {"portraits": {}, "atlases": [], "ability_icons": {}, "fonts": [], "misc": []}

    # 1. charui portraits
    dirs = [d.name for d in CACHE.iterdir() if d.is_dir() and d.name.startswith("charui")]
    total_fail = 0
    for b in sorted(dirs):
        s, f = save_textures(b, OUT / "charui")
        total_fail += f
        if s:
            index["portraits"][b] = f"charui/{b}.png"  # tex name usually == bundle name
    stats["charui"] = {"bundles": len(dirs), "decode_failures": total_fail}

    # 2. atlases
    for b in sorted(d.name for d in CACHE.iterdir()
                    if d.is_dir() and (d.name.endswith("_atlas") or "atlas" in d.name.lower())):
        s, f = save_textures(b, OUT / "atlas")
        if s:
            index["atlases"].append(b)
    stats["atlases"] = len(index["atlases"])

    # 3. fonts (Font objects carry m_FontData = TTF/OTF bytes)
    fonts_out = OUT / "fonts"
    fonts_out.mkdir(parents=True, exist_ok=True)
    for d in sorted(CACHE.iterdir()):
        if not d.is_dir() or not d.name.startswith("ui_"):
            continue
        f = newest_data(d.name)
        if not f:
            continue
        try:
            for obj in UnityPy.load(str(f)).objects:
                if obj.type.name != "Font":
                    continue
                font = obj.read()
                data = getattr(font, "m_FontData", None)
                if data:
                    fn = (font.m_Name or d.name).replace(" ", "_")
                    ext = ".otf" if bytes(data[:4]) == b"OTTO" else ".ttf"
                    (fonts_out / f"{fn}{ext}").write_bytes(bytes(data))
                    index["fonts"].append(fn + ext)
        except Exception:
            pass
    stats["fonts"] = len(index["fonts"])

    # 4. misc (hud, portraits, badges)
    for b in ("hud_player_info", "hud_counter_component",
              "battle_unit_keyboard_binding_badge"):
        s, f = save_textures(b, OUT / "misc")
        if s:
            index["misc"].append(b)
    for d in sorted(CACHE.iterdir()):
        if d.is_dir() and d.name.startswith("commanderportrait"):
            save_textures(d.name, OUT / "misc")
    stats["misc"] = len(list((OUT / "misc").glob("*.png"))) if (OUT / "misc").exists() else 0

    # 5. ability icons -- sample 400 to keep runtime sane (skip logic documented)
    ability_dirs = sorted(d.name for d in CACHE.iterdir()
                          if d.is_dir() and d.name.startswith("ability_"))
    sampled = ability_dirs[:400]
    a_fail = 0
    for b in sampled:
        s, f = save_textures(b, OUT / "ability")
        a_fail += f
    (OUT / "ability" / "_SAMPLE_NOTE.txt").write_text(
        f"sampled {len(sampled)} of {len(ability_dirs)} ability bundles "
        f"(decode failures: {a_fail}); full set = rerun with larger sample.\n",
        encoding="utf-8")
    stats["ability"] = {"sampled": len(sampled), "total_bundles": len(ability_dirs),
                        "decode_failures": a_fail}

    (OUT / "index.json").write_text(
        json.dumps({"stats": stats, **index}, ensure_ascii=False, indent=1),
        encoding="utf-8")
    print("STATS:", json.dumps(stats))


if __name__ == "__main__":
    sys.exit(main())
