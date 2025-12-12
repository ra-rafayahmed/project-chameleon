#ifndef SUPABASE_ANALYZER_H
#define SUPABASE_ANALYZER_H

#include "supabase_models.h"
#include "trie.h"
#include "inverted_index.h"
#include "bloom_filter.h"
#include "union_find.h"
#include "segment_tree.h"
#include <vector>
#include <map>
#include <algorithm>
#include <unordered_map>

class SupabaseAnalyzer {
public:
    // ========== INSTAGRAM PROFILE ANALYSIS ==========
    
    // Analyze Instagram profiles and return engagement metrics
    std::vector<InstagramEngagementMetrics> analyzeInstagramProfiles(
        const std::vector<InstagramProfile>& profiles);
    
    // Extract and analyze caption text using Trie and InvertedIndex
    std::unordered_map<std::string, int> analyzeCaptionWords(
        const std::vector<InstagramProfile>& profiles);
    
    // Find similar Instagram profiles using Levenshtein distance
    std::vector<std::pair<std::string, std::string>> findSimilarProfiles(
        const std::vector<InstagramProfile>& profiles, int threshold = 3);
    
    // Build bloom filter for quick profile lookup
    void buildProfileBloomFilter(
        const std::vector<InstagramProfile>& profiles, BloomFilter& filter);
    
    // ========== WHATSAPP EVENTS ANALYSIS ==========
    
    // Compute presence statistics for WhatsApp events
    std::vector<UserDeviceStats> analyzeWhatsAppEvents(
        const std::vector<WhatsAppEvent>& events);
    
    // Compute RTT statistics using SegmentTree for range queries
    void buildRTTSegmentTree(const std::vector<WhatsAppEvent>& events, SegmentTree& tree);
    
    // Find events within RTT range using SegmentTree
    std::vector<WhatsAppEvent> findEventsByRTTRange(
        const std::vector<WhatsAppEvent>& events, int minRTT, int maxRTT);
    
    // Detect device clusters using UnionFind
    std::unordered_map<std::string, std::vector<std::string>> detectDeviceClusters(
        const std::vector<WhatsAppEvent>& events);
    
    // Analyze presence patterns (available -> recording -> unavailable transitions)
    std::map<std::string, int> analyzePresenceTransitions(
        const std::vector<WhatsAppEvent>& events);
    
    // ========== COMBINED ANALYSIS ==========
    
    // Get summary statistics across all data
    void printAnalysisSummary(
        const std::vector<InstagramProfile>& profiles,
        const std::vector<WhatsAppEvent>& events);
    
private:
    // Helper methods
    std::vector<std::string> tokenizeText(const std::string& text);
    std::string normalizeWord(const std::string& word);
    int levenshteinDistance(const std::string& s1, const std::string& s2);
    int computeAverageRTT(const std::vector<int>& rttValues);
    int computeMinRTT(const std::vector<int>& rttValues);
    int computeMaxRTT(const std::vector<int>& rttValues);
};

#endif
