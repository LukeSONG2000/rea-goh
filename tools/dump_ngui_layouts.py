# -*- coding: utf-8 -*-
"""Dump ORIGINAL serialized NGUI layout data from the freshly cached UI view
prefabs -> catalog/ngui_layouts.json.

SWGoH UI is NGUI: the serialized MonoBehaviours carry the original widget
geometry (per-side anchors + absolute offsets, mWidth/mHeight, mPivot, mDepth,
mColor, sprite names, label text/font size) and the Transforms carry the
original localPosition/localScale/localRotation.  This is the layout truth the
game shipped with -- it supersedes any REFERENCE-MEASURED approximation.

Per bundle we record:
  roots   -- full Transform hierarchy, per node: name/active/layer, transform
             (pos/scale/rot), optional uGUI rect, component class list, NGUI
             widget fields (anchors w/ resolved target GO names, size, pivot,
             depth, color), UISprite atlas+sprite names, UILabel text/font,
             UIGrid/UITable cell params, UIPanel clipping, BoxCollider extents,
             and scalar fields of game scripts (bounded).
  counts  -- GameObject/Transform/MonoBehaviour totals + per-class histogram.
  screen  -- guessed target screen (P2 home / P3 roster / P4 detail / P5
             battle / P6 fleet / other) from bundle + GameObject names.

Usage:
  py tools/dump_ngui_layouts.py [--hours 24] [--limit N] [--out PATH]
                                [--bundles name1,name2,...]
"""
import argparse
import collections
import json
import pathlib
import re
import sys
import time

import UnityPy

ROOT = pathlib.Path(__file__).resolve().parent.parent
CACHE = ROOT / "raw_bundles"
OUT_DEFAULT = ROOT / "catalog" / "ngui_layouts.json"

# UI-ish bundle names worth parsing (the freshly downloaded view/tile family).
UI_NAME_RE = re.compile(
    r"^(ui_|homebase|contextual|contexutal|championship|cantinapatron)"
    r"|(_view$|_tile$|_component$|_atlas$)"
    r"|(unitdetailsui|inventoryui|charactermods|battleui_|prebattleui"
    r"|abilitydetailsui|relic_details|filtersort|pauseui|settingsui"
    r"|objectives_|challengetoast|arena_popup|campaigns_popup|conquestwar_popup"
    r"|option_selection|popupitems|popupsell|popupstore|popupshipments"
    r"|statdetailed|leadersynergy|playerprofileui|commanderselectui"
    r"|quantityselectui|messageui_|energydetailui|currencydetailui"
    r"|consumabledetailui|materialdetailui|equipmentdetailui|artifactdetailui"
    r"|mysteryboxdetailui|playerlevelupui|unitunlockui|characterunlockcelebration"
    r"|characterupgradecelebration|eventmissionui|missiondetailsui|leaderboardui"
    r"|alliesui|guildui|storeui_|storepack|inboxui|chatui|textinputui"
    r"|notificationpromptui|newsletterui|reportpopup|eula|pc_settings|eaconnect"
    r"|accountmanagementui|appratingui|scavenger|fusionui|datacrons"
    r"|battle.*victoryui|battle.*defeatui|territory|conquest_|guildraid"
    r"|coliseum|galacticchallenge|marquee_event|unitguide|seasons|episode_pass"
    r"|lightspeedtoken|loaned_unit|era_|total_era|grindticketui|warcampaign"
    r"|challengereward|matchmakingui|shippvp|championship)",
    re.I,
)

# ---- NGUI component classification -----------------------------------------
WIDGET_BASE_KEYS = ("leftAnchor", "rightAnchor", "bottomAnchor", "topAnchor")

