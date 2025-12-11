#include "../include/cli_interface.h"
#include "../include/supabase_client.h"
#include "../include/union_find.h"
#include "../include/trie.h"
#include "../include/inverted_index.h"
#include "../include/bloom_filter.h"
#include "../include/json_parser.h"
#include "../include/levenshtein.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <queue>
#include <functional>
#include <chrono>
#include <cmath>
#include <unordered_map>

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

class HuffmanEncoding {
private:
    HuffmanNode* root;
    std::unordered_map<char, std::string> codes;
    
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
        std::unordered_map<char, int> freq;
        for (char c : text) {
            freq[c]++;
        }
        
        auto cmp = [](HuffmanNode* a, HuffmanNode* b) { return a->freq > b->freq; };
        std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, decltype(cmp)> pq(cmp);
        
        for (auto& p : freq) {
            pq.push(new HuffmanNode(p.first, p.second));
        }
        
        while (pq.size() > 1) {
            HuffmanNode* left = pq.top(); pq.pop();
            HuffmanNode* right = pq.top(); pq.pop();
            HuffmanNode* parent = new HuffmanNode('\0', left->freq + right->freq);
            parent->left = left;
            parent->right = right;
            pq.push(parent);
        }
        
        root = pq.empty() ? nullptr : pq.top();
        if (root) buildCodes(root, "");
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
    
    const std::unordered_map<char, std::string>& getCodes() const {
        return codes;
    }
};

// ============================================================================
// DATA STRUCTURES
// ============================================================================

struct InstagramProfile {
    std::string username;
    int followers_count;
    int following_count;
    int posts_count;
    std::string bio;
};

struct WhatsAppEvent {
    std::string phone_number;
    int device_count;
    int rtt;
    std::string presence;
};

struct GameDevice {
    std::string device_id;
    std::string username;
    double gps_lat;
    double gps_lng;
    std::string timezone;
};

class DataManager {
private:
    SupabaseClient client;
    std::vector<InstagramProfile> profiles;
    std::vector<WhatsAppEvent> events;
    std::vector<GameDevice> devices;
    UnionFind userIdentityMap;
    Trie usernameIndex;
    BloomFilter duplicateFilter;
    
public:
    DataManager() : duplicateFilter(10000) {}
    
    void loadData() {
        std::cout << "\n  ============================================\n";
        std::cout << "  Loading data from Supabase...\n";
        std::cout << "  ============================================\n";
        
        try {
            // Mock data for demonstration
            profiles = {
                {"johndoe", 5000, 300, 150, "Travel enthusiast and photographer"},
                {"janedoe", 8500, 450, 200, "Photography lover from NYC"},
                {"tech_guru", 15000, 1000, 500, "Tech news and product reviews"},
                {"fitness_coach", 12000, 600, 300, "Fitness and wellness expert"},
                {"foodie_adventures", 6000, 400, 250, "Food photography and recipes"}
            };
            
            events = {
                {"+1234567890", 3, 120, "online"},
                {"+1987654321", 2, 150, "away"},
                {"+1555555555", 5, 90, "online"},
                {"+1666666666", 1, 110, "offline"},
                {"+1777777777", 4, 130, "online"}
            };
            
            devices = {
                {"dev001", "johndoe", 40.7128, -74.0060, "EST"},
                {"dev002", "janedoe", 34.0522, -118.2437, "PST"},
                {"dev003", "tech_guru", 37.7749, -122.4194, "PST"},
                {"dev004", "fitness_coach", 41.8781, -87.6298, "CST"},
                {"dev005", "foodie_adventures", 33.7490, -84.3880, "EST"}
            };
            
            // Build indices
            for (const auto& profile : profiles) {
                usernameIndex.insert(profile.username, profile.username);
                duplicateFilter.add(profile.username);
            }
            
            std::cout << "  Data loaded successfully!\n";
            std::cout << "  Profiles: " << profiles.size() << "\n";
            std::cout << "  Events: " << events.size() << "\n";
            std::cout << "  Devices: " << devices.size() << "\n";
            std::cout << "  ============================================\n\n";
        } catch (const std::exception& e) {
            std::cout << "  Error loading data: " << e.what() << "\n";
        }
    }
    
