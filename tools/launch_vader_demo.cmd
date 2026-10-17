@echo off
setlocal
set "DEMO_EXE=%~dp0..\unity\build\SWGoH-Restored.exe"
if not exist "%DEMO_EXE%" (
  echo Build missing: %DEMO_EXE%
  pause
  exit /b 1
)
start "SWGoH Vader Demo" "%DEMO_EXE%" --vader-demo -screen-fullscreen 0 -screen-width 1600 -screen-height 900 -logFile "%~dp0..\logs\vader_complete\demo-player.log"
