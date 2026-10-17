using System;
using System.Collections.Generic;
using System.Linq;
using Newtonsoft.Json.Linq;
using UnityEngine;

namespace SwgohRestored.Battle
{
    // Same original object/renderer state manifest is consumed by Unity and Web.
    public sealed class OriginalCharacterPresentation : MonoBehaviour
    {
        JObject data;
        readonly Dictionary<string, Transform> objects = new();
        public readonly List<string> Pending = new();
        public void Initialize(string bundle, JObject abilities)
        {
            var asset = Resources.Load<TextAsset>("CharacterPresentation/" + bundle);
            if (!asset) { Pending.Add("Original object state manifest " + bundle); return; }
            data = JObject.Parse(asset.text);
            if (abilities?["characterOverlays"] is JArray overlays)
                foreach (var overlay in overlays)
                {
                    var parent = FindPath(transform, (string)overlay["attachParentHierarchyPath"]);
                    var prefab = Resources.Load<GameObject>("AbilityPresentation/" + (string)overlay["id"]);
                    if (!parent || !prefab) { Pending.Add("Original character VFX " + (string)overlay["id"]); continue; }
                    // The geometry transport retains original empty VFX transforms.
                    // Replace only that exact renderer-free subtree, preserving its
                    // original path for Animator bindings and serialized references.
                    var stub = FindPath(transform, (string)overlay["subtreeHierarchyPath"]);
                    if (stub)
                    {
                        if (stub.GetComponentsInChildren<Renderer>(true).Length != 0)
                            throw new InvalidOperationException("Original VFX subtree already has renderers: " + stub.name);
                        stub.gameObject.SetActive(false);
                        stub.name += "__replaced_geometry_stub";
                        Destroy(stub.gameObject);
                    }
                    var instance = Instantiate(prefab, parent, false);
                    instance.name = ((string)overlay["subtreeHierarchyPath"]).Split('/').Last();
                }
            foreach (var obj in data["gameObjects"])
            {
                var node = FindPath(transform, (string)obj["hierarchyPath"]);
                if (!node) continue;
                objects[(string)obj["pathId"]] = node;
                objects[(string)obj["transformPathId"]] = node;
                // Root visibility is controlled by the battle scene, not by the
                // staging container's inactive state.
                if (node != transform) node.gameObject.SetActive((bool)obj["activeSelf"]);
            }
            foreach (var renderer in data["renderers"])
            {
                var node = FindPath(transform, (string)renderer["hierarchyPath"]);
                if (!node) continue;
                foreach (var r in node.GetComponents<Renderer>()) r.enabled = (bool)renderer["raw"]["m_Enabled"];
            }
            ApplyContext("battle");
        }
        public void ApplyContext(string context)
        {
            if (data == null) return;
            foreach (var component in data["components"].Where(c => (string)c["class"] == "UnitComponent"))
            {
                var props = component["raw"]["_propStates"] as JArray;
                if (props == null) continue;
                for (int i = 0; i < props.Count; i++)
                {
                    var binding = component["referenceBindings"].FirstOrDefault(b => (string)b["fieldPath"] == $"_propStates[{i}].prop");
                    var node = binding == null ? null : FindPath(transform, (string)binding["hierarchyPath"]);
                    if (node) node.gameObject.SetActive(Convert.ToBoolean(((JValue)props[i][context]).Value));
                }
            }
        }
        public void Dispatch(string id)
        {
            if (data == null) return;
            foreach (var component in data["components"].Where(c => (string)c["class"] == "UnitSubObjectAnimationEventHandler"))
            {
                var rows = component["raw"]["_animationEvents"];
                for (int i = 0; i < rows.Count(); i++)
                {
                    if ((string)rows[i]["Id"] != id) continue;
                    var actions = rows[i]["Actions"];
                    for (int n = 0; n < actions.Count(); n++)
                    {
                        var binding = component["referenceBindings"].FirstOrDefault(b => (string)b["fieldPath"] == $"_animationEvents[{i}].Actions[{n}].UnitSubObject");
                        var node = binding == null ? null : FindPath(transform, (string)binding["hierarchyPath"]);
                        if (!node) { Pending.Add("Missing original sub-object " + id); continue; }
                        int kind = (int)actions[n]["Type"];
                        if (kind == 3 || kind == 4) node.gameObject.SetActive(kind == 3);
                        else if (kind == 1) node.GetComponent<Animator>()?.SetTrigger((string)actions[n]["AnimationValue"]);
                        else if (kind == 2) node.GetComponent<Animation>()?.Play((string)actions[n]["AnimationValue"], PlayMode.StopSameLayer);
                        else Pending.Add("Original sub-object action " + kind);
                    }
                    return;
                }
            }
        }
        public Transform ReferencePoint(int point)
        {
            string field = point switch { 0 => "_weapon", 1 => "_offhand", 3 => "_center", 4 => "_head", 5 => "_feet", 6 => "_uiArea", 7 => "_targetReticle", _ => null };
            if (field == null) throw new NotSupportedException("Original random hit-area selection");
            var unit = data?["components"].FirstOrDefault(c => (string)c["class"] == "UnitComponent");
            var binding = unit?["referenceBindings"].FirstOrDefault(b => (string)b["fieldPath"] == "_unitReferenceAreas." + field ||
                (string)b["fieldPath"] == "_unitReferenceAreas." + field + "[0]");
            return binding == null ? null : FindPath(transform, (string)binding["hierarchyPath"]);
        }
        public static Transform FindPath(Transform root, string path)
        {
            if (string.IsNullOrEmpty(path)) return null;
            var names = path.Split('/');
            if (names.Length == 1) return root;
            Transform match = null;
            foreach (Transform node in root.GetComponentsInChildren<Transform>(true))
            {
                if (node.name != names.Last()) continue;
                var candidate = node; int i = names.Length - 1;
                while (candidate != null && i > 0 && candidate.name == names[i]) { i--; candidate = candidate.parent; }
                if (i != 0 || (candidate != root &&
                    !(candidate && candidate.parent == root && candidate.name == names[0]))) continue;
                if (match) throw new InvalidOperationException("Ambiguous original hierarchy " + path);
                match = node;
            }
            return match;
        }
    }
}
