#if UNITY_EDITOR
using UnityEngine;

namespace SwgohRestored.EditorTools
{
    public static class OriginalAbilityShaderBridge
    {
        // No catch-all material replacement: these custom programs are restored
        // from their original DXBC and retain exact original property names.
        public static Shader Resolve(string originalName)
        {
            switch (originalName)
            {
                case "Custom/VFX/Depth Sort Additive":
                case "Custom/VFX/DistortUV Alpha Blended (Probes)":
                case "Legacy Shaders/Particles/Additive":
                case "Legacy Shaders/Particles/Alpha Blended":
                    return Shader.Find(originalName);
                default: return null;
            }
        }
        public static string EvidenceStatus(string originalName)
        {
            return originalName != null && originalName.StartsWith("Custom/VFX/")
                ? "ORIGINAL_DXBC_PROGRAM_AND_PASS_STATES_RECOVERED"
                : "ENGINE_LEGACY_SHADER_REUSED_PROGRAM_VERSION_UNVERIFIED";
        }
    }
}
#endif
