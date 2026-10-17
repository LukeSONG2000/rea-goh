using System;
using System.Linq;
using UnityEngine;
using UnityEngine.SceneManagement;

namespace SwgohRestored.Battle
{
    // Original serialized layout and environment attachment transforms. Unity's
    // glTF importer reverses the exporter's handedness conversion; no X flip here.
    public class BattleStage : MonoBehaviour
    {
        [Serializable] public class Pose
        {
            public string path, componentPathId, transformPathId;
            public Vector3 position, scale = Vector3.one;
            public Quaternion rotation = Quaternion.identity;
        }
        [Serializable] public class StageCamera : Pose { public float fov, near, far; public bool orthographic; }
        [Serializable] public class Restriction { public string tag; public Pose marker; }
        [Serializable] public class Group : Pose { public Pose[] slots; public Restriction[] restricted; }
        [Serializable] public class Layout { public string id; public Group[] groups; public StageCamera camera; }
        [Serializable] public class Layouts { public Layout[] layouts; }
        [Serializable] public class Attachment { public int layout; public Pose marker; }
        [Serializable] public class StageLight : Pose
        {
            public int type, enabled; public Color color; public float intensity;
            public long cullingMask = -1; public float range = 10, spotAngle = 30;
        }
        [Serializable] public class StageLayer { public string path; public int layer; }
        [Serializable] public class Fog
        {
            public int FogEnabled, FogMode; public Color FogColor, AmbientLightColor;
            public float FogDensity, LinearFogStart, LinearFogEnd;
        }
        [Serializable] public class Environment
        {
            public string id; public Attachment[] layouts; public StageLight[] lights;
            public StageLayer[] layers; public Fog fog;
        }
        public Layout OriginalLayout { get; private set; }
        public Environment OriginalEnvironment { get; private set; }
        public Transform LayoutRoot { get; private set; }
        public bool Fleet { get; private set; }

        public void Assemble(bool fleet)
        {
            Fleet = fleet;
            string envId = fleet ? "env_space_endor_01_pre" : "env_tatooine_spaceport_pre";
            string layoutId = fleet ? "battle_ship_pve_layout" : "battle_character_pve_layout";
            var layouts = Read<Layouts>("stage_configuration");
            OriginalLayout = layouts.layouts.First(l => l.id == layoutId);
            OriginalEnvironment = Read<Environment>(envId + "_configuration");
            var prefab = Resources.Load<GameObject>("BattleEnvironments/" + envId);
            if (prefab == null) throw new InvalidOperationException("PENDING-ORIGINAL environment " + envId);
            var environment = Instantiate(prefab, transform);
            environment.name = envId;
            // Restore original object layers for planet/ship lighting masks.
            foreach (var layer in OriginalEnvironment.layers ?? Array.Empty<StageLayer>())
            {
                foreach (var node in environment.GetComponentsInChildren<Transform>(true))
                {
                    string path = node.name;
                    for (var parent = node.parent; parent != null && parent != environment.transform;
                        parent = parent.parent) path = parent.name + "/" + path;
                    if (path == layer.path || path.EndsWith("/" + layer.path)) node.gameObject.layer = layer.layer;
                }
            }
            if (fleet) OriginalSpaceMaterials.Apply(environment);
            LayoutRoot = new GameObject("OriginalLayout").transform;
            LayoutRoot.SetParent(transform, false);
            Apply(LayoutRoot, OriginalEnvironment.layouts.First(l => l.layout == 0).marker);
            var camera = Camera.main;
            if (camera == null)
            {
                camera = new GameObject("BattleCam", typeof(Camera), typeof(AudioListener)).GetComponent<Camera>();
                camera.tag = "MainCamera";
            }
            camera.transform.SetParent(LayoutRoot, false);
            Apply(camera.transform, OriginalLayout.camera);
            camera.fieldOfView = OriginalLayout.camera.fov;
            camera.nearClipPlane = OriginalLayout.camera.near;
            camera.farClipPlane = OriginalLayout.camera.far;
            camera.orthographic = OriginalLayout.camera.orthographic;
            camera.clearFlags = CameraClearFlags.SolidColor;
            camera.backgroundColor = Color.black;
            foreach (var old in new[] { "Floor", "KeyLight" })
            {
                var obj = GameObject.Find(old);
                if (obj != null) obj.SetActive(false);
            }
            var fog = OriginalEnvironment.fog;
            if (fog != null)
            {
                RenderSettings.fog = fog.FogEnabled != 0;
                RenderSettings.fogMode = (FogMode)fog.FogMode;
                RenderSettings.fogColor = fog.FogColor;
                RenderSettings.fogDensity = fog.FogDensity;
                RenderSettings.fogStartDistance = fog.LinearFogStart;
                RenderSettings.fogEndDistance = fog.LinearFogEnd;
                RenderSettings.ambientMode = UnityEngine.Rendering.AmbientMode.Flat;
                RenderSettings.ambientLight = fog.AmbientLightColor;
            }
            foreach (var raw in OriginalEnvironment.lights ?? Array.Empty<StageLight>())
            {
                if (raw.enabled == 0 || raw.intensity <= 0) continue;
                var light = new GameObject(raw.path, typeof(Light)).GetComponent<Light>();
                light.transform.SetParent(transform, false);
                Apply(light.transform, raw);
                light.type = (LightType)raw.type;
                light.color = raw.color;
                light.intensity = raw.intensity;
                light.cullingMask = unchecked((int)raw.cullingMask);
                light.range = raw.range;
                light.spotAngle = raw.spotAngle;
            }
            Debug.Log("[OriginalStage] " + layoutId + " + " + envId);
        }

