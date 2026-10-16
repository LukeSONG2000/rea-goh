# SWGoH CDN UI-prefab fetcher.
# Fetch set: all non-audio packages matching UI patterns (ui_*, homebase*,
# contextual*, commanderselectui, uiglobals, splash, textinputui, any 'ui_'
# substring, *_view) PLUS their transitive dependency closure from the manifest.
# Host: https://eaassets-a.akamaihd.net ONLY (public CDN, no auth).
# Each file: curl -> size check vs manifest field 3 -> retry 5x with 30s sleep.
import json, os, re, subprocess, sys, time

BASE = os.path.dirname(os.path.abspath(__file__))
OUT = os.path.join(BASE, "bundles")
os.makedirs(OUT, exist_ok=True)
ROOT = "https://eaassets-a.akamaihd.net/assetssw.capitalgames.com/PROD/100054/Windows/ETC"

d = json.load(open(os.path.join(BASE, "manifest_packages.json"), encoding="utf-8"))
by = {p["name"]: p for p in d["packages"]}

UI_PAT = re.compile(r"^(ui_|homebase|contextual|commanderselectui|uiglobals|splash|textinputui)")
def is_ui(n):
    if n.startswith("audio_"):
        return False
    return bool(UI_PAT.match(n) or ("ui_" in n) or n.endswith("_view"))

core = [p["name"] for p in by.values() if is_ui(p["name"])]
seen = set()
stack = list(core)
while stack:
    n = stack.pop()
    if n in seen:
        continue
    seen.add(n)
    for dep in by.get(n, {}).get("deps", []):
        if dep not in seen:
            stack.append(dep)
fetch = sorted(seen)
with open(os.path.join(BASE, "fetch_list_full.txt"), "w", encoding="utf-8") as f:
    for n in fetch:
        f.write(f"{n}\t{by[n]['size']}\n")

total = sum(by[n]["size"] for n in fetch)
print(f"fetch set: {len(fetch)} packages, {total:,} bytes", flush=True)

ok = fail = skipped = 0
failed_names = []
t0 = time.time()
for i, n in enumerate(fetch):
    ext = ".wwpkg" if n.startswith("audio_") else ".bundle"
    path = os.path.join(OUT, n + ext)
    want = by[n]["size"]
    if os.path.exists(path) and os.path.getsize(path) == want:
        skipped += 1
        continue
    url = f"{ROOT}/{n}{ext}"
    good = False
    for attempt in range(1, 6):
        r = subprocess.run(
            ["curl", "-sS", "--fail", "-o", path, url],
            capture_output=True, text=True, timeout=300,
        )
        if r.returncode == 0 and os.path.exists(path) and os.path.getsize(path) == want:
            good = True
            break
        if os.path.exists(path):
            os.remove(path)
        code = "err"
        m = re.search(r"^(curl:.*?)(\d{3})?$", (r.stderr or "").strip())
        if r.returncode == 22:  # --fail HTTP error; find code in stderr
            mm = re.search(r"(\d{3})", r.stderr or "")
            code = mm.group(1) if mm else "http"
        if code == "403":
            print(f"[{i+1}/{len(fetch)}] 403 FORBIDDEN {n} - stopping this item", flush=True)
            break
        print(f"[{i+1}/{len(fetch)}] attempt {attempt} failed {n}: {(r.stderr or '').strip()[:120]}; sleep 30", flush=True)
        time.sleep(30)
    if good:
        ok += 1
    else:
        fail += 1
        failed_names.append(n)
    if (i + 1) % 50 == 0:
        el = time.time() - t0
        print(f"[{i+1}/{len(fetch)}] ok={ok} skip={skipped} fail={fail} elapsed={el:.0f}s", flush=True)

print(f"DONE ok={ok} skipped_existing={skipped} failed={fail} in {time.time()-t0:.0f}s", flush=True)
if failed_names:
    print("FAILED:", failed_names, flush=True)
