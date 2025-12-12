#ifndef SUPABASE_MODELS_H
#define SUPABASE_MODELS_H

#include <string>
#include <vector>
#include <map>
#include <ctime>

// ============================================================================
// SUPABASE DATA STRUCTURES - Instagram Profiles
// ============================================================================

struct InstagramPost {
    std::string caption;
    std::string img_url;
    std::string post_url;
};

struct InstagramProfile {
    std::string id;
    std::string user_id;
    std::string username;
    std::string bio;
    int posts_count;
    int followers_count;
    int following_count;
    std::vector<InstagramPost> posts;
    std::vector<std::string> followers_list;
    std::vector<std::string> following_list;
    std::string created_at;
    
    // Constructor
    InstagramProfile() : posts_count(0), followers_count(0), following_count(0) {}
};

// ============================================================================
// SUPABASE DATA STRUCTURES - WhatsApp Events
// ============================================================================

struct DeviceInfo {
    int avg;
    std::string jid;
    int rtt;
    std::string state;
};

struct WhatsAppEvent {
    std::string id;
    std::string user_id;
    std::string phone_number;
    std::string event_time;
    std::string presence;  // available, unavailable, recording, composing, etc.
    int rtt;               // Round Trip Time in milliseconds
    int device_count;
    std::vector<DeviceInfo> devices;
    std::string created_at;
    
    // Constructor
    WhatsAppEvent() : rtt(0), device_count(0) {}
};

// ============================================================================
// SUPABASE DATA STRUCTURES - Game Events
// ============================================================================

struct GameEvent {
    std::string id;
    std::string user_id;
    std::string game_id;
    std::string game_name;
    std::string player_id;
    int score;
    std::string platform; // e.g., "pc", "mobile"
    std::string event_time;
    std::map<std::string, std::string> metadata; // arbitrary key/value
    std::string created_at;

    GameEvent() : score(0) {}
};

// ============================================================================
// ANALYSIS DATA STRUCTURES
// ============================================================================

struct PresenceStatistics {
    std::string presence_status;
    int count;
    double percentage;
    int avg_rtt;
    int min_rtt;
    int max_rtt;
};

struct UserDeviceStats {
    std::string user_id;
    std::string phone_number;
    int total_events;
    int unique_device_count;
    std::string most_common_presence;
    int avg_response_time;
    std::vector<PresenceStatistics> presence_breakdown;
};

struct InstagramEngagementMetrics {
    std::string username;
    int total_posts;
    int total_followers;
    int total_following;
    int avg_caption_length;
    std::vector<std::string> most_common_words;
    double engagement_rate;
};

#endif
