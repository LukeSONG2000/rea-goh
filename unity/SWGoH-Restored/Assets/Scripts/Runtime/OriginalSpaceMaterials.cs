using System;
using System.Linq;
using UnityEngine;

namespace SwgohRestored.Battle
{
    // Recover material state lost by the old opaque-PBR export. This adapter does
    // not claim to recreate the custom planet cloud/mask or atmosphere programs.
    public static class OriginalSpaceMaterials
    {
        [Serializable] class Config { public OriginalMaterial[] materials; public Binding[] bindings; }
        [Serializable] class OriginalMaterial { public string name, shader; public TextureRef[] textures; public Tint[] colors; public Pass[] passes; }
        [Serializable] class TextureRef { public string property, resourcePath; public Vector2 scale, offset; }
        [Serializable] class Tint { public string name; public Color value; }
        [Serializable] class Pass { public float srcBlend, dstBlend, srcBlendAlpha, dstBlendAlpha, zWrite, zTest, cull, colorMask; }
        [Serializable] class Binding { public string renderer, path; public int layer; public bool enabled; public Slot[] slots; }
        [Serializable] class Slot { public int slot; public string material; }

        public static void Apply(GameObject environment)
        {
            var data = Resources.Load<TextAsset>("SpaceMaterials/materials");
            if (data == null) throw new InvalidOperationException("Missing original space material state");
            var config = JsonUtility.FromJson<Config>(data.text);
            var shader = Resources.Load<Shader>("SpaceMaterials/OriginalStateUnlit");
            if (shader == null) throw new InvalidOperationException("Missing explicit material-state bridge");
            var materials = config.materials.ToDictionary(m => m.name, m =>
            {
                var result = new Material(shader) { name = m.name + " [PENDING-ORIGINAL shader program]" };
                var main = m.textures.FirstOrDefault(t => t.property == "_MainTex");
                if (main != null)
                {
                    result.mainTexture = Resources.Load<Texture2D>(main.resourcePath);
                    result.mainTextureScale = main.scale;
                    result.mainTextureOffset = main.offset;
                }
                var tint = m.colors.FirstOrDefault(c => c.name == "_TintColor") ?? m.colors.FirstOrDefault(c => c.name == "_Color");
                result.color = tint?.value ?? Color.white;
                var state = m.passes[0];
                result.SetInt("_SrcBlend", (int)state.srcBlend); result.SetInt("_DstBlend", (int)state.dstBlend);
                result.SetInt("_SrcBlendAlpha", (int)state.srcBlendAlpha); result.SetInt("_DstBlendAlpha", (int)state.dstBlendAlpha);
                result.SetInt("_ZWrite", (int)state.zWrite); result.SetInt("_ZTest", (int)state.zTest);
                result.SetInt("_Cull", (int)state.cull); result.SetInt("_ColorMask", (int)state.colorMask);
                result.renderQueue = state.zWrite == 0 ? 3000 : 2000;
                return result;
            });
            int applied = 0;
            foreach (var renderer in environment.GetComponentsInChildren<Renderer>(true))
            {
                var binding = config.bindings.FirstOrDefault(b => b.renderer == renderer.gameObject.name);
                // glTFast uniquifies duplicate node names, but source material
                // names are unique in this bundle and preserve slot identity.
                if (binding == null)
                    binding = config.bindings.FirstOrDefault(b => b.slots.Any(s =>
                        renderer.sharedMaterials.Any(m => m != null && m.name == s.material)));
                if (binding == null) continue;
                renderer.gameObject.layer = binding.layer;
                renderer.enabled = binding.enabled;
                var slots = renderer.sharedMaterials;
                foreach (var slot in binding.slots) if (slot.slot < slots.Length) slots[slot.slot] = materials[slot.material];
                renderer.sharedMaterials = slots;
                applied++;
            }
            Debug.Log("[OriginalStage] space material states applied=" + applied + "; PENDING-ORIGINAL custom shader program");
        }
    }
}
