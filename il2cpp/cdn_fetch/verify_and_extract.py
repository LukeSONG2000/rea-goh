# One-pass UnityPy verification + font extraction over all fetched CDN bundles.
# Outputs:
#   il2cpp/cdn_fetch/bundle_verification.json  per-bundle object/script census
#   catalog/fonts_original/<name>.ttf/.otf     any Font/TextAsset with >=10KB real font data
#   il2cpp/cdn_fetch/font_inventory.json       Font objects + NGUI UIFont prefab census
import json, os, re, struct, sys, hashlib, collections

import UnityPy

BASE = os.path.dirname(os.path.abspath(__file__))
BUNDLES = os.path.join(BASE, "bundles")
FONTS_OUT = os.path.join(BASE, "..", "..", "catalog", "fonts_original")
os.makedirs(FONTS_OUT, exist_ok=True)

NGUI_NAMES = {
    "UIWidget", "UIPanel", "UIRoot", "UISprite", "UILabel", "UIFont", "UIAtlas",
    "NGUIAtlas", "UITexture", "UIButton", "UIInput", "UIScrollView", "UIScrollBar",
    "UIToggle", "UIProgressBar", "UISlider", "UIGrid", "UITable", "UIStretch",
    "UIAnchor", "UICenterOnChild", "UIEventTrigger", "UIPlayTween", "UIWrapContent",
    "UILabelButton", "UILocalizedLabel", "UIWidgetContainer",
}
TTF_MAGICS = (b"\x00\x01\x00\x00", b"OTTO", b"true", b"ttcf")


def md5(b):
    return hashlib.md5(b).hexdigest()


def font_sanity(blob):
    if len(blob) < 10240 or blob[:4] not in TTF_MAGICS:
        return False
    num_tables = struct.unpack(">H", blob[4:6])[0]
    return 4 <= num_tables <= 200


def font_bytes_from_font_obj(f):
    """UnityPy 1.25.x: Font.m_FontData is a list; older: single FontData block."""
    out = []
    fd = getattr(f, "m_FontData", None)
    if isinstance(fd, (bytes, bytearray)):
        out.append(bytes(fd))
    elif isinstance(fd, list):
        for e in fd:
            if isinstance(e, (bytes, bytearray)):
                out.append(bytes(e))
            elif hasattr(e, "m_FontData"):
                b = e.m_FontData
                if isinstance(b, (bytes, bytearray)):
                    out.append(bytes(b))
    return [b for b in out if b]


def parse_mb_name(raw):
    """MonoBehaviour raw: after 28-byte pre-name header, read serialized m_Name."""
    if len(raw) < 32:
        return ""
    n = struct.unpack_from("<i", raw, 28)[0]
    if 0 <= n <= 512 and 32 + n <= len(raw):
        try:
            return raw[32:32 + n].decode("utf-8", "replace")
        except Exception:
            return ""
    return ""


