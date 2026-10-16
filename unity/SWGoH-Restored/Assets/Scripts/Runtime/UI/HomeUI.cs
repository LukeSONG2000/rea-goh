// HomeUI -- P2 cantina HOME SHELL (acceptance: shots/reference/P2.png).
//
// Programmatic uGUI over the ORIGINAL cantina env (env_home_screen_cantina_01
// GLB staged by SWGoH/UI/6 as the scene backdrop):
//   1. top bar: gear+ESC, level 85, player name, 最高等级/银河力量 caption
//      (total from StreamingAssets/roster.json), four energy counters 144/144
//      (original bolt crops) + 紫色晶体 6,896 + home button
//   2. left rail: 收藏/旅行指南/盟友/邮箱 round buttons + captions + 3+ badges
//      (clickable stubs)
//   3. right rail: 活动已开启 countdown cards (original circle frames, stubs)
//   4. scene hotspots with ORIGINAL PC hotkeys over the 3D backdrop:
//      竞技场S -> RosterScene, 战役G -> BattleDemo, others toast 尚未复现;
//      S/D/F/G/R/E work from the keyboard too
//   5. bottom-left chat bubble + bottom-right green 开始 button (stubs)
//
// Chrome sprites are ORIGINAL atlas crops imported into
// Assets/Resources/UiSprites/widgets by SWGoH/UI/5 (see UiAssetImporter).
// The gold 4th-energy bolt variant was not in the harvest -- the closest
// original bolt crop (icon_ship_energy) stands in, listed PENDING-ORIGINAL.
using System;
using System.IO;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

namespace SwgohRestored.UI
{
    public class HomeUI : MonoBehaviour
    {
        // ---- scene hotspots (P2 labels + original PC key mapping) ----
        // nav: "roster" -> RosterScene (team select), "battle" -> BattleDemo,
        // "" -> stub toast.  badge: "3+"/"1" red notif, "!" gold, "ring" plain.
        public struct Hotspot
        {
            public string name, letter, sub, badge, nav;
            public KeyCode key;
            public Hotspot(string n, string l, string s, string b, string nav, KeyCode k)
            { name = n; letter = l; sub = s; badge = b; this.nav = nav; key = k; }
        }

        public static readonly Hotspot[] Hotspots =
        {
            new Hotspot("竞技场",   "S", "",                       "ring", "roster", KeyCode.S),
            new Hotspot("角斗场",   "D", "",                       "1",    "",       KeyCode.D),
            new Hotspot("达斯·加·加", "F", "时代旅程（阶数 0/4）",   "",     "",       KeyCode.F),
            new Hotspot("战役",     "G", "",                       "!",    "battle", KeyCode.G),
            new Hotspot("商店",     "R", "",                       "3",    "",       KeyCode.R),
            new Hotspot("船运货物", "E", "",                       "",     "",       KeyCode.E),
        };

        /// world anchor over the cantina floor per hotspot -- baked by
        /// SWGoH/UI/6 from the bar-anchored cantina frame
        public Vector3[] HotspotAnchors;

        Canvas _chrome;   // screen chrome (overlay; capture flips to SSC 0.2)
        Canvas _world;    // hotspot canvas (SSC plane 10, BattleHUD bar pattern)
        Text _toast;
        float _toastT;

        // batch capture: marker written by SWGoH/UI/7 before play mode
        const string MarkerPath = @"F:\PPfavorite\Work\goh\unity\ui_capture_home.marker";
        const string ShotPath = @"F:\PPfavorite\Work\goh\unity\ui_home.png";
        bool _capture;
        int _frame;
        bool _shot;

        void Start()
        {
            // one-shot: consume the marker so a shipped player never
            // self-captures + quits on a stale marker file
            _capture = File.Exists(MarkerPath);
            if (_capture) { try { File.Delete(MarkerPath); } catch { } }
            Build();
        }