NGUI_CLASSES = {
    # containers / root
    "UIRoot", "UIPanel", "UIWidget", "UIFont", "NGUIAtlas", "UIAtlas",
    # widgets
    "UISprite", "UILabel", "UITexture", "UILocalizedLabel", "BadgingLabel",
    "UILabelButton", "UI2DSprite", "UISlicedSprite", "UITiledSprite",
    "UIFilledSprite", "UISimpleTexture",
    # interaction
    "UIButton", "UIAnchor", "UIStretch", "UIClickThrough",
    "UIDragScrollView", "UIScrollView", "UIScrollBar", "UIDraggableCamera",
    "UICamera",
    # layout
    "UITable", "UIGrid",
    # indicators
    "UIProgressBar", "UISlider", "UIPopupList", "UIToggle", "UICheckbox",
    # NGUI tweens (animation of the above)
    "TweenAlpha", "TweenColor", "TweenPosition", "TweenScale", "TweenWidth",
    "TweenHeight", "TweenRotation", "TweenVolume", "TweenOrthoSize",
    "UIPlayTween", "UITweener",
}

SCREEN_KEYWORDS = {
    "P2_home": re.compile(
        r"(homebase|cantina|contextualmessage|contexutalmessage|chatbubble"
        r"|storepromo|onebuttonview|twobuttonview|pointatview|propinfo"
        r"|cantinapatron)", re.I),
    "P3_roster": re.compile(
        r"(inventory|roster|itembar|item_bar|unit_tile|holotable|squadtile"
        r"|unit_slot|filtersort|unitlist|alliestile)", re.I),
    "P4_detail": re.compile(
        r"(unitdetails|characterdetail|relic|abilitydetails|charactermods"
        r"|skilloverlay|statdetailed|powerup|era_character|unitguide)", re.I),
    "P5_battle": re.compile(
        r"(battleui|prebattle|battlehud|battle.*victory|battle.*defeat"
        r"|encounterprogress|battlesupport)", re.I),
    "P6_fleet": re.compile(r"(shippvp|fleet|ship_tile|shipstats)", re.I),
}

CONTENT_HINT_RE = re.compile(
    r"(itembar|roster|characterdetail|battlehud|topbar|top_bar|rails?|tab"
    r"|leftbuttongrid|energybar|currencybar|hud|hotkey|keyboardbind|allycode"
    r"|skilllist|relic|tags?|filter|sort|grid|card)", re.I)


def newest_data(bundle_dir):
    subs = [s for s in bundle_dir.iterdir() if s.is_dir()]
    if not subs:
        return None
    newest = max(subs, key=lambda s: s.stat().st_mtime)
    f = newest / "__data"
    return f if f.exists() else None


def rv(v, nd=4):
    """Round a float for compact JSON."""
    return round(float(v), nd)


def vec3(t):
    return {"x": rv(t["x"]), "y": rv(t["y"]), "z": rv(t["z"])}


def quat(t):
    return {"x": rv(t["x"]), "y": rv(t["y"]), "z": rv(t["z"]), "w": rv(t["w"])}


def vec4(t):
    return {"x": rv(t["x"]), "y": rv(t["y"]), "z": rv(t.get("z", 0)),
            "w": t.get("w", 0) if not isinstance(t.get("w"), float) else rv(t["w"])}


def color(t):
    return {"r": rv(t["r"]), "g": rv(t["g"]), "b": rv(t["b"]), "a": rv(t["a"])}


def anchor(a, tf_go_name):
    t = a["target"]
    pid = t["m_PathID"]
    return {
        "target": tf_go_name.get(pid),  # GO name of the anchor target transform
        "relative": rv(a["relative"]),
        "absolute": a["absolute"],
    }


def ptr_name(pptr, mono_name, ext_names):
    """Human-readable name for a PPtr to a MonoBehaviour (atlas/font etc)."""
    if not isinstance(pptr, dict):
        return None
    fid, pid = pptr["m_FileID"], pptr["m_PathID"]
    if pid == 0:
        return None
    if fid != 0:
        return "ext:%s:%s" % (ext_names.get(fid, fid), pid)
    return mono_name.get(pid)


