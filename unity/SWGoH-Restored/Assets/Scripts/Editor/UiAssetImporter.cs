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
using System;
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

        // P5 battle HUD widgets (shared_uicontainer harvest -- the same-named
        // battleui_view crops came out fully transparent, see p5 sprite survey).
        // Sprites listed with a border get spriteBorder so uGUI sliced Images
        // keep their corners when stretched to button size.
        static readonly Dictionary<string, int[]> BattleWidgets = new Dictionary<string, int[]>
        {
            // commander portrait ring + unit health bars
            { "turn_order_frame_opponent", null },
            { "turn_order_frame_allies", null },
            { "unit_health_bg", null },
            { "unit_health_fill", null },
            { "unit_health_fill_glow", null },
            { "Stat_Bar_Bg", null },
            { "Stat_Bar_Fill", null },
            { "progressbar_clipped_bg", null },
            { "progressbar_clipped_bg_small", null },
            // key-binding chips (per-unit letter markers + skill key captions)
            { "bg_key", new[] { 14, 14, 14, 14 } },
            { "keyboard_binding_bg", null },
            // top-left controls row (auto / speed / lock)
            { "auto_battle", null },
            { "bg_toggle", new[] { 16, 16, 16, 16 } },
            { "bg_toggle_icon", null },
            { "button_toggle_icon", null },
            { "lock_filled", null },
            { "icon_flat_lock", null },
            { "button_locked", null },
            { "button_unlocked", null },
            { "btn_circle_idle", null },
            { "btn_circle_selected", null },
            { "btn_circle_highlight", null },
            { "max_set_circle_button", null },
            { "hud_button", null },
            { "hud_button_press", null },
            { "hud_button_disabled", null },
            { "hud_button_highlight", null },
            { "icon_play", null },
            { "icon_stat_speed", null },
            { "icon_timer", null },
            // buff / debuff chips (white silhouettes are tinted at runtime,
            // exactly like the original NGUI tinting)
            { "indicator_buff", null },
            { "indicator_debuff", null },
            { "icon_buff_attack", null },
            { "icon_buff_armor", null },
            { "icon_buff_health", null },
            { "icon_buff_speed", null },
            { "icon_buff_crit_chance", null },
            { "icon_buff_critical_damage", null },
            { "icon_buff_defense", null },
            { "icon_buff_generic", null },
            { "icon_buff_taunt", null },
            { "icon_debuff_taunt", null },
            { "icon_debuff_disarm", null },
            // bottom skill bar ability button frames
            { "battle_btn_frame_gold", new[] { 8, 10, 8, 10 } },
            { "battle_btn_frame_red", new[] { 8, 10, 8, 10 } },
            { "battle_btn_attack_sliced", new[] { 8, 10, 8, 10 } },
            { "battle_btn_round", null },
            { "battle_btn_round_glow", null },
            { "progress_circle_blue", null },
            { "progress_circle_empty", null },
        };

        [MenuItem("SWGoH/UI/4 Import Battle HUD Sprites (P5)")]
        public static void ImportBattleHud()
        {
            Directory.CreateDirectory(WidgetDest);
            var shared = Path.Combine(HarvestRoot, "shared_uicontainer");
            var copied = new List<KeyValuePair<string, int[]>>();
            foreach (var kv in BattleWidgets)
            {
                var src = Path.Combine(shared, kv.Key + ".png");
                if (!File.Exists(src))
                {
                    Debug.LogWarning("[UiAssetImporter] battle widget not harvested: " + kv.Key);
                    continue;
                }
                var dst = Path.Combine(WidgetDest, SwgohRestored.UI.UiKit.Sanitize(kv.Key) + ".png");
                File.Copy(src, dst, true);
                copied.Add(kv);
            }
            // new files must be imported before Configure can reach their
            // TextureImporter (GetAtPath is null until Refresh) -- otherwise
            // they stay plain textures and Resources.Load<Sprite> misses them
            AssetDatabase.Refresh();
            foreach (var kv in copied)
                Configure(Path.Combine(WidgetDest, SwgohRestored.UI.UiKit.Sanitize(kv.Key) + ".png"), kv.Value);
            AssetDatabase.SaveAssets();
            Debug.Log($"[UiAssetImporter] battle HUD widgets imported: {copied.Count}/{BattleWidgets.Count}");
        }

        // P2 home-shell widgets (homebase harvest + shared energy/currency
        // icons).  Resolved through the atlas index so sprites harvested in
        // any bundle are found (icon_collection/icon_inbox live in the
        // allies bundles, chat/icon_time in shared, etc.).  Sprites with a
        // border get spriteBorder for uGUI sliced Images.
        static readonly Dictionary<string, int[]> HomeWidgets = new Dictionary<string, int[]>
        {
            // top-bar energy bolts + purple crystal + counter pills
            { "icon_pathofconquest_energy", null },   // counter 1: blue hex bolt
            { "icon_mod_energy", null },              // counter 2: green bolt
            { "icon_cantina_energy", null },          // counter 3: red bolt
            // counter 4: the gold bolt-in-circle variant was not in the
            // harvest -- blue ship bolt stands in (HomeUI lists it
            // PENDING-ORIGINAL)
            { "icon_ship_energy", null },
            { "icon_premium", null },                 // 紫色晶体
            { "counter_currency_small", null },
            { "counter_currency", null },
            { "player_level", null },
            { "button_home", null },
            { "button_settings", null },
            { "icon_plus", null },
            // left rail
            { "icon_collection", null },
            { "icon_journeyguide", null },
            { "icon_guild", null },                   // 盟友 handshake
            { "icon_inbox", null },                   // 邮箱 envelope
            { "Badge_Notif", null },                  // red 3+ badge
            { "bg_key", new[] { 14, 14, 14, 14 } },   // key/number chips
            { "keyboard_binding_bg", null },
            // right rail event cards
            { "event_icon", null },
            { "icon_decks", null },
            { "btn_circle_idle", null },
            { "btn_circle_selected", null },
            { "icon_time", null },
            // bottom stubs
            { "button_chat", null },
            { "bg_pillpanel", new[] { 10, 10, 10, 10 } },
            { "bg_green", new[] { 10, 10, 10, 10 } }, // 开始 CTA
        };

        // bundle preference when one sprite name was harvested several times
        // (crops are identical everywhere; some bundles came out transparent)
        static readonly string[] HarvestOrder =
        {
            "shared_uicontainer", "alliesui_allycodeview", "homebaseui_view",
            "homebaseui_v2_view", "guildraidmission_shared_header_component",
            "accountmanagementui_successview", "battleui_coliseum_results_view",
            "guildactivties_raids_card", "championship_atlas",
        };

        [MenuItem("SWGoH/UI/5 Import Home Sprites (P2)")]
        public static void ImportHomeSprites()
        {
            Directory.CreateDirectory(WidgetDest);
            var copied = new List<KeyValuePair<string, int[]>>();
            foreach (var kv in HomeWidgets)
            {
                var src = ResolveHarvest(kv.Key);
                if (src == null)
                {
                    Debug.LogWarning("[UiAssetImporter] home sprite not harvested: " + kv.Key);
                    continue;
                }
                var dst = Path.Combine(WidgetDest, SwgohRestored.UI.UiKit.Sanitize(kv.Key) + ".png");
                File.Copy(src, dst, true);
                copied.Add(kv);
            }
            AssetDatabase.Refresh();
            foreach (var kv in copied)
                Configure(Path.Combine(WidgetDest, SwgohRestored.UI.UiKit.Sanitize(kv.Key) + ".png"), kv.Value);
            AssetDatabase.SaveAssets();
            Debug.Log($"[UiAssetImporter] home sprites imported: {copied.Count}/{HomeWidgets.Count}");
        }

        static string _harvestIndex;
        static string ResolveHarvest(string sprite)
        {
            // preferred bundles first, then anything the index knows
            foreach (var bundle in HarvestOrder)
            {
                var cand = Path.Combine(HarvestRoot, bundle, sprite + ".png");
                if (File.Exists(cand)) return cand;
            }
            try
            {
                if (_harvestIndex == null)
                    _harvestIndex = File.ReadAllText(
                        Path.Combine(HarvestRoot, "index.json"));
                var needle = "\"" + sprite + "\": {\"sprite\": \"" + sprite + "\", \"atlas\": \"";
                int at = 0;
                while ((at = _harvestIndex.IndexOf(needle, at, StringComparison.Ordinal)) >= 0)
                {
                    int a0 = at + needle.Length;
                    int a1 = _harvestIndex.IndexOf('"', a0);
                    var bundle = _harvestIndex.Substring(a0, a1 - a0);
                    var cand = Path.Combine(HarvestRoot, bundle, sprite + ".png");
                    if (File.Exists(cand) && !IsTransparent(cand)) return cand;
                    at = a1;
                }
            }
            catch { /* index fallback failed -- treat as not harvested */ }
            return null;
        }

        static bool IsTransparent(string png)
        {
            try
            {
                var tex = new Texture2D(2, 2, TextureFormat.RGBA32, false);
                if (!tex.LoadImage(File.ReadAllBytes(png))) return true;
                var px = tex.GetPixels32();
                bool any = false;
                for (int i = 0; i < px.Length; i++)
                    if (px[i].a > 4) { any = true; break; }
                UnityEngine.Object.DestroyImmediate(tex);
                return !any;
            }
            catch { return false; }
        }

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
            var scenes = new List<EditorBuildSettingsScene>
            {
                new EditorBuildSettingsScene("Assets/Scenes/BattleDemo.unity", true),
                new EditorBuildSettingsScene("Assets/Scenes/RosterScene.unity", true),
                new EditorBuildSettingsScene("Assets/Scenes/DetailScene.unity", true),
            };
            // P2 home shell joins the chain once built (SWGoH/UI/6)
            if (Directory.Exists("Assets/EnvBackdrop") &&
                AssetDatabase.LoadAssetAtPath<GameObject>(EnvBackdropAsset) != null)
                scenes.Add(new EditorBuildSettingsScene("Assets/Scenes/HomeScene.unity", true));
            EditorBuildSettings.scenes = scenes.ToArray();
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
                if (legacy != null) UnityEngine.Object.DestroyImmediate(legacy);
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

        // ---------------- P2 home scene ----------------

        // The cantina backdrop is the ORIGINAL env GLB (the true home-screen
        // environment; env_bar_* are cantina-adjacent environments but
        // env_home_screen_cantina_01_pre is literally the home screen set).
        const string EnvSourceGlb =
            @"F:\PPfavorite\Work\goh\extracted_env\env_home_screen_cantina_01_pre\env_home_screen_cantina_01_pre.glb";
        const string EnvBackdropAsset = "Assets/EnvBackdrop/env_home_screen_cantina_01_pre.glb";

        // hotspot anchors in cantina-frame space -- (along=units into the room
        // from the bar, side=+cross(up,front), up) -- SOLVED numerically against
        // the true camera projection so the P2 labels land on the reference's
        // screen spots over this env: bar counter = 船运(left) / 商店(right),
        // corner game tables = 竞技场(left) / 战役(right) upper edge +
        // 角斗场(left) / 达斯·加·加(right) lower edge
        static readonly Vector3[] HotspotFrame =
        {
            new Vector3(11.20f,  3.41f, 2.94f),// 竞技场 S (left table, upper)
            new Vector3(11.32f,  1.90f, 1.89f),// 角斗场 D (left table, lower)
            new Vector3(11.14f, -1.30f, 1.88f),// 达斯·加·加 F (right table, lower)
            new Vector3(10.96f, -2.77f, 2.78f),// 战役 G (right table, upper)
            new Vector3(1.2f,   -5.31f, 2.51f),// 商店 R (bar right)
            new Vector3(1.5f,    5.39f, 2.59f),// 船运货物 E (bar left)
        };

        [MenuItem("SWGoH/UI/6 Build HomeScene (P2)")]
        public static void BuildHomeScene()
        {
            EnsureEnvBackdrop();
            var scene = EditorSceneManager.NewScene(NewSceneSetup.EmptyScene, NewSceneMode.Single);

            // ---- original cantina env as static backdrop ----
            var prefab = AssetDatabase.LoadAssetAtPath<GameObject>(EnvBackdropAsset);
            var env = (GameObject)PrefabUtility.InstantiatePrefab(prefab);
            env.name = "CantinaEnv";
            env.isStatic = true;
            foreach (var a in env.GetComponentsInChildren<Animation>(true))
                UnityEngine.Object.DestroyImmediate(a);          // static backdrop, no doors
            foreach (var a in env.GetComponentsInChildren<Animator>(true))
                UnityEngine.Object.DestroyImmediate(a);

            // The env is a huge complex (terrace floors span 140+ units) --
            // frame on the BAR: the barback renderer marks the back wall side,
            // bar-front = the open floor where the P2 hotspots sit.
            var bar = ComputeCantinaFrame(env);

            // ---- lighting: 1 directional + ambient ----
            var key = new GameObject("KeyLight");
            key.transform.rotation = Quaternion.Euler(42f, -34f, 0);
            var kl = key.AddComponent<Light>();
            kl.type = LightType.Directional;
            kl.intensity = 1.15f;
            kl.color = new Color(1f, 0.94f, 0.85f);
            RenderSettings.ambientMode = UnityEngine.Rendering.AmbientMode.Flat;
            RenderSettings.ambientLight = new Color(0.36f, 0.37f, 0.44f);

            // ---- camera: wide cantina view like P2 -- pulled back + raised
            // (FOV 55) so the bar counter sits on the upper third and the
            // front holo tables + floor fill the lower half (was 13.5/5.6/42:
            // too tight -- the bar filled the frame and the hotspot labels
            // bunched at screen center)
            var cam = MakeCamera("MainCam",
                bar.center + bar.front * 16.5f + Vector3.up * 6.8f,
                55f, new Color(0.02f, 0.03f, 0.05f));
            cam.transform.LookAt(bar.center + bar.front * 3.0f + Vector3.up * 0.9f);

            // ---- home shell host ----
            var root = new GameObject("HomeRoot");
            var ui = root.AddComponent<SwgohRestored.UI.HomeUI>();
            var anchors = new Vector3[HotspotFrame.Length];
            Vector3 side = Vector3.Cross(Vector3.up, bar.front);   // bar-left +
            for (int i = 0; i < HotspotFrame.Length; i++)
            {
                var f = HotspotFrame[i];
                anchors[i] = bar.center + bar.front * f.x + side * f.y + Vector3.up * f.z;
            }
            ui.HotspotAnchors = anchors;

            Directory.CreateDirectory("Assets/Scenes");
            EditorSceneManager.SaveScene(scene, "Assets/Scenes/HomeScene.unity");

            var list = new List<EditorBuildSettingsScene>(EditorBuildSettings.scenes);
            if (!list.Exists(s => s.path == "Assets/Scenes/HomeScene.unity"))
            {
                list.Add(new EditorBuildSettingsScene("Assets/Scenes/HomeScene.unity", true));
                EditorBuildSettings.scenes = list.ToArray();
            }
            Debug.Log($"[UiAssetImporter] HomeScene built (bar {bar.center} front {bar.front})");
        }

        // bar counter center + the horizontal direction pointing into the room
        static (Vector3 center, Vector3 front) ComputeCantinaFrame(GameObject env)
        {
            Bounds bar = new Bounds(), back = new Bounds();
            bool hb = false, hk = false;
            foreach (var r in env.GetComponentsInChildren<Renderer>(true))
            {
                string n = r.name.ToLowerInvariant();
                if (n.Contains("home_dump_barback")) { back = hb ? back : r.bounds; hk = true; }
                else if (n.Contains("home_dump_bar_mesh")) { if (!hb) { bar = r.bounds; hb = true; } }
            }
            Vector3 center = hb ? bar.center : new Vector3(0f, 1f, 0f);
            Vector3 front = Vector3.forward;
            if (hb && hk)
            {
                var d = center - back.center; d.y = 0f;
                if (d.sqrMagnitude > 0.01f) front = d.normalized;
            }
            return (center, front);
        }


        static void EnsureEnvBackdrop()
        {
            Directory.CreateDirectory("Assets/EnvBackdrop");
            if (AssetDatabase.LoadAssetAtPath<GameObject>(EnvBackdropAsset) == null)
            {
                File.Copy(EnvSourceGlb, EnvBackdropAsset, true);
                AssetDatabase.ImportAsset(EnvBackdropAsset, ImportAssetOptions.ForceUpdate);
            }
        }

        const string HomeMarker = @"F:\PPfavorite\Work\goh\unity\ui_capture_home.marker";

        [MenuItem("SWGoH/UI/7 Capture HomeScene (batch entry)")]
        public static void CaptureHomeScene() => BatchCaptureHomeScene();

        public static void BatchCaptureHomeScene()
        {
            ImportHomeSprites();
            BuildHomeScene();
            File.Delete(@"F:\PPfavorite\Work\goh\unity\ui_home.png");
            File.WriteAllText(HomeMarker, "home-capture");
            EditorSceneManager.OpenScene("Assets/Scenes/HomeScene.unity");
            EditorApplication.isPlaying = true;
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
