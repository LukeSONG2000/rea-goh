// BattleHUD -- programmatic uGUI overlay for the battle: per-unit health
// bars (world-to-screen), a bottom action bar echoing the game's skill row,
// and floating damage numbers.  Built entirely from code (no prefab assets).
//
// P5 squad-battle HUD extensions (acceptance: shots/reference/P5.png):
//   1. top-center battle timer (4:49 countdown, red digits like the original)
//   2. top-left controls row: 自动 / 速度(2x) / 锁定 round buttons
//   3. per-unit key chips (player Q/W/E/R/T, enemies 1..n) + buff icon slots
//   4. bottom skill bar for the CURRENTLY ACTING unit (ui_units.json abilities,
//      ability icon textures + Q/W/E key captions, click or key to cast)
//   5. top-right enemy commander portrait (charui portrait + red ring + HP bar)
// All chrome sprites are ORIGINAL harvested atlas crops imported into
// Assets/Resources/UiSprites/widgets by SWGoH/UI/4 (see UiAssetImporter).
using System.Collections.Generic;
using SwgohRestored.UI;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

namespace SwgohRestored.Battle
{
    public class BattleHUD : MonoBehaviour
    {
        Canvas _canvas;          // world-anchored bars (plane 10)
        Canvas _screenCanvas;    // screen-anchored chrome (plane 0.2: the floor
                                 // at battle-camera distance is CLOSER than 10m
                                 // and depth-occludes bottom-anchored UI on the
                                 // bar canvas -- same reason UiKit uses 0.2)
        readonly Dictionary<string, Bar> _bars = new();
        Text _turnLabel;
        Font _font;

        /// Overlay canvases don't enter cam.Render() -- batch screenshots need
        /// the canvas in ScreenSpaceCamera mode bound to the rendering camera.
        public void SetCamera(Camera cam)
        {
            EnsureCanvas();
            if (cam != null)
            {
                // near clip must be tighter than the chrome canvas plane or
                // the whole HUDScreen canvas is frustum-clipped (BattleDemo's
                // camera ships with a wider near clip than the UI scenes)
                cam.nearClipPlane = Mathf.Min(cam.nearClipPlane, 0.05f);
                _canvas.worldCamera = cam;
                _canvas.renderMode = RenderMode.ScreenSpaceCamera;
                var plane = 10f;
                _canvas.planeDistance = plane;
                _screenCanvas.worldCamera = cam;
                _screenCanvas.renderMode = RenderMode.ScreenSpaceCamera;
                _screenCanvas.planeDistance = 0.2f;
            }
        }

        class Bar
        {
            public RectTransform Root;
            public Image Fill;
            public Text Label;
            public RectTransform Chip;      // key letter chip (right of bar)
            public Text ChipText;
            public RectTransform Buffs;     // buff icon slot row (above bar)
        }

        public void EnsureCanvas()
        {
            if (_canvas != null) return;
            var go = new GameObject("HUD");
            _canvas = go.AddComponent<Canvas>();
            _canvas.renderMode = RenderMode.ScreenSpaceOverlay;
            go.AddComponent<CanvasScaler>().uiScaleMode = CanvasScaler.ScaleMode.ScaleWithScreenSize;
            go.AddComponent<GraphicRaycaster>();
            var screenGo = new GameObject("HUDScreen");
            _screenCanvas = screenGo.AddComponent<Canvas>();
            _screenCanvas.renderMode = RenderMode.ScreenSpaceOverlay;
            // separate ROOT canvas (nested canvases inherit the root's plane
            // distance, which would defeat the near-plane trick)
            screenGo.AddComponent<CanvasScaler>().uiScaleMode = CanvasScaler.ScaleMode.ScaleWithScreenSize;
            screenGo.AddComponent<GraphicRaycaster>();
            if (FindObjectOfType<EventSystem>() == null)
            {
                var es = new GameObject("EventSystem", typeof(EventSystem), typeof(StandaloneInputModule));
                Object.DontDestroyOnLoad(es);
            }
        }

        Font BodyFont()
        {
            if (_font == null) _font = UiKit.FontCjk();
            return _font;
        }