        void Update()
        {
            if (_capture && !_shot)
            {
                _frame++;
                if (_frame >= 45) { _shot = true; CaptureShot(); }
            }
            foreach (var hs in Hotspots)
                if (Input.GetKeyDown(hs.key)) Navigate(hs.nav);
            if (_toast != null && _toastT > 0f)
            {
                _toastT -= Time.deltaTime;
                _toast.color = new Color(_toast.color.r, _toast.color.g, _toast.color.b,
                    Mathf.Clamp01(_toastT / 1.2f));
            }
        }

        // ------------------------------------------------------------------
        void Build()
        {
            var chromeGo = new GameObject("HomeChrome");
            chromeGo.transform.SetParent(transform, false);
            _chrome = UiKit.EnsureCanvas(chromeGo);
            if (FindObjectOfType<EventSystem>() == null)
            {
                var es = new GameObject("EventSystem", typeof(EventSystem), typeof(StandaloneInputModule));
                DontDestroyOnLoad(es);
            }

            BuildTopBar();
            BuildLeftRail();
            BuildRightRail();
            BuildBottom();
            _toast = NewLabel(_chrome.transform, "toast", "", 30,
                new Color(1f, 0.85f, 0.35f), TextAnchor.MiddleCenter, FontStyle.Bold,
                new Vector2(0.5f, 0.3f), new Vector2(0, 0), new Vector2(500, 44));
            _toast.color = new Color(1f, 0.85f, 0.35f, 0f);

            BuildHotspotCanvas();
        }

        // ---- label/rect helpers (point anchor + center pos + size) -------
        static Text NewLabel(Transform parent, string name, string content, int size,
            Color color, TextAnchor anchor, FontStyle style,
            Vector2 anchorPt, Vector2 pos, Vector2 sizeD)
        {
            var t = UiKit.NewText(parent, name, content, size, color, anchor, style);
            UiKit.SetAbs(t.rectTransform, anchorPt, pos, sizeD);
            return t;
        }

        static Image NewSprite(Transform parent, string name, string sprite,
            Color color, Vector2 anchor, Vector2 pos, Vector2 size)
        {
            var img = UiKit.NewImage(parent, name, UiKit.LoadWidget(sprite), color);
            UiKit.SetAbs(img.rectTransform, anchor, pos, size);
            return img;
        }

        // ---- 1. top bar -------------------------------------------------
        void BuildTopBar()
        {
            var bar = UiKit.NewRect(_chrome.transform, "TopBar",
                new Vector2(0, 1), new Vector2(1, 1), new Vector2(0, -110), new Vector2(0, 0));

            // gear / settings (ESC ring) -- anchor (0,1) = screen top-left
            var gear = NewSprite(bar, "Gear", "btn_circle_selected", Color.white,
                new Vector2(0, 1), new Vector2(52, -54), new Vector2(74, 74));
            var gearIcon = UiKit.NewImage(gear.transform, "ico",
                UiKit.LoadWidget("button_settings"), Color.white);
            Stretch(gearIcon.rectTransform);
            gearIcon.rectTransform.offsetMin = new Vector2(17, 17);
            gearIcon.rectTransform.offsetMax = new Vector2(-17, -17);
            // (0,1) inside gear = its top-left corner (15,-91) on screen
            NewLabel(gear.transform, "notif", "169", 17, Color.white,
                TextAnchor.UpperLeft, FontStyle.Bold,
                new Vector2(0, 1), new Vector2(15, -4), new Vector2(36, 16));
            var esc = NewSprite(gear.transform, "esc", "bg_key", Color.white,
                new Vector2(0, 0), new Vector2(-16, -2), new Vector2(46, 22));
            esc.type = Image.Type.Sliced;
            UiKit.NewText(esc.transform, "t", "ESC", 13, Color.white, TextAnchor.MiddleCenter);

            // level + name + power caption (roster.json stat k=25 total)
            NewLabel(bar, "Level", "85", 54, new Color(0.55f, 0.86f, 0.96f),
                TextAnchor.MiddleLeft, FontStyle.Bold,
                new Vector2(0, 1), new Vector2(170, -74), new Vector2(84, 56));
            NewLabel(bar, "Name", "LukeSkyfxxker", 44, new Color(0.95f, 0.97f, 1f),
                TextAnchor.MiddleLeft, FontStyle.Bold,
                new Vector2(0, 1), new Vector2(462, -70), new Vector2(500, 52));
            NewLabel(bar, "PowerCap", "最高等级 / 银河力量： " + PowerTotal(), 24,
                new Color(0.62f, 0.86f, 0.95f), TextAnchor.MiddleLeft, FontStyle.Normal,
                new Vector2(0, 1), new Vector2(566, -102), new Vector2(700, 30));

            // ---- four energy counters + purple crystal ----
            string[] bolts =
            {
                "icon_pathofconquest_energy",   // blue hex bolt
                "icon_mod_energy",              // green bolt
                "icon_cantina_energy",          // red bolt
                "icon_ship_energy",             // bolt -- gold variant PENDING-ORIGINAL
            };
            float x = 1012;
            for (int i = 0; i < bolts.Length; i++)
            {
                MakeCounter(bar, "counter" + i, bolts[i], "144/144", x);
                x += 142;
            }
            MakeCounter(bar, "crystal", "icon_premium", "6,896", x);

            var home = NewSprite(bar, "Home", "button_home", Color.white,
                new Vector2(1, 1), new Vector2(-46, -50), new Vector2(66, 66));
            BindStub(home.gameObject, "尚未复现");
        }

