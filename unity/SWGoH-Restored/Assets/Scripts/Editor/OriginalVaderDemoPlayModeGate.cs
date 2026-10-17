using System;
using System.Collections;
using System.IO;
using System.Linq;
using SwgohRestored.Battle;
using UnityEditor;
using UnityEditor.SceneManagement;
using UnityEngine;
using UnityEngine.UI;
using Newtonsoft.Json.Linq;

namespace SwgohRestored.EditorTools
{
    public static class OriginalVaderDemoPlayModeGate
    {
        const string Flag = "SWGoH.VaderDemoGate";
        public static void Run()
        {
            typeof(BattlePresentationGate).GetMethod("SetCaptureResolution", System.Reflection.BindingFlags.Static | System.Reflection.BindingFlags.NonPublic).Invoke(null, null);
            SessionState.SetBool(Flag, true);
            EditorSceneManager.OpenScene("Assets/Scenes/BattleDemo.unity");
            // Explicit encounter fixture, verified against production roster and
            // graph: seed 4490 naturally locks specials until the player dies;
            // seed 8 permits the four-command demonstration without rule edits.
            if(UnityEngine.Object.FindObjectOfType<BattleDriver>().Seed != 8)
                throw new InvalidOperationException("Production demo encounter must use documented seed 8.");
            EditorApplication.isPlaying = true;
        }
        [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.AfterSceneLoad)]
        static void StartGate()
        {
            if (!SessionState.GetBool(Flag, false)) return;
            SessionState.SetBool(Flag, false);
            new GameObject("OriginalVaderDemoGate").AddComponent<OriginalVaderDemoGateRunner>();
        }
    }

    // Uses the production scene, data, seed, command path and presentation.
    // No injected cooldowns, HP, states, turns or direct simulation commands.
    public class OriginalVaderDemoGateRunner : BattleCommandGateRunner
    {
        const string Output = "F:/PPfavorite/Work/goh/logs/vader_complete/demo-ui";
        BattleDriver driver;
        readonly JArray actions = new JArray();
        void Awake() { Directory.CreateDirectory(Output); Application.logMessageReceived += OnLog; }
        void OnDestroy() { Application.logMessageReceived -= OnLog; }
        void OnLog(string message, string stack, LogType type)
        {
            if (type != LogType.Exception && !(type == LogType.Error &&
                (message.StartsWith("[OriginalPresentation]") || message.StartsWith("[CommandGate]")))) return;
            File.WriteAllText(Output + "/failure.txt", message + "\n" + stack);
            EditorApplication.delayCall += () => EditorApplication.Exit(1);
        }
        string State() => driver.Simulation.Round + ":" + driver.Simulation.Log.Count + ":" +
            string.Join(";", driver.Simulation.Units.Select(u => u.UnitId + ":" + u.CurrentHp + ":" + u.CurrentShield + ":" + u.SpeedFill + ":" +
                string.Join(",",u.CooldownLeft.OrderBy(p=>p.Key).Select(p=>p.Key+"="+p.Value)) + ":" +
                string.Join(",",u.PersistentEffects.Select(p=>p.Definition.id))));
        IEnumerator WaitForManual()
        {
            float deadline = Time.realtimeSinceStartup + 100;
            while ((driver == null || !driver.WaitingForCommand) && Time.realtimeSinceStartup < deadline)
            {
                if (driver == null) driver = UnityEngine.Object.FindObjectOfType<BattleDriver>();
                if (driver != null && driver.Simulation != null && driver.Simulation.CheckEnd(out _)) break;
                yield return null;
            }
            Require(driver != null && driver.WaitingForCommand, "production demo did not reach manual input");
            Require(string.IsNullOrEmpty(driver.PresentationFailure), "presentation failed: " + driver.PresentationFailure);
        }
        IEnumerator Start()
        {
            yield return WaitForManual();
            var sim = driver.Simulation;
            File.WriteAllText(Output + "/encounter.json", new JObject {
                ["seed"]=driver.Seed,["player"]=new JArray(driver.PlayerTeam),["opponent"]=new JArray(driver.OpponentTeam),
                ["scope"]="Production data, explicit seed 8 encounter fixture; unsupported leader training notice retained; no stat/state/cooldown mutations"
            }.ToString());
            var pending=new System.Collections.Generic.Queue<string>(new[] { "specialability_vader03", "specialability_vader02", "specialability_vader01", "basicability_darthvader" });
            int guard=0;
            while(pending.Count>0)
            {
                Require(guard++<16,"production encounter did not make requested abilities legally available in 16 player turns");
                yield return WaitForManual();
                var actor = sim.PendingActor;
                Require(actor != null && actor.BaseId == "VADER", "manual actor must be Vader");
                string requested=pending.Peek(),id=requested;
                int requestedIndex=actor.Abilities.FindIndex(a=>a.Id==requested);
                // Default 1v2 Vader encounter: enemy unique speed contributions
                // let both enemies apply ability block before the player's turn.
                // Spend a real basic turn; never remove states or force cooldowns.
                if(requestedIndex>=0 && !sim.IsAbilityReady(requestedIndex) &&
                    sim.OriginalUnsupportedReason(actor.Abilities[requestedIndex])==null &&
                    actor.PersistentEffects.Any(s=>s.Definition.type==4)) id="basicability_darthvader";
                string before = State();
                yield return new WaitForSecondsRealtime(1);
                Require(State() == before, "manual waiting advanced state before " + id);
                int index = actor.Abilities.FindIndex(a => a.Id == id);
                var diagnostics=new JObject { ["requested"]=requested,["selected"]=id,["round"]=sim.Round,["actor"]=actor.UnitId,
                    ["cooldowns"]=JObject.FromObject(actor.CooldownLeft),
                    ["persistentStates"]=JArray.FromObject(actor.PersistentEffects.Select(s=>new {s.Definition.id,s.Definition.type,s.Amount})),
                    ["eventsSoFar"]=JArray.FromObject(sim.Log),
                    ["abilities"]=JArray.FromObject(actor.Abilities.Select((a,i)=>new {a.Id,a.Kind,a.OriginalTier,a.Cooldown,
                        ready=sim.IsAbilityReady(i),unsupported=sim.OriginalUnsupportedReason(a),legalTargets=sim.GetLegalTargets(i).Select(t=>t.UnitId).ToArray()})) };
                File.WriteAllText(Output + "/" + id + "-readiness.json", diagnostics.ToString());
                string reason=index<0?"missing ability":sim.OriginalUnsupportedReason(actor.Abilities[index]) ??
                    (actor.PersistentEffects.Any(s=>s.Definition.type==4)?"active ability-block state":
                    actor.CooldownLeft.TryGetValue(id,out var cooldown)?"cooldown="+cooldown:"unknown readiness condition");
                Require(index >= 0 && sim.IsAbilityReady(index), "ability not legally ready: " + id + "; " + reason);
                var targets = sim.GetLegalTargets(index);
                Require(targets.Count > 0, "no legal target for " + id);
                var target = targets.OrderByDescending(u => u.HasEffectTag("vader_merciless_marker"))
                    .ThenByDescending(u => u.PersistentEffects.Count(s => s.Definition.descriptiveTag != null && s.Definition.descriptiveTag.Any(t => t.tag == "dot_debuff")))
                    .ThenByDescending(u=>u.CurrentHp).First();
                var skill = GameObject.Find("skill_" + actor.Abilities[index].SkillId)?.GetComponent<Button>();
                Require(skill != null && skill.interactable, "missing/disabled UI skill " + id);
                Click(skill);
                yield return null;
                Require(State() == before, "skill selection committed without target");
                yield return driver.CaptureFrame(Output + "/" + id + "-select.png");
                var button = GameObject.Find("target_" + target.UnitId)?.GetComponent<Button>();
                Require(button != null && button.interactable, "legal target button unavailable");
                int first = sim.Log.Count, round = sim.Round;
                bool targetHadDots=target.HasEffectTag("dot_debuff");
                Click(button);
                Require(!driver.WaitingForCommand && sim.Round == round + 1, "UI command must commit exactly once");
                var committed = sim.Log.Skip(first).ToArray();
                Require(committed.Count(e => e.Kind == "turn" && e.ActorUnitId == actor.UnitId && e.AbilityId == id && e.TargetUnitId == target.UnitId) == 1,
                    "committed ability/target differs from clicked command");
                button.onClick.Invoke();
                Require(sim.Round == round + 1, "double target click submitted another command");
                if (id == "specialability_vader03")
                {
                    Require(actor.HasEffectTag("vader_merciless_buff"), "Merciless buff absent");
                    Require(sim.Units.Where(u=>u.Side!=actor.Side && u.CurrentHp>0).All(u=>u.HasEffectTag("vader_merciless_marker")), "Merciless did not mark living enemies");
                }
                if (id == "specialability_vader02")
                {
                    var affected = committed.Where(e=>e.Kind=="damage" || e.Kind=="crit" || e.Kind=="evade").Select(e=>e.TargetUnitId).Distinct().ToArray();
                    Require(affected.Length > 1, "Force Crush did not resolve damage/evasion for multiple enemies");
                    Require(sim.Units.Any(u=>u.Side!=actor.Side && u.HasEffectTag("dot_debuff")), "Force Crush landed no DOT in production seed");
                }
                if (id == "specialability_vader01" || id == "basicability_darthvader")
                    Require(committed.Any(e=>(e.Kind=="damage" || e.Kind=="crit") && e.TargetUnitId==target.UnitId), "selected attack did not damage selected enemy");
                if (id == "specialability_vader01" && targetHadDots)
                    Require(!target.HasEffectTag("dot_debuff"), "Culling did not clear the selected target's DOT instances");
                actions.Add(new JObject { ["requested"]=requested,["ability"]=id,["fillerForAbilityBlock"]=id!=requested,["actor"]=actor.UnitId,["target"]=target.UnitId,
                    ["round"]=round+1,["events"]=JArray.FromObject(committed),["committedState"]=State() });
                File.WriteAllText(Output + "/actions.json", actions.ToString());
                yield return new WaitForSecondsRealtime(.35f);
                yield return driver.CaptureFrame(Output + "/" + id + "-action.png");
                // Completion is observed through the real Driver loop, not manual FSM callbacks.
                if (!sim.CheckEnd(out _)) yield return WaitForManual();
                else
                {
                    float finishDeadline=Time.realtimeSinceStartup+60;
                    while(driver.ActivePresentation != null && !driver.ActivePresentation.Finished &&
                        string.IsNullOrEmpty(driver.PresentationFailure) && Time.realtimeSinceStartup<finishDeadline) yield return null;
                    Require(driver.ActivePresentation==null || driver.ActivePresentation.Finished,"final attack presentation timed out");
                }
                Require(string.IsNullOrEmpty(driver.PresentationFailure), "presentation completion failure");
                yield return driver.CaptureFrame(Output + "/" + id + "-finished.png");
                if(id==requested)pending.Dequeue();
            }
            File.WriteAllText(Output + "/pass.txt", "PASS: production UI raycast clicks Merciless -> Force Crush -> Culling Blade -> basic; manual wait; target selection; single commit; real Driver settlement; original state assertions; screenshots. Unsupported leader remains explicitly disclosed by production training notice.");
            Debug.Log("[VaderDemoGate] PASS four production UI commands and completed presentation");
            EditorApplication.Exit(0);
        }
    }
}
