# Expected Outputs for Test Sets

This document describes what you should see in each test output file.

## Test Set 1: Basic Functionality Tests

### Expected Output Structure

1. **Union-Find (Option 1)**
   ```
   UNION-FIND (DSU) - Identity Unification
   Total elements: [number]
   Connected groups: [number]
   Identity Groups:
     Group 1: [items] <-> [items]
   ```
   ✅ **Correct if**: Shows connected groups, no crashes

2. **Trie Search (Option 2)**
   ```
   Enter search prefix: user
   Search results for "user":
     - user123 -> user:user123
     - [more matches if any]
   ```
   ✅ **Correct if**: Shows matches starting with "user", or "No matches found"

3. **Inverted Index (Option 3)**
   ```
   Enter search query: travel
   Search results ([number] documents):
     [0] [document text]...
   ```
   ✅ **Correct if**: Shows documents containing "travel"

4. **Segment Tree (Option 4)**
   ```
   RTT Analysis with Segment Tree & Sliding Window:
   RTT Values: [numbers]
   Range [0-4] Min: [number]ms
   Range [0-4] Max: [number]ms
   Range [0-4] Sum: [number]ms
   Sliding Window (size 5):
     Window [0-4]: Avg=[number]ms, Min=[number]ms, Max=[number]ms
   ```
   ✅ **Correct if**: Shows RTT statistics, no crashes, valid numbers

5. **Bloom Filter (Option 5)**
   ```
   Enter item to check: phone
   Result: Item MIGHT exist (could be false positive)
   OR
   Result: Item definitely does NOT exist
   ```
   ✅ **Correct if**: Shows result for "phone" check

6. **MinHash (Option 6)**
   ```
   Enter text to find similar: photographer
   Similar documents: [number]
     - doc[number]
   ```
   ✅ **Correct if**: Shows similar documents or "0 similar documents"

7. **JSON Parser (Option 7)**
   ```
   Original JSON:
     {"username":"test","bio":"sample","posts":[1,2,3]}
   Normalized JSON:
   {
     "username": "test",
     ...
   }
   ```
   ✅ **Correct if**: Shows formatted JSON

8. **Levenshtein (Option 8)**
   ```
   Enter username to match: test_user
   Matching results:
     test_user - Similarity: 100%, Distance: 0
   ```
   ✅ **Correct if**: Shows matches with similarity scores

9. **Data Dashboard (Option 9)**
   ```
   DATA ANALYSIS DASHBOARD
   Table                Status            Records
   users                [OK] Loaded       [number]
   [more tables]
   Statistics:
     Total RTT values: [number]
   ```
   ✅ **Correct if**: Shows table with data, no crashes

10. **Sorting Algorithms (Option 10)**
    ```
    Sorting Algorithms - Performance Analysis:
    Original data: [numbers]
    Performance Results:
      Algorithm          Time (microsec)  Sorted Data (first 10)
      Quick Sort         [number]         [sorted numbers]
      Merge Sort         [number]         [sorted numbers]
      Heap Sort          [number]         [sorted numbers]
      Bubble Sort        [number]         [sorted numbers]
      Insertion Sort     [number]         [sorted numbers]
    ```
    ✅ **Correct if**: Shows all 5 algorithms, sorted data matches, times shown

11. **Huffman Compression (Option 11)**
    ```
    Data source: Database (Supabase) OR Test Data (Fallback)
    Original data: [text]
    Size: [number] bytes
    Character frequencies (top 10):
      '[SPACE]': [number] times
    Compression results:
      Original size: [number] bytes
      Compressed size: [number] bytes
      Compression ratio: [number]%
    Compressed data saved to:
      Full Path: [path]
    ```
    ✅ **Correct if**: Shows compression stats, file path displayed

12. **Exit (Option 12)**
    ```
    Thank you for using Project Chameleon!
    ```
    ✅ **Correct if**: Shows exit message

