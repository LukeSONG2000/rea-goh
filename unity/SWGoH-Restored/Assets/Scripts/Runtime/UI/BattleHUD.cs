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
    public class BattleHUD
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
                var plane = 0.2f;
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
            public RectTransform Buffs;     // actual persistent effects above the bar
            public Text Protection;
            public string StatusSignature;
        }

        public void EnsureCanvas()
        {
            if (_canvas != null) return;
            var go = new GameObject("HUD");
            _canvas = go.AddComponent<Canvas>();
            _canvas.renderMode = RenderMode.ScreenSpaceOverlay;
            var barScaler = go.AddComponent<CanvasScaler>();
            barScaler.uiScaleMode = CanvasScaler.ScaleMode.ScaleWithScreenSize;
            barScaler.referenceResolution = new Vector2(1920, 1080);
            barScaler.matchWidthOrHeight = 0.5f;
            go.AddComponent<GraphicRaycaster>();
            var screenGo = new GameObject("HUDScreen");
            _screenCanvas = screenGo.AddComponent<Canvas>();
            _screenCanvas.renderMode = RenderMode.ScreenSpaceOverlay;
            // separate ROOT canvas (nested canvases inherit the root's plane
            // distance, which would defeat the near-plane trick)
            var screenScaler = screenGo.AddComponent<CanvasScaler>();
            screenScaler.uiScaleMode = CanvasScaler.ScaleMode.ScaleWithScreenSize;
            // same reference space as every other screen (UiKit) -- without it
            // the scaler defaults to 800x600 and inflates all layout numbers
            screenScaler.referenceResolution = new Vector2(1920, 1080);
            screenScaler.matchWidthOrHeight = 0.5f;
            screenGo.AddComponent<GraphicRaycaster>();
            if (Object.FindObjectOfType<EventSystem>() == null)
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
                              string[] buffSprites = null, Color[] buffTints = null, System.Action onPick = null)
        {
            EnsureCanvas();
            var rootGo = new GameObject($"hp_{u.BaseId}", typeof(RectTransform));
            rootGo.transform.SetParent(_canvas.transform, false);
            var root = rootGo.GetComponent<RectTransform>();
            root.sizeDelta = new Vector2(180, 22);

            var bg = CreateImage(rootGo.transform, new Color(0, 0, 0, 0.6f));
            var bgrt = bg.GetComponent<RectTransform>();
            bgrt.anchorMin = Vector2.zero; bgrt.anchorMax = Vector2.one;

            // original bar art: unit_health_bg frame + tintable unit_health_fill
            var frame = bg.GetComponent<Image>();
            var healthBg = UiKit.LoadWidget("unit_health_bg");
            var healthFill = UiKit.LoadWidget("unit_health_fill");
            if (healthBg != null) { frame.sprite = healthBg; frame.color = Color.white; frame.type = Image.Type.Simple; }
            var targetButton = rootGo.AddComponent<Button>();
            targetButton.targetGraphic = frame;
            targetButton.onClick.AddListener(() => onPick?.Invoke());

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
            label.fontSize = 18; label.alignment = TextAnchor.MiddleCenter;
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

            // Slots are populated only from actual persistent-effect definitions.
            // Legacy buffSprites arguments remain API-compatible, but cannot create
            // fabricated status badges unrelated to the simulator state.
            var buffGo = new GameObject("persistent-effects", typeof(RectTransform));
            buffGo.transform.SetParent(root.transform, false);
            var buffs = buffGo.GetComponent<RectTransform>();
            buffs.anchorMin = buffs.anchorMax = new Vector2(0, 1);
            buffs.pivot = Vector2.zero;
            buffs.anchoredPosition = new Vector2(0, 4);
            buffs.sizeDelta = new Vector2(180, 26);
            var protection = NewText(root, "protection", "", 14,
                new Color(0.7f, 0.86f, 1f), TextAnchor.MiddleCenter);
            protection.raycastTarget = false;
            protection.rectTransform.anchorMin = protection.rectTransform.anchorMax = new Vector2(0.5f, 0);
            protection.rectTransform.pivot = new Vector2(0.5f, 1);
            protection.rectTransform.anchoredPosition = new Vector2(0, -1);
            protection.rectTransform.sizeDelta = new Vector2(180, 18);

            // ScreenSpaceCamera world size at planeDistance=10 makes 90 canvas
            // units render ~6x oversized -- 0.30 lands the bar near P5's ~9%
            // screen-width bars.
            root.localScale = Vector3.one;
            _bars[u.UnitId] = new Bar
            {
                Root = root.GetComponent<RectTransform>(),
                Fill = fill,
                Label = label,
                Chip = chip,
                ChipText = chipText,
                Buffs = buffs,
                Protection = protection,
            };
            SetProtection(u.UnitId, u.CurrentShield, u.MaxShield);
            UpdateUnit(u, cam);
        }

        public void SetProtection(string unitId, double current, double max)
        {
            if (!_bars.TryGetValue(unitId, out var bar) || bar.Protection == null) return;
            bar.Protection.gameObject.SetActive(max > 0 || current > 0);
            bar.Protection.text = "护盾 Protection " + System.Math.Max(0, current).ToString("0")
                + " / " + System.Math.Max(0, max).ToString("0");
        }

        public void SetPersistentStates(string unitId, IReadOnlyList<RawEffect> effects)
        {
            if (!_bars.TryGetValue(unitId, out var bar)) return;
            var definitions = new List<RawEffect>();
            var counts = new List<int>();
            var indices = new Dictionary<string, int>();
            if (effects != null)
                foreach (var effect in effects)
                {
                    if (effect == null) continue;
                    string key = string.IsNullOrEmpty(effect.persistentIcon) ? effect.id : effect.persistentIcon;
                    if (indices.TryGetValue(key, out int index)) counts[index]++;
                    else { indices[key] = definitions.Count; definitions.Add(effect); counts.Add(1); }
                }
            var signature = new System.Text.StringBuilder();
            for (int i = 0; i < definitions.Count; i++)
                signature.Append(definitions[i].persistentIcon).Append('|').Append(definitions[i].id)
                    .Append(':').Append(counts[i]).Append(';');
            string current = signature.ToString();
            if (bar.StatusSignature == current) return;
            bar.StatusSignature = current;
            foreach (Transform child in bar.Buffs)
            {
                child.gameObject.SetActive(false);
                Object.Destroy(child.gameObject);
            }
            float x = 0, y = 0;
            for (int i = 0; i < definitions.Count; i++)
            {
                var effect = definitions[i];
                // This is the existing imported original widget location. A missing
                // import remains explicit; never substitute another status's image.
                var sprite = string.IsNullOrEmpty(effect.persistentIcon) ? null
                    : Resources.Load<Sprite>("BattleStatus/" + effect.persistentIcon)
                        ?? Resources.Load<Sprite>("UiSprites/widgets/" + effect.persistentIcon);
                if (sprite == null)
                {
                    if (x > 0) { x = 0; y += 28; }
                    string name = string.IsNullOrEmpty(effect.persistentIcon) ? effect.id : effect.persistentIcon;
                    var pending = NewText(bar.Buffs, "pending-" + effect.id,
                        "PENDING-ORIGINAL " + name + " ×" + counts[i], 11, Color.white,
                        TextAnchor.LowerLeft);
                    pending.raycastTarget = false;
                    pending.horizontalOverflow = HorizontalWrapMode.Wrap;
                    var rt = pending.rectTransform;
                    rt.anchorMin = rt.anchorMax = Vector2.zero; rt.pivot = Vector2.zero;
                    rt.anchoredPosition = new Vector2(0, y); rt.sizeDelta = new Vector2(180, 32);
                    y += 34;
                    continue;
                }
                if (x + 42 > 180) { x = 0; y += 28; }
                var slot = new GameObject("status-" + effect.id, typeof(RectTransform));
                slot.transform.SetParent(bar.Buffs, false);
                var slotRect = (RectTransform)slot.transform;
                slotRect.anchorMin = slotRect.anchorMax = Vector2.zero; slotRect.pivot = Vector2.zero;
                slotRect.anchoredPosition = new Vector2(x, y); slotRect.sizeDelta = new Vector2(24, 24);
                var icon = slot.AddComponent<Image>();
                icon.sprite = sprite; icon.color = Color.white; icon.preserveAspect = true; icon.raycastTarget = false;
                var count = NewText(slot.transform, "count", "×" + counts[i], 14, Color.white,
                    TextAnchor.LowerLeft, FontStyle.Bold);
                count.raycastTarget = false;
                count.rectTransform.anchorMin = count.rectTransform.anchorMax = Vector2.zero;
                count.rectTransform.pivot = Vector2.zero;
                count.rectTransform.anchoredPosition = new Vector2(22, -2);
                count.rectTransform.sizeDelta = new Vector2(24, 18);
                x += 44;
            }
            bar.Buffs.sizeDelta = new Vector2(180, y + (x > 0 ? 28 : 0));
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
            {
                RectTransformUtility.ScreenPointToWorldPointInRectangle(
                    (RectTransform)_canvas.transform, sp, _canvas.worldCamera, out var point);
                rt.position = point;
            }
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
            // ORIGINAL-EXTRACTED (slim JSON battleui_view): BattleTimerLabel at
            // design (0, 306), box 512x34, pivot top, fontSize 32 -> x1.5 onto
            // the 1920x1080 canvas = 768x51 at 81px below the top; NGUI renders
            // the label's own left alignment as optically centered for "4:49"
            // (P5 reference), so keep UpperCenter here
            _timer.fontSize = 48; _timer.fontStyle = FontStyle.Bold;
            _timer.alignment = TextAnchor.UpperCenter;
            _timer.color = new Color(1f, 0.36f, 0.28f);
            _timer.text = "4:49";
            var rt = _timer.rectTransform;
            rt.anchorMin = new Vector2(0.5f, 1); rt.anchorMax = new Vector2(0.5f, 1);
            rt.pivot = new Vector2(0.5f, 1);
            // box taller than the 48pt line height -- a 51px box truncates the
            // glyphs against the top edge and renders nothing
            rt.anchoredPosition = new Vector2(0, -70);
            rt.sizeDelta = new Vector2(768, 96);
            var outline = go.AddComponent<Outline>();
            outline.effectColor = new Color(0f, 0.05f, 0.12f, 0.9f);
            outline.effectDistance = new Vector2(2, -2);
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
            // sized for the 1920x1080 reference space (was tuned against the
            // old default 800x600 scaler -- x1.6 keeps the previous look)
            rt.anchoredPosition = new Vector2(19 + index * 87, -16);
            rt.sizeDelta = new Vector2(74, 74);
            var img = go.AddComponent<Image>();
            if (bg != null) img.sprite = bg;
            else img.color = new Color(0.10f, 0.22f, 0.35f, 0.9f);   // PENDING-ORIGINAL fallback
                if (overlayIcon != null)
                {
                    if (overlayIcon == "2x")
                    {
                        var t = NewText(go.transform, "2x", "2x", 27, Color.white, TextAnchor.MiddleCenter, FontStyle.Bold);
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
                        ic.rectTransform.offsetMin = new Vector2(14, 14);
                        ic.rectTransform.offsetMax = new Vector2(-14, -14);
                        _ctl[id + "_icon"] = ic;
                    }
                }
                var btn = go.AddComponent<Button>();
                btn.onClick.AddListener(() => onClick?.Invoke());
                _ctl[id] = img;

                var cap = NewText(_screenCanvas.transform, "cap_" + id, caption, 19,
                    new Color(0.92f, 0.96f, 1f), TextAnchor.UpperCenter);
                // outline keeps the captions readable over the bright sky band
                var capOl = cap.gameObject.AddComponent<Outline>();
                capOl.effectColor = new Color(0f, 0.05f, 0.12f, 0.9f);
                capOl.effectDistance = new Vector2(2, -2);
                var crt = cap.rectTransform;
                crt.anchorMin = new Vector2(0, 1); crt.anchorMax = new Vector2(0, 1);
                crt.pivot = new Vector2(0.5f, 1);
                crt.anchoredPosition = new Vector2(19 + index * 87 + 37, -94);
                crt.sizeDelta = new Vector2(96, 29);
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
        readonly List<Text> _skillCaptions = new();
        RectTransform _targetBar;

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
            // ORIGINAL-EXTRACTED button frames (slim JSON battle_ability_button):
            // basic 100x100 design sliced battle_btn_attack_sliced, ultimate
            // 128x128 ability_button_ult_bg -> 150 / 192 canvas; the ultimate
            // bg sprite was not in the widget harvest -- gold frame stands in
            // (PENDING-ORIGINAL), documented substitution
            _skillBar.sizeDelta = new Vector2(500, 230);

            _skillTitle = NewText(rootGo.transform, "title", actorLabel, 21,
                new Color(0.95f, 0.93f, 0.8f), TextAnchor.UpperRight, FontStyle.Bold);
            var trt = _skillTitle.rectTransform;
            trt.anchorMin = new Vector2(0, 1); trt.anchorMax = new Vector2(1, 1);
            trt.pivot = new Vector2(0.5f, 1);
            trt.anchoredPosition = new Vector2(0, -20);
            trt.sizeDelta = new Vector2(0, 28);

            _skillButtons.Clear();
            _skillCaptions.Clear();
            float x = 0;
            int key = 0;
            string[] keys = { "Q", "W", "E", "R", "T", "Y" };
            foreach (var sk in ActiveAbilities(entry))
            {
                var btnGo = new GameObject("skill_" + sk.id, typeof(RectTransform));
                btnGo.transform.SetParent(rootGo.transform, false);
                var brt = btnGo.GetComponent<RectTransform>();
                brt.anchorMin = new Vector2(0, 0); brt.anchorMax = new Vector2(0, 0);
                brt.pivot = new Vector2(0, 0);
                brt.anchoredPosition = new Vector2(x, 0);
                brt.sizeDelta = new Vector2(150, 150);
                x += 158;

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
                icon.rectTransform.offsetMin = new Vector2(16, 16);
                icon.rectTransform.offsetMax = new Vector2(-16, -16);

                var keyGo = new GameObject("key", typeof(RectTransform));
                keyGo.transform.SetParent(btnGo.transform, false);
                var krt = keyGo.GetComponent<RectTransform>();
                krt.anchorMin = new Vector2(0.5f, 0); krt.anchorMax = new Vector2(0.5f, 0);
                krt.pivot = new Vector2(0.5f, 0);
                krt.anchoredPosition = new Vector2(0, showKeys ? -16 : 6);
                krt.sizeDelta = new Vector2(36, 26);
                if (showKeys)
                {
                    var keyBg = keyGo.AddComponent<Image>();
                    var kbSp = UiKit.LoadWidget("bg_key");
                    if (kbSp != null) { keyBg.sprite = kbSp; keyBg.type = Image.Type.Sliced; }
                    else keyBg.color = new Color(0.05f, 0.10f, 0.16f, 0.9f);   // PENDING-ORIGINAL fallback
                    var kt = NewText(keyGo.transform, "k", keys[key], 18, Color.white, TextAnchor.MiddleCenter, FontStyle.Bold);
                    Stretch(kt.rectTransform);
                }

                var idx = key;
                var button = btnGo.AddComponent<Button>();
                button.onClick.AddListener(() => onPick?.Invoke(idx));
                _skillButtons.Add(button);
                var caption = NewText(btnGo.transform, "caption", sk.name, 18, Color.white, TextAnchor.MiddleCenter);
                caption.raycastTarget = false;
                caption.rectTransform.anchorMin = new Vector2(0, 1);
                caption.rectTransform.anchorMax = Vector2.one;
                caption.rectTransform.pivot = new Vector2(0.5f, 0);
                caption.rectTransform.anchoredPosition = Vector2.zero;
                caption.rectTransform.sizeDelta = new Vector2(0, 48);
                _skillCaptions.Add(caption);
                key++;
                if (key >= keys.Length) break;
            }
            _skillBar.sizeDelta = new Vector2(Mathf.Max(500, x), 270);
        }

        public void SetSkillAvailability(int index, bool ready, string name, int cooldown)
        {
            if (index < 0 || index >= _skillButtons.Count) return;
            _skillButtons[index].interactable = ready;
            _skillCaptions[index].text = cooldown > 0 ? $"{name}\n冷却 {cooldown}" : name;
        }

        // Temporary command bridge; target names/icons come from original unit data.
        // PENDING-ORIGINAL: replace this list with the recovered targeting presentation.
        public void ShowTargets(IReadOnlyList<BattleUnit> targets, System.Func<BattleUnit, string> label,
                                System.Action<BattleUnit> onPick)
        {
            HideTargets();
            EnsureCanvas();
            var go = new GameObject("manualTargets", typeof(RectTransform));
            go.transform.SetParent(_screenCanvas.transform, false);
            _targetBar = go.GetComponent<RectTransform>();
            _targetBar.anchorMin = _targetBar.anchorMax = new Vector2(0, 0);
            _targetBar.pivot = Vector2.zero;
            _targetBar.anchoredPosition = new Vector2(24, 24);
            _targetBar.sizeDelta = new Vector2(340, targets.Count * 54);
            for (int i = 0; i < targets.Count; i++)
            {
                var target = targets[i];
                var buttonGo = new GameObject("target_" + target.UnitId, typeof(RectTransform), typeof(Image), typeof(Button));
                buttonGo.transform.SetParent(go.transform, false);
                var rt = buttonGo.GetComponent<RectTransform>();
                rt.anchorMin = rt.anchorMax = Vector2.zero;
                rt.pivot = Vector2.zero;
                rt.anchoredPosition = new Vector2(0, (targets.Count - 1 - i) * 54);
                rt.sizeDelta = new Vector2(340, 48);
                buttonGo.GetComponent<Image>().color = new Color(0.05f, 0.16f, 0.23f, 0.95f);
                buttonGo.GetComponent<Button>().onClick.AddListener(() => onPick(target));
                var text = NewText(buttonGo.transform, "label", $"{i + 1} · {label(target)}", 22, Color.white, TextAnchor.MiddleCenter);
                Stretch(text.rectTransform);
                text.raycastTarget = false;
            }
        }

        public void HideTargets()
        {
            if (_targetBar != null) Object.Destroy(_targetBar.gameObject);
            _targetBar = null;
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
            HideTargets();
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
            // ORIGINAL-EXTRACTED: TargetContainer 376x112 design at (436, 291)
            // -> 564x168 canvas, 24px off the right edge / 20px off the top
            rt.anchoredPosition = new Vector2(-24, -20);
            rt.sizeDelta = new Vector2(564, 168);

            // portrait + red opponent ring (turn_order_frame_opponent)
            var portGo = new GameObject("portrait", typeof(RectTransform));
            portGo.transform.SetParent(go.transform, false);
            var prt = portGo.GetComponent<RectTransform>();
            prt.anchorMin = Vector2.one; prt.anchorMax = Vector2.one;
            prt.pivot = Vector2.one;
            prt.anchoredPosition = new Vector2(-3, -3);
            prt.sizeDelta = new Vector2(87, 87);
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
            ring.rectTransform.offsetMin = new Vector2(-6, -6);
            ring.rectTransform.offsetMax = new Vector2(6, 6);

            // name + long HP bar left of the portrait.  ORIGINAL-EXTRACTED
            // TargetContainer 376x112 carries the bar as a child (HealthBar at
            // design (60,-10)); HealthBar-Long 768x32 belongs to the
            // TargetContainer-Boss variant and at full 1152px canvas width it
            // blankets the timer, so the squad HUD keeps the bar inside the
            // container like the P5 reference (right edge at the portrait)
            _cmdrName = NewText(go.transform, "name", unitName, 22,
                new Color(0.93f, 0.97f, 0.85f), TextAnchor.UpperRight, FontStyle.Bold);
            var nrt = _cmdrName.rectTransform;
            nrt.anchorMin = new Vector2(0, 1); nrt.anchorMax = new Vector2(1, 1);
            nrt.pivot = new Vector2(1, 1);
            nrt.anchoredPosition = new Vector2(-99, -6);
            nrt.sizeDelta = new Vector2(-15, 30);

            var barGo = new GameObject("hpbar", typeof(RectTransform));
            barGo.transform.SetParent(go.transform, false);
            var brt = barGo.GetComponent<RectTransform>();
            brt.anchorMin = new Vector2(1, 1); brt.anchorMax = new Vector2(1, 1);
            brt.pivot = new Vector2(1, 1);
            brt.anchoredPosition = new Vector2(-96, -44);
            brt.sizeDelta = new Vector2(462, 44);
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
            frt.offsetMin = new Vector2(3, 3); frt.offsetMax = new Vector2(-3, -3);
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
                _turnLabel.fontSize = 29; _turnLabel.alignment = TextAnchor.UpperLeft;
                _turnLabel.color = new Color(0.95f, 0.79f, 0.29f);
                var rt = _turnLabel.GetComponent<RectTransform>();
                rt.anchorMin = new Vector2(0, 1); rt.anchorMax = new Vector2(0, 1);
                rt.pivot = new Vector2(0, 1);
                rt.anchoredPosition = new Vector2(26, -140);
                rt.sizeDelta = new Vector2(960, 48);
            }
            _turnLabel.text = s;
        }

        public void ShowShieldDamage(string unitId, double amount, Camera cam)
            => ShowStatusEvent(unitId, "护盾 −" + (int)amount);

        public void ShowStatusEvent(string unitId, string label)
        {
            if (!_bars.TryGetValue(unitId, out var bar)) return;
            var go = new GameObject("status-event", typeof(RectTransform));
            go.transform.SetParent(_canvas.transform, false);
            var text = go.AddComponent<Text>();
            text.font = UiKit.FontCjk();
            text.fontSize = 18;
            text.color = new Color(0.6f, 0.8f, 1f);
            text.alignment = TextAnchor.MiddleCenter;
            text.text = label;
            var rt = (RectTransform)go.transform;
            rt.position = bar.Root.position + new Vector3(0, 13f, 0);
            rt.sizeDelta = new Vector2(180, 30);
            rt.localScale = new Vector3(0.3f, 0.3f, 1f);
            go.AddComponent<Floater>();
        }

        public void ShowDamage(string unitId, double amount, bool crit, Camera cam, bool heal = false)
        {
            if (!_bars.TryGetValue(unitId, out var bar)) return;
            var go = new GameObject("dmg", typeof(RectTransform));
            go.transform.SetParent(_canvas.transform, false);
            var t = go.AddComponent<Text>();
            t.font = Resources.GetBuiltinResource<Font>("LegacyRuntime.ttf");
            t.fontSize = crit ? 26 : 19;
            t.fontStyle = crit ? FontStyle.Bold : FontStyle.Normal;
            t.color = heal ? new Color(0.40f, 0.95f, 0.45f)
                    : crit ? new Color(1f, 0.62f, 0.20f) : Color.white;
            t.alignment = TextAnchor.MiddleCenter;
            t.text = heal ? $"+{(int)amount}" : crit ? $"{(int)amount}!" : $"{(int)amount}";
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
