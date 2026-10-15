// UiKit -- shared programmatic-uGUI helpers for the roster/detail screens,
// the ui_units.json data model (staged by unity/stage_ui_data.py from the
// original catalog+loc data) and the batch screenshot driver.
//
// Asset provenance: every widget sprite comes from extracted_ui/atlas_sprites
// (NGUI UIAtlas sprite rects cropped from the original atlas textures by
// extracted_ui/harvest_ui_assets.py) and is imported into
// Assets/Resources/UiSprites/widgets by the SWGoH/UI editor menu.
// Colored rectangles are used ONLY where no original sprite exists
// (each such case is listed PENDING-ORIGINAL in the summary).
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using UnityEngine;
using UnityEngine.UI;

namespace SwgohRestored.UI
{
    // ---- data model (StreamingAssets/ui_units.json) ----
    [Serializable]
    public class SkillEntry
    {
        public string id;
        public string name;
        public string icon;
        public bool icon_ok;
        public string tag;
    }

    [Serializable]
    public class UnitEntry
    {
        public string def_id;
        public string folder;
        public string name;
        public int level;
        public int stars;
        public int power;
        public string alignment;   // light / dark / neutral (categoryId -> ForceAlignment_*)
        public bool legend;
        public string[] tags;      // original category descKey -> loc_cn strings
        public List<SkillEntry> skills;
        public string portrait;    // tex.charui_* name matched in extracted_ui/charui
        public int combat_type;
    }

    public static class UiData
    {
        public static List<UnitEntry> LoadUnits()
        {
            var path = Path.Combine(Application.streamingAssetsPath, "ui_units.json");
            var json = File.ReadAllText(path);
            var wrapped = "{\"items\":" + json + "}";
            return JsonUtility.FromJson<UnitList>(wrapped).items.ToList();
        }

        [Serializable]
        class UnitList { public UnitEntry[] items; }
    }

    public static class UiKit
    {
        // Alignment ring colors measured from shots/reference/P3.png
        // (light=blue, dark=red, neutral=gold).
        public static Color AlignColor(string alignment)
        {
            switch (alignment)
            {
                case "light": return new Color(0.24f, 0.62f, 1.00f);
                case "dark": return new Color(0.90f, 0.20f, 0.18f);
                default: return new Color(0.92f, 0.72f, 0.22f);
            }
        }

        // The game ships a custom TTF (extracted as EmptyFont.ttf, invalid
        // payload) -- PENDING-ORIGINAL.  Use a system CJK face so the original
        // loc_cn strings render; LegacyRuntime covers the latin fallback.
        public static Font FontCjk()
        {
            if (_cjk == null)
            {
                _cjk = Font.CreateDynamicFontFromOSFont(
                    new[] { "Microsoft YaHei", "Microsoft YaHei UI", "SimHei", "SimSun", "Arial" }, 20);
            }
            return _cjk;
        }
        static Font _cjk;

        public static Canvas EnsureCanvas(GameObject host)
        {
            var canvas = host.GetComponent<Canvas>();
            if (canvas != null) return canvas;
            canvas = host.AddComponent<Canvas>();
            canvas.renderMode = RenderMode.ScreenSpaceOverlay;
            var scaler = host.AddComponent<CanvasScaler>();
            scaler.uiScaleMode = CanvasScaler.ScaleMode.ScaleWithScreenSize;
            scaler.referenceResolution = new Vector2(1920, 1080);
            scaler.matchWidthOrHeight = 0.5f;
            host.AddComponent<GraphicRaycaster>();
            return canvas;
        }

        // Overlay canvases don't enter cam.Render() -- batch screenshots need
        // ScreenSpaceCamera bound to the rendering camera (BattleHUD pattern).
        // planeDistance is kept tiny so nearer 3D geometry (floor, pedestal)
        // can never occlude the UI while capturing.
        public static void SetCamera(Canvas canvas, Camera cam)
        {
            if (canvas != null && cam != null)
            {
                canvas.worldCamera = cam;
                canvas.renderMode = RenderMode.ScreenSpaceCamera;
                canvas.planeDistance = 0.2f;
            }
        }

        public static RectTransform NewRect(Transform parent, string name, Vector2 anchorMin,
            Vector2 anchorMax, Vector2 offsetMin, Vector2 offsetMax)
        {
            var go = new GameObject(name, typeof(RectTransform));
            go.transform.SetParent(parent, false);
            var rt = go.GetComponent<RectTransform>();
            rt.anchorMin = anchorMin;
            rt.anchorMax = anchorMax;
            rt.offsetMin = offsetMin;
            rt.offsetMax = offsetMax;
            return rt;
        }

