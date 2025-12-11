#ifndef SUPABASE_CLIENT_H
#define SUPABASE_CLIENT_H

#include <string>
#include <vector>
#include <curl/curl.h>

struct SupabaseConfig {
    std::string url = "https://qdsbdmikobhynjmfzweb.supabase.co";
    std::string api_key = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6InFkc2JkbWlrb2JoeW5qbWZ6d2ViIiwicm9sZSI6ImFub24iLCJpYXQiOjE3NjUzODE1NjQsImV4cCI6MjA4MDk1NzU2NH0.FAxUj8LLxdBxxBvRua_GX2d_QrhXsCQfu6uUUit8PSM";
};

struct WriteData {
    std::string data;
};

// supabase rest api client
class SupabaseClient {
private:
    SupabaseConfig config;
    CURL* curl;
    
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);

public:
    SupabaseClient();
    ~SupabaseClient();
    
    // fetch all records from table
    std::string fetchTable(const std::string& tableName);
    
    // fetch records with filter
    std::string fetchTableFiltered(const std::string& tableName, const std::string& filter);
    
    // get users data
    std::string getUsers();
    
    // get instagram profiles
    std::string getInstagramProfiles();
    
    // get instagram stories
    std::string getInstagramStories();
    
    // get game devices
    std::string getGameDevices();
    
    // get whatsapp events
    std::string getWhatsAppEvents();
};

#endif