        void MakeCounter(RectTransform bar, string name, string icon, string value, float x)
        {
            var root = UiKit.NewRect(bar, name, new Vector2(0, 1), new Vector2(0, 1),
                Vector2.zero, Vector2.zero);
            UiKit.SetAbs(root, new Vector2(0, 1), new Vector2(x, -92), new Vector2(140, 46));
            var ic = UiKit.NewImage(root, "ico", UiKit.LoadWidget(icon), Color.white);
            ic.preserveAspect = true;
            UiKit.SetAbs(ic.rectTransform, new Vector2(0, 0.5f), new Vector2(23, 0), new Vector2(46, 46));
            var pill = UiKit.NewImage(root, "pill", UiKit.LoadWidget("counter_currency_small"), Color.white);
            pill.type = Image.Type.Sliced;
            UiKit.SetAbs(pill.rectTransform, new Vector2(0, 0.5f), new Vector2(94, 0), new Vector2(92, 42));
            UiKit.NewText(pill.transform, "v", value, 23, Color.white, TextAnchor.MiddleCenter,
                FontStyle.Bold);
            var plus = UiKit.NewImage(root, "plus", UiKit.LoadWidget("bg_green"), Color.white);
            plus.type = Image.Type.Sliced;
            UiKit.SetAbs(plus.rectTransform, new Vector2(0, 1), new Vector2(21, 4), new Vector2(24, 24));
            var p = UiKit.NewText(plus.transform, "p", "+", 19,
                new Color(0.10f, 0.38f, 0.14f), TextAnchor.MiddleCenter, FontStyle.Bold);
            Stretch(p.rectTransform);
        }