        static T Read<T>(string name)
        {
            var asset = Resources.Load<TextAsset>("BattlePresentation/" + name);
            if (asset == null) throw new InvalidOperationException("Missing original stage data " + name);
            return JsonUtility.FromJson<T>(asset.text);
        }
        static void Apply(Transform node, Pose pose)
        {
            node.localPosition = pose.position;
            node.localRotation = pose.rotation;
            node.localScale = pose.scale;
        }
        public Pose Slot(int side, int index, bool capital = false)
        {
            var group = OriginalLayout.groups.First(g => g.path.EndsWith(side == 0 ? "/PlayerMarkers" : "/EnemyMarkers"));
            var cap = (group.restricted ?? Array.Empty<Restriction>()).FirstOrDefault(r => r.tag == "role_capital");
            if (capital) return cap?.marker ?? throw new InvalidOperationException("No original capital marker");
            // _unitMarkers refers to components; restricted markers refer to
            // transforms. Compare resolved transform identity, not pointer kind.
            var normal = group.slots.Where(s => cap == null || s.transformPathId != cap.marker.transformPathId).ToArray();
            if (index < 0 || index >= normal.Length) throw new InvalidOperationException("No original slot " + index);
            return normal[index];
        }
        public void Place(GameObject model, int side, int index, bool capital = false)
        {
            model.transform.SetParent(LayoutRoot, false);
            Apply(model.transform, Slot(side, index, capital));
        }
    }

    // An explicitly labelled asset/layout preview, never a replacement fleet simulator.
    public class FleetPresentationPreview : MonoBehaviour
    {
        [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.AfterSceneLoad)]
        static void Boot()
        {
            if (!System.Environment.GetCommandLineArgs().Contains("--fleet-preview")) return;
            foreach (var root in SceneManager.GetActiveScene().GetRootGameObjects()) root.SetActive(false);
            new GameObject("FleetPresentationPreview").AddComponent<FleetPresentationPreview>();
        }
        public BattleStage Stage { get; private set; }
        void Start()
        {
            Stage = gameObject.AddComponent<BattleStage>();
            Stage.Assemble(true);
            for (int side = 0; side < 2; side++)
            {
                Spawn("ship_chimaera_pre", side, 0, true);
                for (int slot = 0; slot < 3; slot++)
                    Spawn(side == 0 ? "ship_xwing_pre" : "ship_tiefighter_pre", side, slot, false);
            }
            var hud = new BattleHUD();
            hud.SetTurnText("舰队原始场景预览 · 战斗与增援待恢复（PENDING-ORIGINAL）");
            hud.SetCamera(Camera.main);
        }
        void Spawn(string id, int side, int slot, bool capital)
        {
            var prefab = Resources.Load<GameObject>("FleetModels/" + id);
            if (prefab == null) throw new InvalidOperationException("Missing original fleet model " + id);
            var model = Instantiate(prefab);
            model.name = id + "#" + side + "_" + slot;
            Stage.Place(model, side, slot, capital);
        }
    }
}
