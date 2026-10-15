// BattleDriver -- spawns two teams from StreamingAssets/roster.json, runs the
// decoded BattleSim deterministically, then replays the event log on the 3D
// rigs (state-machine animation names from catalog/char_anim_states.json).
using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using UnityEngine;
#if UNITY_EDITOR
using UnityEditor;
#endif

namespace SwgohRestored.Battle
{
    public class BattleDriver : MonoBehaviour
    {
        [Header("Teams (def_ids) -- leave empty for the default duel")]
        public string[] PlayerTeam = { "JYNERSO", "CASSIANANDOR", "K2SO" };
        public string[] OpponentTeam = { "VADER", "GRANDMOFFTARKIN", "DARTHTRAYA" };
        public long Seed = 4490;

        [Header("Stage")]
        public float SlotSpacing = 2.2f;
        public float TeamGap = 6.0f;
        public GameObject CharacterPrefabRoot;   // imported GLB prefabs live here

        // Batch-mode screenshot harness: PlayModeCapture.Run sets these before
        // play mode starts (edit-mode scene objects don't survive the play-mode
        // domain reload, so the capture loop must live here).
        public static bool CaptureEnabled;
        public static int[] CaptureFrames = { 240, 480, 800 };
        int _captureFrame;

        readonly Dictionary<string, GameObject> _rigs = new();
        readonly Dictionary<string, Animator> _anims = new();
        readonly Dictionary<string, BattleUnit> _unitsById = new();
        BattleSim _sim;
        readonly BattleHUD _hud = new BattleHUD();

        [Serializable]
        class StatKV { public string k; public double v; }

        [Serializable]
        class RosterEntry
        {
            public string def_id, name, folder;
            public bool anim;
            public List<StatKV> stats;
        }

        void Start()
        {
            // statics don't survive the play-mode domain reload -- the batch
            // harness signals us with a marker file instead
            if (File.Exists(@"F:\PPfavorite\Work\goh\unity\capture.marker"))
                CaptureEnabled = true;
            StartCoroutine(RunBattle());
        }

        void Update()
        {
            if (_sim != null && Camera.main != null)
                foreach (var u in _sim.Units) _hud.UpdateUnit(u, Camera.main);
            if (!CaptureEnabled) return;
            _captureFrame++;
            foreach (var f in CaptureFrames)
                if (_captureFrame == f)
                {
                    CaptureShot($"F:/PPfavorite/Work/goh/unity/shot_{f}.png");
                    Debug.Log($"[Capture] frame {_captureFrame} -> shot_{f}.png");
                }
            if (_captureFrame >= CaptureFrames[CaptureFrames.Length - 1] + 10)
            {
                Debug.Log($"[Capture] done at frame {_captureFrame}");
#if UNITY_EDITOR
                EditorApplication.isPlaying = false;
#endif
                Application.Quit(0);
            }
        }

        // ScreenCapture.CaptureScreenshot is unreliable in batchmode; render
        // the camera into a RenderTexture and write the bytes ourselves.
        void CaptureShot(string path)
        {
            var cam = Camera.main;
            if (cam == null) { Debug.LogError("[Capture] no camera"); return; }
            _hud.SetCamera(cam);   // overlay canvas won't render into cam.Render()
            var diag = _anims.Values.FirstOrDefault();
            if (diag != null)
                Debug.Log($"[Capture@{path}] animator ctrl={(diag.runtimeAnimatorController ? diag.runtimeAnimatorController.name : "NULL")} " +
                          $"stateInfohash={diag.GetCurrentAnimatorStateInfo(0).shortNameHash} len={diag.GetCurrentAnimatorStateInfo(0).length:0.00}");
            var prevRT = cam.targetTexture;
            var rt = new RenderTexture(1280, 720, 24);
            cam.targetTexture = rt;
            cam.Render();
            RenderTexture.active = rt;
            var tex = new Texture2D(1280, 720, TextureFormat.RGB24, false);
            tex.ReadPixels(new Rect(0, 0, 1280, 720), 0, 0);
            tex.Apply();
            cam.targetTexture = prevRT;
            RenderTexture.active = null;
            System.IO.File.WriteAllBytes(path, tex.EncodeToPNG());
            Destroy(tex);
            Destroy(rt);
        }

        IEnumerator RunBattle()
        {
            var roster = LoadRoster();
            var player = BuildTeam(roster, PlayerTeam, 0);
            var opponent = BuildTeam(roster, OpponentTeam, 1);

            _sim = new BattleSim(player, opponent, Seed);
            foreach (var u in player.Concat(opponent)) _hud.TrackUnit(u, Camera.main);
            int guard = 0;
            while (!_sim.CheckEnd(out _) && guard++ < 500)
            {
                var actor = _sim.ProcessUnitsSpeed();
                if (actor == null) break;
                _sim.RunTurn(actor);
            }
            Debug.Log($"[BattleSim] simulated {_sim.Log.Count} events, guard={guard}");
            foreach (var ev in _sim.Log.Where(e => e.Kind == "damage").Take(4))
                Debug.Log($"[BattleSim] sample dmg {ev.Actor}->{ev.Target} = {ev.Amount}");
            foreach (var u in player.Concat(opponent))
                Debug.Log($"[BattleSim] {u.BaseId} hp={u.CurrentHp:0} atk={u.GetStat(6)} spd={u.Speed}");
            var jyn = _anims.Values.FirstOrDefault();
            if (jyn != null)
                Debug.Log($"[AnimDiag] enabled={jyn.enabled} ctrl={(jyn.runtimeAnimatorController ? jyn.runtimeAnimatorController.name : "NULL")} stateHash={Animator.StringToHash("Attack")}");
            else
                Debug.Log("[AnimDiag] no animators registered");

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
                double hpStat = 1000;
                var statMap = new Dictionary<int, double>();
                if (e.stats != null)
                    foreach (var kv in e.stats)
                    {
                        if (int.TryParse(kv.k, out var s)) statMap[s] = kv.v;
                        if (kv.k == "1") hpStat = kv.v;   // MAX_HEALTH
                    }
                var u = new BattleUnit
                {
                    BaseId = e.def_id,
                    UnitId = $"{e.def_id}#{side}_{slot}",
                    Side = side,
                    Level = 85,
                    MaxHp = hpStat,
                    CurrentHp = hpStat,
                };
                foreach (var kv in statMap) u.Stats[kv.Key] = kv.Value;
                _unitsById[u.UnitId] = u;
                SpawnRig(e.folder, side, slot, u);
                list.Add(u);
                slot++;
            }
            return list;
        }

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
            go.name = u.UnitId;        // BattleHUD resolves bars by UnitId
            go.SetActive(true);   // staged prefabs park inactive
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
                        _hud.SetTurnText($"Round {_sim.Round}  ·  {ev.Actor}");
                        PlayClip(ev.Actor, "Attack", "Idle");
                        yield return new WaitForSeconds(1.1f);
                        break;
                    case "damage":
                    case "crit":
                        var go = RigOf(ev.Target);
                        if (go) StartCoroutine(Flash(go));
                        PlayClip(ev.Target, "Hit");
                        _hud.ShowDamage(ev.Target, ev.Amount, ev.Kind == "crit", Camera.main);
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
