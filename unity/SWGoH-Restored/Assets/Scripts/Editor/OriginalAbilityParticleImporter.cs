#if UNITY_EDITOR
using System;
using System.IO;
using System.Linq;
using System.Collections.Generic;
using Newtonsoft.Json.Linq;
using UnityEditor;
using UnityEngine;
using SwgohRestored.Battle;

namespace SwgohRestored.EditorTools
{
    // Imports serialized original modules, then reports every source leaf.
    // A successful import is not a claim of Unity 6000 -> 2022 compatibility.
    public static class OriginalAbilityParticleImporter
    {
        const string AssetsRoot = "Assets/Resources/AbilityPresentation";
        static readonly JArray Report = new JArray();
        static JObject Manifest;
        static readonly Dictionary<string, Material> Materials = new Dictionary<string, Material>();
        static readonly Dictionary<string, UnityEngine.Object> OriginalObjects = new Dictionary<string, UnityEngine.Object>();
        static string CurrentBundle;
        static string Repository => Path.GetFullPath(Path.Combine(Application.dataPath, "../../.."));
        public static void Import()
        {
            Report.Clear(); Materials.Clear();
            Manifest = JObject.Parse(File.ReadAllText(AssetsRoot + "/particle-resources.json"));
            var graph = JObject.Parse(File.ReadAllText(AssetsRoot + "/VADER.json"));
            foreach (JObject ability in graph["abilities"].Concat(graph["characterOverlays"] ?? new JArray()))
            {
                string id = (string)ability["id"], assetPath = AssetsRoot + "/" + id + ".prefab";
                File.Copy(Path.Combine(Repository, "logs/vader_complete/prefabs", id + ".prefab"), assetPath, true);
                AssetDatabase.ImportAsset(assetPath, ImportAssetOptions.ForceSynchronousImport | ImportAssetOptions.ForceUpdate);
                var root = PrefabUtility.LoadPrefabContents(assetPath);
                try
                {
                    var ids = root.GetComponent<OriginalAbilityObjectIds>();
                    if (!ids) throw new InvalidDataException("Original pathID mapping missing: " + id);
                    string bundle = (string)ability["source"]["bundle"];
                    CurrentBundle = bundle; OriginalObjects.Clear();
                    foreach (string group in new[] { "gameObjects", "transforms", "particles", "particleRenderers" })
                        foreach (JObject row in ability[group])
                        {
                            string type = (string)row["type"], pid = (string)row["pathId"];
                            Transform tr = ids.Resolve(type == "GameObject" || type == "Transform" ? pid : (string)row["raw"]["m_GameObject"]["m_PathID"]);
                            if (tr) OriginalObjects[pid] = type == "GameObject" ? (UnityEngine.Object)tr.gameObject : type == "Transform" ? tr : type == "ParticleSystem" ? (UnityEngine.Object)tr.GetComponent<ParticleSystem>() : tr.GetComponent<ParticleSystemRenderer>();
                        }
                    foreach (JObject row in ability["particleRenderers"])
                    {
                        var tr = ids.Resolve((string)row["raw"]["m_GameObject"]["m_PathID"]);
                        var renderer = tr ? tr.GetComponent<ParticleSystemRenderer>() : null;
                        if (!renderer) continue;
                        renderer.sharedMaterials = ((JArray)row["raw"]["m_Materials"]).Select(p => LoadMaterial(bundle, (string)row["pathId"], (JObject)p)).ToArray();
                        var serializedRenderer = new SerializedObject(renderer);
                        foreach (var field in ((JObject)row["raw"]).Properties())
                        {
                            if (!(field.Value is JObject pointer) || pointer["m_PathID"] == null) continue;
                            var edge = Edge(bundle, (string)row["pathId"], pointer);
                            var builtin = OriginalAbilityBuiltinBridge.Resolve((string)edge?["external"], (long)pointer["m_PathID"]);
                            if (!builtin) continue;
                            var property = serializedRenderer.FindProperty(field.Name);
                            if (property == null || property.propertyType != SerializedPropertyType.ObjectReference)
                                throw new InvalidDataException("Original builtin field absent: " + field.Name);
                            property.objectReferenceValue = builtin;
                        }
                        serializedRenderer.ApplyModifiedPropertiesWithoutUndo();
                    }
                    PrefabUtility.SaveAsPrefabAsset(root, assetPath);
                    foreach (string group in new[] { "gameObjects", "transforms", "particles", "particleRenderers" })
                        foreach (JObject row in ability[group])
                        {
                            string type = (string)row["type"], pid = (string)row["pathId"];
                            Transform tr = ids.Resolve(type == "GameObject" || type == "Transform" ? pid : (string)row["raw"]["m_GameObject"]["m_PathID"]);
                            UnityEngine.Object component = null;
                            if (tr) component = type == "GameObject" ? (UnityEngine.Object)tr.gameObject : type == "Transform" ? tr : type == "ParticleSystem" ? (UnityEngine.Object)tr.GetComponent<ParticleSystem>() : tr.GetComponent<ParticleSystemRenderer>();
                            if (!component) { Record(id, pid, "", "MISSING_COMPONENT", null, null); continue; }
                            var serialized = new SerializedObject(component);
                            Audit(id, pid, serialized, (JObject)row["raw"], "", ids);
                        }
                }
                finally { PrefabUtility.UnloadPrefabContents(root); }
            }
            AssetDatabase.SaveAssets();
            string path = Path.Combine(Repository, "logs/vader_complete/particle-import-readback.json");
            var output = new JObject { ["sourceUnityVersion"] = "6000.3.17f1", ["targetUnityVersion"] = Application.unityVersion,
                ["status"] = "PENDING-ORIGINAL until mismatches and missing shader/mesh dependencies are resolved", ["fields"] = Report,
                ["counts"] = JObject.FromObject(Report.OfType<JObject>().GroupBy(r => (string)r["status"]).ToDictionary(g => g.Key, g => g.Count())) };
            File.WriteAllText(path, output.ToString());
            Debug.Log("Original particle import readback: " + path + " " + output["counts"]);
        }

