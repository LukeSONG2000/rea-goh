using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Security.Cryptography;
using System.Text;
using System.Text.Json;
using SwgohRestored.Battle;

// Real exported data, controlled combat stats. This tests graph interpretation,
// not player-account progression or equivalence of the entire original client.
public static class OriginalEffectRegression
{
    static readonly JsonSerializerOptions Json = new() { IncludeFields = true };
    static OriginalEffectGraph Graph;
    static Dictionary<string, OriginalUnitMenu> Menus;
    static void Check(bool condition, string message)
    { if (!condition) throw new Exception(message); }
    static T Load<T>(string path) => JsonSerializer.Deserialize<T>(File.ReadAllText(path), Json);

    static void LoadGraph(string directory)
    {
        var manifest = Load<RawEffectManifest>(Path.Combine(directory, "battle_effects.json"));
        Check(manifest.schemaVersion == 2, "real schema 2 manifest required");
        var effects = manifest.effectFiles.SelectMany(file =>
            Load<RawEffectShard>(Path.Combine(directory, file)).effects).ToArray();
        Graph = new OriginalEffectGraph(effects, manifest.abilities);
        Menus = Load<OriginalUnitMenu[]>(Path.Combine(directory, "abilities.json")).ToDictionary(u => u.def_id);
        Check(effects.Length == 63035 && manifest.abilities.Length == 6732, "full original graph loaded");
        Check(Graph.Effects["basicability_vader_block_jedi_u03"].chance == 100, "guaranteed original chance");
        Check(Graph.Effects["basicability_vader_block_u03"].chance == 80, "ordinary original chance");
    }

    static BattleUnit Unit(string hero, string id, int side, int speed)
    {
        var unit = new BattleUnit { BaseId = hero, UnitId = id, Side = side,
            CurrentHp = 100000, MaxHp = 100000,
            Stats = new Dictionary<int, double> { [5] = speed, [6] = 1000,
                [7] = 1000, [28] = -1, [29] = -1, [26] = -1, [27] = -1 } };
        unit.Categories.UnionWith(Menus[hero].categoryId);
        unit.Abilities.AddRange(Menus[hero].abilities.Select(Graph.Bind));
        return unit;
    }
    static int Basic(BattleUnit unit) => unit.Abilities.FindIndex(a => a.Kind == "basic");
    static void Submit(BattleSim sim, int index, BattleUnit target)
    {
        string reason = sim.OriginalUnsupportedReason(sim.PendingActor.Abilities[index]);
        Check(reason == null, "real graph rejected: " + reason);
        Check(sim.TrySubmitCommand(index, target, out var error), "submission rejected: " + error);
    }
    static string Snapshot(BattleSim sim)
    {
        var context = typeof(BattleSim).GetField("_ctx", BindingFlags.Instance | BindingFlags.NonPublic).GetValue(sim);
        var seed = typeof(JavaRandom).GetField("_seed", BindingFlags.Instance | BindingFlags.NonPublic);
        string rng = string.Join(",", typeof(BattleContext).GetFields().Select(f => seed.GetValue(f.GetValue(context))));
        string units = string.Join(";", sim.Units.Select(u => $"{u.UnitId}:{u.CurrentHp:R}:{u.CurrentShield:R}:{u.SpeedFill:R}:{u.SpeedFillOverflow:R}:" +
            string.Join(",", u.CooldownLeft.OrderBy(p => p.Key).Select(p => p.Key + "=" + p.Value)) + ":" +
            string.Join(",", u.PersistentEffects.Select(e => e.Definition.id + ":" + string.Join("/", e.Expiration.Select(c => $"{c.MatchedCount}:{c.SawOwnTurnStart}"))))));
        return sim.Round + "|" + sim.PendingActor?.UnitId + "|" + rng + "|" + units + "|" + JsonSerializer.Serialize(sim.Log, Json);
    }

