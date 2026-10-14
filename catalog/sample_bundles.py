"""Sample representative bundles per category with UnityPy, aggregate per category."""
import json, os, sys, time
from collections import Counter
from multiprocessing import Pool

CATALOG = r"F:\PPfavorite\Work\goh\catalog\bundles.json"
OUT = r"F:\PPfavorite\Work\goh\catalog\samples.json"

def sample_one(task):
    cat, path, bundle = task
    import UnityPy
    r = {"bundle": bundle, "category": cat, "path": path, "error": None,
         "n_objects": 0, "types": {}, "go": [], "tex": [], "ta": [], "clip": [],
         "mesh_names": [], "container_n": 0, "container_samples": []}
    try:
        env = UnityPy.load(path)
        types = Counter()
        go, tex, ta, clip, mesh = [], [], [], [], []
        for obj in env.objects:
            t = obj.type.name
            types[t] += 1
            try:
                if t == "GameObject" and len(go) < 40:
                    go.append(obj.parse_as_object().m_Name)
                elif t == "Texture2D" and len(tex) < 40:
                    tex.append(obj.parse_as_object().m_Name)
                elif t == "TextAsset" and len(ta) < 40:
                    ta.append(obj.parse_as_object().m_Name)
                elif t == "AnimationClip" and len(clip) < 40:
                    clip.append(obj.parse_as_object().m_Name)
                elif t == "Mesh" and len(mesh) < 40:
                    mesh.append(obj.parse_as_object().m_Name)
            except Exception:
                pass
        r["n_objects"] = sum(types.values())
        r["types"] = dict(types)
        r["go"] = go; r["tex"] = tex; r["ta"] = ta; r["clip"] = clip; r["mesh_names"] = mesh
        try:
            cont = env.container
            r["container_n"] = len(cont)
            r["container_samples"] = list(cont.keys())[:30]
        except Exception:
            pass
    except Exception as e:
        r["error"] = repr(e)[:200]
    return r

def pick(names):
    """Evenly spaced sample of bundle names, sorted."""
    names = sorted(names)
    n = len(names)
    if n >= 40: k = 10
    elif n >= 8: k = min(n, 8)
    elif n >= 2: k = min(n, 6)
    else: k = n
    if k >= n: return names
    step = n / k
    return [names[min(n - 1, int(i * step))] for i in range(k)]

def main():
    es = json.load(open(CATALOG, encoding="utf-8"))
    cats = {}
    for e in es:
        cats.setdefault(e["category"], []).append(e)
    tasks = []
    for c, items in sorted(cats.items()):
        if c == "char":
            continue  # char_ counted only, never opened
        for b in pick([i["bundle"] for i in items]):
            d = next(i for i in items if i["bundle"] == b)
            tasks.append((c, d["path"], d["bundle"]))
    print(f"{len(tasks)} bundles to sample across {len(cats)-1} categories", flush=True)
    t0 = time.time()
    results = []
    with Pool(processes=8) as pool:
        for i, res in enumerate(pool.imap_unordered(sample_one, tasks, chunksize=1)):
            results.append(res)
            if (i + 1) % 50 == 0:
                print(f"  {i+1}/{len(tasks)}  {time.time()-t0:.0f}s", flush=True)
    # aggregate
    agg = {}
    for r in results:
        a = agg.setdefault(r["category"], {"sampled": [], "types_total": Counter(), "go": [], "tex": [],
                                           "ta": [], "clip": [], "mesh": [], "container_samples": [],
                                           "errors": 0, "n_objects_sum": 0})
        a["sampled"].append({"bundle": r["bundle"], "size_mb": None, "n_objects": r["n_objects"],
                             "types": r["types"], "error": r["error"]})
        a["types_total"].update(r["types"])
        a["n_objects_sum"] += r["n_objects"] or 0
        for k_list, r_key in (("go", "go"), ("tex", "tex"), ("ta", "ta"), ("clip", "clip"),
                              ("mesh", "mesh_names"), ("container_samples", "container_samples")):
            for v in r[r_key]:
                if v and v not in a[k_list] and len(a[k_list]) < 30:
                    a[k_list].append(v)
        if r["error"]:
            a["errors"] += 1
    # re-attach size
    size_by_bundle = {e["bundle"]: e["size_mb"] for e in es}
    count_by_cat = {c: len(v) for c, v in cats.items()}
    out = {}
    for c, a in agg.items():
        for s in a["sampled"]:
            s["size_mb"] = size_by_bundle.get(s["bundle"])
        out[c] = {"n_bundles": count_by_cat.get(c, 0), "sampled_n": len(a["sampled"]),
                  "types_total": dict(a["types_total"].most_common()),
                  "go": a["go"], "tex": a["tex"], "ta": a["ta"], "clip": a["clip"],
                  "mesh": a["mesh"], "container_samples": a["container_samples"],
                  "errors": a["errors"], "sampled": a["sampled"]}
    with open(OUT, "w", encoding="utf-8") as f:
        json.dump(out, f, ensure_ascii=False, indent=1)
    print(f"done in {time.time()-t0:.0f}s -> {OUT}", flush=True)
    errs = [ (c, s['bundle'], s['error']) for c, a in out.items() for s in a['sampled'] if s['error'] ]
    print(f"errors: {len(errs)}")
    for e in errs[:15]: print("  ", e)

if __name__ == "__main__":
    main()
