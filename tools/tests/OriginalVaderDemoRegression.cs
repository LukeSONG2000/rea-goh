using System;using System.IO;using System.Linq;using System.Text.Json;using SwgohRestored.Battle;
var options=new JsonSerializerOptions{IncludeFields=true};
string root="unity/SWGoH-Restored/Assets/StreamingAssets";
T Load<T>(string name)=>JsonSerializer.Deserialize<T>(File.ReadAllText(root+"/"+name),options);
var m=Load<RawEffectManifest>("battle_effects.json");var g=new OriginalEffectGraph(m.effectFiles.SelectMany(f=>Load<RawEffectShard>(f).effects),m.abilities);
var menu=Load<OriginalUnitMenu[]>("abilities.json").Single(x=>x.def_id=="VADER");
var roster=JsonDocument.Parse(File.ReadAllText(root+"/roster.json")).RootElement.EnumerateArray().Single(x=>x.GetProperty("def_id").GetString()=="VADER");
BattleUnit U(int side,int slot){var u=new BattleUnit{BaseId="VADER",UnitId=$"VADER#{side}_{slot}",Side=side,Level=85,IsLeader=slot==0};foreach(var v in roster.GetProperty("stats").EnumerateArray())u.Stats[int.Parse(v.GetProperty("k").GetString())]=v.GetProperty("v").GetDouble();u.MaxHp=u.CurrentHp=u.GetStat(1);u.MaxShield=u.CurrentShield=u.GetStat(25);u.Categories.UnionWith(menu.categoryId);u.SquadPositionPriority=menu.squadPositionPriority;u.Abilities=menu.abilities.Select(g.Bind).ToList();u.Passives=menu.passives.Select(g.Bind).ToList();return u;}

void Check(bool value,string reason){if(!value)throw new Exception(reason);}
string Run(int seed){
var sim=new BattleSim(new[]{U(0,0)},new[]{U(1,0),U(1,1)},seed);Check(sim.TryInitializeOriginalPassives(out var reason,true),reason);
var q=new System.Collections.Generic.Queue<string>(new[]{"specialability_vader03","specialability_vader02","specialability_vader01","basicability_darthvader"});
for(int turn=0;turn<20 && q.Count>0 && !sim.CheckEnd(out _);turn++){
 var actor=sim.PrepareNextTurn();if(actor==null)break;if(actor.Side!=0){sim.RunAutomaticTurn();continue;}
 if(seed==4490){Check(sim.Round==2 && sim.Log.Count==34,"seed4490 exact startup trace");Check(actor.PersistentEffects.Count(s=>s.Definition.type==4)==2,"two real ability blocks");Check(actor.Abilities.Where(a=>a.Kind=="special").All(a=>!sim.IsAbilityReady(actor.Abilities.IndexOf(a))&&sim.OriginalUnsupportedReason(a)==null),"native ability block, not missing graph");return JsonSerializer.Serialize(sim.Log,options);}
 string id=q.Peek();int ix=actor.Abilities.FindIndex(x=>x.Id==id);Check(sim.IsAbilityReady(ix),id+" must be naturally ready");
 var target=sim.GetLegalTargets(ix).OrderByDescending(u=>u.HasEffectTag("vader_merciless_marker")).ThenByDescending(u=>u.PersistentEffects.Count(s=>s.Definition.descriptiveTag!=null&&s.Definition.descriptiveTag.Any(t=>t.tag=="dot_debuff"))).ThenByDescending(u=>u.CurrentHp).First();
 int first=sim.Log.Count;bool hadDot=target.HasEffectTag("dot_debuff");Check(sim.TrySubmitCommand(ix,target,out reason),reason);
 if(id=="specialability_vader03")Check(actor.HasEffectTag("vader_merciless_buff"),"merciless buff");
 if(id=="specialability_vader02"){Check(sim.Log.Skip(first).Where(e=>e.Kind=="damage"||e.Kind=="crit"||e.Kind=="evade").Select(e=>e.TargetUnitId).Distinct().Count()==2,"production two target AOE");Check(sim.Units.Any(u=>u.Side==1&&u.HasEffectTag("dot_debuff")),"production DOT lands");}
 if(id=="specialability_vader01"&&hadDot)Check(!target.HasEffectTag("dot_debuff"),"Culling clears DOT");q.Dequeue();
}
Check(q.Count==0,"all four commands complete before battle end");return JsonSerializer.Serialize(sim.Log,options);
}
foreach(int seed in new[]{4490,8})Check(Run(seed)==Run(seed),"production seed deterministic "+seed);
Console.WriteLine("PASS production roster seed4490 legitimate ability-block + seed8 four-active chain; both traces deterministic twice. Leader remains explicit PENDING training fixture.");



