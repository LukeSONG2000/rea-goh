"""Build full inventory of SWGoH Unity cache bundles. Read-only scan."""
import os, json, sys, time

ROOT = r"C:\Users\PPfavorite\AppData\LocalLow\Unity\Electronic Arts_SWGoH"
OUT = r"F:\PPfavorite\Work\goh\catalog\bundles.json"

KNOWN = {"char_": "char", "battle_character": "battle_character", "shared_": "shared"}

def category(name):
    if name.startswith("char_"):
        return "char"
    if name.startswith("battle_character"):
        return "battle_character"
    if name.startswith("shared_"):
        return "shared"
    # prefix before first '_'
    p = name.split("_", 1)[0]
    return p or name

def main():
    t0 = time.time()
    entries = []
    broken = []
    with os.scandir(ROOT) as it:
        dirs = [e.name for e in it if e.is_dir()]
    dirs.sort()
    for name in dirs:
        bdir = os.path.join(ROOT, name)
        best = None  # (mtime, path, size)
        try:
            with os.scandir(bdir) as hit:
                for h in hit:
                    if not h.is_dir():
                        continue
                    data = os.path.join(h.path, "__data")
                    if os.path.isfile(data):
                        st = os.stat(data)
                        if best is None or st.st_mtime > best[0]:
                            best = (st.st_mtime, data, st.st_size)
        except OSError as e:
            broken.append((name, str(e)))
            continue
        if best is None:
            broken.append((name, "no __data"))
            continue
        entries.append({
            "bundle": name,
            "path": best[1],
            "size_mb": round(best[2] / (1024 * 1024), 3),
            "size_bytes": best[2],
            "mtime": int(best[0]),
            "category": category(name),
        })

    with open(OUT, "w", encoding="utf-8") as f:
        json.dump(entries, f, ensure_ascii=False, indent=1)

    # stats
    cats = {}
    for e in entries:
        c = cats.setdefault(e["category"], {"count": 0, "mb": 0.0})
        c["count"] += 1
        c["mb"] += e["size_mb"]
    print(f"scanned {len(entries)} bundles in {time.time()-t0:.1f}s, broken={len(broken)}")
    for c, v in sorted(cats.items(), key=lambda kv: -kv[1]["count"]):
        print(f"{c:24s} n={v['count']:5d}  {v['mb']/1024:8.2f} GB")
    if broken:
        print("BROKEN:", broken[:10])

if __name__ == "__main__":
    main()