        public void TrackUnit(BattleUnit u, Camera cam, string chipLetter = null,
                              string[] buffSprites = null, Color[] buffTints = null)
        {
            EnsureCanvas();
            var rootGo = new GameObject($"hp_{u.BaseId}", typeof(RectTransform));
            rootGo.transform.SetParent(_canvas.transform, false);
            var root = rootGo.GetComponent<RectTransform>();
            root.sizeDelta = new Vector2(90, 14);

            var bg = CreateImage(rootGo.transform, new Color(0, 0, 0, 0.6f));
            var bgrt = bg.GetComponent<RectTransform>();
            bgrt.anchorMin = Vector2.zero; bgrt.anchorMax = Vector2.one;

            // original bar art: unit_health_bg frame + tintable unit_health_fill
            var frame = bg.GetComponent<Image>();
            var healthBg = UiKit.LoadWidget("unit_health_bg");
            var healthFill = UiKit.LoadWidget("unit_health_fill");
            if (healthBg != null) { frame.sprite = healthBg; frame.color = Color.white; frame.type = Image.Type.Simple; }

            var fillGo = new GameObject("fill", typeof(RectTransform));
            fillGo.transform.SetParent(root.transform, false);
            var fill = fillGo.AddComponent<Image>();
            fill.sprite = healthFill;
            fill.color = u.Side == 0 ? new Color(0.30f, 0.80f, 0.35f) : new Color(0.88f, 0.32f, 0.30f);
            fill.type = Image.Type.Simple;
            var frt = fill.GetComponent<RectTransform>();
            frt.anchorMin = new Vector2(0, 0); frt.anchorMax = new Vector2(1, 1);
            frt.pivot = new Vector2(0, 0.5f);
            frt.anchoredPosition = Vector2.zero;
            frt.sizeDelta = Vector2.zero;

            var labelGo = new GameObject("label", typeof(RectTransform));
            labelGo.transform.SetParent(root.transform, false);
            var label = labelGo.AddComponent<Text>();
            label.font = Resources.GetBuiltinResource<Font>("LegacyRuntime.ttf");
            label.fontSize = 11; label.alignment = TextAnchor.MiddleCenter;
            label.color = Color.white;
            var lrt = label.GetComponent<RectTransform>();
            lrt.anchorMin = Vector2.zero; lrt.anchorMax = Vector2.one; lrt.sizeDelta = Vector2.zero;

            // ---- P5 key chip (letter marker right of the bar, like A-G in P5) ----
            RectTransform chip = null;
            Text chipText = null;
            if (!string.IsNullOrEmpty(chipLetter))
            {
                var chipGo = new GameObject("keychip", typeof(RectTransform));
                chipGo.transform.SetParent(root.transform, false);
                chip = chipGo.GetComponent<RectTransform>();
                chip.anchorMin = new Vector2(1, 0.5f); chip.anchorMax = new Vector2(1, 0.5f);
                chip.pivot = new Vector2(0, 0.5f);
                chip.anchoredPosition = new Vector2(3, 0);
                chip.sizeDelta = new Vector2(24, 24);
                var chipImg = chipGo.AddComponent<Image>();
                var keyBg = UiKit.LoadWidget("bg_key");
                if (keyBg != null) { chipImg.sprite = keyBg; chipImg.type = Image.Type.Sliced; }
                else chipImg.color = new Color(0.05f, 0.10f, 0.16f, 0.9f);   // PENDING-ORIGINAL fallback
                chipText = NewText(chipGo.transform, "letter", chipLetter, 16, Color.white,
                    TextAnchor.MiddleCenter, FontStyle.Bold);
                Stretch(chipText.rectTransform);
            }

            // ---- P5 buff icon slots above the bar (original buff icon crops) ----
            RectTransform buffs = null;
            if (buffSprites != null && buffSprites.Length > 0)
            {
                var buffGo = new GameObject("buffs", typeof(RectTransform));
                buffGo.transform.SetParent(root.transform, false);
                buffs = buffGo.GetComponent<RectTransform>();
                buffs.anchorMin = new Vector2(0, 1); buffs.anchorMax = new Vector2(0, 1);
                buffs.pivot = new Vector2(0, 0);
                buffs.anchoredPosition = new Vector2(2, 2);
                buffs.sizeDelta = new Vector2(70, 18);
                float bx = 0;
                for (int i = 0; i < buffSprites.Length; i++)
                {
                    var slot = new GameObject("buff" + i, typeof(RectTransform));
                    slot.transform.SetParent(buffs, false);
                    var srt = slot.GetComponent<RectTransform>();
                    srt.anchorMin = new Vector2(0, 0); srt.anchorMax = new Vector2(0, 0);
                    srt.pivot = new Vector2(0, 0);
                    srt.anchoredPosition = new Vector2(bx, 0);
                    srt.sizeDelta = new Vector2(16, 16);
                    var img = slot.AddComponent<Image>();
                    var sp = UiKit.LoadWidget(buffSprites[i]);
                    if (sp != null) { img.sprite = sp; img.color = buffTints != null && i < buffTints.Length ? buffTints[i] : Color.white; }
                    else img.color = new Color(0.1f, 0.55f, 0.25f, 0.9f);   // PENDING-ORIGINAL fallback
                    bx += 18;
                }
            }

            // ScreenSpaceCamera world size at planeDistance=10 makes 90 canvas
            // units render ~6x oversized -- 0.30 lands the bar near P5's ~9%
            // screen-width bars.
            root.localScale = new Vector3(0.30f, 0.30f, 1f);
            _bars[u.UnitId] = new Bar
            {
                Root = root.GetComponent<RectTransform>(),
                Fill = fill,
                Label = label,
                Chip = chip,
                ChipText = chipText,
                Buffs = buffs,
            };
            UpdateUnit(u, cam);
        }

