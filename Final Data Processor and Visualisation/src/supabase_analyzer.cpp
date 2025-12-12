#include "../include/supabase_analyzer.h"
#include <sstream>
#include <cctype>
#include <cmath>
#include <numeric>
#include <iostream>
#include <iomanip>

std::vector<std::string> SupabaseAnalyzer::tokenizeText(const std::string& text) {
    std::vector<std::string> tokens;
    std::stringstream ss(text);
    std::string token;
    
    while (ss >> token) {
        // Remove punctuation
        token.erase(std::remove_if(token.begin(), token.end(),
            [](unsigned char c) { return !std::isalnum(c); }), token.end());
        
        if (!token.empty()) {
            tokens.push_back(normalizeWord(token));
        }
    }
    
    return tokens;
}

std::string SupabaseAnalyzer::normalizeWord(const std::string& word) {
    std::string normalized = word;
    std::transform(normalized.begin(), normalized.end(), 
                   normalized.begin(), ::tolower);
    return normalized;
}

int SupabaseAnalyzer::levenshteinDistance(const std::string& s1, const std::string& s2) {
    size_t m = s1.length();
    size_t n = s2.length();
    
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1));
    
    for (size_t i = 0; i <= m; i++) {
        dp[i][0] = i;
    }
    for (size_t j = 0; j <= n; j++) {
        dp[0][j] = j;
    }
    
    for (size_t i = 1; i <= m; i++) {
        for (size_t j = 1; j <= n; j++) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                dp[i][j] = 1 + std::min({dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]});
            }
        }
    }
    
    return dp[m][n];
}

int SupabaseAnalyzer::computeAverageRTT(const std::vector<int>& rttValues) {
    if (rttValues.empty()) return 0;
    int sum = std::accumulate(rttValues.begin(), rttValues.end(), 0);
    return sum / rttValues.size();
}

int SupabaseAnalyzer::computeMinRTT(const std::vector<int>& rttValues) {
    if (rttValues.empty()) return 0;
    return *std::min_element(rttValues.begin(), rttValues.end());
}

int SupabaseAnalyzer::computeMaxRTT(const std::vector<int>& rttValues) {
    if (rttValues.empty()) return 0;
    return *std::max_element(rttValues.begin(), rttValues.end());
}

std::unordered_map<std::string, int> SupabaseAnalyzer::analyzeCaptionWords(
    const std::vector<InstagramProfile>& profiles) {
    
    std::unordered_map<std::string, int> wordFrequency;
    
    for (const auto& profile : profiles) {
        for (const auto& post : profile.posts) {
            std::vector<std::string> tokens = tokenizeText(post.caption);
            for (const auto& token : tokens) {
                // Skip common stop words
                if (token.length() > 2) {
                    wordFrequency[token]++;
                }
            }
        }
    }
    
    return wordFrequency;
}

std::vector<InstagramEngagementMetrics> SupabaseAnalyzer::analyzeInstagramProfiles(
    const std::vector<InstagramProfile>& profiles) {
    
    std::vector<InstagramEngagementMetrics> metrics;
    auto wordFreq = analyzeCaptionWords(profiles);
    
    // Find top words
    std::vector<std::pair<std::string, int>> sortedWords(
        wordFreq.begin(), wordFreq.end());
    std::sort(sortedWords.begin(), sortedWords.end(),
        [](const auto& a, const auto& b) { return a.second > b.second; });
    
    for (const auto& profile : profiles) {
        InstagramEngagementMetrics metric;
        metric.username = profile.username;
        metric.total_posts = profile.posts_count;
        metric.total_followers = profile.followers_count;
        metric.total_following = profile.following_count;
        
        // Calculate avg caption length
        int totalCaptionLength = 0;
        for (const auto& post : profile.posts) {
            totalCaptionLength += post.caption.length();
        }
        metric.avg_caption_length = profile.posts.empty() ? 
            0 : totalCaptionLength / profile.posts.size();
        
        // Top 5 most common words
        for (size_t i = 0; i < std::min(size_t(5), sortedWords.size()); i++) {
            metric.most_common_words.push_back(sortedWords[i].first);
        }
        
        // Calculate engagement rate
        metric.engagement_rate = (profile.posts_count > 0) ?
            static_cast<double>(profile.followers_count) / profile.posts_count : 0.0;
        
        metrics.push_back(metric);
    }
    
    return metrics;
}

