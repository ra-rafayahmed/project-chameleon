@echo off
REM Project Chameleon - Automated Testing Script (Batch Version)
REM This script runs 3 different test scenarios and captures outputs

echo ========================================
echo Project Chameleon - Test Runner
echo ========================================
echo.

REM Check if executable exists
if exist ".\build\bin\Debug\chameleon_dsa.exe" (
    set EXE_PATH=.\build\bin\Debug\chameleon_dsa.exe
) else if exist ".\bin\chameleon_dsa.exe" (
    set EXE_PATH=.\bin\chameleon_dsa.exe
) else (
    echo Error: Executable not found. Please build the project first.
    pause
    exit /b 1
)

REM Create test results directory
if not exist "test_results" mkdir test_results

echo Running Test Set 1: Basic Functionality Tests...
echo.
< test_inputs\test_set1.txt "%EXE_PATH%" > test_results\output_set1.txt 2>&1

timeout /t 2 /nobreak >nul

echo Running Test Set 2: Edge Cases and Error Handling...
echo.
< test_inputs\test_set2.txt "%EXE_PATH%" > test_results\output_set2.txt 2>&1

timeout /t 2 /nobreak >nul

echo Running Test Set 3: Comprehensive Feature Testing...
echo.
< test_inputs\test_set3.txt "%EXE_PATH%" > test_results\output_set3.txt 2>&1

echo.
echo ========================================
echo All tests completed!
echo Results saved in: test_results\
echo ========================================
echo.
pause

