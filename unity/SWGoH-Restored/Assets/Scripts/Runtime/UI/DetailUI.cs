// DetailUI -- P4 character detail screen (acceptance: shots/reference/P4.png).
// Left skill list (icon + name + maxed/type tag), center 3D GLB rig on a
// holographic pedestal with slow auto-rotate, tag chips row, relic board
// top-right, unit power bottom, back/home navigation and the battle-test
// entry that loads the existing BattleDemo scene.
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

namespace SwgohRestored.UI
{
    public class DetailUI : MonoBehaviour
    {
        public static string SelectedDefId = "SUPREMELEADERKYLOREN";

        public Transform CharacterPrefabRoot;   // staged GLB rigs (editor scene builder)
        public Transform RigMount;              // separate root: the canvas host's
                                                // transform is driven by the canvas
                                                // system and must not parent 3D
        public float RotateSpeed = 14f;

        UnitEntry _unit;
        GameObject _rig;

        void Start()
        {
            var units = UiData.LoadUnits();
            _unit = units.Find(u => u.def_id == SelectedDefId)
                     ?? units.Find(u => u.def_id == "SUPREMELEADERKYLOREN");
            if (_unit != null) SelectedDefId = _unit.def_id;
            BuildUi();
            SpawnRig();
        }

        void BuildUi()
        {
            _canvas = UiKit.EnsureCanvas(gameObject);
            var driver = gameObject.AddComponent<UiCaptureDriver>();
            driver.SceneId = "detail";
            driver.CaptureFrame = 45;
            driver.OutputPath = @"F:\PPfavorite\Work\goh\unity\ui_detail.png";

            // ---- top bar: back + home ----
            var backBtn = UiKit.NewImage(_canvas.transform, "Back", UiKit.LoadWidget("button_back"), Color.white);
            var brt = backBtn.rectTransform;
            brt.anchorMin = new Vector2(0, 1); brt.anchorMax = new Vector2(0, 1);
            brt.anchoredPosition = new Vector2(52, -52);
            brt.sizeDelta = new Vector2(76, 76);
            backBtn.gameObject.AddComponent<Button>().onClick.AddListener(() =>
                SceneManager.LoadScene("RosterScene"));
            var homeBtn = UiKit.NewImage(_canvas.transform, "Home", UiKit.LoadWidget("button_home"), Color.white);
            var hrt = homeBtn.rectTransform;
            hrt.anchorMin = new Vector2(1, 1); hrt.anchorMax = new Vector2(1, 1);
            hrt.anchoredPosition = new Vector2(-52, -52);
            hrt.sizeDelta = new Vector2(64, 64);
            homeBtn.gameObject.AddComponent<Button>().onClick.AddListener(() =>
                SceneManager.LoadScene("RosterScene"));

            // ---- name + tag chips ----
            UiKit.NewText(_canvas.transform, "UnitName", _unit.name, 40, new Color(0.95f, 0.93f, 0.82f),
                TextAnchor.MiddleLeft, new Vector2(0, 1), new Vector2(1, 1),
                new Vector2(120, -66), new Vector2(-320, -6), FontStyle.Bold);

            var chipRow = UiKit.NewRect(_canvas.transform, "TagChips",
                new Vector2(0, 1), new Vector2(1, 1), new Vector2(120, -110), new Vector2(-320, -74));
            float x = 0;
            foreach (var tag in _unit.tags)
            {
                float w = tag.Length * 20f + 30f;
                var chip = UiKit.NewImage(chipRow, "chip_" + tag, UiKit.LoadWidget("bg_pillpanel"),
                    new Color(0.85f, 0.95f, 1.15f, 0.5f));
                var rt = chip.GetComponent<RectTransform>();
                rt.anchorMin = new Vector2(0, 0.5f); rt.anchorMax = new Vector2(0, 0.5f);
                rt.pivot = new Vector2(0, 0.5f);
                rt.anchoredPosition = new Vector2(x, 0);
                rt.sizeDelta = new Vector2(w, 34);
                UiKit.NewText(chip.transform, "t", tag, 19, new Color(0.88f, 0.96f, 1.1f), TextAnchor.MiddleCenter);
                x += w + 10;
            }

            // ---- currency chip top-right (unit power; account credits are
            // not staged -- documented substitution) ----
            var cur = UiKit.NewImage(_canvas.transform, "Currency", UiKit.LoadWidget("counter_currency_small"),
                Color.white);
            var crt = cur.rectTransform;
            crt.anchorMin = new Vector2(1, 1); crt.anchorMax = new Vector2(1, 1);
            crt.anchoredPosition = new Vector2(-215, -50);
            crt.sizeDelta = new Vector2(245, 52);
            UiKit.NewText(cur.transform, "t", UiKit.FormatNumber(_unit.power), 28,
                new Color(1f, 0.87f, 0.4f), TextAnchor.MiddleRight,
                new Vector2(0, 0), new Vector2(1, 1), new Vector2(-120, 0), new Vector2(-40, 0));

            // ---- left skill list ----
            var skillList = UiKit.NewRect(_canvas.transform, "SkillList",
                new Vector2(0, 0), new Vector2(0, 1), new Vector2(60, 90), new Vector2(430, -140));
            float y = 0;
            foreach (var sk in _unit.skills)
            {
                var row = UiKit.NewRect(skillList, "skill_" + sk.id,
                    new Vector2(0, 1), new Vector2(1, 1),
                    new Vector2(0, -y - 70), new Vector2(0, -y));
                var frame = UiKit.NewImage(row, "frame", UiKit.LoadWidget("detail_ability_button"), Color.white);
                UiKit.SetAbs(frame.rectTransform, new Vector2(0, 1), new Vector2(30, -35), new Vector2(60, 60));
                var icon = UiKit.LoadIcon(sk.icon);
                var iconImg = UiKit.NewImage(frame.transform, "icon", icon, Color.white);
                iconImg.preserveAspect = true;
                UiKit.Stretch(iconImg.rectTransform, Vector2.zero, Vector2.one);
                iconImg.rectTransform.offsetMin = new Vector2(5, 5);
                iconImg.rectTransform.offsetMax = new Vector2(-5, -5);
                // explicit from-top bands: name in the top half, sub below it
                UiKit.NewText(row, "name", sk.name, 23, new Color(0.93f, 0.96f, 1f), TextAnchor.LowerLeft,
                    new Vector2(0, 1), new Vector2(1, 1), new Vector2(72, -34), new Vector2(-4, -2));
                UiKit.NewText(row, "sub", "满级" + sk.tag, 17, new Color(0.62f, 0.66f, 0.72f), TextAnchor.UpperLeft,
                    new Vector2(0, 1), new Vector2(1, 1), new Vector2(72, -66), new Vector2(-4, -36));
                y += 72;
            }

            // ---- relic board top-right (original panel + badge tiles;
            // the connector wiring between tiles is PENDING-ORIGINAL) ----
            var board = UiKit.NewImage(_canvas.transform, "RelicBoard",
                UiKit.LoadWidget("detail_relic_background"), Color.white);
            var brd = board.rectTransform;
            brd.anchorMin = new Vector2(1, 1); brd.anchorMax = new Vector2(1, 1);
            brd.anchoredPosition = new Vector2(-160, -330);
            brd.sizeDelta = new Vector2(250, 230);
            var badge = UiKit.LoadWidget("icon_relic_badge");
            for (int i = 0; i < 6; i++)
            {
                var tile = UiKit.NewImage(board.transform, "tile" + i, badge,
                    new Color(1f, 0.84f, 0.35f));
                var trt = tile.rectTransform;
                trt.anchorMin = new Vector2(0, 1); trt.anchorMax = new Vector2(0, 1);
                trt.pivot = new Vector2(0.5f, 0.5f);
                float col = i % 3, rowI = i / 3;
                trt.anchoredPosition = new Vector2(52 + col * 78, -52 - rowI * 110);
                trt.sizeDelta = new Vector2(56, 56);
                UiKit.NewText(tile.transform, "tier", "15", 14, Color.white, TextAnchor.MiddleCenter,
                    FontStyle.Bold);
            }

            // ---- bottom: gear tier + unit power ----
            UiKit.NewText(_canvas.transform, "GearTier", "装备等级XIII", 28, new Color(0.95f, 0.45f, 0.25f),
                TextAnchor.LowerCenter, new Vector2(0.5f, 0), new Vector2(0.5f, 0),
                new Vector2(-200, 108), new Vector2(200, 150));
            UiKit.NewText(_canvas.transform, "Power", "强度：" + UiKit.FormatNumber(_unit.power), 32,
                new Color(0.95f, 0.97f, 1f), TextAnchor.LowerCenter, new Vector2(0.5f, 0), new Vector2(0.5f, 0),
                new Vector2(-200, 52), new Vector2(200, 100));
            var leftArrow = UiKit.NewImage(_canvas.transform, "LeftArrow", UiKit.LoadWidget("pip_gold"),
                Color.white);
            var lrt = leftArrow.rectTransform;
            lrt.anchorMin = new Vector2(0.5f, 0); lrt.anchorMax = new Vector2(0.5f, 0);
            lrt.anchoredPosition = new Vector2(-270, 66);
            lrt.sizeDelta = new Vector2(30, 40);
            lrt.localRotation = Quaternion.Euler(0, 0, 180);
            var rightArrow = UiKit.NewImage(_canvas.transform, "RightArrow", UiKit.LoadWidget("pip_gold"),
                Color.white);
            var rrt = rightArrow.rectTransform;
            rrt.anchorMin = new Vector2(0.5f, 0); rrt.anchorMax = new Vector2(0.5f, 0);
            rrt.anchoredPosition = new Vector2(270, 66);
            rrt.sizeDelta = new Vector2(30, 40);

            // ---- battle test entry (loads the existing BattleDemo scene) ----
            var battleBtn = UiKit.NewImage(_canvas.transform, "BattleTest",
                UiKit.LoadWidget("button_blue"), Color.white);
            var btnRt = battleBtn.rectTransform;
            btnRt.anchorMin = new Vector2(1, 0); btnRt.anchorMax = new Vector2(1, 0);            btnRt.anchoredPosition = new Vector2(-120, 60);
            btnRt.sizeDelta = new Vector2(220, 58);
            UiKit.NewText(battleBtn.transform, "t", "战斗测试", 24, Color.white, TextAnchor.MiddleCenter,
                FontStyle.Bold);
            battleBtn.gameObject.AddComponent<Button>().onClick.AddListener(() =>
                SceneManager.LoadScene("BattleDemo"));
        }