        static Image CreateImage(Transform parent, Color color)
        {
            var go = new GameObject("bg", typeof(RectTransform));
            go.transform.SetParent(parent, false);
            var img = go.AddComponent<Image>();
            img.color = color;
            return img;
        }

        static Text NewText(Transform parent, string name, string content, int size, Color color,
                            TextAnchor anchor, FontStyle style = FontStyle.Normal)
        {
            var go = new GameObject(name, typeof(RectTransform));
            go.transform.SetParent(parent, false);
            var t = go.AddComponent<Text>();
            t.font = UiKit.FontCjk();
            t.fontSize = size;
            t.fontStyle = style;
            t.color = color;
            t.alignment = anchor;
            t.text = content;
            t.horizontalOverflow = HorizontalWrapMode.Overflow;
            t.verticalOverflow = VerticalWrapMode.Overflow;
            return t;
        }

        static void Stretch(RectTransform rt)
        {
            rt.anchorMin = Vector2.zero; rt.anchorMax = Vector2.one;
            rt.offsetMin = Vector2.zero; rt.offsetMax = Vector2.zero;
        }

        public void UpdateUnit(BattleUnit u, Camera cam, float fracOverride = -1f,
                               bool? aliveOverride = null, Vector3? anchorOverride = null)
        {
            if (!_bars.TryGetValue(u.UnitId, out var bar)) return;
            bool show = aliveOverride ?? u.Alive;
            bar.Root.gameObject.SetActive(show);
            if (!show) return;
            var rig = GameObject.Find(u.UnitId);   // SpawnRig names the instance by UnitId
            Vector3 anchor = anchorOverride
                ?? (rig ? rig.transform.position + Vector3.up * 2.2f : Vector3.zero);
            var rt = bar.Root;
            var sp = cam != null ? cam.WorldToScreenPoint(anchor) : Vector3.zero;
            if (_canvas != null && _canvas.worldCamera != null &&
                _canvas.renderMode == RenderMode.ScreenSpaceCamera)
                rt.position = anchor;      // SSC canvas: element lives in world space
            else
                rt.position = new Vector3(sp.x, sp.y, 0);   // overlay: screen px
            float frac = fracOverride >= 0
                ? Mathf.Clamp01(fracOverride)
                : (u.MaxHp > 0 ? Mathf.Clamp01((float)(u.CurrentHp / u.MaxHp)) : 0);
            bar.Fill.rectTransform.anchorMin = Vector2.zero;
            bar.Fill.rectTransform.anchorMax = new Vector2(frac, 1);
            bar.Fill.rectTransform.sizeDelta = Vector2.zero;
            if (bar.Label != null) bar.Label.text = $"{(int)(fracOverride >= 0 ? fracOverride * u.MaxHp : u.CurrentHp)}";
        }

        // ------------------------------------------------------------------
        // 1. battle timer (top-center, red digits "4:49" like the original)
        // ------------------------------------------------------------------
        Text _timer;

