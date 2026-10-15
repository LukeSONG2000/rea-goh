// RosterUI -- P3 inventory screen (acceptance: shots/reference/P3.png).
// Left vertical tab bar, filter row, scrollable card grid (portrait ring,
// star row, level diamond, name, alignment chip), top bar with galactic
// power total.  Original widget sprites from Assets/Resources/UiSprites.
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

namespace SwgohRestored.UI
{
    public class RosterUI : MonoBehaviour
    {
        public string DefaultDetailUnit = "SUPREMELEADERKYLOREN";

        Canvas _canvas;
        readonly Dictionary<string, Text> _tabTexts = new();
        readonly Dictionary<string, Image> _tabBgs = new();
        RectTransform _gridContent;
        readonly List<GameObject> _cards = new();
        Text _powerText;
        List<UnitEntry> _units;

        // Screen labels are the official loc_cn values; keys documented in
        // StreamingAssets/ui_labels.json.  时代单位/光速代币 are older-version
        // strings not present in the current loc dump (REFERENCE-MEASURED).
        static readonly (string key, string label, bool enabled)[] Tabs =
        {
            ("tab_era_units", "时代单位", false),
            ("tab_characters", "角色", true),
            ("tab_all", "全部", true),
            ("tab_favorites", "收藏", false),
            ("tab_lightspeed", "光速代币", false),
            ("tab_squads", "小队", false),
            ("tab_ships", "飞船", false),
            ("tab_fleets", "舰队", false),
            ("tab_datacrons", "数据全像记录器", false),
        };

        bool _showAll;   // 角色 tab filters combat_type==1, 全部 shows everything

        void Start()
        {
            _units = UiData.LoadUnits();
            Build();
            ApplyFilter();
        }