        // ---- 2. left rail -----------------------------------------------
        void BuildLeftRail()
        {
            // rail strip spans y -150..+150; children anchored (0,1) hang from
            // its top edge (150 below screen top)
            var rail = UiKit.NewRect(_chrome.transform, "LeftRail",
                new Vector2(0, 0), new Vector2(0, 1), new Vector2(0, 150), new Vector2(150, -150));
            var entries = new[]
            {
                ("收藏", "icon_collection", "3+"),
                ("旅行指南", "icon_journeyguide", ""),
                ("盟友", "icon_guild", ""),
                ("邮箱", "icon_inbox", "3+"),
            };
            for (int i = 0; i < entries.Length; i++)
            {
                var (label, icon, badge) = entries[i];
                float y = -128 - i * 150;
                var btn = NewSprite(rail, "btn_" + label, "btn_circle_selected", Color.white,
                    new Vector2(0, 1), new Vector2(66, y), new Vector2(88, 88));
                var ic = UiKit.NewImage(btn.transform, "ico", UiKit.LoadWidget(icon), Color.white);
                ic.preserveAspect = true;
                Stretch(ic.rectTransform);
                ic.rectTransform.offsetMin = new Vector2(21, 21);
                ic.rectTransform.offsetMax = new Vector2(-21, -21);
                BindStub(btn.gameObject, "尚未复现");

                // numbered key chip at the circle's bottom-left
                var chip = NewSprite(rail, "chip_" + label, "bg_key", Color.white,
                    new Vector2(0, 1), new Vector2(34, y + 56), new Vector2(30, 30));
                chip.type = Image.Type.Sliced;
                UiKit.NewText(chip.transform, "n", (i + 1).ToString(), 19, Color.white,
                    TextAnchor.MiddleCenter, FontStyle.Bold);

                if (!string.IsNullOrEmpty(badge))
                {
                    var b = NewSprite(rail, "badge_" + label, "Badge_Notif", Color.white,
                        new Vector2(0, 1), new Vector2(100, y - 34), new Vector2(44, 44));
                    UiKit.NewText(b.transform, "n", badge, 17, Color.white, TextAnchor.MiddleCenter,
                        FontStyle.Bold);
                }

                var cap = NewLabel(rail, "cap_" + label, label, 27, Color.white,
                    TextAnchor.UpperCenter, FontStyle.Bold,
                    new Vector2(0, 1), new Vector2(66, y - 60), new Vector2(120, 32));
                AddOutline(cap);
            }
        }

        // ---- 3. right rail (event countdown cards) -----------------------
        void BuildRightRail()
        {
            // mirrored strip on the right; (0,1) anchor = its top-left corner
            // (x = -162 from the right edge)
            var rail = UiKit.NewRect(_chrome.transform, "RightRail",
                new Vector2(1, 0), new Vector2(1, 1), new Vector2(-162, 150), new Vector2(-6, -150));
            var entries = new (string icon, string line1, string line2)[]
            {
                ("!",          "活动已开启", "2天 23小时"),
                ("portrait",   "活动已开启", "5天 1小时"),
                ("event_icon", "公会活动",   "18分 30秒"),
                ("icon_decks", "技能跃升II", "2天 22小时"),
            };
            for (int i = 0; i < entries.Length; i++)
            {
                var (icon, line1, line2) = entries[i];
                float y = -128 - i * 150;
                var btn = NewSprite(rail, "ev" + i, "btn_circle_selected", Color.white,
                    new Vector2(0, 1), new Vector2(66, y), new Vector2(88, 88));
                if (icon == "portrait")
                {
                    var port = UiKit.NewImage(btn.transform, "ico",
                        UiKit.LoadCircularPortrait("SUPREMELEADERKYLOREN", 96), Color.white);
                    port.preserveAspect = true;
                    Stretch(port.rectTransform);
                    port.rectTransform.offsetMin = new Vector2(8, 8);
                    port.rectTransform.offsetMax = new Vector2(-8, -8);
                }
                else if (icon == "!")
                {
                    UiKit.NewText(btn.transform, "ico", "!", 44, new Color(1f, 0.82f, 0.25f),
                        TextAnchor.MiddleCenter, FontStyle.Bold);
                }
                else
                {
                    var ic = UiKit.NewImage(btn.transform, "ico", UiKit.LoadWidget(icon), Color.white);
                    ic.preserveAspect = true;
                    Stretch(ic.rectTransform);
                    ic.rectTransform.offsetMin = new Vector2(20, 20);
                    ic.rectTransform.offsetMax = new Vector2(-20, -20);
                }
                BindStub(btn.gameObject, "尚未复现");

                var chip = NewSprite(rail, "chip" + i, "bg_key", Color.white,
                    new Vector2(0, 1), new Vector2(34, y + 56), new Vector2(30, 30));
                chip.type = Image.Type.Sliced;
                UiKit.NewText(chip.transform, "n", (i + 5).ToString(), 19, Color.white,
                    TextAnchor.MiddleCenter, FontStyle.Bold);

                var l1 = NewLabel(rail, "l1_" + i, line1, 25, Color.white,
                    TextAnchor.UpperCenter, FontStyle.Bold,
                    new Vector2(0, 1), new Vector2(66, y - 56), new Vector2(170, 30));
                AddOutline(l1);
                var l2 = NewLabel(rail, "l2_" + i, line2, 26, new Color(1f, 0.85f, 0.35f),
                    TextAnchor.UpperCenter, FontStyle.Bold,
                    new Vector2(0, 1), new Vector2(66, y - 88), new Vector2(170, 32));
                AddOutline(l2);
            }
        }