        // anchors-relative placement used by both screens
        public static void SetAbs(RectTransform rt, Vector2 anchor, Vector2 pos, Vector2 size)
        {
            rt.anchorMin = anchor;
            rt.anchorMax = anchor;
            rt.anchoredPosition = pos;
            rt.sizeDelta = size;
        }

        public static void Stretch(RectTransform rt, Vector2 offsetMin, Vector2 offsetMax)
        {
            rt.anchorMin = Vector2.zero;
            rt.anchorMax = Vector2.one;
            rt.offsetMin = offsetMin;
            rt.offsetMax = offsetMax;
        }

        public static Image NewImage(Transform parent, string name, Sprite sprite, Color color)
        {
            var go = new GameObject(name, typeof(RectTransform));
            go.transform.SetParent(parent, false);
            var img = go.AddComponent<Image>();
            img.sprite = sprite;
            img.color = color;
            return img;
        }

        public static Text NewText(Transform parent, string name, string content, int size, Color color,
            TextAnchor anchor, Vector2 anchorMin, Vector2 anchorMax, Vector2 offsetMin, Vector2 offsetMax,
            FontStyle style = FontStyle.Normal)
        {
            var go = new GameObject(name, typeof(RectTransform));
            go.transform.SetParent(parent, false);
            var t = go.AddComponent<Text>();
            t.font = FontCjk();
            t.fontSize = size;
            t.fontStyle = style;
            t.color = color;
            t.alignment = anchor;
            t.text = content;
            t.horizontalOverflow = HorizontalWrapMode.Overflow;
            t.verticalOverflow = VerticalWrapMode.Overflow;
            var rt = go.GetComponent<RectTransform>();
            rt.anchorMin = anchorMin;
            rt.anchorMax = anchorMax;
            rt.offsetMin = offsetMin;
            rt.offsetMax = offsetMax;
            return t;
        }

        // full-stretch child helpers
        public static Text NewText(Transform parent, string name, string content, int size, Color color,
            TextAnchor anchor, FontStyle style = FontStyle.Normal)
        {
            return NewText(parent, name, content, size, color, anchor,
                Vector2.zero, Vector2.one, Vector2.zero, Vector2.zero, style);
        }

        public static Sprite LoadWidget(string name)
        {
            var sp = Resources.Load<Sprite>("UiSprites/widgets/" + name);
            if (sp == null) Debug.LogWarning("[UiKit] widget sprite missing: " + name);
            return sp;
        }

        public static Sprite LoadPortrait(string defId)
        {
            return Resources.Load<Sprite>("UiSprites/portraits/" + defId);
        }

        // circular-masked portrait (uGUI Mask proved unreliable in this setup,
        // so alpha is baked into a cached texture instead)
        public static Sprite LoadCircularPortrait(string defId, int size)
        {
            if (_circular.TryGetValue(defId, out var cached) && cached != null) return cached;
            var src = Resources.Load<Texture2D>("UiSprites/portraits/" + defId);
            if (src == null) return null;
            int w = Mathf.Min(size, src.width), h = Mathf.Min(size, src.height);
            var rt = RenderTexture.GetTemporary(w, h, 0, RenderTextureFormat.ARGB32);
            var prev = RenderTexture.active;
            Graphics.Blit(src, rt);
            RenderTexture.active = rt;
            var read = new Texture2D(w, h, TextureFormat.RGBA32, false);
            read.ReadPixels(new Rect(0, 0, w, h), 0, 0);
            read.Apply();
            RenderTexture.active = prev;
            RenderTexture.ReleaseTemporary(rt);
            var px = read.GetPixels32();
            float r = w * 0.5f;
            for (int yy = 0; yy < h; yy++)
                for (int xx = 0; xx < w; xx++)
                {
                    float dx = xx - r + 0.5f, dy = yy - r + 0.5f;
                    float d = Mathf.Sqrt(dx * dx + dy * dy);
                    byte a = px[yy * w + xx].a;
                    byte m = (byte)(d <= r - 1f ? 255 : Mathf.Clamp01((r - d) / 1.5f) * 255);
                    px[yy * w + xx] = new Color32(px[yy * w + xx].r, px[yy * w + xx].g, px[yy * w + xx].b,
                        (byte)(a * m / 255));
                }
            read.SetPixels32(px);
            read.Apply();
            var sp = Sprite.Create(read, new Rect(0, 0, w, h), new Vector2(0.5f, 0.5f), 100f);
            sp.name = "c_" + defId;
            _circular[defId] = sp;
            return sp;
        }
        static readonly Dictionary<string, Sprite> _circular = new();