saved_fonts = {}
font_inventory = []
results = {}
files = sorted(os.listdir(BUNDLES))
total = len(files)
for idx, fn in enumerate(files, 1):
    path = os.path.join(BUNDLES, fn)
    pkg = fn.rsplit(".", 1)[0]
    try:
        env = UnityPy.load(path)
    except Exception as e:
        results[pkg] = {"error": str(e)}
        continue

    types = collections.Counter()
    scripts = {}          # pid -> (namespace, class)
    mb_by_script = collections.Counter()
    rec = {
        "file_bytes": os.path.getsize(path),
        "types": {},
        "gos": 0, "transforms": 0, "rect_transforms": 0,
        "ngui_mb": 0, "ugui_mb": 0, "tmp_mb": 0, "other_mb": 0,
        "scripts_ngui": [], "scripts_ugui": [], "scripts_tmp": [],
        "fonts": [], "ngui_uifonts": [],
    }
    font_objs = []
    text_assets = []
    for o in env.objects:
        t = o.type.name
        types[t] += 1
        if t == "GameObject":
            rec["gos"] += 1
        elif t == "Transform":
            rec["transforms"] += 1
        elif t == "RectTransform":
            rec["rect_transforms"] += 1
        elif t == "MonoScript":
            try:
                ms = o.read()
                scripts[o.path_id] = (getattr(ms, "m_Namespace", "") or "", ms.m_ClassName)
            except Exception:
                pass
        elif t == "Font":
            font_objs.append(o)
        elif t == "TextAsset":
            text_assets.append(o)

    for pid, (ns, cls) in scripts.items():
        if ns == "UnityEngine.UI":
            rec["scripts_ugui"].append(cls)
        elif ns == "TMPro" or cls.startswith("TextMeshPro") or cls == "TMP_FontAsset":
            rec["scripts_tmp"].append(cls)
        elif ns == "" and (cls in NGUI_NAMES or (cls.startswith("UI") and cls[2:3].isupper())):
            rec["scripts_ngui"].append(cls)

    for o in env.objects:
        if o.type.name != "MonoBehaviour":
            continue
        raw = o.get_raw_data() or b""
        spid = None
        try:
            mb = o.read()
            spid = mb.m_Script.m_PathID
        except Exception:
            # header-only fallback: m_GameObject(12)+enabled(4)+m_Script(12)
            if len(raw) >= 28:
                fid, pid = struct.unpack_from("<iq", raw, 16)
                if fid == 0:
                    spid = pid
        if spid is None:
            rec["other_mb"] += 1
            continue
        s = scripts.get(spid)
        cls = s[1] if s else "<external-script>"
        mb_by_script[cls] += 1
        if s and s[0] == "UnityEngine.UI":
            rec["ugui_mb"] += 1
        elif s and s[0] == "TMPro":
            rec["tmp_mb"] += 1
        elif s and s[0] == "" and (cls in NGUI_NAMES or (cls.startswith("UI") and cls[2:3].isupper())):
            rec["ngui_mb"] += 1
        else:
            rec["other_mb"] += 1
        if cls == "UIFont":
            gopid = struct.unpack_from("<iq", raw, 0)[1] if len(raw) >= 12 else 0
            rec["ngui_uifonts"].append({
                "path_id": o.path_id,
                "go_name": parse_mb_name(raw) or gopid,
                "raw_len": len(raw),
            })

    rec["types"] = dict(types)
    rec["mb_by_script"] = dict(mb_by_script)
    results[pkg] = rec

    # ---- fonts ----
    for o in font_objs:
        try:
            f = o.read()
        except Exception as e:
            font_inventory.append({"bundle": pkg, "path_id": o.path_id, "error": str(e)})
            continue
        name = getattr(f, "m_Name", "")
        names = list(getattr(f, "m_FontNames", []) or [])
        blobs = font_bytes_from_font_obj(f)
        raw = o.get_raw_data() or b""
        # fallback raw scan inside the Font object only
        if not blobs:
            for magic in TTF_MAGICS:
                i = raw.find(magic)
                if i > 0 and i + 10240 <= len(raw):
                    blobs.append(raw[i:])
                    break
        entry = {
            "bundle": pkg, "path_id": o.path_id, "name": name,
            "font_names": names,
            "font_data_bytes": [len(b) for b in blobs],
            "valid": [font_sanity(b) for b in blobs],
            "rendering_mode": getattr(f, "m_FontRenderingMode", None),
        }
        font_inventory.append(entry)
        for b in blobs:
            if not font_sanity(b):
                continue
            ext = ".otf" if b[:4] == b"OTTO" else ".ttf"
            key = (name or "unnamed", md5(b))
            base_name = (name or "unnamed").replace("/", "_")
            out_name = f"{base_name}_{md5(b)[:8]}{ext}"
            out_path = os.path.join(FONTS_OUT, out_name)
            if key not in saved_fonts and not os.path.exists(out_path):
                with open(out_path, "wb") as fh:
                    fh.write(b)
                saved_fonts[key] = out_name

    for o in text_assets:
        try:
            ta = o.read()
            blob = ta.m_Script
            if isinstance(blob, str):
                blob = blob.encode("utf-8", "surrogateescape")
        except Exception:
            continue
        if blob and font_sanity(blob[:len(blob)]):
            out_name = f"{ta.m_Name}_{md5(blob)[:8]}.ttf"
            out_path = os.path.join(FONTS_OUT, out_name)
            if not os.path.exists(out_path):
                with open(out_path, "wb") as fh:
                    fh.write(blob)
            saved_fonts[(ta.m_Name, md5(blob))] = out_name
    if idx % 100 == 0:
        print(f"[{idx}/{total}] processed; fonts saved so far: {len(saved_fonts)}", flush=True)

with open(os.path.join(BASE, "bundle_verification.json"), "w", encoding="utf-8") as f:
    json.dump(results, f)
with open(os.path.join(BASE, "font_inventory.json"), "w", encoding="utf-8") as f:
    json.dump(font_inventory, f, indent=1)

# aggregate
agg = {
    "bundles_total": len(results),
    "bundles_with_rectTransforms": sum(1 for r in results.values() if r.get("rect_transforms")),
    "bundles_with_ngui_scripts": sum(1 for r in results.values() if r.get("scripts_ngui")),
    "bundles_with_ugui_scripts": sum(1 for r in results.values() if r.get("scripts_ugui")),
    "bundles_with_tmp": sum(1 for r in results.values() if r.get("scripts_tmp")),
    "total_gameobjects": sum(r.get("gos", 0) for r in results.values()),
    "total_transforms": sum(r.get("transforms", 0) for r in results.values()),
    "total_rectTransforms": sum(r.get("rect_transforms", 0) for r in results.values()),
    "total_ngui_mb": sum(r.get("ngui_mb", 0) for r in results.values()),
    "total_ugui_mb": sum(r.get("ugui_mb", 0) for r in results.values()),
    "total_tmp_mb": sum(r.get("tmp_mb", 0) for r in results.values()),
    "font_objects": len([e for e in font_inventory if "name" in e]),
    "fonts_saved": sorted(saved_fonts.values()),
}
with open(os.path.join(BASE, "verification_summary.json"), "w", encoding="utf-8") as f:
    json.dump(agg, f, indent=1)
print(json.dumps(agg, indent=1))
print("DONE")
