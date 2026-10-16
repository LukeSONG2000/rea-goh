using System;
using System.Linq;
using System.Reflection;
using SwgohRestored.Battle;

// Standalone regression runner: compile with BattleSim.cs and JavaRandom.cs.
// This verifies command semantics and reproducibility, not original-client parity.
public static class BattleCommandRegression
{
    static void Check(bool value, string label)
    { if (!value) throw new Exception(label); }

    static BattleSim Fixture()
    {
        BattleUnit Unit(string id, int side, int speed) => new BattleUnit {
            UnitId = id, BaseId = side == 0 ? "actor" : "same-enemy", Side = side,
            CurrentHp = 10000, MaxHp = 10000,
            Stats = new System.Collections.Generic.Dictionary<int, double> {
                [Stat.Speed] = speed, [6] = 100, [7] = 50 }
        };
        var a = Unit("actor-0", 0, 200);
        a.Abilities.Add(new UnitAbility { Id = "basic", Kind = "basic", Chance = 100,
            DamageMultiplier = 1, DamageType = 2, DmgSide = 3, DmgSelect = 2 });
        a.Abilities.Add(new UnitAbility { Id = "special", Kind = "special", Chance = 100,
            DamageMultiplier = 3, DamageType = 2, DmgSide = 3, DmgSelect = 2, Cooldown = 3 });
        a.CooldownLeft["special"] = 3;
        return new BattleSim(new[] { a }, new[] { Unit("enemy-0", 1, 100), Unit("enemy-1", 1, 90) }, 42);
    }

    static string Snapshot(BattleSim sim)
    {
        var context = (BattleContext)typeof(BattleSim).GetField("_ctx", BindingFlags.Instance | BindingFlags.NonPublic).GetValue(sim);
        var seed = typeof(JavaRandom).GetField("_seed", BindingFlags.Instance | BindingFlags.NonPublic);
        string rng = string.Join(",", typeof(BattleContext).GetFields().Select(f => seed.GetValue(f.GetValue(context))));
        string units = string.Join(";", sim.Units.Select(u => $"{u.UnitId}:{u.CurrentHp:R}:{u.SpeedFill:R}:{u.SpeedFillOverflow:R}:" +
            string.Join(",", u.CooldownLeft.OrderBy(p => p.Key).Select(p => p.Key + "=" + p.Value))));
        string events = string.Join(";", sim.Log.Select(e => $"{e.Kind}:{e.ActorUnitId}:{e.TargetUnitId}:{e.Amount:R}:{e.AbilityId}"));
        return sim.Round + "|" + sim.PendingActor?.UnitId + "|" + rng + "|" + units + "|" + events;
    }

    public static void Main()
    {
        var sim = Fixture();
        Check(!sim.TrySubmitCommand(0, sim.Units[1], out _), "command needs prepared actor");
        var actor = sim.PrepareNextTurn();
        Check(actor.UnitId == "actor-0" && actor.CooldownLeft["special"] == 2, "prepare ticks once");
        string waiting = Snapshot(sim);
        for (int i = 0; i < 100; i++) {
            Check(sim.PrepareNextTurn() == actor, "same pending actor");
            sim.GetLegalTargets(0); sim.IsAbilityReady(1); sim.ProcessUnitsSpeed();
        }
        Check(Snapshot(sim) == waiting, "waiting/querying does not advance state or RNG");
        Check(!sim.TrySubmitCommand(1, sim.Units[1], out _), "cooldown rejected");
        Check(!sim.TrySubmitCommand(99, sim.Units[1], out _), "index rejected");
        Check(!sim.TrySubmitCommand(0, actor, out _), "wrong side rejected");
        Check(!sim.TrySubmitCommand(0, null, out _), "null rejected");
        Check(!sim.TrySubmitCommand(0, new BattleUnit { CurrentHp = 1, Side = 1 }, out _), "foreign target rejected");
        Check(Snapshot(sim) == waiting, "invalid commands mutate nothing including RNG");
        sim.Units[1].CurrentHp = 0;
        string deadState = Snapshot(sim);
        Check(!sim.TrySubmitCommand(0, sim.Units[1], out _), "dead target rejected");
        Check(Snapshot(sim) == deadState, "dead command mutates nothing");
        sim.Units[1].CurrentHp = 10000;
        Check(sim.TrySubmitCommand(0, sim.Units[1], out _), "valid command accepted");
        Check(sim.Round == 1 && sim.PendingActor == null && sim.Log[0].Kind == "turn", "commit transitions once");
        Check(sim.Units[1].CurrentHp < 10000 && sim.Units[2].CurrentHp == 10000, "chosen unique target hit");
        Check(sim.Log.All(e => e.ActorUnitId != null && e.TargetUnitId != null), "event identities populated");
        var same = Fixture(); same.PrepareNextTurn(); same.TrySubmitCommand(0, same.Units[1], out _);
        Check(Snapshot(sim) == Snapshot(same), "same seed and commands are deterministic");
        var different = Fixture(); different.PrepareNextTurn(); different.TrySubmitCommand(0, different.Units[2], out _);
        Check(different.Units[1].CurrentHp == 10000 && different.Units[2].CurrentHp < 10000, "different selection changes result");
        var special = Fixture(); special.Units[0].CooldownLeft["special"] = 0; special.PrepareNextTurn();
        special.TrySubmitCommand(1, special.Units[1], out _);
        Check(special.Units[1].CurrentHp < sim.Units[1].CurrentHp, "different ability changes result");
        Check(special.Units[0].CooldownLeft["special"] == 3, "cooldown applied on command");
        var automatic = Fixture(); automatic.RunAutomaticTurn();
        Check(Snapshot(automatic) == Snapshot(same), "auto uses command path");
        var legacy = Fixture(); legacy.RunTurn(legacy.ProcessUnitsSpeed());
        Check(Snapshot(legacy) == Snapshot(same), "old RunTurn remains compatible");
        Console.WriteLine("PASS: waiting, invalid commands/RNG, legal targets, identities, choices, cooldown, determinism, auto and legacy.");
    }
}
