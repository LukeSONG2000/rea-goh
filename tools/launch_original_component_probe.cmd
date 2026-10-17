@echo off
setlocal
set "PROBE_ROOT=%~dp0.."
set "PROBE_EXE=%PROBE_ROOT%\logs\high_reuse\components\build\OriginalComponentProbe.exe"
if not exist "%PROBE_EXE%" (
  echo Original component probe has not been built. See tools/runtime_reuse/component_probe/README.md.
  pause
  exit /b 1
)
start "Original Unity component preview" "%PROBE_EXE%" --probe-output "%PROBE_ROOT%\logs\high_reuse\components\interactive" -logFile "%PROBE_ROOT%\logs\high_reuse\components\interactive-player.log"
endlocal
