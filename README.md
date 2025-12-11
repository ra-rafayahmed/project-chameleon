# Project Chameleon - Advanced DSA Implementation

A comprehensive C++ application implementing the top 10 most suitable Data Structures and Algorithms for Project Chameleon's multi-source data analysis system.

##  Features

### Top 10 DSA Techniques Implemented

1. **Graph + Union-Find (DSU)** - Unifies multi-source identities (usernames, phones, device IDs)
2. **Trie** - Fast indexing and prefix search for usernames, phones, and device IDs
3. **Inverted Index** - Full-text search across bios, captions, and JSON content
4. **Segment Tree + Sliding Window** - Efficient RTT analysis for WhatsApp events
5. **Bloom Filter** - Fast duplicate detection during data ingestion
6. **MinHash / LSH** - Similarity detection across text and JSON documents
7. **JSON Parser (DFS)** - Normalizes raw JSON using depth-first traversal
8. **Levenshtein Distance** - Fuzzy matching for user/phone linking
9. **Hash Maps** - Device/user linking and deduplication
10. **Priority Queue** - Scheduling and ranking operations

##  Project Structure

```
DSA/
├── include/              # Header files
│   ├── union_find.h
│   ├── trie.h
│   ├── inverted_index.h
│   ├── segment_tree.h
│   ├── bloom_filter.h
│   ├── minhash.h
│   ├── json_parser.h
│   ├── levenshtein.h
│   ├── supabase_client.h
│   └── cli_interface.h
├── src/                  # Implementation files
│   ├── main.cpp          # Main CLI application
│   ├── union_find.cpp
│   ├── trie.cpp
│   ├── inverted_index.cpp
│   ├── segment_tree.cpp
│   ├── bloom_filter.cpp
│   ├── minhash.cpp
│   ├── json_parser.cpp
│   ├── levenshtein.cpp
│   ├── supabase_client.cpp
│   └── cli_interface.cpp
├── CMakeLists.txt        # CMake build configuration
├── Makefile              # Make build configuration
└── README.md
```

##  Building

### Prerequisites

- C++17 compiler (g++, clang++, or MSVC)
- CMake 3.10+ (optional, for CMake build)
- libcurl development libraries

### Linux/Mac

```bash
# Install dependencies
sudo apt-get install libcurl4-openssl-dev  # Ubuntu/Debian
# or
brew install curl  # macOS

# Build with Make
make

# Build with CMake
mkdir build && cd build
cmake ..
make

# Run
./bin/chameleon_dsa
```

### Windows

```bash
# Install vcpkg and curl
vcpkg install curl

# Build with CMake
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake
cmake --build .

# Run
.\bin\chameleon_dsa.exe
```

##  Usage

The application features a beautiful CLI interface with interactive menus:

1. **Union-Find (DSU)** - Unify identities across multiple sources
2. **Trie** - Search usernames, phones, and device IDs with prefix matching
3. **Inverted Index** - Search bios, captions, and JSON content
4. **Segment Tree + Sliding Window** - Analyze WhatsApp RTT patterns
5. **Bloom Filter** - Check for duplicate entries
6. **MinHash / LSH** - Find similar documents
7. **JSON Parser** - Normalize and parse JSON data
8. **Levenshtein Distance** - Fuzzy match usernames and phone numbers
9. **Data Analysis Dashboard** - View statistics and summaries

## 🔧 Configuration

Supabase connection details are configured in `include/supabase_client.h`. Update the URL and API key if needed:

```cpp
struct SupabaseConfig {
    std::string url = "https://your-project.supabase.co";
    std::string api_key = "your-api-key";
};
```

##  Database Schema Support

The application works with the following Supabase tables:

- `users` - Master users table
- `instagram_profiles` - Instagram profile data
- `instagram_stories` - Story metadata
- `game_devices` - Device telemetry
- `whatsapp_events` - WhatsApp RTT timeline

##  Algorithm Details

### Union-Find (DSU)
- Path compression for O(α(n)) amortized time
- Union by rank optimization
- Groups related identities (username ↔ phone ↔ device_id)

### Trie
- O(m) search time where m is key length
- Prefix search for autocomplete
- Efficient memory usage

### Inverted Index
- Tokenization and indexing
- AND/OR query support
- Term frequency tracking

### Segment Tree
- O(log n) range queries
- O(log n) updates
- Range min/max/sum operations

### Bloom Filter
- O(k) insertion and lookup where k is number of hash functions
- Space-efficient probabilistic data structure
- Configurable false positive rate

### MinHash / LSH
- Jaccard similarity estimation
- Locality-sensitive hashing for approximate nearest neighbor
- Configurable signature size and bands

### JSON Parser
- Recursive descent parser
- DFS traversal for normalization
- Key extraction and path queries

### Levenshtein Distance
- Dynamic programming approach
- O(mn) time complexity
- Similarity ratio calculation

##  Notes

- All algorithms are implemented from scratch
- Code includes lowercase comments as requested
- The CLI interface provides an intuitive way to interact with all features
- Data is loaded from Supabase at startup
- Sample data is used if database is empty or unavailable

##  Security

- API keys should be stored securely in production
- Consider using environment variables for sensitive data
- Validate all user inputs

##  License

This project is part of Project Chameleon research initiative.