def scalar_fields(tt, cap=64):
    """Capture bounded scalar fields of a game-script MonoBehaviour."""
    out = {}
    for k, v in tt.items():
        if k in ("m_GameObject", "m_Script", "m_Enabled", "m_Name", "m_EditorHideFlags",
                 "m_EditorClassIdentifier"):
            continue
        if isinstance(v, bool):
            out[k] = v
        elif isinstance(v, int) and abs(v) < 10 ** 9:
            out[k] = v
        elif isinstance(v, float):
            out[k] = rv(v)
        elif isinstance(v, str):
            if len(v) <= cap:
                out[k] = v
            else:
                out[k] = v[:cap] + "..."
        # dicts (PPtr/colors), lists, everything else: skipped on purpose
    return out


def classify(tt, script_names, ext_names, ngui_counts, game_counts, unresolved):
    """Return (class_name, family) for a MonoBehaviour typetree."""
    sc = tt.get("m_Script") or {}
    fid, pid = sc.get("m_FileID", 0), sc.get("m_PathID", 0)
    if fid == 0 and pid in script_names:
        cn = script_names[pid]
    elif fid != 0:
        cn = "ext:%s:%s" % (ext_names.get(fid, fid), pid)
        unresolved += 1
    else:
        cn = "unresolved:%s" % pid
        unresolved += 1
    if cn in NGUI_CLASSES or (cn.startswith("Tween")):
        family = "ngui"
        ngui_counts[cn] += 1
    elif cn.startswith(("UI", "Ngui")) or any(k in tt for k in WIDGET_BASE_KEYS):
        # game-specific NGUI-derived components (SmartButton, UIPlayerHud, ...)
        family = "game_ngui"
        game_counts[cn] += 1
    else:
        family = "game"
        game_counts[cn] += 1
    return cn, family


def widget_common(tt, tf_go_name, ext_names):
    w = {
        "width": tt.get("mWidth"), "height": tt.get("mHeight"),
        "pivot": tt.get("mPivot"), "depth": tt.get("mDepth"),
        "color": color(tt["mColor"]) if "mColor" in tt else None,
        "aspectRatio": rv(tt["aspectRatio"]) if "aspectRatio" in tt else None,
        "keepAspectRatio": tt.get("keepAspectRatio"),
        "updateAnchors": tt.get("updateAnchors"),
    }
    for side in ("left", "right", "bottom", "top"):
        k = side + "Anchor"
        if k in tt:
            w[k] = anchor(tt[k], tf_go_name)
    return {k: v for k, v in w.items() if v is not None}


