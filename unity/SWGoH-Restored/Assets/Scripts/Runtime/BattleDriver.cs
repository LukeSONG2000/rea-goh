// Incremental command-driven battle presentation. Native manual-selection entry
// points are documented in the recovery ledger; this Unity bridge is new code.
// Original ordered effect graphs execute only the recovered operator subset.
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
        public string[] PlayerTeam = { "VADER" };
        public string[] OpponentTeam = { "VADER", "VADER" };
        public long Seed = 8; // Explicit demo encounter seed; original rules/stats unchanged.

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
        readonly Dictionary<string, Dictionary<long, RawEffect>> _replayStates = new();
        OriginalEffectGraph _originalGraph;
        readonly Dictionary<string, double> _replayShield = new();
        readonly Dictionary<string, OriginalUnitMenu> _originalMenus = new();
        BattleSim _sim;
        BattleStage _stage;
        public BattleStage Stage => _stage;
        readonly BattleHUD _hud = new BattleHUD();
        List<UnitEntry> _uiUnits;
        BattleUnit _commander;                                   // enemy team lead

        // P5 HUD state
        float _speedMult = 1f;      // 速度 2x toggle multiplies replay waits
        bool _autoBattle;           // Manual commands are the default.
        bool _lockOn;               // 锁定 -- camera follows the acting unit
        Vector3 _lockOffset;
        GameObject _lastActorRig;
        Action<int> _onSkillPick;
        bool _waitingForCommand;
        int _selectedSkill = int.MinValue;
        IReadOnlyList<BattleUnit> _legalTargets = Array.Empty<BattleUnit>();
        public bool WaitingForCommand => _waitingForCommand;
        public BattleSim Simulation => _sim;
        public OriginalAbilityPresentation ActivePresentation { get; private set; }
        public string PresentationFailure { get; private set; }
        readonly Dictionary<string, Newtonsoft.Json.Linq.JObject> _presentationData = new();
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
            // Imported scene instances are templates, not combatants. Reimporting
            // a GLB can restore its authored active state; hide the whole library.
            if (CharacterPrefabRoot) CharacterPrefabRoot.SetActive(false);
            // statics don't survive the play-mode domain reload -- the batch
            // harness signals us with a marker file instead.  One-shot: consume
            // it so a shipped player never self-captures + quits on a stale marker.
            const string marker = @"F:\PPfavorite\Work\goh\unity\capture.marker";
            if (File.Exists(marker))
            {
                CaptureEnabled = true;
                try { File.Delete(marker); } catch { /* editor harness owns it */ }
                Application.targetFrameRate = 60;    // deterministic capture pacing
                QualitySettings.vSyncCount = 0;
            }
            StartCoroutine(RunBattle());
        }

        void Update()
        {
            RefreshUnitBars();
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
                    CaptureShot($"F:/PPfavorite/Work/goh/logs/rea_gap_review/shot_{f}.png");
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

        void RefreshUnitBars()
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
                    Vector3? anchor = null;
                    if (_rigs.TryGetValue(u.UnitId, out var rig))
                    {
                        // Particle bounds can span the arena; bars track the body.
                        var renderers = rig.GetComponentsInChildren<Renderer>()
                            .Where(r => r.enabled && (r is SkinnedMeshRenderer || r is MeshRenderer)).ToArray();
                        if (renderers.Length > 0)
                        {
                            var bounds = renderers[0].bounds;
                            foreach (var renderer in renderers.Skip(1)) bounds.Encapsulate(renderer.bounds);
                            anchor = new Vector3(bounds.center.x, bounds.max.y, bounds.center.z);
                        }
                    }
                    _hud.UpdateUnit(u, Camera.main, frac, alive, anchor);
                    _hud.SetProtection(u.UnitId, _replayShield.TryGetValue(u.UnitId, out var shield) ? shield : u.CurrentShield, u.MaxShield);
                    _hud.SetTurnMeter(u.UnitId, u == _sim.PendingActor ? GameConstants.UnitSpeedFillMax : u.SpeedFill,
                        GameConstants.UnitSpeedFillMax);
                    if (_replayStates.TryGetValue(u.UnitId, out var states))
                        _hud.SetPersistentStates(u.UnitId, states.Values.ToArray());
                }
            if (_commander != null)
                _hud.SetCommanderHealth(_replayHp.TryGetValue(_commander.UnitId, out var chp) ? chp : _commander.CurrentHp,
                    _commander.MaxHp, _replayShield.TryGetValue(_commander.UnitId, out var cs) ? cs : _commander.CurrentShield,
                    _commander.MaxShield);
        }

        void HandleKeys()
        {
            if (Input.GetKeyDown(KeyCode.C)) ToggleAuto();
            if (Input.GetKeyDown(KeyCode.B)) ToggleSpeed();
            if (Input.GetKeyDown(KeyCode.Z)) ToggleLock();
            if (_onSkillPick != null)
            {
                if (Input.GetKeyDown(KeyCode.Q)) _onSkillPick(0);
                if (Input.GetKeyDown(KeyCode.W)) _onSkillPick(1);
                if (Input.GetKeyDown(KeyCode.E)) _onSkillPick(2);
                if (Input.GetKeyDown(KeyCode.R)) _onSkillPick(3);
                if (Input.GetKeyDown(KeyCode.T)) _onSkillPick(4);
                if (Input.GetKeyDown(KeyCode.Y)) _onSkillPick(5);
            }
            if (_waitingForCommand)
            {
                for (int i = 0; i < _legalTargets.Count && i < 9; i++)
                    if (Input.GetKeyDown((KeyCode)((int)KeyCode.Alpha1 + i)))
                    { SubmitTarget(_legalTargets[i]); break; }
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
            StartCoroutine(CaptureFrame(path));
        }

        public IEnumerator CaptureFrame(string path)
        {
            var cam = Camera.main;
            if (cam == null) { Debug.LogError("[Capture] no camera"); yield break; }
            _hud.SetCamera(cam);   // overlay canvas won't render into cam.Render()
            var prevRT = cam.targetTexture;
            int width = Mathf.Max(1, Screen.width), height = Mathf.Max(1, Screen.height);
            var rt = new RenderTexture(width, height, 24);
            cam.targetTexture = rt;
            // Match the actual viewport; a fixed 1280x720 target crops camera-space
            // canvas controls when the batch GameView has a different aspect ratio.
            yield return null;
            Canvas.ForceUpdateCanvases();
            RefreshUnitBars();
            cam.Render();
            RenderTexture.active = rt;
            var tex = new Texture2D(width, height, TextureFormat.RGB24, false);
            tex.ReadPixels(new Rect(0, 0, width, height), 0, 0);
            tex.Apply();
            cam.targetTexture = prevRT;
            RenderTexture.active = null;
            Canvas.ForceUpdateCanvases();
            RefreshUnitBars();
            System.IO.File.WriteAllBytes(path, tex.EncodeToPNG());
            Destroy(tex);
            Destroy(rt);
            yield return null;
        }

        IEnumerator RunBattle()
        {
            if (!TryLoadBattleData(out var roster, out var abilityMap, out var failure))
            {
                _hud.SetTurnText("原始战斗数据加载失败，战斗已停止");
                Debug.LogError("[BattleDriver] " + failure);
                yield break;
            }
            _stage = new GameObject("OriginalBattleStage").AddComponent<BattleStage>();
            _stage.Assemble(false);
            var player = BuildTeam(roster, PlayerTeam, 0, abilityMap);
            var opponent = BuildTeam(roster, OpponentTeam, 1, abilityMap);
            _uiUnits = UiData.LoadUnits();

            _sim = new BattleSim(player, opponent, Seed);
            foreach (var u in player.Concat(opponent))
            {
                _replayHp[u.UnitId] = u.MaxHp;
                _replayShield[u.UnitId] = u.CurrentShield;
                _replayStates[u.UnitId] = new Dictionary<long, RawEffect>();
                _hud.TrackUnit(u, Camera.main, ChipLetter(u), onPick: () => SubmitTarget(u));
            }
            BuildP5Chrome(opponent.Count > 0 ? opponent[0] : null);
            if (!_sim.TryInitializeOriginalPassives(out var passiveFailure, allowUnsupportedForTraining: true))
            {
                _hud.SetTurnText("原始被动初始化失败：" + passiveFailure);
                yield break;
            }
            foreach (var ev in _sim.Log) ApplyReplayEvent(ev);
            gameObject.AddComponent<OriginalTrainingNotice>();

            _replayStart = Time.time;
            _replayActive = true;
            int guard = 0;
            while (!_sim.CheckEnd(out _) && guard++ < 500 && Time.time - _replayStart < BattleSeconds)
            {
                int firstEvent = _sim.Log.Count;
                var actor = _sim.PrepareNextTurn();
                // START_OF_TURN effects are resolved/presented before input.
                // A unit killed by DOT never receives a skill-selection screen.
                if (_sim.Log.Count > firstEvent) yield return PresentEvents(firstEvent);
                firstEvent = _sim.Log.Count;
                if (actor == null) break;
                if (actor.Side == 0 && !_autoBattle)
                {
                    BeginManualInput(actor);
                    while (_waitingForCommand && !_autoBattle && Time.time - _replayStart < BattleSeconds)
                        yield return null;
                    if (Time.time - _replayStart >= BattleSeconds) break;
                }
                if (_sim.PendingActor != null) _sim.RunAutomaticTurn();
                EndManualInput();
                // Only this committed action exists; no later turn is precomputed.
                yield return PresentEvents(firstEvent);
                foreach (var unit in _sim.Units)
                {
                    _replayHp[unit.UnitId] = unit.CurrentHp;
                    _replayShield[unit.UnitId] = unit.CurrentShield;
                }
            }
            EndManualInput();
            _replayActive = false;
            _sim.CheckEnd(out int winner);
            _hud.SetTurnText(winner == 0 ? "胜利" : winner == 1 ? "战败" : "战斗结束（超时或无法行动）");
            Debug.Log($"[BattleDriver] finished events={_sim.Log.Count} winnerSide={winner}");
        }

        void BeginManualInput(BattleUnit actor)
        {
            _waitingForCommand = true;
            _selectedSkill = int.MinValue;
            var original = EntryOf(actor.BaseId);
            var entry = new UnitEntry { def_id = actor.BaseId, skills = new List<SkillEntry>() };
            // Display order is the executor's order, not an independent UI ordering.
            foreach (var ability in actor.Abilities)
            {
                var source = original?.skills?.Find(sk => sk.id == ability.Id || sk.id == ability.SkillId);
                entry.skills.Add(new SkillEntry { id = ability.SkillId, name = ability.Name,
                    icon = source?.icon, tag = ability.Kind == "basic" ? "基本" : "特殊" });
            }
            if (entry.skills.Count == 0)
                entry.skills.Add(new SkillEntry { id = "fallback", name = "攻击", tag = "基本" });
            _onSkillPick = PickSkill;
            _hud.ShowSkillBar(entry, $"{original?.name ?? actor.BaseId} · 选择技能，再选择目标", PickSkill);
            for (int i = 0; i < actor.Abilities.Count; i++)
            {
                var ability = actor.Abilities[i];
                actor.CooldownLeft.TryGetValue(ability.Id, out int cooldown);
                string unsupported = _sim.OriginalUnsupportedReason(ability);
                _hud.SetSkillAvailability(i, _sim.IsAbilityReady(i) && _sim.GetLegalTargets(i).Count > 0,
                    ability.Name, cooldown, unsupported);
            }
            _hud.SetTurnText($"{original?.name ?? actor.BaseId} · 等待指令（Q/W/E/R/T/Y）");
            Debug.Log($"[BattleDriver] awaiting command actor={actor.UnitId} events={_sim.Log.Count}");
        }

        void PickSkill(int index)
        {
            if (!_waitingForCommand || _sim.PendingActor == null) return;
            var actor = _sim.PendingActor;
            int abilityIndex = actor.Abilities.Count == 0 && index == 0 ? -1 : index;
            if (!_sim.IsAbilityReady(abilityIndex) || _sim.GetLegalTargets(abilityIndex).Count == 0) return;
            _selectedSkill = abilityIndex;
            _legalTargets = _sim.GetLegalTargets(abilityIndex);
            _hud.HighlightSkill(index);
            _hud.ShowTargets(_legalTargets, u => EntryOf(u.BaseId)?.name ?? u.BaseId, SubmitTarget);
            string name = abilityIndex < 0 ? "攻击" : actor.Abilities[abilityIndex].Name;
            _hud.SetTurnText($"{name} · 选择目标（点击下方姓名或数字键）");
        }

        void SubmitTarget(BattleUnit target)
        {
            if (!_waitingForCommand || _selectedSkill == int.MinValue) return;
            _commander = target;
            var entry = EntryOf(target.BaseId);
            _hud.BuildCommander(entry, entry?.name ?? target.BaseId);
            if (!_sim.TrySubmitCommand(_selectedSkill, target, out string error))
            { _hud.SetTurnText(error); return; }
            Debug.Log($"[BattleDriver] command committed skill={_selectedSkill} target={target.UnitId}");
            EndManualInput();
        }

        void EndManualInput()
        {
            _waitingForCommand = false;
            _onSkillPick = null;
            _selectedSkill = int.MinValue;
            _legalTargets = Array.Empty<BattleUnit>();
            _hud.HideSkillBar();
        }

        // ---- P5 chrome: timer, controls row, commander portrait ------------
        void BuildP5Chrome(BattleUnit commander)
        {
            _hud.BuildTimer();
            _hud.BuildControls(ToggleAuto, ToggleSpeed, ToggleLock);
            _hud.SetControlState("auto", _autoBattle);
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
            const string pool = "ASDFG";
            return u.Side == 0 ? pool[SlotOf(u) % pool.Length].ToString() : (SlotOf(u) + 1).ToString();
        }

        int SlotOf(BattleUnit u)
        {
            int i = u.UnitId.LastIndexOf('_');
            return i >= 0 && int.TryParse(u.UnitId.Substring(i + 1), out var s) ? s : 0;
        }

        UnitEntry EntryOf(string defId) =>
            _uiUnits != null ? _uiUnits.Find(x => x.def_id == defId) : null;

        IEnumerator PresentEvents(int start)
        {
            int end = _sim.Log.Count;
            int turn = -1;
            for (int n = start; n < end; n++) if (_sim.Log[n].Kind == "turn") { turn = n; break; }
            if (turn >= 0)
            {
                var command = _sim.Log[turn];
                _unitsById.TryGetValue(command.ActorUnitId ?? "", out var unit);
                if (unit != null && !_presentationData.ContainsKey(unit.BaseId))
                    _presentationData[unit.BaseId] = OriginalAbilityPresentation.Load(unit.BaseId);
                var data = unit != null ? _presentationData[unit.BaseId] : null;
                if (data != null && data["abilities"].Any(a => (string)a["id"] == command.AbilityId))
                {
                    for (int n = start; n < turn; n++) ApplyReplayEvent(_sim.Log[n]);
                    _lastActorRig = RigOf(command.ActorUnitId);
                    _hud.SetTurnText($"第{command.Turn}回合 · {EntryOf(unit.BaseId)?.name ?? unit.BaseId} · {command.AbilityName}");
                    var anim = _anims[command.ActorUnitId];
                    var receiver = anim.GetComponent<OriginalAbilityAnimationEvents>();
                    ActivePresentation = new GameObject("OriginalAbilityFSM").AddComponent<OriginalAbilityPresentation>();
                    receiver.Active = ActivePresentation;
                    ActivePresentation.BindAreaMarkers(data, _stage, unit.Side);
                    ActivePresentation.AffectedUnits = _sim.Log.Skip(turn + 1).Take(end - turn - 1)
                        .Where(e => e.TargetUnitId != null && e.Kind != "status_remove")
                        .Select(e => RigOf(e.TargetUnitId)?.transform).Where(t => t != null).Distinct().ToArray();
                    var target = RigOf(command.TargetUnitId);
                    bool evaded = _sim.Log.Skip(turn + 1).Take(end - turn - 1).Any(e => e.Kind == "evade" && e.TargetUnitId == command.TargetUnitId);
                    ActivePresentation.Begin(data, command.AbilityId, _lastActorRig.transform, target ? target.transform : _lastActorRig.transform,
                        anim, evaded, () => { for (int n = turn + 1; n < end; n++) ApplyReplayEvent(_sim.Log[n]); });
                    while (!ActivePresentation.Finished && ActivePresentation.Failure == null)
                    {
                        ActivePresentation.PlaybackSpeed = _speedMult; anim.speed = _speedMult;
                        yield return null;
                    }
                    if (ActivePresentation.Failure != null)
                    {
                        PresentationFailure = ActivePresentation.Failure;
                        _hud.SetTurnText("原始动作链待恢复，已暂停：" + PresentationFailure);
                        // No guessed hit timestamp or substitute action on failure.
                        while (true) yield return null;
                    }
                    receiver.Active = null;
                    Destroy(ActivePresentation.gameObject); ActivePresentation = null;
                    yield break;
                }
            }
            for (int i = start; i < end; i++)
            {
                var ev = _sim.Log[i];
                if (ev.Kind == "turn")
                {
                    _unitsById.TryGetValue(ev.ActorUnitId ?? "", out var actor);
                    _rigs.TryGetValue(ev.ActorUnitId ?? "", out _lastActorRig);
                    _hud.SetTurnText($"第{ev.Turn}回合 · {EntryOf(actor?.BaseId)?.name ?? ev.Actor} · {ev.AbilityName}");
                    PlayClip(ev.ActorUnitId, "Attack", "Idle");
                    yield return new WaitForSeconds(0.6f / _speedMult);
                }
                else if (ev.Kind == "damage" || ev.Kind == "crit" || ev.Kind == "heal")
                {
                    string id = ev.TargetUnitId;
                    if (id != null && _unitsById.TryGetValue(id, out var target))
                    {
                        double old = _replayHp[id];
                        _replayHp[id] = ev.Kind == "heal" ? Math.Min(target.MaxHp, old + ev.Amount) : Math.Max(0, old - ev.HpDamage);
                        if (ev.Kind != "heal")
                            _replayShield[id] = Math.Max(0, _replayShield[id] - ev.ShieldDamage);
                        if (target == _commander) _hud.UpdateCommander((float)(_replayHp[id] / target.MaxHp));
                        var go = RigOf(id);
                        if (go && ev.Kind != "heal") { StartCoroutine(Flash(go)); PlayClip(id, "Hit"); }
                        if (ev.Kind == "heal" || ev.HpDamage > 0)
                            _hud.ShowDamage(id, ev.Kind == "heal" ? ev.Amount : ev.HpDamage,
                                ev.Kind == "crit", Camera.main, ev.Kind == "heal");
                        if (ev.ShieldDamage > 0) _hud.ShowShieldDamage(id, ev.ShieldDamage, Camera.main);
                    }
                    yield return new WaitForSeconds(0.35f / _speedMult);
                }
                else if (ev.Kind == "status_add" || ev.Kind == "status_remove" || ev.Kind == "resist")
                {
                    if (_replayStates.TryGetValue(ev.TargetUnitId, out var states))
                    {
                        if (ev.Kind == "status_add") states[ev.EffectInstanceId] = _originalGraph.Effects[ev.EffectId];
                        else if (ev.Kind == "status_remove") states.Remove(ev.EffectInstanceId);
                    }
                    string label = ev.EffectType == 4 ? "技能封锁" : ev.EffectType == 8 ? "速度变化" :
                        ev.EffectIcon == "icon_damage_over_time" ? "持续伤害" : "持续效果";
                    _hud.ShowStatusEvent(ev.TargetUnitId, ev.Kind == "resist" ? "抵抗" :
                        label + (ev.Kind == "status_remove" ? "结束" : ""));
                    yield return new WaitForSeconds(0.3f / _speedMult);
                }
                else if (ev.Kind == "death")
                {
                    if (ev.TargetUnitId != null) _replayHp[ev.TargetUnitId] = 0;
                    PlayClip(ev.TargetUnitId, "Death");
                    yield return new WaitForSeconds(0.6f / _speedMult);
                }
            }
        }

        void ApplyReplayEvent(BattleEvent ev)
        {
            string id = ev.TargetUnitId;
            if (id == null || !_unitsById.TryGetValue(id, out var unit)) return;
            if (ev.Kind == "damage" || ev.Kind == "crit" || ev.Kind == "heal")
            {
                _replayHp[id] = ev.Kind == "heal" ? Math.Min(unit.MaxHp, _replayHp[id] + ev.Amount) : Math.Max(0, _replayHp[id] - ev.HpDamage);
                if (ev.Kind != "heal") _replayShield[id] = Math.Max(0, _replayShield[id] - ev.ShieldDamage);
                else _replayShield[id] = Math.Min(unit.MaxShield, _replayShield[id] + ev.ShieldHeal);
                if (unit == _commander) _hud.UpdateCommander((float)(_replayHp[id] / unit.MaxHp));
                if (ev.Kind == "heal" || ev.HpDamage > 0) _hud.ShowDamage(id, ev.Kind == "heal" ? ev.Amount : ev.HpDamage, ev.Kind == "crit", Camera.main, ev.Kind == "heal");
                if (ev.ShieldDamage > 0) _hud.ShowShieldDamage(id, ev.ShieldDamage, Camera.main);
            }
            else if (ev.Kind == "status_add" || ev.Kind == "status_remove" || ev.Kind == "resist")
            {
                if (_replayStates.TryGetValue(id, out var states))
                {
                    if (ev.Kind == "status_add") states[ev.EffectInstanceId] = _originalGraph.Effects[ev.EffectId];
                    else if (ev.Kind == "status_remove") states.Remove(ev.EffectInstanceId);
                }
                if (ev.Kind == "resist") _hud.ShowStatusEvent(id, "抵抗");
            }
            else if (ev.Kind == "death")
            {
                _replayHp[id] = 0;
                if (_anims.TryGetValue(id, out var anim) && anim) anim.SetTrigger("Death");
            }
        }

        List<RosterEntry> LoadRoster()
        {
            var path = Path.Combine(Application.streamingAssetsPath, "roster.json");
            var json = File.ReadAllText(path);
            return JsonUtilityArray<RosterEntry>.FromJson(json);
        }

        bool TryLoadBattleData(out List<RosterEntry> roster,
            out Dictionary<string, List<UnitAbility>> abilities, out string failure)
        {
            roster = null;
            abilities = null;
            try
            {
                roster = LoadRoster();
                abilities = LoadAbilities();
                foreach (var id in PlayerTeam.Concat(OpponentTeam))
                {
                    if (!roster.Any(r => r.def_id == id) || !abilities.TryGetValue(id, out var list) || list.Count == 0)
                        throw new InvalidDataException("Missing original roster/ability menu for " + id);
                }
                failure = null;
                return true;
            }
            catch (Exception ex)
            {
                failure = ex.Message;
                return false;
            }
        }

        Dictionary<string, List<UnitAbility>> LoadAbilities()
        {
            string root = Application.streamingAssetsPath;
            // RawEffect contains recursive original payload fields. Unity's inline
            // serializer imposes a depth limit even for empty branches; preserve
            // the actual JSON tree and null/empty distinctions with Newtonsoft.
            var manifest = Newtonsoft.Json.JsonConvert.DeserializeObject<RawEffectManifest>(File.ReadAllText(Path.Combine(root, "battle_effects.json")));
            if (manifest == null || manifest.schemaVersion != 2 || manifest.abilities == null ||
                manifest.effectFiles == null || manifest.effectFiles.Length == 0)
                throw new InvalidDataException("Expected original effect graph manifest schema 2.");
            var effects = new List<RawEffect>();
            foreach (string filename in manifest.effectFiles)
            {
                if (string.IsNullOrEmpty(filename) || Path.GetFileName(filename) != filename)
                    throw new InvalidDataException("Invalid original effect shard name.");
                var shard = Newtonsoft.Json.JsonConvert.DeserializeObject<RawEffectShard>(File.ReadAllText(Path.Combine(root, filename)));
                if (shard?.effects == null) throw new InvalidDataException("Invalid original effect shard " + filename);
                effects.AddRange(shard.effects);
            }
            var graph = new OriginalEffectGraph(effects, manifest.abilities);
            _originalGraph = graph;
            var map = new Dictionary<string, List<UnitAbility>>(StringComparer.Ordinal);
            _originalMenus.Clear();
            foreach (var menu in Newtonsoft.Json.JsonConvert.DeserializeObject<OriginalUnitMenu[]>(File.ReadAllText(Path.Combine(root, "abilities.json"))))
            {
                if (menu.abilities == null || menu.abilities.Any(a => a.effectReference == null))
                    throw new InvalidDataException("Missing original effect references for " + menu.def_id);
                _originalMenus.Add(menu.def_id, menu);
                map.Add(menu.def_id, menu.abilities.Select(graph.Bind).ToList());
            }
            Debug.Log($"[BattleDriver] original graph: {effects.Count} effects, {manifest.abilities.Length} abilities, {map.Count} unit menus");
            return map;
        }

        List<BattleUnit> BuildTeam(List<RosterEntry> roster, string[] ids, int side,
                                   Dictionary<string, List<UnitAbility>> abilityMap)
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
                u.MaxShield = u.CurrentShield = Math.Max(0, u.GetStat(25));
                u.Categories.UnionWith(_originalMenus[e.def_id].categoryId ?? Array.Empty<string>());
                u.SquadPositionPriority = _originalMenus[e.def_id].squadPositionPriority;
                u.Abilities = abilityMap[e.def_id];
                u.IsLeader = slot == 0;
                u.Passives = (_originalMenus[e.def_id].passives ?? Array.Empty<OriginalAbilityMenu>()).Select(_originalGraph.Bind).ToList();
                foreach (var a in u.Abilities) u.CooldownLeft[a.Id] = 0;
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
            // The static and animated GLB are separate source exports. Prefer the
            // original curve-bearing rig, not the identically shaped static mesh.
            var t = CharacterPrefabRoot.transform.Find(folder + ".anim") ?? CharacterPrefabRoot.transform.Find(folder);
            if (t == null)
            {
                Debug.LogWarning($"rig not imported: {folder}");
                return;
            }
            var go = Instantiate(t.gameObject, transform);
            _stage.Place(go, side, slot);
            go.name = u.UnitId;        // BattleHUD resolves bars by UnitId
            go.SetActive(true);   // staged prefabs park inactive
            _rigs[u.UnitId] = go;
            _anims[u.UnitId] = go.GetComponentInChildren<Animator>();
            if (!_presentationData.ContainsKey(u.BaseId)) _presentationData[u.BaseId] = OriginalAbilityPresentation.Load(u.BaseId);
            go.AddComponent<OriginalCharacterPresentation>().Initialize(folder, _presentationData[u.BaseId]);
            var originalController = Resources.Load<RuntimeAnimatorController>("AbilityPresentation/" + u.BaseId + "Animator");
            if (originalController && _anims[u.UnitId])
            {
                _anims[u.UnitId].runtimeAnimatorController = originalController;
                _anims[u.UnitId].gameObject.AddComponent<OriginalAbilityAnimationEvents>();
            }
        }

        GameObject RigOf(string unitId) =>
            unitId != null && _rigs.TryGetValue(unitId, out var rig) ? rig : null;

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
