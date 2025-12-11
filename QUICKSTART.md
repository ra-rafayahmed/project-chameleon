# Quick Start Guide - Project Chameleon DSA

## 🚀 Quick Build & Run

### Linux/Mac
```bash
make
./bin/chameleon_dsa
```

### Windows
```bash
build.bat
bin\chameleon_dsa.exe
```

### Using CMake
```bash
mkdir build && cd build
cmake ..
make  # or cmake --build . on Windows
./bin/chameleon_dsa  # or bin\chameleon_dsa.exe on Windows
```

## 📋 Prerequisites

1. **C++17 Compiler**
   - Linux: `sudo apt-get install g++`
   - Mac: `xcode-select --install`
   - Windows: Install MinGW or Visual Studio

2. **libcurl**
   - Linux: `sudo apt-get install libcurl4-openssl-dev`
   - Mac: `brew install curl`
   - Windows: Use vcpkg or download pre-built binaries

## 🎯 Features Overview

The CLI interface provides access to 10 advanced DSA techniques:

1. **Union-Find** - Unify identities (username ↔ phone ↔ device)
2. **Trie** - Fast prefix search
3. **Inverted Index** - Full-text search
4. **Segment Tree** - RTT analysis
5. **Bloom Filter** - Duplicate detection
6. **MinHash/LSH** - Similarity search
7. **JSON Parser** - Normalization
8. **Levenshtein** - Fuzzy matching
9. **Data Dashboard** - Statistics

## 🔧 Troubleshooting

### Build Errors

**"curl/curl.h: No such file"**
- Install libcurl development package
- Check include paths in CMakeLists.txt or Makefile

**"undefined reference to curl_*"**
- Link libcurl: `-lcurl`
- Check library installation

### Runtime Errors

**"Failed to connect to Supabase"**
- Check internet connection
- Verify API key in `include/supabase_client.h`
- Check Supabase URL

**Empty results**
- Database might be empty
- Application uses sample data as fallback
- Check Supabase table permissions

## 📝 Code Organization

```
include/     - Header files (.h)
src/         - Implementation files (.cpp)
bin/         - Compiled executable (created after build)
```

## 🎨 CLI Navigation

- Use number keys (1-10) to select menu options
- Press Enter to continue after viewing results
- Type text when prompted for search queries
- Use Ctrl+C to exit

## 💡 Example Usage

1. Start the application
2. Select option 1 (Union-Find) to see identity unification
3. Select option 2 (Trie) and enter a prefix to search
4. Select option 9 (Dashboard) to see data statistics

Enjoy exploring Project Chameleon! 

