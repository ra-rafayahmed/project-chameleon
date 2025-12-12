# Quick Test Guide

## Running Tests

### Option 1: PowerShell (Recommended)
```powershell
.\run_tests.ps1
```

### Option 2: Batch File
```batch
test_runner.bat
```

### Option 3: Manual Verification
```powershell
.\verify_tests.ps1
.\verify_tests.ps1 -Detailed
```

## Test Output Locations

- **Test Set 1**: `test_results\output_set1.txt`
- **Test Set 2**: `test_results\output_set2.txt`
- **Test Set 3**: `test_results\output_set3.txt`
- **Compression Files**: `output\compressed_data_*.bin`

## What Each Test Set Does

### Test Set 1: Basic Functionality
- Tests all 12 menu options with standard inputs
- Uses common search terms and expected data
- **Should show**: All features working correctly

### Test Set 2: Edge Cases
- Tests with non-existent data
- Uses unusual/edge case inputs
- **Should show**: Graceful error handling, no crashes

### Test Set 3: Comprehensive
- Tests features multiple times
- Uses varied real-world inputs
- **Should show**: Consistency and robustness

## Quick Verification

After running tests, check:

1. **Output files exist**: `test_results\output_set*.txt`
2. **Files are not empty**: Should be several KB each
3. **No crashes**: Search for "abort" in output files
4. **Compression files**: Check `output\` directory

## Viewing Results

```powershell
# View Test Set 1 results
Get-Content test_results\output_set1.txt

# Search for errors
Select-String -Path test_results\output_*.txt -Pattern "error|crash|abort"

# Count lines in each file
Get-ChildItem test_results\output_*.txt | ForEach-Object { 
    "$($_.Name): $((Get-Content $_.FullName).Count) lines" 
}
```

## Expected File Sizes

- Each output file: **5-20 KB** (depending on data)
- Compression files: **1-5 KB** each
- If files are much smaller, tests may not have completed

## Troubleshooting

### Tests Don't Run
- **Error**: "Executable not found"
  - **Fix**: Build the project first: `build.bat` or CMake

### Empty Output Files
- **Cause**: Input redirection issue
  - **Fix**: Check that `test_inputs\` directory exists with test files

### Tests Crash
- **Cause**: Application bug
  - **Fix**: Run executable manually to identify issue: `.\build\bin\Debug\chameleon_dsa.exe`

### No Compression Files
- **Cause**: Output directory creation failed
  - **Fix**: Check write permissions, verify `output\` directory exists

## Success Criteria

✅ **All Tests Pass If**:
- All 3 output files created and non-empty
- No "abort()" or crash messages
- All 12 menu options tested
- Compression files created
- Verification script shows PASS

❌ **Tests Fail If**:
- Output files missing or empty
- Crash messages present
- Features missing from output
- Compression files not created

