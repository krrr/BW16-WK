param(
    [switch]$Upload,
    [switch]$Monitor,
    [switch]$Ota,
    [string]$OtaIp = "192.168.4.1"
)

& (Join-Path $PSScriptRoot "build-all.ps1") @PSBoundParameters
