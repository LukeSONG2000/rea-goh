# -*- coding: utf-8 -*-
"""Battle demo API + static server (stdlib only, deploy-safe).

Routes:
    /api/roster                     -> playable roster [{def_id, name}]
    /api/battle?a=A,B&b=C,D&seed=N  -> {units, events, winner}
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


def roster_payload():
    r = data_units.roster()
    out = []
    for k, v in sorted(r.items()):
        folder = v.get("char") or ""
        anim = os.path.exists(os.path.join(
            ROOT, "extracted", folder, folder + ".anim.glb"))
        out.append({"def_id": k, "name": (v.get("en") or k),
                    "folder": folder, "anim": anim})
    return out


def battle_payload(a, b, seed):
    a = [x for x in a if x]
    b = [x for x in b if x]
    if not a or not b:
        return {"error": "both teams need at least one unit"}
    events, winner, units = engine.battle(a, b, seed)
    return {"units": units, "events": events, "winner": winner, "seed": seed}


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
                return self.send_json(battle_payload(a, b, seed))
            except SystemExit as e:      # data_units.resolve errors
                return self.send_json({"error": str(e)}, 400)
            except Exception as e:
                return self.send_json({"error": f"{type(e).__name__}: {e}"}, 500)
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
