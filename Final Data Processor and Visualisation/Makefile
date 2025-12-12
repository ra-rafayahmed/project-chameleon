CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -I./include
LIBS = -lcurl
TARGET = bin/chameleon_dsa

# source files
SOURCES = src/main.cpp \
          src/union_find.cpp \
          src/trie.cpp \
          src/inverted_index.cpp \
          src/segment_tree.cpp \
          src/bloom_filter.cpp \
          src/minhash.cpp \
          src/json_parser.cpp \
          src/levenshtein.cpp \
          src/supabase_client.cpp \
          src/cli_interface.cpp

# object files
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
	rm -rf bin

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
