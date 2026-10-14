"""Scan every character bundle for its AnimatorController and record the
clips it actually references -- the game's own binding, not a heuristic.

For each char_*_pre bundle:
  * find AnimatorController objects, sweep their typetrees for PPtrs
  * resolve each PPtr: in-bundle AnimationClip -> "char_bundle_local";
    external (fileID) -> match external name (shared_animationcontainer etc.)
  * record controller name + ordered unique clip list

Output: catalog/char_anim_bindings.json
  { char: {"controller": name,
           "clips": [{"name","source","path_id","muscle":bool}]} }

Usage:  py tools/scan_anim_bindings.py [N ...]   (limit to first N chars for a smoke run)
"""
import glob, json, os, struct, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, 'tools'))
import UnityPy

BUNDLE_ROOT = r"C:\Users\PPfavorite\AppData\LocalLow\Unity\Electronic Arts_SWGoH"


def latest_bundle(name):
    paths = glob.glob(os.path.join(BUNDLE_ROOT, name, "*", "__data"))
    if not paths:
        return None
    return max(paths, key=os.path.getmtime)


def walk_pptrs(value, out):
    """Recursively collect every PPtr {m_FileID, m_PathID} in a typetree."""
    if isinstance(value, dict):
        if set(value.keys()) >= {"m_FileID", "m_PathID"} and isinstance(value["m_PathID"], int):
            out.append((value["m_FileID"], value["m_PathID"]))
            return
        for v in value.values():
            walk_pptrs(v, out)
    elif isinstance(value, list):
        for v in value:
            walk_pptrs(v, out)


def scan_char(char):
    path = latest_bundle(char)
    if not path:
        return None
    env = UnityPy.load(path)

    # in-bundle clips + controllers
    clips_in = {}     # path_id -> name
    controllers = []
    for obj in env.objects:
        if obj.type.name == "AnimationClip":
            try:
                clips_in[obj.path_id] = obj.read_typetree()["m_Name"]
            except Exception:
                pass
        elif obj.type.name == "AnimatorController":
            controllers.append(obj)

    # external assets file names (for shared bundle resolution)
    externals = []
    af = env.file
    if af is not None and getattr(af, "externals", None):
        externals = [os.path.splitext(os.path.basename(e.path))[0] for e in af.externals]

    def resolve(fid, pid):
        if pid == 0:
            return None
        if fid == 0:
            if pid in clips_in:
                return {"name": clips_in[pid], "source": "char_bundle_local", "path_id": pid}
            return None
        src = externals[fid - 1] if 0 < fid <= len(externals) else f"ext{fid}"
        # clip name unknown from here; muscle_clip parse gives m_Name later,
        # keep catalog name lookup cheap: return source+path_id only
        return {"name": None, "source": src, "path_id": pid}

    result = []
    for ctrl in controllers:
        try:
            tt = ctrl.read_typetree()
        except Exception:
            continue
        pptrs = []
        walk_pptrs(tt, pptrs)
        refs, seen = [], set()
        for fid, pid in pptrs:
            r = resolve(fid, pid)
            if not r or (r["source"], r["path_id"]) in seen:
                continue
            seen.add((r["source"], r["path_id"]))
            refs.append(r)
        if refs:
            result.append({"controller": tt.get("m_Name", "?"), "clips": refs})
    return result or None


def main():
    import re
    chars = sorted(os.path.basename(p) for p in glob.glob(os.path.join(BUNDLE_ROOT, "char_*_pre")))
    if len(sys.argv) > 1:
        chars = chars[:int(sys.argv[1])]
    out = {}
    for i, c in enumerate(chars):
        try:
            r = scan_char(c)
            if r:
                out[c] = r
        except Exception as e:
            print(f"{c}: ERROR {e}", flush=True)
        if (i + 1) % 50 == 0:
            print(f"[{i+1}/{len(chars)}] bound so far: {len(out)}", flush=True)
    dst = os.path.join(ROOT, "catalog", "char_anim_bindings.json")
    with open(dst, "w", encoding="utf-8") as f:
        json.dump(out, f, ensure_ascii=False, indent=1)
    n_clips = sum(len(k["clips"]) for v in out.values() for k in v)
    print(f"done: {len(out)}/{len(chars)} chars have controllers, "
          f"{n_clips} clip refs -> {dst}")


if __name__ == "__main__":
    main()
