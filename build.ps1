param(
    [switch]$Clean,
    [switch]$Export,
    [switch]$Run
)

if ($Clean) {
    Write-Host "Cleaning all build and distribution artifacts..." -ForegroundColor Cyan
    if (Test-Path build) { Remove-Item -Recurse -Force build }
    if (Test-Path dist) { Remove-Item -Recurse -Force dist }
}

cmake -S . -B build -G Ninja -DCMAKE_C_COMPILER=gcc
if ($LASTEXITCODE -ne 0) {
    Write-Error "CMake configuration failed."
    exit $LASTEXITCODE
}

cmake --build build
if ($LASTEXITCODE -ne 0) {
    Write-Error "Build failed."
    exit $LASTEXITCODE
}
Write-Host "Build completed successfully!" -ForegroundColor Green

if ($Export -or $Run) {
    $distDir = "dist"
    Write-Host "Exporting files to $distDir..." -ForegroundColor Cyan
    if (Test-Path $distDir) { Remove-Item -Recurse -Force $distDir }
    New-Item -ItemType Directory -Path "$distDir\data" | Out-Null

    Copy-Item "build\movie-ticket-console.exe" -Destination "$distDir"
    Copy-Item "data\*.csv" -Destination "$distDir\data\"
    Write-Host "Export complete!" -ForegroundColor Green
}

if ($Run) {
    $exePath = "dist\movie-ticket-console.exe"
    Write-Host "Running from dist directory..." -ForegroundColor Yellow
    Push-Location dist
    & ".\movie-ticket-console.exe"
    Pop-Location
}