def ngui_details(cn, tt, tf_go_name, mono_name, ext_names):
    """Layout-relevant fields per NGUI class."""
    d = widget_common(tt, tf_go_name, ext_names)
    if cn == "UISprite":
        d["sprite"] = tt.get("mSpriteName")
        d["atlas"] = ptr_name(tt.get("mAtlas"), mono_name, ext_names)
        d["type"] = tt.get("mType")
        d["fillDirection"] = tt.get("mFillDirection")
        d["fillAmount"] = rv(tt.get("mFillAmount", 1.0))
        d["flip"] = tt.get("mFlip")
        d["fillCenter"] = tt.get("mFillCenter")
    elif cn in ("UILabel", "UILocalizedLabel", "BadgingLabel", "UILabelButton"):
        d["text"] = tt.get("mText")
        d["fontSize"] = tt.get("mFontSize")
        d["fontStyle"] = tt.get("mFontStyle")
        d["alignment"] = tt.get("mAlignment")
        d["overflow"] = tt.get("mOverflow")
        d["maxLineCount"] = tt.get("mMaxLineCount")
        d["effectStyle"] = tt.get("mEffectStyle")
        ed = tt.get("mEffectDistance")
        if ed:
            d["effectDistance"] = {"x": rv(ed["x"]), "y": rv(ed["y"])}
        d["spacingX"] = tt.get("mSpacingX")
        d["spacingY"] = tt.get("mSpacingY")
        if tt.get("mApplyGradient"):
            d["gradientTop"] = color(tt["mGradientTop"])
            d["gradientBottom"] = color(tt["mGradientBottom"])
        d["font"] = ptr_name(tt.get("mFont"), mono_name, ext_names)
        ttf = tt.get("mTrueTypeFont")
        if ttf and ttf.get("m_PathID"):
            d["trueTypeFont"] = ptr_name(ttf, mono_name, ext_names)
    elif cn == "UITexture":
        d["textureRect"] = tt.get("mRect")
        d["uvRect"] = tt.get("mUVRect") if "mUVRect" in tt else None
    elif cn == "UIPanel":
        d["alpha"] = rv(tt.get("mAlpha", 1.0))
        d["clipping"] = tt.get("mClipping")
        cr = tt.get("mClipRange")
        if cr:
            d["clipRange"] = vec4(cr)
        cs = tt.get("mClipSoftness")
        if cs:
            d["clipSoftness"] = {"x": rv(cs["x"]), "y": rv(cs["y"])}
        d["panelDepth"] = tt.get("mDepth")
        d["startingRenderQueue"] = tt.get("startingRenderQueue")
    elif cn == "UIGrid":
        d.update({k: tt.get(k) for k in
                  ("arrangement", "sorting", "pivot", "maxPerLine",
                   "cellWidth", "cellHeight", "hideInactive", "keepWithinPanel",
                   "animateSmoothly")})
        if isinstance(d.get("cellWidth"), float):
            d["cellWidth"] = rv(d["cellWidth"])
        if isinstance(d.get("cellHeight"), float):
            d["cellHeight"] = rv(d["cellHeight"])
    elif cn == "UITable":
        d.update({k: tt.get(k) for k in
                  ("columns", "direction", "sorting", "pivot", "cellAlignment",
                   "hideInactive", "keepWithinPanel")})
        pad = tt.get("padding")
        if pad:
            d["padding"] = {"x": rv(pad["x"]), "y": rv(pad["y"])}
        io = tt.get("initialOffset")
        if io:
            d["initialOffset"] = {"x": rv(io["x"]), "y": rv(io["y"])}
    elif cn == "UIButton":
        d["tweenTarget"] = tf_go_name.get(
            (tt.get("tweenTarget") or {}).get("m_PathID"))
        for k in ("hover", "pressed", "disabledColor"):
            if k in tt:
                d[k] = color(tt[k])
        for k in ("hoverSprite", "pressedSprite", "disabledSprite"):
            if tt.get(k):
                d[k] = tt[k]
    elif cn in ("UIScrollView",):
        d["movement"] = tt.get("movement")
        d["contentPivot"] = tt.get("contentPivot")
        d["momentumAmount"] = rv(tt.get("momentumAmount", 0.0))
        d["dampenStrength"] = rv(tt.get("dampenStrength", 0.0))
    elif cn in ("UIScrollBar", "UIProgressBar", "UISlider"):
        d["value"] = rv(tt.get("mValue", 0.0))
        d["steps"] = tt.get("numberOfSteps")
        d["fill"] = tt.get("mFill")
        if cn == "UIScrollBar":
            d["dir"] = tt.get("mDir")
            d["size"] = rv(tt.get("mSize", 0.0))
    elif cn == "UIRoot":
        d.update({k: tt.get(k) for k in
                  ("scalingStyle", "manualWidth", "mManualHeight",
                   "minimumHeight", "maximumHeight", "fitWidth", "fitHeight",
                   "wideScreenAspectRatioThreshold")})
    elif cn in ("NGUIAtlas", "UIAtlas"):
        d["atlasName"] = tt.get("m_Name")
        d["pixelSize"] = rv(tt.get("mPixelSize", 1.0))
        d["sprites"] = len(tt.get("mSprites") or [])
        d["replacement"] = ptr_name(tt.get("mReplacement"), mono_name, ext_names)
    elif cn == "UIFont":
        mf = tt.get("mFont") or {}
        d["bitmapSize"] = mf.get("mSize")
        d["dynamicFontSize"] = tt.get("mDynamicFontSize")
        d["dynamicFont"] = ptr_name(tt.get("mDynamicFont"), mono_name, ext_names)
        d["replacement"] = ptr_name(tt.get("mReplacement"), mono_name, ext_names)
    elif cn in ("UIAnchor", "UIStretch"):
        d["side"] = tt.get("side")
        ro = tt.get("relativeOffset")
        if ro:
            d["relativeOffset"] = {"x": rv(ro["x"]), "y": rv(ro["y"])}
        po = tt.get("pixelOffset")
        if po:
            d["pixelOffset"] = {"x": rv(po["x"]), "y": rv(po["y"])}
        if cn == "UIStretch":
            d["style"] = tt.get("style")
            d["sourceRect"] = tt.get("sourceRect")
    # tweens / anything else NGUI: common fields only
    if cn.startswith("Tween"):
        d["tweenFactor"] = rv(tt.get("mFactor", 0.0))
    return d


