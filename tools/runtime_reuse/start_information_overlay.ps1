$ErrorActionPreference='Stop'
$repoRoot=(Resolve-Path (Join-Path $PSScriptRoot '../..')).Path
$targets=@(Get-Process SWGoH -ErrorAction SilentlyContinue | Where-Object {$_.Path -eq 'E:\Games\EA Games\SWGoH\SWGoH.exe'})
if($targets.Count -ne 1){throw 'Open one original client first.'}
$statusPath=Join-Path $repoRoot 'logs/high_reuse/battle-info-status.json'
$running=$false
if(Test-Path -LiteralPath $statusPath){$status=Get-Content -LiteralPath $statusPath -Raw | ConvertFrom-Json; $running=(!$status.detached -and $status.pid -eq $targets[0].Id -and [bool](Get-Process -Id $status.hostPid -ErrorAction SilentlyContinue))}
$trialStatus=Join-Path $repoRoot "logs/high_reuse/trial-status-$($targets[0].Id).json"
if(!$running -and (Test-Path -LiteralPath $trialStatus)){
 $trial=Get-Content -LiteralPath $trialStatus -Raw | ConvertFrom-Json
 $owner=Get-CimInstance Win32_Process -Filter "ProcessId=$($trial.hostPid)"
 if($owner -and !$trial.detached -and $trial.pid -eq $targets[0].Id -and $owner.CommandLine -like '*run_captured_trial.mjs*'){$running=$true}
}
if(!$running){& (Join-Path $PSScriptRoot 'start_battle_info.ps1')}
$script=Join-Path $PSScriptRoot 'information_overlay.py'
Start-Process -FilePath (Get-Command py).Source -WorkingDirectory $repoRoot -ArgumentList @(('"'+$script+'"'),'--pid',$targets[0].Id) -WindowStyle Hidden