        // ---- 5. bottom stubs ---------------------------------------------
        void BuildBottom()
        {
            // chat bubble (bottom-left)
            var bubble = NewSprite(_chrome.transform, "ChatBubble", "bg_pillpanel",
                new Color(0.06f, 0.12f, 0.20f, 0.92f),
                new Vector2(0, 0), new Vector2(140, 76), new Vector2(256, 58));
            bubble.type = Image.Type.Sliced;
            UiKit.NewText(bubble.transform, "t", "世界频道：大家好！", 21,
                new Color(0.88f, 0.94f, 1f), TextAnchor.MiddleLeft,
                new Vector2(0, 0), new Vector2(1, 1), new Vector2(66, 0), new Vector2(-10, 0));
            var chatIc = UiKit.NewImage(bubble.transform, "ico",
                UiKit.LoadWidget("button_chat"), Color.white);
            UiKit.SetAbs(chatIc.rectTransform, new Vector2(0, 0.5f), new Vector2(32, 0), new Vector2(46, 46));
            BindStub(bubble.gameObject, "尚未复现");

            // green 开始 button (bottom-right)
            var start = NewSprite(_chrome.transform, "StartButton", "bg_green", Color.white,
                new Vector2(1, 0), new Vector2(-140, 60), new Vector2(250, 78));
            start.type = Image.Type.Sliced;
            var st = UiKit.NewText(start.transform, "t", "开始", 38, Color.white,
                TextAnchor.MiddleCenter, FontStyle.Bold);
            Stretch(st.rectTransform);
            BindStub(start.gameObject, "尚未复现");
        }

        // ---- 4. scene hotspots (world-anchored labels + key chips) -------
        // WORLD-SPACE canvas at identity with a small uniform scale: child
        // canvas-px coordinates map linearly to true world positions and the
        // canvas renders through any camera with no re-layout.  (The previous
        // ScreenSpaceCamera path re-scaled child offsets between the 640x480
        // batch game view and the 1600x900 capture RT, crushing all labels
        // toward screen center.)  Labels anchor to the real scene geometry
        // exactly like the original NGUI panel over the cantina set.
        const float CanvasScale = 0.0135f;   // 46px name ~35px on screen at bar depth (P2)
        const float RefDepth = 15.5f;        // camera->bar distance that anchors the size norm

