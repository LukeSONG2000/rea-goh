// BattleHUD -- programmatic uGUI overlay for the battle: per-unit health
// bars (world-to-screen), a bottom action bar echoing the game's skill row,
// and floating damage numbers.  Built entirely from code (no prefab assets).
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

namespace SwgohRestored.Battle
{
    public class BattleHUD : MonoBehaviour
    {
        Canvas _canvas;
        readonly Dictionary<string, Bar> _bars = new();
        Text _turnLabel;

        /// Overlay canvases don't enter cam.Render() -- batch screenshots need
        /// the canvas in ScreenSpaceCamera mode bound to the rendering camera.
        public void SetCamera(Camera cam)
        {
            EnsureCanvas();
            if (cam != null)
            {
                _canvas.worldCamera = cam;
                _canvas.renderMode = RenderMode.ScreenSpaceCamera;
                var plane = 10f;
                _canvas.planeDistance = plane;
            }
        }

        class Bar
        {
            public RectTransform Root;
            public Image Fill;
            public Text Label;
        }

        public void EnsureCanvas()
        {
            if (_canvas != null) return;
            var go = new GameObject("HUD");
            _canvas = go.AddComponent<Canvas>();
            _canvas.renderMode = RenderMode.ScreenSpaceOverlay;
            go.AddComponent<CanvasScaler>().uiScaleMode = CanvasScaler.ScaleMode.ScaleWithScreenSize;
            go.AddComponent<GraphicRaycaster>();
        }

        public void TrackUnit(BattleUnit u, Camera cam)
        {
            EnsureCanvas();
            var rootGo = new GameObject($"hp_{u.BaseId}", typeof(RectTransform));
            rootGo.transform.SetParent(_canvas.transform, false);
            var root = rootGo.GetComponent<RectTransform>();
            root.sizeDelta = new Vector2(90, 14);

            var bg = CreateImage(rootGo.transform, new Color(0, 0, 0, 0.6f));
            var bgrt = bg.GetComponent<RectTransform>();
            bgrt.anchorMin = Vector2.zero; bgrt.anchorMax = Vector2.one;

            var fillGo = new GameObject("fill", typeof(RectTransform));
            fillGo.transform.SetParent(root.transform, false);
            var fill = fillGo.AddComponent<Image>();
            fill.color = u.Side == 0 ? new Color(0.30f, 0.80f, 0.35f) : new Color(0.88f, 0.32f, 0.30f);
            var frt = fill.GetComponent<RectTransform>();
            frt.anchorMin = Vector2.zero; frt.anchorMax = Vector2.one;
            frt.offsetMin = new Vector2(2, 2); frt.offsetMax = new Vector2(-2, -2);

            var labelGo = new GameObject("label", typeof(RectTransform));
            labelGo.transform.SetParent(root.transform, false);
            var label = labelGo.AddComponent<Text>();
            label.font = Resources.GetBuiltinResource<Font>("LegacyRuntime.ttf");
            label.fontSize = 11; label.alignment = TextAnchor.MiddleCenter;
            label.color = Color.white;
            var lrt = label.GetComponent<RectTransform>();
            lrt.anchorMin = Vector2.zero; lrt.anchorMax = Vector2.one; lrt.sizeDelta = Vector2.zero;

            // ScreenSpaceCamera world size at planeDistance=10 makes 90 canvas
            // units render ~6x oversized -- scale bars down to ~1.8m worth.
            root.localScale = new Vector3(0.16f, 0.16f, 1f);
            _bars[u.UnitId] = new Bar { Root = root.GetComponent<RectTransform>(), Fill = fill, Label = label };
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

        public void UpdateUnit(BattleUnit u, Camera cam)
        {
            if (!_bars.TryGetValue(u.UnitId, out var bar)) return;
            bar.Root.gameObject.SetActive(u.Alive);
            if (!u.Alive) return;
            var rig = GameObject.Find(u.UnitId);   // SpawnRig names the instance by UnitId
            Vector3 anchor = rig ? rig.transform.position + Vector3.up * 2.2f : Vector3.zero;
            var sp = cam.WorldToScreenPoint(anchor);
            var rt = bar.Root;
            if (_canvas != null && _canvas.worldCamera != null &&
                _canvas.renderMode == RenderMode.ScreenSpaceCamera)
                rt.position = anchor;      // SSC canvas: element lives in world space
            else
                rt.position = new Vector3(sp.x, sp.y, 0);   // overlay: screen px
            float frac = u.MaxHp > 0 ? Mathf.Clamp01((float)(u.CurrentHp / u.MaxHp)) : 0;
            bar.Fill.transform.localScale = new Vector3(frac, 1, 1);
            if (bar.Label != null) bar.Label.text = $"{(int)u.CurrentHp}";
        }

        public void SetTurnText(string s)
        {
            EnsureCanvas();
            if (_turnLabel == null)
            {
                var go = new GameObject("turn", typeof(RectTransform));
                go.transform.SetParent(_canvas.transform, false);
                _turnLabel = go.AddComponent<Text>();
                _turnLabel.font = Resources.GetBuiltinResource<Font>("LegacyRuntime.ttf");
                _turnLabel.fontSize = 18; _turnLabel.alignment = TextAnchor.UpperLeft;
                _turnLabel.color = new Color(0.95f, 0.79f, 0.29f);
                var rt = _turnLabel.GetComponent<RectTransform>();
                rt.anchorMin = new Vector2(0, 1); rt.anchorMax = new Vector2(0, 1);
                rt.pivot = new Vector2(0, 1);
                rt.anchoredPosition = new Vector2(16, -14);
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
            rt.localScale = new Vector3(0.16f, 0.16f, 1f);
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
