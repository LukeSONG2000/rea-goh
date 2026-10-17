using UnityEngine;
namespace SwgohRestored.EditorTools
{
    public static class OriginalAbilityImportGate
    {
        public static void Run()
        {
            OriginalAbilityAnimatorImporter.ImportVader();
            OriginalAbilityParticleImporter.Import();
            Debug.Log("[OriginalAbilityImportGate] imported; inspect field readback for fidelity status");
        }
    }
}
