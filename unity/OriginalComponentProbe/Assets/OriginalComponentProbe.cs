using System;
using System.IO;
using System.Linq;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Playables;
using UnityEngine.Animations;

// Diagnostics only: no replacement game scripts, shaders, rig, or animation export.
public class OriginalComponentProbe : MonoBehaviour
{
    [Serializable] public class Source { public string id,path,sha256,cab,version; public string[] dependencies,prefabs; }
    [Serializable] public class Sources { public string requiredVersion; public Source[] bundles; }
    [Serializable] public class BundleResult { public string id; public bool loaded; public string error; public int assetCount; }
    [Serializable] public class ClipResult { public string name; public float length; public bool humanMotion; public int bonesChanged; public string error; }
    [Serializable] public class AnimatorResult { public string path,avatar,controller,originalCullingMode; public bool valid,human,initialized,originalEnabled; public int clips,bonesChanged; public string error; public List<ClipResult> clipSamples=new(); }
    [Serializable] public class ShaderResult { public string material,shader; public bool supported; public int passes; public string[] textures; }
    [Serializable] public class ParticleResult { public string path; public int particles; public bool renderer,material,mesh; public string error; }
    [Serializable] public class PrefabResult { public string source,asset; public bool loaded,originalRootActive; public int transforms,renderers,missingScripts,missingMeshes; public List<string> missingScriptPaths=new(),missingMeshPaths=new(); public List<AnimatorResult> animators=new(); public List<ShaderResult> materials=new(); public List<ParticleResult> particles=new(); public string error; }
    [Serializable] public class Report { public string unityVersion,graphicsDevice,graphicsApi; public bool exactVersion; public List<BundleResult> bundles=new(); public List<PrefabResult> prefabs=new(); public List<string> diagnostics=new(),previewControls=new(); public int frames; public bool completed; }
    readonly List<GameObject> instances=new();
    readonly List<AssetBundle> bundles=new();
    Report report=new(); Camera view; int selected; string output; float exitAfter,readyAt;
    PlayableGraph previewGraph; AnimationClipPlayable previewPlayable; AnimationClip previewClip; Vector2 scroll;
    static string PathOf(Transform t) => t.parent ? PathOf(t.parent)+"/"+t.name : t.name;
    void Log(string condition,string trace,LogType type) { if(type==LogType.Exception || type==LogType.Error || type==LogType.Warning) report.diagnostics.Add(type+": "+condition); }
    IEnumerator Start()
    {
        output=GetArg("--probe-output")??System.IO.Path.GetFullPath(System.IO.Path.Combine(Application.dataPath,"../../../../../logs/high_reuse/components/player"));
        Directory.CreateDirectory(output); float.TryParse(GetArg("--probe-exit-seconds"),out exitAfter);
        Application.logMessageReceived+=Log;
        report.unityVersion=Application.unityVersion;report.graphicsDevice=SystemInfo.graphicsDeviceName;report.graphicsApi=SystemInfo.graphicsDeviceType.ToString();
        var sources=JsonUtility.FromJson<Sources>(File.ReadAllText(System.IO.Path.Combine(Application.streamingAssetsPath,"sources.json")));
        report.exactVersion=Application.unityVersion==sources.requiredVersion;
        view=new GameObject("Diagnostic camera").AddComponent<Camera>();view.backgroundColor=new Color(.10f,.11f,.14f);view.clearFlags=CameraClearFlags.SolidColor;
        var light=new GameObject("Diagnostic light").AddComponent<Light>();light.type=LightType.Directional;light.transform.rotation=Quaternion.Euler(35,-35,0);
        // Load every dependency first; no assets are requested until all CABs are registered.
        foreach(var source in sources.bundles)
        {
            var row=new BundleResult{id=source.id};report.bundles.Add(row);
            try {var bundle=AssetBundle.LoadFromFile(source.path);bundles.Add(bundle);row.loaded=bundle;row.assetCount=bundle?bundle.GetAllAssetNames().Length:0;if(!bundle)row.error="LoadFromFile returned null";}
            catch(Exception e){bundles.Add(null);row.error=e.ToString();}
        }
        for(int i=0;i<sources.bundles.Length;i++)foreach(var asset in sources.bundles[i].prefabs)
        {
            var row=new PrefabResult{source=sources.bundles[i].id,asset=asset};report.prefabs.Add(row);
            try
            {
                var prefab=bundles[i]?bundles[i].LoadAsset<GameObject>(asset):null;row.loaded=prefab;if(!prefab)throw new Exception("Original prefab failed to load");
                var go=Instantiate(prefab);instances.Add(go);go.name=prefab.name;row.originalRootActive=go.activeSelf;go.SetActive(true);
                var ts=go.GetComponentsInChildren<Transform>(true);row.transforms=ts.Length;
                foreach(var t in ts)foreach(var c in t.GetComponents<Component>())if(!c){row.missingScripts++;row.missingScriptPaths.Add(PathOf(t));}
                var renderers=go.GetComponentsInChildren<Renderer>(true);row.renderers=renderers.Length;
                foreach(var r in renderers)
                {
                    if(r is SkinnedMeshRenderer sm && !sm.sharedMesh){row.missingMeshes++;row.missingMeshPaths.Add(PathOf(r.transform));}
                    if(r is MeshRenderer && (!r.GetComponent<MeshFilter>() || !r.GetComponent<MeshFilter>().sharedMesh)){row.missingMeshes++;row.missingMeshPaths.Add(PathOf(r.transform));}
                }
                foreach(var m in renderers.SelectMany(r=>r.sharedMaterials).Where(m=>m).Distinct())
                    row.materials.Add(new ShaderResult{material=m.name,shader=m.shader?m.shader.name:null,supported=m.shader && m.shader.isSupported,passes=m.passCount,textures=m.GetTexturePropertyNames().Select(n=>n+"="+(m.GetTexture(n)?m.GetTexture(n).name:"NULL")).ToArray()});
                foreach(var a in go.GetComponentsInChildren<Animator>(true))
                {
                    var ar=new AnimatorResult{path=PathOf(a.transform),avatar=a.avatar?a.avatar.name:null,valid=a.avatar && a.avatar.isValid,human=a.avatar && a.avatar.isHuman,controller=a.runtimeAnimatorController?a.runtimeAnimatorController.name:null,clips=a.runtimeAnimatorController?a.runtimeAnimatorController.animationClips.Length:0};row.animators.Add(ar);
                    ar.originalEnabled=a.enabled;ar.originalCullingMode=a.cullingMode.ToString();a.enabled=true;a.cullingMode=AnimatorCullingMode.AlwaysAnimate;
                    try {a.Rebind();a.Update(0);var before=ts.Select(t=>t.localPosition).ToArray();var rot=ts.Select(t=>t.localRotation).ToArray();a.Update(.5f);ar.initialized=a.isInitialized;ar.bonesChanged=ts.Where((t,j)=>Vector3.Distance(before[j],t.localPosition)>1e-6 || Quaternion.Angle(rot[j],t.localRotation)>.001f).Count();}
                    catch(Exception e){ar.error=e.ToString();}
                    if(a.runtimeAnimatorController && a.avatar && a.avatar.isValid)
                    foreach(var clip in a.runtimeAnimatorController.animationClips.Distinct())
                    {
                        var cr=new ClipResult{name=clip.name,length=clip.length,humanMotion=clip.humanMotion};ar.clipSamples.Add(cr);var graph=PlayableGraph.Create("Original clip verification");
                        try {graph.SetTimeUpdateMode(DirectorUpdateMode.Manual);var playable=AnimationClipPlayable.Create(graph,clip);var channel=AnimationPlayableOutput.Create(graph,"Original",a);channel.SetSourcePlayable(playable);graph.Play();graph.Evaluate(0);var before=ts.Select(t=>t.localPosition).ToArray();var rot=ts.Select(t=>t.localRotation).ToArray();graph.Evaluate(Mathf.Min(.5f,clip.length));cr.bonesChanged=ts.Where((t,j)=>Vector3.Distance(before[j],t.localPosition)>1e-6 || Quaternion.Angle(rot[j],t.localRotation)>.001f).Count();}
                        catch(Exception e){cr.error=e.ToString();}
                        finally {graph.Destroy();}
                    }
                    a.Rebind();a.Update(0);
                }
                foreach(var p in go.GetComponentsInChildren<ParticleSystem>(true))
                {
                    var pr=new ParticleResult{path=PathOf(p.transform)};row.particles.Add(pr);
                    try {p.Simulate(.5f,false,true,true);pr.particles=p.particleCount;var r=p.GetComponent<ParticleSystemRenderer>();pr.renderer=r;pr.material=r && r.sharedMaterial;pr.mesh=r && r.mesh;}
                    catch(Exception e){pr.error=e.ToString();}
                }
                go.SetActive(false);
            }
            catch(Exception e){row.error=e.ToString();}
        }
        if(instances.Count>0)Select(0);
        readyAt=Time.realtimeSinceStartup;report.completed=true;Save();Debug.Log("ORIGINAL_COMPONENT_PROBE_COMPLETE "+output);
        yield return new WaitForSeconds(3);
        for(int i=0;i<instances.Count;i++){Select(i);yield return null;Capture("direct-original-"+i+".png");}Select(0);Save();
    }
    void Select(int i)
    {
        if(previewGraph.IsValid())previewGraph.Destroy();previewClip=null;
        foreach(var go in instances)go.SetActive(false);selected=i;var current=instances[i];current.SetActive(true);
        var rs=current.GetComponentsInChildren<Renderer>();var b=new Bounds(current.transform.position,Vector3.one);
        // Diagnostic framing uses the character's physical meshes, not oversized
        // VFX simulation bounds. Ability-only prefabs use their particle bounds.
        bool character=rs.Any(r=>r is SkinnedMeshRenderer);
        foreach(var r in rs)if(r.enabled && (!character || !(r is ParticleSystemRenderer)))b.Encapsulate(r.bounds);
        var center=b.center;var size=Mathf.Max(1,b.extents.magnitude);view.transform.position=center+new Vector3(0,size*.25f,size*2.5f);view.transform.LookAt(center);view.nearClipPlane=.01f;view.farClipPlane=1000;
        ReplayParticles();
    }
    void ReplayParticles(){foreach(var p in instances[selected].GetComponentsInChildren<ParticleSystem>()){p.Simulate(0,false,true,true);p.Play(false);}}
    void PreviewAnimation(AnimationClip clip)
    {
        report.previewControls.Add("clip:"+clip.name);Save();
        if(previewGraph.IsValid())previewGraph.Destroy();previewClip=clip;
        var animator=instances[selected].GetComponentInChildren<Animator>();
        previewGraph=PlayableGraph.Create("Diagnostic original clip loop");previewPlayable=AnimationClipPlayable.Create(previewGraph,clip);
        var channel=AnimationPlayableOutput.Create(previewGraph,"Original animation preview",animator);channel.SetSourcePlayable(previewPlayable);previewGraph.Play();
    }
    void Update()
    {
        report.frames++;
        if(previewGraph.IsValid() && previewClip && previewClip.length>0 && previewPlayable.GetTime()>=previewClip.length)previewPlayable.SetTime(previewPlayable.GetTime()%previewClip.length);
        if(report.completed && exitAfter>0 && Time.realtimeSinceStartup-readyAt>exitAfter){Save();Application.Quit();}
    }
    void Save(){if(output!=null)File.WriteAllText(System.IO.Path.Combine(output,"report.json"),JsonUtility.ToJson(report,true));}
    void Capture(string name)
    {
        var rt=new RenderTexture(1280,720,24);view.targetTexture=rt;view.Render();var old=RenderTexture.active;RenderTexture.active=rt;
        var image=new Texture2D(1280,720,TextureFormat.RGB24,false);image.ReadPixels(new Rect(0,0,1280,720),0,0);image.Apply();File.WriteAllBytes(System.IO.Path.Combine(output,name),image.EncodeToPNG());
        RenderTexture.active=old;view.targetTexture=null;Destroy(image);rt.Release();Destroy(rt);
    }
    void OnGUI()
    {
        var evt=Event.current;
        if(evt.type==EventType.KeyDown)
        {
            int index=(int)evt.keyCode-(int)KeyCode.Alpha1;
            if(index>=0 && index<instances.Count){report.previewControls.Add("key:"+evt.keyCode);Select(index);Save();evt.Use();}
            else if(evt.keyCode==KeyCode.R && instances.Count>0){report.previewControls.Add("key:R");ReplayParticles();Save();evt.Use();}
            else if(evt.keyCode==KeyCode.Escape){Application.Quit();evt.Use();}
        }
        GUILayout.BeginArea(new Rect(12,12,350,Mathf.Min(690,Screen.height-24)),GUI.skin.box);
        scroll=GUILayout.BeginScrollView(scroll);
        GUILayout.Label("Original AssetBundle component probe / "+Application.unityVersion);
        GUILayout.Label("Original mesh / Avatar / Animator / particles / shaders. Game scripts are NOT replaced.");
        GUILayout.Label("Component preview, not battle. Original scene lighting is NOT restored; this is not 1:1 rendering.");
        GUILayout.Label("1-5: select original prefab | R: replay particles | Esc: close");
        for(int i=0;i<instances.Count;i++)if(GUILayout.Button((i+1)+"  "+instances[i].name))Select(i);
        if(instances.Count>0)
        {
            var animator=instances[selected].GetComponentInChildren<Animator>();
            if(animator && animator.runtimeAnimatorController)
            {
                GUILayout.Label("Original clips (diagnostic loop; no ability execution):");
                foreach(var clip in animator.runtimeAnimatorController.animationClips.Distinct())if(GUILayout.Button((previewClip==clip?"> ":"")+clip.name))PreviewAnimation(clip);
                if(GUILayout.Button("Return to original controller")){if(previewGraph.IsValid())previewGraph.Destroy();previewClip=null;animator.Rebind();}
            }
            else if(GUILayout.Button("Replay original active particle systems"))ReplayParticles();
        }
        if(report.completed)GUILayout.Label("Missing scripts: "+report.prefabs.Sum(p=>p.missingScripts)+"; report: "+output);
        GUILayout.EndScrollView();
        GUILayout.EndArea();
    }
    static string GetArg(string name){var a=Environment.GetCommandLineArgs();int i=Array.IndexOf(a,name);return i>=0 && i+1<a.Length?a[i+1]:null;}
}
