using System;
using System.Collections.Generic;
using SwgohRestored.Battle;

// Compile with the actual battle sources. Does not claim live-client parity:
// checks formulas and boundary cases supported by the cited native bodies.
public static class OriginalDamageRegression
{
    static void Check(bool ok, string message) { if (!ok) throw new Exception(message); }
    static BattleUnit Unit(string id, int side) => new BattleUnit {
        UnitId = id, BaseId = id, Side = side, Level = 85, CombatType = 1,
        CurrentHp = 10000, MaxHp = 10000,
        Stats = new Dictionary<int, double> { [1] = 10000, [6] = 100, [7] = 200,
            [22] = 100000, [23] = 100000 }
    };
    static RawEffect Effect(int damageType = 2, string source = "ATTACK_DAMAGE") => new RawEffect {
        id = "generic_damage_fixture", type = 1, chance = 100, applyType = 1,
        damageType = damageType, multiplierAmountDecimal = 10000,
        additiveAmountDecimal = "0", param = new[] { source }
    };
    static BattleSim Sim(BattleUnit a, BattleUnit t, long seed = 42) => new BattleSim(new[] { a }, new[] { t }, seed);

    public static void Main()
    {
        var a = Unit("a", 0); var t = Unit("t", 1); t.CurrentShield = 40;
        var sim = Sim(a, t); var hit = sim.OriginalDamage(a, t, Effect());
        Check(hit.Amount == 100 && hit.HpDamage == 60 && hit.ShieldDamage == 40, "shield split");
        Check(t.CurrentHp == 9940 && t.CurrentShield == 0, "shield consumed");
        a = Unit("a", 0); t = Unit("t", 1); a.Stats[34] = .5; t.Stats[34] = 1; t.CurrentShield = 1000;
        hit = Sim(a,t).OriginalDamage(a,t,Effect());
        Check(hit.HpDamage == 50 && hit.ShieldDamage == 50, "penetration belongs to attacker");
        a = Unit("a", 0); t = Unit("t", 1); t.Stats[8] = 637.5;
        hit = Sim(a,t).OriginalDamage(a,t,Effect());
        Check(hit.Amount == 50, "character mitigation uses level*7.5 without ship offset");
        a.Stats[10] = 137.5; a.Stats[56] = 500.0 / 637.5;
        hit = Sim(a,t).OriginalDamage(a,t,Effect());
        Check(hit.Amount == 100, "flat and target-percent penetration");
        a = Unit("a", 0); t = Unit("t", 1); t.Stats[8] = 100000000;
        hit = Sim(a,t).OriginalDamage(a,t,Effect());
        Check(hit.Amount >= 10 && hit.Amount <= 11, "mitigation capped at 90 percent");
        a = Unit("a", 0); t = Unit("t", 1); t.Stats[8] = 100000000; a.Stats[14] = 100000000;
        hit = Sim(a,t).OriginalDamage(a,t,Effect(1));
        Check(hit.Amount == 100 && hit.Kind == "damage", "unmitigable bypasses crit and armor");
        a = Unit("a", 0); t = Unit("t", 1); t.CurrentHp = 20;
        var noKill = Effect(); noKill.param = new[] { "ATTACK_DAMAGE", "NO_KILL" };
        hit = Sim(a,t).OriginalDamage(a,t,noKill);
        Check(hit.HpDamage == 19 && t.CurrentHp == 1, "NO_KILL");
        a = Unit("a", 0); t = Unit("t", 1); a.Stats[28] = 1000; a.Stats[29] = -1000;
        hit = Sim(a,t).OriginalDamage(a,t,Effect(3,"ABILITY_POWER"));
        Check(hit.Amount == 200 && hit.Kind == "damage", "special crit uses stat29 not physical stat28");
        a = Unit("a", 0); t = Unit("t", 1);
        var variance = Effect(); variance.resultVarianceDecimal = 500;
        var expected = new JavaRandom(42); expected.NextInt(100);
        int amount = 100 + expected.NextInt(11) - 5;
        hit = Sim(a,t).OriginalDamage(a,t,variance);
        Check(hit.Amount == amount, "native seeded variance bounds/order");
        a = Unit("a",0); t = Unit("t",1); sim=Sim(a,t);
        sim.OriginalDamage(a,t,Effect(1)); // no crit and zero variance: no RNG draws
        var afterZeroVariance=sim.OriginalDamage(a,t,variance);
        var referenceA=Unit("a",0); var referenceT=Unit("t",1);
        var withoutFirst=Sim(referenceA,referenceT).OriginalDamage(referenceA,referenceT,variance);
        Check(afterZeroVariance.Amount==withoutFirst.Amount && afterZeroVariance.Kind==withoutFirst.Kind,
            "zero variance/unmitigable do not consume RNG");
        a = Unit("a", 0); t = Unit("t", 1);
        var percent = Effect(1,"DAMAGE_HEALTH_PERCENTAGE"); percent.multiplierAmountDecimal=500;
        hit = Sim(a,t).OriginalDamage(a,t,percent);
        Check(hit.Amount == 500, "health percentage uses target max HP");
        var invalid = Effect(); invalid.contextMultiplierDecimal=1;
        Check(BattleSim.ValidateOriginalDamage(invalid)!=null, "unknown context rejected");
        a=Unit("a",0); t=Unit("t",1); sim=Sim(a,t);
        try { sim.OriginalDamage(a,t,invalid); throw new Exception("unsupported effect accepted"); }
        catch (NotSupportedException) { }
        Check(t.CurrentHp==10000 && sim.Log.Count==0, "rejection occurs before state mutation");
        var retry=sim.OriginalDamage(a,t,variance);
        var freshA=Unit("a",0); var freshT=Unit("t",1);
        var fresh=Sim(freshA,freshT).OriginalDamage(freshA,freshT,variance);
        Check(retry.Amount==fresh.Amount && retry.Kind==fresh.Kind, "rejection does not consume RNG");
        a.RelicTier=1;
        Check(BattleSim.ValidateOriginalDamage(a,t,Effect())!=null, "unknown relic table rejected before commit");
        a.RelicTier=0;
        var dead = Unit("t",1); dead.CurrentHp=1; sim=Sim(a,dead); sim.OriginalDamage(a,dead,Effect());
        Check(sim.Log.Exists(e=>e.Kind=="death" && e.TargetUnitId=="t" && e.ActorUnitId=="a"), "death identity");
        a=Unit("a",0); t=Unit("t",1); t.Stats[12]=120000;
        Check(Sim(a,t).OriginalEvaded(a,t,2), "physical dodge stats");
        Check(!Sim(a,t).OriginalEvaded(a,t,1), "unavoidable bypasses dodge");
        t.Stats[19]=2;
        Check(Sim(a,t).OriginalResisted(a,t), "resistance stats");
        Console.WriteLine("PASS: native-supported damage, integer HP/shield, penetration, crit families, variance, evasion/resistance, rejection.");
    }
}
