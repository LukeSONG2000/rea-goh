// Editor tooling for the restored project.
// "Import Playable Roster" copies the exported anim GLBs (273 playable units)
// into Assets/Characters/<folder>.glb -- glTFast's ScriptedImporter picks them
// up automatically.  "Build Animators" then wires every imported rig with an
// AnimatorController whose states mirror the game's own state machine names
// (catalog/char_anim_states.json, embedded as StreamingAssets/anim_states.json).
using System.Collections.Generic;
using System.IO;
using System.Linq;
using UnityEditor;
using UnityEngine;

namespace SwgohRestored.EditorTools
{
    public static class RestoredImporter
    {
        const string SourceRoot = @"F:\PPfavorite\Work\goh\extracted";
        const string DestDir = "Assets/Characters";

        [MenuItem("SWGoH/Import Playable Roster (GLBs)")]
        public static void ImportRoster()
        {
            Directory.CreateDirectory(DestDir);
            var roster = JsonUtilityWrapper.ReadList<RosterJson>(
                Path.Combine(Application.streamingAssetsPath, "roster.json"));
            int n = 0;
            foreach (var r in roster)
            {
                if (string.IsNullOrEmpty(r.folder)) continue;
                var src = Path.Combine(SourceRoot, r.folder, r.folder + ".anim.glb");
                if (!File.Exists(src))
                    src = Path.Combine(SourceRoot, r.folder, r.folder + ".glb");
                if (!File.Exists(src))
                {
                    Debug.LogWarning($"missing glb: {r.folder}");
                    continue;
                }
                var dst = Path.Combine(DestDir, r.folder + ".glb");
                if (!File.Exists(dst) || File.GetLastWriteTimeUtc(src) > File.GetLastWriteTimeUtc(dst))
                    File.Copy(src, dst, true);
                n++;
            }
            AssetDatabase.Refresh();
            Debug.Log($"[RestoredImporter] staged {n} GLBs -> {DestDir}");
        }

        [MenuItem("SWGoH/Build Animators from State Tables")]
        public static void BuildAnimators()
        {
            Directory.CreateDirectory("Assets/Animators");
            var states = JsonUtilityWrapper.ReadList<CharStates>(
                Path.Combine(Application.streamingAssetsPath, "anim_states.json"));
            int built = 0, skipped = 0;
            foreach (var cs in states)
            {
                // AssetDatabase paths must be forward-slashed: a backslash path
                // loads the main GameObject but silently drops sub-assets
                // (AnimationClips), producing empty controllers.
                var glbPath = $"{DestDir}/{cs.folder}.glb";
                var asset = AssetDatabase.LoadAssetAtPath<GameObject>(glbPath);
                if (asset == null) { skipped++; continue; }

                // glTFast puts imported clips as sub-assets of the .glb asset
                var raw = AssetDatabase.LoadAllAssetsAtPath(glbPath);
                var clips = raw.OfType<AnimationClip>()
                               .Where(c => !c.name.StartsWith("__"))
                               .ToArray();
                if (cs.folder == "char_jynerso_pre")
                    Debug.Log($"[BuildDiag] path={glbPath} rawAssets={raw.Length} " +
                              $"types={string.Join(",", raw.Select(a => a.GetType().Name).Distinct().Take(10))} " +
                              $"clips={clips.Length}");
                if (clips.Length == 0) { skipped++; continue; }

                var ctrlPath = $"Assets/Animators/{cs.folder}.controller";
                // CreateAnimatorControllerAtPath does NOT overwrite an existing
                // asset -- a stale empty controller would silently survive every
                // rebuild.  Delete first.
                AssetDatabase.DeleteAsset(ctrlPath);
                var controller = UnityEditor.Animations.AnimatorController.CreateAnimatorControllerAtPath(ctrlPath);
                var root = controller.layers[0].stateMachine;

                string defaultState = null;
                foreach (var s in cs.states)
                    if (s.@default) { defaultState = Strip(s.name); break; }

                var byName = clips.ToLookup(c => c.name);
                var added = new HashSet<string>();
                var addedStates = new List<UnityEditor.Animations.AnimatorState>();
                int gx = 0, gy = 0;
                void AddStateWithClip(string label, AnimationClip clip)
                {
                    var st = root.AddState(label);   // canonical creation API
                    st.motion = clip;
                    st.writeDefaultValues = false;
                    addedStates.Add(st);
                    if (label == defaultState) root.defaultState = st;
                }
                foreach (var s in cs.states)
                {
                    var label = Strip(s.name);
                    if (!added.Add(label)) continue;
                    var clip = byName[label].FirstOrDefault()
                               ?? byName.Where(g => g.Key.StartsWith(label)).FirstOrDefault()?.First();
                    if (clip == null) continue;
                    AddStateWithClip(label, clip);
                }
                // any clip without a state entry (blend-tree variants) still playable
                foreach (var c in clips)
                    if (!added.Contains(c.name))
                        AddStateWithClip(c.name, c);

                // the default label ("Idle") often isn't an exact state name
                // (clips are Idle@0.0/Idle@1.0) -- an Animator with no default
                // state never plays anything
                if (root.defaultState == null && addedStates.Count > 0)
                {
                    var idle = addedStates.FirstOrDefault(s => s.name.StartsWith("Idle"))
                               ?? addedStates[0];
                    root.defaultState = idle;
                }

                EditorUtility.SetDirty(controller);
                built++;
            }
            AssetDatabase.SaveAssets();
            AssetDatabase.Refresh();
            Debug.Log($"[RestoredImporter] animators built={built} skipped={skipped}");
        }

