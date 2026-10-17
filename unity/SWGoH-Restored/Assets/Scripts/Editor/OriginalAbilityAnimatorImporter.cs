using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using Newtonsoft.Json.Linq;
using UnityEditor;
using UnityEditor.Animations;
using UnityEngine;

namespace SwgohRestored.EditorTools
{
    // Rebuilds the compiled controller graph exported from the original bundle.
    // Curves come exclusively from the corresponding semantic GLB clips; events
    // are installed on copies, never on the imported GLB sub-assets.
    public static class OriginalAbilityAnimatorImporter
    {
        const string Root = "Assets/Resources/AbilityPresentation";
        static JToken D(JToken t) => t?["data"] ?? t;
        static int I(JToken t, string k) => (int?)t?[k] ?? 0;
        static float F(JToken t, string k) => (float?)t?[k] ?? 0;
        static bool B(JToken t, string k) => (bool?)t?[k] ?? false;
        static IEnumerable<JToken> A(JToken t) => t as JArray ?? new JArray();
        static string Safe(string s) => string.Concat(s.Select(c => char.IsLetterOrDigit(c) || c == '_' || c == '-' ? c : '_'));

        [MenuItem("SWGoH/Restore Original Ability Animator (VADER)")]
        public static void ImportVader() => Import("VADER");

        public static void Import(string unit)
        {
            var json = JObject.Parse(File.ReadAllText($"{Root}/{unit}.json"));
            var character = json["character"];
            string bundle = (string)character["source"]["bundle"];
            string glb = $"Assets/Characters/{bundle}.anim.glb";
            var clips = AssetDatabase.LoadAllAssetsAtPath(glb).OfType<AnimationClip>().ToArray();
            if (clips.Length == 0) throw new InvalidDataException("No imported semantic animation clips: " + glb);
            var sourceControllers = A(character["controllers"]).ToArray();
            if (sourceControllers.Length != 1) throw new NotSupportedException("Expected one original controller; refusing an arbitrary controller selection.");
            string directory = $"{Root}/{unit}Clips";
            Directory.CreateDirectory(directory);
            AssetDatabase.Refresh();
            string path = $"{Root}/{unit}Animator.controller";
            var controller = AssetDatabase.LoadAssetAtPath<AnimatorController>(path);
            if (controller == null) controller = AnimatorController.CreateAnimatorControllerAtPath(path);
            // Keep the controller GUID stable across repeated imports.
            foreach (var obj in AssetDatabase.LoadAllAssetsAtPath(path))
                if (obj != controller) UnityEngine.Object.DestroyImmediate(obj, true);
            controller.layers = Array.Empty<AnimatorControllerLayer>();
            controller.parameters = Array.Empty<AnimatorControllerParameter>();
            var context = new ImportContext(character, sourceControllers[0], controller, clips, directory);
            context.Build();
            EditorUtility.SetDirty(controller);
            AssetDatabase.SaveAssets();
            AssetDatabase.ImportAsset(path);
            File.WriteAllText($"{Root}/{unit}Animator.binding-report.json", context.Report.ToString());
            AssetDatabase.Refresh();
            Debug.Log($"Original animator restored: {path}; {context.Report}");
        }

