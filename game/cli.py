# -*- coding: utf-8 -*-
"""CLI battle runner.

Usage:
    py game/cli.py DARTHVIDER,GRANDMOFFTARKIN vs ADMIRALACKBAR,REY --seed 42
    py game/cli.py --roster 3            # suggest 3 playable teams

defIds are game_roster.json keys (model-backed units); match is
case-insensitive and English-name substrings work too ("ackbar").
"""
import os
import sys

GAME = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(GAME)
for p in (ROOT,):
    if p not in sys.path:
        sys.path.insert(0, p)

from game import data_units, engine  # noqa: E402


def resolve(name):
    """defId / english-name lookup over the playable roster."""
    r = data_units.roster()
    up = name.upper()
    if up in r:
        return up
    for did, meta in r.items():
        if (meta.get("en") or "").upper() == up:
            return did
    hits = [did for did, meta in r.items()
            if up in did or up in (meta.get("en") or "").upper()]
    if len(hits) == 1:
        return hits[0]
    if not hits:
        raise SystemExit(f"unknown unit {name!r} (not in playable roster)")
    raise SystemExit(f"ambiguous {name!r}: {hits[:8]}")


def main():
    import argparse
    ap = argparse.ArgumentParser()
    ap.add_argument("team_a")
    ap.add_argument("vs")
    ap.add_argument("team_b")
    ap.add_argument("--seed", type=int, default=1)
    args = ap.parse_args()

    a = [resolve(x) for x in args.team_a.split(",") if x.strip()]
    b = [resolve(x) for x in args.team_b.split(",") if x.strip()]
    events, winner = engine.battle(a, b, args.seed)

    print(f"seed={args.seed}  A={a}  vs  B={b}")
    for e in events:
        line = (f"  turn {e['turn']:>3}  {e['actor_name']}"
                f"  -> {e.get('target_name')}")
        if e.get("damage") is not None:
            line += f"  dmg {e['damage']:>9,.1f}   target hp {e['hp_after']:>10,.1f}"
        print(line)
    print(f"winner: {winner}  ({len(events)} turns)")


if __name__ == "__main__":
    main()
