using System.IO;
using UnityEditor;
using UnityEditor.SceneManagement;
using UnityEditor.Build.Reporting;
using UnityEngine;
public static class BuildComponentProbe
{
    public static void Build()
    {
        if(Application.unityVersion!="6000.3.17f1")throw new System.Exception("Exact original Editor required");
        var scene=EditorSceneManager.NewScene(NewSceneSetup.EmptyScene,NewSceneMode.Single);
        new GameObject("Direct original component probe").AddComponent<OriginalComponentProbe>();
        Directory.CreateDirectory("Assets/Scenes");EditorSceneManager.SaveScene(scene,"Assets/Scenes/Probe.unity");
        PlayerSettings.companyName="LocalOriginalComponentProbe";PlayerSettings.productName="OriginalComponentProbe";
        PlayerSettings.defaultScreenWidth=1280;PlayerSettings.defaultScreenHeight=720;PlayerSettings.fullScreenMode=FullScreenMode.Windowed;
        PlayerSettings.SetScriptingBackend(UnityEditor.Build.NamedBuildTarget.Standalone,ScriptingImplementation.Mono2x);
        PlayerSettings.SetGraphicsAPIs(BuildTarget.StandaloneWindows64,new[]{UnityEngine.Rendering.GraphicsDeviceType.Direct3D11});
        EditorUserBuildSettings.development=false;
        EditorUserBuildSettings.connectProfiler=false;
        EditorUserBuildSettings.allowDebugging=false;
        var path=Path.GetFullPath("../../logs/high_reuse/components/build/OriginalComponentProbe.exe");Directory.CreateDirectory(Path.GetDirectoryName(path));
        var report=BuildPipeline.BuildPlayer(new BuildPlayerOptions{scenes=new[]{"Assets/Scenes/Probe.unity"},locationPathName=path,target=BuildTarget.StandaloneWindows64,options=BuildOptions.None});
        if(report.summary.result!=BuildResult.Succeeded)throw new System.Exception("Build failed: "+report.summary.result);
        Debug.Log("ORIGINAL_COMPONENT_BUILD_OPTIONS "+report.summary.options+"; development="+EditorUserBuildSettings.development+"; connectProfiler="+EditorUserBuildSettings.connectProfiler+"; allowDebugging="+EditorUserBuildSettings.allowDebugging);
        Debug.Log("ORIGINAL_COMPONENT_BUILD_SUCCESS "+path);
    }
}
