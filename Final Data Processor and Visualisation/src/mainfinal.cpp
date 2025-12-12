// ============================================================================
// PROJECT CHAMELEON - SUPABASE DATA ANALYSIS WITH DSA
// Complete implementation with Huffman encoding, hybrid sorting, and analysis
// ============================================================================

#include "../include/supabase_client.h"
#include <algorithm>

#include "../include/supabase_models.h"
#include "../include/supabase_analyzer.h"
#include "../include/union_find.h"
#include "../include/trie.h"
#include "../include/inverted_index.h"
#include "../include/bloom_filter.h"
#include "../include/segment_tree.h"
#include "../include/levenshtein.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <iomanip>
#include <fstream>
#include <queue>
#include <string>
#include <sstream>
#include <cmath>
#include <chrono>
#define NOMINMAX  
#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#else
#include <sys/stat.h>
#include <sys/types.h>


#endif

// Ensure Windows macros don't collide with std::min/std::max
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

// Forward declarations for functions defined later in this file
void performHuffmanEncodingFromFile();
void sortGameEvents(std::vector<GameEvent>& events);
void searchInstagramProfile(const std::vector<InstagramProfile>& profiles);
void searchWhatsAppEvent(const std::vector<WhatsAppEvent>& events);
void searchGameEvent(const std::vector<GameEvent>& games);

// ============================================================================
// HUFFMAN ENCODING IMPLEMENTATION
// ============================================================================

struct HuffmanNode {
    char ch;
    int freq;
    HuffmanNode* left;
    HuffmanNode* right;
    
    HuffmanNode(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
};

struct CompareNode {
    bool operator()(HuffmanNode* a, HuffmanNode* b) {
        return a->freq > b->freq;
    }
};

class HuffmanEncoding {
private:
    HuffmanNode* root;
    std::map<char, std::string> codes;
    std::map<char, int> frequencies;
    
    void buildCodes(HuffmanNode* node, std::string code) {
        if (!node) return;
        if (!node->left && !node->right) {
            codes[node->ch] = code.empty() ? "0" : code;
            return;
        }
        buildCodes(node->left, code + "0");
        buildCodes(node->right, code + "1");
    }
    
    void deleteTree(HuffmanNode* node) {
        if (!node) return;
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }
    
public:
    HuffmanEncoding() : root(nullptr) {}
    
    ~HuffmanEncoding() {
        deleteTree(root);
    }
    
    void build(const std::string& text) {
        frequencies.clear();
        codes.clear();
        
        // Count frequencies
        for (char c : text) {
            frequencies[c]++;
        }
        
        // Build Huffman tree
        std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, CompareNode> pq;
        
        for (auto& p : frequencies) {
            pq.push(new HuffmanNode(p.first, p.second));
        }
        
        if (pq.size() == 1) {
            root = pq.top();
            pq.pop();
            buildCodes(root, "0");
            return;
        }
        
        while (pq.size() > 1) {
            HuffmanNode* left = pq.top(); pq.pop();
            HuffmanNode* right = pq.top(); pq.pop();
            HuffmanNode* parent = new HuffmanNode('\0', left->freq + right->freq);
            parent->left = left;
            parent->right = right;
            pq.push(parent);
        }
        
        if (!pq.empty()) {
            root = pq.top();
            pq.pop();
            buildCodes(root, "");
        }
    }
    
    std::string encode(const std::string& text) {
        std::string encoded;
        for (char c : text) {
            if (codes.find(c) != codes.end()) {
                encoded += codes[c];
            }
        }
        return encoded;
    }
    
    const std::map<char, std::string>& getCodes() const {
        return codes;
    }
    
    const std::map<char, int>& getFrequencies() const {
        return frequencies;
    }
    
