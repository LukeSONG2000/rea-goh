using System;
using UnityEngine;

namespace SwgohRestored.Battle
{
    /// <summary>Original open CatmullRom DOPath position sampler. Coordinates must
    /// already share the caller's chosen local/world space. No orientation is invented.</summary>
    public sealed class OriginalAbilityPath
    {
        readonly Vector3[] points;
        readonly float[] times, lengths;
        public float Length { get; }
        public int PointCount => points.Length;
        public int Subdivisions => times.Length;

        public OriginalAbilityPath(Vector3 start, Vector3[] waypoints)
        {
            if (waypoints == null || waypoints.Length == 0)
                throw new ArgumentException("Original DOPath requires at least one waypoint.", nameof(waypoints));
            CheckFinite(start);
            foreach (var point in waypoints) CheckFinite(point);
            // PathPlugin.SetChangeValue 18249BF10 uses per-axis Approximately.
            // CatmullRom.minInputWaypoints 180C05600 is exactly 1 (mov eax,1;ret).
            bool prepend = waypoints.Length <= 1 || !Same(start, waypoints[0]);
            points = new Vector3[waypoints.Length + (prepend ? 1 : 0)];
            if (prepend) points[0] = start;
            Array.Copy(waypoints, 0, points, prepend ? 1 : 0, waypoints.Length);
            // DOPathAction 180D37590 supplies literal resolution 10.
            int subdivisions = points.Length * 10;
            times = new float[subdivisions]; lengths = new float[subdivisions];
            Vector3 previous = RawPoint(0);
            float total = 0, step = 1f / subdivisions;
            for (int i = 1; i <= subdivisions; i++)
            {
                float time = i * step;
                Vector3 current = RawPoint(time);
                total += Vector3.Distance(previous, current);
                times[i - 1] = time; lengths[i - 1] = total;
                previous = current;
            }
            Length = total;
        }

        /// <summary>Convenience entry; cache an instance when sampling each frame.</summary>
        public static Vector3 Evaluate(Vector3 start, Vector3[] waypoints, float normalizedTime, string easeOverride, int easeType)
            => new OriginalAbilityPath(start, waypoints).Evaluate(normalizedTime, easeOverride, easeType);

        public Vector3 Evaluate(float normalizedTime, string easeOverride, int easeType)
        {
            if (float.IsNaN(normalizedTime) || float.IsInfinity(normalizedTime))
                throw new ArgumentOutOfRangeException(nameof(normalizedTime));
            float eased = Ease(Mathf.Clamp01(normalizedTime), easeOverride, easeType);
            return RawPoint(ConstantPathPercentage(eased));
        }

        public static float Ease(float time, string easeOverride, int easeType)
        {
            // DOTweenAction 180D392B0 accepts a recognized enum name. Unknown
            // strings retain EaseType; known but unrecovered eases must not become linear.
            if (!string.IsNullOrEmpty(easeOverride))
            {
                int index = Array.IndexOf(EaseNames, easeOverride);
                if (index >= 0) easeType = index;
            }
            switch (easeType)
            {
                case 1: return time; // DG.Tweening.Ease.Linear
                // EaseManager.Evaluate 1824B7BC0, switch case 0x14.
                case 20: return 1f - Mathf.Sqrt(1f - time * time);
                case 21: float shifted = time - 1f; return Mathf.Sqrt(1f - shifted * shifted);
                default: throw new NotSupportedException("PENDING-ORIGINAL path easing " + easeType);
            }
        }

        // Path.ConvertToConstantPathPerc 1824AB4B0, length tables 1824AA360.
        public float ConstantPathPercentage(float percentage)
        {
            if (percentage <= 0 || percentage >= 1) return Mathf.Clamp01(percentage);
            if (Length <= 0) return percentage;
            float distance = percentage * Length, previousTime = 0, previousLength = 0;
            for (int i = 0; i < lengths.Length; i++)
            {
                if (lengths[i] > distance)
                    return Mathf.Clamp01(previousTime + (distance - previousLength) /
                        (lengths[i] - previousLength) * (times[i] - previousTime));
                previousTime = times[i]; previousLength = lengths[i];
            }
            return 1f;
        }

        // CatmullRomDecoder.GetPoint 1824A9F00 / FinalizePath 1824A9BE0.
        public Vector3 RawPoint(float percentage)
        {
            float scaled = Mathf.Clamp01(percentage) * (points.Length - 1);
            int segment = Math.Min(Mathf.FloorToInt(scaled), points.Length - 2);
            float t = scaled - segment, t2 = t * t, t3 = t2 * t;
            Vector3 a = segment == 0 ? points[1] : points[segment - 1];
            Vector3 b = points[segment], c = points[segment + 1];
            Vector3 d = segment + 2 < points.Length ? points[segment + 2]
                : 2f * points[points.Length - 1] - points[points.Length - 2];
            return .5f * (2f * b + (-a + c) * t + (2f * a - 5f * b + 4f * c - d) * t2
                + (-a + 3f * b - 3f * c + d) * t3);
        }

        static bool Same(Vector3 a, Vector3 b) => Mathf.Approximately(a.x,b.x) && Mathf.Approximately(a.y,b.y) && Mathf.Approximately(a.z,b.z);
        static void CheckFinite(Vector3 p)
        {
            if (float.IsNaN(p.x) || float.IsNaN(p.y) || float.IsNaN(p.z) || float.IsInfinity(p.x) || float.IsInfinity(p.y) || float.IsInfinity(p.z))
                throw new ArgumentException("Nonfinite original path waypoint.");
        }
        static readonly string[] EaseNames = {
            "Unset", "Linear", "InSine", "OutSine", "InOutSine", "InQuad", "OutQuad", "InOutQuad",
            "InCubic", "OutCubic", "InOutCubic", "InQuart", "OutQuart", "InOutQuart", "InQuint", "OutQuint", "InOutQuint",
            "InExpo", "OutExpo", "InOutExpo", "InCirc", "OutCirc", "InOutCirc", "InElastic", "OutElastic", "InOutElastic",
            "InBack", "OutBack", "InOutBack", "InBounce", "OutBounce", "InOutBounce", "Flash", "InFlash", "OutFlash", "InOutFlash",
            "INTERNAL_Zero", "INTERNAL_Custom"
        };
    }
}
