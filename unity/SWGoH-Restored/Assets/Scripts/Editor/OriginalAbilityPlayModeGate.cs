using System.Collections;
using System.IO;
using System.Linq;
using SwgohRestored.Battle;
using UnityEditor;
using UnityEditor.SceneManagement;
using UnityEngine;

namespace SwgohRestored.EditorTools
{
    public static class OriginalAbilityPlayModeGate
    {
        const string Flag = "SWGoH.OriginalAbilityGate";
        public static void ImportAndRun()
        {
            OriginalAbilityImportGate.Run();
            Run();
        }
        public static void Run()
        {
            typeof(BattlePresentationGate).GetMethod("SetCaptureResolution", System.Reflection.BindingFlags.Static | System.Reflection.BindingFlags.NonPublic).Invoke(null, null);
            SessionState.SetBool(Flag, true);
            EditorSceneManager.OpenScene("Assets/Scenes/BattleDemo.unity");
            EditorApplication.isPlaying = true;
        }
        [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.AfterSceneLoad)]
        static void StartGate()
        {
            if (!SessionState.GetBool(Flag, false)) return;
            SessionState.SetBool(Flag, false);
            new GameObject("OriginalAbilityGate").AddComponent<OriginalAbilityGateRunner>();
        }
    }
    public class OriginalAbilityGateRunner : BattleCommandGateRunner
    {
        const string Output = "F:/PPfavorite/Work/goh/logs/vader_complete";
        void Awake() { Application.logMessageReceived += OnLog; }
        void OnDestroy() { Application.logMessageReceived -= OnLog; }
        void OnLog(string message, string stack, LogType type)
        {
            if (type == LogType.Exception || (type == LogType.Error && message.StartsWith("[OriginalPresentation]")))
            {
                File.WriteAllText(Output + "/original-ability-failure.txt", message + "\n" + stack);
                EditorApplication.delayCall += () => EditorApplication.Exit(1);
            }
        }
        IEnumerator Start()
        {
            float deadline = Time.realtimeSinceStartup + 150;
            BattleDriver driver;
            do { yield return null; driver = Object.FindObjectOfType<BattleDriver>(); }
            while ((driver == null || !driver.WaitingForCommand) && Time.realtimeSinceStartup < deadline);
            Require(driver != null && driver.WaitingForCommand, "original animation fixture ready");
            var units = driver.Simulation.Units;
            var actor = GameObject.Find(units.First(u => u.Side == 0).UnitId).transform;
            var target = GameObject.Find(units.First(u => u.Side == 1).UnitId).transform;
            var animator = actor.GetComponentInChildren<Animator>();
            var relay = animator.GetComponent<OriginalAbilityAnimationEvents>();
            Require(relay != null, "original controller with event relay");
            var origin = actor.position;
            foreach (var abilityId in new[] { "basicability_darthvader", "specialability_vader02", "specialability_vader01", "specialability_vader03" })
            {
            var run = new GameObject("OriginalAbilityFSM").AddComponent<OriginalAbilityPresentation>();
            relay.Active = run;
            float appliedTime = -1, started = Time.time;
            var data = OriginalAbilityPresentation.Load("VADER");
            run.BindAreaMarkers(data, driver.Stage, 0);
            run.AffectedUnits = units.Where(u => u.Side == 1).Select(u => GameObject.Find(u.UnitId).transform).ToArray();
            run.Begin(data, abilityId, actor, target, animator, false,
                () => appliedTime = Time.time - started);
            bool shot = false;
            while (!run.Finished && run.Failure == null && Time.realtimeSinceStartup < deadline)
            {
                if (!shot && run.Applied) { shot = true; yield return driver.CaptureFrame(Output + "/" + abilityId + "-impact.png"); }
                yield return null;
            }
            File.WriteAllLines(Output + "/" + abilityId + "-fsm-trace.txt", run.Trace);
            Require(run.Failure == null, run.Failure ?? "original FSM no failure");
            Require(run.Finished && run.Applied, "original FSM applied and finished");
            if (abilityId == "basicability_darthvader") Require(appliedTime > 1.5f && appliedTime < 2.5f, "settlement follows final hit event, not initial hit");
            Require(Vector3.Distance(origin, actor.position) < .005f, "original backward jump returns to formation");
            yield return driver.CaptureFrame(Output + "/" + abilityId + "-finished.png");
            relay.Active = null; Destroy(run.gameObject);
            yield return new WaitForSeconds(1);
            }
            File.WriteAllText(Output + "/original-ability-playmode-pass.txt", "PASS four original FSMs/animation events/movement/settlement/finish");
            EditorApplication.Exit(0);
        }
    }
}