        [MenuItem("SWGoH/Setup Demo Battle Scene")]
        public static void SetupScene() => SetupSceneInternal(save: false);

        // batch-mode entry: creates the scene and saves it to disk
        public static void SetupSceneAndSave() => SetupSceneInternal(save: true);

        static void SetupSceneInternal(bool save)
        {
            var scene = UnityEngine.SceneManagement.SceneManager.GetActiveScene();
            var root = new GameObject("BattleRoot");
            var driver = root.AddComponent<global::SwgohRestored.Battle.BattleDriver>();

            var camGo = new GameObject("BattleCam");
            camGo.transform.position = new Vector3(0, 5.5f, -11f);
            camGo.transform.rotation = Quaternion.Euler(24f, 0, 0);
            var cam = camGo.AddComponent<Camera>();
            cam.backgroundColor = new Color(0.06f, 0.07f, 0.10f);
            camGo.AddComponent<AudioListener>();
            cam.tag = "MainCamera";

            var lightGo = new GameObject("KeyLight");
            lightGo.transform.position = new Vector3(6, 12, -6);
            lightGo.transform.rotation = Quaternion.Euler(45f, -35f, 0);
            lightGo.AddComponent<Light>().type = LightType.Directional;

            var floor = GameObject.CreatePrimitive(PrimitiveType.Plane);
            floor.name = "Floor";
            floor.transform.position = Vector3.zero;
            floor.transform.localScale = new Vector3(4, 1, 2);

            var imported = new GameObject("ImportedCharacters");
            imported.transform.SetParent(root.transform, false);
            driver.CharacterPrefabRoot = imported;

            // stage every imported rig: replace glTFast's legacy Animation with
            // the state-machine AnimatorController built by BuildAnimators()
            int staged = 0;
            foreach (var glb in Directory.GetFiles("Assets/Characters", "*.glb"))
            {
                var folder = Path.GetFileNameWithoutExtension(glb);
                var prefab = AssetDatabase.LoadAssetAtPath<GameObject>(glb);
                if (prefab == null) continue;
                var inst = (GameObject)PrefabUtility.InstantiatePrefab(prefab, imported.transform);
                inst.name = folder;
                inst.SetActive(false);
                var legacy = inst.GetComponentInChildren<Animation>(true);
                if (legacy != null) Object.DestroyImmediate(legacy);
                if (inst.GetComponent<Animator>() == null)
                    inst.AddComponent<Animator>();
                var ctrl = AssetDatabase.LoadAssetAtPath<RuntimeAnimatorController>(
                    $"Assets/Animators/{folder}.controller");
                if (ctrl != null)
                    inst.GetComponentInChildren<Animator>(true).runtimeAnimatorController = ctrl;
                staged++;
            }
            Debug.Log($"[RestoredImporter] staged {staged} rigs under ImportedCharacters");

            UnityEditor.SceneManagement.EditorSceneManager.MarkSceneDirty(scene);
            if (save)
            {
                Directory.CreateDirectory("Assets/Scenes");
                UnityEditor.SceneManagement.EditorSceneManager.SaveScene(scene,
                    "Assets/Scenes/BattleDemo.unity");
                Debug.Log("[RestoredImporter] demo scene saved -> Assets/Scenes/BattleDemo.unity");
            }
            else
            {
                Debug.Log("[RestoredImporter] demo scene created -- save it (Ctrl+S) then press Play");
            }
        }

        [MenuItem("SWGoH/Debug Dump Glb Assets (jyn)")]
        public static void DebugDumpGlb()
        {
            var p = "Assets/Characters/char_jynerso_pre.glb";
            var main = AssetDatabase.LoadAllAssetsAtPath(p);
            Debug.Log($"[Dump] LoadAllAssetsAtPath: {string.Join(", ", main.Select(a => $"{a.GetType().Name}:{a.name}"))}");
            var reps = AssetDatabase.LoadAllAssetRepresentationsAtPath(p);
            Debug.Log($"[Dump] Representations: {string.Join(", ", reps.Select(a => $"{a.GetType().Name}:{a.name}"))}");

            var ctrl = AssetDatabase.LoadAssetAtPath<UnityEditor.Animations.AnimatorController>(
                "Assets/Animators/char_jynerso_pre.controller");
            if (ctrl == null) { Debug.Log("[Dump] controller: NULL"); return; }
            var sm = ctrl.layers[0].stateMachine;
            Debug.Log($"[Dump] controller states={sm.states.Length} default={(sm.defaultState ? sm.defaultState.name : "NULL")} " +
                      $"names=[{string.Join(",", sm.states.Select(s => s.state.name))}]");
        }

        static string Strip(string stateName)
        {
            var i = stateName.IndexOf('.');
            return i >= 0 ? stateName.Substring(i + 1).Trim() : stateName.Trim();
        }

        // ---- JSON shapes (anim_states.json: [{folder, states:[{name,default}]}]) ----
        [System.Serializable]
        public class RosterJson { public string def_id, name, folder; public bool anim; }

        [System.Serializable]
        public class StateJson { public string name; public bool @default; public bool loop; }

        [System.Serializable]
        public class CharStates { public string folder; public List<StateJson> states; }
    }

    static class JsonUtilityWrapper
    {
        [System.Serializable]
        class Wrapper<T> { public T[] items; }
        public static List<T> ReadList<T>(string path)
        {
            var json = File.ReadAllText(path);
            return JsonUtility.FromJson<Wrapper<T>>("{\"items\":" + json + "}").items.ToList();
        }
    }
}
