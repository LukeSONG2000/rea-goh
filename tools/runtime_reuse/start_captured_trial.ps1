param(
 [Parameter(Mandatory=$true)][string]$Battle,
 [Parameter(Mandatory=$true)][string]$Inventory,
 [Parameter(Mandatory=$true)][ValidateRange(0,255)][int]$SquadType,
 [ValidateSet('visual','headless')][string]$Mode='visual',
 [ValidateRange(15,600)][int]$Duration=300,
 [switch]$Auto,
 [switch]$TraceAI,
 [switch]$EnemyBasicDemo,
 [switch]$EnemyControl
)
$ErrorActionPreference='Stop'
$repo=(Resolve-Path (Join-Path $PSScriptRoot '../..')).Path
$targets=@(Get-Process SWGoH -ErrorAction SilentlyContinue | Where-Object {$_.Path -eq 'E:\Games\EA Games\SWGoH\SWGoH.exe'})
if($targets.Count -ne 1){throw 'Exactly one supported original client is required'}
$gameId=$targets[0].Id
$out=Join-Path $repo 'logs/high_reuse'
$prefix=Join-Path $out ('trial-'+(Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
$battlePath=(Resolve-Path -LiteralPath $Battle).Path
$inventoryPath=(Resolve-Path -LiteralPath $Inventory).Path
$prepare=@((Join-Path $PSScriptRoot 'prepare_captured_trial.py'),'--battle',$battlePath,'--inventory',$inventoryPath,'--dump',(Join-Path $repo 'il2cpp/dump/cs/il2cpp.cs'),'--output-prefix',$prefix,'--squad-type',$SquadType,'--mode',$Mode,'--duration',$Duration)
if($Auto){$prepare+='--auto'}
if($TraceAI){$prepare+='--trace-ai'}
if($EnemyBasicDemo){$prepare+='--enemy-basic-demo'}
if($EnemyControl){$prepare+='--enemy-control'}
& py @prepare
if($LASTEXITCODE -ne 0){throw 'Trial preparation failed; original client untouched'}
& node --check ($prefix+'.js')
if($LASTEXITCODE -ne 0){throw 'Prepared script syntax check failed'}
$restoreDisplay=$false
try{
 $statusPath=Join-Path $out 'battle-info-status.json'
 if(Test-Path -LiteralPath $statusPath){
  $status=Get-Content -LiteralPath $statusPath -Raw -Encoding utf8 | ConvertFrom-Json
  $hostProcess=Get-CimInstance Win32_Process -Filter "ProcessId=$($status.hostPid)"
  if(!$status.detached -and $hostProcess){
   if($status.pid -ne $gameId -or $hostProcess.CommandLine -notlike '*tools\runtime_reuse\run_battle_info.mjs*'){throw 'Unrecognized live runtime owner; leave it untouched'}
   Set-Content -LiteralPath (Join-Path $out 'battle-info.stop') -Value stop
   $deadline=(Get-Date).AddSeconds(15)
   while((Get-Process -Id $status.hostPid -ErrorAction SilentlyContinue) -and (Get-Date) -lt $deadline){Start-Sleep -Milliseconds 200}
   if(Get-Process -Id $status.hostPid -ErrorAction SilentlyContinue){throw 'Display host still running; trial not attached'}
   $restoreDisplay=$true
  }
 }
 Write-Host "Trial uses captured snapshots. No borrowed support. Evidence: $prefix-evidence.json"
 if($Mode -eq 'visual'){Write-Host "Play with the original controls. This session returns Home after at most $Duration seconds."}
 & node (Join-Path $PSScriptRoot 'run_captured_trial.mjs') $gameId ($prefix+'.json') ($prefix+'-evidence.json')
 if($LASTEXITCODE -ne 0){throw "Trial validation failed; inspect $prefix-evidence.json"}
}finally{
 if($restoreDisplay -and (Get-Process -Id $gameId -ErrorAction SilentlyContinue)){
  $lock=Join-Path $out "runtime-session-$gameId.lock"
  if(Test-Path -LiteralPath $lock){Write-Warning 'Runtime ownership remains held; display host not restarted'}
  else{& (Join-Path $PSScriptRoot 'start_battle_info.ps1')}
 }
}
