using System;
using System.Collections.Generic;

namespace SwgohRestored.Battle
{
    // Native BattleEffectContext result slots are ordered by executed module,
    // including empty/failed modules, not by effect ID or target count.
    // GetContextValue 0x18136A570 sums target values in referenced slots;
    // AggregateContextValues 0x18136AB50 preserves their per-target sums.
    public sealed class OriginalEffectContext
    {
        readonly List<Dictionary<BattleUnit,float>> _results = new();
        public int Count => _results.Count;
        public void AddResult(Dictionary<BattleUnit,float> result)
        {
            if (result == null) throw new ArgumentNullException(nameof(result));
            _results.Add(new Dictionary<BattleUnit,float>(result));
        }
        public Dictionary<BattleUnit,float> Aggregate(int[] indices)
        {
            var sums = new Dictionary<BattleUnit,float>();
            // With no explicit indices, native obtains the previous result.
            // At activation start there is no previous module result.
            if (indices == null || indices.Length == 0)
            {
                if (_results.Count != 0)
                    foreach (var pair in _results[_results.Count-1]) sums[pair.Key]=pair.Value;
                return sums;
            }
            foreach (int index in indices)
            {
                // Native logs out-of-range references and continues. Validation
                // rejects those references before a command mutates the battle.
                if (index < 0 || index >= _results.Count) continue;
                foreach (var pair in _results[index])
                {
                    sums.TryGetValue(pair.Key,out float previous);
                    sums[pair.Key]=previous+pair.Value;
                }
            }
            return sums;
        }
        public float Sum(int[] indices)
        {
            float total=0;
            if(indices==null || indices.Length==0)
            {
                if(_results.Count!=0) foreach(var value in _results[_results.Count-1].Values) total+=value;
                return total;
            }
            foreach(int index in indices)
                if(index>=0 && index<_results.Count)
                    foreach(var value in _results[index].Values) total+=value;
            return total;
        }
    }
}
