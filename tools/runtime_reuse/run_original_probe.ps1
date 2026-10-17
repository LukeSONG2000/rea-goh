param(
    [ValidateSet('headless','visual')][string]$Mode='headless',
    [int]$OriginalProcessId=0
)
$ErrorActionPreference='Stop'
$repoRoot=(Resolve-Path (Join-Path $PSScriptRoot '../..')).Path
$outDir=Join-Path $repoRoot 'logs/high_reuse'
New-Item -ItemType Directory -Force $outDir | Out-Null
if(!$OriginalProcessId){
    $targets=@(Get-Process SWGoH -ErrorAction SilentlyContinue | Where-Object {$_.Path -eq 'E:\Games\EA Games\SWGoH\SWGoH.exe'})
    if($targets.Count -ne 1){throw 'Open the original client and leave it at the Home screen first.'}
    $OriginalProcessId=$targets[0].Id
}
$target=Get-Process -Id $OriginalProcessId
if($target.Path -ne 'E:\Games\EA Games\SWGoH\SWGoH.exe'){throw 'This probe only supports the verified original client.'}
$assembly=Join-Path (Split-Path $target.Path) 'GameAssembly.dll'
if((Get-FileHash -Algorithm SHA256 $assembly).Hash.ToLowerInvariant() -ne '12cc254525a58aedea122a2967d0476da05b98ef0c219dc18cfc0b7207538db6'){
    throw 'Original GameAssembly revision changed; revalidate native contracts before running.'
}
$entry=if($Mode -eq 'visual'){'probe_visual.js'}else{'probe_simulation.js'}
$combined=Join-Path $outDir ('cli-'+$Mode+'.js')
$evidence=Join-Path $outDir ('cli-'+$Mode+'-evidence.json')
Get-Content @((Join-Path $PSScriptRoot 'runtime_api.js'),(Join-Path $PSScriptRoot 'offline_socket_guard.js'),(Join-Path $PSScriptRoot 'original_fixture.js'),(Join-Path $PSScriptRoot $entry)) | Set-Content -Encoding utf8 $combined
Write-Host "Running original $Mode probe in PID $OriginalProcessId. Keep the client open; validation detaches after original-state cleanup."
& node (Join-Path $PSScriptRoot 'run_until_complete.mjs') $OriginalProcessId $combined $evidence $Mode
if($LASTEXITCODE -ne 0){throw "REA probe failed; see $evidence"}
& py (Join-Path $PSScriptRoot 'summarize_runtime_probe.py') $evidence $Mode
if($LASTEXITCODE -ne 0){throw "Probe did not pass; see $evidence"}