## Test Set 2: Edge Cases and Error Handling

### Expected Output Structure

Similar structure to Test Set 1, but with:

1. **Trie Search for "xyz123nonexistent"**
   ```
   Search results for "xyz123nonexistent":
     No matches found.
   ```
   ✅ **Correct if**: Shows "No matches found" (graceful handling)

2. **Inverted Index for "zzzzzzzzzzzzzzz"**
   ```
   Search results (0 documents):
   ```
   ✅ **Correct if**: Shows 0 results, no crashes

3. **Bloom Filter for "new_item_not_in_filter"**
   ```
   Result: Item definitely does NOT exist
   ```
   ✅ **Correct if**: Correctly identifies non-existent item

4. **Levenshtein for "very_long_username_that_does_not_exist"**
   ```
   Matching results:
     [may show no matches or fuzzy matches with low similarity]
   ```
   ✅ **Correct if**: Handles long strings without crashing

**Overall**: This test should complete without crashes even with non-existent data.

## Test Set 3: Comprehensive Feature Testing

### Expected Output Structure

Similar to Test Set 1, but with:
- Multiple searches with different inputs
- Multiple tests of the same features
- Real-world data scenarios

**Key Differences**:
- Trie search for "alex" should find matches
- Inverted Index for "food" should find relevant documents
- Bloom Filter for "alex_photo" may show positive (if added earlier)
- Additional searches test consistency

✅ **Correct if**: 
- All features work multiple times
- Results are consistent
- No crashes with repeated operations

## Verification Checklist

### For Each Test Set Output File:

- [ ] File exists and is not empty
- [ ] Contains all 12 menu option outputs
- [ ] No "abort()" or crash messages
- [ ] Numbers are valid (not NaN, not negative where inappropriate)
- [ ] Text output is readable (no encoding issues)
- [ ] File paths shown correctly
- [ ] Compression files created (check `output/` directory)

### Common Issues to Watch For:

1. **Empty Output File**
   - Cause: Executable not found, input redirection failed
   - Fix: Check executable path, verify input file format

2. **Partial Output**
   - Cause: Application crashed mid-test
   - Fix: Run executable manually to identify crash point

3. **Encoding Issues**
   - Cause: Unicode characters in output
   - Fix: Should be fixed in latest code (ASCII only)

4. **Missing Compression Files**
   - Cause: Directory creation failed, permissions issue
   - Fix: Check `output/` directory exists, verify write permissions

## Sample Correct Output Excerpts

### Good Output Example (Union-Find):
```
================================================================
        PROJECT CHAMELEON - DSA ANALYZER
================================================================

  UNION-FIND (DSU) - Identity Unification
  Union-Find Operations:
  ---------------------------------------------------------------
  Total elements: 14
  Connected groups: 6

  Identity Groups:
    Group 1: phone <-> device1 <-> 1234567890
    Group 2: 5555555555
```

### Good Output Example (Sorting):
```
  Performance Results:
  ---------------------------------------------------------------
  Algorithm          Time (microsec)  Sorted Data (first 10)
  ---------------------------------------------------------------
  Quick Sort         12               3 5 11 12 22 23 25 34 45 64
  Merge Sort         15               3 5 11 12 22 23 25 34 45 64
  Heap Sort          18               3 5 11 12 22 23 25 34 45 64
  Bubble Sort        45               3 5 11 12 22 23 25 34 45 64
  Insertion Sort     32               3 5 11 12 22 23 25 34 45 64
```

### Bad Output Example (Crash):
```
...normal output...
Debug Error!
abort() has been called
```

## Running Manual Verification

After automated tests, manually verify:

1. Check output file sizes (should be substantial)
2. Search for keywords:
   - "Error" - should be minimal
   - "abort" - should not appear
   - "crashed" - should not appear
3. Verify compression files exist in `output/` directory
4. Check that all 12 menu options are represented in outputs

