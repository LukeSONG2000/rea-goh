# -*- coding: utf-8 -*-
"""Dump original battle UI layout prefab hierarchies (RectTransform trees)
from battle_* layout bundles -> catalog/ui_layouts.json.

The dump records, per GameObject: name, active state, RectTransform anchors/
position/size, and which Image/Sprite or Text component it carries -- enough
to rebuild the layout 1:1 in uGUI."""
import json
import pathlib
import sys

import UnityPy

CACHE = pathlib.Path(__file__).resolve().parent.parent / "raw_bundles"
OUT = pathlib.Path(__file__).resolve().parent.parent / "catalog" / "ui_layouts.json"


def newest_data(name):
    d = CACHE / name
    if not d.is_dir():
        return None
    subs = sorted((s for s in d.iterdir() if s.is_dir()),
                  key=lambda s: s.stat().st_mtime)
    if not subs:
        return None
    f = subs[-1] / "__data"
    return f if f.exists() else None


def vec(t):
    return {k: round(getattr(t, k), 3) for k in ("x", "y", "z") if hasattr(t, k)}


def rect_data(rt):
    return {
        "anchorsMin": vec(rt.m_AnchorMin), "anchorsMax": vec(rt.m_AnchorMax),
        "anchoredPos": vec(rt.m_AnchoredPosition),
        "sizeDelta": vec(rt.m_SizeDelta), "pivot": vec(rt.m_Pivot),
    }


def dump_bundle(name):
    f = newest_data(name)
    if not f:
        return None
    env = UnityPy.load(str(f))
    objs = {o.path_id: o for o in env.objects}
    gos = {pid: o.read() for pid, o in objs.items()
           if o.type.name == "GameObject"}
    rts = {pid: o.read() for pid, o in objs.items()
           if o.type.name == "RectTransform"}
    # sprite names for Image components
    sprites = {}
    for pid, o in objs.items():
        if o.type.name == "Sprite":
            try:
                sprites[pid] = o.read().m_Name
            except Exception:
                pass

    def describe_go(pid):
        go = gos[pid]
        node = {"name": go.m_Name, "active": bool(go.m_IsActive), "children": []}
        rt = rts.get(pid)
        if rt:
            node["rect"] = rect_data(rt)
        for comp in go.m_Components:
            cpid = comp.component.m_PathID
            if cpid not in objs:
                continue
            tname = objs[cpid].type.name
            if tname == "Image":
                try:
                    img = objs[cpid].read()
                    spid = img.m_Sprite.m_PathID
                    node.setdefault("image", {})["sprite"] = sprites.get(spid)
                except Exception:
                    node.setdefault("image", {})["sprite"] = None
            elif tname == "Text":
                try:
                    t = objs[cpid].read()
                    node["text"] = t.m_Text
                except Exception:
                    pass
        return node

    # walk RectTransform hierarchy from roots
    child_of = {}
    for pid, rt in rts.items():
        for ch in rt.m_Children:
            child_of[ch.m_PathID] = pid
    roots = [pid for pid in rts if pid not in child_of]

    def walk(pid):
        if pid not in gos:
            return None
        node = describe_go(pid)
        for ch in rts[pid].m_Children:
            c = walk(ch.m_PathID)
            if c:
                node["children"].append(c)
        return node

    tree = [walk(r) for r in roots]
    tree = [t for t in tree if t]
    return {"bundle": name, "gos": len(gos), "rects": len(rts),
            "sprites": len(sprites), "roots": tree}


def main():
    targets = sorted(d.name for d in CACHE.iterdir()
                     if d.is_dir() and "_layout" in d.name.lower())
    out = {}
    for t in targets:
        try:
            r = dump_bundle(t)
        except Exception as e:
            r = {"bundle": t, "error": str(e)}
        if r:
            out[t] = r
    OUT.write_text(json.dumps(out, ensure_ascii=False), encoding="utf-8")
    print(f"dumped {len(out)}/{len(targets)} layout bundles -> {OUT.name}")
    for k, v in list(out.items())[:6]:
        print(f"  {k}: gos={v.get('gos')} rects={v.get('rects')} sprites={v.get('sprites')}")


if __name__ == "__main__":
    sys.exit(main())