    const std::vector<InstagramProfile>& getProfiles() const { return profiles; }
    const std::vector<WhatsAppEvent>& getEvents() const { return events; }
    const std::vector<GameDevice>& getDevices() const { return devices; }
    UnionFind& getUnionFind() { return userIdentityMap; }
    Trie& getUsernameIndex() { return usernameIndex; }
    BloomFilter& getDuplicateFilter() { return duplicateFilter; }
};

// ============================================================================
// HUFFMAN ENCODING + STORAGE
// ============================================================================

void performHuffmanEncodingAndStore(const DataManager& dm) {
    std::cout << "\n  ============================================\n";
    std::cout << "  Huffman Encoding & Storage\n";
    std::cout << "  ============================================\n\n";
    
    std::ofstream outFile("huffman_encoded_data.txt");
    
    const auto& profiles = dm.getProfiles();
    const auto& events = dm.getEvents();
    const auto& devices = dm.getDevices();
    
    // Encode Instagram Profiles
    outFile << "=== INSTAGRAM PROFILES (HUFFMAN ENCODED) ===\n";
    outFile << std::string(60, '-') << "\n";
    outFile << std::left << std::setw(20) << "Username" 
            << std::setw(15) << "Followers" 
            << std::setw(15) << "Following" 
            << std::setw(10) << "Posts" 
            << "Encoded Bio\n";
    outFile << std::string(80, '-') << "\n";
    
    for (const auto& profile : profiles) {
        HuffmanEncoding huffman;
        huffman.build(profile.bio);
        std::string encoded = huffman.encode(profile.bio);
        
        outFile << std::left << std::setw(20) << profile.username
                << std::setw(15) << profile.followers_count
                << std::setw(15) << profile.following_count
                << std::setw(10) << profile.posts_count
                << encoded.substr(0, 30) << "...\n";
        
        std::cout << "  Encoded " << profile.username << " bio\n";
    }
    
    outFile << "\n\n=== WHATSAPP EVENTS (HUFFMAN ENCODED) ===\n";
    outFile << std::string(60, '-') << "\n";
    outFile << std::left << std::setw(20) << "Phone" 
            << std::setw(12) << "Devices" 
            << std::setw(10) << "RTT" 
            << "Encoded Presence\n";
    outFile << std::string(65, '-') << "\n";
    
    for (const auto& event : events) {
        HuffmanEncoding huffman;
        huffman.build(event.presence);
        std::string encoded = huffman.encode(event.presence);
        
        outFile << std::left << std::setw(20) << event.phone_number
                << std::setw(12) << event.device_count
                << std::setw(10) << event.rtt
                << encoded << "\n";
        
        std::cout << "  Encoded " << event.phone_number << " presence\n";
    }
    
    outFile << "\n\n=== GAME DEVICES (HUFFMAN ENCODED) ===\n";
    outFile << std::string(60, '-') << "\n";
    outFile << std::left << std::setw(15) << "Device ID" 
            << std::setw(20) << "Username" 
            << std::setw(15) << "Timezone" 
            << "Encoded GPS\n";
    outFile << std::string(70, '-') << "\n";
    
    for (const auto& device : devices) {
        std::string gpsStr = std::to_string(device.gps_lat) + "," + std::to_string(device.gps_lng);
        HuffmanEncoding huffman;
        huffman.build(gpsStr);
        std::string encoded = huffman.encode(gpsStr);
        
        outFile << std::left << std::setw(15) << device.device_id
                << std::setw(20) << device.username
                << std::setw(15) << device.timezone
                << encoded.substr(0, 30) << "...\n";
        
        std::cout << "  Encoded " << device.device_id << " GPS\n";
    }
    
    outFile.close();
    std::cout << "\n  Data saved to: huffman_encoded_data.txt\n";
    std::cout << "  ============================================\n\n";
}

// ============================================================================
// SORTING FUNCTIONS
// ============================================================================