std::vector<std::pair<std::string, std::string>> SupabaseAnalyzer::findSimilarProfiles(
    const std::vector<InstagramProfile>& profiles, int threshold) {
    
    std::vector<std::pair<std::string, std::string>> similarPairs;
    
    for (size_t i = 0; i < profiles.size(); i++) {
        for (size_t j = i + 1; j < profiles.size(); j++) {
            int distance = levenshteinDistance(profiles[i].username, profiles[j].username);
            if (distance <= threshold) {
                similarPairs.push_back({profiles[i].username, profiles[j].username});
            }
        }
    }
    
    return similarPairs;
}

void SupabaseAnalyzer::buildProfileBloomFilter(
    const std::vector<InstagramProfile>& profiles, BloomFilter& bf) {

    for (const auto& profile : profiles) {
        bf.add(profile.username);
        bf.add(profile.id);
        bf.add(profile.user_id);
    }
}

std::vector<UserDeviceStats> SupabaseAnalyzer::analyzeWhatsAppEvents(
    const std::vector<WhatsAppEvent>& events) {
    
    std::map<std::string, std::vector<WhatsAppEvent>> userEvents;
    
    // Group events by user_id
    for (const auto& event : events) {
        if (!event.user_id.empty()) {
            userEvents[event.user_id].push_back(event);
        }
    }
    
    std::vector<UserDeviceStats> stats;
    
    for (const auto& [userId, eventsForUser] : userEvents) {
        UserDeviceStats stat;
        stat.user_id = userId;
        stat.total_events = eventsForUser.size();
        
        // Get phone number from first event
        for (const auto& evt : eventsForUser) {
            if (!evt.phone_number.empty()) {
                stat.phone_number = evt.phone_number;
                break;
            }
        }
        
        // Count unique devices
        std::unordered_map<std::string, int> deviceCount;
        std::map<std::string, int> presenceCount;
        std::vector<int> rttValues;
        
        for (const auto& evt : eventsForUser) {
            for (const auto& device : evt.devices) {
                deviceCount[device.jid]++;
            }
            
            if (!evt.presence.empty()) {
                presenceCount[evt.presence]++;
            }
            
            if (evt.rtt > 0) {
                rttValues.push_back(evt.rtt);
            }
        }
        
        stat.unique_device_count = deviceCount.size();
        
        // Most common presence
        if (!presenceCount.empty()) {
            auto maxPresence = std::max_element(
                presenceCount.begin(), presenceCount.end(),
                [](const auto& a, const auto& b) { return a.second < b.second; }
            );
            stat.most_common_presence = maxPresence->first;
        }
        
        // RTT statistics
        stat.avg_response_time = computeAverageRTT(rttValues);
        
        // Breakdown by presence
        for (const auto& [presence, count] : presenceCount) {
            PresenceStatistics ps;
            ps.presence_status = presence;
            ps.count = count;
            ps.percentage = (static_cast<double>(count) / eventsForUser.size()) * 100.0;
            ps.avg_rtt = 0; // Could be computed per presence if needed
            ps.min_rtt = 0;
            ps.max_rtt = 0;
            stat.presence_breakdown.push_back(ps);
        }
        
        stats.push_back(stat);
    }
    
    return stats;
}

void SupabaseAnalyzer::buildRTTSegmentTree(
    const std::vector<WhatsAppEvent>& events, SegmentTree& tree) {

    std::vector<int> rttValues;
    for (const auto& event : events) {
        if (event.rtt > 0) {
            rttValues.push_back(event.rtt);
        }
    }

    // Sort for SegmentTree construction
    std::sort(rttValues.begin(), rttValues.end());

    // Reconstruct tree using assignment (SegmentTree has constructor taking vector)
    tree = SegmentTree(rttValues);
}

std::vector<WhatsAppEvent> SupabaseAnalyzer::findEventsByRTTRange(
    const std::vector<WhatsAppEvent>& events, int minRTT, int maxRTT) {
    
    std::vector<WhatsAppEvent> result;
    
    for (const auto& event : events) {
        if (event.rtt >= minRTT && event.rtt <= maxRTT) {
            result.push_back(event);
        }
    }
    
    return result;
}

