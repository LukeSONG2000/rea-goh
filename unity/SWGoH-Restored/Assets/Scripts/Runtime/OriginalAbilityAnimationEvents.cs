using UnityEngine;
using System.Linq;
namespace SwgohRestored.Battle
{
    public sealed class OriginalAbilityAnimationEvents : MonoBehaviour
    {
        public OriginalAbilityPresentation Active;
        public void FireAbilityFSMEvent(string value) { if (Active) Active.FireAbilityFSMEvent(value); }
        public void PostAudioEvent(string value) { if (Active) Active.PostAudioEvent(value); }
        public void StartParticleSystem(string value)
        {
            var particle = GetComponentsInChildren<ParticleSystem>(true).FirstOrDefault(p => p.gameObject.name == value);
            if (particle) { particle.Simulate(0, true, true, true); particle.Play(true); }
            else if (Active) Active.Trace.Add("Original particle name has no match (no-op): " + value);
        }
        public void PlaySubObjectAnimationEvent(string value)
        {
            GetComponentInParent<OriginalCharacterPresentation>()?.Dispatch(value);
        }
    }
}
