#!/usr/bin/env pwsh
# Compile script for VisualPhysicsEngine

Write-Host "=== Compiling VisualPhysicsEngine ===" -ForegroundColor Green

# Create build directory if it doesn't exist
if (!(Test-Path "build")) {
    Write-Host "Creating build directory..." -ForegroundColor Yellow
    New-Item -ItemType Directory -Name "build" | Out-Null
}

# Navigate to build directory
Set-Location "build"

try {
    # Configure CMake
    Write-Host "Configuring CMake..." -ForegroundColor Yellow
    cmake .. 
    if ($LASTEXITCODE -ne 0) {
        throw "CMake configuration failed"
    }

    # Build the project
    Write-Host "Building project..." -ForegroundColor Yellow
    cmake --build . --config Release
    if ($LASTEXITCODE -ne 0) {
        throw "Build failed"
    }

    Write-Host "✅ Compilation successful!" -ForegroundColor Green
    Write-Host "Executable created: build/Release/VisualPhysicsEngine.exe" -ForegroundColor Cyan
} catch {
    Write-Host "❌ Compilation failed: $_" -ForegroundColor Red
    Set-Location ".."
    exit 1
}

# Return to project root
Set-Location ".."
