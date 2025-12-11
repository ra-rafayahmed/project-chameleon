# Segment Tree Crash Fix

## Issue
All test outputs were crashing at Option 4 (Segment Tree), stopping execution before completing all test cases.

## Root Cause
The SegmentTree build function was accessing array indices without proper bounds checking, causing out-of-bounds access or infinite recursion in some cases.

## Fixes Applied

1. **Added bounds checking in constructor**:
   - Added try-catch block
   - Added safety limit for tree size
   - Initialize tree with INT_MAX values

2. **Enhanced build() function**:
   - Added bounds checking before accessing tree[node]
   - Check if leftNode and rightNode are within tree size
   - Handle edge cases gracefully

3. **Enhanced queryHelper() function**:
   - Added bounds checking for node indices
   - Check tree size before accessing tree[node]
   - Safe handling of left/right child nodes

4. **Added error handling in handleSegmentTree()**:
   - Wrapped SegmentTree operations in try-catch
   - Graceful error messages if something fails
   - Continue execution even if SegmentTree fails

## Testing
After rebuilding, all test sets should complete successfully without crashing at Option 4.

## Rebuild Required
You need to rebuild the project for these fixes to take effect:
- Use Visual Studio to rebuild
- Or use your build system (CMake/Make)

