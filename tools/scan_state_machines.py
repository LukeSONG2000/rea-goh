"""Scan every character bundle's AnimatorController STATE MACHINE -- the
game's authoritative action table, replacing the flat PPtr sweep in
scan_anim_bindings.py (whose output had arbitrary order and unresolved
external names, so the viewer guessed wrong actions).

Runtime controller format (Unity 2022, verified on char_jynerso_pre):
  m_TOS                          [(hash, "Layer -> State" path), ...]
  m_AnimationClips               global PPtr table; blend nodes index into it
  m_Controller.m_LayerArray      [{data: {m_StateMachineIndex, ...}}]
  m_Controller.m_StateMachineArray[i].data:
      m_DefaultState             default state INDEX (matches AnyState dests)
      m_StateConstantArray[j].data:
          m_NameID / m_FullPathID  -> m_TOS lookup for the state name
          m_PathID                 direct motion PPtr (clip) when no blend tree
          m_Loop / m_Speed
          m_BlendTreeConstantArray[{m_NodeArray: [...]}]
              node.m_ClipID        index into m_AnimationClips (0xFFFFFFFF=none)
              node.m_ChildIndices + parent m_Blend1dData.m_ChildThresholdArray

Output: catalog/char_anim_states.json
  { char: {"controller": name,
           "states": [{"name","layer","default","loop","speed",
                       "motion": [{"file_id","path_id","threshold","source"}]}]} }

Usage:  py tools/scan_state_machines.py [N ...]   (limit for a smoke run)
"""
import glob, json, os, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, 'tools'))
import UnityPy

BUNDLE_ROOT = r"C:\Users\PPfavorite\AppData\LocalLow\Unity\Electronic Arts_SWGoH"
NONE32 = 0xFFFFFFFF


def latest_bundle(name):
    paths = glob.glob(os.path.join(BUNDLE_ROOT, name, "*", "__data"))
    if not paths:
        return None
    return max(paths, key=os.path.getmtime)


def clamp_thresholds(nodes):
    """Parent node i owns thresholds for its m_ChildIndices children.
    Writes into each child's `data` dict (the scan loop reads there)."""
    for parent in nodes:
        p = parent["data"]
        kids = p.get("m_ChildIndices") or []
        thr = (p.get("m_Blend1dData", {}).get("data", {})
               .get("m_ChildThresholdArray")) or []
        for k, t in zip(kids, thr):
            if 0 <= k < len(nodes):
                nodes[k]["data"]["_threshold"] = t


def scan_char(char):
    path = latest_bundle(char)
    if not path:
        return None
    env = UnityPy.load(path)

    clips_in = {}
    controllers = []
    for obj in env.objects:
        if obj.type.name == "AnimationClip":
            try:
                clips_in[obj.path_id] = obj.read_typetree()["m_Name"]
            except Exception:
                pass
        elif obj.type.name == "AnimatorController":
            controllers.append(obj)
    if not controllers:
        return None
    ctrl = controllers[0]
    tt = ctrl.read_typetree()
    paths = dict(tt.get("m_TOS") or [])
    clip_table = []
    for e in tt.get("m_AnimationClips") or []:
        d = e.get("data", e) if isinstance(e, dict) else e
        if isinstance(d, dict):
            clip_table.append((d.get("m_FileID", 0), d.get("m_PathID", 0)))

    af = env.file
    externals = []
    if af is not None and getattr(af, "externals", None):
        externals = [os.path.splitext(os.path.basename(e.path))[0]
                     for e in af.externals]

    def motion_entry(fid, pid, threshold=None):
        if pid == 0:
            return None
        if fid == 0:
            if pid not in clips_in:
                return None      # runtime blend-tree PPtr, not a real asset
            return {"file_id": 0, "path_id": pid, "source": "char_bundle_local",
                    "name": clips_in[pid], **({"threshold": round(threshold, 6)}
                                              if threshold is not None else {})}
        src = externals[fid - 1] if 0 < fid <= len(externals) else f"ext{fid}"
        ent = {"file_id": fid, "path_id": pid, "source": src, "name": None}
        if threshold is not None:
            ent["threshold"] = round(threshold, 6)
        return ent

    states_out = []
    layers = tt["m_Controller"]["m_LayerArray"]
    machines = tt["m_Controller"]["m_StateMachineArray"]
    for layer in layers:
        ld = layer["data"]
        sm = machines[ld["m_StateMachineIndex"]]["data"]
        default = sm.get("m_DefaultState", -1)
        for si, sconst in enumerate(sm.get("m_StateConstantArray") or []):
            sd = sconst["data"]
            name = (paths.get(sd.get("m_FullPathID"))
                    or paths.get(sd.get("m_NameID"))
                    or f"State_{si}")
            motions = []
            # m_PathID is a bare runtime PPtr path_id with implied file_id 0;
            # motion_entry returns None for it when it's a blend-tree ref.
            direct = motion_entry(0, sd.get("m_PathID", 0))
            if direct is not None:
                motions.append(direct)
            for btwrap in sd.get("m_BlendTreeConstantArray") or []:
                nodes = btwrap["data"].get("m_NodeArray") or []
                clamp_thresholds(nodes)
                for node in nodes:
                    nd = node["data"]
                    cid = nd.get("m_ClipID", NONE32)
                    if cid == NONE32 or not (0 <= cid < len(clip_table)):
                        continue
                    mf, mp = clip_table[cid]
                    ent = motion_entry(mf, mp, nd.get("_threshold"))
                    if ent:
                        motions.append(ent)
            if not motions:
                continue
            states_out.append({
                "name": name,
                "layer": ld["m_StateMachineIndex"],
                "default": si == default,
                "loop": bool(sd.get("m_Loop")),
                "speed": sd.get("m_Speed", 1.0),
                "motion": motions,
            })

    if not states_out:
        return None
    return {
        "controller": tt.get("m_Name") or ctrl.read_typetree().get("m_Name"),
        "states": states_out,
    }


def main():
    chars = sorted(os.path.basename(p)
                   for p in glob.glob(os.path.join(BUNDLE_ROOT, "char_*_pre")))
    if len(sys.argv) > 1:
        if sys.argv[1].isdigit():
            chars = chars[:int(sys.argv[1])]
        else:
            keys = sys.argv[1:]
            chars = [c for c in chars if any(k in c for k in keys)]
    out_path = os.path.join(ROOT, "catalog", "char_anim_states.json")
    try:
        out = json.load(open(out_path, encoding="utf-8"))
    except Exception:
        out = {}
    for i, c in enumerate(chars):
        try:
            r = scan_char(c)
            if r:
                out[c] = r
            else:
                print(f"{c}: no states", flush=True)
        except Exception as e:
            print(f"{c}: ERROR {e}", flush=True)
        if (i + 1) % 50 == 0:
            print(f"[{i+1}/{len(chars)}] with states: {len(out)}", flush=True)
    with open(out_path, "w", encoding="utf-8") as f:
        json.dump(out, f, ensure_ascii=False, indent=1)
    n_states = sum(len(v["states"]) for v in out.values())
    n_named = sum(1 for v in out.values() for s in v["states"]
                  if s["name"] != f"State_{v['states'].index(s)}")
    print(f"done: {len(out)}/{len(chars)} chars, {n_states} states -> {out_path}")


if __name__ == "__main__":
    main()