        void Build()
        {
            _canvas = UiKit.EnsureCanvas(gameObject);
            var driver = gameObject.AddComponent<UiCaptureDriver>();
            driver.SceneId = "roster";
            driver.CaptureFrame = 45;
            driver.OutputPath = @"F:\PPfavorite\Work\goh\unity\ui_roster.png";

            // ---- top bar ----
            var topBar = UiKit.NewRect(_canvas.transform, "TopBar",
                new Vector2(0, 1), new Vector2(1, 1), new Vector2(0, -90), new Vector2(0, 0));
            var backBtn = UiKit.NewImage(topBar, "Back", UiKit.LoadWidget("button_back"), Color.white);
            SetAbs(backBtn.rectTransform, new Vector2(0, 1), new Vector2(62, -47), new Vector2(66, 70));
            BindNav(backBtn, null);

            UiKit.NewText(topBar, "Title", "物品栏", 44, new Color(0.92f, 0.95f, 1f),
                TextAnchor.MiddleLeft, new Vector2(0, 1), new Vector2(1, 1),
                new Vector2(110, -60), new Vector2(-700, -14), FontStyle.Bold);
            UiKit.NewText(topBar, "Subtitle", "点击头像进行研究和升级", 20, new Color(0.62f, 0.72f, 0.85f),
                TextAnchor.MiddleLeft, new Vector2(0, 1), new Vector2(1, 1),
                new Vector2(112, -90), new Vector2(-700, -58));

            long totalPower = 0;
            foreach (var u in _units) totalPower += u.power;
            _powerText = UiKit.NewText(topBar, "GalacticPower",
                "银河力量：" + UiKit.FormatNumber(totalPower), 36, new Color(1f, 0.85f, 0.35f),
                TextAnchor.MiddleRight, new Vector2(1, 1), new Vector2(1, 1),
                new Vector2(-420, -66), new Vector2(-120, -2));
            var homeBtn = UiKit.NewImage(topBar, "Home", UiKit.LoadWidget("button_home"), Color.white);
            SetAbs(homeBtn.rectTransform, new Vector2(1, 1), new Vector2(-64, -47), new Vector2(64, 64));
            BindNav(homeBtn, null);

            // ---- filter row (visual, matches P3) ----
            var filterRow = UiKit.NewRect(_canvas.transform, "FilterRow",
                new Vector2(0, 1), new Vector2(1, 1), new Vector2(250, -160), new Vector2(-20, -96));
            var drop1 = UiKit.NewImage(filterRow, "Drop1", UiKit.LoadWidget("bg_tab_idle"), Color.white);
            SetAbs(drop1.rectTransform, new Vector2(0, 0.5f), new Vector2(115, 0), new Vector2(230, 56));
            UiKit.NewText(drop1.rectTransform, "t", "全部", 24, new Color(0.85f, 0.92f, 1f), TextAnchor.MiddleLeft,
                new Vector2(0, 0), new Vector2(1, 1), new Vector2(18, 0), new Vector2(-50, 0));
            var dArrow = UiKit.NewImage(drop1.rectTransform, "arrow", UiKit.LoadWidget("pip_blue"), Color.white);
            SetAbs(dArrow.rectTransform, new Vector2(1, 0.5f), new Vector2(-26, 0), new Vector2(16, 22));
            var drop2 = UnityEngine.Object.Instantiate(drop1, filterRow);
            drop2.name = "Drop2";
            drop2.rectTransform.anchoredPosition = new Vector2(365, 0);

            var chk = UiKit.NewImage(filterRow, "ModsCheck", UiKit.LoadWidget("GearCheckbox"), Color.white);
            SetAbs(chk.rectTransform, new Vector2(0, 0.5f), new Vector2(530, 0), new Vector2(34, 34));
            UiKit.NewText(filterRow, "ModsLabel", "改装件", 24, new Color(0.85f, 0.92f, 1f), TextAnchor.MiddleLeft,
                new Vector2(0, 0.5f), new Vector2(0, 0.5f), new Vector2(560, -14), new Vector2(760, 14));
            var filterBtn = UiKit.NewImage(filterRow, "FilterBtn", UiKit.LoadWidget("filter_button_small"), Color.white);
            SetAbs(filterBtn.rectTransform, new Vector2(1, 0.5f), new Vector2(-42, 0), new Vector2(56, 56));

            // ---- left tab bar ----
            var tabBar = UiKit.NewRect(_canvas.transform, "TabBar",
                new Vector2(0, 0), new Vector2(0, 1), new Vector2(8, 40), new Vector2(250, -170));
            var navPanel = UiKit.NewImage(tabBar, "NavPanel", UiKit.LoadWidget("RoundedRect_4pt"),
                new Color(0.07f, 0.11f, 0.17f, 0.92f));
            navPanel.type = Image.Type.Sliced;
            Stretch(navPanel.rectTransform, Vector2.zero, Vector2.one);
            navPanel.rectTransform.SetAsFirstSibling();

            float rowH = (1080f - 170f - 60f) / Tabs.Length;
            for (int i = 0; i < Tabs.Length; i++)
            {
                var (key, label, enabled) = Tabs[i];
                var row = UiKit.NewRect(tabBar, "Tab_" + key,
                    new Vector2(0, 1), new Vector2(1, 1),
                    new Vector2(6, -rowH * (i + 1) + 3), new Vector2(-6, -rowH * i - 3));
                var bg = UiKit.NewImage(row, "bg", UiKit.LoadWidget("side_nav_btn_unselected"),
                    enabled ? Color.white : new Color(0.55f, 0.55f, 0.55f, 0.35f));
                bg.type = Image.Type.Sliced;
                Stretch(bg.rectTransform, Vector2.zero, Vector2.one);
                var txt = UiKit.NewText(row, "t", label, 26,
                    enabled ? new Color(0.88f, 0.94f, 1f) : new Color(0.55f, 0.60f, 0.66f),
                    TextAnchor.MiddleLeft, new Vector2(0, 0), new Vector2(1, 1),
                    new Vector2(24, 0), new Vector2(-34, 0));
                _tabTexts[key] = txt;
                _tabBgs[key] = bg;
                if (enabled)
                {
                    var btn = row.gameObject.AddComponent<Button>();
                    btn.targetGraphic = bg;
                    string k = key;
                    btn.onClick.AddListener(() => OnTab(k));
                }
            }

            // ---- scroll grid ----
            var scrollRoot = UiKit.NewRect(_canvas.transform, "GridScroll",
                new Vector2(0, 0), new Vector2(1, 1), new Vector2(268, 40), new Vector2(-20, -170));
            var viewport = UiKit.NewRect(scrollRoot, "Viewport",
                Vector2.zero, Vector2.one, Vector2.zero, Vector2.zero);
            viewport.gameObject.AddComponent<RectMask2D>();
            var content = UiKit.NewRect(viewport, "Content",
                new Vector2(0, 1), new Vector2(1, 1), new Vector2(0, 0), new Vector2(0, 0));
            var grid = content.gameObject.AddComponent<GridLayoutGroup>();
            grid.cellSize = new Vector2(118, 168);
            grid.spacing = new Vector2(10, 14);
            grid.padding = new RectOffset(2, 2, 4, 4);
            grid.constraint = GridLayoutGroup.Constraint.FixedColumnCount;
            grid.constraintCount = 11;
            grid.childAlignment = TextAnchor.UpperLeft;
            content.gameObject.AddComponent<ContentSizeFitter>().verticalFit =
                ContentSizeFitter.FitMode.PreferredSize;
            var scroll = scrollRoot.gameObject.AddComponent<ScrollRect>();
            scroll.viewport = viewport;
            scroll.content = content;
            scroll.horizontal = false;
            scroll.scrollSensitivity = 24;
            _gridContent = content;
        }

