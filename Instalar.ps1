param([string]$GameDir = '')
$ErrorActionPreference = 'Stop'
if (Get-Process pkp -ErrorAction SilentlyContinue) { throw 'Feche o jogo antes de instalar / Close the game before installing.' }
if (!$GameDir -and (Test-Path -LiteralPath (Join-Path $PSScriptRoot 'installation.json'))) { $GameDir = (Get-Content (Join-Path $PSScriptRoot 'installation.json') -Raw | ConvertFrom-Json).gameDir }
if (!$GameDir) { $GameDir = (Read-Host 'Pasta Recompia com pkp.exe / Recompia folder containing pkp.exe').Trim('"') }
$gameRoot = (Resolve-Path -LiteralPath $GameDir).Path
if (!(Test-Path -LiteralPath (Join-Path $gameRoot 'pkp_rt.dll')) -or !(Test-Path -LiteralPath (Join-Path $gameRoot 'mods'))) { throw 'Pasta Recompia invalida / Invalid Recompia folder.' }
$source = Join-Path $PSScriptRoot 'dist\poketempo'
if (!(Test-Path -LiteralPath (Join-Path $source 'bin\windows-x64\mod.dll'))) { throw 'Compile o mod primeiro / Build the mod first.' }
$backupRoot = Join-Path $PSScriptRoot ('backups\' + (Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Path $backupRoot -Force | Out-Null
$savePath = Join-Path $gameRoot 'save'
if (Test-Path -LiteralPath $savePath) { Copy-Item -LiteralPath $savePath -Destination (Join-Path $backupRoot 'save') -Recurse }
$destination = Join-Path $gameRoot 'mods\poketempo'
if (Get-ChildItem -LiteralPath (Join-Path $gameRoot 'mods') -Filter 'poketempo-*.pkmod' -File) { throw 'Ja existe um .pkmod do PokeTempo. Use somente o .pkmod OU a pasta / Keep only the .pkmod OR the folder install.' }
if (Test-Path -LiteralPath $destination) { Copy-Item -LiteralPath $destination -Destination (Join-Path $backupRoot 'poketempo') -Recurse }
New-Item -ItemType Directory -Path (Join-Path $destination 'bin\windows-x64') -Force | Out-Null
Copy-Item -LiteralPath (Join-Path $source 'mod.toml') -Destination (Join-Path $destination 'mod.toml') -Force
Copy-Item -LiteralPath (Join-Path $source 'bin\windows-x64\mod.dll') -Destination (Join-Path $destination 'bin\windows-x64\mod.dll') -Force
Copy-Item -LiteralPath (Join-Path $source 'LICENSE'),(Join-Path $source 'LICENSE-Modtopia') -Destination $destination -Force
$deployment = @{gameDir=$gameRoot} | ConvertTo-Json
[IO.File]::WriteAllText((Join-Path $PSScriptRoot 'installation.json'), $deployment, [Text.UTF8Encoding]::new($false))
Write-Host ('Instalado / Installed: ' + $destination)
Write-Host ('Backup: ' + $backupRoot)
Write-Host 'Launcher: Mods > PokeTempo > permitir codigo / allow code. Set up again if requested.'