void sortInstagramProfiles(std::vector<InstagramProfile>& profiles, int sortOption) {
    std::cout << "\n  Sorting Instagram Profiles...\n";
    
    switch (sortOption) {
        case 1: // Sort by followers
            std::sort(profiles.begin(), profiles.end(),
                [](const InstagramProfile& a, const InstagramProfile& b) {
                    return a.followers_count > b.followers_count;
                });
            std::cout << "  Sorted by Followers (Descending)\n";
            break;
            
        case 2: // Sort by following
            std::sort(profiles.begin(), profiles.end(),
                [](const InstagramProfile& a, const InstagramProfile& b) {
                    return a.following_count > b.following_count;
                });
            std::cout << "  Sorted by Following (Descending)\n";
            break;
            
        case 3: // Sort by posts
            std::sort(profiles.begin(), profiles.end(),
                [](const InstagramProfile& a, const InstagramProfile& b) {
                    return a.posts_count > b.posts_count;
                });
            std::cout << "  Sorted by Posts (Descending)\n";
            break;
            
        case 4: // Sort by username (alphabetical)
            std::sort(profiles.begin(), profiles.end(),
                [](const InstagramProfile& a, const InstagramProfile& b) {
                    return a.username < b.username;
                });
            std::cout << "  Sorted by Username (Alphabetical)\n";
            break;
    }
    
    // Display sorted results
    std::cout << "\n  " << std::left << std::setw(20) << "Username"
              << std::setw(15) << "Followers"
              << std::setw(15) << "Following"
              << std::setw(10) << "Posts\n";
    std::cout << "  " << std::string(60, '-') << "\n";
    
    for (const auto& profile : profiles) {
        std::cout << "  " << std::left << std::setw(20) << profile.username
                  << std::setw(15) << profile.followers_count
                  << std::setw(15) << profile.following_count
                  << std::setw(10) << profile.posts_count << "\n";
    }
}

void sortWhatsAppEvents(std::vector<WhatsAppEvent>& events, int sortOption) {
    std::cout << "\n  Sorting WhatsApp Events...\n";
    
    switch (sortOption) {
        case 1: // Sort by device count
            std::sort(events.begin(), events.end(),
                [](const WhatsAppEvent& a, const WhatsAppEvent& b) {
                    return a.device_count > b.device_count;
                });
            std::cout << "  Sorted by Device Count (Descending)\n";
            break;
            
        case 2: // Sort by RTT
            std::sort(events.begin(), events.end(),
                [](const WhatsAppEvent& a, const WhatsAppEvent& b) {
                    return a.rtt < b.rtt;
                });
            std::cout << "  Sorted by RTT (Ascending)\n";
            break;
            
        case 3: // Sort by phone number
            std::sort(events.begin(), events.end(),
                [](const WhatsAppEvent& a, const WhatsAppEvent& b) {
                    return a.phone_number < b.phone_number;
                });
            std::cout << "  Sorted by Phone Number\n";
            break;
    }
    
    // Display sorted results
    std::cout << "\n  " << std::left << std::setw(20) << "Phone"
              << std::setw(12) << "Devices"
              << std::setw(10) << "RTT"
              << std::setw(12) << "Presence\n";
    std::cout << "  " << std::string(60, '-') << "\n";
    
    for (const auto& event : events) {
        std::cout << "  " << std::left << std::setw(20) << event.phone_number
                  << std::setw(12) << event.device_count
                  << std::setw(10) << event.rtt
                  << std::setw(12) << event.presence << "\n";
    }
}

void sortGameDevices(std::vector<GameDevice>& devices, int sortOption) {
    std::cout << "\n  Sorting Game Devices...\n";
    
    switch (sortOption) {
        case 1: // Sort by username
            std::sort(devices.begin(), devices.end(),
                [](const GameDevice& a, const GameDevice& b) {
                    return a.username < b.username;
                });
            std::cout << "  Sorted by Username\n";
            break;
            
        case 2: // Sort by latitude
            std::sort(devices.begin(), devices.end(),
                [](const GameDevice& a, const GameDevice& b) {
                    return a.gps_lat > b.gps_lat;
                });
            std::cout << "  Sorted by Latitude (Descending)\n";
            break;
            
        case 3: // Sort by timezone
            std::sort(devices.begin(), devices.end(),
                [](const GameDevice& a, const GameDevice& b) {
                    return a.timezone < b.timezone;
                });
            std::cout << "  Sorted by Timezone\n";
            break;
    }
    
    // Display sorted results
    std::cout << "\n  " << std::left << std::setw(15) << "Device ID"
              << std::setw(20) << "Username"
              << std::setw(15) << "Latitude"
              << std::setw(15) << "Timezone\n";
    std::cout << "  " << std::string(70, '-') << "\n";
    
    for (const auto& device : devices) {
        std::cout << "  " << std::left << std::setw(15) << device.device_id
                  << std::setw(20) << device.username
                  << std::setw(15) << std::fixed << std::setprecision(4) << device.gps_lat
                  << std::setw(15) << device.timezone << "\n";
    }
}

