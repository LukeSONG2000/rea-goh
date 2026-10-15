# -*- coding: utf-8 -*-
"""char_rancor_jabba_gl_pre: the one char whose clips are muscle-format but
whose bundle Avatar is a placeholder (m_HumanBoneIndex all -1, empty
skeleton), so the v2 muscle route refuses it.  The rig itself is humanoid-
SHAPED with the standard hmn_ leaf names (pelvicJNT/spine1JNT/spine2JNT/
neckJNT/headJNT/jawJNT + L/R clav/shldr/elbw/wrst) minus legs.

Fix: synthesize the humanoid table by borrowing the axes (preQ/postQ/sgn/
limits) from a standard donor avatar (char_jynerso_pre -- same rig naming
family) and re-anchoring each mapped bone to the rancor TOS by leaf name;
bones the rancor lacks stay -1 and their muscles are simply never routed.

Run:  py tools/export_rancor_jabba.py
Out:  extracted/char_rancor_jabba_gl_pre/char_rancor_jabba_gl_pre.anim.glb
"""
import json
import os
import sys

TOOLS = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(TOOLS)
sys.path.insert(0, TOOLS)

import UnityPy

import export_animation_v2 as v2
from export_animation import (
    HUMAN_ID_BONES, latest_bundle, load_avatar, build_joint_table)
from scan_anim_bindings import scan_char

CHAR = "char_rancor_jabba_gl_pre"
DONOR = "char_jynerso_pre"


def synthetic_avatar(char_env, donor_av):
    """Donor humanoid table re-anchored to the rancor skeleton by leaf name."""
    rancor_av = next(o.read_typetree() for o in char_env.objects
                     if o.type.name == "Avatar")
    # rancor leaf -> path-hash (m_TOS hashes the Animator-relative path)
    leaf_hash = {}
    for h, p in rancor_av["m_TOS"]:
        if p:
            leaf_hash.setdefault(p.split("/")[-1], int(h))

    hu = donor_av["m_Avatar"]["m_Human"]["data"]
    skel = hu["m_Skeleton"]["data"]
    donor_tos = {int(h): p for h, p in donor_av["m_TOS"]}
    axes_arr = skel["m_AxesArray"]

    nodes, ids, axes_out = [], [], []
    axes_remap = {}                      # donor axes id -> new index
    bone_index = []
    for hid, n in enumerate(hu["m_HumanBoneIndex"]):
        if n < 0 or n >= len(skel["m_Node"]):
            bone_index.append(-1)
            continue
        leaf = donor_tos.get(skel["m_ID"][n], "/").split("/")[-1]
        new_id = leaf_hash.get(leaf)
        if new_id is None:               # bone absent from the rancor rig
            bone_index.append(-1)
            continue
        d_ax = skel["m_Node"][n]["m_AxesId"]
        if d_ax not in axes_remap:
            axes_remap[d_ax] = len(axes_out)
            axes_out.append(axes_arr[d_ax])
        nodes.append({"m_AxesId": axes_remap[d_ax]})
        ids.append(new_id)
        bone_index.append(len(nodes) - 1)

    av = json.loads(json.dumps(donor_av))   # deep copy, keep m_HumanDescription
    av["m_TOS"] = rancor_av["m_TOS"]
    av["m_Avatar"]["m_Human"]["data"] = {
        "m_HumanBoneIndex": bone_index,
        "m_Skeleton": {"data": {"m_Node": nodes, "m_ID": ids,
                                "m_AxesArray": axes_out}},
    }
    return av


def main():
    char_env = UnityPy.load(latest_bundle(CHAR))
    donor_av = load_avatar(UnityPy.load(latest_bundle(DONOR)))
    syn = synthetic_avatar(char_env, donor_av)
    joints = build_joint_table(syn)
    print("synthetic joints:", {b: r["leaf"] for b, r in sorted(joints.items())})

    entry = scan_char(CHAR)
    refs = [r for k in entry for r in k["clips"]] if entry else []
    print(f"{len(refs)} clip refs from controller")

    v2.load_avatar = lambda env: syn     # route the muscle path through syn
    shared_names = v2.shared_clip_names()
    status, n, total = v2.export_char_v2(CHAR, refs, shared_names, force=True)
    print(f"export: {status}, {n} clips, {total} bytes")


if __name__ == "__main__":
    main()
