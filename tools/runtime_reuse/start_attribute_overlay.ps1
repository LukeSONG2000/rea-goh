$ErrorActionPreference='Stop'
$repoRoot=(Resolve-Path (Join-Path $PSScriptRoot '../..')).Path
$targets=@(Get-Process SWGoH -ErrorAction SilentlyContinue | Where-Object {$_.Path -eq 'E:\Games\EA Games\SWGoH\SWGoH.exe'})
if($targets.Count -ne 1){throw 'Open one original client first.'}
$statusPath=Join-Path $repoRoot 'logs/high_reuse/battle-info-status.json'
$running=$false
if(Test-Path -LiteralPath $statusPath){$status=Get-Content -LiteralPath $statusPath -Raw | ConvertFrom-Json; $running=(!$status.detached -and $status.pid -eq $targets[0].Id -and [bool](Get-Process -Id $status.hostPid -ErrorAction SilentlyContinue))}
if(!$running){& (Join-Path $PSScriptRoot 'start_battle_info.ps1')}
$script=Join-Path $PSScriptRoot 'attribute_overlay.py'
Start-Process -FilePath (Get-Command py).Source -WorkingDirectory $repoRoot -ArgumentList @(('"'+$script+'"'),'--pid',$targets[0].Id) -WindowStyle Hidden
