// BattleDriver -- spawns two teams from StreamingAssets/roster.json, runs the
// decoded BattleSim deterministically, then replays the event log on the 3D
// rigs (state-machine animation names from catalog/char_anim_states.json).
using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using UnityEngine;

namespace SwgohRestored.Battle
{
    public class BattleDriver : MonoBehaviour
    {
        [Header("Teams (def_ids) -- leave empty for the default duel")]
        public string[] PlayerTeam = { "JYNERSO" };
        public string[] OpponentTeam = { "VADER" };
        public long Seed = 4490;

        [Header("Stage")]
        public float SlotSpacing = 2.2f;
        public float TeamGap = 6.0f;
        public GameObject CharacterPrefabRoot;   // imported GLB prefabs live here

        readonly Dictionary<string, GameObject> _rigs = new();
        readonly Dictionary<string, Animator> _anims = new();
        readonly Dictionary<string, BattleUnit> _unitsById = new();
        BattleSim _sim;

        [Serializable]
        class RosterEntry
        {
            public string def_id, name, folder;
            public bool anim;
            public Dictionary<string, double> stats;
        }

        void Start() { StartCoroutine(RunBattle()); }

        IEnumerator RunBattle()
        {
            var roster = LoadRoster();
            var player = BuildTeam(roster, PlayerTeam, 0);
            var opponent = BuildTeam(roster, OpponentTeam, 1);

            _sim = new BattleSim(player, opponent, Seed);
            int guard = 0;
            while (!_sim.CheckEnd(out _) && guard++ < 500)
            {
                var actor = _sim.ProcessUnitsSpeed();
                if (actor == null) break;
                _sim.RunTurn(actor);
            }
            Debug.Log($"[BattleSim] simulated {_sim.Log.Count} events, guard={guard}");

            yield return Replay();
        }

        List<RosterEntry> LoadRoster()
        {
            var path = Path.Combine(Application.streamingAssetsPath, "roster.json");
            var json = File.ReadAllText(path);
            return JsonUtilityArray<RosterEntry>.FromJson(json);
        }

        List<BattleUnit> BuildTeam(List<RosterEntry> roster, string[] ids, int side)
        {
            var byDef = roster.ToDictionary(r => r.def_id, r => r);
            var list = new List<BattleUnit>();
            int slot = 0;
            foreach (var id in ids)
            {
                if (!byDef.TryGetValue(id, out var e))
                {
                    Debug.LogWarning($"roster miss: {id}");
                    continue;
                }
                var u = new BattleUnit
                {
                    BaseId = e.def_id,
                    UnitId = $"{e.def_id}#{side}_{slot}",
                    Side = side,
                    Level = 85,
                    MaxHp = e.stats.TryGetValue(((int)Stat2(1)).ToString(), out var hp) ? hp : 1000,
                    CurrentHp = e.stats.TryGetValue(((int)Stat2(1)).ToString(), out var hp2) ? hp2 : 1000,
                };
                foreach (var kv in e.stats) u.Stats[int.Parse(kv.Key)] = kv.Value;
                _unitsById[u.UnitId] = u;
                SpawnRig(e.folder, side, slot, u);
                list.Add(u);
                slot++;
            }
            return list;
        }

        static int Stat2(int v) => v;  // MAX_HEALTH = 1 (runtime enum)

        void SpawnRig(string folder, int side, int slot, BattleUnit u)
        {
            if (CharacterPrefabRoot == null || string.IsNullOrEmpty(folder)) return;
            var t = CharacterPrefabRoot.transform.Find(folder);
            if (t == null)
            {
                Debug.LogWarning($"rig not imported: {folder}");
                return;
            }
            var go = Instantiate(t.gameObject, transform);
            float x = (side == 0 ? -1 : 1) * TeamGap;
            float z = (slot - 2) * SlotSpacing;
            go.transform.position = new Vector3(x, 0, z);
            go.transform.rotation = Quaternion.LookRotation(side == 0 ? Vector3.right : Vector3.left);
            _rigs[u.UnitId] = go;
            _anims[u.UnitId] = go.GetComponentInChildren<Animator>();
        }

        IEnumerator Replay()
        {
            foreach (var ev in _sim.Log)
            {
                switch (ev.Kind)
                {
                    case "turn":
                        PlayClip(ev.Actor, "Attack", "Idle");
                        yield return new WaitForSeconds(1.1f);
                        break;
                    case "damage":
                    case "crit":
                        var go = RigOf(ev.Target);
                        if (go) StartCoroutine(Flash(go));
                        PlayClip(ev.Target, "Hit");
                        yield return new WaitForSeconds(0.4f);
                        break;
                    case "death":
                        PlayClip(ev.Target, "Death");
                        yield return new WaitForSeconds(0.8f);
                        break;
                }
            }
            Debug.Log("[BattleDriver] replay finished");
        }

        GameObject RigOf(string baseId) =>
            _rigs.FirstOrDefault(kv => kv.Key.StartsWith(baseId + "#")).Value;

        void PlayClip(string baseId, string clip, string fallback = null)
        {
            var rig = RigOf(baseId);
            var anim = rig ? rig.GetComponentInChildren<Animator>() : null;
            if (anim == null || !anim.isActiveAndEnabled) return;
            if (anim.HasState(0, Animator.StringToHash(clip)))
                anim.Play(clip, 0, 0f);
            else if (fallback != null && anim.HasState(0, Animator.StringToHash(fallback)))
                anim.Play(fallback, 0, 0f);
        }

        IEnumerator Flash(GameObject go)
        {
            foreach (var r in go.GetComponentsInChildren<Renderer>())
                foreach (var m in r.materials) m.EnableKeyword("_EMISSION");
            yield return new WaitForSeconds(0.12f);
            foreach (var r in go.GetComponentsInChildren<Renderer>())
                foreach (var m in r.materials) m.DisableKeyword("_EMISSION");
        }
    }

    // JsonUtility lacks top-level array support; wrap it.
    static class JsonUtilityArray<T>
    {
        [Serializable]
        class Wrapper { public T[] items; }
        public static List<T> FromJson(string json)
        {
            var wrapped = "{\"items\":" + json + "}";
            return JsonUtility.FromJson<Wrapper>(wrapped).items.ToList();
        }
    }
}