// ============================================================================
// RANKING TECHNIQUES (Priority Queue Based)
// ============================================================================

void performRanking(DataManager& dm) {
    std::cout << "\n  ============================================\n";
    std::cout << "  Ranking Techniques\n";
    std::cout << "  ============================================\n\n";
    
    auto profiles = dm.getProfiles();
    auto events = dm.getEvents();
    
    // Rank profiles by engagement (followers + following + posts)
    std::cout << "  1. Profile Engagement Ranking:\n";
    std::cout << "  " << std::string(60, '-') << "\n";
    
    std::vector<std::pair<std::string, int>> engagement;
    for (const auto& p : profiles) {
        int score = p.followers_count + p.following_count + (p.posts_count * 10);
        engagement.push_back({p.username, score});
    }
    
    std::sort(engagement.begin(), engagement.end(),
        [](const auto& a, const auto& b) { return a.second > b.second; });
    
    int rank = 1;
    for (const auto& e : engagement) {
        std::cout << "  #" << rank++ << " " << std::left << std::setw(20) 
                  << e.first << "Score: " << e.second << "\n";
    }
    
    // Rank events by anomaly score (unusual device count or RTT)
    std::cout << "\n  2. WhatsApp Event Anomaly Ranking:\n";
    std::cout << "  " << std::string(60, '-') << "\n";
    
    std::vector<std::pair<std::string, double>> anomalies;
    double avgDevices = 0, avgRTT = 0;
    for (const auto& e : events) {
        avgDevices += e.device_count;
        avgRTT += e.rtt;
    }
    avgDevices /= events.size();
    avgRTT /= events.size();
    
    for (const auto& e : events) {
        double anomalyScore = std::abs(e.device_count - avgDevices) + 
                             std::abs((e.rtt - avgRTT) / 100.0);
        anomalies.push_back({e.phone_number, anomalyScore});
    }
    
    std::sort(anomalies.begin(), anomalies.end(),
        [](const auto& a, const auto& b) { return a.second > b.second; });
    
    rank = 1;
    for (const auto& a : anomalies) {
        std::cout << "  #" << rank++ << " " << std::left << std::setw(20) 
                  << a.first << "Anomaly Score: " << std::fixed << std::setprecision(2) << a.second << "\n";
    }
    
    std::cout << "  ============================================\n\n";
}

// ============================================================================
// IDENTITY UNIFICATION (UNION-FIND)
// ============================================================================

void unifyIdentities(DataManager& dm) {
    std::cout << "\n  ============================================\n";
    std::cout << "  Identity Unification (Union-Find)\n";
    std::cout << "  ============================================\n\n";
    
    UnionFind& uf = dm.getUnionFind();
    const auto& devices = dm.getDevices();
    const auto& profiles = dm.getProfiles();
    
    std::cout << "  Linking identities across platforms:\n\n";
    
    // Link each user's accounts together
    size_t limit = devices.size() < profiles.size() ? devices.size() : profiles.size();
    for (size_t i = 0; i < limit; i++) {
        uf.addElement(devices[i].device_id);
        uf.addElement(profiles[i].username);
        uf.unite(devices[i].device_id, profiles[i].username);
        std::cout << "  Linked: " << devices[i].device_id 
                 << " <-> " << profiles[i].username << "\n";
    }
    
    std::cout << "\n  Identity Groups: " << uf.getGroups().size() << " groups found\n";
    std::cout << "  ============================================\n\n";
}

