#!/usr/bin/env pwsh
# Combined build and run script for VisualPhysicsEngine

Write-Host "=== Build and Run VisualPhysicsEngine ===" -ForegroundColor Magenta

# Compile first
Write-Host "Step 1: Compiling..." -ForegroundColor Yellow
& "./compile.ps1"
if ($LASTEXITCODE -ne 0) {
    Write-Host "❌ Build failed, stopping..." -ForegroundColor Red
    exit 1
}

Write-Host ""
Write-Host "Step 2: Running..." -ForegroundColor Yellow
& "./run.ps1"
