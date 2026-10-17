using System.Collections;
using System.IO;
using System.Linq;
using SwgohRestored.Battle;
using UnityEditor;
using UnityEditor.SceneManagement;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;
using System.Collections.Generic;

namespace SwgohRestored.EditorTools
{
    public static class BattleCommandPlayModeGate
    {
        const string Flag = "SWGoH.CommandGate";
        public static void Run()
        {
            SessionState.SetBool(Flag, true);
            EditorSceneManager.OpenScene("Assets/Scenes/BattleDemo.unity");
            EditorApplication.isPlaying = true;
        }

        [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.AfterSceneLoad)]
        static void StartGate()
        {
            if (!SessionState.GetBool(Flag, false)) return;
            SessionState.SetBool(Flag, false);
            new GameObject("CommandRegressionGate").AddComponent<BattleCommandGateRunner>();
        }
    }

    public class BattleCommandGateRunner : MonoBehaviour
    {
        const string Output = "F:/PPfavorite/Work/goh/logs/effect_recovery";
        void Require(bool condition, string message)
        {
            if (condition) return;
            Debug.LogError("[CommandGate] FAIL " + message);
            EditorApplication.Exit(1);
            throw new System.Exception(message);
        }

        string State(BattleSim sim) => sim.Round + ":" + sim.Log.Count + ":" +
            string.Join(";", sim.Units.Select(u => u.UnitId + ":" + u.CurrentHp + ":" + u.SpeedFill));

        void Click(Button button)
        {
            Canvas.ForceUpdateCanvases();
            var canvas = button.GetComponentInParent<Canvas>();
            var rt = (RectTransform)button.transform;
            var corners = new Vector3[4];
            rt.GetWorldCorners(corners);
            foreach (var corner in corners)
            {
                var screen = RectTransformUtility.WorldToScreenPoint(
                    canvas.renderMode == RenderMode.ScreenSpaceOverlay ? null : canvas.worldCamera, corner);
                Require(screen.x >= 0 && screen.x <= Screen.width && screen.y >= 0 && screen.y <= Screen.height,
                    "button clipped: " + button.name);
            }
            var position = RectTransformUtility.WorldToScreenPoint(
                canvas.renderMode == RenderMode.ScreenSpaceOverlay ? null : canvas.worldCamera,
                rt.TransformPoint(rt.rect.center));
            Require(position.x >= 0 && position.x <= Screen.width && position.y >= 0 && position.y <= Screen.height,
                "button outside screen: " + button.name);
            var pointer = new PointerEventData(EventSystem.current) { position = position, button = PointerEventData.InputButton.Left };
            var hits = new List<RaycastResult>();
            EventSystem.current.RaycastAll(pointer, hits);
            Require(hits.Count > 0 && hits[0].gameObject.GetComponentInParent<Button>() == button,
                "button blocked by raycast: " + button.name);
            ExecuteEvents.ExecuteHierarchy(hits[0].gameObject, pointer, ExecuteEvents.pointerClickHandler);
        }

        IEnumerator Start()
        {
            Directory.CreateDirectory(Output);
            float deadline = Time.realtimeSinceStartup + 90;
            BattleDriver driver;
            do { yield return null; driver = Object.FindObjectOfType<BattleDriver>(); }
            while ((driver == null || !driver.WaitingForCommand) && Time.realtimeSinceStartup < deadline);
            Require(driver != null && driver.WaitingForCommand, "manual input must become available");
            var sim = driver.Simulation;
            string before = State(sim);
            yield return new WaitForSeconds(2);
            Require(State(sim) == before, "waiting changed combat state");
            var actor = sim.PendingActor;
            int skill = actor.Abilities.FindIndex(a => a.Kind == "basic");
            Require(skill >= 0, "default actor has basic");
            var skillButton = GameObject.Find("skill_" + actor.Abilities[skill].SkillId)?.GetComponent<Button>();
            Require(skillButton != null && skillButton.interactable, "skill button ready");
            Require(skillButton.transform.Find("icon").GetComponent<Image>().sprite != null, "original skill icon mapping");
            Click(skillButton);
            yield return null;
            Require(State(sim) == before, "selecting ability committed combat before selecting target");
            var targets = sim.GetLegalTargets(skill);
            Require(targets.Count > 1, "multiple targets for choice test");
            var target = targets[targets.Count - 1];
            yield return driver.CaptureFrame(Output + "/manual-select-target.png");
            int first = sim.Log.Count;
            int round = sim.Round;
            var targetButton = GameObject.Find("target_" + target.UnitId)?.GetComponent<Button>();
            Require(targetButton != null, "target UI button");
            Click(targetButton);
            Require(!driver.WaitingForCommand && sim.Round == round + 1, "button must commit exactly once");
            Require(sim.Log.Skip(first).Any(e => e.Kind == "turn" && e.TargetUnitId == target.UnitId && e.AbilityId == actor.Abilities[skill].Id), "UI selected wrong skill/target");
            var hits = sim.Log.Skip(first).Where(e => e.Kind == "damage" || e.Kind == "crit").ToArray();
            Require(actor.Abilities[skill].OriginalGraph != null, "production must bind original graph");
            Require(hits.Length == 1 && hits[0].EffectId == "basicability_vader_damage_u05",
                "Vader ordinary branch must execute exactly one original damage effect");
            Require(hits[0].ShieldDamage > 0 && hits[0].HpDamage == 0,
                "original protection absorbs damage without reducing HP");
            File.WriteAllText(Output + "/pilot-command-events.json", JsonUtility.ToJson(new EventTrace {
                events = sim.Log.Skip(first).ToArray() }, true));
            // Double click must not commit another turn while the action is presented.
            targetButton.onClick.Invoke();
            Require(sim.Round == round + 1, "double click committed extra turn");
            yield return new WaitForSeconds(0.7f);
            yield return driver.CaptureFrame(Output + "/manual-action.png");
            while (!driver.WaitingForCommand && !sim.CheckEnd(out _) && Time.realtimeSinceStartup < deadline)
                yield return null;
            Require(driver.WaitingForCommand, "next player turn did not wait");
            before = State(sim);
            yield return new WaitForSeconds(1);
            Require(State(sim) == before, "next manual turn auto-advanced");
            var auto = GameObject.Find("ctl_auto").GetComponent<Button>();
            Click(auto);
            yield return null;
            Require(!driver.WaitingForCommand, "auto toggle did not release pending turn");
            // Turn automatic off during animation; the next player must wait again.
            Click(auto);
            while (!driver.WaitingForCommand && !sim.CheckEnd(out _) && Time.realtimeSinceStartup < deadline)
                yield return null;
            Require(driver.WaitingForCommand, "auto off did not restore input");
            yield return driver.CaptureFrame(Output + "/manual-resumed.png");
            Debug.Log("[CommandGate] PASS UI raycast + pointer clicks, wait, skill, target, double-click, auto on/off and screenshots");
            File.WriteAllText(Output + "/command-playmode-pass.txt", "PASS: UI raycast + pointer clicks; waiting; skill and target; double click; auto on/off.");
            EditorApplication.Exit(0);
        }

        [System.Serializable] public class EventTrace { public BattleEvent[] events; }
    }
}
