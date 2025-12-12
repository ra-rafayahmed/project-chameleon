#ifndef SUPABASE_CLIENT_H
#define SUPABASE_CLIENT_H

#include <string>
#include <vector>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include "supabase_models.h"

using json = nlohmann::json;

struct SupabaseConfig {
    std::string url = "https://qdsbdmikobhynjmfzweb.supabase.co";
    std::string api_key = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6InFkc2JkbWlrb2JoeW5qbWZ6d2ViIiwicm9sZSI6ImFub24iLCJpYXQiOjE3NjUzODE1NjQsImV4cCI6MjA4MDk1NzU2NH0.FAxUj8LLxdBxxBvRua_GX2d_QrhXsCQfu6uUUit8PSM";
};

struct WriteData {
    std::string data;
};

// supabase rest api client with data parsing
class SupabaseClient {
private:
    SupabaseConfig config;
    CURL* curl;
    
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
    
    // Helper methods for parsing JSON responses
    InstagramProfile parseInstagramProfile(const json& profileJson);
    WhatsAppEvent parseWhatsAppEvent(const json& eventJson);
    GameEvent parseGameEvent(const json& gameJson);
    DeviceInfo parseDeviceInfo(const json& deviceJson);
    InstagramPost parseInstagramPost(const json& postJson);

public:
    SupabaseClient();
    ~SupabaseClient();
    
    // fetch all records from table
    std::string fetchTable(const std::string& tableName);
    
    // fetch records with filter
    std::string fetchTableFiltered(const std::string& tableName, const std::string& filter);
    
    // get users data
    std::string getUsers();
    
    // Parse and get Instagram profiles from JSON
    std::vector<InstagramProfile> getInstagramProfiles();
    
    // Parse and get Instagram stories
    std::string getInstagramStories();
    
    // get game devices
    std::string getGameDevices();
    // Parse and get Game events from JSON
    std::vector<GameEvent> getGameEvents();
    std::vector<GameEvent> loadGameEventsFromFile(const std::string& filepath);
    
    // Parse and get WhatsApp events from JSON
    std::vector<WhatsAppEvent> getWhatsAppEvents();
    
    // Load data from local JSON file (for offline testing)
    std::vector<InstagramProfile> loadInstagramProfilesFromFile(const std::string& filepath);
    std::vector<WhatsAppEvent> loadWhatsAppEventsFromFile(const std::string& filepath);
};

#endif

