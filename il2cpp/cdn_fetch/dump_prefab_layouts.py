# Dump original UI layout truth for the main screens from CDN prefab bundles.
# The shipped prefabs are NGUI (Transform + UIWidget-family MonoBehaviours,
# typetrees stripped). This walks the Transform hierarchy and decodes each
# MonoBehaviour's serialized fields at offsets derived from the il2cpp dump
# (il2cpp/dump/cs/il2cpp.cs: UIRect/UIWidget/UILabel/UISprite/UIPanel/UIFont),
# plus any RectTransform data if a bundle ever has one.
#
# MonoBehaviour serialized layout (empty m_Name => data starts at 32):
#   m_GameObject PPtr(12) m_Enabled u8+align(4) m_Script PPtr(12) m_Name str
#   UIRect      : leftAnchor(20) rightAnchor(20) bottomAnchor(20) topAnchor(20)
#                 updateAnchors i32(4)                        -> 84 bytes
#   UIWidget    : mColor 16f, mPivot i32, mWidth i32, mHeight i32, mDepth i32,
#                 mMat PPtr(12), 2 bools+align, keepAspectRatio i32,
#                 aspectRatio f32                             -> 56 bytes
#   UILabel     : keepCrisp i32, mTrueTypeFont PPtr, mFont PPtr, mText str,
#                 mFontSize i32, mFontStyle i32, mAlignment i32, ...
#   UISprite    : mAtlas PPtr(12), mSpriteName str, 2 bools
#   UIPanel     : bools, shadowMode i32, bools, renderQueue i32,
#                 startingRenderQueue i32, mClipTexture PPtr(12), mAlpha f32,
#                 mClipping i32, mClipRange vec4, mClipSoftness vec2,
#                 mDepth i32, mSortingOrder i32, mSortingLayerName str
#   AnchorPoint : target PPtr(12), relative f32, absolute i32           (20)
import json, os, struct, sys

import UnityPy

BASE = os.path.dirname(os.path.abspath(__file__))
BUNDLES = os.path.join(BASE, "bundles")
OUT = os.path.join(BASE, "..", "..", "catalog", "ui_prefab_layouts")
os.makedirs(OUT, exist_ok=True)

TARGETS = [
    "homebaseui_view", "homebaseui_v2_view",
    "inventoryui_masterinventoryview",
    "unitdetailsui_characterview",
    "battleui_view",
    "textinputui_view",
]

# classes parsed with the UIWidget base layout (subclass fields simply trail)
WIDGET_CLASSES = {"UIWidget", "UIStretchChildren"}
# UILabel subclasses: base UILabel fields (mText/mFontSize/...) come first
LABEL_CLASSES = {"UILabel", "UILocalizedLabel"}
SPRITE_CLASSES = {"UISprite"}


def r(v, nd=3):
    return round(v, nd)


def u32(b, o):
    return struct.unpack_from("<i", b, o)[0]


def f32(b, o):
    return struct.unpack_from("<f", b, o)[0]


def read_str(b, o):
    """serialized string at o: i32 length + utf8 + align4; returns (str, next_off) or (None,o)."""
    if o + 4 > len(b):
        return None, o
    n = u32(b, o)
    if n < 0 or n > 8192 or o + 4 + n > len(b):
        return None, o
    try:
        s = b[o + 4:o + 4 + n].decode("utf-8")
    except UnicodeDecodeError:
        return None, o
    if s and any(ord(c) < 9 or (11 <= ord(c) < 32) for c in s):
        return None, o
    return s, o + 4 + ((n + 3) & ~3)


def mb_header(raw):
    """returns (go_path_id, script_path_id, data_start) or None."""
    if len(raw) < 32:
        return None
    go = struct.unpack_from("<iq", raw, 0)
    fid, spid = struct.unpack_from("<iq", raw, 16)
    name, off = read_str(raw, 28)
    return go[1], spid, off


