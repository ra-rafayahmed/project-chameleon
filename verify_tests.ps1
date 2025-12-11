# Test Output Verification Script
# Analyzes test output files and reports issues

param(
    [switch]$Detailed
)

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Project Chameleon - Test Verification" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

$testDir = "test_results"
if (-not (Test-Path $testDir)) {
    Write-Host "Error: Test results directory not found!" -ForegroundColor Red
    Write-Host "Run tests first: .\run_tests.ps1" -ForegroundColor Yellow
    exit 1
}

$testFiles = @(
    @{Name="Test Set 1: Basic Functionality"; File="output_set1.txt"},
    @{Name="Test Set 2: Edge Cases"; File="output_set2.txt"},
    @{Name="Test Set 3: Comprehensive"; File="output_set3.txt"}
)

$allPassed = $true

foreach ($test in $testFiles) {
    $filePath = Join-Path $testDir $test.File
    Write-Host "Checking: $($test.Name)" -ForegroundColor Yellow
    
    if (-not (Test-Path $filePath)) {
        Write-Host "  [FAIL] Output file not found!" -ForegroundColor Red
        $allPassed = $false
        continue
    }
    
    $content = Get-Content $filePath -Raw
    $lines = Get-Content $filePath
    $size = (Get-Item $filePath).Length
    
    # Basic checks
    $checks = @{
        "File exists" = $true
        "File not empty" = ($size -gt 0)
        "Has content" = ($lines.Count -gt 10)
        "No crashes" = ($content -notmatch "abort\(\)|Debug Error|runtime error|crash")
        "Has menu options" = ($content -match "MAIN MENU|PROJECT CHAMELEON")
        "Has Union-Find" = ($content -match "Union-Find|UNION-FIND")
        "Has Trie" = ($content -match "Trie|TRIE")
        "Has Segment Tree" = ($content -match "Segment Tree|SEGMENT")
        "Has Sorting" = ($content -match "Sorting|Quick Sort|Merge Sort")
        "Has Huffman" = ($content -match "Huffman|HUFFMAN|Compression")
    }
    
    $passed = 0
    $failed = 0
    
    foreach ($check in $checks.GetEnumerator()) {
        if ($check.Value) {
            if ($Detailed) {
                Write-Host "  [PASS] $($check.Key)" -ForegroundColor Green
            }
            $passed++
        } else {
            Write-Host "  [FAIL] $($check.Key)" -ForegroundColor Red
            $failed++
            $allPassed = $false
        }
    }
    
    # Count menu options tested
    $menuOptionsFound = 0
    for ($i = 1; $i -le 12; $i++) {
        # Check for various indicators of menu option execution
        if ($content -match "Option $i|case $i|$i\.") {
            $menuOptionsFound++
        }
    }
    
    # Check for feature-specific indicators
    $features = @(
        "Union-Find", "Trie", "Inverted Index", "Segment Tree",
        "Bloom Filter", "MinHash", "JSON Parser", "Levenshtein",
        "Data Dashboard", "Sorting", "Huffman", "Exit"
    )
    
    $featuresFound = ($features | Where-Object { $content -match $_ }).Count
    
    Write-Host "  Status: $passed/$($checks.Count) checks passed" -ForegroundColor $(if ($failed -eq 0) { "Green" } else { "Red" })
    Write-Host "  Size: $size bytes, $($lines.Count) lines" -ForegroundColor Gray
    Write-Host "  Features detected: $featuresFound/12" -ForegroundColor Gray
    
    if ($failed -gt 0 -or $featuresFound -lt 8) {
        Write-Host "  [WARNING] Some checks failed or features missing" -ForegroundColor Yellow
    }
    
    Write-Host ""
}

# Check compression files
Write-Host "Checking compression files..." -ForegroundColor Yellow
$outputDir = "output"
if (Test-Path $outputDir) {
    $compressedFiles = Get-ChildItem $outputDir -Filter "compressed_data_*.bin"
    if ($compressedFiles.Count -gt 0) {
        Write-Host "  [PASS] Found $($compressedFiles.Count) compression file(s)" -ForegroundColor Green
        foreach ($file in $compressedFiles) {
            Write-Host "    - $($file.Name) ($($file.Length) bytes)" -ForegroundColor Gray
        }
    } else {
        Write-Host "  [WARN] No compression files found in output/ directory" -ForegroundColor Yellow
    }
} else {
    Write-Host "  [WARN] Output directory not found" -ForegroundColor Yellow
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
if ($allPassed) {
    Write-Host "Overall Status: PASS" -ForegroundColor Green
} else {
    Write-Host "Overall Status: FAIL - Some issues detected" -ForegroundColor Red
    Write-Host "Review the failed checks above" -ForegroundColor Yellow
}
Write-Host "========================================" -ForegroundColor Cyan

exit $(if ($allPassed) { 0 } else { 1 })

