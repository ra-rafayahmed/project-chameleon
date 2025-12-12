@echo off
echo Building Project Chameleon DSA...

REM Check if g++ is available
where g++ >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo Error: g++ compiler not found. Please install MinGW or use Visual Studio.
    echo For Visual Studio, use CMake instead.
    pause
    exit /b 1
)

REM Create bin directory
if not exist bin mkdir bin

REM Build with g++
g++ -std=c++17 -Wall -Wextra -O2 -I./include -c src/main.cpp -o src/main.o
g++ -std=c++17 -Wall -Wextra -O2 -I./include -c src/union_find.cpp -o src/union_find.o
g++ -std=c++17 -Wall -Wextra -O2 -I./include -c src/trie.cpp -o src/trie.o
g++ -std=c++17 -Wall -Wextra -O2 -I./include -c src/inverted_index.cpp -o src/inverted_index.o
g++ -std=c++17 -Wall -Wextra -O2 -I./include -c src/segment_tree.cpp -o src/segment_tree.o
g++ -std=c++17 -Wall -Wextra -O2 -I./include -c src/bloom_filter.cpp -o src/bloom_filter.o
g++ -std=c++17 -Wall -Wextra -O2 -I./include -c src/minhash.cpp -o src/minhash.o
g++ -std=c++17 -Wall -Wextra -O2 -I./include -c src/json_parser.cpp -o src/json_parser.o
g++ -std=c++17 -Wall -Wextra -O2 -I./include -c src/levenshtein.cpp -o src/levenshtein.o
g++ -std=c++17 -Wall -Wextra -O2 -I./include -c src/supabase_client.cpp -o src/supabase_client.o
g++ -std=c++17 -Wall -Wextra -O2 -I./include -c src/cli_interface.cpp -o src/cli_interface.o

g++ -std=c++17 -o bin/chameleon_dsa.exe src/*.o -lcurl

if %ERRORLEVEL% EQU 0 (
    echo.
    echo Build successful! Run bin\chameleon_dsa.exe to execute.
) else (
    echo Build failed. Make sure libcurl is installed.
)

REM Clean up object files
del src\*.o 2>nul

pause