def parse_widget(raw, ds, cls):
    d = {}
    try:
        cs = ds + 84  # UIWidget fields after UIRect
        if cs + 56 > len(raw):
            return d
        col = [f32(raw, cs + i * 4) for i in range(4)]
        if all(0.0 <= c <= 1.001 for c in col):
            d["color"] = [r(c, 4) for c in col]
        w, h, dep = u32(raw, cs + 20), u32(raw, cs + 24), u32(raw, cs + 28)
        if 0 <= w <= 8192 and 0 <= h <= 8192 and -100000 <= dep <= 100000:
            d.update(width=w, height=h, depth=dep)
        else:
            return d
        # anchors (UIRect, at ds): target PPtr + relative f32 + absolute i32
        anch = {}
        for i, an in enumerate(("leftAnchor", "rightAnchor", "bottomAnchor", "topAnchor")):
            o = ds + i * 20
            rel, ab = f32(raw, o + 12), u32(raw, o + 16)
            tgt_fid, tgt_pid = struct.unpack_from("<iq", raw, o)
            ok = (abs(rel) <= 1.001) and (-8192 <= ab <= 8192)
            if ok:
                anch[an] = {"relative": r(rel, 4), "absolute": ab,
                            "target": None if tgt_pid == 0 else tgt_pid}
        if anch:
            d["anchors"] = anch
        if cls in LABEL_CLASSES:
            # empirical (verified vs raw bytes): keepCrisp serializes as 8 bytes,
            # then 2 PPtr(12) -> mText string at cs+88
            o = cs + 56 + 32
            txt, o2 = read_str(raw, o)
            if txt is not None:
                d["text"] = txt
                after = o2
                fs = u32(raw, after)
                if 1 <= fs <= 200:
                    d["fontSize"] = fs
                    align = u32(raw, after + 8)
                    if 0 <= align <= 4:
                        d["alignment"] = align
                    ovf = u32(raw, after + 52)
                    if 0 <= ovf <= 5:
                        d["overflow"] = ovf
        elif cls == "UISprite":
            # empirical: UIBasicSprite contributes 80 serialized bytes
            # (type/fill/fillAmount/invert/flip/gradients/centerTypes) then
            # mAtlas PPtr(12) -> mSpriteName string at cs+148
            o = cs + 136 + 12
            spn, _ = read_str(raw, o)
            if spn is not None:
                d["spriteName"] = spn
    except (struct.error, UnicodeDecodeError):
        pass
    return d


def parse_panel(raw, ds):
    d = {}
    try:
        cs = ds + 84
        alpha = f32(raw, cs + 36)
        clip = [f32(raw, cs + 44 + i * 4) for i in range(4)]
        dep, order = u32(raw, cs + 68), u32(raw, cs + 72)
        if 0.0 <= alpha <= 1.001 and -100000 <= dep <= 100000:
            d.update(alpha=r(alpha, 4), depth=dep,
                     clipRange=[r(c, 2) for c in clip],
                     sortingOrder=order if -100 <= order <= 1000 else None)
    except struct.error:
        pass
    return d


def parse_uifont(raw, ds):
    """UIFont: mMat PPtr(12) mUVRect(16) BMFont{size,base,w,h,str,glyphs}
    mAtlas PPtr(12) mReplacement PPtr(12) mSymbols list mDynamicFont PPtr(12)
    mDynamicFontSize i32 mDynamicFontStyle i32."""
    d = {}
    try:
        rf, rp = struct.unpack_from("<iq", raw, ds + 64)  # mReplacement UIFont
        if rf == 0 and rp != 0:
            d["replacementUIFont"] = rp
        bm_size, bm_base, bm_w, bm_h = (u32(raw, ds + 28 + i * 4) for i in range(4))
        glyphs_off = ds + 28 + 16 + 4  # after empty mSpriteName string
        n_glyphs = u32(raw, glyphs_off)
        if 0 < n_glyphs < 100000 and 0 < bm_w <= 16384 and 0 < bm_h <= 16384:
            d["mode"] = "bitmap"
            d["bitmap"] = {"glyphs": n_glyphs, "atlasWidth": bm_w, "atlasHeight": bm_h,
                           "fontSize": bm_size}
        else:
            d["mode"] = "dynamic"
        # dynamic font reference (fixed offset when symbol list empty)
        fid, pid = struct.unpack_from("<iq", raw, ds + 80)
        size = u32(raw, ds + 92)
        style = u32(raw, ds + 96)
        if fid == 0 and pid != 0:
            d["dynamicFont"] = pid
            if 1 <= size <= 200:
                d["dynamicFontSize"] = size
            if 0 <= style <= 3:
                d["dynamicFontStyle"] = style
        else:
            # non-empty mSymbols shifts the tail; scan for a Font PPtr followed
            # by (fontSize 1..200, fontStyle 0..3)
            font_pids = getattr(parse_uifont, "_font_pids", frozenset())
            for off in range(0, len(raw) - 28, 4):
                f2, p2 = struct.unpack_from("<iq", raw, off)
                if f2 == 0 and p2 in font_pids:
                    s2 = u32(raw, off + 12)
                    st2 = u32(raw, off + 16)
                    if 1 <= s2 <= 200 and 0 <= st2 <= 3:
                        d["dynamicFont"] = p2
                        d["dynamicFontSize"] = s2
                        d["dynamicFontStyle"] = st2
                        d["symbols"] = True
                        break
    except struct.error:
        pass
    return d