        public void BuildTimer()
        {
            EnsureCanvas();
            if (_timer != null) return;
            var go = new GameObject("timer", typeof(RectTransform));
            go.transform.SetParent(_screenCanvas.transform, false);
            _timer = go.AddComponent<Text>();
            _timer.font = Resources.GetBuiltinResource<Font>("LegacyRuntime.ttf");
            _timer.fontSize = 30; _timer.fontStyle = FontStyle.Bold;
            _timer.alignment = TextAnchor.UpperCenter;
            _timer.color = new Color(1f, 0.36f, 0.28f);
            _timer.text = "4:49";
            var rt = _timer.rectTransform;
            rt.anchorMin = new Vector2(0.5f, 1); rt.anchorMax = new Vector2(0.5f, 1);
            rt.pivot = new Vector2(0.5f, 1);
            rt.anchoredPosition = new Vector2(0, -10);
            rt.sizeDelta = new Vector2(140, 40);
        }

        public void UpdateTimer(float remainingSeconds)
        {
            if (_timer == null) BuildTimer();
            int total = Mathf.Max(0, Mathf.CeilToInt(remainingSeconds));
            _timer.text = $"{total / 60}:{total % 60:00}";
        }

        // ------------------------------------------------------------------
        // 2. top-left controls row: 自动 / 速度(2x) / 锁定
        // ------------------------------------------------------------------
        readonly Dictionary<string, Image> _ctl = new();
        readonly Dictionary<string, Text> _ctlCap = new();

        public void BuildControls(System.Action onAuto, System.Action onSpeed, System.Action onLock)
        {
            EnsureCanvas();
            if (_ctl.Count > 0) return;
            MakeControl("auto", UiKit.LoadWidget("auto_battle"), null, "自动", onAuto, 0);
            MakeControl("speed", UiKit.LoadWidget("btn_circle_selected"), "2x", "速度", onSpeed, 1);
            MakeControl("lock", UiKit.LoadWidget("hud_button"), "lock_filled", "锁定", onLock, 2);
        }

        void MakeControl(string id, Sprite bg, string overlayIcon, string caption,
                         System.Action onClick, int index)
        {
            var go = new GameObject("ctl_" + id, typeof(RectTransform));
            go.transform.SetParent(_screenCanvas.transform, false);
            var rt = go.GetComponent<RectTransform>();
            rt.anchorMin = new Vector2(0, 1); rt.anchorMax = new Vector2(0, 1);
            rt.pivot = new Vector2(0, 1);
            rt.anchoredPosition = new Vector2(12 + index * 54, -10);
            rt.sizeDelta = new Vector2(46, 46);
            var img = go.AddComponent<Image>();
            if (bg != null) img.sprite = bg;
            else img.color = new Color(0.10f, 0.22f, 0.35f, 0.9f);   // PENDING-ORIGINAL fallback
            if (overlayIcon != null)
            {
                if (overlayIcon == "2x")
                {
                    var t = NewText(go.transform, "2x", "2x", 17, Color.white, TextAnchor.MiddleCenter, FontStyle.Bold);
                    Stretch(t.rectTransform);
                }
                else
                {
                    var icGo = new GameObject("icon", typeof(RectTransform));
                    icGo.transform.SetParent(go.transform, false);
                    var ic = icGo.AddComponent<Image>();
                    ic.sprite = UiKit.LoadWidget(overlayIcon);
                    ic.color = Color.white;
                    Stretch(ic.rectTransform);
                    ic.rectTransform.offsetMin = new Vector2(9, 9);
                    ic.rectTransform.offsetMax = new Vector2(-9, -9);
                    _ctl[id + "_icon"] = ic;
                }
            }
            var btn = go.AddComponent<Button>();
            btn.onClick.AddListener(() => onClick?.Invoke());
            _ctl[id] = img;

            var cap = NewText(_screenCanvas.transform, "cap_" + id, caption, 12,
                new Color(0.92f, 0.96f, 1f), TextAnchor.UpperCenter);
            var crt = cap.rectTransform;
            crt.anchorMin = new Vector2(0, 1); crt.anchorMax = new Vector2(0, 1);
            crt.pivot = new Vector2(0.5f, 1);
            crt.anchoredPosition = new Vector2(12 + index * 54 + 23, -58);
            crt.sizeDelta = new Vector2(60, 18);
            _ctlCap[id] = cap;
        }

