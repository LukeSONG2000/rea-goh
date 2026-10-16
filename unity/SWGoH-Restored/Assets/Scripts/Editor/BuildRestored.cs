// BuildRestored -- Windows standalone player build for the SWGoH restoration.
//
// BOOT FLOW: HomeScene ships as scene 0 so the player boots into the P2
// cantina home; the scenes already cross-navigate
//   HomeScene --S--> RosterScene --tap--> DetailScene --back--> RosterScene
//   HomeScene --G--> BattleDemo, DetailScene --> BattleDemo
// The explicit EditorBuildSettings list here is the source of truth for the
// build (idempotent -- safe to re-run).
using System.IO;
using System.Linq;
using UnityEditor;
using UnityEngine;

namespace SwgohRestored.EditorTools
{
    public static class BuildRestored
    {
        // boot order: home first, then the P3/P4/P5 screens
        static readonly string[] SceneOrder =
        {
            "Assets/Scenes/HomeScene.unity",
            "Assets/Scenes/RosterScene.unity",
            "Assets/Scenes/DetailScene.unity",
            "Assets/Scenes/BattleDemo.unity",
        };

        public const string OutputExe = @"F:\PPfavorite\Work\goh\unity\build\SWGoH-Restored.exe";

        [MenuItem("SWGoH/Build/Windows Player")]
        public static void BuildWindowsPlayer()
        {
            // verify every scene exists before touching build settings
            foreach (var scene in SceneOrder)
                if (!File.Exists(scene))
                    throw new InvalidOperationException($"scene missing: {scene}");

            var list = SceneOrder.Select(s => new EditorBuildSettingsScene(s, true)).ToArray();
            EditorBuildSettings.scenes = list;

            var dir = Path.GetDirectoryName(OutputExe);
            Directory.CreateDirectory(dir);

            var options = new BuildPlayerOptions
            {
                scenes = SceneOrder,
                locationPathName = OutputExe,
                target = BuildTarget.StandaloneWindows64,
                options = BuildOptions.None,
            };
            var report = BuildPipeline.BuildPlayer(options);
            var summary = report.summary; // Build.Reporting.BuildSummary
            UnityEngine.Debug.Log($"[BuildRestored] {summary.platform} -> {OutputExe} " +
                      $"result={summary.result} size={summary.totalSize} errors={summary.totalErrors}");
            // totalErrors can count editor-session licensing handshake noise
            // (telemetry), not build failures -- the result is the gate
            if (summary.result != UnityEditor.Build.Reporting.BuildResult.Succeeded)
                throw new InvalidOperationException($"build failed: {summary.result}");
        }

        // headless smoke gate: run the built exe for a few seconds and require
        // the process to stay alive (no boot crash) + no errors in Player.log
        [MenuItem("SWGoH/Build/Smoke Run Player")]
        public static void SmokeRunPlayer()
        {
            if (!File.Exists(OutputExe))
                throw new InvalidOperationException("exe not built yet");
            var psi = new System.Diagnostics.ProcessStartInfo(OutputExe) { UseShellExecute = false };
            var p = System.Diagnostics.Process.Start(psi);
            System.Threading.Thread.Sleep(15000);
            bool alive = !((System.Diagnostics.Process)p).HasExited;
            string log = Path.Combine(
                System.Environment.GetFolderPath(System.Environment.SpecialFolder.LocalApplicationData) +
                    @"Low\DefaultCompany\SWGoH-Restored", "Player.log");
            string logText = File.Exists(log) ? File.ReadAllText(log) : "(no log)";
            bool logErr = logText.Contains("CRASH") || logText.Contains("Failed to load");
            if (alive) ((System.Diagnostics.Process)p).Kill();
            UnityEngine.Debug.Log($"[BuildRestored] smoke: aliveAfter15s={alive} logError={logErr} log={log}");
            if (!alive || logErr)
                throw new InvalidOperationException("player smoke run failed");
        }
    }

    // InvalidOperationException without pulling System into scope surprises
    internal class InvalidOperationException : System.Exception
    {
        public InvalidOperationException(string msg) : base(msg) { }
    }
}
