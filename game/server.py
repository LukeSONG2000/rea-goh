# -*- coding: utf-8 -*-
"""Battle demo API + static server (stdlib only, deploy-safe).

Routes:
    /api/roster                     -> playable roster [{def_id, name}]
    /api/battle?a=A,B&b=C,D&seed=N  -> {units, events, winner, seed,
                                        env, staging?}
    /api/staging?env=ENV_ID         -> original arena staging markers
                                       (catalog/battle_staging.json; {}
                                        when unknown)
    /...                            -> static files from repo root
                                       (extracted/, viewer/, catalog/, ...)

Run:  py game/server.py [port]      (default 8138, binds 127.0.0.1;
on the deploy box caddy reverse-proxies /api/* to it)
"""
import json
import os
import sys
from http.server import SimpleHTTPRequestHandler, ThreadingHTTPServer
from urllib.parse import urlparse, parse_qs

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
for p in (ROOT, os.path.join(ROOT, "il2cpp", "offline")):
    if p not in sys.path:
        sys.path.insert(0, p)

from game import data_units, engine  # noqa: E402

# ---------------------------------------------------------------------------
# battle staging (bug P1 fix): catalog/battle_staging.json holds the original
# per-arena unit markers decoded from the battle_*_layout bundles (see
# tools/dump_battle_staging.py).  Positions are stored in Unity space; the
# exported env GLBs are X-mirrored, so we mirror x / quaternion here and hand
# the client GLB-space coords it can use directly.
# ---------------------------------------------------------------------------
_STAGING = None


def load_staging():
    global _STAGING
    if _STAGING is None:
        path = os.path.join(ROOT, "catalog", "battle_staging.json")
        if os.path.exists(path):
            with open(path, encoding="utf-8") as f:
                _STAGING = json.load(f)
    return _STAGING


def _mirror_pos(p):
    return [-p[0], p[1], p[2]]


def _mirror_rot(q):
    return [q[0], -q[1], -q[2], q[3]]


def staging_for_env(env):
    """GLB-space staging markers for an env id (client defaults apply when
    the env has no concrete gamedata binding: pvp default for the demo)."""
    st = load_staging()
    if not st:
        return None
    layout = (st.get("env_index", {}).get(env or "")
              or st.get("_meta", {}).get("defaults", {}).get("pvp"))
    lay = (st.get("layouts", {}).get(layout) or {})
    if lay.get("kind") != "battle":
        return None
    out = {"layout": layout, "player": [], "enemy": []}
    for side in ("player", "enemy"):
        for s in lay.get("slots", {}).get(side, []):
            out[side].append({"slot": s["slot"], "label": s["label"],
                              "tag": s["tag"],
                              "pos": _mirror_pos(s["pos"]),
                              "rot": _mirror_rot(s["rot"])})
    cam = lay.get("camera")
    out["camera"] = ({"pos": _mirror_pos(cam["pos"]),
                      "rot": _mirror_rot(cam["rot"])} if cam else None)
    return out


_PORTRAITS = None


def portraits():
    """def_id -> official UI portrait path (extracted_ui/charui, resolved
    from units.json thumbnailName; game/portraits.json built offline)."""
    global _PORTRAITS
    if _PORTRAITS is None:
        path = os.path.join(ROOT, "game", "portraits.json")
        _PORTRAITS = json.load(open(path, encoding="utf-8")) \
            if os.path.exists(path) else {}
    return _PORTRAITS


def roster_payload():
    r = data_units.roster()
    ports = portraits()
    out = []
    for k, v in sorted(r.items()):
        folder = v.get("char") or ""
        anim = os.path.exists(os.path.join(
            ROOT, "extracted", folder, folder + ".anim.glb"))
        out.append({"def_id": k, "name": (v.get("en") or k),
                    "folder": folder, "anim": anim,
                    # web-rooted so /viewer pages resolve it (404s would
                    # otherwise strip the card image via onerror)
                    "portrait": ('/' + ports[k]) if ports.get(k) else ""})
    return out


def battle_payload(a, b, seed, env=None):
    a = [x for x in a if x]
    b = [x for x in b if x]
    if not a or not b:
        return {"error": "both teams need at least one unit"}
    events, winner, units = engine.battle(a, b, seed)
    out = {"units": units, "events": events, "winner": winner, "seed": seed,
           "env": env or ""}
    staging = staging_for_env(env)
    if staging:
        out["staging"] = staging
    return out


class Handler(SimpleHTTPRequestHandler):
    def __init__(self, *args, **kw):
        super().__init__(*args, directory=ROOT, **kw)

    def log_message(self, fmt, *args):    # quiet
        pass

    def send_json(self, obj, code=200):
        body = json.dumps(obj, ensure_ascii=False).encode()
        self.send_response(code)
        self.send_header("Content-Type", "application/json; charset=utf-8")
        self.send_header("Cache-Control", "no-store")
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def do_GET(self):
        u = urlparse(self.path)
        if u.path == "/api/roster":
            try:
                return self.send_json(roster_payload())
            except Exception as e:
                return self.send_json({"error": str(e)}, 500)
        if u.path == "/api/battle":
            try:
                q = parse_qs(u.query)
                a = (q.get("a", [""])[0]).split(",")
                b = (q.get("b", [""])[0]).split(",")
                seed = int(q.get("seed", ["1"])[0])
                env = (q.get("env", [""])[0]) or None
                return self.send_json(battle_payload(a, b, seed, env))
            except SystemExit as e:      # data_units.resolve errors
                return self.send_json({"error": str(e)}, 400)
            except Exception as e:
                return self.send_json({"error": f"{type(e).__name__}: {e}"}, 500)
        if u.path == "/api/staging":
            q = parse_qs(u.query)
            env = (q.get("env", [""])[0]) or None
            return self.send_json(staging_for_env(env) or {})
        if u.path == "/":
            self.send_response(302)
            self.send_header("Location", "/viewer/battle.html")
            self.end_headers()
            return
        return super().do_GET()


if __name__ == "__main__":
    port = int(sys.argv[1]) if len(sys.argv) > 1 else 8138
    srv = ThreadingHTTPServer(("127.0.0.1", port), Handler)
    print(f"serving {ROOT} on http://127.0.0.1:{port}", flush=True)
    srv.serve_forever()