        static JObject Edge(string bundle, string owner, JObject pointer) => Manifest["references"].OfType<JObject>().FirstOrDefault(r =>
            (string)r["sourceBundle"] == bundle && (string)r["ownerPathId"] == owner && JToken.DeepEquals(r["pointer"], pointer));
        static Material LoadMaterial(string bundle, string owner, JObject pointer)
        {
            var edge = Edge(bundle, owner, pointer);
            string resource = (string)edge?["resource"];
            if (resource == null) return null;
            if (Materials.TryGetValue(resource, out var cached)) return cached;
            var data = JObject.Parse(File.ReadAllText(AssetsRoot + "/" + resource + ".json"));
            var raw = (JObject)data["raw"];
            var shaderEdge = Edge((string)data["source"]["bundle"], (string)data["pathId"], (JObject)raw["m_Shader"]);
            string shaderResource = (string)shaderEdge?["resource"];
            var shaderRaw = shaderResource == null ? null : JObject.Parse(File.ReadAllText(AssetsRoot + "/" + shaderResource + ".json"))["raw"];
            string shaderName = (string)shaderRaw?["m_ParsedForm"]?["m_Name"];
            var sourceProperties = new HashSet<string>((shaderRaw?["m_ParsedForm"]?["m_PropInfo"]?["m_Props"] ?? new JArray()).Select(p => (string)p["m_Name"]));
            string MissingPropertyStatus(string property) => sourceProperties.Contains(property) ? "PENDING_SHADER_PROPERTY" : "ORIGINAL_UNDECLARED_SAVED_PROPERTY";
            // Bridge accepts only original engine names or evidence-backed
            // recovered custom programs, with explicit per-program status.
            var shader = OriginalAbilityShaderBridge.Resolve(shaderName);
            if (!shader) { Record(bundle, owner, resource, "PENDING_SHADER", shaderName, null); Materials[resource] = null; return null; }
            var material = new Material(shader) { name = (string)raw["m_Name"], renderQueue = (int)raw["m_CustomRenderQueue"] };
            foreach (JArray pair in raw["m_SavedProperties"]["m_Floats"])
                if (material.HasProperty((string)pair[0])) material.SetFloat((string)pair[0], (float)pair[1]);
                else Record(bundle, owner, (string)pair[0], MissingPropertyStatus((string)pair[0]), pair[1], null);
            foreach (JArray pair in raw["m_SavedProperties"]["m_Colors"])
                if (material.HasProperty((string)pair[0])) material.SetColor((string)pair[0], new Color((float)pair[1]["r"], (float)pair[1]["g"], (float)pair[1]["b"], (float)pair[1]["a"]));
                else Record(bundle, owner, (string)pair[0], MissingPropertyStatus((string)pair[0]), pair[1], null);
            foreach (JArray pair in raw["m_SavedProperties"]["m_TexEnvs"])
            {
                string property = (string)pair[0]; var value = pair[1];
                if (!material.HasProperty(property)) { Record(bundle, owner, property, MissingPropertyStatus(property), value, null); continue; }
                var textureEdge = Edge((string)data["source"]["bundle"], (string)data["pathId"], (JObject)value["m_Texture"]);
                string textureResource = (string)textureEdge?["resource"];
                if (textureResource == null || !material.HasProperty(property)) continue;
                string texturePath = AssetsRoot + "/" + textureResource + ".png";
                var textureRaw = JObject.Parse(File.ReadAllText(AssetsRoot + "/" + textureResource + ".json"))["raw"];
                var importer = AssetImporter.GetAtPath(texturePath) as TextureImporter;
                if (importer)
                {
                    importer.textureType = TextureImporterType.Default;
                    importer.sRGBTexture = (int)textureRaw["m_ColorSpace"] == 1;
                    importer.alphaSource = TextureImporterAlphaSource.FromInput;
                    importer.textureCompression = TextureImporterCompression.Uncompressed;
                    importer.npotScale = TextureImporterNPOTScale.None;
                    importer.maxTextureSize = Mathf.NextPowerOfTwo(Mathf.Max((int)textureRaw["m_Width"], (int)textureRaw["m_Height"]));
                    importer.filterMode = (FilterMode)(int)textureRaw["m_TextureSettings"]["m_FilterMode"];
                    importer.wrapModeU = (TextureWrapMode)(int)textureRaw["m_TextureSettings"]["m_WrapU"];
                    importer.wrapModeV = (TextureWrapMode)(int)textureRaw["m_TextureSettings"]["m_WrapV"];
                    importer.wrapModeW = (TextureWrapMode)(int)textureRaw["m_TextureSettings"]["m_WrapW"];
                    importer.anisoLevel = (int)textureRaw["m_TextureSettings"]["m_Aniso"];
                    importer.mipMapBias = (float)textureRaw["m_TextureSettings"]["m_MipBias"];
                    importer.mipmapEnabled = (int)textureRaw["m_MipCount"] > 1;
                    importer.SaveAndReimport();
                }
                material.SetTexture(property, AssetDatabase.LoadAssetAtPath<Texture2D>(texturePath));
                material.SetTextureScale(property, new Vector2((float)value["m_Scale"]["x"], (float)value["m_Scale"]["y"]));
                material.SetTextureOffset(property, new Vector2((float)value["m_Offset"]["x"], (float)value["m_Offset"]["y"]));
            }
            string materialPath = AssetsRoot + "/" + resource + ".mat";
            var existing = AssetDatabase.LoadAssetAtPath<Material>(materialPath);
            if (existing) { EditorUtility.CopySerialized(material, existing); UnityEngine.Object.DestroyImmediate(material); material = existing; }
            else AssetDatabase.CreateAsset(material, materialPath);
            Record(bundle, owner, resource, OriginalAbilityShaderBridge.EvidenceStatus(shaderName), shaderName, shader.name);
            return Materials[resource] = material;
        }

