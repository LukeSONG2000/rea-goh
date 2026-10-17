using UnityEngine;

namespace SwgohRestored.Battle
{
    // Stable original serialized identities; never infer attachments from display names.
    public sealed class OriginalAbilityObjectIds : MonoBehaviour
    {
        public string[] ids;
        public Transform[] objects;
        public Transform Resolve(string id)
        {
            if (ids == null || objects == null) return null;
            for (int i = 0; i < ids.Length && i < objects.Length; i++)
                if (ids[i] == id) return objects[i];
            return null;
        }
    }
}
