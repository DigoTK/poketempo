param([string]$GameDir = '', [string]$ZigPath = '')
$ErrorActionPreference = 'Stop'
Set-Location $PSScriptRoot
if (!$GameDir -and (Test-Path -LiteralPath 'installation.json')) { $GameDir = (Get-Content installation.json -Raw | ConvertFrom-Json).gameDir }
$zig = $ZigPath
if (!$zig -and $GameDir) { $zig = Join-Path $GameDir 'tools\build\zig\zig.exe' }
if (!$zig) { throw 'Pass -GameDir <Recompia folder> or -ZigPath <zig.exe>.' }
if (!(Test-Path -LiteralPath $zig)) { throw 'Zig not found in the game tools folder.' }
New-Item -ItemType Directory -Force -Path 'vendor\modtopia','dist\poketempo\bin\windows-x64','build' | Out-Null
if (!(Test-Path -LiteralPath 'vendor\modtopia\modtopia.h')) { throw 'Modtopia SDK header is missing from vendor/modtopia.' }
& $zig c++ -std=c++17 -O2 -UNDEBUG -Wall -Wextra -Werror -Isrc -Ivendor tests/clock_test.cpp -o build/clock_test.exe
if ($LASTEXITCODE) { throw 'Clock test build failed' }
& .\build\clock_test.exe
if ($LASTEXITCODE) { throw 'Clock tests failed' }
& $zig c++ -std=c++17 -O2 -Wall -Wextra -Werror -shared -Ivendor src/mod.cpp -o dist/poketempo/bin/windows-x64/mod.dll
if ($LASTEXITCODE) { throw 'Mod build failed' }
Copy-Item -LiteralPath mod.toml -Destination dist/poketempo/mod.toml
Copy-Item -LiteralPath LICENSE -Destination dist/poketempo/LICENSE
Copy-Item -LiteralPath vendor/modtopia/LICENSE -Destination dist/poketempo/LICENSE-Modtopia
& $zig c++ -std=c++17 -O2 -UNDEBUG -Wall -Wextra -Werror -Ivendor tests/mod_host.cpp -o build/mod_host.exe
if ($LASTEXITCODE) { throw 'Host test build failed' }
$testDir = Join-Path $PSScriptRoot ('build\profile-' + [guid]::NewGuid().ToString('N'))
New-Item -ItemType Directory $testDir | Out-Null
$modLibrary = Join-Path $PSScriptRoot 'dist\poketempo\bin\windows-x64\mod.dll'
& .\build\mod_host.exe $modLibrary $testDir seed
if ($LASTEXITCODE) { throw 'DLL integration test failed' }
& .\build\mod_host.exe $modLibrary $testDir resume
if ($LASTEXITCODE) { throw 'DLL persistence test failed' }
Write-Host 'Mod built: dist/poketempo'