    static void VaderCategories()
    {
        foreach (var categories in new[] { new[] { "profession_jedi" }, new[] { "affiliation_rebels" },
            new[] { "profession_jedi", "affiliation_rebels" }, Array.Empty<string>() })
        {
            var vader = Unit("VADER", "vader", 0, 300);
            var target = Unit("SNOWTROOPER", "target", 1, 200);
            target.Categories.Clear(); target.Categories.UnionWith(categories);
            var sim = new BattleSim(new[] { vader }, new[] { target }, 4490);
            Check(sim.PrepareNextTurn() == vader, "Vader acts first");
            Submit(sim, Basic(vader), target);
            var damage = sim.Log.Where(e => e.Kind == "damage" || e.Kind == "crit").ToArray();
            Check(damage.Length == 1, "mutually exclusive branches must yield exactly one damage event");
            string expected = categories.Length == 0 ? "basicability_vader_damage_u05" : "basicability_vader_damage_jedi_u05";
            Check(damage[0].EffectId == expected, "category branch chosen from original rule");
            Check(damage[0].Amount >= 3079 && damage[0].Amount <= 3403,
                "1000 attack * 3.241 +/-5%, not doubled or level multiplied: " + damage[0].Amount);
            if (categories.Length != 0)
                Check(target.PersistentEffects.Count == 1, "guaranteed Jedi/rebel block applies");
        }
    }

    static void IndependentStatusExpiry()
    {
        var first = Unit("VADER", "first", 0, 300);
        var second = Unit("VADER", "second", 0, 250);
        var targetA = Unit("SNOWTROOPER", "A", 1, 200);
        var targetB = Unit("SNOWTROOPER", "B", 1, 150);
        targetA.Categories.Add("profession_jedi"); targetB.Categories.Add("profession_jedi");
        var sim = new BattleSim(new[] { first, second }, new[] { targetA, targetB }, 4490);
        Check(sim.PrepareNextTurn() == first, "first caster turn"); Submit(sim, Basic(first), targetA);
        Check(sim.PrepareNextTurn() == second, "second caster turn"); Submit(sim, Basic(second), targetB);
        Check(targetA.PersistentEffects.Count == 1 && targetB.PersistentEffects.Count == 1, "two statuses retained");
        Check(!ReferenceEquals(targetA.PersistentEffects[0], targetB.PersistentEffects[0]), "status instances independent");
        Check(!ReferenceEquals(targetA.PersistentEffects[0].Expiration[0], targetB.PersistentEffects[0].Expiration[0]), "condition counters independent");
        Check(sim.PrepareNextTurn() == targetA, "target A own full turn begins");
        Check(targetA.PersistentEffects.Count == 1, "status must survive target turn start");
        Submit(sim, Basic(targetA), first);
        Check(targetA.PersistentEffects.Count == 0, "status expires after target full turn");
        Check(targetB.PersistentEffects.Count == 1, "another target full turn does not expire status B");
        Check(sim.Log.Any(e => e.Kind == "status_remove" && e.TargetUnitId == "A"), "expiry event emitted");
    }

    static void UnsupportedIsAtomic()
    {
        // Vader's four active graphs are now supported. Retain a real graph
        // rejection fixture from Traya instead of assuming Vader stays pending.
        var vader = Unit("DARTHTRAYA", "unsupported-actor", 0, 300);
        var target = Unit("SNOWTROOPER", "target", 1, 200);
        var sim = new BattleSim(new[] { vader }, new[] { target }, 4490);
        sim.PrepareNextTurn();
        int unsupported = vader.Abilities.FindIndex(a => sim.OriginalUnsupportedReason(a) != null);
        Check(unsupported >= 0, "real unsupported graph fixture exists");
        string before = Snapshot(sim);
        for (int i = 0; i < 5; i++)
            Check(!sim.TrySubmitCommand(unsupported, target, out _), "unsupported skill rejected");
        Check(Snapshot(sim) == before, "unsupported graph must leave all state and five RNG streams unchanged");
    }

    static void Condition(BattleSim sim, BattleUnit target, int eventType)
    {
        typeof(BattleSim).GetMethod("OriginalConditionEvent", BindingFlags.NonPublic | BindingFlags.Instance)
            .Invoke(sim, new object[] { target, eventType });
    }