def parse_bundle(name, data_file):
    env = UnityPy.load(str(data_file))
    objs = {o.path_id: o for o in env.objects}

    # external file names (for ext:... refs)
    ext_names = {}
    try:
        for i, ext in enumerate(env.file.externals):
            p = getattr(ext, "path", None)
            ext_names[i + 1] = str(p) if p else str(i + 1)
    except Exception:
        pass

    # MonoScript class names
    script_names = {}
    for pid, o in objs.items():
        if o.type.name == "MonoScript":
            try:
                script_names[pid] = o.read().m_ClassName
            except Exception:
                pass

    gos, trs, mbs = {}, {}, {}
    for pid, o in objs.items():
        tn = o.type.name
        if tn == "GameObject":
            gos[pid] = o.read()
        elif tn in ("Transform", "RectTransform"):
            trs[pid] = o.read_typetree()
        elif tn == "MonoBehaviour":
            mbs[pid] = o

    ngui_counts = collections.Counter()
    game_counts = collections.Counter()
    unresolved_scripts = 0
    errors = []

    # per-MonoBehaviour classified data
    mb_data = {}
    for pid, o in mbs.items():
        try:
            tt = o.read_typetree()
        except Exception as e:
            errors.append("mb %s typetree: %s" % (pid, e))
            continue
        cn, family = classify(tt, script_names, ext_names,
                              ngui_counts, game_counts, unresolved_scripts)
        mb_data[pid] = (cn, family, tt)

    # transform pid -> GO name (for anchor targets / parents)
    tr_go, go_tr = {}, {}
    for pid, tt in trs.items():
        gpid = tt["m_GameObject"]["m_PathID"]
        tr_go[pid] = gpid
        go_tr[gpid] = pid
    tf_go_name = {}
    for pid, tt in trs.items():
        g = gos.get(tt["m_GameObject"]["m_PathID"])
        if g is not None:
            tf_go_name[pid] = g.m_Name

    # mono pid -> readable name (atlas/font GO or m_Name)
    mono_name = {}
    for pid, (cn, family, tt) in mb_data.items():
        gpid = (tt.get("m_GameObject") or {}).get("m_PathID", 0)
        g = gos.get(gpid) if gpid else None
        nm = tt.get("m_Name") or (g.m_Name if g is not None else None)
        mono_name[pid] = nm or cn

    # box colliders
    colliders = {}
    for pid, o in objs.items():
        if o.type.name == "BoxCollider":
            try:
                tt = o.read_typetree()
                colliders[tt["m_GameObject"]["m_PathID"]] = {
                    "center": vec3(tt["m_Center"]),
                    "size": vec3(tt["m_Size"]),
                    "enabled": bool(tt.get("m_Enabled", 1)),
                }
            except Exception:
                pass

    # build component view per GO
    def go_components(go):
        comps = []
        for pair in go.m_Component:
            try:
                cpid = pair.component.m_PathID
            except Exception:
                cpid = pair["component"]["m_PathID"]
            d = mb_data.get(cpid)
            if d is not None:
                cn, family, tt = d
                comps.append((cpid, cn, family, tt))
                continue
            o = objs.get(cpid)
            comps.append((cpid, o.type.name if o else "missing", "builtin", {}))
        return comps

    # hierarchy walk
    child_of = {pid: tt["m_Father"]["m_PathID"]
                for pid, tt in trs.items() if tt["m_Father"]["m_PathID"] != 0}
    root_trs = [pid for pid in trs if pid not in child_of]

    def build_node(tpid):
        tt = trs[tpid]
        gpid = tt["m_GameObject"]["m_PathID"]
        go = gos.get(gpid)
        if go is None:
            return None
        node = {
            "name": go.m_Name,
            "active": bool(go.m_IsActive),
            "pos": vec3(tt["m_LocalPosition"]),
            "scale": vec3(tt["m_LocalScale"]),
            "rot": quat(tt["m_LocalRotation"]),
            "components": [],
            "children": [],
        }
        if tt.get("_rect"):  # RectTransform extras
            node["rect"] = {
                "anchorsMin": vec3(tt["m_AnchorMin"]),
                "anchorsMax": vec3(tt["m_AnchorMax"]),
                "anchoredPos": vec3(tt["m_AnchoredPosition"]),
                "sizeDelta": vec3(tt["m_SizeDelta"]),
                "pivot": vec3(tt["m_Pivot"]),
            }
        if gpid in colliders:
            node["collider"] = colliders[gpid]
        for cpid, cn, family, ctt in go_components(go):
            node["components"].append(cn)
            if family == "ngui":
                det = ngui_details(cn, ctt, tf_go_name, mono_name, ext_names)
                node.setdefault("ngui", {})[cn] = det
            elif family == "game_ngui" and cn not in node.get("ngui", {}):
                sc = scalar_fields(ctt)
                if sc:
                    node.setdefault("gameScript", {})[cn] = sc
            elif family == "game":
                sc = scalar_fields(ctt)
                if sc:
                    node.setdefault("gameScript", {})[cn] = sc
        for ch in tt["m_Children"]:
            c = build_node(ch["m_PathID"])
            if c:
                node["children"].append(c)
        return node

    roots = []
    for r in sorted(root_trs):
        n = build_node(r)
        if n:
            roots.append(n)

    # content hints from GO names
    hints = set()
    for g in gos.values():
        if CONTENT_HINT_RE.search(g.m_Name):
            hints.add(g.m_Name)

    tr_count = len(trs)
    rect_count = sum(1 for tt in trs.values() if tt.get("_rect"))
    pos_count = sum(1 for tt in trs.values()
                    if any(abs(tt["m_LocalPosition"][k]) > 1e-4
                           for k in ("x", "y", "z")))
    return {
        "bundle": name,
        "data_file": str(data_file),
        "counts": {
            "gameObjects": len(gos),
            "transforms": tr_count,
            "rectTransforms": rect_count,
            "transformsWithLocalPos": pos_count,
            "monoBehaviours": len(mbs),
            "monoscripts": len(script_names),
            "unresolvedScriptRefs": unresolved_scripts,
        },
        "nguiComponents": dict(ngui_counts),
        "gameComponents": dict(game_counts.most_common()),
        "contentHints": sorted(hints)[:40],
        "roots": roots,
        "errors": errors[:10],
    }