        /// Visual state of the control buttons (toggled = bright, off = dim).
        public void SetControlState(string id, bool on)
        {
            if (_ctl.TryGetValue(id, out var img)) img.color = on ? Color.white : new Color(0.55f, 0.60f, 0.68f, 0.9f);
            if (id == "lock" && _ctl.TryGetValue("lock_icon", out var ic))
                ic.color = on ? new Color(1f, 0.85f, 0.35f) : new Color(0.75f, 0.80f, 0.9f);
        }

        // ------------------------------------------------------------------
        // 4. bottom skill bar (currently acting unit, Q/W/E captions)
        // ------------------------------------------------------------------
        RectTransform _skillBar;
        Text _skillTitle;
        readonly List<Button> _skillButtons = new();

        public void ShowSkillBar(UnitEntry entry, string actorLabel, System.Action<int> onPick, bool showKeys = true)
        {
            EnsureCanvas();
            HideSkillBar();
            var rootGo = new GameObject("skillbar", typeof(RectTransform));
            rootGo.transform.SetParent(_screenCanvas.transform, false);
            _skillBar = rootGo.GetComponent<RectTransform>();
            _skillBar.anchorMin = new Vector2(1, 0); _skillBar.anchorMax = new Vector2(1, 0);
            _skillBar.pivot = new Vector2(1, 0);
            _skillBar.anchoredPosition = new Vector2(-10, 12);
            _skillBar.sizeDelta = new Vector2(240, 104);

            _skillTitle = NewText(rootGo.transform, "title", actorLabel, 14,
                new Color(0.95f, 0.93f, 0.8f), TextAnchor.UpperRight, FontStyle.Bold);
            var trt = _skillTitle.rectTransform;
            trt.anchorMin = new Vector2(0, 1); trt.anchorMax = new Vector2(1, 1);
            trt.pivot = new Vector2(0.5f, 1);
            trt.anchoredPosition = new Vector2(0, -16);
            trt.sizeDelta = new Vector2(0, 20);

            _skillButtons.Clear();
            float x = 0;
            int key = 0;
            string[] keys = { "Q", "W", "E" };
            foreach (var sk in ActiveAbilities(entry))
            {
                var btnGo = new GameObject("skill_" + sk.id, typeof(RectTransform));
                btnGo.transform.SetParent(rootGo.transform, false);
                var brt = btnGo.GetComponent<RectTransform>();
                brt.anchorMin = new Vector2(0, 0); brt.anchorMax = new Vector2(0, 0);
                brt.pivot = new Vector2(0, 0);
                brt.anchoredPosition = new Vector2(x, 0);
                brt.sizeDelta = new Vector2(72, 72);
                x += 80;

                var frame = btnGo.AddComponent<Image>();
                bool isBasic = sk.tag != null && sk.tag.Contains("基本");
                var frameSp = UiKit.LoadWidget(isBasic ? "battle_btn_attack_sliced" : "battle_btn_frame_gold");
                if (frameSp != null) { frame.sprite = frameSp; frame.type = Image.Type.Sliced; }
                else frame.color = new Color(0.45f, 0.36f, 0.12f, 0.95f);   // PENDING-ORIGINAL fallback

                var iconGo = new GameObject("icon", typeof(RectTransform));
                iconGo.transform.SetParent(btnGo.transform, false);
                var icon = iconGo.AddComponent<Image>();
                icon.sprite = UiKit.LoadIcon(sk.icon);
                icon.preserveAspect = true;
                Stretch(icon.rectTransform);
                icon.rectTransform.offsetMin = new Vector2(8, 8);
                icon.rectTransform.offsetMax = new Vector2(-8, -8);

                var keyGo = new GameObject("key", typeof(RectTransform));
                keyGo.transform.SetParent(btnGo.transform, false);
                var krt = keyGo.GetComponent<RectTransform>();
                krt.anchorMin = new Vector2(0.5f, 0); krt.anchorMax = new Vector2(0.5f, 0);
                krt.pivot = new Vector2(0.5f, 0);
                krt.anchoredPosition = new Vector2(0, showKeys ? -12 : 3);
                krt.sizeDelta = new Vector2(24, 17);
                if (showKeys)
                {
                    var keyBg = keyGo.AddComponent<Image>();
                    var kbSp = UiKit.LoadWidget("bg_key");
                    if (kbSp != null) { keyBg.sprite = kbSp; keyBg.type = Image.Type.Sliced; }
                    else keyBg.color = new Color(0.05f, 0.10f, 0.16f, 0.9f);   // PENDING-ORIGINAL fallback
                    var kt = NewText(keyGo.transform, "k", keys[key], 11, Color.white, TextAnchor.MiddleCenter, FontStyle.Bold);
                    Stretch(kt.rectTransform);
                }

                var idx = key;
                var button = btnGo.AddComponent<Button>();
                button.onClick.AddListener(() => onPick?.Invoke(idx));
                _skillButtons.Add(button);
                key++;
                if (key >= 3) break;
            }
        }

