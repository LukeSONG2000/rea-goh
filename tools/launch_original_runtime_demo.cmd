@echo off
setlocal
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0runtime_reuse\run_original_probe.ps1" -Mode visual
pause
