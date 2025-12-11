#include "../include/supabase_client.h"
#include <iostream>

SupabaseClient::SupabaseClient() {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
}

SupabaseClient::~SupabaseClient() {
    if (curl) {
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

size_t SupabaseClient::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t total_size = size * nmemb;
    ((WriteData*)userp)->data.append((char*)contents, total_size);
    return total_size;
}

std::string SupabaseClient::fetchTable(const std::string& tableName) {
    if (!curl) {
        std::cerr << "curl not initialized" << std::endl;
        return "";
    }
    
    std::string url = config.url + "/rest/v1/" + tableName;
    WriteData response;
    
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    
    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, ("apikey: " + config.api_key).c_str());
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "Prefer: return=representation");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    
    CURLcode res = curl_easy_perform(curl);
    curl_slist_free_all(headers);
    
    if (res != CURLE_OK) {
        std::cerr << "curl request failed: " << curl_easy_strerror(res) << std::endl;
        return "";
    }
    
    return response.data;
}

std::string SupabaseClient::fetchTableFiltered(const std::string& tableName, const std::string& filter) {
    if (!curl) {
        return "";
    }
    
    std::string url = config.url + "/rest/v1/" + tableName + "?" + filter;
    WriteData response;
    
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    
    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, ("apikey: " + config.api_key).c_str());
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    
    CURLcode res = curl_easy_perform(curl);
    curl_slist_free_all(headers);
    
    if (res != CURLE_OK) {
        return "";
    }
    
    return response.data;
}

std::string SupabaseClient::getUsers() {
    return fetchTable("users");
}

std::string SupabaseClient::getInstagramProfiles() {
    return fetchTable("instagram_profiles");
}

std::string SupabaseClient::getInstagramStories() {
    return fetchTable("instagram_stories");
}

std::string SupabaseClient::getGameDevices() {
    return fetchTable("game_devices");
}

std::string SupabaseClient::getWhatsAppEvents() {
    return fetchTable("whatsapp_events");
}

