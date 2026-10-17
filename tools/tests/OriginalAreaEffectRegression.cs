using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text.Json;
using SwgohRestored.Battle;

// Real exported graph. Stats and direct lifecycle dispatch are controlled fixtures,
// not a claim of complete native-client parity or account progression.
public static class OriginalAreaEffectRegression
{
    static readonly JsonSerializerOptions Json = new() { IncludeFields = true };
    static OriginalEffectGraph Graph;
    static Dictionary<string, OriginalUnitMenu> Menus;
    static void Check(bool ok, string message) { if (!ok) throw new Exception(message); }
    static T Load<T>(string path) => JsonSerializer.Deserialize<T>(File.ReadAllText(path), Json);
    static BattleUnit Unit(string hero, string id, int side, int speed = 200)
    {
        var u = new BattleUnit { BaseId=hero, UnitId=id, Side=side, Level=85, CombatType=1,
            CurrentHp=100000, MaxHp=100000, Stats=new Dictionary<int,double> {
                [1]=100000, [5]=speed, [6]=1000, [7]=1000, [18]=100,
                [28]=-1, [29]=-1, [26]=-1, [27]=-1 } };
        u.Categories.UnionWith(Menus[hero].categoryId);
        u.Abilities.AddRange(Menus[hero].abilities.Select(Graph.Bind));
        return u;
    }
    static int Skill(BattleUnit u,string id) => u.Abilities.FindIndex(a=>a.Id==id);
    static void Condition(BattleSim sim,BattleUnit u,int type) => typeof(BattleSim)
        .GetMethod("OriginalConditionEvent",BindingFlags.NonPublic|BindingFlags.Instance)
        .Invoke(sim,new object[]{u,type});
    static string Snapshot(BattleSim sim)
    {
        var ctx=typeof(BattleSim).GetField("_ctx",BindingFlags.NonPublic|BindingFlags.Instance).GetValue(sim);
        var seed=typeof(JavaRandom).GetField("_seed",BindingFlags.NonPublic|BindingFlags.Instance);
        string rng=string.Join(",",typeof(BattleContext).GetFields().Select(f=>seed.GetValue(f.GetValue(ctx))));
        return sim.Round+"|"+sim.PendingActor?.UnitId+"|"+rng+"|"+
            string.Join(";",sim.Units.Select(u=>$"{u.UnitId}:{u.CurrentHp:R}:{u.CurrentShield:R}:{u.SpeedFill:R}:{u.SpeedFillOverflow:R}:"+
                string.Join(",",u.CooldownLeft.OrderBy(p=>p.Key))+":"+u.PersistentEffects.Count))+"|"+JsonSerializer.Serialize(sim.Log,Json);
    }
    static (BattleSim sim,BattleUnit caster,BattleUnit a,BattleUnit b) Cast(string hero,string ability,long seed)
    {
        var caster=Unit(hero,"caster",0,500); var a=Unit("SNOWTROOPER","a",1);
        var b=Unit("SNOWTROOPER","b",1,150);
        var sim=new BattleSim(new[]{caster},new[]{a,b},seed);
        Check(sim.PrepareNextTurn()==caster,"caster starts");
        int index=Skill(caster,ability); Check(index>=0,"real menu ability exists");
        Check(sim.OriginalUnsupportedReason(caster.Abilities[index])==null,"real AOE unsupported: "+sim.OriginalUnsupportedReason(caster.Abilities[index]));
        Check(sim.TrySubmitCommand(index,a,out var error),"AOE submit: "+error);
        return (sim,caster,a,b);
    }
    static void AreaAndLifecycle()
    {
        bool all=false,mixed=false;
        for(long seed=0;seed<200 && (!all || !mixed);seed++)
        {
            var (sim,caster,a,b)=Cast("VADER","specialability_vader02",seed);
            var damage=sim.Log.Where(e=>e.Kind=="damage"||e.Kind=="crit").ToArray();
            Check(damage.Length==2 && damage.Select(e=>e.TargetUnitId).Distinct().Count()==2,"AOE hits each target exactly once");
            Check(damage.All(e=>e.EffectId=="specialability_vader02_damage_u05" && e.Amount>=1047 && e.Amount<=1157),"1.102 damage +/-5 percent");
            foreach(var target in new[]{a,b})
            {
                int dots=target.PersistentEffects.Count(s=>s.Definition.type==9);
                if(dots>0 && dots<3) mixed=true;
                if(dots!=3 || !target.PersistentEffects.Any(s=>s.Definition.type==8) || all) continue;
                all=true;
                Check(target.PersistentEffects.Where(s=>s.Definition.type==9).Select(s=>s.InstanceId).Distinct().Count()==3,"three independent DOT instances");
                double speed=target.Stats[5]; Check(target.GetStat(5)==(int)(speed*.75),"speed down truncates 25 percent initial speed");
                // Reinsert one real speed definition as a controlled same-line instance.
                var slow=target.PersistentEffects.Single(s=>s.Definition.type==8);
                Check(slow.Amount!=0 && slow.IsDebuff && !slow.IsBuff,"real slow has nonzero classified amount");
                var extra=new OriginalPersistentEffect { Definition=slow.Definition,Source=slow.Source,
                    Host=slow.Host,Graph=slow.Graph,Amount=slow.Amount,IsBuff=slow.IsBuff,
                    IsDebuff=slow.IsDebuff,StackingLine=slow.StackingLine };
                target.PersistentEffects.Add(extra);
                Check(target.GetStat(5)==(int)(speed*.75),"same stacking line does not double nonzero slow");
                target.PersistentEffects.Remove(extra);
                for(int turn=1;turn<=2;turn++)
                {
                    double hp=target.CurrentHp; int start=sim.Log.Count;
                    Condition(sim,target,25);
                    Check(target.CurrentHp==hp-15000,"three DOT ticks each five percent max health");
                    var events=sim.Log.Skip(start).ToArray();
                    Check(events.Count(e=>e.EffectId=="basic_dot_damage" && e.Kind=="damage")==3,"three child damage events");
                    Check(target.PersistentEffects.Count(s=>s.Definition.type==9)==(turn==1?3:0),"DOT expires after second final tick");
                    if(turn==2) Check(Array.FindIndex(events,e=>e.Kind=="status_remove")>Array.FindLastIndex(events,e=>e.Kind=="damage"),"all DOT ticks precede every expiry in shared unit trackers");
                    Condition(sim,target,32);
                }
                Check(target.GetStat(5)==speed,"speed restores after two complete turns");
                Console.WriteLine("PASS: full DOT/slow lifecycle seed "+seed);
            }
        }
        Check(all && mixed,"seed sweep observes all three and independently resisted DOT applications");
    }
    static void DeathAndGeneralization()
    {
        bool deathObserved=false;
        for(long seed=0;seed<200;seed++)
        {
            var (sim,caster,a,b)=Cast("VADER","specialability_vader02",seed);
            if(!a.PersistentEffects.Any(s=>s.Definition.type==9)) continue;
            a.CurrentHp=1; a.SpeedFill=1000; caster.SpeedFill=0; b.SpeedFill=0;
            var pending=sim.PrepareNextTurn();
            Check(!a.Alive && pending!=a && (pending==null||pending.Alive),"DOT death skips dead pending actor");
            Check(sim.Log.Any(e=>e.Kind=="death"&&e.TargetUnitId==a.UnitId),"DOT death event retained");
            deathObserved=true;
            break;
        }
        Check(deathObserved,"seed sweep exercised DOT turn-start death");
        bool slowed=false;
        for(long seed=0;seed<50 && !slowed;seed++)
        {
            var (sim,caster,a,b)=Cast("PHASMA","specialability_phasma02",seed);
            Check(sim.Log.Count(e=>e.Kind=="damage"||e.Kind=="crit")==2,"Phasma uses same AOE interpreter");
            slowed=new[]{a,b}.Any(t=>t.GetStat(5)==(int)(t.Stats[5]*.75));
        }
        Check(slowed,"Phasma original speed down uses shared stat path");
    }
    static void SeedDoubleRun()
    {
        string Run()
        {
            var (sim,caster,a,b)=Cast("VADER","specialability_vader02",0);
            for(int turn=0;turn<2;turn++)
                foreach(var target in new[]{a,b}) { Condition(sim,target,25);Condition(sim,target,32); }
            return Snapshot(sim);
        }
        Check(Run()==Run(),"same-seed real AOE and lifecycle double-run preserves state/log/RNG");
    }
    static void FractionalRealSpeed()
    {
        bool observed=false;
        for(long seed=0;seed<50 && !observed;seed++)
        {
            var (_,_,_,target)=Cast("VADER","specialability_vader02",seed);
            if(!target.PersistentEffects.Any(s=>s.Definition.type==8)) continue;
            Check(target.Stats[5]==150 && target.GetStat(5)==112,"real initial speed 150 minus 25 percent truncates 112.5 to 112");
            observed=true;
        }
        Check(observed,"fractional real-speed fixture exercised");
    }
    static void AreaRngAndPriorityOrder()
    {
        const long seed=42;
        var caster=Unit("VADER","caster",0,500);
        var a=Unit("SNOWTROOPER","a",1);
        var b=Unit("SNOWTROOPER","b",1);
        a.SquadPositionPriority=0; b.SquadPositionPriority=-1;
        var sim=new BattleSim(new[]{caster},new[]{a,b},seed);
        Check(sim.PrepareNextTurn()==caster,"RNG fixture caster starts");
        var expected=new JavaRandom(seed);
        // Native affected-union insertion order a,b: physical and mental evade
        // rolls per target. Unavoidable apply type consumes no draw.
        for(int i=0;i<4;i++) expected.NextInt(100);
        // ApplyToValidTargets rolls ALL chances before sorting by priority.
        expected.NextInt(100); expected.NextInt(100);
        float baseDamage=1000*1.102f;
        int variance=(int)(baseDamage*.05f);
        double[] amounts=new double[2];
        for(int i=0;i<2;i++)
        {
            expected.NextInt(100); // crit roll still consumed; controlled chance < 0
            amounts[i]=(int)(baseDamage+expected.NextInt(variance*2|1)-variance);
        }
        Check(sim.TrySubmitCommand(Skill(caster,"specialability_vader02"),a,out var error),"RNG fixture submit: "+error);
        var hits=sim.Log.Where(e=>e.Kind=="damage"||e.Kind=="crit").Take(2).ToArray();
        Check(hits.Length==2 && hits[0].TargetUnitId=="b" && hits[1].TargetUnitId=="a",
            "damage follows SquadPositionPriority rather than affected-pool insertion order");
        Check(hits[0].Amount==amounts[0] && hits[1].Amount==amounts[1],
            $"AOE draws all chance rolls before sorted crit/variance: expected {amounts[0]},{amounts[1]}, got {hits[0].Amount},{hits[1].Amount}");
    }
    static void ApplyFixture(BattleSim sim,OriginalEffectGraph graph,RawEffect effect,BattleUnit owner,BattleUnit target)
    {
        typeof(BattleSim).GetMethod("ExecuteOriginalReferences",BindingFlags.NonPublic|BindingFlags.Instance)
            .Invoke(sim,new object[]{graph,new[]{new RawEffectReference{id=effect.id}},owner,target,"fixture","fixture",null});
    }
    static RawEffect CloneSlow(string id) // Synthetic definition fixture; catalog rows remain untouched.
    {
        var e=JsonSerializer.Deserialize<RawEffect>(JsonSerializer.Serialize(Graph.Effects["specialability_vader01_speeddown_BASE"],Json),Json);
        e.id=id; return e;
    }
    static void SyntheticClassificationAndTagTransitions()
    {
        foreach(bool forceBuff in new[]{true,false})
        {
            var effect=CloneSlow("fixture_forced_classification");
            effect.param=new[]{"SPEED",forceBuff?"FORCE_BUFF":"FORCE_DEBUFF","UNRESISTABLE"};
            effect.multiplierAmountDecimal=forceBuff?-2500:2500;
            var graph=new OriginalEffectGraph(new[]{effect},Array.Empty<RawAbility>());
            var caster=Unit("VADER","caster",0);var target=Unit("SNOWTROOPER","target",1);
            var sim=new BattleSim(new[]{caster},new[]{target},0);
            ApplyFixture(sim,graph,effect,caster,target);
            var state=target.PersistentEffects.Single();
            Check(state.IsBuff==forceBuff && state.IsDebuff!=forceBuff,"force classification overrides multiplier sign");
            Check(state.Amount==(forceBuff?-50:50),"force classification preserves signed modifier amount");
        }
        // Synthetic wire-28 observer uses a real DOT child to make firings observable.
        var tagged=CloneSlow("fixture_tag_transition");
        tagged.param=new[]{"SPEED","UNRESISTABLE"};
        tagged.descriptiveTag=new[]{new RawTag{tag="fixture_observed_tag"}};
        var watcher=new RawEffect{id="fixture_tag_observer",type=9,
            effectReference=new[]{new RawEffectReference{id="basic_dot_damage"}}};
        var fixtureGraph=new OriginalEffectGraph(new[]{tagged,watcher,Graph.Effects["basic_dot_damage"]},Array.Empty<RawAbility>());
        var source=Unit("VADER","source",0);var host=Unit("SNOWTROOPER","host",1);
        var testSim=new BattleSim(new[]{source},new[]{host},0);
        var observer=new OriginalPersistentEffect{Definition=watcher,Graph=fixtureGraph,Source=source,Host=host};
        observer.Triggers.Add(new OriginalConditionState{Definition=new RawCondition{conditionType=28,conditionValue="fixture_observed_tag"}});
        host.PersistentEffects.Add(observer);
        ApplyFixture(testSim,fixtureGraph,tagged,source,host);
        Check(host.CurrentHp==95000,"wire28 fires when matching tag count changes zero to one");
        ApplyFixture(testSim,fixtureGraph,tagged,source,host);
        Check(host.CurrentHp==95000,"wire28 does not fire when existing tag count changes one to two");
        host.PersistentEffects.RemoveAll(s=>s.Definition==tagged);
        ApplyFixture(testSim,fixtureGraph,tagged,source,host);
        Check(host.CurrentHp==90000,"wire28 fires again after tag count returns to zero");
    }
    static void AtomicArea()
    {
        foreach(bool ship in new[]{true,false})
        {
            var caster=Unit("VADER","caster",0,500);var a=Unit("SNOWTROOPER","valid",1);
            var b=Unit("SNOWTROOPER","invalid",1);
            if(ship)b.CombatType=2;else b.RelicTier=1;
            var sim=new BattleSim(new[]{caster},new[]{a,b},42);sim.PrepareNextTurn();
            string before=Snapshot(sim);
            Check(!sim.TrySubmitCommand(Skill(caster,"specialability_vader02"),a,out _),"unsupported secondary AOE target rejected");
            Check(Snapshot(sim)==before,"AOE rejection preserves state and all RNG streams");
        }
    }
    public static void Main(string[] args)
    {
        string dir=args.Length>0?args[0]:"unity/SWGoH-Restored/Assets/StreamingAssets";
        var manifest=Load<RawEffectManifest>(Path.Combine(dir,"battle_effects.json"));
        Graph=new OriginalEffectGraph(manifest.effectFiles.SelectMany(f=>Load<RawEffectShard>(Path.Combine(dir,f)).effects),manifest.abilities);
        Menus=Load<OriginalUnitMenu[]>(Path.Combine(dir,"abilities.json")).ToDictionary(u=>u.def_id);
        AreaAndLifecycle();DeathAndGeneralization();AtomicArea();SeedDoubleRun();
        FractionalRealSpeed();SyntheticClassificationAndTagTransitions();AreaRngAndPriorityOrder();
        var audit=new BattleSim(Array.Empty<BattleUnit>(),Array.Empty<BattleUnit>(),0);
        var supported=Menus.Values.SelectMany(menu=>menu.abilities.Select(a=>(unit:menu.def_id,ability:Graph.Bind(a))))
            .Where(pair=>audit.OriginalUnsupportedReason(pair.ability)==null).ToArray();
        Console.WriteLine("CAPABILITY: "+supported.Length+" activated menu abilities across "+supported.Select(p=>p.unit).Distinct().Count()+" units; operator validation, not whole-kit completion.");
        Console.WriteLine("PASS: real Vader/Phasma AOE graph, independent DOT resistance/lifecycle, slow stacking/expiry, DOT death, atomic secondary-target rejection.");
    }
}
