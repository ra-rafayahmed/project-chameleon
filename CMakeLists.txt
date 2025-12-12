cmake_minimum_required(VERSION 3.10)

# Set vcpkg toolchain BEFORE project() call
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")

project(ChameleonDSA)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)

set(SOURCES
	src/mainfinal.cpp
	src/union_find.cpp
	src/trie.cpp
	src/inverted_index.cpp
	src/segment_tree.cpp
	src/bloom_filter.cpp
	src/minhash.cpp
	src/json_parser.cpp
	src/levenshtein.cpp
	src/supabase_client.cpp
	src/supabase_analyzer.cpp
	src/cli_interface.cpp
)

add_executable(chameleon_dsa ${SOURCES})

target_include_directories(chameleon_dsa PRIVATE
	${CURL_INCLUDE_DIRS}
	${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(chameleon_dsa PRIVATE 
	CURL::libcurl
	nlohmann_json::nlohmann_json
)

target_compile_features(chameleon_dsa PRIVATE cxx_std_17)

set_target_properties(chameleon_dsa PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