        static void Record(string ability, string id, string path, string status, object expected, object actual)
        { Report.Add(new JObject { ["ability"] = ability, ["pathId"] = id, ["field"] = path, ["status"] = status,
            ["expected"] = expected == null ? JValue.CreateNull() : JToken.FromObject(expected), ["actual"] = actual == null ? JValue.CreateNull() : JToken.FromObject(actual) }); }

        static void Audit(string ability, string id, SerializedObject target, JToken value, string path, OriginalAbilityObjectIds ids)
        {
            if (path == "m_Tag")
            {
                var tag = target.FindProperty("m_TagString");
                // Builtin tag index zero is Untagged. Custom indices require
                // the original TagManager and must not be guessed.
                Record(ability, id, path, (int)value == 0 && tag?.stringValue == "Untagged" ? "MATCH_BINARY_TAG_TO_EDITOR_STRING" : "PENDING_TAG_MANAGER", value, tag?.stringValue);
                return;
            }
            if (path == "m_Name" && target.targetObject is GameObject root && root.GetComponent<OriginalAbilityObjectIds>())
            {
                Record(ability, id, path, root.name == (string)value ? "MATCH" : "EDITOR_PREFAB_FILENAME_NORMALIZATION", value, root.name);
                return;
            }
            if (path == "m_Component" && value is JArray components)
            {
                // The candidate intentionally contains particle/hierarchy
                // components only. Original audio/FSM scripts remain pending;
                // do not compare shifted component-array indices as identity.
                var go = (GameObject)target.targetObject;
                foreach (JObject entry in components)
                {
                    string componentId = (string)entry["component"]["m_PathID"];
                    OriginalObjects.TryGetValue(componentId, out var component);
                    bool found = component is Component c && c.gameObject == go;
                    Record(ability, id, path + ".originalPathId[" + componentId + "]", found ? "MATCH_REFERENCE" : "PENDING_ORIGINAL_COMPONENT", entry["component"], component ? component.GetType().Name : null);
                }
                return;
            }
            if (value is JObject obj)
            {
                if (obj["m_PathID"] != null)
                {
                    var prop = target.FindProperty(path); var actual = prop?.objectReferenceValue;
                    string pid = (string)obj["m_PathID"];
                    OriginalObjects.TryGetValue(pid, out var resolved);
                    if ((int)obj["m_FileID"] != 0) resolved = null;
                    var edge = Edge(CurrentBundle, id, obj);
                    if ((string)edge?["type"] == "Material" && Materials.TryGetValue((string)edge["resource"], out var material)) resolved = material;
                    if (!resolved) resolved = OriginalAbilityBuiltinBridge.Resolve((string)edge?["external"], (long)obj["m_PathID"]);
                    bool equal = pid == "0" ? actual == null : resolved && actual == resolved;
                    Record(ability, id, path, equal ? "MATCH_REFERENCE" : "PENDING_REFERENCE", obj, actual ? actual.name : null); return;
                }
                foreach (var entry in obj.Properties()) Audit(ability, id, target, entry.Value, path.Length == 0 ? entry.Name : path + "." + entry.Name, ids);
                return;
            }
            if (value is JArray array)
            {
                var prop = target.FindProperty(path);
                Record(ability, id, path + ".Array.size", prop != null && prop.isArray && prop.arraySize == array.Count ? "MATCH" : "MISMATCH_ARRAY", array.Count, prop != null && prop.isArray ? (object)prop.arraySize : null);
                for (int i = 0; i < array.Count; i++) Audit(ability, id, target, array[i], path + ".Array.data[" + i + "]", ids);
                return;
            }
            var field = target.FindProperty(path);
            if (field == null) { Record(ability, id, path, target.targetObject is ParticleSystemRenderer ? "PENDING_RENDERER_VERSION_FIELD" : "MISSING_SERIALIZED_FIELD", value, null); return; }
            object result;
            switch (field.propertyType)
            {
                case SerializedPropertyType.Integer: result = field.longValue; break;
                case SerializedPropertyType.Enum: result = field.intValue; break;
                case SerializedPropertyType.Boolean: result = field.boolValue; break;
                case SerializedPropertyType.Float: result = field.doubleValue; break;
                case SerializedPropertyType.String: result = field.stringValue; break;
                default: Record(ability, id, path, "PENDING_SERIALIZED_TYPE", value, field.propertyType.ToString()); return;
            }
            bool matches = JToken.DeepEquals(value, JToken.FromObject(result));
            if (value.Type == JTokenType.Float || value.Type == JTokenType.Integer)
                try { matches = Convert.ToDouble(result) == (double)value; } catch (Exception) { matches = false; }
            Record(ability, id, path, matches ? "MATCH" : "MISMATCH", value, result);
        }
    }
}
#endif