        /// basic + special abilities only (leader/unique passives aren't cast).
        public static List<SkillEntry> ActiveAbilities(UnitEntry entry)
        {
            var list = new List<SkillEntry>();
            if (entry?.skills == null) return list;
            foreach (var sk in entry.skills)
                if (sk.tag != null && (sk.tag.Contains("基本") || sk.tag.Contains("特殊")))
                    list.Add(sk);
            return list;
        }

        public void HighlightSkill(int index)
        {
            for (int i = 0; i < _skillButtons.Count; i++)
                if (_skillButtons[i] != null)
                    _skillButtons[i].image.color = i == index ? new Color(1f, 0.9f, 0.55f) : Color.white;
        }

        public void HideSkillBar()
        {
            if (_skillBar != null) Object.Destroy(_skillBar.gameObject);
            _skillBar = null;
            _skillButtons.Clear();
        }

        // ------------------------------------------------------------------
        // 5. enemy commander portrait (top-right: portrait + red ring + HP bar)
        // ------------------------------------------------------------------
        Image _cmdrFill;
        Text _cmdrName;

        public void BuildCommander(UnitEntry entry, string unitName)
        {
            EnsureCanvas();
            if (_cmdrName != null) return;
            var go = new GameObject("commander", typeof(RectTransform));
            go.transform.SetParent(_screenCanvas.transform, false);
            var rt = go.GetComponent<RectTransform>();
            rt.anchorMin = new Vector2(1, 1); rt.anchorMax = new Vector2(1, 1);
            rt.pivot = new Vector2(1, 1);
            rt.anchoredPosition = new Vector2(-8, -8);
            rt.sizeDelta = new Vector2(240, 62);

            // portrait + red opponent ring (turn_order_frame_opponent)
            var portGo = new GameObject("portrait", typeof(RectTransform));
            portGo.transform.SetParent(go.transform, false);
            var prt = portGo.GetComponent<RectTransform>();
            prt.anchorMin = Vector2.one; prt.anchorMax = Vector2.one;
            prt.pivot = Vector2.one;
            prt.anchoredPosition = new Vector2(-2, -2);
            prt.sizeDelta = new Vector2(58, 58);
            var port = portGo.AddComponent<Image>();
            var circ = UiKit.LoadCircularPortrait(entry != null ? entry.def_id : "", 96);
            if (circ != null) port.sprite = circ;
            else port.color = new Color(0.35f, 0.10f, 0.10f, 0.9f);   // PENDING-ORIGINAL fallback
            var ringGo = new GameObject("ring", typeof(RectTransform));
            ringGo.transform.SetParent(portGo.transform, false);
            var ring = ringGo.AddComponent<Image>();
            var ringSp = UiKit.LoadWidget("turn_order_frame_opponent");
            if (ringSp != null) ring.sprite = ringSp;
            else ring.color = new Color(0.85f, 0.15f, 0.12f, 0.9f);   // PENDING-ORIGINAL fallback
            Stretch(ring.rectTransform);
            ring.rectTransform.offsetMin = new Vector2(-4, -4);
            ring.rectTransform.offsetMax = new Vector2(4, 4);

            // name + green HP bar left of the portrait
            _cmdrName = NewText(go.transform, "name", unitName, 15,
                new Color(0.93f, 0.97f, 0.85f), TextAnchor.UpperRight, FontStyle.Bold);
            var nrt = _cmdrName.rectTransform;
            nrt.anchorMin = new Vector2(0, 1); nrt.anchorMax = new Vector2(1, 1);
            nrt.pivot = new Vector2(1, 1);
            nrt.anchoredPosition = new Vector2(-66, 0);
            nrt.sizeDelta = new Vector2(-10, 22);

            var barGo = new GameObject("hpbar", typeof(RectTransform));
            barGo.transform.SetParent(go.transform, false);
            var brt = barGo.GetComponent<RectTransform>();
            brt.anchorMin = new Vector2(0, 1); brt.anchorMax = new Vector2(1, 1);
            brt.pivot = new Vector2(1, 1);
            brt.anchoredPosition = new Vector2(-66, -24);
            brt.sizeDelta = new Vector2(-16, 13);
            var barBg = barGo.AddComponent<Image>();
            var bgSp = UiKit.LoadWidget("progressbar_clipped_bg");
            if (bgSp != null) { barBg.sprite = bgSp; barBg.color = Color.white; }
            else barBg.color = new Color(0, 0, 0, 0.65f);   // PENDING-ORIGINAL fallback

            var fillGo = new GameObject("fill", typeof(RectTransform));
            fillGo.transform.SetParent(barGo.transform, false);
            _cmdrFill = fillGo.AddComponent<Image>();
            var fillSp = UiKit.LoadWidget("unit_health_fill");
            if (fillSp != null) _cmdrFill.sprite = fillSp;
            _cmdrFill.color = new Color(0.35f, 0.85f, 0.38f);
            var frt = _cmdrFill.rectTransform;
            frt.anchorMin = Vector2.zero; frt.anchorMax = Vector2.one;
            frt.offsetMin = new Vector2(2, 2); frt.offsetMax = new Vector2(-2, -2);
        }