        sealed class ImportContext
        {
            readonly JToken character, source, raw, compiled;
            readonly AnimatorController controller;
            readonly AnimationClip[] imported;
            readonly string clipDirectory;
            readonly Dictionary<ulong, string> names = new Dictionary<ulong, string>();
            readonly Dictionary<string, AnimationClip> copied = new Dictionary<string, AnimationClip>();
            readonly JArray bindings = new JArray(), pending = new JArray();
            int stateCount, transitionCount, eventCount;
            public JObject Report => new JObject {
                ["sourceControllerPathId"] = source["pathId"]?.DeepClone(),
                ["sourceBundle"] = character["source"]?.DeepClone(),
                ["states"] = stateCount, ["transitions"] = transitionCount,
                ["parameters"] = controller.parameters.Length, ["clips"] = copied.Count,
                ["events"] = eventCount, ["bindings"] = bindings, ["pendingOriginal"] = pending
            };
            public ImportContext(JToken character, JToken source, AnimatorController controller, AnimationClip[] imported, string directory)
            {
                this.character = character; this.source = source; this.controller = controller;
                this.imported = imported; clipDirectory = directory;
                raw = source["raw"]; compiled = raw["m_Controller"];
                foreach (var pair in A(raw["m_TOS"])) names[(ulong)pair[0]] = (string)pair[1];
            }
            string Name(JToken hash)
            {
                ulong id = (ulong?)hash ?? 0;
                if (id == 0 || id == uint.MaxValue) return "";
                if (!names.TryGetValue(id, out string name)) throw new InvalidDataException("Missing original controller hash " + id);
                return name;
            }
            public void Build()
            {
                if (A(raw["m_StateMachineBehaviours"]).Any()) throw new NotSupportedException("Original StateMachineBehaviours require restoration.");
                var defaults = D(compiled["m_DefaultValues"]);
                foreach (var entry in A(D(compiled["m_Values"])["m_ValueArray"]))
                {
                    var p = D(entry);
                    var type = (AnimatorControllerParameterType)I(p, "m_Type");
                    int index = I(p, "m_Index");
                    var parameter = new AnimatorControllerParameter { name = Name(p["m_ID"]), type = type };
                    switch (type)
                    {
                        case AnimatorControllerParameterType.Float: parameter.defaultFloat = (float)defaults["m_FloatValues"][index]; break;
                        case AnimatorControllerParameterType.Int: parameter.defaultInt = (int)defaults["m_IntValues"][index]; break;
                        case AnimatorControllerParameterType.Bool:
                        case AnimatorControllerParameterType.Trigger: parameter.defaultBool = (bool)defaults["m_BoolValues"][index]; break;
                        default: throw new NotSupportedException("Parameter type " + type);
                    }
                    controller.AddParameter(parameter);
                }
                var layers = new List<AnimatorControllerLayer>();
                foreach (var wrapped in A(compiled["m_LayerArray"]))
                {
                    var l = D(wrapped); int mi = I(l, "m_StateMachineIndex");
                    var machine = D(compiled["m_StateMachineArray"][mi]);
                    // The compiled count includes the owning layer. Ordinary layers
                    // each own a distinct machine and use synchronized-slot index 0.
                    // It is not the editor API's syncedLayerIndex (whose sentinel is -1).
                    int users = A(compiled["m_LayerArray"]).Count(x => I(D(x), "m_StateMachineIndex") == mi);
                    if (users != 1 || I(machine, "m_SynchronizedLayerCount") > 1 || I(l, "m_StateMachineSynchronizedLayerIndex") != 0)
                        throw new NotSupportedException("Shared original state machine requires synchronized-layer motion overrides.");
                    var sm = new AnimatorStateMachine { name = Name(l["m_Binding"]) };
                    AssetDatabase.AddObjectToAsset(sm, controller);
                    var layer = new AnimatorControllerLayer {
                        name = sm.name, stateMachine = sm, defaultWeight = F(l, "m_DefaultWeight"),
                        blendingMode = (AnimatorLayerBlendingMode)I(l, "(int&)m_LayerBlendingMode"),
                        iKPass = B(l, "m_IKPass"), syncedLayerIndex = -1,
                        syncedLayerAffectsTiming = B(l, "m_SyncedLayerAffectsTiming")
                    };
                    // These are compiled humanoid muscle masks, not AvatarMask body-part bits.
                    // Generic exported rigs have no humanoid avatar; retain the source evidence.
                    var mask = l["m_BodyMask"];
                    if ((ulong?)mask?["word0"] != uint.MaxValue || (ulong?)mask?["word1"] != uint.MaxValue || (ulong?)mask?["word2"] != 524287UL)
                        pending.Add(new JObject { ["layer"] = sm.name, ["reason"] = "PENDING-ORIGINAL compiled humanoid body mask on exported generic rig", ["bodyMask"] = mask?.DeepClone() });
                    if (A(D(l["m_SkeletonMask"])["m_Data"]).Any()) throw new NotSupportedException("Nonempty original skeleton mask requires original path binding.");
                    BuildMachine(sm, machine, mi);
                    layers.Add(layer);
                }
                controller.layers = layers.ToArray();
            }
            void BuildMachine(AnimatorStateMachine sm, JToken machine, int mi)
            {
                var rawStates = A(machine["m_StateConstantArray"]).Select(D).ToArray();
                var states = new AnimatorState[rawStates.Length];
                for (int si = 0; si < states.Length; si++)
                {
                    var s = rawStates[si];
                    var bound = A(character["boundStates"]).Single(x => (string)x["controllerPathId"] == (string)source["pathId"] && I(x,"machineIndex") == mi && I(x,"stateIndex") == si);
                    var state = sm.AddState(Name(s["m_NameID"]), new Vector3(240 * (si % 3), 90 * (si / 3), 0));
                    states[si] = state; stateCount++;
                    state.speed = F(s, "m_Speed"); state.cycleOffset = F(s,"m_CycleOffset");
                    state.mirror = B(s,"m_Mirror"); state.iKOnFeet = B(s,"m_IKOnFeet");
                    state.writeDefaultValues = B(s,"m_WriteDefaultValues"); state.tag = Name(s["m_TagID"]);
                    state.speedParameter = Name(s["m_SpeedParamID"]); state.speedParameterActive = state.speedParameter.Length > 0;
                    state.mirrorParameter = Name(s["m_MirrorParamID"]); state.mirrorParameterActive = state.mirrorParameter.Length > 0;
                    state.cycleOffsetParameter = Name(s["m_CycleOffsetParamID"]); state.cycleOffsetParameterActive = state.cycleOffsetParameter.Length > 0;
                    state.timeParameter = Name(s["m_TimeParamID"]); state.timeParameterActive = state.timeParameter.Length > 0;
                    var trees = A(s["m_BlendTreeConstantArray"]).Select(D).ToArray();
                    if (trees.Length > 1) throw new NotSupportedException("Multiple compiled motion roots require restoration.");
                    if (trees.Length == 1)
                    {
                        var nodes = A(trees[0]["m_NodeArray"]).Select(D).ToArray();
                        state.motion = Motion(nodes, 0, bound, state.name, null, mi, si);
                    }
                }
                sm.defaultState = states[I(machine,"m_DefaultState")];
                for (int si = 0; si < states.Length; si++)
                    foreach (var t in A(rawStates[si]["m_TransitionConstantArray"]))
                        Transition(states[si].AddTransition(states[I(D(t),"m_DestinationState")]), D(t));
                foreach (var t in A(machine["m_AnyStateTransitionConstantArray"]))
                    Transition(sm.AddAnyStateTransition(states[I(D(t),"m_DestinationState")]), D(t));
                foreach (var entry in A(machine["m_SelectorStateConstantArray"]).Select(D))
                {
                    foreach (var t in A(entry["m_TransitionConstantArray"]).Select(D))
                    {
                        int destination = I(t,"m_Destination");
                        if (!B(entry,"m_IsEntry"))
                        {
                            if (destination != 30000 || A(t["m_ConditionConstantArray"]).Any()) throw new NotSupportedException("Nontrivial original exit selector.");
                            continue;
                        }
                        // The unconditional compiled default entry is represented by defaultState.
                        if (destination == I(machine,"m_DefaultState") && !A(t["m_ConditionConstantArray"]).Any()) continue;
                        if (destination < 0 || destination >= states.Length) throw new NotSupportedException("Original entry destination " + destination);
                        Conditions(sm.AddEntryTransition(states[destination]), t);
                    }
                }
            }
            Motion Motion(JToken[] nodes, int index, JToken bound, string semantic, float? threshold, int mi, int si)
            {
                var node = nodes[index]; var children = A(node["m_ChildIndices"]).Select(x => (int)x).ToArray();
                if (children.Length == 0)
                {
                    int clipIndex = I(node,"m_ClipID");
                    var binding = A(bound["clips"]).Single(x => I(x,"controllerClipIndex") == clipIndex);
                    return Clip(binding, semantic + (threshold.HasValue ? "@" + threshold.Value.ToString("0.0########", CultureInfo.InvariantCulture) : ""), mi, si);
                }
                if (I(node,"m_BlendType") != 0) throw new NotSupportedException("Original blend tree type " + I(node,"m_BlendType") + " requires exact restoration.");
                var tree = new BlendTree { name = semantic + " Original Blend", blendType = BlendTreeType.Simple1D, blendParameter = Name(node["m_BlendEventID"]), useAutomaticThresholds = false };
                AssetDatabase.AddObjectToAsset(tree, controller);
                var thresholds = D(node["m_Blend1dData"])["m_ChildThresholdArray"];
                var motions = new ChildMotion[children.Length];
                for (int k = 0; k < children.Length; k++)
                {
                    var child = nodes[children[k]];
                    if (F(child,"m_Duration") != 1) throw new NotSupportedException("Nonunit original child duration requires verified timeScale mapping.");
                    float value = (float)thresholds[k];
                    motions[k] = new ChildMotion { motion = Motion(nodes, children[k], bound, semantic, value, mi, si), threshold = value, timeScale = 1, cycleOffset = F(child,"m_CycleOffset"), mirror = B(child,"m_Mirror") };
                }
                tree.children = motions;
                return tree;
            }
            AnimationClip Clip(JToken binding, string semantic, int mi, int si)
            {
                string key = mi + "_" + si + "_" + semantic;
                if (copied.TryGetValue(key, out var result)) return result;
                var candidates = imported.Where(c => c.name == semantic).ToArray();
                if (candidates.Length != 1) throw new InvalidDataException($"Exact semantic clip '{semantic}' resolved {candidates.Length} times; no fuzzy fallback.");
                var original = A(character["clips"]).Single(c => (string)c["pathId"] == (string)binding["pathId"] && (string)c["sourceBundle"] == (string)binding["sourceBundle"]);
                if (F(original,"startTime") != 0) throw new NotSupportedException("Nonzero original clip start requires event time rebasing verification.");
                var clone = UnityEngine.Object.Instantiate(candidates[0]); clone.name = semantic;
                clone.frameRate = F(original,"sampleRate");
                var muscle = original["muscleClipSettings"];
                if (muscle == null) throw new InvalidDataException("Original clip loop/root settings were not exported: " + semantic);
                var settings = AnimationUtility.GetAnimationClipSettings(clone);
                settings.startTime = F(muscle,"m_StartTime"); settings.stopTime = F(muscle,"m_StopTime");
                settings.loopTime = B(muscle,"m_LoopTime"); settings.loopBlend = B(muscle,"m_LoopBlend");
                settings.loopBlendOrientation = B(muscle,"m_LoopBlendOrientation");
                settings.loopBlendPositionY = B(muscle,"m_LoopBlendPositionY");
                settings.loopBlendPositionXZ = B(muscle,"m_LoopBlendPositionXZ");
                settings.cycleOffset = F(muscle,"m_CycleOffset"); settings.mirror = B(muscle,"m_Mirror");
                settings.orientationOffsetY = F(muscle,"m_OrientationOffsetY"); settings.level = F(muscle,"m_Level");
                settings.keepOriginalOrientation = B(muscle,"m_KeepOriginalOrientation");
                settings.keepOriginalPositionY = B(muscle,"m_KeepOriginalPositionY");
                settings.keepOriginalPositionXZ = B(muscle,"m_KeepOriginalPositionXZ");
                settings.heightFromFeet = B(muscle,"m_HeightFromFeet");
                AnimationUtility.SetAnimationClipSettings(clone, settings);
                var events = new List<AnimationEvent>();
                foreach (var e in A(original["events"]))
                {
                    if ((long?)e["objectReferenceParameter"]?["m_PathID"] != 0) throw new NotSupportedException("Animation event object reference needs original PPtr resolution.");
                    events.Add(new AnimationEvent { time = F(e,"time"), functionName = (string)e["functionName"], stringParameter = (string)e["data"], floatParameter = F(e,"floatParameter"), intParameter = I(e,"intParameter"), messageOptions = (SendMessageOptions)I(e,"messageOptions") });
                }
                AnimationUtility.SetAnimationEvents(clone, events.ToArray());
                string path = clipDirectory + "/" + Safe(key) + ".anim";
                result = AssetDatabase.LoadAssetAtPath<AnimationClip>(path);
                if (result == null) { AssetDatabase.CreateAsset(clone, path); result = clone; }
                else { EditorUtility.CopySerialized(clone, result); UnityEngine.Object.DestroyImmediate(clone); EditorUtility.SetDirty(result); }
                copied[key] = result; eventCount += events.Count;
                bindings.Add(new JObject { ["semanticClip"] = semantic, ["originalName"] = binding["name"]?.DeepClone(), ["originalPathId"] = binding["pathId"]?.DeepClone(), ["sourceBundle"] = binding["sourceBundle"]?.DeepClone(), ["asset"] = path, ["events"] = events.Count });
                return result;
            }
            void Conditions(AnimatorTransitionBase transition, JToken data)
            {
                foreach (var c in A(data["m_ConditionConstantArray"]).Select(D))
                    transition.AddCondition((AnimatorConditionMode)I(c,"m_ConditionMode"), F(c,"m_EventThreshold"), Name(c["m_EventID"]));
            }
            void Transition(AnimatorStateTransition transition, JToken data)
            {
                transitionCount++;
                transition.duration = F(data,"m_TransitionDuration"); transition.offset = F(data,"m_TransitionOffset");
                transition.exitTime = F(data,"m_ExitTime"); transition.hasExitTime = B(data,"m_HasExitTime");
                transition.hasFixedDuration = B(data,"m_HasFixedDuration");
                transition.interruptionSource = (TransitionInterruptionSource)I(data,"m_InterruptionSource");
                transition.orderedInterruption = B(data,"m_OrderedInterruption"); transition.canTransitionToSelf = B(data,"m_CanTransitionToSelf");
                Conditions(transition, data);
            }
        }
    }
}
