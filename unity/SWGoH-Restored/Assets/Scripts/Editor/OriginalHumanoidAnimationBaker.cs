#if UNITY_EDITOR
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using Newtonsoft.Json.Linq;
using UnityEditor;
using UnityEngine;
using UnityEngine.Animations;
using UnityEngine.Playables;

namespace SwgohRestored.EditorTools
{
    // Original scalar curves -> original compiled Avatar -> Unity native solver.
    // Produces engine-space samples only. The sole asset exporter writes GLBs.
    public static class OriginalHumanoidAnimationBaker
    {
        static Vector3 V(JToken t) => new Vector3((float)t["x"],(float)t["y"],(float)t["z"]);
        static Quaternion Q(JToken t) => new Quaternion((float)t["x"],(float)t["y"],(float)t["z"],(float)t["w"]);
        static string Safe(string text) => string.Concat(text.Select(c => char.IsLetterOrDigit(c)||c=='_'||c=='-' ? c : '_'));
        static JArray Vec(Vector3 v) => new JArray(v.x,v.y,v.z);
        static JArray Quat(Quaternion q) => new JArray(q.x,q.y,q.z,q.w);
        public static void Run()
        {
            var args=Environment.GetCommandLineArgs();
            int at=Array.IndexOf(args,"-nativeAnimationManifest");
            string manifestPath=at>=0 ? args[at+1] : Path.GetFullPath(Path.Combine(Application.dataPath,"../../../logs/pose_recovery/native_sources/char_vader_pre/manifest.json"));
            Bake(manifestPath);
        }
        public static void RunAll()
        {
            var args=Environment.GetCommandLineArgs();int at=Array.IndexOf(args,"-nativeAnimationManifestRoot");
            string directory=at>=0 ? args[at+1] : Path.GetFullPath(Path.Combine(Application.dataPath,"../../../logs/pose_recovery/native_sources"));
            var results=new JArray();
            foreach(string child in Directory.GetDirectories(directory).OrderBy(p=>p,StringComparer.Ordinal))
            {
                string path=Path.Combine(child,"manifest.json");if(!File.Exists(path))continue;
                var row=new JObject { ["manifest"]=path };
                try
                {
                    Bake(path);
                    var baked=JObject.Parse(File.ReadAllText(Path.Combine(child,"baked/manifest.json")));
                    row["baked"] = ((JArray)baked["clips"]).Count(c=>(string)c["status"]=="BAKED_NATIVE");
                    row["pending"] = ((JArray)baked["clips"]).Count(c=>(string)c["status"]!="BAKED_NATIVE");
                }
                catch(Exception error) { row["status"]="PENDING_ORIGINAL";row["error"]=error.ToString();Debug.LogError(error); }
                results.Add(row);
                File.WriteAllText(Path.Combine(directory,"batch-bake-results.json"),results.ToString());
            }
        }
        public static void Bake(string manifestPath)
        {
            string directory=Path.GetDirectoryName(Path.GetFullPath(manifestPath));
            var manifest=JObject.Parse(File.ReadAllText(manifestPath));
            string character=(string)manifest["character"];
            string avatarFile=(string)manifest["selectedAvatarFile"];
            if (string.IsNullOrEmpty(avatarFile))
            {
                var avatars=(JArray)manifest["avatars"];
                if (avatars.Count!=1) throw new InvalidDataException("Manifest must resolve the Animator's original Avatar unambiguously.");
                avatarFile=(string)avatars[0]["file"];
            }
            var avatarRaw=OriginalHumanoidPoseProbe.Read(Path.Combine(directory,avatarFile));
            var hierarchy=JObject.Parse(File.ReadAllText(Path.Combine(directory,(string)manifest["hierarchyFile"])));
            var hierarchyHashes=new HashSet<uint>();
            foreach(var transform in (JArray)hierarchy["transforms"])
            {
                string path=(string)transform["hierarchyPath"];
                if(string.IsNullOrEmpty(path))throw new InvalidDataException("Original Transform hierarchy path is missing.");
                for(;;)
                {
                    hierarchyHashes.Add(PathCrc(path));
                    int slash=path.IndexOf('/');if(slash<0)break;path=path.Substring(slash+1);
                }
            }
            string assetDirectory="Assets/OriginalPoseProbe/"+Safe(character);
            Directory.CreateDirectory(assetDirectory);
            string avatarPath=assetDirectory+"/original-avatar.asset";
            OriginalHumanoidPoseProbe.WriteAsset(avatarPath,"Avatar",90,9000000,avatarRaw);
            AssetDatabase.ImportAsset(avatarPath,ImportAssetOptions.ForceSynchronousImport|ImportAssetOptions.ForceUpdate);
            var avatar=AssetDatabase.LoadAssetAtPath<Avatar>(avatarPath);
            if (!avatar || !avatar.isValid || !avatar.isHuman)
                throw new InvalidDataException("Production bake requires the original compiled human Avatar; no inferred retargeting fallback.");
            string outputDirectory=Path.Combine(directory,"baked"); Directory.CreateDirectory(outputDirectory);
            File.WriteAllText(Path.Combine(outputDirectory,"avatar-readback.json"),EditorJsonUtility.ToJson(avatar,true));
            var results=new JArray();
            foreach (JObject entry in (JArray)manifest["clips"])
            {
                var result=new JObject { ["source"]=entry.DeepClone() };
                GameObject root=null; PlayableGraph graph=default;
                try
                {
                    var raw=OriginalHumanoidPoseProbe.Read(Path.Combine(directory,(string)entry["file"]));
                    var clip=OriginalHumanoidPoseProbe.ExpandMuscles(raw,avatarRaw,hierarchyHashes);
                    string stem=Safe((string)entry["sourceBundle"]+"_"+(string)entry["pathId"]);
                    string clipPath=assetDirectory+"/"+stem+".anim";
                    var existing=AssetDatabase.LoadAssetAtPath<AnimationClip>(clipPath);
                    if (existing) { EditorUtility.CopySerialized(clip,existing); UnityEngine.Object.DestroyImmediate(clip); clip=existing; }
                    else AssetDatabase.CreateAsset(clip,clipPath);
                    EditorUtility.SetDirty(clip); AssetDatabase.SaveAssets();
                    File.WriteAllText(Path.Combine(outputDirectory,stem+".clip-readback.json"),EditorJsonUtility.ToJson(clip,true));
                    var slots=(JArray)raw["m_MuscleClip"]["m_IndexArray"];
                    bool sourceHuman=slots.Any(x=>(int)x>=0);
                    if (sourceHuman && !clip.humanMotion) throw new InvalidDataException("Original humanoid curves did not compile as human motion.");
                    root=Skeleton(avatarRaw);
                    var animator=root.AddComponent<Animator>(); animator.avatar=avatar;
                    animator.cullingMode=AnimatorCullingMode.AlwaysAnimate; animator.applyRootMotion=false;
                    graph=PlayableGraph.Create("NativeBake_"+stem); graph.SetTimeUpdateMode(DirectorUpdateMode.Manual);
                    var playable=AnimationClipPlayable.Create(graph,clip); playable.SetApplyFootIK(false);
                    var output=AnimationPlayableOutput.Create(graph,"Original",animator); output.SetSourcePlayable(playable); graph.Play();
                    var nodes=root.GetComponentsInChildren<Transform>(true).Where(t=>t!=root.transform).ToArray();
                    var humanBones=new HashSet<string>(((JArray)avatarRaw["m_HumanDescription"]["m_Human"]).Select(b=>(string)b["m_BoneName"]));
                    var transformPaths=new HashSet<string>(OriginalHumanoidPoseProbe.LastSourceCurveBindings
                        .Where(b=>(string)b["type"]=="Transform").Select(b=>(string)b["path"]));
                    var tracks=nodes.Select(t=>new JObject { ["path"]=AnimationUtility.CalculateTransformPath(t,root.transform),
                        ["name"]=t.name, ["humanBone"]=humanBones.Contains(t.name),
                        ["sourceTransformBinding"]=transformPaths.Contains(AnimationUtility.CalculateTransformPath(t,root.transform)),
                        ["positions"]=new JArray(), ["rotations"]=new JArray(), ["scales"]=new JArray() }).ToArray();
                    float start=(float)raw["m_MuscleClip"]["m_StartTime"], stop=(float)raw["m_MuscleClip"]["m_StopTime"], rate=(float)raw["m_SampleRate"];
                    if (start!=0 || rate<=0 || stop<=start) throw new NotSupportedException("Nonzero clip origin or invalid source sampling interval.");
                    var times=new List<float>();
                    for (int frame=0;frame/rate < stop-1e-6f;frame++) times.Add(frame/rate);
                    times.Add(stop);
                    var previous=new Quaternion[nodes.Length];
                    for (int frame=0;frame<times.Count;frame++)
                    {
                        playable.SetTime(times[frame]); graph.Evaluate(0);
                        for (int n=0;n<nodes.Length;n++)
                        {
                            var node=nodes[n]; var q=node.localRotation;
                            if (frame>0 && Quaternion.Dot(previous[n],q)<0) q=new Quaternion(-q.x,-q.y,-q.z,-q.w);
                            previous[n]=q;
                            ((JArray)tracks[n]["positions"]).Add(Vec(node.localPosition));
                            ((JArray)tracks[n]["rotations"]).Add(Quat(q));
                            ((JArray)tracks[n]["scales"]).Add(Vec(node.localScale));
                        }
                    }
                    var baked=new JObject { ["character"]=character, ["source"]=entry.DeepClone(), ["sourceName"]=raw["m_Name"],
                        ["unityVersion"]=Application.unityVersion, ["avatarMode"]="original compiled Avatar",
                        ["space"]="Unity local TRS relative to original Avatar hierarchy; external prefab Scaler excluded",
                        ["sampleRate"]=rate,["startTime"]=start,["stopTime"]=stop,["times"]=new JArray(times),
                        ["tracks"]=new JArray(tracks),["events"]=raw["m_Events"]?.DeepClone(),
                        ["sourceCurveBindings"]=OriginalHumanoidPoseProbe.LastSourceCurveBindings.DeepClone(),
                        ["unboundTransformBindings"]=OriginalHumanoidPoseProbe.LastUnboundTransformBindings.DeepClone(),
                        ["hierarchyEvidence"]=hierarchy["source"]?.DeepClone(),
                        ["loopTime"]=raw["m_MuscleClip"]["m_LoopTime"], ["humanMotion"]=clip.humanMotion };
                    string file=stem+".json";File.WriteAllText(Path.Combine(outputDirectory,file),baked.ToString());
                    result["status"]="BAKED_NATIVE";result["file"]=file;result["samples"]=times.Count;
                    result["sourceStop"]=stop;result["compiledLength"]=clip.length;
                    result["unboundTransformBindings"]=OriginalHumanoidPoseProbe.LastUnboundTransformBindings.DeepClone();
                }
                catch(Exception error) { result["status"]="PENDING_ORIGINAL";result["error"]=error.ToString();Debug.LogError(error); }
                finally { if(graph.IsValid())graph.Destroy();if(root)UnityEngine.Object.DestroyImmediate(root); }
                results.Add(result);
            }
            File.WriteAllText(Path.Combine(outputDirectory,"manifest.json"),new JObject { ["character"]=character,["clips"]=results }.ToString());
            AssetDatabase.SaveAssets();
        }
        static uint PathCrc(string path)
        {
            uint crc=0xffffffff;
            foreach(byte value in System.Text.Encoding.UTF8.GetBytes(path))
            {
                crc^=value;
                for(int bit=0;bit<8;bit++)crc=(crc>>1)^((crc&1)!=0?0xedb88320u:0u);
            }
            return crc^0xffffffff;
        }
        static GameObject Skeleton(JObject avatar)
        {
            var description=(JObject)avatar["m_HumanDescription"];
            var bones=(JArray)description["m_Skeleton"];
            // Older original assets omit HumanDescription parent names. Their
            // compiled Avatar still records exact root-relative paths in m_TOS.
            var paths=((JArray)avatar["m_TOS"]).Select(t=>(string)t["second"]).Where(p=>!string.IsNullOrEmpty(p))
                .GroupBy(p=>p.Split('/').Last()).ToDictionary(g=>g.Key,g=>g.ToArray());
            var nodes=new Dictionary<string,GameObject>();
            foreach(var bone in bones) nodes.Add((string)bone["m_Name"],new GameObject((string)bone["m_Name"]));
            GameObject root=null;
            foreach(var bone in bones)
            {
                var obj=nodes[(string)bone["m_Name"]];string parent=(string)bone["m_ParentName"];
                if(string.IsNullOrEmpty(parent) && bone!=bones[0])
                {
                    string name=(string)bone["m_Name"];
                    if(!paths.TryGetValue(name,out var matches) || matches.Length!=1)
                        throw new InvalidDataException("Original Avatar path must resolve omitted parent uniquely: "+name);
                    string path=matches[0];int slash=path.LastIndexOf('/');
                    parent=slash<0?(string)bones[0]["m_Name"]:path.Substring(0,slash).Split('/').Last();
                    if(!nodes.ContainsKey(parent))throw new InvalidDataException("Original Avatar parent absent from description: "+path);
                }
                if(!string.IsNullOrEmpty(parent))obj.transform.SetParent(nodes[parent].transform,false);
                else if(root)throw new InvalidDataException("Multiple Avatar roots."); else root=obj;
                obj.transform.localPosition=V(bone["m_Position"]);obj.transform.localRotation=Q(bone["m_Rotation"]);obj.transform.localScale=V(bone["m_Scale"]);
            }
            return root;
        }
    }
}
#endif
