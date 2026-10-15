// Batch-mode play capture: drops a marker file (statics don't survive the
// play-mode domain reload), opens BattleDemo and enters play mode.  The
// driver's Update() loop renders screenshots and quits when done.
// Run WITHOUT -nographics so camera rendering works.
using System.IO;
using UnityEditor;
using UnityEditor.SceneManagement;

namespace SwgohRestored.EditorTools
{
    public static class PlayModeCapture
    {
        const string Marker = @"F:\PPfavorite\Work\goh\unity\capture.marker";

        public static void Run()
        {
            File.WriteAllText(Marker, "capture");
            EditorSceneManager.OpenScene("Assets/Scenes/BattleDemo.unity");
            EditorApplication.isPlaying = true;
        }
    }
}