def screen_guess(rec, bundle_name):
    for screen, rx in SCREEN_KEYWORDS.items():
        if rx.search(bundle_name):
            return screen
    text = bundle_name + " " + " ".join(rec.get("contentHints", []))
    for screen, rx in SCREEN_KEYWORDS.items():
        if rx.search(text):
            return screen
    return "other"


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--hours", type=float, default=24.0,
                    help="freshness window for the downloaded __data files")
    ap.add_argument("--limit", type=int, default=0, help="process at most N bundles")
    ap.add_argument("--out", default=str(OUT_DEFAULT))
    ap.add_argument("--bundles", default="",
                    help="comma-separated explicit bundle names (overrides regex)")
    ap.add_argument("--quiet", action="store_true")
    args = ap.parse_args()

    now = time.time()
    names = []
    if args.bundles:
        names = [n.strip() for n in args.bundles.split(",") if n.strip()]
    else:
        for d in sorted(CACHE.iterdir()):
            if not d.is_dir() or not UI_NAME_RE.search(d.name):
                continue
            f = newest_data(d)
            if not f:
                continue
            if args.hours > 0 and now - f.stat().st_mtime > args.hours * 3600:
                continue
            names.append(d.name)
    if args.limit:
        names = names[:args.limit]

    out = {"_meta": {}, "bundles": {}, "_summary": {}}
    t0 = time.time()
    ok = fail = 0
    for i, name in enumerate(names, 1):
        f = newest_data(CACHE / name)
        try:
            rec = parse_bundle(name, f)
            rec["screen"] = screen_guess(rec, name)
            out["bundles"][name] = rec
            ok += 1
        except Exception as e:
            out["bundles"][name] = {"bundle": name, "error": str(e),
                                    "data_file": str(f)}
            fail += 1
        if not args.quiet and i % 25 == 0:
            print("  %d/%d  (%.0fs)" % (i, len(names), time.time() - t0),
                  flush=True)

    # ---- summary -----------------------------------------------------------
    per_screen = collections.Counter()
    for name, rec in out["bundles"].items():
        if "error" in rec:
            continue
        per_screen[rec["screen"]] += 1
    ngui_total = collections.Counter()
    for rec in out["bundles"].values():
        for k, v in rec.get("nguiComponents", {}).items():
            ngui_total[k] += v

    out["_meta"] = {
        "generated": time.strftime("%Y-%m-%d %H:%M:%S"),
        "method": "UnityPy read_typetree of NGUI MonoBehaviours in cached view "
                  "prefab bundles; serialized values are the original shipped "
                  "layout (anchors/absolute offsets, mWidth/mHeight, mPivot, "
                  "mDepth, localPosition), NOT measured approximations",
        "freshnessHours": args.hours,
        "designResolutionNote": "UIRoot components carry the NGUI design size "
                                "(typically manualWidth=1280/manualHeight=720, "
                                "fitWidth); NGUI units are pixels in that space",
        "bundleSelection": "name regex: fresh ui_*/homebase*/contextual*/"
                           "contexutal*/championship*/cantinapatron* plus "
                           "*_view/_tile/_component and known screen families",
        "topLevelKeys": {"_meta": "this", "bundles": "per-bundle data keyed by "
                         "bundle name", "_summary": "totals + screen mapping"},
    }
    out["_summary"] = {
        "bundlesProcessed": len(names),
        "bundlesOk": ok,
        "bundlesFailed": fail,
        "screenAssignment": dict(per_screen),
        "nguiComponentTotals": dict(ngui_total.most_common()),
        "screenBundles": {},
    }
    for name, rec in sorted(out["bundles"].items()):
        if "error" in rec:
            continue
        out["_summary"]["screenBundles"].setdefault(rec["screen"], []).append(
            {"bundle": name, "gos": rec["counts"]["gameObjects"],
             "ngui": sum(rec["nguiComponents"].values())})

    dest = pathlib.Path(args.out)
    dest.parent.mkdir(parents=True, exist_ok=True)
    dest.write_text(json.dumps(out, ensure_ascii=False, separators=(",", ":")),
                    encoding="utf-8")
    print("dumped %d/%d bundles (%d failed) -> %s  [%.0fs]"
          % (ok, len(names), fail, dest, time.time() - t0))
    print("screen assignment:", dict(per_screen))
    print("NGUI totals (top):", ngui_total.most_common(12))
    return 0


if __name__ == "__main__":
    sys.exit(main())