        void BuildHotspotCanvas()
        {
            var worldGo = new GameObject("HotspotCanvas");
            worldGo.transform.SetParent(transform, false);
            _world = UiKit.EnsureCanvas(worldGo);
            _world.renderMode = RenderMode.WorldSpace;
            var wt = _world.transform;
            wt.position = Vector3.zero;
            // face the camera's yaw: identity presented its BACK to this camera
            // (mirrored text), and a fixed 180 keeps rows skewed when the view
            // is oblique -- billboarding on yaw keeps labels flat like P2
            var cam0 = Camera.main;
            float yaw = cam0 != null ? cam0.transform.eulerAngles.y : 180f;
            wt.rotation = Quaternion.Euler(0f, yaw, 0f);
            wt.localScale = new Vector3(CanvasScale, CanvasScale, CanvasScale);
            var cam = Camera.main;
            if (cam != null) _world.worldCamera = cam;   // click raycasting only
            if (HotspotAnchors == null || HotspotAnchors.Length < Hotspots.Length)
            {
                Debug.LogWarning("[HomeUI] hotspot anchors not baked -- skipping scene labels");
                return;
            }
            for (int i = 0; i < Hotspots.Length; i++)
            {
                var hs = Hotspots[i];
                var root = UiKit.NewRect(_world.transform, "hs_" + hs.name,
                    new Vector2(0.5f, 0.5f), new Vector2(0.5f, 0.5f),
                    Vector2.zero, Vector2.zero);
                // label+chip row ~430 canvas px wide; pivot at the right edge
                // so the row grows leftward from its world anchor (the chip
                // straddles the anchor point like the reference)
                var rrt = root.GetComponent<RectTransform>();
                rrt.sizeDelta = new Vector2(430, 64);
                rrt.pivot = new Vector2(1f, 0.5f);
                // depth-normalize label size: nearer labels shrink so every
                // hotspot renders at a similar screen size (P2 look)
                if (cam != null)
                {
                    float d = Vector3.Distance(cam.transform.position, HotspotAnchors[i]);
                    float s = Mathf.Clamp(d / RefDepth, 0.55f, 1.5f);
                    root.localScale = new Vector3(s, s, 1f);
                }

                // letter chip (original bg_key) right of the name
                var chip = UiKit.NewImage(root, "chip", UiKit.LoadWidget("bg_key"), Color.white);
                chip.type = Image.Type.Sliced;
                UiKit.SetAbs(chip.rectTransform, new Vector2(1, 0.5f), new Vector2(6, 0), new Vector2(48, 46));
                UiKit.NewText(chip.transform, "k", hs.letter, 30, Color.white,
                    TextAnchor.MiddleCenter, FontStyle.Bold);

                // hotspot name (right-aligned toward the chip, outlined)
                var nameT = UiKit.NewText(root, "name", hs.name, 46, new Color(0.94f, 0.97f, 1f),
                    TextAnchor.MiddleRight, FontStyle.Bold);
                var nrt = nameT.rectTransform;
                nrt.anchorMin = new Vector2(0, 0.5f);
                nrt.anchorMax = new Vector2(1, 0.5f);
                nrt.pivot = new Vector2(1, 0.5f);
                nrt.anchoredPosition = new Vector2(-54, 0);
                nrt.sizeDelta = new Vector2(-110, 54);
                AddOutline(nameT);

                // badge to the right of the chip
                if (!string.IsNullOrEmpty(hs.badge))
                {
                    bool gold = hs.badge == "!";
                    var b = UiKit.NewImage(root, "badge", UiKit.LoadWidget("Badge_Notif"),
                        gold ? new Color(1f, 0.82f, 0.25f) : Color.white);
                    UiKit.SetAbs(b.rectTransform, new Vector2(1, 0.5f), new Vector2(62, -2), new Vector2(34, 34));
                    if (gold)
                        UiKit.NewText(b.transform, "n", "!", 22, new Color(0.35f, 0.2f, 0.02f),
                            TextAnchor.MiddleCenter, FontStyle.Bold);
                    else if (hs.badge != "ring")
                        UiKit.NewText(b.transform, "n", hs.badge, 17, Color.white,
                            TextAnchor.MiddleCenter, FontStyle.Bold);
                }

                // subtitle line (e.g. 时代旅程（阶数 0/4）) hangs below the row:
                // pivot top-right so the rect grows DOWNWARD from the root bottom
                if (!string.IsNullOrEmpty(hs.sub))
                {
                    var sub = UiKit.NewText(root, "sub", hs.sub, 30, new Color(1f, 0.85f, 0.35f),
                        TextAnchor.UpperRight, FontStyle.Bold);
                    var srt = sub.rectTransform;
                    srt.anchorMin = new Vector2(0, 0);
                    srt.anchorMax = new Vector2(1, 0);
                    srt.pivot = new Vector2(1, 1);
                    srt.anchoredPosition = new Vector2(4, -4);
                    srt.sizeDelta = new Vector2(0, 36);
                    AddOutline(sub);
                }

                var btn = root.gameObject.AddComponent<Button>();
                string nav = hs.nav;
                btn.onClick.AddListener(() => Navigate(nav));

                var rt = root.GetComponent<RectTransform>();
                rt.position = HotspotAnchors[i];   // world canvas: honored exactly
            }
        }