def dump_bundle(pkg):
    path = os.path.join(BUNDLES, pkg + ".bundle")
    env = UnityPy.load(path)
    objs = {o.path_id: o for o in env.objects}
    parse_uifont._font_pids = frozenset(
        pid for pid, o in objs.items() if o.type.name == "Font")
    scripts = {}
    for pid, o in objs.items():
        if o.type.name == "MonoScript":
            try:
                scripts[pid] = o.read().m_ClassName
            except Exception:
                pass
    gos = {}
    for pid, o in objs.items():
        if o.type.name == "GameObject":
            try:
                g = o.read()
                gos[pid] = g
            except Exception:
                pass
    rts = {}
    rt_types = {}
    for pid, o in objs.items():
        if o.type.name in ("Transform", "RectTransform"):
            try:
                rts[pid] = o.read()
                rt_types[pid] = o.type.name
            except Exception:
                pass

    def comp_classes(go):
        out = []
        for c in go.m_Components:
            p = c.m_PathID
            t = objs.get(p)
            if t is None:
                out.append("<external>")
                continue
            if t.type.name == "MonoBehaviour":
                raw = t.get_raw_data() or b""
                h = mb_header(raw)
                out.append(scripts.get(h[1], "?") if h else "?")
            else:
                out.append(t.type.name)
        return out

    def widget_info(go):
        info = []
        for c in go.m_Components:
            t = objs.get(c.m_PathID)
            if t is None or t.type.name != "MonoBehaviour":
                continue
            raw = t.get_raw_data() or b""
            h = mb_header(raw)
            if not h:
                continue
            _, spid, ds = h
            cls = scripts.get(spid, "")
            if cls == "UIPanel":
                d = parse_panel(raw, ds)
                if d:
                    d["class"] = "UIPanel"
                    info.append(d)
            elif cls == "UIFont":
                d = parse_uifont(raw, ds)
                if d:
                    d["class"] = "UIFont"
                    info.append(d)
            elif cls in WIDGET_CLASSES or cls in LABEL_CLASSES or cls in SPRITE_CLASSES:
                d = parse_widget(raw, ds, cls)
                if d:
                    d["class"] = cls
                    info.append(d)
        return info

    child_of = {}
    for pid, tr in rts.items():
        for ch in tr.m_Children:
            child_of[ch.m_PathID] = pid
    roots = [pid for pid in rts if pid not in child_of]

    def walk(pid):
        tr = rts.get(pid)
        go = None
        if tr is not None:
            gref = getattr(tr.m_GameObject, "m_PathID", 0)
            if getattr(tr.m_GameObject, "m_FileID", 0) == 0:
                go = gos.get(gref)
        if tr is None:
            return None
        if go is None:
            # root transform whose GameObject lives in another bundle/file
            node = {"name": "<external:GameObject>", "external": True, "layer": None,
                    "components": [], "localPos": None, "localRot": None,
                    "localScale": None}
        else:
            node = {
                "name": go.m_Name, "active": bool(go.m_IsActive), "layer": go.m_Layer,
                "components": comp_classes(go),
            }
        lp = tr.m_LocalPosition
        lr = tr.m_LocalRotation
        ls = tr.m_LocalScale
        node["localPos"] = {"x": r(lp.x), "y": r(lp.y), "z": r(lp.z)}
        node["localRot"] = {"x": r(lr.x, 5), "y": r(lr.y, 5), "z": r(lr.z, 5), "w": r(lr.w, 5)}
        node["localScale"] = {"x": r(ls.x, 4), "y": r(ls.y, 4), "z": r(ls.z, 4)}
        if rt_types.get(pid) == "RectTransform":
            node["rect"] = {
                "anchorMin": {"x": r(tr.m_AnchorMin.x), "y": r(tr.m_AnchorMin.y)},
                "anchorMax": {"x": r(tr.m_AnchorMax.x), "y": r(tr.m_AnchorMax.y)},
                "anchoredPosition": {"x": r(tr.m_AnchoredPosition.x), "y": r(tr.m_AnchoredPosition.y)},
                "sizeDelta": {"x": r(tr.m_SizeDelta.x), "y": r(tr.m_SizeDelta.y)},
                "pivot": {"x": r(tr.m_Pivot.x), "y": r(tr.m_Pivot.y)},
            }
        wi = widget_info(go) if go is not None else []
        if wi:
            node["ngui"] = wi
        kids = []
        for ch in tr.m_Children:
            c = walk(ch.m_PathID)
            if c:
                kids.append(c)
        ext = sum(1 for ch in tr.m_Children if ch.m_PathID not in rts)
        if ext:
            node["external_children"] = ext
        node["children"] = kids
        return node

    tree = [t for t in (walk(p) for p in roots) if t]
    return {
        "bundle": pkg,
        "gameobjects": len(gos),
        "transforms": len(rts),
        "rect_transforms": sum(1 for t in rt_types.values() if t == "RectTransform"),
        "roots": tree,
    }


def main():
    summary = []
    for pkg in TARGETS:
        path = os.path.join(BUNDLES, pkg + ".bundle")
        if not os.path.exists(path):
            print(f"MISSING bundle for {pkg}")
            continue
        try:
            res = dump_bundle(pkg)
        except Exception as e:
            import traceback
            traceback.print_exc()
            res = {"bundle": pkg, "error": str(e)}
        out = os.path.join(OUT, pkg + ".json")
        with open(out, "w", encoding="utf-8") as f:
            json.dump(res, f, ensure_ascii=False, indent=1)
        summary.append((pkg, res.get("gameobjects"), res.get("transforms"),
                        res.get("rect_transforms"), os.path.getsize(out)))
    for s in summary:
        print(f"{s[0]}: gos={s[1]} transforms={s[2]} rectTransforms={s[3]} -> {s[4]:,} bytes")


if __name__ == "__main__":
    main()