    void displayEncodingTable() {
    std::cout << "\n+---------------------------------------------------------------+\n";
    std::cout << "|                    HUFFMAN ENCODING TABLE                    |\n";
    std::cout << "+-------------+-------------+-----------------------------------+\n";
    std::cout << "|   Char      | Frequency   | Huffman Code                      |\n";
    std::cout << "+-------------+-------------+-----------------------------------+\n";

    for (const auto& [ch, code] : codes) {
        std::string charDisplay;
        if (ch == ' ') charDisplay = "SPACE";
        else if (ch == '\n') charDisplay = "NEWLN";
        else if (ch == '\t') charDisplay = "TAB";
        else charDisplay = std::string(1, ch);

        std::cout << "| " << std::left << std::setw(11) << charDisplay
                  << " | " << std::setw(11) << frequencies.at(ch)
                  << " | " << std::setw(33) << code << " |\n";
    }

    std::cout << "+-------------+-------------+-----------------------------------+\n";
}

    
    void displayCompressionStats(const std::string& originalText) {
        int originalBits = originalText.length() * 8;
        std::string encoded = encode(originalText);
        int encodedBits = encoded.length();
        double compressionRatio = (1.0 - (double)encodedBits / originalBits) * 100;
        
       
        std::cout << "        COMPRESSION STATISTICS                       \n";
        
        std::cout << " Original Size:       " << std::setw(28) << originalBits << " bits \n";
        std::cout << " Encoded Size:        " << std::setw(28) << encodedBits << " bits \n";
        std::cout << " Compression Ratio:   " << std::setw(26) << std::fixed 
                  << std::setprecision(2) << compressionRatio << "  % \n";
        std::cout << " Space Saved:         " << std::setw(28) << (originalBits - encodedBits) << " bits \n";
        std::cout << " Unique Characters:   " << std::setw(28) << codes.size() << "      \n";
       
       
        std::cout << "\n";
    }
};

// ============================================================================
// HYBRID SORT IMPLEMENTATION (Merge Sort + Insertion Sort)
// ============================================================================

template<typename T, typename Comparator>
void insertionSort(std::vector<T>& arr, int low, int high, Comparator comp) {
    for (int i = low + 1; i <= high; i++) {
        T key = arr[i];
        int j = i - 1;
        while (j >= low && comp(key, arr[j])) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

template<typename T, typename Comparator>
void merge(std::vector<T>& arr, int low, int mid, int high, Comparator comp) {
    std::vector<T> left(arr.begin() + low, arr.begin() + mid + 1);
    std::vector<T> right(arr.begin() + mid + 1, arr.begin() + high + 1);
    
    int i = 0, j = 0, k = low;
    while (i < left.size() && j < right.size()) {
        if (comp(left[i], right[j])) {
            arr[k++] = left[i++];
        } else {
            arr[k++] = right[j++];
        }
    }
    while (i < left.size()) arr[k++] = left[i++];
    while (j < right.size()) arr[k++] = right[j++];
}

template<typename T, typename Comparator>
void hybridSort(std::vector<T>& arr, int low, int high, Comparator comp, int threshold = 16) {
    if (high - low < threshold) {
        insertionSort(arr, low, high, comp);
    } else if (low < high) {
        int mid = low + (high - low) / 2;
        hybridSort(arr, low, mid, comp, threshold);
        hybridSort(arr, mid + 1, high, comp, threshold);
        merge(arr, low, mid, high, comp);
    }
}

template<typename T, typename Comparator>
void performHybridSort(std::vector<T>& arr, Comparator comp) {
    if (!arr.empty()) {
        hybridSort(arr, 0, arr.size() - 1, comp);
    }
}

// Overload for pairs
template<typename T1, typename T2>
void performHybridSort(std::vector<std::pair<T1, T2>>& arr, int low, int high,
                       std::function<bool(const std::pair<T1, T2>&, const std::pair<T1, T2>&)> comp) {
    if (!arr.empty() && low < high) {
        hybridSort(arr, low, high, comp);
    }
}

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void createDirectory(const std::string& path) {
#ifdef _WIN32
    CreateDirectoryA(path.c_str(), NULL);
#else
    mkdir(path.c_str(), 0777);
#endif
}

void printHeader(const std::string& title) {
    std::cout << "\n" << std::string(80, '=') << "\n";
    std::cout << std::string((80 - title.length()) / 2, ' ') << title << "\n";
    std::cout << std::string(80, '=') << "\n";
}

void printSubHeader(const std::string& title) {
    std::cout << "\n" << std::string(60, '-') << "\n";
    std::cout << "  " << title << "\n";
    std::cout << std::string(60, '-') << "\n";
}

void waitForEnter() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore();
    std::cin.get();
}

// ============================================================================
// HUFFMAN ENCODING FROM FILE
// ============================================================================

void performHuffmanEncodingFromFile() {
    printHeader("HUFFMAN ENCODING & COMPRESSION");
    
    std::ifstream inFile("supabase_data.txt");
    if (!inFile.is_open()) {
        std::cerr << "Error: Could not open supabase_data.txt!\n";
        return;
    }
    
    std::stringstream buffer;
    buffer << inFile.rdbuf();
    std::string fileContent = buffer.str();
    inFile.close();
    
    if (fileContent.empty()) {
        std::cerr << "Error: supabase_data.txt is empty!\n";
        return;
    }
    
    HuffmanEncoding huffman;
    huffman.build(fileContent);
    std::string encoded = huffman.encode(fileContent);
    
    // Display encoding table and compression stats
    huffman.displayEncodingTable();
    huffman.displayCompressionStats(fileContent);
    
    // Save encoded data to file
    createDirectory("output");
    std::ofstream outFile("output/huffman_encoded_data.txt");
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not create output file!\n";
        return;
    }
    
    outFile << encoded;
    outFile.close();
    
    std::cout << "\nCombined Huffman encoding completed and saved to output/huffman_encoded_data.txt\n";
}

// ============================================================================
// SEARCH FUNCTIONALITY WITH HASH TABLE
// ============================================================================

// Simple hash table for fast O(1) search
template<typename K, typename V>
class SimpleHashTable {
private:
    struct Node {
        K key;
        V value;
        Node* next;
        Node(const K& k, const V& v) : key(k), value(v), next(nullptr) {}
    };
    
