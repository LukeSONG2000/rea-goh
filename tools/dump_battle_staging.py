# -*- coding: utf-8 -*-
"""Decode original battle staging (unit placement) -> catalog/battle_staging.json
(bug P1: "units spawn outside the arena").

Faithful source chain
---------------------
1. Every ``battle_*_layout`` bundle holds a 3D staging prefab: a single root
   (``prefab.<bundle>``) whose ``PlayerMarkers`` / ``EnemyMarkers`` children
   carry ``N-UnitMarker`` transforms -- the exact slots the game attaches unit
   GameObjects to (il2cpp: ``PlaceUnitInScene: _unitLayout must not be null``,
   ``/Unit Layout Root/UnitMarkers/...``, "Attempting to get an ally marker
   but it already has a unit assigned on layout").
2. Which layout pairs with which environment comes from gamedata encounters
   (``catalog/gamedata/campaign.json`` / ``campaign_gas.json`` /
   ``conquestMission.json``): ``environment = {prefab: "env.env_X", layout:
   "STANDARD"|"FLIPPED"|..., layoutPrefab: "prefab.battle_Y_layout"}``.  An
   empty ``layoutPrefab`` means the client default, confirmed by il2cpp string
   literals: PVE -> ``battle_character_pve_layout``, PVP ->
   ``battle_character_pvp_layout`` (ships: ``battle_ship_{pve,pvp}_layout``).
3. Positions are stored in raw Unity world space (X mirrored on the client --
   exported env GLBs use the same X-mirror, see tools/swgoh_extract.py
   conv_v3/quat_to_mat).

Marker catalogue (census over all 142 bundles, see module bottom of
catalog/ui_layouts.json history): ``PlayerMarkers`` / ``EnemyMarkers`` groups
with ``1-UnitMarker`` .. ``6-UnitMarker`` (6 = "Ally" slot, labelled
``6-UnitMarker (Ally)``), raid layouts add ``7-UnitMarker (Big)``; AOE centres
live in ``PlayerMarkersAOE`` / ``EnemyMarkersAOE`` (``All``/``FrontRow``/
``BackRow``); ``CameraMarker`` > ``Camera`` frames the arena.
"""
import collections
import json
import pathlib
import re
import sys

import UnityPy

CACHE = pathlib.Path(r"C:\Users\PPfavorite\AppData\LocalLow\Unity"
                     r"\Electronic Arts_SWGoH")
ROOT = pathlib.Path(__file__).resolve().parent.parent
OUT = ROOT / "catalog" / "battle_staging.json"
GAMEDATA = ROOT / "catalog" / "gamedata"

SLOT_RE = re.compile(r"^(\d+)-UnitMarker( \((\w+)\))?$")
AOE_KEYS = ("All", "FrontRow", "BackRow", "Center")
# client defaults when an encounter's layoutPrefab is empty (il2cpp literals)
LAYOUT_DEFAULTS = {
    "pve": "battle_character_pve_layout",
    "pvp": "battle_character_pvp_layout",
    "ship_pve": "battle_ship_pve_layout",
    "ship_pvp": "battle_ship_pvp_layout",
}


def newest_data(folder):
    subs = sorted((s for s in folder.iterdir() if s.is_dir()),
                  key=lambda s: s.stat().st_mtime)
    if not subs:
        return None
    f = subs[-1] / "__data"
    return f if f.exists() else None


def xform_chain(t):
    """Root->local world matrix of a parsed Transform (numpy 4x4)."""
    M = local_matrix(t)
    fa = t.m_Father.m_PathID
    if fa and fa in TRS:
        M = xform_chain(TRS[fa]) @ M
    return M


def local_matrix(t):
    import numpy as np
    lp, lr, ls = t.m_LocalPosition, t.m_LocalRotation, t.m_LocalScale
    x, y, z, w = lr.x, lr.y, lr.z, lr.w
    R = np.array([
        [1 - 2 * (y * y + z * z), 2 * (x * y - z * w), 2 * (x * z + y * w)],
        [2 * (x * y + z * w), 1 - 2 * (x * x + z * z), 2 * (y * z - x * w)],
        [2 * (x * z - y * w), 2 * (y * z + x * w), 1 - 2 * (x * x + y * y)]])
    M = np.eye(4)
    M[:3, :3] = R * np.array([ls.x, ls.y, ls.z])
    M[:3, 3] = (lp.x, lp.y, lp.z)
    return M


TRS = {}      # module-level transform index for xform_chain
GOS = {}


def load_layout(name):
    f = newest_data(CACHE / name)
    if f is None:
        return None
    env = UnityPy.load(str(f))
    TRS.clear()
    GOS.clear()
    for o in env.objects:
        tn = o.type.name
        if tn == "Transform":
            try:
                TRS[o.path_id] = o.read()
            except Exception:
                pass
        elif tn == "GameObject":
            try:
                GOS[o.path_id] = o.read()
            except Exception:
                pass
    return {"env": env}


def go_of(t):
    return GOS.get(t.m_GameObject.m_PathID)


def world_pos(t):
    M = xform_chain(t)
    return [round(float(M[0, 3]), 4), round(float(M[1, 3]), 4),
            round(float(M[2, 3]), 4)]


def world_rot(t):
    """World rotation as xyzw quaternion (used for unit facing)."""
    import numpy as np
    M = xform_chain(t)[:3, :3]
    # mat -> quat (shear-free assumption holds for staging markers)
    tr = M[0, 0] + M[1, 1] + M[2, 2]
    if tr > 0:
        s = np.sqrt(tr + 1.0) * 2
        w = 0.25 * s
        x = (M[2, 1] - M[1, 2]) / s
        y = (M[0, 2] - M[2, 0]) / s
        z = (M[1, 0] - M[0, 1]) / s
    else:
        x = y = z = 0.0
        w = 1.0
    n = (x * x + y * y + z * z + w * w) ** 0.5 or 1.0
    return [round(v / n, 4) for v in (x, y, z, w)]


