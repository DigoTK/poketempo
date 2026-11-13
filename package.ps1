param([string]$PythonPath = '')
$ErrorActionPreference = 'Stop'
Set-Location $PSScriptRoot
if (!$PythonPath -and (Test-Path -LiteralPath installation.json)) {
    $gameDir = (Get-Content installation.json -Raw | ConvertFrom-Json).gameDir
    if ($gameDir) { $PythonPath = Join-Path $gameDir 'tools\python\python.exe' }
}
if (!$PythonPath -or !(Test-Path -LiteralPath $PythonPath)) { $PythonPath = (Get-Command python -ErrorAction Stop).Source }
& $pythonPath (Join-Path $PSScriptRoot 'tools\package.py')
if ($LASTEXITCODE) { throw 'Packaging failed' }
