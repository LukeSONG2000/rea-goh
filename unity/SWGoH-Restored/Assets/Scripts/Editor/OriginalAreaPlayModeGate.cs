using System.Collections;
using System.IO;
using System.Linq;
using System.Collections.Generic;
using SwgohRestored.Battle;
using UnityEditor;
using UnityEditor.SceneManagement;
using UnityEngine;
using UnityEngine.UI;

namespace SwgohRestored.EditorTools
{
    public static class OriginalAreaPlayModeGate
    {
        const string Flag = "SWGoH.AreaGate";
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
            new GameObject("OriginalAreaGate").AddComponent<OriginalAreaGateRunner>();
        }
    }

    public class OriginalAreaGateRunner : BattleCommandGateRunner
    {
        const string Output = "F:/PPfavorite/Work/goh/logs/effect_phase2";
        IEnumerator Start()
        {
            Directory.CreateDirectory(Output);
            float deadline = Time.realtimeSinceStartup + 150;
            BattleDriver driver;
            do { yield return null; driver = Object.FindObjectOfType<BattleDriver>(); }
            while ((driver == null || !driver.WaitingForCommand) && Time.realtimeSinceStartup < deadline);
            Require(driver != null && driver.WaitingForCommand, "area pilot manual input");
            var sim = driver.Simulation;
            foreach (var icon in new[] { "icon_curse", "icon_damage_over_time", "icon_buff_speed" })
            {
                var original = Resources.Load<Sprite>("BattleStatus/" + icon);
                Require(original != null && original.rect.width == 32 && original.rect.height == 32,
                    "original battle atlas sprite import: " + icon);
            }
            var actor = sim.PendingActor;
            int index = actor.Abilities.FindIndex(a => a.Id == "specialability_vader02");
            Require(index >= 0 && sim.IsAbilityReady(index), "original Force Crush must be available");
            var skill = GameObject.Find("skill_" + actor.Abilities[index].SkillId)?.GetComponent<Button>();
            Require(skill != null && skill.interactable, "Force Crush UI enabled");
            Click(skill); yield return null;
            var target = sim.GetLegalTargets(index).Last();
            int first = sim.Log.Count;
            Click(GameObject.Find("target_" + target.UnitId).GetComponent<Button>());
            var damage = sim.Log.Skip(first).Where(e => e.Kind == "damage" || e.Kind == "crit").ToArray();
            Require(damage.Length == 2 && damage.Select(e => e.TargetUnitId).Distinct().Count() == 2,
                "Force Crush hits both original targets exactly once");
            Require(damage.All(e => e.EffectId == "specialability_vader02_damage_u05"), "original AOE node");
            var dotIds = sim.Units.SelectMany(u => u.PersistentEffects).Where(e => e.Definition.type == 9)
                .Select(e => e.InstanceId).ToArray();
            Require(dotIds.Length > 0, "fixture applied real DOT instances");
            Require(actor.CooldownLeft[actor.Abilities[index].Id] == 5, "original cooldown");
            int submissions = 0;
            while (Time.realtimeSinceStartup < deadline && !sim.CheckEnd(out _))
            {
                yield return null;
                if (!driver.WaitingForCommand) continue;
                if (submissions == 0) yield return driver.CaptureFrame(Output + "/force-crush-status.png");
                bool expired = !sim.Units.SelectMany(u => u.PersistentEffects).Any(e => dotIds.Contains(e.InstanceId));
                if (expired) break;
                Require(submissions++ < 12, "DOT lifecycle did not finish");
                var active = sim.PendingActor;
                int basic = active.Abilities.FindIndex(a => a.Kind == "basic");
                Click(GameObject.Find("skill_" + active.Abilities[basic].SkillId).GetComponent<Button>());
                yield return null;
                var next = sim.GetLegalTargets(basic).Last();
                Click(GameObject.Find("target_" + next.UnitId).GetComponent<Button>());
            }
            Require(!sim.Units.SelectMany(u => u.PersistentEffects).Any(e => dotIds.Contains(e.InstanceId)), "DOT expires after final tick");
            Require(sim.Log.Any(e => e.EffectId == "basic_dot_damage" && e.Amount > 0), "nested original DOT damage executed");
            Require(sim.Log.Count(e => e.Kind == "status_remove" && dotIds.Contains(e.EffectInstanceId)) == dotIds.Length,
                "each DOT instance removed once");
            Require(driver.WaitingForCommand && sim.PendingActor.Alive, "input resumes for a living actor after start effects");
            yield return driver.CaptureFrame(Output + "/force-crush-expired.png");
            File.WriteAllText(Output + "/area-playmode-events.json", JsonUtility.ToJson(new EventTrace { events = sim.Log.ToArray() }, true));
            File.WriteAllText(Output + "/area-playmode-pass.txt", "PASS: original Force Crush UI; two targets; cooldown; independent DOTs; nested ticks and expiry; manual input resumes.");
            Debug.Log("[AreaGate] PASS original AOE, persistent instances, nested damage, expiry and UI input");
            EditorApplication.Exit(0);
        }
    }
}
