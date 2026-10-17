param([int]$Seconds = 25)
$ErrorActionPreference = 'Stop'
$repo = (Resolve-Path (Join-Path $PSScriptRoot '../../..')).Path
$editor = 'F:\Unity\6000.3.17f1\Editor\Unity.exe'
$project = Join-Path $repo 'unity/OriginalComponentProbe'
$logs = Join-Path $repo 'logs/high_reuse/components'
if (Get-Process Unity -ErrorAction SilentlyContinue) { throw 'Another Unity Editor is running; wait for its owner.' }
if (Test-Path (Join-Path $repo 'unity/SWGoH-Restored/Temp/UnityLockfile')) { throw 'Restored Unity lock exists; wait for its owner.' }
if (Test-Path (Join-Path $project 'Temp/UnityLockfile')) { throw 'Probe Unity lock exists; investigate the owning process.' }
$build = Start-Process -FilePath $editor -ArgumentList @('-batchmode','-quit','-projectPath',('"'+$project+'"'),'-executeMethod','BuildComponentProbe.Build','-logFile',('"'+(Join-Path $logs 'editor-build.log')+'"')) -WindowStyle Hidden -PassThru
$build.WaitForExit()
if ($build.ExitCode -ne 0) { throw "Editor failed: $($build.ExitCode). See editor-build.log" }
$exe = Join-Path $logs 'build/OriginalComponentProbe.exe'
$player = Start-Process -FilePath $exe -ArgumentList @('--probe-output',('"'+(Join-Path $logs 'player')+'"'),'--probe-exit-seconds',"$Seconds",'-logFile',('"'+(Join-Path $logs 'player.log')+'"')) -WindowStyle Hidden -PassThru
$player.WaitForExit()
@{buildExitCode=$build.ExitCode;playerExitCode=$player.ExitCode;playerId=$player.Id} | ConvertTo-Json | Set-Content (Join-Path $logs 'process-result.json')
if ($player.ExitCode -ne 0) { throw "Player failed: $($player.ExitCode). See player.log" }
