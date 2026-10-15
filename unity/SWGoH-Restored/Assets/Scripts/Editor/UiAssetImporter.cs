// Editor tooling for the UI screens (P3 roster / P4 detail).
//
// "SWGoH/UI/1 Import All UI Assets":
//   - copies the ORIGINAL harvested NGUI widget sprites
//     (extracted_ui/atlas_sprites/* -- sprite rects cropped from the original
//     atlas textures by extracted_ui/harvest_ui_assets.py) into
//     Assets/Resources/UiSprites/widgets
//   - copies the matched original charui portraits into .../portraits
//     (def_id mapping from StreamingAssets/ui_units.json, staged by
//     unity/stage_ui_data.py from catalog/units.json thumbnailName)
//   - copies the original ability icons into .../icons
//   - configures every texture as a uGUI Sprite (border -> spriteBorder)
//
// "SWGoH/UI/2 Build Roster+Detail Scenes": creates Assets/Scenes/
// RosterScene.unity and DetailScene.unity (camera, lights, pedestal, staged
// GLB rigs) and registers them in EditorBuildSettings.
//
// "SWGoH/UI/3 Capture UI Screens (batch entry)": batch-mode entry -- imports,
// builds scenes, drops the capture marker, opens RosterScene and enters play
// mode; the runtime UiCaptureDriver then captures ui_roster.png, chains into
// DetailScene, captures ui_detail.png and quits.
using System.Collections.Generic;
using System.IO;
using System.Linq;
using UnityEditor;
using UnityEditor.SceneManagement;
using UnityEngine;

namespace SwgohRestored.EditorTools
{
    public static class UiAssetImporter
    {
        const string HarvestRoot = @"F:\PPfavorite\Work\goh\extracted_ui\atlas_sprites";
        const string PortraitRoot = @"F:\PPfavorite\Work\goh\extracted_ui\charui";
        const string IconRoot = @"F:\PPfavorite\Work\goh\extracted_ui\atlas_icons";
        const string WidgetDest = "Assets/Resources/UiSprites/widgets";
        const string PortraitDest = "Assets/Resources/UiSprites/portraits";
        const string IconDest = "Assets/Resources/UiSprites/icons";

        // original widget sprites used by RosterUI/DetailUI (name -> search order
        // across harvest bundles; crops are identical everywhere)
        static readonly string[] Widgets =
        {
            "BIT_Portrait_Frame", "rarity_star", "rarity_star_empty", "player_level",
            "level_label_bg", "button_back", "button_home", "bg_tab_idle", "bg_tab_highlight",
            "side_nav_btn_unselected", "side_nav_btn_selected", "side_nav_bg", "side_nav_divider",
            "arrow_tab", "counter_currency_small", "scroll_bar_handle", "scroll_bar_track",
            "GearCheckbox", "filter_button_small", "RoundedRect_4pt", "RoundedRectDisabled_4pt",
            "bg_pillpanel", "detail_relic_background", "icon_relic_badge", "detail_ability_button",
            "detail_ability_button_cta", "detail_ability_button_pressed", "detail_round_button",
            "detail_round_button_cta", "detail_ultimate_button", "detail_radial_progress",
            "SlotAdditionSign", "Badge_Notif", "pip_gold", "pip_blue", "xmark", "checkmark_large",
            "button_blue", "button_blue_pressed", "button_56", "button_64", "glow_ring",
            "unit_border", "tab_corner", "HeaderDeco_Gold", "modchip_diamond_base",
            "modchip_diamond_light", "BG_small_box_gradient", "bg_modal_02", "scroll_list_item_info",
            "slot_nopanel", "squareframe_grayscale", "Circle_Stroke_2pt", "Circle_Stroke_2pt_Glow",
        };

        [MenuItem("SWGoH/UI/1 Import All UI Assets")]
        public static void ImportAll()
        {
            var units = ReadUnitList();
            ImportWidgets();
            ImportPortraits(units);
            ImportIcons(units);
            AssetDatabase.Refresh();
            AssetDatabase.SaveAssets();
            Debug.Log("[UiAssetImporter] import complete");
        }

        static List<SwgohRestored.UI.UnitEntry> ReadUnitList()
        {
            var path = Path.Combine(Application.streamingAssetsPath, "ui_units.json");
            var json = "{\"items\":" + File.ReadAllText(path) + "}";
            return JsonUtility.FromJson<UnitList>(json).items.ToList();
        }

        [System.Serializable]
        class UnitList { public SwgohRestored.UI.UnitEntry[] items; }

