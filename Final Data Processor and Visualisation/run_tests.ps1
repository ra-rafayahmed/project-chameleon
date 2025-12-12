# Enhanced Test Runner with Better Output Handling
# Project Chameleon - Automated Testing Script

param(
    [switch]$Verbose
)

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Project Chameleon - Test Runner" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Find executable
$exePath = $null
if (Test-Path ".\build\bin\Debug\chameleon_dsa.exe") {
    $exePath = ".\build\bin\Debug\chameleon_dsa.exe"
} elseif (Test-Path ".\bin\chameleon_dsa.exe") {
    $exePath = ".\bin\chameleon_dsa.exe"
} else {
    Write-Host "Error: Executable not found. Please build the project first." -ForegroundColor Red
    Write-Host "Try running: build.bat" -ForegroundColor Yellow
    exit 1
}

Write-Host "Found executable: $exePath" -ForegroundColor Green
Write-Host ""

# Create directories
$testDir = "test_results"
$testInputDir = "test_inputs"

if (-not (Test-Path $testDir)) {
    New-Item -ItemType Directory -Path $testDir | Out-Null
    Write-Host "Created directory: $testDir" -ForegroundColor Yellow
}

if (-not (Test-Path $testInputDir)) {
    Write-Host "Error: Test inputs directory not found!" -ForegroundColor Red
    exit 1
}

# Function to run a test set
function Run-TestSet {
    param(
        [string]$SetName,
        [string]$InputFile,
        [string]$OutputFile
    )
    
    Write-Host "Running $SetName..." -ForegroundColor Yellow
    Write-Host "  Input: $InputFile" -ForegroundColor Gray
    Write-Host "  Output: $OutputFile" -ForegroundColor Gray
    
    $startTime = Get-Date
    
    try {
        # Redirect input and capture output
        Get-Content $InputFile | & $exePath *> $OutputFile
        
        $endTime = Get-Date
        $duration = ($endTime - $startTime).TotalSeconds
        
        if (Test-Path $OutputFile) {
            $size = (Get-Item $OutputFile).Length
            Write-Host "  Status: SUCCESS" -ForegroundColor Green
            Write-Host "  Duration: $([math]::Round($duration, 2)) seconds" -ForegroundColor Gray
            Write-Host "  Output size: $size bytes" -ForegroundColor Gray
        } else {
            Write-Host "  Status: WARNING - Output file not created" -ForegroundColor Yellow
        }
    } catch {
        Write-Host "  Status: ERROR - $($_.Exception.Message)" -ForegroundColor Red
    }
    
    Write-Host ""
    Start-Sleep -Seconds 1
}

# Run all test sets
Write-Host "Starting test execution..." -ForegroundColor Cyan
Write-Host ""

Run-TestSet -SetName "Test Set 1: Basic Functionality" `
    -InputFile "$testInputDir\test_set1.txt" `
    -OutputFile "$testDir\output_set1.txt"

Run-TestSet -SetName "Test Set 2: Edge Cases and Error Handling" `
    -InputFile "$testInputDir\test_set2.txt" `
    -OutputFile "$testDir\output_set2.txt"

Run-TestSet -SetName "Test Set 3: Comprehensive Feature Testing" `
    -InputFile "$testInputDir\test_set3.txt" `
    -OutputFile "$testDir\output_set3.txt"

# Summary
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Test Execution Summary" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan

$allFiles = @(
    "$testDir\output_set1.txt",
    "$testDir\output_set2.txt",
    "$testDir\output_set3.txt"
)

foreach ($file in $allFiles) {
    if (Test-Path $file) {
        $size = (Get-Item $file).Length
        $lines = (Get-Content $file | Measure-Object -Line).Lines
        Write-Host "$([System.IO.Path]::GetFileName($file)): $lines lines, $size bytes" -ForegroundColor Green
    } else {
        Write-Host "$([System.IO.Path]::GetFileName($file)): NOT FOUND" -ForegroundColor Red
    }
}

Write-Host ""
Write-Host "Results saved in: $testDir\" -ForegroundColor Green
Write-Host "View results with: Get-Content $testDir\output_set1.txt" -ForegroundColor Yellow
Write-Host "========================================" -ForegroundColor Cyan

