#if UNITY_EDITOR
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using UnityEditor;
using UnityEngine;
using UnityEngine.Animations;
using UnityEngine.Playables;

namespace SwgohRestored.EditorTools
{
    // Isolated compatibility probe, not a production replacement. Original Unity
    // 6000 serialized muscles must survive import before native evaluation counts.
    public static class OriginalHumanoidPoseProbe
    {
        internal static JArray LastSourceCurveBindings { get; private set; } = new JArray();
        const string AssetRoot = "Assets/OriginalPoseProbe";
        static string Repository => Path.GetFullPath(Path.Combine(Application.dataPath, "../../.."));
        static Vector3 V(JToken t) => new Vector3((float)t["x"], (float)t["y"], (float)t["z"]);
        static Quaternion Q(JToken t) => new Quaternion((float)t["x"], (float)t["y"], (float)t["z"], (float)t["w"]);
        internal static JObject Read(string path)
        {
            var json = JObject.Parse(File.ReadAllText(path));
            var raw = (JObject)(json["raw"] ?? json);
            if (json["typeTree"] != null) Versions(raw, json["typeTree"]);
            return raw;
        }
        static void Versions(JToken value, JToken schema)
        {
            if (value is JObject obj)
            {
                int version = (int?)schema["version"] ?? 1;
                if (version > 1) obj.AddFirst(new JProperty("serializedVersion",version));
                foreach (var child in (JArray)schema["children"])
                    if (obj[(string)child["name"]] is JToken field) Versions(field,child);
            }
            else if (value is JArray array)
            {
                var container = ((JArray)schema["children"]).FirstOrDefault(c => (string)c["name"] == "Array");
                var element = (container?["children"] as JArray)?.FirstOrDefault(c => (string)c["name"] == "data");
                if (element != null)
                    for (int i=0;i<array.Count;i++)
                    {
                        var fields = (JArray)element["children"];
                        if (array[i] is JArray tuple && fields.Count==tuple.Count &&
                            fields.Count==2 && (string)fields[0]["name"]=="first")
                            array[i]=new JObject { ["first"]=tuple[0].DeepClone(), ["second"]=tuple[1].DeepClone() };
                        Versions(array[i],element);
                    }
            }
        }