        public static Sprite LoadIcon(string texName)
        {
            if (string.IsNullOrEmpty(texName)) return null;
            return Resources.Load<Sprite>("UiSprites/icons/" + Sanitize(texName));
        }

        public static string Sanitize(string name)
        {
            return name.Replace(' ', '_').Replace('.', '_');
        }

        // Circular alpha mask for portraits -- generated at runtime, no asset.
        public static Sprite CircleMask(int size)
        {
            if (_circle != null) return _circle;
            var tex = new Texture2D(size, size, TextureFormat.RGBA32, false);
            float r = size * 0.5f;
            var px = new Color32[size * size];
            for (int y = 0; y < size; y++)
                for (int x = 0; x < size; x++)
                {
                    float dx = x - r + 0.5f, dy = y - r + 0.5f;
                    float d = Mathf.Sqrt(dx * dx + dy * dy);
                    byte a = (byte)(d <= r - 1.5f ? 255 : Mathf.Clamp01((r - d) / 2f) * 255);
                    px[y * size + x] = new Color32(255, 255, 255, a);
                }
            tex.SetPixels32(px);
            tex.Apply();
            _circle = Sprite.Create(tex, new Rect(0, 0, size, size), new Vector2(0.5f, 0.5f), 100f);
            _circle.name = "circle_mask";
            return _circle;
        }
        static Sprite _circle;

        public static string FormatNumber(long v)
        {
            return v.ToString("N0", System.Globalization.CultureInfo.InvariantCulture);
        }
    }

    // Batch capture: the batch harness writes the marker file before entering
    // play mode (statics don't survive the domain reload).  Roster captures
    // ui_roster.png then loads DetailScene; detail captures ui_detail.png and
    // quits.  Manual play (no marker) never captures, and BattleDemo is
    // untouched by this driver.
    public class UiCaptureDriver : MonoBehaviour
    {
        public string SceneId;          // "roster" | "detail"
        public int CaptureFrame = 45;
        public string OutputPath;

        int _frame;
        bool _captured;

        void Start()
        {
            if (SceneId == "roster" && File.Exists(@"F:\PPfavorite\Work\goh\unity\ui_capture.marker"))
                _active = true;
            else if (SceneId == "detail")
                _active = File.Exists(@"F:\PPfavorite\Work\goh\unity\ui_capture.marker")
                          || File.Exists(@"F:\PPfavorite\Work\goh\unity\ui_capture_detail.marker");
        }

        bool _active;

        void Update()
        {
            if (!_active || _captured) return;
            _frame++;
            if (_frame == CaptureFrame)
            {
                _captured = true;
                CaptureShot(OutputPath);
                Debug.Log($"[UiCapture] {SceneId} -> {OutputPath}");
                if (SceneId == "roster")
                {
                    File.WriteAllText(@"F:\PPfavorite\Work\goh\unity\ui_capture_detail.marker", "go");
                    UnityEngine.SceneManagement.SceneManager.LoadScene("DetailScene");
                }
                else
                {
#if UNITY_EDITOR
                    // a batch editor does not exit when play mode stops -- force it
                    UnityEditor.EditorApplication.isPlaying = false;
                    if (Application.isBatchMode) UnityEditor.EditorApplication.Exit(0);
#endif
                    Application.Quit(0);
                }
            }
        }

        void CaptureShot(string path)
        {
            var cam = Camera.main;
            if (cam == null) { Debug.LogError("[UiCapture] no camera"); return; }
            var canvas = GetComponentInParent<Canvas>();
            if (canvas == null) canvas = FindObjectOfType<Canvas>();
            UiKit.SetCamera(canvas, cam);   // overlay canvas won't render into cam.Render()
            var prevRT = cam.targetTexture;
            var rt = new RenderTexture(1600, 900, 24);
            cam.targetTexture = rt;
            cam.Render();
            RenderTexture.active = rt;
            var tex = new Texture2D(1600, 900, TextureFormat.RGB24, false);
            tex.ReadPixels(new Rect(0, 0, 1600, 900), 0, 0);
            tex.Apply();
            cam.targetTexture = prevRT;
            RenderTexture.active = null;
            File.WriteAllBytes(path, tex.EncodeToPNG());
            Destroy(tex);
            Destroy(rt);
            // leave the canvas in overlay mode for interactive play
            if (canvas != null) canvas.renderMode = RenderMode.ScreenSpaceOverlay;
        }
    }
}