    static void RepeatedStatusesAndConditionBoundaries()
    {
        var first = Unit("VADER", "first", 0, 300);
        var second = Unit("VADER", "second", 0, 250);
        var target = Unit("SNOWTROOPER", "target", 1, 200);
        target.Categories.Add("profession_jedi");
        var sim = new BattleSim(new[] { first, second }, new[] { target }, 4490);
        sim.PrepareNextTurn(); Submit(sim, Basic(first), target);
        sim.PrepareNextTurn(); Submit(sim, Basic(second), target);
        Check(target.PersistentEffects.Count == 2, "repeated same-target applications coexist instead of replacing");
        Check(!ReferenceEquals(target.PersistentEffects[0], target.PersistentEffects[1]), "same-target instances independent");
        Check(target.PersistentEffects.Select(e => e.Source.UnitId).SequenceEqual(new[] { "first", "second" }),
            "repeated applications retain their original sources");
        Condition(sim, target, 32); // end without a start since application
        Check(target.PersistentEffects.Count == 2, "partial current turn must not count as a full turn");
        Condition(sim, target, 21); // original death expiration is OR with full turn
        Check(target.PersistentEffects.Count == 0, "death alone satisfies an alternative original expiry condition");

        // Synthetic threshold fixture: use the original raw condition shape,
        // changing only its count to exercise >1 without claiming a catalog row.
        var thresholdState = new OriginalPersistentEffect {
            Definition = Graph.Effects["basicability_vader_block_jedi_u03"], Source = first };
        thresholdState.Expiration.Add(new OriginalConditionState {
            Definition = new RawCondition { conditionType = 49, conditionValue = "2" } });
        target.PersistentEffects.Add(thresholdState);
        Condition(sim, target, -1); Condition(sim, target, 32);
        Check(target.PersistentEffects.Count == 1, "threshold two survives first completed full turn");
        Condition(sim, target, 32);
        Check(target.PersistentEffects.Count == 1, "duplicate end without another start is not another full turn");
        Condition(sim, target, -1); Condition(sim, target, 32);
        Check(target.PersistentEffects.Count == 0, "threshold two expires after second completed full turn");
    }

    static void CurseBlocksOnlySpecialGate()
    {
        var vader = Unit("VADER", "vader", 0, 300);
        var target = Unit("SNOWTROOPER", "target", 1, 200);
        target.Categories.Add("profession_jedi");
        // Capability-only fixture: bind the real supported basic graph again,
        // classify this copy as special so unsupported real special graphs do
        // not confound the curse gate. This is not an original Snowtrooper skill.
        var special = Graph.Bind(Menus["SNOWTROOPER"].abilities.Single(a => a.kind == "basic"));
        special.Kind = "special";
        target.Abilities.Add(special);
        int specialIndex = target.Abilities.Count - 1;
        var sim = new BattleSim(new[] { vader }, new[] { target }, 4490);
        sim.PrepareNextTurn(); Submit(sim, Basic(vader), target);
        Check(sim.PrepareNextTurn() == target && target.PersistentEffects.Count == 1, "cursed target turn prepared");
        Check(sim.IsAbilityReady(Basic(target)), "curse keeps basic selectable");
        Check(!sim.IsAbilityReady(specialIndex), "curse blocks otherwise executable special");
        string before = Snapshot(sim);
        Check(!sim.TrySubmitCommand(specialIndex, vader, out _), "cursed special submission rejected");
        Check(Snapshot(sim) == before, "cursed special rejection leaves state and RNG unchanged");
        Condition(sim, target, 24);
        Check(sim.IsAbilityReady(specialIndex), "special becomes selectable when original status expires");
    }

    static void OriginalGraphSeedDoubleRun()
    {
        string Run()
        {
            var vader = Unit("VADER", "vader", 0, 300);
            var snow = Unit("SNOWTROOPER", "snow", 1, 200);
            snow.Categories.Add("profession_jedi");
            var sim = new BattleSim(new[] { vader }, new[] { snow }, 4490);
            for (int turn = 0; turn < 20; turn++)
            {
                var actor = sim.PrepareNextTurn();
                Check(actor != null, "double-run fixture must reach 20 complete turns");
                Submit(sim, Basic(actor), actor == vader ? snow : vader);
            }
            Check(sim.Round == 20, "double-run commits twenty turns");
            return Snapshot(sim);
        }
        string first = Run(), second = Run();
        Check(first == second, "real graph same-seed double-run differs in state/RNG/log/status");
        string digest = Convert.ToHexString(SHA256.HashData(Encoding.UTF8.GetBytes(first))).ToLowerInvariant();
        Console.WriteLine("PASS: original graph seed 4490, 20-turn double-run snapshot SHA256 " + digest);
    }

    public static void Main(string[] args)
    {
        LoadGraph(args.Length > 0 ? args[0] : Path.Combine(Directory.GetCurrentDirectory(),
            "unity/SWGoH-Restored/Assets/StreamingAssets"));
        VaderCategories(); IndependentStatusExpiry(); UnsupportedIsAtomic();
        RepeatedStatusesAndConditionBoundaries(); CurseBlocksOnlySpecialGate();
        OriginalGraphSeedDoubleRun();
        Console.WriteLine("PASS: real schema 2 graph, four Vader category branches, single damage, original chances, independent/repeated statuses, full-turn/partial-turn/death/count-two expiry, basic/special curse gate, Snowtrooper same interpreter, unsupported command atomicity including RNG.");
    }
}
