$ErrorActionPreference='Stop'
$repoRoot=(Resolve-Path (Join-Path $PSScriptRoot '../..')).Path
$targets=@(Get-Process SWGoH -ErrorAction SilentlyContinue | Where-Object {$_.Path -eq 'E:\Games\EA Games\SWGoH\SWGoH.exe'})
if($targets.Count -ne 1){throw 'Open one original client first.'}
$script=Join-Path $PSScriptRoot 'attribute_overlay.py'
Start-Process -FilePath (Get-Command py).Source -WorkingDirectory $repoRoot -ArgumentList @(('"'+$script+'"'),'--pid',$targets[0].Id) -WindowStyle Hidden
