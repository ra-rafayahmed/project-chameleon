# Project Chameleon - Testing Guide

This document explains how to run automated tests and what to expect.

## Quick Start

### Windows (PowerShell)
```powershell
.\test_runner.ps1
```

### Windows (Command Prompt)
```batch
test_runner.bat
```

## Test Sets Overview

### Test Set 1: Basic Functionality Tests
**Purpose**: Verify all features work with standard inputs
**Input File**: `test_inputs/test_set1.txt`
**Output File**: `test_results/output_set1.txt`

**What to Verify**:
- ✅ All menu options execute without crashes
- ✅ Union-Find shows connected groups
- ✅ Trie finds matches for "user"
- ✅ Inverted Index finds documents with "travel"
- ✅ Segment Tree displays RTT statistics
- ✅ Bloom Filter detects duplicates
- ✅ MinHash finds similar documents
- ✅ JSON Parser normalizes JSON correctly
- ✅ Levenshtein finds fuzzy matches
- ✅ Data Dashboard shows statistics
- ✅ Sorting algorithms complete and show times
- ✅ Huffman compression generates file

**Expected Behavior**:
- No crashes or error messages
- All features display meaningful results
- File created in `output/` directory for Huffman compression

### Test Set 2: Edge Cases and Error Handling
**Purpose**: Test with non-existent data and edge cases
**Input File**: `test_inputs/test_set2.txt`
**Output File**: `test_results/output_set2.txt`

**What to Verify**:
- ✅ Application handles missing data gracefully
- ✅ No crashes with empty results
- ✅ Appropriate "not found" or "no matches" messages
- ✅ Edge cases handled correctly
- ✅ Error messages are user-friendly

**Expected Behavior**:
- Searches with no results show "No matches found" or similar
- No crashes occur
- Application continues normally

### Test Set 3: Comprehensive Feature Testing
**Purpose**: Test multiple scenarios with real-world data
**Input File**: `test_inputs/test_set3.txt`
**Output File**: `test_results/output_set3.txt`

**What to Verify**:
- ✅ Multiple searches work correctly
- ✅ Different inputs produce different results
- ✅ All features tested multiple times
- ✅ Application handles repeated feature access

**Expected Behavior**:
- Results vary based on input
- Multiple tests of same feature show consistency
- Application maintains state correctly

## Manual Testing Guide

If you want to test manually, here are suggested inputs:

### Option 1: Union-Find
- **Input**: `1`
- **Expected**: Shows connected identity groups

### Option 2: Trie
- **Input**: `2`, then `user`
- **Expected**: Lists items starting with "user"
- **Alternative**: Try `alex`, `device`, `123`

### Option 3: Inverted Index
- **Input**: `3`, then `travel`
- **Expected**: Shows documents containing "travel"
- **Alternative**: Try `food`, `city`, `photographer`

### Option 4: Segment Tree
- **Input**: `4`
- **Expected**: Shows RTT analysis, min/max/sum statistics
- **Verify**: No crashes, valid numbers displayed

### Option 5: Bloom Filter
- **Input**: `5`, then `phone`
- **Expected**: Shows whether item might exist
- **Alternative**: Try existing items vs new items

### Option 6: MinHash/LSH
- **Input**: `6`, then `travel`
- **Expected**: Shows similar documents
- **Alternative**: Try different text queries

### Option 7: JSON Parser
- **Input**: `7`
- **Expected**: Shows normalized JSON with proper formatting

### Option 8: Levenshtein
- **Input**: `8`, then `test_user`
- **Expected**: Shows fuzzy matches with similarity scores
- **Alternative**: Try `alex`, `user123`, misspellings

### Option 9: Data Dashboard
- **Input**: `9`
- **Expected**: Shows data statistics table
- **Verify**: No crashes, all tables shown

### Option 10: Sorting Algorithms
- **Input**: `10`
- **Expected**: Shows performance comparison of 5 sorting algorithms
- **Verify**: All algorithms complete, times shown

### Option 11: Huffman Compression
- **Input**: `11`
- **Expected**: Shows compression statistics and creates `.bin` file
- **Verify**: File created in `output/` directory
- **Check**: Compression ratio displayed

## Interpreting Results

### Good Results (PASS)
- ✅ No crashes or error dialogs
- ✅ All features execute successfully
- ✅ Output files contain expected content
- ✅ Numbers and statistics are reasonable
- ✅ Files are created where expected

### Bad Results (FAIL)
- ❌ Crashes or abort() errors
- ❌ Blank or corrupted output files
- ❌ Features not executing
- ❌ Invalid numbers or statistics
- ❌ Files not created

## Output File Locations

- Test Set 1: `test_results/output_set1.txt`
- Test Set 2: `test_results/output_set2.txt`
- Test Set 3: `test_results/output_set3.txt`
- Huffman Files: `output/compressed_data_*.bin`

## Troubleshooting

### Script Fails to Run
- Ensure the executable is built: `build.bat` or `cmake`
- Check executable path in script matches your build location
- Verify test_inputs directory exists

### Tests Crash
- Check if database connection is working
- Verify all source files are compiled
- Check for missing dependencies (libcurl)

### Output Files Empty
- Check if executable path is correct
- Verify input redirection is working
- Run executable manually to check for errors

### No Compression Files
- Check if `output/` directory is created
- Verify write permissions
- Check disk space

## Example Expected Output Snippets

### Union-Find (Option 1)
```
  Total elements: 14
  Connected groups: 6
  Identity Groups:
    Group 1: phone <-> device1 <-> 1234567890
```

### Trie (Option 2)
```
  Enter search prefix: user
  Search results for "user":
    - user123 -> user:user123
```

### Data Dashboard (Option 9)
```
  DATA ANALYSIS DASHBOARD
  ---------------------------------------------------------------
  Table                Status            Records
  users                [OK] Loaded       5
```

### Sorting (Option 10)
```
  Algorithm          Time (microsec)  Sorted Data (first 10)
  ---------------------------------------------------------------
  Quick Sort         15               3 5 11 12 22 23 25 34 45 64
  Merge Sort         18               3 5 11 12 22 23 25 34 45 64
```

### Huffman (Option 11)
```
  Data source: Database (Supabase)
  Compression results:
    Original size: 88 bytes
    Compressed size: 53 bytes
    Compression ratio: 38.92%
  Compressed data saved to:
    Full Path: C:\...\output\compressed_data_*.bin
```

