#!/usr/bin/env pwsh
# Run script for VisualPhysicsEngine

Write-Host "=== Running VisualPhysicsEngine ===" -ForegroundColor Green

# Check if executable exists
$exePath = "build/Release/VisualPhysicsEngine.exe"
if (!(Test-Path $exePath)) {
    Write-Host "❌ Executable not found: $exePath" -ForegroundColor Red
    Write-Host "Please run './compile.ps1' first to build the project." -ForegroundColor Yellow
    exit 1
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
