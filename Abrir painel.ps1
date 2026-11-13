$ErrorActionPreference = 'Stop'
$panelUrl = 'http://127.0.0.1:43821'
if (!(Test-Path -LiteralPath (Join-Path $PSScriptRoot 'installation.json')) -or !(Get-Content (Join-Path $PSScriptRoot 'installation.json') -Raw | ConvertFrom-Json).gameDir) {
    & (Join-Path $PSScriptRoot 'Configurar painel.ps1')
}
$nodePath = (Get-Command node -ErrorAction SilentlyContinue).Source
if (!$nodePath) {
    $bundledNode = Join-Path $env:USERPROFILE '.cache\codex-runtimes\codex-primary-runtime\dependencies\node\bin\node.exe'
    if (Test-Path -LiteralPath $bundledNode) { $nodePath = $bundledNode }
}
if (!$nodePath) { Write-Host 'Instale Node.js 22+ / Install Node.js 22+ (nodejs.org).'; Read-Host 'Enter'; exit 1 }
$started = $false
try { $response = Invoke-RestMethod "$panelUrl/api/status" -TimeoutSec 2; $started = $null -ne $response.config } catch {}
if (!$started) {
    $scriptPath = Join-Path $PSScriptRoot 'panel\server.mjs'
    Start-Process -FilePath $nodePath -ArgumentList ('"' + $scriptPath + '"') -WorkingDirectory $PSScriptRoot -WindowStyle Hidden -RedirectStandardOutput (Join-Path $PSScriptRoot 'panel.log') -RedirectStandardError (Join-Path $PSScriptRoot 'panel-error.log')
    for ($attempt=0; $attempt -lt 30; $attempt++) {
        Start-Sleep -Milliseconds 200
        try { $response=Invoke-RestMethod "$panelUrl/api/status" -TimeoutSec 1; if ($null -ne $response.config) {$started=$true;break} } catch {}
    }
}
if (!$started) { throw 'Nao foi possivel abrir o painel / Could not start the panel. See panel-error.log.' }
Start-Process $panelUrl