        static void Stretch(RectTransform rt, Vector2 min, Vector2 max)
        {
            rt.anchorMin = Vector2.zero;
            rt.anchorMax = Vector2.one;
            rt.offsetMin = min;
            rt.offsetMax = max;
        }

        void OnTab(string key)
        {
            _showAll = key == "tab_all";
            ApplyFilter();
        }

        void ApplyFilter()
        {
            foreach (var (key, _, enabled) in Tabs)
            {
                if (!enabled) continue;
                bool selected = (_showAll && key == "tab_all") || (!_showAll && key == "tab_characters");
                _tabBgs[key].sprite = UiKit.LoadWidget(selected ? "side_nav_btn_selected" : "side_nav_btn_unselected");
                _tabTexts[key].color = selected
                    ? new Color(1f, 0.95f, 0.75f)
                    : new Color(0.88f, 0.94f, 1f);
                _tabTexts[key].fontStyle = selected ? FontStyle.Bold : FontStyle.Normal;
            }
            foreach (var card in _cards) Destroy(card);
            _cards.Clear();
            foreach (var u in _units)
            {
                if (!_showAll && u.combat_type != 1) continue;
                _cards.Add(BuildCard(u));
            }
            LayoutRebuilder.ForceRebuildLayoutImmediate(_gridContent);
        }