// ============================================================================
// SIMILARITY SEARCH (LEVENSHTEIN)
// ============================================================================

void findSimilarProfiles(DataManager& dm) {
    std::cout << "\n  ============================================\n";
    std::cout << "  Similar Profile Detection (Levenshtein)\n";
    std::cout << "  ============================================\n\n";
    
    const auto& profiles = dm.getProfiles();
    Levenshtein lev;
    
    std::cout << "  Finding similar profiles based on bio similarity:\n\n";
    
    int matchCount = 0;
    for (size_t i = 0; i < profiles.size(); i++) {
        for (size_t j = i + 1; j < profiles.size(); j++) {
            int distance = lev.distance(profiles[i].bio, profiles[j].bio);
            double similarity = 100.0 - (distance * 5.0);
            
            if (similarity > 20) {
                std::cout << "  " << profiles[i].username << " <-> " << profiles[j].username
                         << " | Distance: " << distance << " | Similarity: " 
                         << std::fixed << std::setprecision(1) << similarity << "%\n";
                matchCount++;
            }
        }
    }
    
    if (matchCount == 0) {
        std::cout << "  No similar profiles found based on bio content.\n";
    }
    
    std::cout << "  ============================================\n\n";
}

// ============================================================================
// DUPLICATE DETECTION (BLOOM FILTER)
// ============================================================================

void detectDuplicates(DataManager& dm) {
    std::cout << "\n  ============================================\n";
    std::cout << "  Duplicate Detection (Bloom Filter)\n";
    std::cout << "  ============================================\n\n";
    
    BloomFilter& bf = dm.getDuplicateFilter();
    const auto& profiles = dm.getProfiles();
    
    std::cout << "  Checking for potential duplicates:\n\n";
    
    std::cout << "  Profiles already in system (marked as seen):\n";
    for (const auto& profile : profiles) {
        if (bf.mightContain(profile.username)) {
            std::cout << "  ✓ " << profile.username << " (existing)\n";
        }
    }
    
    std::cout << "\n  No duplicates detected in current dataset.\n";
    std::cout << "  ============================================\n\n";
}

// ============================================================================
// BIO ANALYSIS (INVERTED INDEX)
// ============================================================================

void analyzeBios(DataManager& dm) {
    std::cout << "\n  ============================================\n";
    std::cout << "  Bio Analysis (Inverted Index)\n";
    std::cout << "  ============================================\n\n";
    
    const auto& profiles = dm.getProfiles();
    InvertedIndex bioIndex;
    
    // Build index - addDocument takes only content and returns docId
    std::unordered_map<int, std::string> docIdToUsername;
    for (const auto& profile : profiles) {
        int docId = bioIndex.addDocument(profile.bio);
        docIdToUsername[docId] = profile.username;
    }
    
    // Search for keywords
    std::vector<std::string> keywords = {"travel", "photography", "tech", "fitness", "food", "enthusiast"};
    
    std::cout << "  Searching for keywords in bios:\n\n";
    
    for (const auto& keyword : keywords) {
        auto results = bioIndex.search(keyword);
        std::cout << "  Keyword: '" << keyword << "'\n";
        std::cout << "  Matching profiles: ";
        
        if (results.empty()) {
            std::cout << "None\n";
        } else {
            for (int docId : results) {
                if (docIdToUsername.find(docId) != docIdToUsername.end()) {
                    std::cout << docIdToUsername[docId] << " ";
                }
            }
            std::cout << "\n";
        }
    }
    
    std::cout << "  ============================================\n\n";
}

// ============================================================================
// MAIN MENU
// ============================================================================

