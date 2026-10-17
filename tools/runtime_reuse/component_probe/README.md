# Direct original component probe

Independent project: `unity/OriginalComponentProbe`. Does not open or modify the
existing Unity 2022 restoration project. All original bundle files are read-only.

The probe loads original Windows64 Unity 6000.3.17f1 bundles and their exact CAB
dependencies, then instantiates the original character and four ability prefabs.
No GLB, extracted textures, recreated shaders, animation baking, replacement
game-script stubs, or serialized-field version translation is involved.

Runtime reports separately record bundle/prefab loading, missing script paths,
Avatar validity, original controller clips sampled through native Playables,
native bone changes, original particle simulation and shader support/texture
references. Diagnostic camera/light and prefab selection buttons are test harness
code. They do not reconstruct battle logic or imply missing FSM/Wwise scripts work.
Original inactive child states are retained. A selected diagnostic prefab root is
enabled explicitly to inspect its engine components.

Prepare local sources:

```
py tools/runtime_reuse/component_probe/prepare_sources.py
```

Exact Editor provenance:

- https://unity.com/releases/editor/whats-new/6000.3.17f1
- Official Windows installer revision: `cf0352b38e81`
- https://docs.unity.com/en-us/engine/6000.3/manual/get-started/install-and-upgrade/getting-started-installing-unity/installing-unity

Only after confirming no other Unity Editor session is running:

```
F:\Unity\6000.3.17f1\Editor\Unity.exe -batchmode -quit -projectPath F:\PPfavorite\Work\goh\unity\OriginalComponentProbe -executeMethod BuildComponentProbe.Build -logFile F:\PPfavorite\Work\goh\logs\high_reuse\components\editor-build.log
```

Build output is `logs/high_reuse/components/build/OriginalComponentProbe.exe`.
Run interactively to inspect the original components, or pass
`--probe-output <absolute directory> --probe-exit-seconds 25` for an automated
report and screenshot. This diagnostic executable reads local bundle paths from
`StreamingAssets/sources.json`; it does not contact game services.

Persistent preview: double-click `tools/launch_original_component_probe.cmd`.
Press 1–5 to select the original character or ability particle prefab; R restarts
its original active particle systems; Escape closes. Character clip buttons loop
the original native animation with a diagnostic PlayableGraph. They do not run
the ability FSM or deal damage. The original scene lighting is not restored;
the diagnostic camera/light preview must not be described as a 1:1 game image.
Automated runs still use `--probe-exit-seconds 25` and emit reports/screenshots.