        GameObject BuildCard(UnitEntry u)
        {
            var card = new GameObject("card_" + u.def_id, typeof(RectTransform));
            card.transform.SetParent(_gridContent, false);

            // card backing -- dark blue panel
            var bg = UiKit.NewImage(card.transform, "bg", UiKit.LoadWidget("squareframe_grayscale"),
                new Color(0.10f, 0.15f, 0.22f, 0.95f));
            bg.type = Image.Type.Sliced;
            Stretch(bg.rectTransform, Vector2.zero, Vector2.one);
            bg.rectTransform.SetAsFirstSibling();

            // ---- circular portrait with faction ring ----
            var ring = UiKit.NewImage(card.transform, "ring", UiKit.LoadWidget("BIT_Portrait_Frame"),
                UiKit.AlignColor(u.alignment));
            SetAbs(ring.rectTransform, new Vector2(0.5f, 1f), new Vector2(0, -64), new Vector2(92, 92));
            var glow = UiKit.NewImage(card.transform, "glow", UiKit.LoadWidget("glow_ring"),
                Alpha(UiKit.AlignColor(u.alignment), 0.45f));
            SetAbs(glow.rectTransform, new Vector2(0.5f, 1f), new Vector2(0, -64), new Vector2(104, 104));
            glow.rectTransform.SetAsFirstSibling();

            var portrait = UiKit.LoadCircularPortrait(u.def_id, 256);
            var portImg = UiKit.NewImage(card.transform, "portrait", portrait,
                portrait ? Color.white : new Color(0.25f, 0.3f, 0.38f, 1f));
            portImg.preserveAspect = true;
            SetAbs(portImg.rectTransform, new Vector2(0.5f, 1f), new Vector2(0, -62), new Vector2(80, 80));

            // level diamond (player_level sprite + number)
            var lvl = UiKit.NewImage(card.transform, "level", UiKit.LoadWidget("player_level"), Color.white);
            SetAbs(lvl.rectTransform, new Vector2(0.5f, 1f), new Vector2(-38, -104), new Vector2(30, 30));
            UiKit.NewText(lvl.transform, "n", u.level.ToString(), 15, Color.white, TextAnchor.MiddleCenter,
                FontStyle.Bold);

            // ---- star row ----
            var starRow = UiKit.NewRect(card.transform, "stars",
                new Vector2(0.5f, 1f), new Vector2(0.5f, 1f),
                new Vector2(-55, -126), new Vector2(55, -112));
            var gold = UiKit.LoadWidget("rarity_star");
            var gray = UiKit.LoadWidget("rarity_star_empty");
            for (int s = 0; s < 7; s++)
            {
                var star = UiKit.NewImage(starRow, "star" + s, s < u.stars ? gold : gray, Color.white);
                var rt = star.GetComponent<RectTransform>();
                rt.anchorMin = new Vector2((float)s / 7, 0);
                rt.anchorMax = new Vector2((float)(s + 1) / 7, 1);
                rt.offsetMin = Vector2.zero;
                rt.offsetMax = Vector2.zero;
            }

            // ---- name + alignment chip ----
            var nameTxt = UiKit.NewText(card.transform, "name", u.name, 14, new Color(0.95f, 0.97f, 1f),
                TextAnchor.UpperCenter, new Vector2(0, 0), new Vector2(1, 1),
                new Vector2(2, 20), new Vector2(-2, 42));
            nameTxt.horizontalOverflow = HorizontalWrapMode.Wrap;
            nameTxt.verticalOverflow = VerticalWrapMode.Overflow;
            string alignLabel = u.alignment == "dark" ? "黑暗" : u.alignment == "light" ? "光明" : "中立";
            UiKit.NewText(card.transform, "align", alignLabel, 12, Alpha(UiKit.AlignColor(u.alignment), 0.9f),
                TextAnchor.UpperCenter, new Vector2(0, 0), new Vector2(1, 1),
                new Vector2(2, 2), new Vector2(-2, 18));

            var btn = card.AddComponent<Button>();
            btn.targetGraphic = bg;
            string defId = u.def_id;
            btn.onClick.AddListener(() =>
            {
                DetailUI.SelectedDefId = defId;
                UnityEngine.SceneManagement.SceneManager.LoadScene("DetailScene");
            });
            return card;
        }

        static Color Alpha(Color c, float a)
        {
            c.a = a;
            return c;
        }

        static void SetAbs(RectTransform rt, Vector2 anchor, Vector2 pos, Vector2 size)
        {
            rt.anchorMin = anchor;
            rt.anchorMax = anchor;
            rt.anchoredPosition = pos;
            rt.sizeDelta = size;
        }

        static void BindNav(Image img, UnityEngine.Events.UnityAction onClick)
        {
            var btn = img.gameObject.AddComponent<Button>();
            btn.targetGraphic = img;
            if (onClick != null) btn.onClick.AddListener(onClick);
        }
    }
}
