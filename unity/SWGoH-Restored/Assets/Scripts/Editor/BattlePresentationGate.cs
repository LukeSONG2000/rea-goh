using System.Collections;
using System.IO;
using System.Linq;
using SwgohRestored.Battle;
using UnityEditor;
using UnityEditor.SceneManagement;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

namespace SwgohRestored.EditorTools
{
    public static class BattlePresentationGate
    {
        const string Flag = "SWGoH.PresentationGate";
        public static void Run()
        {
            SetCaptureResolution();
            SessionState.SetBool(Flag, true);
            EditorSceneManager.OpenScene("Assets/Scenes/BattleDemo.unity");
            EditorApplication.isPlaying = true;
        }
        static void SetCaptureResolution()
        {
            // The editor's headless default is 640x480, not the supplied widescreen reference.
            var assembly = typeof(Editor).Assembly;
            var sizesType = assembly.GetType("UnityEditor.GameViewSizes");
            var singleton = typeof(ScriptableSingleton<>).MakeGenericType(sizesType);
            var sizes = singleton.GetProperty("instance", System.Reflection.BindingFlags.Public |
                System.Reflection.BindingFlags.Static | System.Reflection.BindingFlags.FlattenHierarchy).GetValue(null);
            var groupType = assembly.GetType("UnityEditor.GameViewSizeGroupType");
            var group = sizesType.GetMethod("GetGroup").Invoke(sizes, new[] { System.Enum.Parse(groupType, "Standalone") });
            var sizeType = assembly.GetType("UnityEditor.GameViewSize");
            var kind = assembly.GetType("UnityEditor.GameViewSizeType");
            var entry = System.Activator.CreateInstance(sizeType, new[] { System.Enum.Parse(kind, "FixedResolution"), (object)1920, 1080, "Original battle validation" });
            group.GetType().GetMethod("AddCustomSize").Invoke(group, new[] { entry });
            int index = (int)group.GetType().GetMethod("GetBuiltinCount").Invoke(group, null) +
                (int)group.GetType().GetMethod("GetCustomCount").Invoke(group, null) - 1;
            var viewType = assembly.GetType("UnityEditor.GameView");
            var view = EditorWindow.GetWindow(viewType);
            viewType.GetProperty("selectedSizeIndex").SetValue(view, index);
        }
        [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.AfterSceneLoad)]
        static void StartGate()
        {
            if (!SessionState.GetBool(Flag, false)) return;
            SessionState.SetBool(Flag, false);
            var driver = Object.FindObjectOfType<BattleDriver>();
            driver.PlayerTeam = Enumerable.Repeat("VADER", 5).ToArray();
            driver.OpponentTeam = Enumerable.Repeat("VADER", 5).ToArray();
            new GameObject("PresentationGate").AddComponent<BattlePresentationGateRunner>();
        }
    }
    public class BattlePresentationGateRunner : BattleCommandGateRunner
    {
        const string Output = "F:/PPfavorite/Work/goh/logs/visual_phase3";
        IEnumerator Start()
        {
            Directory.CreateDirectory(Output);
            float deadline = Time.realtimeSinceStartup + 90;
            BattleDriver driver;
            do { yield return null; driver = Object.FindObjectOfType<BattleDriver>(); }
            while ((driver == null || !driver.WaitingForCommand) && Time.realtimeSinceStartup < deadline);
            Require(driver != null && driver.WaitingForCommand, "presentation manual battle ready");
            Require(driver.Simulation.Units.Count() == 10, "all original squad slots populated");
            Require(GameObject.Find("Floor") == null, "invented floor disabled");
            var stage = driver.Stage;
            CheckCamera(stage);
            foreach (var unit in driver.Simulation.Units)
            {
                var model = GameObject.Find(unit.UnitId);
                Require(model != null, "original model exists " + unit.UnitId);
                int slot = int.Parse(unit.UnitId.Split('_').Last());
                var pose = stage.Slot(unit.Side, slot);
                Require(Vector3.Distance(model.transform.localPosition, pose.position) < 0.0001f, "original slot position");
                Require(Quaternion.Angle(model.transform.localRotation, pose.rotation) < 0.01f, "original slot rotation");
                var hp = GameObject.Find("hp_" + unit.BaseId);
                Require(hp != null, "unit HUD exists");
            }
            yield return new WaitForSeconds(1);
            yield return driver.CaptureFrame(Output + "/squad-original-stage.png");
            var actor = driver.Simulation.PendingActor;
            int basic = actor.Abilities.FindIndex(a => a.Kind == "basic");
            Click(GameObject.Find("skill_" + actor.Abilities[basic].SkillId).GetComponent<Button>());
            yield return null;
            // Verify all target buttons remain on screen and really receive clicks.
            var target = driver.Simulation.GetLegalTargets(basic).Last();
            var button = GameObject.Find("target_" + target.UnitId)?.GetComponent<Button>();
            Require(button != null, "five-target selection available");
            Click(button);
            yield return null;
            Require(driver.Simulation.Log.Any(e => e.TargetUnitId == target.UnitId), "visual changes preserve manual command");
            var oldRoots = SceneManager.GetActiveScene().GetRootGameObjects();
            foreach (var root in oldRoots) if (root != gameObject) root.SetActive(false);
            var preview = new GameObject("FleetPreview").AddComponent<FleetPresentationPreview>();
            yield return null;
            yield return null;
            Require(preview.Stage != null && preview.Stage.Fleet, "fleet original layout active");
            CheckCamera(preview.Stage);
            Require(preview.Stage.LayoutRoot.GetComponentsInChildren<Renderer>().Length > 6, "fleet original models loaded");
            foreach (int side in new[] { 0, 1 })
            {
                var capital = GameObject.Find("ship_chimaera_pre#" + side + "_0");
                Require(capital != null, "capital model exists");
                var pose = preview.Stage.Slot(side, 0, true);
                Require(Vector3.Distance(capital.transform.localPosition, pose.position) < 0.0001f, "capital restricted marker respected");
                Require(preview.Stage.Slot(side, 0).transformPathId != pose.transformPathId,
                    "ordinary ships cannot consume the capital marker (component/transform pointer types differ)");
            }
            yield return new WaitForSeconds(1);
            Capture(Output + "/fleet-original-stage.png");
            File.WriteAllText(Output + "/presentation-pass.txt", "PASS: original squad and fleet camera, attachment, slots, capital restriction, real squad UI command. Fleet is a presentation preview, not combat.\n");
            Debug.Log("[PresentationGate] PASS original stages and squad input");
            EditorApplication.Exit(0);
        }
        void CheckCamera(BattleStage stage)
        {
            Require(Vector3.Distance(Camera.main.transform.localPosition, stage.OriginalLayout.camera.position) < 0.0001f, "original camera position");
            Require(Quaternion.Angle(Camera.main.transform.localRotation, stage.OriginalLayout.camera.rotation) < 0.01f, "original camera rotation");
            Require(Mathf.Abs(Camera.main.fieldOfView - stage.OriginalLayout.camera.fov) < 0.001f, "original camera FOV");
        }
        void Capture(string path)
        {
            var cam = Camera.main;
            var rt = new RenderTexture(Screen.width, Screen.height, 24);
            cam.targetTexture = rt; cam.Render(); RenderTexture.active = rt;
            var image = new Texture2D(Screen.width, Screen.height, TextureFormat.RGB24, false);
            image.ReadPixels(new Rect(0, 0, Screen.width, Screen.height), 0, 0); image.Apply();
            File.WriteAllBytes(path, image.EncodeToPNG());
            cam.targetTexture = null; RenderTexture.active = null;
            Object.Destroy(image); Object.Destroy(rt);
        }
    }
}
