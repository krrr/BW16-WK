param(
    [switch]$Upload,
    [switch]$Monitor
)

$root = Split-Path -Parent $MyInvocation.MyCommand.Path

Write-Host "=== 1. Build frontend ===" -ForegroundColor Cyan
Push-Location (Join-Path $root "frontend")
pnpm run build
if ($LASTEXITCODE -ne 0) { Pop-Location; exit 1 }
Pop-Location

Write-Host "`n=== 2. Gzip compress -> C header ===" -ForegroundColor Cyan
node (Join-Path (Join-Path $root "scripts") "compress-to-c.mjs")
if ($LASTEXITCODE -ne 0) { exit 1 }

Write-Host "`n=== 3. Build firmware ===" -ForegroundColor Cyan
if ($Upload) {
    pio run --target upload --target postbuild_image
} else {
    pio run
}
if ($LASTEXITCODE -ne 0) { exit 1 }

Write-Host "`n=== 4. Generate OTA image (ota_all.bin) ===" -ForegroundColor Cyan
python (Join-Path (Join-Path $root "scripts") "make_ota_bin.py")
if ($LASTEXITCODE -ne 0) { exit 1 }

Write-Host "`n=== DONE ===" -ForegroundColor Green

if ($Monitor) {
    pio device monitor
}
