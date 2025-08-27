#!/usr/bin/env pwsh
# Run script for VisualPhysicsEngine

Write-Host "=== Running VisualPhysicsEngine ===" -ForegroundColor Green

# Check if executable exists (try Debug first, then Release)
$exePath = "build/Debug/VisualPhysicsEngine.exe"
if (!(Test-Path $exePath)) {
    $exePath = "build/Release/VisualPhysicsEngine.exe"
    if (!(Test-Path $exePath)) {
        Write-Host "❌ Executable not found in Debug or Release" -ForegroundColor Red
        Write-Host "Please run './compile.ps1' first to build the project." -ForegroundColor Yellow
        exit 1
    } else {
        Write-Host "⚠️  Running Release build (validation layers disabled)" -ForegroundColor Yellow
    }
} else {
    Write-Host "🔍 Running Debug build (validation layers enabled)" -ForegroundColor Magenta
}

Write-Host "Starting application..." -ForegroundColor Yellow
Write-Host "----------------------------------------" -ForegroundColor Cyan

try {
    # Run the executable
    & $exePath
    $exitCode = $LASTEXITCODE
    
    Write-Host "----------------------------------------" -ForegroundColor Cyan
    if ($exitCode -eq 0) {
        Write-Host "✅ Application completed successfully!" -ForegroundColor Green
    } else {
        Write-Host "⚠️  Application exited with code: $exitCode" -ForegroundColor Yellow
    }
} catch {
    Write-Host "❌ Failed to run application: $_" -ForegroundColor Red
    exit 1
}
