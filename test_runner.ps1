# Project Chameleon - Automated Testing Script
# This script runs 3 different test scenarios and captures outputs

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Project Chameleon - Test Runner" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

$exePath = ".\build\bin\Debug\chameleon_dsa.exe"
if (-not (Test-Path $exePath)) {
    $exePath = ".\bin\chameleon_dsa.exe"
}
if (-not (Test-Path $exePath)) {
    Write-Host "Error: Executable not found. Please build the project first." -ForegroundColor Red
    exit 1
}

# Create test results directory
$testDir = "test_results"
if (-not (Test-Path $testDir)) {
    New-Item -ItemType Directory -Path $testDir | Out-Null
}

Write-Host "Running Test Set 1: Basic Functionality Tests..." -ForegroundColor Yellow
Get-Content "test_inputs\test_set1.txt" | & $exePath | Tee-Object -FilePath "$testDir\output_set1.txt"

Start-Sleep -Seconds 2

Write-Host "Running Test Set 2: Edge Cases and Error Handling..." -ForegroundColor Yellow
Get-Content "test_inputs\test_set2.txt" | & $exePath | Tee-Object -FilePath "$testDir\output_set2.txt"

Start-Sleep -Seconds 2

Write-Host "Running Test Set 3: Comprehensive Feature Testing..." -ForegroundColor Yellow
Get-Content "test_inputs\test_set3.txt" | & $exePath | Tee-Object -FilePath "$testDir\output_set3.txt"

Write-Host ""
Write-Host "========================================" -ForegroundColor Green
Write-Host "All tests completed!" -ForegroundColor Green
Write-Host "Results saved in: $testDir\" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green