def children(t):
    out = []
    for ch in t.m_Children:
        ct = TRS.get(ch.m_PathID)
        if ct is not None:
            out.append(ct)
    return out


def parse_layout(name):
    load_layout(name)
    roots = [t for t in TRS.values() if t.m_Father.m_PathID == 0]
    if not roots:
        return None
    # single-prefab bundles: root 'prefab.<name>'; ship layouts nest under
    # 'ShipLayout'; use the deepest 'Markers' group wherever it lives
    markers = {}
    aoe = {}
    camera = None
    for t in TRS.values():
        go = go_of(t)
        if go is None:
            continue
        nm = go.m_Name
        if nm in ("PlayerMarkers", "EnemyMarkers"):
            side = "player" if nm == "PlayerMarkers" else "enemy"
            slots = []
            for ct in children(t):
                cgo = go_of(ct)
                if cgo is None:
                    continue
                m = SLOT_RE.match(cgo.m_Name)
                if m:
                    slots.append({
                        "slot": int(m.group(1)),
                        "label": cgo.m_Name,
                        "tag": m.group(3) or "",
                        "pos": world_pos(ct),
                        "rot": world_rot(ct),
                    })
            slots.sort(key=lambda s: s["slot"])
            if slots:
                markers[side] = slots
        elif nm in ("PlayerMarkersAOE", "EnemyMarkersAOE"):
            side = "player" if nm == "PlayerMarkersAOE" else "enemy"
            d = {}
            for ct in children(t):
                cgo = go_of(ct)
                if cgo is not None and cgo.m_Name in AOE_KEYS:
                    d[cgo.m_Name] = world_pos(ct)
            if d:
                aoe[side] = d
        elif nm == "CameraMarker":
            for ct in children(t):
                cgo = go_of(ct)
                if cgo is not None and cgo.m_Name == "Camera":
                    camera = {"pos": world_pos(ct), "rot": world_rot(ct)}
    if not markers:
        return {"kind": "other", "root": go_of(roots[0]).m_Name}
    return {
        "kind": "battle",
        "player": [s["pos"] for s in markers.get("player", [])],
        "enemy": [s["pos"] for s in markers.get("enemy", [])],
        "slots": {side: [{"slot": s["slot"], "label": s["label"],
                          "tag": s["tag"], "pos": s["pos"], "rot": s["rot"]}
                         for s in markers.get(side, [])]
                  for side in markers},
        "aoe": aoe,
        "camera": camera,
    }


def env_layout_pairs():
    """gamedata encounters: env id -> concrete layout bundle (or default)."""
    pairs = collections.Counter()      # (env, layout) concrete bindings
    for fname in ("campaign.json", "campaign_gas.json", "conquestMission.json"):
        data = json.loads((GAMEDATA / fname).read_text(encoding="utf-8"))
        text = json.dumps(data.get("data", data))
        for m in re.finditer(
                r'"prefab":\s*"env\.([a-z0-9_]+)",\s*"layout":\s*"([^"]*)",'
                r'\s*"layoutPrefab":\s*"prefab\.([a-z0-9_]+)"', text):
            pairs[(m.group(1), m.group(3))] += 1
    return pairs


def main():
    bundles = sorted(d.name for d in CACHE.iterdir()
                     if d.is_dir() and "_layout" in d.name.lower())
    layouts = {}
    for name in bundles:
        try:
            r = parse_layout(name)
        except Exception as e:
            r = {"kind": "error", "error": f"{type(e).__name__}: {e}"}
        if r:
            layouts[name] = r
    battles = {k: v for k, v in layouts.items() if v.get("kind") == "battle"}

    # env -> layout index (concrete gamedata bindings; defaults fill the rest)
    env_index = {}
    for (env, layout), n in env_layout_pairs().items():
        if layout in battles:
            env_index.setdefault(env, layout)

    out = {
        "_meta": {
            "source": "battle_*_layout bundles (cache) + gamedata encounters",
            "coords": "Unity world space, Y up, Z depth (player at -Z); "
                      "exported env GLBs are X-mirrored: client negates x",
            "slots": "player/enemy arrays are slot order 1..N; slot 1..5 = "
                     "squad, 6 = ally, 7 (tag 'Big') = raid boss",
            "defaults": LAYOUT_DEFAULTS,
            "layouts_decoded": len(battles),
            "layouts_other": sum(1 for v in layouts.values()
                                 if v.get("kind") == "other"),
            "env_bindings": len(env_index),
        },
        "layouts": layouts,
        "env_index": env_index,
    }
    OUT.write_text(json.dumps(out, ensure_ascii=False, indent=1),
                   encoding="utf-8")
    print(f"wrote {OUT}  layouts={len(layouts)} "
          f"(battle={len(battles)}) env_bindings={len(env_index)}")

    # quick census print
    sides = collections.Counter(
        (len(v.get("player", [])), len(v.get("enemy", [])))
        for v in battles.values())
    for shape, n in sorted(sides.items(), key=lambda x: -x[1]):
        print(f"  shape player/enemy {shape}: {n} layouts")
    demo = ("battle_character_pvp_layout", "battle_character_pve_layout",
            "battle_character_event_default_layout")
    for d in demo:
        if d in battles:
            v = battles[d]
            print(f"  {d}: player={v['player'][:2]}... enemy={v['enemy'][:2]}...")
    return 0


if __name__ == "__main__":
    sys.exit(main())