        // ------------------------------------------------------------------
        static void Stretch(RectTransform rt)
        {
            rt.anchorMin = Vector2.zero;
            rt.anchorMax = Vector2.one;
            rt.offsetMin = Vector2.zero;
            rt.offsetMax = Vector2.zero;
        }

        static void AddOutline(Text t)
        {
            var ol = t.gameObject.AddComponent<Outline>();
            ol.effectColor = new Color(0f, 0.05f, 0.12f, 0.9f);
            ol.effectDistance = new Vector2(2, -2);
        }

        static void BindStub(GameObject go, string msg)
        {
            var btn = go.AddComponent<Button>();
            btn.targetGraphic = go.GetComponent<Image>();
            btn.onClick.AddListener(() => { Toast(msg); });
        }

        // toasts are shown on the chrome canvas from any click handler
        static HomeUI _instance;

        static void Toast(string msg)
        {
            if (_instance == null || _instance._toast == null) return;
            _instance._toast.text = msg;
            _instance._toastT = 1.2f;
        }

        void Navigate(string nav)
        {
            switch (nav)
            {
                case "roster":
                    SceneManager.LoadScene("RosterScene");
                    break;
                case "battle":
                    SceneManager.LoadScene("BattleDemo");
                    break;
                default:
                    Toast("尚未复现");
                    break;
            }
        }

        // ---- roster.json galactic power (stat key 25) ---------------------
        [Serializable] class RosterStat { public string k; public double v; }
        [Serializable] class RosterEntry { public RosterStat[] stats; }
        [Serializable] class RosterList { public RosterEntry[] items; }

        static string PowerTotal()
        {
            try
            {
                var json = File.ReadAllText(Path.Combine(Application.streamingAssetsPath, "roster.json"));
                var list = JsonUtility.FromJson<RosterList>("{\"items\":" + json + "}");
                double tot = 0;
                foreach (var e in list.items)
                    foreach (var s in e.stats)
                        if (s.k == "25") tot += s.v;
                return tot >= 1e6 ? (tot / 1e6).ToString("0.0") + "M" : UiKit.FormatNumber((long)tot);
            }
            catch (Exception ex)
            {
                Debug.LogWarning("[HomeUI] roster.json power total unavailable: " + ex.Message);
                return "--";
            }
        }

        // ---- batch capture (SWGoH/UI/7 entry) -----------------------------
        void CaptureShot()
        {
            var cam = Camera.main;
            if (cam == null) { Debug.LogError("[HomeCapture] no camera"); return; }
            // The canvas lays out against the batch game view (640x480), so the
            // camera's target texture must exist BEFORE the canvases bind --
            // then the SSC canvas re-layouts against the 1600x900 pixel rect
            // and renders at the 1920x1080 design proportions.
            var prev = cam.targetTexture;
            var rt = new RenderTexture(1600, 900, 24);
            cam.targetTexture = rt;
            cam.nearClipPlane = Mathf.Min(cam.nearClipPlane, 0.05f);
            UiKit.SetCamera(_chrome, cam);   // overlay chrome won't enter cam.Render()
            // the hotspot canvas is world-space -- it renders through cam.Render()
            // natively and must NOT be flipped to ScreenSpaceCamera
            Canvas.ForceUpdateCanvases();
            cam.Render();
            RenderTexture.active = rt;
            var tex = new Texture2D(1600, 900, TextureFormat.RGB24, false);
            tex.ReadPixels(new Rect(0, 0, 1600, 900), 0, 0);
            tex.Apply();
            cam.targetTexture = prev;
            RenderTexture.active = null;
            File.WriteAllBytes(ShotPath, tex.EncodeToPNG());
            Destroy(tex);
            Destroy(rt);
            _chrome.renderMode = RenderMode.ScreenSpaceOverlay;
            Debug.Log("[HomeCapture] home -> " + ShotPath);
#if UNITY_EDITOR
            UnityEditor.EditorApplication.isPlaying = false;
            if (Application.isBatchMode) UnityEditor.EditorApplication.Exit(0);
#else
            Application.Quit(0);
#endif
        }

        void OnEnable() { _instance = this; }
        void OnDisable() { if (_instance == this) _instance = null; }
    }
}