        static void Configure(string assetPath, int[] border)
        {
            var ti = AssetImporter.GetAtPath(assetPath) as TextureImporter;
            if (ti == null) return;
            ti.textureType = TextureImporterType.Sprite;
            ti.spriteImportMode = SpriteImportMode.Single;
            ti.mipmapEnabled = false;
            ti.filterMode = FilterMode.Bilinear;
            ti.alphaIsTransparency = true;
            ti.textureCompression = TextureImporterCompression.Uncompressed;
            ti.maxTextureSize = 4096;
            if (border != null && border.Length == 4 && (border[0] > 0 || border[1] > 0 || border[2] > 0 || border[3] > 0))
                ti.spriteBorder = new Vector4(border[0], border[1], border[2], border[3]);
            ti.SaveAndReimport();
        }

        static void ImportWidgets()
        {
            Directory.CreateDirectory(WidgetDest);
            // prefer the canonical shared bundles first
            string[] order = { "shared_uicontainer", "unit_details_atlas", "inventoryui_masterinventoryview" };
            int imported = 0;
            foreach (var name in Widgets)
            {
                string src = null;
                foreach (var bundle in order)
                {
                    var cand = Path.Combine(HarvestRoot, bundle, name + ".png");
                    if (File.Exists(cand)) { src = cand; break; }
                }
                if (src == null)
                {
                    Debug.LogWarning("[UiAssetImporter] widget not harvested: " + name);
                    continue;
                }
                var dst = Path.Combine(WidgetDest, SwgohRestored.UI.UiKit.Sanitize(name) + ".png");
                File.Copy(src, dst, true);
                Configure(dst, null);
                imported++;
            }
            Debug.Log($"[UiAssetImporter] widgets imported: {imported}/{Widgets.Length}");
        }

        static void ImportPortraits(List<SwgohRestored.UI.UnitEntry> units)
        {
            Directory.CreateDirectory(PortraitDest);
            int ok = 0, miss = 0;
            foreach (var u in units)
            {
                if (string.IsNullOrEmpty(u.portrait)) { miss++; continue; }
                var src = Path.Combine(PortraitRoot, u.portrait + ".png");
                if (!File.Exists(src)) { miss++; continue; }
                var dst = Path.Combine(PortraitDest, u.def_id + ".png");
                File.Copy(src, dst, true);
                Configure(dst, null);
                ok++;
            }
            int total = units.Count;
            Debug.Log($"[UiAssetImporter] portraits imported: {ok}/{total} " +
                      $"(match rate {100.0 * ok / total:0.0}% -- {miss} units without a charui portrait)");
        }

        static void ImportIcons(List<SwgohRestored.UI.UnitEntry> units)
        {
            Directory.CreateDirectory(IconDest);
            var wanted = new HashSet<string>();
            foreach (var u in units)
                foreach (var sk in u.skills ?? new List<SwgohRestored.UI.SkillEntry>())
                    if (!string.IsNullOrEmpty(sk.icon))
                        wanted.Add(sk.icon);
            int ok = 0, miss = 0;
            foreach (var icon in wanted)
            {
                var src = Path.Combine(IconRoot, icon + ".png");
                if (!File.Exists(src)) { miss++; continue; }
                var dst = Path.Combine(IconDest, SwgohRestored.UI.UiKit.Sanitize(icon) + ".png");
                File.Copy(src, dst, true);
                Configure(dst, null);
                ok++;
            }
            Debug.Log($"[UiAssetImporter] ability icons imported: {ok}/{wanted.Count} ({miss} not harvested)");
        }

        // ---------------- scene building ----------------

        [MenuItem("SWGoH/UI/2 Build Roster+Detail Scenes")]
        public static void BuildScenes()
        {
            BuildRosterScene();
            BuildDetailScene();
            EditorBuildSettings.scenes = new[]
            {
                new EditorBuildSettingsScene("Assets/Scenes/BattleDemo.unity", true),
                new EditorBuildSettingsScene("Assets/Scenes/RosterScene.unity", true),
                new EditorBuildSettingsScene("Assets/Scenes/DetailScene.unity", true),
            };
            Debug.Log("[UiAssetImporter] scenes built + build settings updated");
        }

        static Camera MakeCamera(string name, Vector3 pos, float fov, Color bg)
        {
            var go = new GameObject(name);
            go.transform.position = pos;
            var cam = go.AddComponent<Camera>();
            cam.fieldOfView = fov;
            cam.backgroundColor = bg;
            cam.clearFlags = CameraClearFlags.SolidColor;
            cam.nearClipPlane = 0.05f;      // UiKit.SetCamera puts the SSC canvas plane at 0.2
            cam.tag = "MainCamera";
            go.AddComponent<AudioListener>();
            return cam;
        }

        static Material Mat(Color color, Color emission)
        {
            var m = new Material(Shader.Find("Standard"));
            m.color = color;
            m.EnableKeyword("_EMISSION");
            m.SetColor("_EmissionColor", emission);
            return m;
        }