        public void UpdateCommander(float frac)
        {
            if (_cmdrFill == null) return;
            frac = Mathf.Clamp01(frac);
            _cmdrFill.rectTransform.anchorMin = Vector2.zero;
            _cmdrFill.rectTransform.anchorMax = new Vector2(frac, 1);
        }

        public void SetTurnText(string s)
        {
            EnsureCanvas();
            if (_turnLabel == null)
            {
                var go = new GameObject("turn", typeof(RectTransform));
                go.transform.SetParent(_screenCanvas.transform, false);
                _turnLabel = go.AddComponent<Text>();
                _turnLabel.font = UiKit.FontCjk();
                _turnLabel.fontSize = 18; _turnLabel.alignment = TextAnchor.UpperLeft;
                _turnLabel.color = new Color(0.95f, 0.79f, 0.29f);
                var rt = _turnLabel.GetComponent<RectTransform>();
                rt.anchorMin = new Vector2(0, 1); rt.anchorMax = new Vector2(0, 1);
                rt.pivot = new Vector2(0, 1);
                rt.anchoredPosition = new Vector2(16, -80);
                rt.sizeDelta = new Vector2(600, 30);
            }
            _turnLabel.text = s;
        }

        public void ShowDamage(string unitId, double amount, bool crit, Camera cam)
        {
            if (!_bars.TryGetValue(unitId, out var bar)) return;
            var go = new GameObject("dmg", typeof(RectTransform));
            go.transform.SetParent(_canvas.transform, false);
            var t = go.AddComponent<Text>();
            t.font = Resources.GetBuiltinResource<Font>("LegacyRuntime.ttf");
            t.fontSize = crit ? 26 : 19;
            t.fontStyle = crit ? FontStyle.Bold : FontStyle.Normal;
            t.color = crit ? new Color(1f, 0.62f, 0.20f) : Color.white;
            t.alignment = TextAnchor.MiddleCenter;
            t.text = crit ? $"{(int)amount}!" : $"{(int)amount}";
            var rt = go.GetComponent<RectTransform>();
            rt.position = bar.Root.position + new Vector3(Random.Range(-4f, 4f), 3f, 0);
            rt.sizeDelta = new Vector2(120, 30);
            rt.localScale = new Vector3(0.30f, 0.30f, 1f);
            go.AddComponent<Floater>();
        }
    }

    /// Rises and fades, then self-destructs.
    public class Floater : MonoBehaviour
    {
        float _t;
        Text _text;
        void Start() { _text = GetComponent<Text>(); }
        void Update()
        {
            _t += Time.deltaTime;
            transform.Translate(0, 34f * Time.deltaTime, 0);
            if (_text) _text.color = new Color(_text.color.r, _text.color.g, _text.color.b, 1f - _t / 0.9f);
            if (_t >= 0.9f) Destroy(gameObject);
        }
    }
}