std::unordered_map<std::string, std::vector<std::string>> SupabaseAnalyzer::detectDeviceClusters(
    const std::vector<WhatsAppEvent>& events) {
    
    // Use UnionFind to cluster devices by user_id
    std::unordered_map<std::string, int> deviceToId;
    UnionFind uf;
    int nextId = 0;
    
    for (const auto& event : events) {
        for (const auto& device : event.devices) {
            if (deviceToId.find(device.jid) == deviceToId.end()) {
                deviceToId[device.jid] = nextId++;
            }
        }
    }
    
    // Initialize UnionFind with proper size
    uf.Initialize(nextId);
    
    // Union devices from same events
    for (const auto& event : events) {
        if (event.devices.size() > 1) {
            int firstDeviceId = deviceToId[event.devices[0].jid];
            for (size_t i = 1; i < event.devices.size(); i++) {
                int deviceId = deviceToId[event.devices[i].jid];
                uf.Union(firstDeviceId, deviceId);
            }
        }
    }
    
    // Group devices by their cluster
    std::unordered_map<std::string, std::vector<std::string>> clusters;
    
    for (const auto& [jid, id] : deviceToId) {
        int rootId = uf.Find(id);
        clusters[std::to_string(rootId)].push_back(jid);
    }
    
    return clusters;
}

std::map<std::string, int> SupabaseAnalyzer::analyzePresenceTransitions(
    const std::vector<WhatsAppEvent>& events) {
    
    std::map<std::string, int> transitions;
    
    // Group events by user and sort by time
    std::map<std::string, std::vector<WhatsAppEvent>> userEvents;
    
    for (const auto& event : events) {
        if (!event.user_id.empty()) {
            userEvents[event.user_id].push_back(event);
        }
    }
    
    // Analyze transitions for each user
    for (auto& [userId, evts] : userEvents) {
        // Sort by event_time
        std::sort(evts.begin(), evts.end(),
            [](const WhatsAppEvent& a, const WhatsAppEvent& b) {
                return a.event_time < b.event_time;
            }
        );
        
        // Count transitions
        for (size_t i = 0; i + 1 < evts.size(); i++) {
            std::string transition = evts[i].presence + " -> " + evts[i + 1].presence;
            transitions[transition]++;
        }
    }
    
    return transitions;
}

void SupabaseAnalyzer::printAnalysisSummary(
    const std::vector<InstagramProfile>& profiles,
    const std::vector<WhatsAppEvent>& events) {
    
    std::cout << "\n" << std::string(80, '=') << std::endl;
    std::cout << "SUPABASE DATA ANALYSIS SUMMARY" << std::endl;
    std::cout << std::string(80, '=') << std::endl;
    
    // Instagram Analysis
    std::cout << "\n--- INSTAGRAM PROFILES ---" << std::endl;
    std::cout << "Total Profiles: " << profiles.size() << std::endl;
    
    auto engagementMetrics = analyzeInstagramProfiles(profiles);
    for (const auto& metric : engagementMetrics) {
        std::cout << "\nProfile: " << metric.username << std::endl;
        std::cout << "  Posts: " << metric.total_posts << std::endl;
        std::cout << "  Followers: " << metric.total_followers << std::endl;
        std::cout << "  Following: " << metric.total_following << std::endl;
        std::cout << "  Avg Caption Length: " << metric.avg_caption_length << " chars" << std::endl;
        std::cout << "  Engagement Rate: " << std::fixed << std::setprecision(2) 
                  << metric.engagement_rate << std::endl;
    }
    
    // WhatsApp Analysis
    std::cout << "\n--- WHATSAPP EVENTS ---" << std::endl;
    std::cout << "Total Events: " << events.size() << std::endl;
    
    auto userStats = analyzeWhatsAppEvents(events);
    for (const auto& stat : userStats) {
        std::cout << "\nUser: " << stat.user_id.substr(0, 8) << "..." << std::endl;
        std::cout << "  Total Events: " << stat.total_events << std::endl;
        std::cout << "  Unique Devices: " << stat.unique_device_count << std::endl;
        std::cout << "  Most Common Presence: " << stat.most_common_presence << std::endl;
        std::cout << "  Avg Response Time (RTT): " << stat.avg_response_time << " ms" << std::endl;
    }
    
    // Device Clusters
    auto clusters = detectDeviceClusters(events);
    std::cout << "\n--- DEVICE CLUSTERS ---" << std::endl;
    std::cout << "Total Clusters: " << clusters.size() << std::endl;
    for (const auto& [clusterId, devices] : clusters) {
        std::cout << "Cluster " << clusterId << " (" << devices.size() << " devices):" << std::endl;
        for (const auto& device : devices) {
            std::cout << "  - " << device << std::endl;
        }
    }
    
    // Presence Transitions
    auto transitions = analyzePresenceTransitions(events);
    std::cout << "\n--- PRESENCE TRANSITIONS ---" << std::endl;
    for (const auto& [transition, count] : transitions) {
        std::cout << transition << ": " << count << " times" << std::endl;
    }
    
    std::cout << "\n" << std::string(80, '=') << std::endl;
}