        static void BuildRosterScene()
        {
            var scene = EditorSceneManager.NewScene(NewSceneSetup.EmptyScene, NewSceneMode.Single);
            var root = new GameObject("RosterRoot");
            root.AddComponent<SwgohRestored.UI.RosterUI>();
            MakeCamera("UICam", new Vector3(0, 0, -10), 60, new Color(0.016f, 0.024f, 0.045f));
            Directory.CreateDirectory("Assets/Scenes");
            EditorSceneManager.SaveScene(scene, "Assets/Scenes/RosterScene.unity");
        }

        static void BuildDetailScene()
        {
            var scene = EditorSceneManager.NewScene(NewSceneSetup.EmptyScene, NewSceneMode.Single);
            var root = new GameObject("DetailRoot");
            var ui = root.AddComponent<SwgohRestored.UI.DetailUI>();
            MakeCamera("UICam", new Vector3(0, 1.7f, -3.9f), 32, new Color(0.02f, 0.03f, 0.055f))
                .transform.rotation = Quaternion.Euler(11, 0, 0);

            var key = new GameObject("KeyLight");
            key.transform.rotation = Quaternion.Euler(38f, -28f, 0);
            var kl = key.AddComponent<Light>();
            kl.type = LightType.Directional;
            kl.intensity = 1.6f;
            var rim = new GameObject("RimLight");
            rim.transform.rotation = Quaternion.Euler(200f, 160f, 0);
            var rl = rim.AddComponent<Light>();
            rl.type = LightType.Directional;
            rl.intensity = 0.7f;
            rl.color = new Color(0.55f, 0.72f, 1f);
            var fill = new GameObject("FrontFill");
            fill.transform.rotation = Quaternion.Euler(12f, 8f, 0);
            var fl = fill.AddComponent<Light>();
            fl.type = LightType.Directional;
            fl.intensity = 0.5f;
            fl.color = new Color(1f, 0.95f, 0.85f);

            var floor = GameObject.CreatePrimitive(PrimitiveType.Plane);
            floor.name = "Floor";
            floor.transform.localScale = new Vector3(8, 1, 8);
            floor.GetComponent<MeshRenderer>().sharedMaterial =
                Mat(new Color(0.20f, 0.24f, 0.30f), Color.black);

            // holographic pedestal: flat gold emissive disc + glow ring
            // (REFERENCE-MEASURED from P4; original holo platform art is 3D env
            // content -- PENDING-ORIGINAL)
            var disc = GameObject.CreatePrimitive(PrimitiveType.Cylinder);
            disc.name = "PedestalDisc";
            disc.transform.localScale = new Vector3(1.6f, 0.015f, 1.6f);
            disc.transform.position = new Vector3(0, 0.02f, 0);
            disc.GetComponent<MeshRenderer>().sharedMaterial =
                Mat(new Color(0.85f, 0.70f, 0.35f), new Color(0.45f, 0.35f, 0.12f));
            var ring = GameObject.CreatePrimitive(PrimitiveType.Cylinder);
            ring.name = "PedestalRing";
            ring.transform.localScale = new Vector3(2.3f, 0.004f, 2.3f);
            ring.transform.position = new Vector3(0, 0.012f, 0);
            ring.GetComponent<MeshRenderer>().sharedMaterial =
                Mat(new Color(0.90f, 0.78f, 0.45f), new Color(0.6f, 0.48f, 0.18f));

            var glowLight = new GameObject("PedestalLight");
            glowLight.transform.position = new Vector3(0, 0.4f, 0);
            var gl = glowLight.AddComponent<Light>();
            gl.type = LightType.Point;
            gl.range = 5f;
            gl.color = new Color(1f, 0.85f, 0.5f);
            gl.intensity = 0.8f;

            // stage every imported rig, same recipe as BattleDemo
            // 3D rig mount -- must live OUTSIDE the canvas host GameObject:
            // the uGUI canvas drives its host transform every frame while in
            // ScreenSpaceCamera mode, and children would ride along with it
            var imported = new GameObject("ImportedCharacters");
            ui.CharacterPrefabRoot = imported.transform;
            var mount = new GameObject("RigMount");
            ui.RigMount = mount.transform;
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
            Debug.Log($"[UiAssetImporter] detail scene staged {staged} rigs");

            EditorSceneManager.SaveScene(scene, "Assets/Scenes/DetailScene.unity");
        }

        // ---------------- batch capture ----------------

        const string Marker = @"F:\PPfavorite\Work\goh\unity\ui_capture.marker";

        [MenuItem("SWGoH/UI/3 Capture UI Screens (batch entry)")]
        public static void CaptureUiScreens() => CaptureUiScreensInternal();

        public static void BatchCaptureUiScreens() => CaptureUiScreensInternal();

        static void CaptureUiScreensInternal()
        {
            ImportAll();
            BuildScenes();
            File.Delete(@"F:\PPfavorite\Work\goh\unity\ui_capture_detail.marker");
            File.WriteAllText(Marker, "ui-capture");
            EditorSceneManager.OpenScene("Assets/Scenes/RosterScene.unity");
            EditorApplication.isPlaying = true;
        }
    }
}
