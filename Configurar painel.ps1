param([string]$GameDir = '')
$ErrorActionPreference = 'Stop'
if (!$GameDir) { $GameDir = (Read-Host 'Pasta Recompia com pkp.exe / Recompia folder containing pkp.exe').Trim('"') }
$gameRoot = (Resolve-Path -LiteralPath $GameDir).Path
if (!(Test-Path -LiteralPath (Join-Path $gameRoot 'pkp_rt.dll'))) { throw 'Pasta Recompia invalida / Invalid Recompia folder.' }
[IO.File]::WriteAllText((Join-Path $PSScriptRoot 'installation.json'), (@{gameDir=$gameRoot}|ConvertTo-Json), [Text.UTF8Encoding]::new($false))
Write-Host 'Painel configurado. Abra Abrir painel.cmd / Panel configured. Open Abrir painel.cmd.'
