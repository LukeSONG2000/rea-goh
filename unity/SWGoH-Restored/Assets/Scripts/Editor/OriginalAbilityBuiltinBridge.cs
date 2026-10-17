#if UNITY_EDITOR
using System;
using UnityEditor;
using UnityEngine;

namespace SwgohRestored.EditorTools
{
    public static class OriginalAbilityBuiltinBridge
    {
        // Read directly from Unity 2022.3.62f1 Editor/Data/Resources/unity default resources:
        // Mesh pathID 10207 Sphere; 10209 Plane (Quad is 10210).
        // Validate the actual engine resource identity before returning anything.
        public static Mesh Resolve(string external, long originalPathId)
        {
            if (external != "Library/unity default resources") return null;
            string name = originalPathId == 10207 ? "Sphere.fbx" : originalPathId == 10209 ? "Plane.fbx" : null;
            if (name == null) return null;
            // Resource filenames are aliases and may resolve to older meshes
            // (the same builtin file also has pSphere1/10200 and pPlane1/10204).
            // Select by the verified serialized identity, never by alias alone.
            foreach (string alias in new[] { name, "New-" + name })
            {
                var candidate = Resources.GetBuiltinResource<Mesh>(alias);
                if (candidate && AssetDatabase.TryGetGUIDAndLocalFileIdentifier(candidate, out string candidateGuid, out long candidateId)
                    && candidateId == originalPathId) return candidate;
            }
            foreach (var obj in AssetDatabase.LoadAllAssetsAtPath(external))
                if (obj is Mesh candidate && AssetDatabase.TryGetGUIDAndLocalFileIdentifier(candidate, out string candidateGuid, out long candidateId)
                    && candidateId == originalPathId) return candidate;
            var mesh = Resources.GetBuiltinResource<Mesh>(name);
            string guid = null; long id = 0;
            bool found = mesh && AssetDatabase.TryGetGUIDAndLocalFileIdentifier(mesh, out guid, out id);
            if (!found || id != originalPathId)
                throw new InvalidOperationException("Original builtin mesh identity mismatch: " + external + ":" + originalPathId
                    + " resource=" + name + " found=" + found + " GUID=" + guid + " localID=" + id
                    + " mesh=" + (mesh ? mesh.name + " vertices=" + mesh.vertexCount + " assetPath=" + AssetDatabase.GetAssetPath(mesh) : "null"));
            return mesh;
        }

        public static void Validate()
        {
            foreach (long id in new long[] { 10207, 10209 })
            {
                var mesh = Resolve("Library/unity default resources", id);
                Debug.Log("Original builtin mesh verified pathID=" + id + " name=" + mesh.name + " vertices=" + mesh.vertexCount);
            }
        }

        public static void Diagnose()
        {
            foreach (string name in new[] { "Sphere.fbx", "Plane.fbx", "Quad.fbx", "New-Sphere.fbx", "New-Plane.fbx" })
            {
                var mesh = Resources.GetBuiltinResource<Mesh>(name);
                if (!mesh) { Debug.Log("BUILTIN_DIAG " + name + " null"); continue; }
                bool found = AssetDatabase.TryGetGUIDAndLocalFileIdentifier(mesh, out string guid, out long id);
                Debug.Log("BUILTIN_DIAG " + name + " name=" + mesh.name + " vertices=" + mesh.vertexCount
                    + " found=" + found + " GUID=" + guid + " localID=" + id + " path=" + AssetDatabase.GetAssetPath(mesh)
                    + " bounds=" + mesh.bounds);
            }
            foreach (var obj in AssetDatabase.LoadAllAssetsAtPath("Library/unity default resources"))
                if (obj is Mesh mesh)
                {
                    bool found = AssetDatabase.TryGetGUIDAndLocalFileIdentifier(mesh, out string guid, out long id);
                    Debug.Log("BUILTIN_ASSET_DIAG " + mesh.name + " vertices=" + mesh.vertexCount + " found=" + found + " GUID=" + guid + " localID=" + id);
                }
        }
    }
}
#endif