        Canvas _canvas;

        void SpawnRig()
        {
            if (CharacterPrefabRoot == null)
            {
                Debug.LogError("[DetailUI] CharacterPrefabRoot not wired");
                return;
            }
            if (string.IsNullOrEmpty(_unit.folder))
            {
                Debug.LogWarning($"[DetailUI] unit has no folder: {_unit.def_id}");
                return;
            }
            var t = CharacterPrefabRoot.transform.Find(_unit.folder);
            if (t == null)
            {
                Debug.LogWarning($"[DetailUI] rig not staged: {_unit.folder}");
                return;
            }
            _rig = Instantiate(t.gameObject, RigMount != null ? RigMount : transform);
            _rig.name = _unit.def_id;
            _rig.transform.localPosition = new Vector3(0, 0.02f, 0);
            _rig.transform.localRotation = Quaternion.Euler(0, 165f, 0);   // face the camera
            _rig.SetActive(true);
            var rigAnim = _rig.GetComponentInChildren<Animator>();
            if (rigAnim != null)
            {
                // The exported clips carry baked root keys ((-320,-240)
                // authored / (196,147) in Idle) that fling the mesh off-screen
                // and fight every phase of transform pinning.  These units are
                // static meshes -- the bind pose already IS the standing idle
                // figure, so drop the animator and let Update() auto-rotate.
                rigAnim.enabled = false;
                Destroy(rigAnim);
            }
            var rends = _rig.GetComponentsInChildren<Renderer>();
            var bounds = new Bounds(_rig.transform.position, Vector3.zero);
            foreach (var r in rends) bounds.Encapsulate(r.bounds);
            Debug.Log($"[DetailUI] spawned {_unit.folder} renderers={rends.Length} " +
                      $"bounds={bounds.center} {bounds.size}");
        }

        void Update()
        {
            if (_rig != null)
                _rig.transform.Rotate(0, RotateSpeed * Time.deltaTime, 0, Space.World);
        }
    }
}
