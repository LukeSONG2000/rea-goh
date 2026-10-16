// BattleDriver -- spawns two teams from StreamingAssets/roster.json, runs the
// decoded BattleSim deterministically, then replays the event log on the 3D
// rigs (state-machine animation names from catalog/char_anim_states.json).
//
// P5 squad-battle HUD wiring (acceptance: shots/reference/P5.png):
//   - battle timer counts down 4:49 over real elapsed replay time
//   - controls row: 自动 (log-driven auto), 速度 2x (multiplies replay waits),
//     锁定 (camera follow of the acting unit) -- keys B / C / V
//   - per-unit key chips (player Q/W/E/R/T, enemies 1..n) + buff icon slots
//   - on each player "turn" event the bottom skill bar offers the actor's
//     ui_units.json abilities; Q/W/E or click picks the ability whose NAME is
//     then shown in the turn label + log (effectReference isn't in
//     ui_units.json, so per acceptance the damage stays flat-ATK from the log)
//   - enemy commander portrait top-right (first enemy unit's charui portrait)
using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using SwgohRestored.UI;
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
        readonly Dictionary<string, double> _replayHp = new();   // HP as replayed
        BattleSim _sim;
        readonly BattleHUD _hud = new BattleHUD();
        List<UnitEntry> _uiUnits;
        BattleUnit _commander;                                   // enemy team lead

        // P5 HUD state
        float _speedMult = 1f;      // 速度 2x toggle multiplies replay waits
        bool _autoBattle = true;    // 自动 (log-driven replay is auto by nature)
        bool _lockOn;               // 锁定 -- camera follows the acting unit
        Vector3 _lockOffset;
        GameObject _lastActorRig;
        Action<int> _onSkillPick;
        bool _replayActive;
        float _replayStart;
        const float BattleSeconds = 4f * 60 + 49;   // 4:49

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
            {
                CaptureEnabled = true;
                Application.targetFrameRate = 60;    // deterministic capture pacing
                QualitySettings.vSyncCount = 0;
            }
            StartCoroutine(RunBattle());
        }

        void Update()
        {
            if (_sim != null && Camera.main != null)
                foreach (var u in _sim.Units)
                {
                    // bars follow the REPLAY (per-turn HP), not the final sim
                    // state -- units that die late in the battle stay visible
                    // until their death event is replayed
                    double hp = _replayHp.TryGetValue(u.UnitId, out var h) ? h : u.MaxHp;
                    float frac = u.MaxHp > 0 ? Mathf.Clamp01((float)(hp / u.MaxHp)) : 0f;
                    bool alive = _replayHp.Count == 0 ? u.Alive : hp > 0;
                    Vector3? anchor = _rigs.TryGetValue(u.UnitId, out var rig)
                        ? rig.transform.position + Vector3.up * 2.2f : (Vector3?)null;
                    _hud.UpdateUnit(u, Camera.main, frac, alive, anchor);
                }
            if (_replayActive)
                _hud.UpdateTimer(BattleSeconds - (Time.time - _replayStart));
            if (_lockOn && _lastActorRig != null && Camera.main != null)
            {
                var desired = _lastActorRig.transform.position + _lockOffset;
                Camera.main.transform.position =
                    Vector3.Lerp(Camera.main.transform.position, desired, Time.deltaTime * 2f);
            }
            HandleKeys();
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
                if (Application.isBatchMode) UnityEditor.EditorApplication.Exit(0);
#endif
                Application.Quit(0);
            }
        }

        void HandleKeys()
        {
            if (Input.GetKeyDown(KeyCode.B)) ToggleAuto();
            if (Input.GetKeyDown(KeyCode.V)) ToggleSpeed();
            if (Input.GetKeyDown(KeyCode.C)) ToggleLock();
            if (_onSkillPick != null)
            {
                if (Input.GetKeyDown(KeyCode.Q)) _onSkillPick(0);
                if (Input.GetKeyDown(KeyCode.W)) _onSkillPick(1);
                if (Input.GetKeyDown(KeyCode.E)) _onSkillPick(2);
            }
        }

        void ToggleAuto()
        {
            _autoBattle = !_autoBattle;
            _hud.SetControlState("auto", _autoBattle);
            Debug.Log($"[BattleHUD] 自动 = {_autoBattle}");
        }

        void ToggleSpeed()
        {
            _speedMult = _speedMult > 1f ? 1f : 2f;
            _hud.SetControlState("speed", _speedMult > 1f);
            Debug.Log($"[BattleHUD] 倍速 = {_speedMult}x");
        }

        void ToggleLock()
        {
            _lockOn = !_lockOn;
            _hud.SetControlState("lock", _lockOn);
            if (_lockOn && Camera.main != null && _lastActorRig != null)
                _lockOffset = Camera.main.transform.position - _lastActorRig.transform.position;
            Debug.Log($"[BattleHUD] 锁定 = {_lockOn}");
        }

        // ScreenCapture.CaptureScreenshot is unreliable in batchmode; render
        // the camera into a RenderTexture and write the bytes ourselves.
        void CaptureShot(string path)
        {
            var cam = Camera.main;
            if (cam == null) { Debug.LogError("[Capture] no camera"); return; }
            _hud.SetCamera(cam);   // overlay canvas won't render into cam.Render()
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
            _uiUnits = UiData.LoadUnits();

            _sim = new BattleSim(player, opponent, Seed);
            foreach (var u in player.Concat(opponent))
            {
                _replayHp[u.UnitId] = u.MaxHp;
                _hud.TrackUnit(u, Camera.main, ChipLetter(u), BuffIcons(u.Side), BuffTints(u.Side));
            }
            BuildP5Chrome(opponent.Count > 0 ? opponent[0] : null);

            int guard = 0;
            while (!_sim.CheckEnd(out _) && guard++ < 500)
            {
                var actor = _sim.ProcessUnitsSpeed();
                if (actor == null) break;
                _sim.RunTurn(actor);
            }
            bool ended = _sim.CheckEnd(out int winnerSide);
            Debug.Log($"[BattleSim] simulated {_sim.Log.Count} events, guard={guard}, " +
                      $"winnerSide={winnerSide} (0=player,1=opponent,-1=unfinished)");
            // v0 sim crits nearly always (base crit chance data not hydrated),
            // so sample the "crit" kind too
            foreach (var ev in _sim.Log.Where(e => e.Kind == "damage" || e.Kind == "crit").Take(4))
                Debug.Log($"[BattleSim] sample dmg {ev.Actor}->{ev.Target} = {ev.Amount}");
            foreach (var u in player.Concat(opponent))
                Debug.Log($"[BattleSim] {u.BaseId} hp={u.CurrentHp:0} atk={u.GetStat(6)} spd={u.Speed}");

            yield return Replay();
        }

        // ---- P5 chrome: timer, controls row, commander portrait ------------
        void BuildP5Chrome(BattleUnit commander)
        {
            _hud.BuildTimer();
            _hud.BuildControls(ToggleAuto, ToggleSpeed, ToggleLock);
            _hud.SetControlState("auto", true);
            _hud.SetControlState("speed", false);
            _hud.SetControlState("lock", false);
            _commander = commander;
            if (commander != null)
            {
                var entry = EntryOf(commander.BaseId);
                _hud.BuildCommander(entry, entry != null ? entry.name : commander.BaseId);
            }
        }

        string ChipLetter(BattleUnit u)
        {
            // P5: player team uses the Q/W/E/R/T hotkey pool, enemies are plain
            const string pool = "QWERT";
            return u.Side == 0 ? pool[SlotOf(u) % pool.Length].ToString() : (SlotOf(u) + 1).ToString();
        }

        int SlotOf(BattleUnit u)
        {
            int i = u.UnitId.LastIndexOf('_');
            return i >= 0 && int.TryParse(u.UnitId.Substring(i + 1), out var s) ? s : 0;
        }

        // Original buff/debuff icon crops; white silhouettes tinted like NGUI did
        static readonly string[] PlayerBuffs = { "indicator_buff", "icon_buff_attack", "icon_buff_speed" };
        static readonly string[] EnemyBuffs = { "indicator_debuff", "icon_debuff_taunt", "icon_debuff_disarm" };
        static readonly Color BuffGreen = new(0.36f, 0.92f, 0.42f);
        static readonly Color DebuffRed = new(1f, 0.42f, 0.36f);

        static string[] BuffIcons(int side) => side == 0 ? PlayerBuffs : EnemyBuffs;

        static Color[] BuffTints(int side) => side == 0
            ? new[] { Color.white, BuffGreen, BuffGreen }
            : new[] { Color.white, Color.white, DebuffRed };

        UnitEntry EntryOf(string defId) =>
            _uiUnits != null ? _uiUnits.Find(x => x.def_id == defId) : null;

        BattleUnit UnitOf(BattleEvent ev) =>
            _unitsById.Values.FirstOrDefault(u => u.BaseId == ev.Actor && ev.ActorSide == (u.Side == 0 ? "player" : "opponent"))
            ?? _unitsById.Values.FirstOrDefault(u => u.BaseId == ev.Actor || u.UnitId == ev.Target)
            ?? _unitsById.Values.FirstOrDefault(u => u.BaseId == ev.Target);

        /// log events carry baseIds; sim units are keyed by UnitId
        string TargetUnitId(string baseId) =>
            _unitsById.Values.FirstOrDefault(u => u.BaseId == baseId)?.UnitId;

        // ------------------------------------------------------------------
        // Replay with P5 pacing: speed multiplier, ability choice windows and
        // capture-frame alignment (capture frames land inside player turns so
        // the skill bar is on screen at 240/480/800).
        // ------------------------------------------------------------------
        IEnumerator Replay()
        {
            _replayStart = Time.time;
            _replayActive = true;
            var waits = ComputeWaits(_sim.Log);
            for (int i = 0; i < _sim.Log.Count; i++)
            {
                var ev = _sim.Log[i];
                float w = waits[i] / _speedMult;
                switch (ev.Kind)
                {
                    case "turn":
                        yield return TurnStep(ev, w);
                        break;
                    case "damage":
                    case "crit":
                        var tid = TargetUnitId(ev.Target);
                        if (tid != null && _replayHp.TryGetValue(tid, out var hp))
                        {
                            hp = Math.Max(0, hp - ev.Amount);
                            _replayHp[tid] = hp;
                            if (_commander != null && tid == _commander.UnitId && _commander.MaxHp > 0)
                                _hud.UpdateCommander((float)(hp / _commander.MaxHp));
                        }
                        var go = RigOf(ev.Target);
                        if (go) StartCoroutine(Flash(go));
                        PlayClip(ev.Target, "Hit");
                        _hud.ShowDamage(ev.Target, ev.Amount, ev.Kind == "crit", Camera.main);
                        yield return new WaitForSeconds(w);
                        break;
                    case "death":
                        var dtid = TargetUnitId(ev.Target);
                        if (dtid != null) _replayHp[dtid] = 0;
                        if (_commander != null && dtid == _commander.UnitId) _hud.UpdateCommander(0);
                        PlayClip(ev.Target, "Death");
                        yield return new WaitForSeconds(w);
                        break;
                }
            }
            _replayActive = false;
            _hud.HideSkillBar();
            Debug.Log("[BattleDriver] replay finished");
        }

        IEnumerator TurnStep(BattleEvent ev, float wait)
        {
            var actor = UnitOf(ev);
            string baseId = actor?.BaseId ?? ev.Actor;
            var entry = EntryOf(baseId);
            var abilities = BattleHUD.ActiveAbilities(entry);
            string actorName = entry != null ? entry.name : baseId;
            int chosen = 0;
            bool isPlayer = ev.ActorSide == "player";
            var rig = RigOf(baseId);
            if (rig != null) _lastActorRig = rig;

            void SetLabel()
            {
                string abName = abilities.Count > 0 ? abilities[Mathf.Clamp(chosen, 0, abilities.Count - 1)].name : "攻击";
                _hud.SetTurnText($"第{ev.Turn}回合 · {actorName} · {abName}");
            }

            if (abilities.Count > 0)
            {
                if (isPlayer)
                {
                    // pause window: Q/W/E or click picks the ability for the turn
                    _onSkillPick = idx =>
                    {
                        if (idx < abilities.Count) { chosen = idx; _hud.HighlightSkill(idx); SetLabel(); }
                    };
                    _hud.ShowSkillBar(entry, $"{actorName} ▶ 选择技能", _onSkillPick, showKeys: true);
                }
                else
                {
                    chosen = 0;   // enemy side auto-picks (basic)
                    _hud.ShowSkillBar(entry, $"{actorName} · 敌方行动", null, showKeys: false);
                }
            }
            SetLabel();
            var used = abilities.Count > 0 ? abilities[Mathf.Clamp(chosen, 0, abilities.Count - 1)] : null;
            Debug.Log($"[BattleDriver] turn {ev.Turn} {ev.ActorSide} {actorName} uses {(used != null ? used.name + " (" + used.id + ")" : "basic attack")}");
            PlayClip(baseId, "Attack", "Idle");
            // 自动 off = manual pacing: player turns linger for a key/click
            if (isPlayer && !_autoBattle) wait *= 2.5f;
            yield return new WaitForSeconds(wait);
            _onSkillPick = null;
        }

        static float BaseWait(BattleEvent ev)
        {
            switch (ev.Kind)
            {
                case "turn": return ev.ActorSide == "player" ? 1.5f : 0.9f;
                case "death": return 0.8f;
                case "damage":
                case "crit": return 0.4f;
                default: return 0.3f;
            }
        }

        /// Capture-mode pacing: stretch the player-turn window that precedes
        /// each capture frame so 240/480/800 land inside a skill-bar window.
        float[] ComputeWaits(List<BattleEvent> log)
        {
            var waits = new float[log.Count];
            for (int i = 0; i < log.Count; i++) waits[i] = BaseWait(log[i]);
            if (!CaptureEnabled) return waits;

            const float fps = 60f;
            var starts = new float[log.Count + 1];
            float t = 24f;   // frames before the first replay event (build/sim)
            for (int i = 0; i < log.Count; i++) { starts[i] = t; t += waits[i] * fps; }
            starts[log.Count] = t;
            float shift = 0;
            foreach (var f in CaptureFrames)
            {
                int best = -1;
                for (int i = 0; i < log.Count; i++)
                {
                    if (log[i].Kind != "turn" || log[i].ActorSide != "player") continue;
                    if (starts[i] + shift <= f - 18f) best = i;
                }
                if (best < 0) continue;
                float windowEnd = starts[best] + shift + waits[best] * fps;
                float extra = f + 18f - windowEnd;
                if (extra > 0) { waits[best] += extra / fps; shift += extra; }
            }
            Debug.Log($"[Capture] replay pacing aligned to frames {string.Join(",", CaptureFrames)}");
            return waits;
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
