# -*- coding: utf-8 -*-
"""(Re)build catalog/gamedata/ from the swgoh-utils/gamedata mirror.

github.com is intermittently unreachable from this machine; the codeload
tarball endpoint keeps working when git/https are reset, so that is the
primary path.  effect.json ships only brotli-compressed and exceeds the
GitHub 100MB blob limit uncompressed -- the .br (1.5MB) is committed instead
and decompressed here (pip install brotli).

Run:  py tools/build_gamedata.py            # fetch + extract + decompress
      py tools/build_gamedata.py --effects-only
"""
import io
import json
import os
import tarfile
import urllib.request

CAT = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))),
                   "catalog")
GD = os.path.join(CAT, "gamedata")
TARBALL = "https://codeload.github.com/swgoh-utils/gamedata/tar.gz/refs/heads/main"


def fetch(out_path):
    if os.path.exists(out_path):
        print("using existing", out_path)
        return
    print("fetching", TARBALL)
    data = urllib.request.urlopen(TARBALL, timeout=120).read()
    with open(out_path, "wb") as f:
        f.write(data)


def extract(tgz, effects_only=False):
    os.makedirs(GD, exist_ok=True)
    with tarfile.open(tgz, "r:gz") as tf:
        for m in tf.getmembers():
            if m.isdir() or "/." in m.name:
                continue
            name = os.path.basename(m.name)
            if effects_only and name != "effect.json.br":
                continue
            if name.endswith(".br"):
                dest = os.path.join(GD, name)
                if os.path.exists(dest):
                    continue
                open(dest, "wb").write(tf.extractfile(m).read())
                continue
            if effects_only:
                continue
            dest = os.path.join(GD, name)
            if os.path.exists(dest):
                continue
            with open(dest, "wb") as f:
                f.write(tf.extractfile(m).read())
            if name == "effect.json.br":
                continue


def decompress_effects():
    import brotli
    src = os.path.join(GD, "effect.json.br")
    dst = os.path.join(GD, "effect.json")
    if not os.path.exists(src):
        raise SystemExit("effect.json.br missing -- run with --effects-only or full fetch")
    if os.path.exists(dst):
        print("effect.json already present")
        return
    raw = brotli.decompress(open(src, "rb").read())
    json.loads(raw)                      # sanity
    open(dst, "wb").write(raw)
    print(f"effect.json: {len(raw)/1e6:.1f}MB")


def main():
    import argparse
    ap = argparse.ArgumentParser()
    ap.add_argument("--effects-only", action="store_true")
    a = ap.parse_args()
    if not a.effects_only:
        fetch(os.path.join(os.environ.get("TEMP", "/tmp"), "gamedata.tar.gz"))
        extract(os.path.join(os.environ.get("TEMP", "/tmp"), "gamedata.tar.gz"))
    decompress_effects()


if __name__ == "__main__":
    main()