    std::vector<Node*> buckets;
    int size;
    
    int hash(const std::string& key) {
        int h = 0;
        for (char c : key) {
            h = (h * 31 + c) % size;
        }
        return h < 0 ? h + size : h;
    }
    
public:
    SimpleHashTable(int capacity = 1000) : size(capacity) {
        buckets.resize(size, nullptr);
    }
    
    ~SimpleHashTable() {
        for (auto& bucket : buckets) {
            Node* curr = bucket;
            while (curr) {
                Node* temp = curr;
                curr = curr->next;
                delete temp;
            }
        }
    }
    
    void insert(const K& key, const V& value) {
        int idx = hash(key);
        Node* newNode = new Node(key, value);
        newNode->next = buckets[idx];
        buckets[idx] = newNode;
    }
    
    bool find(const K& key, V& result) {
        int idx = hash(key);
        Node* curr = buckets[idx];
        while (curr) {
            if (curr->key == key) {
                result = curr->value;
                return true;
            }
            curr = curr->next;
        }
        return false;
    }
};

void searchInstagramProfile(const std::vector<InstagramProfile>& profiles) {
    printSubHeader("SEARCH INSTAGRAM PROFILE");
    
    // Build hash table for fast lookup
    SimpleHashTable<std::string, int> hashTable;
    for (size_t i = 0; i < profiles.size(); i++) {
        hashTable.insert(profiles[i].username, i);
    }
    
    std::cout << "\nEnter username to search: ";
    std::string username;
    std::getline(std::cin, username);
    
    int idx;
    if (hashTable.find(username, idx)) {
        const auto& profile = profiles[idx];
        std::cout << "\n" << std::string(70, '=') << "\n";
        std::cout << "PROFILE FOUND\n";
        std::cout << std::string(70, '=') << "\n";
        std::cout << "Username: " << profile.username << "\n";
        std::cout << "Bio: " << profile.bio << "\n";
        std::cout << "Followers: " << profile.followers_count << "\n";
        std::cout << "Following: " << profile.following_count << "\n";
        std::cout << "Posts: " << profile.posts_count << "\n";
        
        std::cout << std::string(70, '=') << "\n";
    } else {
        std::cout << "\nProfile not found!\n";
    }
}

void searchWhatsAppEvent(const std::vector<WhatsAppEvent>& events) {
    printSubHeader("SEARCH WHATSAPP EVENT");
    
    // Build map for fast lookup - properly handles multiple events per phone
    std::map<std::string, std::vector<int>> phoneMap;
    for (size_t i = 0; i < events.size(); i++) {
        phoneMap[events[i].phone_number].push_back(i);
    }
    
    std::cout << "\nEnter phone number to search: ";
    std::string phone;
    std::getline(std::cin, phone);
    
    auto it = phoneMap.find(phone);
    if (it != phoneMap.end() && !it->second.empty()) {
        std::cout << "\n" << std::string(70, '=') << "\n";
        std::cout << "FOUND " << it->second.size() << " EVENT(S)\n";
        std::cout << std::string(70, '=') << "\n";
        
        for (int idx : it->second) {
            const auto& event = events[idx];
            std::cout << "\nPhone Number: " << event.phone_number << "\n";
            std::cout << "Presence: " << event.presence << "\n";
            std::cout << "RTT: " << event.rtt << " ms\n";
            std::cout << "Device Count: " << event.device_count << "\n";
            std::cout << "Event Time: " << event.event_time << "\n";
            std::cout << std::string(70, '-') << "\n";
        }
    } else {
        std::cout << "\nNo events found for this phone number!\n";
    }
}



// ============================================================================
// SORTING FUNCTIONS
// ============================================================================

void sortInstagramProfiles(std::vector<InstagramProfile>& profiles) {
    printSubHeader("SORT INSTAGRAM PROFILES");
    
    std::cout << "\nSort by:\n";
    std::cout << "  1. Username (A-Z)\n";
    std::cout << "  2. Followers Count (High to Low)\n";
    std::cout << "  3. Following Count (High to Low)\n";
    std::cout << "  4. Posts Count (High to Low)\n";
    std::cout << "  5. Bio Length (Long to Short)\n";
    std::cout << "\nEnter choice: ";
    
    int choice;
    std::cin >> choice;
    std::cin.ignore();
    
    auto startTime = std::chrono::high_resolution_clock::now();
    
    switch (choice) {
        case 1:
            performHybridSort(profiles, [](const InstagramProfile& a, const InstagramProfile& b) {
                return a.username < b.username;
            });
            std::cout << "\nSorted by Username (Alphabetical)\n";
            break;
        case 2:
            performHybridSort(profiles, [](const InstagramProfile& a, const InstagramProfile& b) {
                return a.followers_count > b.followers_count;
            });
            std::cout << "\nSorted by Followers Count (Descending)\n";
            break;
        case 3:
            performHybridSort(profiles, [](const InstagramProfile& a, const InstagramProfile& b) {
                return a.following_count > b.following_count;
            });
            std::cout << "\nSorted by Following Count (Descending)\n";
            break;
        case 4:
            performHybridSort(profiles, [](const InstagramProfile& a, const InstagramProfile& b) {
                return a.posts_count > b.posts_count;
            });
            std::cout << "\nSorted by Posts Count (Descending)\n";
            break;
        case 5:
            performHybridSort(profiles, [](const InstagramProfile& a, const InstagramProfile& b) {
                return a.bio.length() > b.bio.length();
            });
            std::cout << "\nSorted by Bio Length (Descending)\n";
            break;
        default:
            std::cout << "\nInvalid choice!\n";
            return;
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    
    std::cout << "Time taken: " << duration.count() << " microseconds\n";
    
    // Display sorted results
    std::cout << "\n" << std::string(100, '-') << "\n";
    std::cout << std::left << std::setw(20) << "Username"
              << std::setw(15) << "Followers"
              << std::setw(15) << "Following"
              << std::setw(12) << "Posts"
              << std::setw(10) << "Bio Len" << "\n";
    std::cout << std::string(100, '-') << "\n";
    
    for (const auto& profile : profiles) {
        std::cout << std::left << std::setw(20) << profile.username
                  << std::setw(15) << profile.followers_count
                  << std::setw(15) << profile.following_count
                  << std::setw(12) << profile.posts_count
                  << std::setw(10) << profile.bio.length() << "\n";
    }
    std::cout << std::string(100, '-') << "\n";
}

void sortWhatsAppEvents(std::vector<WhatsAppEvent>& events) {
    printSubHeader("SORT WHATSAPP EVENTS");
    
    std::cout << "\nSort by:\n";
    std::cout << "  1. RTT (Low to High)\n";
    std::cout << "  2. Device Count (High to Low)\n";
    std::cout << "  3. Phone Number (Alphabetical)\n";
    std::cout << "  4. Presence Status (Alphabetical)\n";
    std::cout << "  5. Event Time (Chronological)\n";
    std::cout << "\nEnter choice: ";
    
    int choice;
    std::cin >> choice;
    std::cin.ignore();
    
    auto startTime = std::chrono::high_resolution_clock::now();
    
    switch (choice) {
        case 1:
            performHybridSort(events, [](const WhatsAppEvent& a, const WhatsAppEvent& b) {
                return a.rtt < b.rtt;
            });
            std::cout << "\nSorted by RTT (Ascending)\n";
            break;
        case 2:
            performHybridSort(events, [](const WhatsAppEvent& a, const WhatsAppEvent& b) {
                return a.device_count > b.device_count;
            });
            std::cout << "\nSorted by Device Count (Descending)\n";
            break;
        case 3:
            performHybridSort(events, [](const WhatsAppEvent& a, const WhatsAppEvent& b) {
                return a.phone_number < b.phone_number;
            });
            std::cout << "\nSorted by Phone Number (Alphabetical)\n";
            break;
        case 4:
            performHybridSort(events, [](const WhatsAppEvent& a, const WhatsAppEvent& b) {
                return a.presence < b.presence;
            });
            std::cout << "\nSorted by Presence Status\n";
            break;
        case 5:
            performHybridSort(events, [](const WhatsAppEvent& a, const WhatsAppEvent& b) {
                return a.event_time < b.event_time;
            });
            std::cout << "\nSorted by Event Time (Chronological)\n";
            break;
        default:
            std::cout << "\nInvalid choice!\n";
            return;
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    
    std::cout << "Time taken: " << duration.count() << " microseconds\n";
    
    // Display sorted results (first 20)
    std::cout << "\n" << std::string(90, '-') << "\n";
    std::cout << std::left << std::setw(20) << "Phone Number"
              << std::setw(15) << "Presence"
              << std::setw(12) << "RTT (ms)"
              << std::setw(15) << "Devices" << "\n";
    std::cout << std::string(90, '-') << "\n";
    
    for (size_t i = 0; i < (std::min)(events.size(), (size_t)20); i++) {
        const auto& event = events[i];
        std::cout << std::left << std::setw(20) << event.phone_number
                  << std::setw(15) << event.presence
                  << std::setw(12) << event.rtt
                  << std::setw(15) << event.device_count << "\n";
    }
    std::cout << std::string(90, '-') << "\n";
    std::cout << "Showing " << (std::min)(events.size(), (size_t)20) 
              << " of " << events.size() << " events\n";
}


int main() {
    printHeader("PROJECT CHAMELEON - DATA ANALYSIS SYSTEM");
    
    std::cout << "\n[INFO] Initializing Supabase client...\n";
    SupabaseClient client;
    SupabaseAnalyzer analyzer;
    
    // Load data
    std::cout << "[INFO] Loading data from Supabase...\n";
    
    std::vector<InstagramProfile> profiles = client.getInstagramProfiles();
    std::vector<WhatsAppEvent> events = client.getWhatsAppEvents();
    std::vector<GameEvent> games = client.getGameEvents();
    
    // Fallback to file if no data
    if (profiles.empty()) {
        std::cout << "[WARN] No profiles from Supabase, trying local file...\n";
        profiles = client.loadInstagramProfilesFromFile("supabase_data.txt");
    }
    if (events.empty()) {
        std::cout << "[WARN] No events from Supabase, trying local file...\n";
        events = client.loadWhatsAppEventsFromFile("supabase_data.txt");
    }
    if (games.empty()) {
        std::cout << "[WARN] No game events from Supabase, trying local file...\n";
        games = client.loadGameEventsFromFile("supabase_data.txt");
    }
    
    if (profiles.empty() && events.empty() && games.empty()) {
        std::cerr << "\n[ERROR] No data available! Please check your Supabase connection or data files.\n";
        return 1;
    }
    
    std::cout << "\nData loaded successfully!\n";
    std::cout << "  Instagram Profiles: " << profiles.size() << "\n";
    std::cout << "  WhatsApp Events: " << events.size() << "\n";
    std::cout << "  Game Events: " << games.size() << "\n";
    
    // Main menu loop
    bool running = true;
    while (running) {
        printHeader("MAIN MENU");
        
        std::cout << "  1. Huffman Encoding & Compression\n";
        std::cout << "  2. Sort Instagram Profiles\n";
        std::cout << "  3. Sort WhatsApp Events\n";
        std::cout << "  4. Sort Game Events\n";
        std::cout << "  5. Search Instagram Profile\n";
        std::cout << "  6. Search WhatsApp Event\n";
        std::cout << "  7. Advanced Analytics Menu\n";
        std::cout << "  0. Exit\n";
        std::cout << "\n  Enter choice: ";
        
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        
        switch (choice) {
            case 1:
                performHuffmanEncodingFromFile();
                break;
                
            case 2:
                if (profiles.empty()) {
                    std::cout << "\n[ERROR] No Instagram profiles to sort!\n";
                } else {
                    sortInstagramProfiles(profiles);
                }
                break;
                
            case 3:
                if (events.empty()) {
                    std::cout << "\n[ERROR] No WhatsApp events to sort!\n";
                } else {
                    sortWhatsAppEvents(events);
                }
                break;
                
            case 4:
                if (games.empty()) {
                    std::cout << "\n[ERROR] No Game events to sort!\n";
                } else {
                    sortGameEvents(games);
                }
                break;
                
            case 5:
                if (profiles.empty()) {
                    std::cout << "\n[ERROR] No Instagram profiles to search!\n";
                } else {
                    searchInstagramProfile(profiles);
                }
                break;
                
            case 6:
                if (events.empty()) {
                    std::cout << "\n[ERROR] No WhatsApp events to search!\n";
                } else {
                    searchWhatsAppEvent(events);
                }
                break;
                
            case 7: {
                // Advanced Analytics submenu
                bool subRunning = true;
                while (subRunning) {
                    printSubHeader("ADVANCED ANALYTICS");
                    std::cout << "\n 1. Similarity Analysis (Levenshtein Distance)\n";
                    std::cout << " 2. Duplicate Detection (Bloom Filter)\n";
                    std::cout << " 3. Device Clustering (Union-Find)\n";
                    std::cout << " 4. Bio Keyword Analysis (Inverted Index)\n";
                    std::cout << " 5. RTT Range Queries (Segment Tree)\n";
                    std::cout << " 6. Game Score Distribution\n";
                    std::cout << " 7. Top Players (by Total Score)\n";
                    std::cout << " 8. Platform Breakdown (Game Events)\n";
                    std::cout << " 0. Back to Main Menu\n";
                    std::cout << "\n Enter choice: ";
                    
                    int subChoice;
                    std::cin >> subChoice;
                    std::cin.ignore();
                    
                    switch (subChoice) {
                        case 1: {
                            printHeader("PROFILE SIMILARITY ANALYSIS");
                            if (profiles.empty()) {
                                std::cout << "\n[ERROR] No Instagram profiles available for analysis!\n";
                                // Removed waitForEnter()
                                break;
                            }
                            auto similarPairs = analyzer.findSimilarProfiles(profiles, 3);
                            std::cout << "\nFound " << similarPairs.size() << " similar profile pairs:\n\n";
                            for (const auto& pair : similarPairs) {
                                std::cout << "  - " << pair.first << " <-> " << pair.second << "\n";
                            }
                            // Removed waitForEnter()
                            break;
                        }
                        case 2: {
                            printHeader("DUPLICATE DETECTION");
                            if (profiles.empty()) {
                                std::cout << "\n[ERROR] No Instagram profiles available for analysis!\n";
                                // Removed waitForEnter()
                                break;
                            }
                            BloomFilter bf(10000, 5);
                            analyzer.buildProfileBloomFilter(profiles, bf);
                            std::cout << "\nBloom filter built with " << profiles.size() << " profiles\n";
                            std::cout << "Filter size: " << bf.getSize() << " bits\n";
                            std::cout << "Hash functions: " << bf.getNumHashFunctions() << "\n";
                            // Removed waitForEnter()
                            break;
                        }
                        case 3: {
                            printHeader("DEVICE CLUSTERING");
                            if (events.empty()) {
                                std::cout << "\n[ERROR] No WhatsApp events available for analysis!\n";
                                // Removed waitForEnter()
                                break;
                            }
                            auto clusters = analyzer.detectDeviceClusters(events);
                            std::cout << "\nDetected " << clusters.size() << " device clusters:\n\n";
                            int clusterNum = 1;
                            for (const auto& [id, devices] : clusters) {
                                std::cout << "Cluster " << clusterNum++ << " (" << devices.size() << " devices):\n";
                                for (size_t i = 0; i < (std::min)((size_t)3, devices.size()); i++) {
                                    std::cout << "  - " << devices[i] << "\n";
                                }
                                if (devices.size() > 3) {
                                    std::cout << "  ... and " << (devices.size() - 3) << " more\n";
                                }
                                std::cout << "\n";
                            }
                            // Removed waitForEnter()
                            break;
                        }
                        case 4: {
                            printHeader("BIO KEYWORD ANALYSIS");
                            if (profiles.empty()) {
                                std::cout << "\n[ERROR] No Instagram profiles available for analysis!\n";
                                // Removed waitForEnter()
                                break;
                            }
                            auto wordFreq = analyzer.analyzeCaptionWords(profiles);
                            
                            std::vector<std::pair<std::string, int>> sortedWords(wordFreq.begin(), wordFreq.end());
                            
                            // Use standard sort for pairs
                            std::sort(sortedWords.begin(), sortedWords.end(),
                                [](const auto& a, const auto& b) { return a.second > b.second; });
                            
                            std::cout << "\nTop 20 Most Common Words:\n\n";
                            std::cout << std::left << std::setw(20) << "Word" << "Frequency\n";
                            std::cout << std::string(40, '-') << "\n";
                            
                            for (size_t i = 0; i < (std::min)((size_t)20, sortedWords.size()); i++) {
                                std::cout << std::left << std::setw(20) << sortedWords[i].first 
                                         << sortedWords[i].second << "\n";
                            }
                            // Removed waitForEnter()
                            break;
                        }
                        case 5: {
                            printHeader("RTT RANGE QUERIES");
                            if (events.empty()) {
                                std::cout << "\n[ERROR] No WhatsApp events available for analysis!\n";
                                // Removed waitForEnter()
                                break;
                            }
                            
                            // Collect RTT values
                            std::vector<int> rttValues;
                            for (const auto& event : events) {
                                if (event.rtt > 0) {
                                    rttValues.push_back(event.rtt);
                                }
                            }
                            
                            if (rttValues.empty()) {
                                std::cout << "\n[ERROR] No RTT values available!\n";
                                // Removed waitForEnter()
                                break;
                            }
                            
                            SegmentTree st(rttValues);
                            
                            std::cout << "\nEnter minimum RTT (ms): ";
                            int minRTT;
                            std::cin >> minRTT;
                            std::cout << "Enter maximum RTT (ms): ";
                            int maxRTT;
                            std::cin >> maxRTT;
                            std::cin.ignore();
                            
                            auto rangeEvents = analyzer.findEventsByRTTRange(events, minRTT, maxRTT);
                            std::cout << "\nFound " << rangeEvents.size() << " events in range [" 
                                            << minRTT << ", " << maxRTT << "]:\n\n";
                            
                            for (size_t i = 0; i < (std::min)((size_t)10, rangeEvents.size()); i++) {
                                std::cout << " - " << rangeEvents[i].phone_number 
                                         << " | RTT: " << rangeEvents[i].rtt << "ms\n";
                            }
                            if (rangeEvents.size() > 10) {
                                std::cout << " ... and " << (rangeEvents.size() - 10) << " more\n";
                            }
                            // Removed waitForEnter()
                            break;
                        }
                        case 6: {
                            printHeader("GAME SCORE DISTRIBUTION");
                            if (games.empty()) {
                                std::cout << "\n[ERROR] No game events available for analysis!\n";
                                // Removed waitForEnter()
                                break;
                            }
                            std::map<int, int> histogram;
                            for (const auto& g : games) histogram[g.score]++;
                            std::vector<std::pair<int,int>> buckets(histogram.begin(), histogram.end());
                            std::sort(buckets.begin(), buckets.end(), [](auto &a, auto &b){ return a.first < b.first; });
                            std::cout << "\nScore | Count\n" << std::string(25, '-') << "\n";
                            for (const auto &p : buckets) {
                                std::cout << std::left << std::setw(6) << p.first << " | " << p.second << "\n";
                            }
                            // Removed waitForEnter()
                            break;
                        }
                        case 7: {
                            printHeader("TOP PLAYERS BY TOTAL SCORE");
                            if (games.empty()) {
                                std::cout << "\n[ERROR] No game events available for analysis!\n";
                                // Removed waitForEnter()
                                break;
                            }
                            std::map<std::string, long long> totals;
                            for (const auto& g : games) totals[g.player_id] += g.score;
                            std::vector<std::pair<std::string,long long>> vec(totals.begin(), totals.end());
                            std::sort(vec.begin(), vec.end(), [](auto &a, auto &b){ return a.second > b.second; });
                            std::cout << "\nPlayer ID" << std::setw(20) << "" << "Total Score\n" << std::string(50, '-') << "\n";
                            for (size_t i = 0; i < (std::min)((size_t)20, vec.size()); ++i) {
                                std::cout << std::left << std::setw(30) << vec[i].first << vec[i].second << "\n";
                            }
                            // Removed waitForEnter()
                            break;
                        }
                        case 8: {
                            printHeader("PLATFORM BREAKDOWN (GAME EVENTS)");
                            if (games.empty()) {
                                std::cout << "\n[ERROR] No game events available for analysis!\n";
                                // Removed waitForEnter()
                                break;
                            }
                            std::map<std::string, int> counts;
                            for (const auto& g : games) counts[g.platform]++;
                            std::cout << "\nPlatform | Count\n" << std::string(30, '-') << "\n";
                            for (const auto &p : counts) {
                                std::cout << std::left << std::setw(15) << p.first << " | " << p.second << "\n";
                            }
                            // Removed waitForEnter()
                            break;
                        }
                        case 0:
                            subRunning = false;
                            break;
                        default:
                            std::cout << "\nInvalid choice!\n";
                            // Removed waitForEnter()
                    }
                }
                break;
            }
                
            case 0:
                printHeader("THANK YOU");
                std::cout << "\nExiting Project Chameleon...\n";
                std::cout << "All encoded data saved in: output/huffman_encoded_data.txt\n\n";
                running = false;
                break;
                
            default:
                std::cout << "\n[ERROR] Invalid choice! Please try again.\n";
                // Removed waitForEnter()
        }
    }
    
    return 0;
}

// ============================================================================
// SORT GAME EVENTS IMPLEMENTATION
// ============================================================================

void sortGameEvents(std::vector<GameEvent>& events) {
    printSubHeader("SORT GAME EVENTS");
    std::cout << "\nSort by:\n";
    std::cout << " 1. Score (High to Low)\n";
    std::cout << " 2. Game Name (A-Z)\n";
    std::cout << " 3. Player ID (A-Z)\n";
    std::cout << " 4. Event Time (Chronological)\n";
    std::cout << "\nEnter choice: ";

    int choice;
    std::cin >> choice;
    std::cin.ignore();

    auto startTime = std::chrono::high_resolution_clock::now();

    switch (choice) {
        case 1:
            performHybridSort(events, [](const GameEvent& a, const GameEvent& b) { 
                return a.score > b.score; 
            });
            std::cout << "\n[OK] Sorted by Score (Descending)\n";
            break;
        case 2:
            performHybridSort(events, [](const GameEvent& a, const GameEvent& b) { 
                return a.game_name < b.game_name; 
            });
            std::cout << "\n[OK] Sorted by Game Name (Alphabetical)\n";
            break;
        case 3:
            performHybridSort(events, [](const GameEvent& a, const GameEvent& b) { 
                return a.player_id < b.player_id; 
            });
            std::cout << "\n[OK] Sorted by Player ID (Alphabetical)\n";
            break;
        case 4:
            performHybridSort(events, [](const GameEvent& a, const GameEvent& b) { 
                return a.event_time < b.event_time; 
            });
            std::cout << "\n[OK] Sorted by Event Time (Chronological)\n";
            break;
        default:
            std::cout << "\nInvalid choice!\n";
            return;
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    std::cout << "Time taken: " << duration.count() << " microseconds\n";

    // Display top results
    std::cout << "\n" << std::string(90, '-') << "\n";
    std::cout << std::left << std::setw(20) << "Game Name" 
              << std::setw(20) << "Player ID" 
              << std::setw(12) << "Score" 
              << std::setw(20) << "Platform" << "\n";
    std::cout << std::string(90, '-') << "\n";
    
    for (size_t i = 0; i < (std::min)(events.size(), (size_t)20); i++) {
        const auto& e = events[i];
        std::cout << std::left << std::setw(20) << e.game_name 
                  << std::setw(20) << e.player_id 
                  << std::setw(12) << e.score 
                  << std::setw(20) << e.platform << "\n";
    }
    std::cout << std::string(90, '-') << "\n";
    std::cout << "Showing " << (std::min)(events.size(), (size_t)20) 
              << " of " << events.size() << " events\n";
}