using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text.Json;
using SwgohRestored.Battle;

// Real maximum-tier exported references; controlled stats/turn meter are fixtures.
// This suite intentionally fails until each complete activation is supported.
public static class OriginalVaderCompleteRegression
{
    static readonly JsonSerializerOptions Json = new(){IncludeFields=true};
    static OriginalEffectGraph Graph;
    static Dictionary<string,OriginalUnitMenu> Menus;
    static void Check(bool ok,string message){if(!ok)throw new Exception(message);}
    static T Load<T>(string p)=>JsonSerializer.Deserialize<T>(File.ReadAllText(p),Json);
    static BattleUnit Unit(string hero,string id,int side,int speed)
    {
        var u=new BattleUnit{BaseId=hero,UnitId=id,Side=side,Level=85,CombatType=1,
            MaxHp=100000,CurrentHp=100000,Stats=new Dictionary<int,double>{
                [1]=100000,[5]=speed,[6]=1000,[7]=1000,[18]=100,[28]=-1,[29]=-1,[26]=-1,[27]=-1}};
        u.Categories.UnionWith(Menus[hero].categoryId);
        u.Abilities.AddRange(Menus[hero].abilities.Select(Graph.Bind));return u;
    }
    static void Cast(BattleSim sim,BattleUnit actor,string id,BattleUnit target)
    {
        Check(sim.PrepareNextTurn()==actor,"fixture actor must be pending");
        int index=actor.Abilities.FindIndex(a=>a.Id==id);
        Check(index>=0,"real ability menu row missing");
        Check(sim.TrySubmitCommand(index,target,out var reason),id+" rejected: "+reason);
    }
    static void CullingRemovesSelectedDebuffs()
    {
        var vader=Unit("VADER","vader",0,500);var a=Unit("SNOWTROOPER","a",1,200);
        var b=Unit("SNOWTROOPER","b",1,150);var sim=new BattleSim(new[]{vader},new[]{a,b},0);
        Cast(sim,vader,"specialability_vader02",a);
        Check(a.PersistentEffects.Count>=3,"real Force Crush generated the debuff fixture");
        int otherCount=b.PersistentEffects.Count;
        int counted=a.PersistentEffects.Count(s=>(s.Definition.descriptiveTag??Array.Empty<RawTag>()).Any(t=>t.tag=="countable_debuff"));
        // Controlled scheduling fixture; effect/ability definitions remain original.
        vader.SpeedFill=1000;a.SpeedFill=0;b.SpeedFill=0;
        sim.PrepareNextTurn();
        Check(!sim.GetLegalTargets(vader.Abilities.FindIndex(x=>x.Id=="specialability_vader01")).Contains(vader),
            "Culling's self buffs do not make self a legal selected-enemy command");
        int first=sim.Log.Count;
        Cast(sim,vader,"specialability_vader01",a);
        Check(a.PersistentEffects.All(s=>!(s.Definition.descriptiveTag??Array.Empty<RawTag>()).Any(t=>t.tag=="clearable_debuff"&&!t.exclude)),
            "Culling dispels original clearable debuffs on selected target");
        Check(b.PersistentEffects.Count==otherCount,"Culling must not dispel the unselected target");
        var hits=sim.Log.Skip(first).Where(e=>e.Kind=="damage"||e.Kind=="crit").ToArray();
        Check(hits.Length==1 && hits[0].TargetUnitId==a.UnitId,"Culling uses exactly one original damage branch");
        float expected=1000*2.032f*(1+counted*.5f);
        int variance=(int)(expected*.05f);
        Check(hits[0].Amount>=(int)expected-variance && hits[0].Amount<=(int)expected+variance,
            "pre-dispel count scales original 2.032 source damage by 1+count*.5");
        Check(vader.GetStat(28)==-1 && !vader.PersistentEffects.Any(s=>s.Definition.id=="specialability_vader01_crit_chance"),
            "temporary crit chance expires on PERFORMMOVE after skill references");
    }
    static void CullingKillRefillsTurnMeter()
    {
        var vader=Unit("VADER","vader",0,500);var target=Unit("SNOWTROOPER","target",1,200);
        var survivor=Unit("SNOWTROOPER","survivor",1,150);target.CurrentHp=1;
        var sim=new BattleSim(new[]{vader},new[]{target,survivor},0);
        Cast(sim,vader,"specialability_vader01",target);
        Check(!target.Alive && vader.SpeedFill==GameConstants.UnitSpeedFillMax,"KILL trigger refills turn meter");
        Check(sim.PrepareNextTurn()==vader,"Culling kill grants next eligible turn");
        Check(!vader.PersistentEffects.Any(s=>s.Definition.id=="trigger_extra_turn_self_ENEMYKILLED"),
            "kill trigger expires at end of its activation turn");
    }
    static void MercilessOperatorSlices()
    {
        // Operator fixture: execute an explicitly selected subset of unchanged
        // original records. This is NOT a complete Merciless activation test.
        var vader=Unit("VADER","vader",0,500);var enemy=Unit("SNOWTROOPER","enemy",1,200);
        var sim=new BattleSim(new[]{vader},new[]{enemy},0);
        var slice=Graph.Bind(Menus["VADER"].abilities[0]);
        string[] ids={"specialability_vader03_merciless_attack_buff_ZETA",
            "specialability_vader03_merciless_ability_buff_ZETA",
            "specialability_vader03_merciless_attack_critchance_buff_ZETA",
            "specialability_vader03_merciless_ability_critchance_buff_ZETA",
            "specialability_vader03_merciless_critdamage_buff_ZETA",
            "specialability_vader03_stun_immunity"};
        slice.OriginalReferences=ids.Select(id=>new RawEffectReference{id=id,maxBonusMove=1}).ToArray();
        vader.Abilities.Clear();vader.Abilities.Add(slice);
        Check(sim.OriginalUnsupportedReason(slice)==null,"original Merciless operator subset is admitted");
        Cast(sim,vader,slice.Id,vader);
        foreach(int stat in new[]{6,7,16,28,29})
        {
            var state=vader.PersistentEffects.Single(s=>s.Definition.type==8 &&
                s.Definition.param.Any(p=>BattleSim.OriginalModifierStats.TryGetValue(p,out int found)&&found==stat));
            float expected=(float)vader.GetInitialStat(stat)+state.Amount;
            Check(vader.GetStat(stat)==(stat==6||stat==7?(int)expected:expected),"native stat family "+stat);
        }
        var immunity=vader.PersistentEffects.Single(s=>s.Definition.type==6);
        Check(immunity.IsBuff && !immunity.IsDebuff,"original immunity is a buff and needs no resistance roll");
        // Synthetic incoming record ONLY labels the tag under test; original
        // immunity's effectedTag list remains unchanged.
        var incoming=new RawEffect{descriptiveTag=new[]{new RawTag{tag="paralysis_debuff"}},
            target=Graph.Effects["basicability_vader_block_u03"].target};
        var targets=typeof(BattleSim).GetMethod("OriginalTargets",System.Reflection.BindingFlags.NonPublic|System.Reflection.BindingFlags.Instance);
        var blocked=(IEnumerable<BattleUnit>)targets.Invoke(sim,new object[]{incoming,enemy,vader});
        Check(!blocked.Any(),"immunity removes incoming tagged module before chance/evasion");
        vader.PersistentEffects.Remove(immunity);
        var unblocked=(IEnumerable<BattleUnit>)targets.Invoke(sim,new object[]{incoming,enemy,vader});
        Check(unblocked.Contains(vader),"removing last immunity grant restores target eligibility");
    }
    static void ContextualExecuteOriginalSlice()
    {
        // Original query + contextual dispel; synthetic states provide marker
        // and stealth tags without pretending their unrelated kits are restored.
        var vader=Unit("VADER","vader",0,500);var enemy=Unit("SNOWTROOPER","enemy",1,200);
        var sim=new BattleSim(new[]{vader},new[]{enemy},12);
        var stealth=new OriginalPersistentEffect{Host=enemy,Source=enemy,
            Definition=new RawEffect{type=4,id="fixture-stealth",descriptiveTag=new[]{new RawTag{tag="stealth_buff"}}}};
        enemy.PersistentEffects.Add(stealth);
        var refs=new[]{"specialability_vader03_merciless_dispel_stealth_enemy_all_query",
            "specialability_vader03_merciless_dispel_stealth_enemy_all_CE"}
            .Select(id=>new RawEffectReference{id=id,maxBonusMove=1}).ToArray();
        var method=typeof(BattleSim).GetMethod("ExecuteOriginalReferences",System.Reflection.BindingFlags.NonPublic|System.Reflection.BindingFlags.Instance);
        var ctx=(BattleContext)typeof(BattleSim).GetField("_ctx",System.Reflection.BindingFlags.NonPublic|System.Reflection.BindingFlags.Instance).GetValue(sim);
        var seed=typeof(JavaRandom).GetField("_seed",System.Reflection.BindingFlags.NonPublic|System.Reflection.BindingFlags.Instance);
        var before=seed.GetValue(ctx.BattleRandom);
        method.Invoke(sim,new object[]{Graph,refs,vader,enemy,"fixture-slice","fixture-slice",null});
        Check(enemy.PersistentEffects.Contains(stealth),"zero query sum skips whole contextual dispel");
        Check(before.Equals(seed.GetValue(ctx.BattleRandom)),"skipped module consumes no chance RNG");
        vader.PersistentEffects.Add(new OriginalPersistentEffect{Host=vader,Source=vader,
            Definition=new RawEffect{type=6,id="fixture-query-marker",descriptiveTag=new[]{new RawTag{tag="merciless_stealth_dispel_marker"}}}});
        method.Invoke(sim,new object[]{Graph,refs,vader,enemy,"fixture-slice","fixture-slice",null});
        Check(!enemy.PersistentEffects.Contains(stealth),"positive self-query sum permits enemy contextual dispel");
    }
    static void UniqueSpeedOriginalSlice()
    {
        // Execute the original handler's child chain, without claiming bootup
        // event scheduling or the other unique branches are implemented here.
        var vader=Unit("VADER","vader",0,500);var enemy=Unit("SNOWTROOPER","enemy",1,200);
        var sim=new BattleSim(new[]{vader},new[]{enemy},0);
        var slice=Graph.Bind(Menus["VADER"].abilities[0]);
        slice.OriginalReferences=Graph.Effects["uniqueability_vader01_speed_handler_u06"].effectReference;
        vader.Abilities.Clear();vader.Abilities.Add(slice);
        Check(sim.OriginalUnsupportedReason(slice)==null,"query driven original stat slice is admitted");
        Cast(sim,vader,slice.Id,vader);
        Check(vader.Speed==516,"original self Empire and Sith queries add 8 speed independently");
        Check(vader.PersistentEffects.Count(s=>s.Definition.id=="uniqueability_vader01_speed_u06")==2,
            "zero Jedi/Rebel context produces no state; distinct stacking lines retain both contributions");
    }
    static void FullUniqueInitializationAndDotRemovalRecovery()
    {
        var vader=Unit("VADER","vader",0,500);var enemy=Unit("SNOWTROOPER","enemy",1,200);
        vader.MaxShield=10000;vader.CurrentShield=1000;
        vader.Passives.Add(Graph.Bind(Menus["VADER"].passives.Single(p=>p.kind=="uniqueskill")));
        var sim=new BattleSim(new[]{vader},new[]{enemy},0);
        Check(sim.TryInitializeOriginalPassives(out var reason),"full real unique initialization: "+reason);
        Check(vader.Speed==516 && vader.IsImmuneToEffect("delay"),"unique startup speed and real immunity");
        int states=vader.PersistentEffects.Count, events=sim.Log.Count;
        Check(sim.TryInitializeOriginalPassives(out reason)&&states==vader.PersistentEffects.Count&&events==sim.Log.Count,
            "passive startup is idempotent");
        Cast(sim,vader,"specialability_vader02",enemy);
        int dots=enemy.PersistentEffects.Count(s=>s.Definition.descriptiveTag.Any(t=>t.tag=="dot_debuff"));
        Check(dots>0,"real DOT removal fixture landed");
        vader.CurrentHp=80000;vader.SpeedFill=1000;enemy.SpeedFill=0;
        Cast(sim,vader,"specialability_vader01",enemy);
        Check(vader.CurrentHp==Math.Min(100000,80000+5000*dots),"unique heals 5% per removed DOT instance");
        Check(vader.CurrentShield==1000+200*dots,"unique restores 2% protection per removed DOT instance");
    }
    static string MercilessFullUniqueTurnChain()
    {
        var vader=Unit("VADER","vader",0,500);var a=Unit("SNOWTROOPER","a",1,200);
        var b=Unit("SNOWTROOPER","b",1,150);
        vader.Passives.Add(Graph.Bind(Menus["VADER"].passives.Single(p=>p.kind=="uniqueskill")));
        var sim=new BattleSim(new[]{vader},new[]{a,b},8);
        Check(sim.TryInitializeOriginalPassives(out var reason),reason);
        Cast(sim,vader,"specialability_vader03",a);
        Check(a.HasEffectTag("vader_merciless_marker")&&b.HasEffectTag("vader_merciless_marker"),"Merciless marks both enemies");
        Check(vader.IsImmuneToEffect("paralysis_debuff") && vader.IsImmuneToEffect("fear_debuff"),"Merciless immunity tags applied");
        int basic=vader.Abilities.FindIndex(x=>x.Id=="basicability_darthvader");
        Check(sim.PrepareNextTurn()==vader,"Merciless priority recovery grants immediate turn");
        Check(sim.GetLegalTargets(basic).Count==2,"both marked enemies are initially selectable");
        Cast(sim,vader,"basicability_darthvader",a);
        Check(!a.HasEffectTag("vader_merciless_marker")&&b.HasEffectTag("vader_merciless_marker"),"STARTMOVE consumes only selected marker");
        Check(a.GetPersistentTauntAmount()==0 && b.GetPersistentTauntAmount()==0,
            "native END_OF_ROUND expires all temporary taunts before the next turn starts");
        Check(sim.PrepareNextTurn()==vader,"unique STARTMOVE recovery grants next immediate turn");
        Check(sim.GetLegalTargets(basic).SequenceEqual(new[]{b}),"original taunt ranking forces remaining marked target");
        Cast(sim,vader,"basicability_darthvader",b);
        Check(sim.PrepareNextTurn()==vader,"last marked hit still grants its original priority turn");
        Cast(sim,vader,"basicability_darthvader",a);
        Check(!vader.HasEffectTag("vader_merciless_buff") && !vader.HasEffectTag("specialability_vader03_taunt_handler"),
            "first unmarked move clears Merciless buff and taunt handler by contextual skip");
        Check(vader.SpeedFill==0 && a.GetPersistentTauntAmount()==0 && b.GetPersistentTauntAmount()==0,
            "unmarked move grants no extra turn and clears remaining taunts");
        return JsonSerializer.Serialize(sim.Log,Json);
    }
    static void ExplicitTrainingReadiness()
    {
        var vader=Unit("VADER","vader",0,500);var enemy=Unit("SNOWTROOPER","enemy",1,200);
        vader.IsLeader=true;vader.Passives.AddRange(Menus["VADER"].passives.Select(Graph.Bind));
        var sim=new BattleSim(new[]{vader},new[]{enemy},2);
        var readiness=sim.GetOriginalPassiveReadiness();
        Check(readiness.Single(x=>x.Kind=="uniqueskill").Supported,"unique readiness exposes support");
        Check(!readiness.Single(x=>x.Kind=="leaderskill").Supported,"leader readiness exposes pending operators");
        Check(!sim.TryInitializeOriginalPassives(out var reason) && reason.Contains("leaderability"),"strict startup rejects incomplete leader");
        Check(sim.Log.Count==0 && vader.PersistentEffects.Count==0 && enemy.PersistentEffects.Count==0,
            "strict passive rejection is atomic before applying unique");
        Check(sim.TryInitializeOriginalPassives(out reason,allowUnsupportedForTraining:true),reason);
        Check(vader.Passives.Count==2 && sim.Log.Any(e=>e.Kind=="pending_original"&&e.AbilityId=="leaderability_vader_BASE"),
            "explicit training keeps both declared passives and records skipped leader");
        Check(vader.Speed==516,"training fixture activates supported original unique");
    }
    static void RecoveryRoundingAndImmunity()
    {
        // Controlled health pools exercise native nearest/ties-to-even using
        // unchanged unique recovery records (5% HP and 2% protection).
        var vader=Unit("VADER","vader",0,500);var enemy=Unit("SNOWTROOPER","enemy",1,200);
        vader.MaxHp=50;vader.Stats[1]=50;vader.CurrentHp=10;vader.MaxShield=75;vader.CurrentShield=0;
        var sim=new BattleSim(new[]{vader},new[]{enemy},0);
        typeof(BattleSim).GetMethod("ExecuteOriginalReferences",System.Reflection.BindingFlags.NonPublic|System.Reflection.BindingFlags.Instance)
            .Invoke(sim,new object[]{Graph,Graph.Effects["uniqueability_vader01_recovery_handler"].effectReference,
                enemy,enemy,"fixture-recovery","fixture-recovery",null});
        Check(vader.CurrentHp==12 && vader.CurrentShield==2,"2.5 HP rounds to 2 and 1.5 protection rounds to 2");
        vader.Stats[24]=.5;
        // Synthetic immunity fixture uses the recovered GameConstants tag.
        vader.PersistentEffects.Add(new OriginalPersistentEffect{Definition=new RawEffect{type=6,
            effectedTag=new[]{new RawTag{tag=GameConstants.HealthRecoveryImmunityTag}}}});
        sim.OriginalDamage(vader,enemy,Graph.Effects["basicability_vader_damage_u05"]);
        Check(vader.CurrentHp==12,"actual health recovery immunity prevents health steal without rejecting damage");
    }
    static void MercilessDyingHostCleanup()
    {
        var vader=Unit("VADER","vader",0,500);var a=Unit("SNOWTROOPER","a",1,200);
        var b=Unit("SNOWTROOPER","b",1,150);var sim=new BattleSim(new[]{vader},new[]{a,b},3);
        Cast(sim,vader,"specialability_vader03",a);
        Check(a.HasEffectTag("vader_merciless_marker")&&b.HasEffectTag("vader_merciless_marker"),"death fixture begins with original markers");
        // Controlled death event fixture, isolated from encounter-end cleanup.
        vader.CurrentHp=0;
        typeof(BattleSim).GetMethod("OriginalConditionEvent",System.Reflection.BindingFlags.NonPublic|System.Reflection.BindingFlags.Instance)
            .Invoke(sim,new object[]{vader,21});
        Check(!a.HasEffectTag("vader_merciless_marker")&&!b.HasEffectTag("vader_merciless_marker"),
            "dead host's original handler clears living enemy markers before encounter end");
    }
    public static void Main(string[] args)
    {
        string dir=args.Length>0?args[0]:"unity/SWGoH-Restored/Assets/StreamingAssets";
        var manifest=Load<RawEffectManifest>(Path.Combine(dir,"battle_effects.json"));
        Graph=new OriginalEffectGraph(manifest.effectFiles.SelectMany(f=>Load<RawEffectShard>(Path.Combine(dir,f)).effects),manifest.abilities);
        Menus=Load<OriginalUnitMenu[]>(Path.Combine(dir,"abilities.json")).ToDictionary(u=>u.def_id);
        CullingRemovesSelectedDebuffs();CullingKillRefillsTurnMeter();MercilessOperatorSlices();ContextualExecuteOriginalSlice();UniqueSpeedOriginalSlice();
        FullUniqueInitializationAndDotRemovalRecovery();
        Check(MercilessFullUniqueTurnChain()==MercilessFullUniqueTurnChain(),"Merciless seed double-run determinism");
        ExplicitTrainingReadiness();
        RecoveryRoundingAndImmunity();
        MercilessDyingHostCleanup();
        Console.WriteLine("PASS: real Culling, full unique startup/DOT removal recovery, Merciless marked-target bonus-turn chain; explicit leader PENDING training fixture.");
    }
}
