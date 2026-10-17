$ErrorActionPreference='Stop'
$repoRoot=(Resolve-Path (Join-Path $PSScriptRoot '../..')).Path
$outDir=Join-Path $repoRoot 'logs/high_reuse'
New-Item -ItemType Directory -Force $outDir | Out-Null
$targets=@(Get-Process SWGoH -ErrorAction SilentlyContinue | Where-Object {$_.Path -eq 'E:\Games\EA Games\SWGoH\SWGoH.exe'})
if($targets.Count -ne 1){throw 'Open the original supported client first.'}
$assembly=Join-Path (Split-Path $targets[0].Path) 'GameAssembly.dll'
if((Get-FileHash -Algorithm SHA256 $assembly).Hash.ToLowerInvariant() -ne '12cc254525a58aedea122a2967d0476da05b98ef0c219dc18cfc0b7207538db6'){throw 'Unsupported original client revision.'}
$statusPath=Join-Path $outDir 'battle-info-status.json'
if(Test-Path -LiteralPath $statusPath){
 $previous=Get-Content -LiteralPath $statusPath -Raw | ConvertFrom-Json
 if(!$previous.detached -and (Get-Process -Id $previous.hostPid -ErrorAction SilentlyContinue)){throw 'Battle information helper already running. Stop it before restarting.'}
}
$stopPath=Join-Path $outDir 'battle-info.stop'
if(Test-Path -LiteralPath $stopPath){Remove-Item -LiteralPath $stopPath}
$combined=Join-Path $outDir 'battle-info-combined.js'
$parts=@('runtime_api.js','unity_scene_api.js','battle_status_reader.js','battle_info_ui.js') | ForEach-Object {Join-Path $PSScriptRoot $_}
Get-Content $parts | Set-Content -Encoding utf8 $combined
$hostScript=Join-Path $PSScriptRoot 'run_battle_info.mjs'
$arguments=@(('"'+$hostScript+'"'),$targets[0].Id,('"'+$combined+'"'),('"'+$outDir+'"'))
$helper=Start-Process -FilePath (Get-Command node).Source -ArgumentList $arguments -WindowStyle Hidden -PassThru -RedirectStandardOutput (Join-Path $outDir 'battle-info-host.log') -RedirectStandardError (Join-Path $outDir 'battle-info-host-error.log')
Write-Host "Battle information display helper started: PID $($helper.Id). Long-press a unit to inspect."
Write-Host 'Stop with tools/stop_battle_info.cmd. Client restart requires starting this helper again.'