        public static void Run()
        {
            string input = Path.Combine(Repository, "logs/pose_recovery");
            Directory.CreateDirectory(input);
            var report = new JObject { ["unityVersion"] = Application.unityVersion,
                ["scope"] = "Native compatibility probe only; no production assets replaced." };
            GameObject root = null; PlayableGraph graph = default;
            try
            {
                var avatarRaw = Read(Path.Combine(input, "char_vader_pre/avatar.json"));
                var clipRaw = Read(Path.Combine(input, "char_vader_pre/idle-clip.json"));
                report["sourceEvents"] = clipRaw["m_Events"]?.DeepClone();
                Directory.CreateDirectory(AssetRoot);
                string clipPath = AssetRoot + "/original-idle.anim";
                WriteAsset(clipPath, "AnimationClip", 74, 7400000, clipRaw);
                string avatarPath = AssetRoot + "/original-avatar.asset";
                WriteAsset(avatarPath, "Avatar", 90, 9000000, avatarRaw);
                AssetDatabase.Refresh(ImportAssetOptions.ForceSynchronousImport);
                var clip = AssetDatabase.LoadAssetAtPath<AnimationClip>(clipPath);
                if (!clip) throw new InvalidDataException("Original AnimationClip YAML could not be loaded.");
                if (!clip.humanMotion)
                {
                    report["clipMode"] = "Original runtime payload not editor-readable; original dense/constant values expanded as Animator editor curves";
                    clip = ExpandMuscles(clipRaw, avatarRaw);
                    AssetDatabase.CreateAsset(clip, AssetDatabase.GenerateUniqueAssetPath(AssetRoot + "/expanded-original-idle.anim"));
                }
                else report["clipMode"] = "Original runtime payload imported";
                report["clip"] = new JObject { ["name"] = clip.name, ["humanMotion"] = clip.humanMotion,
                    ["length"] = clip.length, ["frameRate"] = clip.frameRate,
                    ["sourceMuscleClipSize"] = clipRaw["m_MuscleClipSize"] };
                report["probeLimitations"] = "Dense/constant fallback preserves Animator scalars and exact-path Transform curves. Events are retained in source evidence, not invoked on this isolated skeleton. Original Unity 6000 -> 2022 native compatibility still requires verification.";
                File.WriteAllText(Path.Combine(input,"clip-readback.json"), EditorJsonUtility.ToJson(clip, true));
                if (!clip.humanMotion || clip.length <= 0)
                    throw new InvalidDataException("Original compressed humanoid payload did not survive import; do not bake a static pose.");

                var desc = (JObject)avatarRaw["m_HumanDescription"];
                var skeleton = (JArray)desc["m_Skeleton"];
                var objects = new Dictionary<string, GameObject>();
                foreach (JObject bone in skeleton)
                {
                    string name = (string)bone["m_Name"];
                    if (objects.ContainsKey(name)) throw new InvalidDataException("Ambiguous original skeleton name: " + name);
                    objects[name] = new GameObject(name);
                }
                foreach (JObject bone in skeleton)
                {
                    var obj = objects[(string)bone["m_Name"]];
                    string parent = (string)bone["m_ParentName"];
                    if (!string.IsNullOrEmpty(parent)) obj.transform.SetParent(objects[parent].transform, false);
                    else if (!root) root = obj;
                    else throw new InvalidDataException("Multiple original skeleton roots.");
                    obj.transform.localPosition = V(bone["m_Position"]);
                    obj.transform.localRotation = Q(bone["m_Rotation"]);
                    obj.transform.localScale = V(bone["m_Scale"]);
                }
                var avatar = AssetDatabase.LoadAssetAtPath<Avatar>(avatarPath);
                report["rawAvatarValid"] = avatar && avatar.isValid && avatar.isHuman;
                if (!avatar || !avatar.isValid || !avatar.isHuman)
                {
                    avatar = AvatarBuilder.BuildHumanAvatar(root, Description(desc));
                    report["avatarMode"] = "HumanDescription reconstructed by current Unity AvatarBuilder; original compiled Avatar did not load";
                }
                else report["avatarMode"] = "Original compiled Avatar imported";
                if (!avatar || !avatar.isValid || !avatar.isHuman)
                    throw new InvalidDataException("Neither original Avatar nor original HumanDescription produced a valid human avatar.");
                File.WriteAllText(Path.Combine(input,"avatar-readback.json"), EditorJsonUtility.ToJson(avatar,true));
                var animator = root.AddComponent<Animator>(); animator.avatar = avatar;
                animator.cullingMode = AnimatorCullingMode.AlwaysAnimate;
                graph = PlayableGraph.Create("OriginalHumanoidPoseProbe");
                graph.SetTimeUpdateMode(DirectorUpdateMode.Manual);
                var output = AnimationPlayableOutput.Create(graph, "Original", animator);
                var playable = AnimationClipPlayable.Create(graph, clip);
                playable.SetApplyFootIK(false); // Do not add optional foot IK to the source clip.
                output.SetSourcePlayable(playable); graph.Play();
                var samples = new JArray();
                foreach (double time in new[] { 0.0, Math.Min(.25, clip.length * .25), Math.Min(.5, clip.length * .5) })
                {
                    playable.SetTime(time); graph.Evaluate(0);
                    var transforms = new JArray();
                    foreach (var pair in objects)
                    {
                        var tr = pair.Value.transform;
                        transforms.Add(new JObject { ["name"] = pair.Key, ["parent"] = tr.parent ? tr.parent.name : "",
                            ["position"] = Vec(tr.localPosition), ["rotation"] = new JArray(tr.localRotation.x,tr.localRotation.y,tr.localRotation.z,tr.localRotation.w),
                            ["scale"] = Vec(tr.localScale), ["worldPosition"] = Vec(tr.position) });
                    }
                    var pose = new HumanPose();
                    using (var handler = new HumanPoseHandler(avatar, root.transform)) handler.GetHumanPose(ref pose);
                    samples.Add(new JObject { ["time"] = time, ["transforms"] = transforms,
                        ["muscles"] = new JArray(pose.muscles), ["bodyPosition"] = Vec(pose.bodyPosition) });
                }
                report["samples"] = samples; report["status"] = "SAMPLED_REQUIRES_SOURCE_READBACK_AND_VISUAL_REVIEW";
            }
            catch (Exception error) { report["status"] = "PENDING_ORIGINAL"; report["error"] = error.ToString(); Debug.LogError(error); }
            finally
            {
                if (graph.IsValid()) graph.Destroy();
                if (root) UnityEngine.Object.DestroyImmediate(root);
                File.WriteAllText(Path.Combine(input,"native-pose-probe.json"), report.ToString());
            }
        }
        static JArray Vec(Vector3 v) => new JArray(v.x,v.y,v.z);
        internal static JArray LastUnboundTransformBindings { get; private set; } = new JArray();
        internal static AnimationClip ExpandMuscles(JObject raw, JObject avatar, HashSet<uint> originalHierarchyHashes = null)
        {
            LastSourceCurveBindings = new JArray();
            LastUnboundTransformBindings = new JArray();
            // Independent parser precedent: uTinyRipper AnimationClipConverter
            // AddAnimatorMuscleCurve passes the stored scalar value unchanged to
            // the Animator property named by HumanoidMuscleType.ToAttributeString.
            // This expands compression; Unity, not this probe, solves humanoid pose.
            var mc = raw["m_MuscleClip"]; var packed = mc["m_Clip"]["data"];
            int streamedCount=(int)packed["m_StreamedClip"]["curveCount"];
            var dense = packed["m_DenseClip"]; var constants = packed["m_ConstantClip"]["data"];
            int count = (int)dense["m_CurveCount"], frames = (int)dense["m_FrameCount"];
            float rate = (float)dense["m_SampleRate"], start = (float)dense["m_BeginTime"], stop = (float)mc["m_StopTime"];
            var samples = (JArray)dense["m_SampleArray"];
            var streamed=StreamedCurves(packed["m_StreamedClip"],stop);
            var clip = new AnimationClip { name = (string)raw["m_Name"], frameRate = (float)raw["m_SampleRate"], legacy = false };
            AnimationCurve Curve(int index)
            {
                if(index<streamedCount)
                {
                    if(!streamed.TryGetValue(index,out var original))throw new InvalidDataException("Missing streamed curve "+index);
                    return original;
                }
                index-=streamedCount;
                var keys = new List<Keyframe>();
                if (index < count)
                    for (int frame = 0; frame < frames; frame++)
                    {
                        float time=start+frame/rate;
                        if (time > stop+1e-6f) break; // Dense payload includes a padding frame beyond authored stop.
                        keys.Add(new Keyframe(Math.Min(time,stop), (float)samples[frame * count + index]));
                    }
                else
                {
                    float value = (float)constants[index-count];
                    keys.Add(new Keyframe(0,value)); keys.Add(new Keyframe(stop,value));
                }
                var curve = new AnimationCurve(keys.ToArray());
                for (int k=0;k<curve.length;k++)
                {
                    AnimationUtility.SetKeyLeftTangentMode(curve,k,AnimationUtility.TangentMode.Linear);
                    AnimationUtility.SetKeyRightTangentMode(curve,k,AnimationUtility.TangentMode.Linear);
                }
                return curve;
            }
            var slots = (JArray)mc["m_IndexArray"];
            void Install(EditorCurveBinding binding,int sourceIndex)
            {
                AnimationUtility.SetEditorCurve(clip,binding,Curve(sourceIndex));
                LastSourceCurveBindings.Add(new JObject { ["sourceCurveIndex"]=sourceIndex,
                    ["path"]=binding.path,["propertyName"]=binding.propertyName,["type"]=binding.type.Name });
            }
            for (int slot = 0; slot < slots.Count; slot++)
                if ((int)slots[slot] >= 0)
                    Install(EditorCurveBinding.FloatCurve("",typeof(Animator),MuscleAttribute(slot)),(int)slots[slot]);
            var paths = ((JArray)avatar["m_TOS"]).ToDictionary(t => unchecked((uint)(long)(t is JObject ? t["first"] : t[0])), t => (string)(t is JObject ? t["second"] : t[1]));
            int cursor=0;
            foreach (var binding in (JArray)raw["m_ClipBindingConstant"]["genericBindings"])
            {
                int type=(int)binding["typeID"], attribute=(int)binding["attribute"];
                int dimensions=type==4 ? (attribute==2 ? 4 : 3) : 1;
                if (type==4)
                {
                    if (attribute<1 || attribute>3) throw new NotSupportedException("Transform attribute "+attribute);
                    uint hash=(uint)binding["path"];
                    if (!paths.TryGetValue(hash,out var path))
                    {
                        if(originalHierarchyHashes==null || originalHierarchyHashes.Contains(hash))
                            throw new InvalidDataException("Unresolved original transform CRC "+hash);
                        LastUnboundTransformBindings.Add(new JObject { ["pathCRC"]=hash,["attribute"]=attribute,
                            ["sourceCurveIndex"]=cursor,["dimensions"]=dimensions,
                            ["reason"]="Absent from original Avatar m_TOS and every original prefab Transform path suffix; no target invented" });
                        cursor+=dimensions;continue;
                    }
                    string property=attribute==1 ? "m_LocalPosition." : attribute==2 ? "m_LocalRotation." : "m_LocalScale.";
                    for (int component=0;component<dimensions;component++)
                        Install(EditorCurveBinding.FloatCurve(path,typeof(Transform),property+"xyzw"[component]),cursor+component);
                }
                else if (type!=95) throw new NotSupportedException("Original non-Animator curve type "+type);
                cursor+=dimensions;
            }
            var settings = AnimationUtility.GetAnimationClipSettings(clip);
            settings.startTime = (float)mc["m_StartTime"]; settings.stopTime = stop;
            settings.loopTime = (bool)mc["m_LoopTime"]; settings.loopBlend = (bool)mc["m_LoopBlend"];
            settings.loopBlendOrientation = (bool)mc["m_LoopBlendOrientation"];
            settings.loopBlendPositionY = (bool)mc["m_LoopBlendPositionY"];
            settings.loopBlendPositionXZ = (bool)mc["m_LoopBlendPositionXZ"];
            settings.orientationOffsetY = (float)mc["m_OrientationOffsetY"];
            settings.level = (float)mc["m_Level"]; settings.cycleOffset = (float)mc["m_CycleOffset"];
            settings.mirror = (bool)mc["m_Mirror"];
            settings.keepOriginalOrientation = (bool)mc["m_KeepOriginalOrientation"];
            settings.keepOriginalPositionY = (bool)mc["m_KeepOriginalPositionY"];
            settings.keepOriginalPositionXZ = (bool)mc["m_KeepOriginalPositionXZ"];
            settings.heightFromFeet = (bool)mc["m_HeightFromFeet"];
            AnimationUtility.SetAnimationClipSettings(clip,settings);
            return clip;
        }
        sealed class StreamKey
        {
            public float time,value,x,y,z;
            public float Value(float t) { float d=t-time;return value+z*d+y*d*d+x*d*d*d; }
            public float Slope(float t) { float d=t-time;return z+2*y*d+3*x*d*d; }
        }
        static Dictionary<int,AnimationCurve> StreamedCurves(JToken source,float stop)
        {
            // Original streamed representation v(t)=value+z*dt+y*dt²+x*dt³.
            // Encoder/decoder independently documented by uTinyRipper's
            // Parser/Classes/AnimationClip/Editor/StreamedCurveKey.cs.
            var words=(JArray)source["data"];var keys=new Dictionary<int,List<StreamKey>>();
            var warmup=new Dictionary<int,StreamKey>();
            float Float(int index)=>BitConverter.ToSingle(BitConverter.GetBytes((uint)words[index]),0);
            for(int cursor=0;cursor<words.Count;)
            {
                float time=Float(cursor);int count=(int)words[cursor+1];cursor+=2;
                if(count<0 || cursor+count*5>words.Count)throw new InvalidDataException("Invalid streamed frame length.");
                for(int k=0;k<count;k++,cursor+=5)
                {
                    int index=(int)words[cursor];
                    if(time<=-1e30f)
                    {
                        warmup[index]=new StreamKey {time=time,x=Float(cursor+1),y=Float(cursor+2),z=Float(cursor+3),value=Float(cursor+4)};
                        continue;
                    }
                    if(time>=1e30f)continue;
                    if(!keys.TryGetValue(index,out var list))keys[index]=list=new List<StreamKey>();
                    list.Add(new StreamKey { time=time,x=Float(cursor+1),y=Float(cursor+2),z=Float(cursor+3),value=Float(cursor+4) });
                }
            }
            var result=new Dictionary<int,AnimationCurve>();
            foreach(var pair in keys)
            {
                var original=pair.Value.OrderBy(k=>k.time).ToList();
                if(original[0].time>0)
                {
                    // Keep the authored positive timestamp. Native curve pre-wrap is
                    // constant only when the original warm-up payload proves that value.
                    if(!warmup.TryGetValue(pair.Key,out var before) || before.x!=0 || before.y!=0 || before.z!=0 || before.value!=original[0].value)
                        throw new NotSupportedException("Streamed curve pre-origin value is not proven constant.");
                    original.Insert(0,new StreamKey {time=0,value=before.value});
                }
                var sample=original.Where(k=>k.time<=stop).ToList();
                if(sample.Count==0)throw new InvalidDataException("No streamed keys in clip range.");
                var last=sample[sample.Count-1];
                if(last.time<stop)sample.Add(new StreamKey { time=stop,value=last.Value(stop),z=last.Slope(stop) });
                var frames=sample.Select(k=>new Keyframe(k.time,k.value,0,k.z)).ToArray();
                for(int i=1;i<frames.Length;i++)
                {
                    var prev=sample[i-1];float dt=sample[i].time-prev.time;
                    if(dt<=0)throw new InvalidDataException("Duplicate streamed key time.");
                    if(prev.x==0 && prev.y==0 && prev.z==0)
                    {
                        frames[i-1].outTangent=float.PositiveInfinity;frames[i].inTangent=float.PositiveInfinity;
                    }
                    else
                    {
                        // Invert the original Hermite coefficient construction.
                        float delta=sample[i].value-prev.value;
                        frames[i].inTangent=(3*delta-2*prev.z*dt-prev.y*dt*dt)/dt;
                    }
                }
                var curve=new AnimationCurve(frames);
                // Verify imported cubic against the original payload within each segment.
                for(int i=0;i<sample.Count-1;i++)
                    foreach(float fraction in new[]{.25f,.5f,.75f})
                    {
                        float t=Mathf.Lerp(sample[i].time,sample[i+1].time,fraction);
                        float expected=sample[i].Value(t),actual=curve.Evaluate(t);
                        if(Mathf.Abs(expected-actual)>1e-4f*Mathf.Max(1,Mathf.Abs(expected)))
                            throw new InvalidDataException("Streamed tangent round-trip differs on curve "+pair.Key);
                    }
                result[pair.Key]=curve;
            }
            return result;
        }
        static string MuscleAttribute(int slot)
        {
            // uTinyRipper HumanoidMuscleType/LimbType/FingerDoFType enum mappings.
            string[] suffix = { "T.x","T.y","T.z","Q.x","Q.y","Q.z","Q.w" };
            if (slot < 7) return "Motion" + suffix[slot];
            if (slot < 14) return "Root" + suffix[slot-7];
            if (slot < 42) return new[]{"LeftFoot","RightFoot","LeftHand","RightHand"}[(slot-14)/7]+suffix[(slot-14)%7];
            if (slot < 97) return HumanTrait.MuscleName[slot-42];
            if (slot < 137)
            {
                int finger=slot-97;
                return (finger<20 ? "LeftHand." : "RightHand.")+new[]{"Thumb","Index","Middle","Ring","Little"}[(finger%20)/4]+"."+new[]{"1 Stretched","Spread","2 Stretched","3 Stretched"}[finger%4];
            }
            throw new NotSupportedException("Original TDoF channel requires exact editor binding mapping: "+slot);
        }
        internal static HumanDescription Description(JObject d) => new HumanDescription {
            human = ((JArray)d["m_Human"]).Select(t => new HumanBone {
                boneName = (string)t["m_BoneName"], humanName = (string)t["m_HumanName"],
                limit = new HumanLimit { useDefaultValues = !(bool)t["m_Limit"]["m_Modified"],
                    min = V(t["m_Limit"]["m_Min"]), max = V(t["m_Limit"]["m_Max"]),
                    center = V(t["m_Limit"]["m_Value"]), axisLength = (float)t["m_Limit"]["m_Length"] }
            }).ToArray(),
            skeleton = ((JArray)d["m_Skeleton"]).Select(t => new SkeletonBone { name = (string)t["m_Name"],
                position = V(t["m_Position"]), rotation = Q(t["m_Rotation"]), scale = V(t["m_Scale"]) }).ToArray(),
            upperArmTwist = (float)d["m_ArmTwist"], lowerArmTwist = (float)d["m_ForeArmTwist"],
            upperLegTwist = (float)d["m_UpperLegTwist"], lowerLegTwist = (float)d["m_LegTwist"],
            armStretch = (float)d["m_ArmStretch"], legStretch = (float)d["m_LegStretch"],
            feetSpacing = (float)d["m_FeetSpacing"], hasTranslationDoF = (bool)d["m_HasTranslationDoF"]
        };
        internal static void WriteAsset(string path, string type, int classId, int fileId, JObject raw)
        {
            var builder = new StringBuilder("%YAML 1.1\n%TAG !u! tag:unity3d.com,2011:\n--- !u!" + classId + " &" + fileId + "\n" + type + ":\n");
            builder.Append("  m_ObjectHideFlags: 0\n");
            Emit(builder, raw, 2); File.WriteAllText(path, builder.ToString());
        }
        static void Emit(StringBuilder b, JToken token, int indent)
        {
            string pad = new string(' ', indent);
            if (token is JObject obj)
                foreach (var p in obj.Properties())
                {
                    b.Append(pad).Append(p.Name).Append(':');
                    Value(b,p.Value,indent);
                }
            else if (token is JArray array)
                foreach (var item in array)
                {
                    if (item is JObject itemObject && itemObject.HasValues)
                    {
                        var nested = new StringBuilder(); Emit(nested,item,indent+2);
                        b.Append(pad).Append("- ").Append(nested.ToString().Substring(indent+2));
                    }
                    else { b.Append(pad).Append('-'); Value(b,item,indent); }
                }
        }
        static void Value(StringBuilder b, JToken value, int indent)
        {
            if (value is JObject obj && obj["m_FileID"] != null && obj["m_PathID"] != null)
            {
                if ((long)obj["m_PathID"] != 0) throw new NotSupportedException("Non-null original PPtr needs exact resolution.");
                b.Append(" {fileID: 0}\n");
            }
            else if (value is JContainer container && container.HasValues) { b.Append('\n'); Emit(b,value,indent+2); }
            else b.Append(' ').Append(value.Type == JTokenType.Boolean ? ((bool)value ? "1" : "0") : value.ToString(Formatting.None)).Append('\n');
        }
    }
}
#endif