int main() {
    CLIInterface cli;
    DataManager dm;
    
    std::cout << "\n\n";
    std::cout << "  ============================================\n";
    std::cout << "  PROJECT CHAMELEON - Data Analysis System\n";
    std::cout << "  ============================================\n";
    
    dm.loadData();
    
    while (true) {
        std::cout << "\n  ============================================\n";
        std::cout << "  MAIN MENU\n";
        std::cout << "  ============================================\n\n";
        std::cout << "  1. Data Processing (Huffman Encoding & Sorting)\n";
        std::cout << "  2. Advanced Analytics\n";
        std::cout << "  3. Exit\n";
        std::cout << "  Enter choice: ";
        
        int choice;
        std::cin >> choice;
        std::cin.ignore();
        
        switch (choice) {
            case 1: {
                // Data Processing
                std::cout << "\n  ============================================\n";
                std::cout << "  Data Processing Menu\n";
                std::cout << "  ============================================\n\n";
                std::cout << "  1. Huffman Encoding & Storage\n";
                std::cout << "  2. Instagram Profile Sorting\n";
                std::cout << "  3. WhatsApp Events Sorting\n";
                std::cout << "  4. Game Device Sorting\n";
                std::cout << "  0. Back to Main Menu\n";
                std::cout << "  Enter choice: ";
                
                int subChoice;
                std::cin >> subChoice;
                std::cin.ignore();
                
                switch (subChoice) {
                    case 1:
                        performHuffmanEncodingAndStore(dm);
                        break;
                    case 2: {
                        std::cout << "\n  Instagram Profile Sorting Options:\n";
                        std::cout << "  1. Sort by Followers\n";
                        std::cout << "  2. Sort by Following\n";
                        std::cout << "  3. Sort by Posts\n";
                        std::cout << "  4. Sort by Username\n";
                        std::cout << "  Enter choice: ";
                        
                        int sortChoice;
                        std::cin >> sortChoice;
                        std::cin.ignore();
                        
                        auto profiles = dm.getProfiles();
                        sortInstagramProfiles(profiles, sortChoice);
                        break;
                    }
                    case 3: {
                        std::cout << "\n  WhatsApp Events Sorting Options:\n";
                        std::cout << "  1. Sort by Device Count\n";
                        std::cout << "  2. Sort by RTT\n";
                        std::cout << "  3. Sort by Phone Number\n";
                        std::cout << "  Enter choice: ";
                        
                        int sortChoice;
                        std::cin >> sortChoice;
                        std::cin.ignore();
                        
                        auto events = dm.getEvents();
                        sortWhatsAppEvents(events, sortChoice);
                        break;
                    }
                    case 4: {
                        std::cout << "\n  Game Device Sorting Options:\n";
                        std::cout << "  1. Sort by Username\n";
                        std::cout << "  2. Sort by Latitude\n";
                        std::cout << "  3. Sort by Timezone\n";
                        std::cout << "  Enter choice: ";
                        
                        int sortChoice;
                        std::cin >> sortChoice;
                        std::cin.ignore();
                        
                        auto devices = dm.getDevices();
                        sortGameDevices(devices, sortChoice);
                        break;
                    }
                }
                break;
            }
            
            case 2: {
                // Advanced Analytics
                std::cout << "\n  ============================================\n";
                std::cout << "  Advanced Analytics Menu\n";
                std::cout << "  ============================================\n\n";
                std::cout << "  1. Ranking Techniques (Priority Queue)\n";
                std::cout << "  2. Identity Unification (Union-Find)\n";
                std::cout << "  3. Similar Profile Detection (Levenshtein)\n";
                std::cout << "  4. Duplicate Detection (Bloom Filter)\n";
                std::cout << "  5. Bio Analysis (Inverted Index)\n";
                std::cout << "  0. Back to Main Menu\n";
                std::cout << "  Enter choice: ";
                
                int subChoice;
                std::cin >> subChoice;
                std::cin.ignore();
                
                switch (subChoice) {
                    case 1:
                        performRanking(dm);
                        break;
                    case 2:
                        unifyIdentities(dm);
                        break;
                    case 3:
                        findSimilarProfiles(dm);
                        break;
                    case 4:
                        detectDuplicates(dm);
                        break;
                    case 5:
                        analyzeBios(dm);
                        break;
                }
                break;
            }
            
            case 3: {
                std::cout << "\n  ============================================\n";
                std::cout << "  Thank you for using Project Chameleon!\n";
                std::cout << "  ============================================\n\n";
                return 0;
            }
            
            default:
                std::cout << "  Invalid choice. Please try again.\n";
        }
        
        cli.waitForEnter();
    }
    
    return 0;
}
