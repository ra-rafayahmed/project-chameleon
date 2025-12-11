#include "../include/supabase_client.h"
#include <iostream>
#include <fstream>
#include <sstream>

SupabaseClient::SupabaseClient() {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
}

// Helper functions for safe JSON extraction
static std::string get_string_safe(const json& j, const std::string& key, const std::string& def = "") {
    return (j.contains(key) && !j[key].is_null()) ? j[key].get<std::string>() : def;
}

static int get_int_safe(const json& j, const std::string& key, int def = 0) {
    return (j.contains(key) && !j[key].is_null()) ? j[key].get<int>() : def;
}

// Forward declaration for helper defined later in this file
static std::string extractRawJsonForTable(const std::string& filepath, const std::string& tableName);

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

InstagramPost SupabaseClient::parseInstagramPost(const json& postJson) {
    InstagramPost post;
    post.caption = get_string_safe(postJson, "caption", "");
    post.img_url = get_string_safe(postJson, "img_url", "");
    post.post_url = get_string_safe(postJson, "post_url", "");
    return post;
}

DeviceInfo SupabaseClient::parseDeviceInfo(const json& deviceJson) {
    DeviceInfo device;
    device.avg = get_int_safe(deviceJson, "avg", 0);
    device.jid = get_string_safe(deviceJson, "jid", "");
    device.rtt = get_int_safe(deviceJson, "rtt", 0);
    device.state = get_string_safe(deviceJson, "state", "");
    return device;
}

InstagramProfile SupabaseClient::parseInstagramProfile(const json& profileJson) {
    InstagramProfile profile;
    profile.id = get_string_safe(profileJson, "id", "");
    profile.user_id = get_string_safe(profileJson, "user_id", "");
    profile.username = get_string_safe(profileJson, "username", "");
    profile.bio = get_string_safe(profileJson, "bio", "");
    profile.posts_count = get_int_safe(profileJson, "posts_count", 0);
    profile.followers_count = get_int_safe(profileJson, "followers_count", 0);
    profile.following_count = get_int_safe(profileJson, "following_count", 0);
    profile.created_at = get_string_safe(profileJson, "created_at", "");
    
    // Parse raw JSON for posts
    if (profileJson.contains("raw") && profileJson["raw"].contains("posts")) {
        const auto& postsJson = profileJson["raw"]["posts"];
        if (postsJson.is_array()) {
            for (const auto& postJson : postsJson) {
                profile.posts.push_back(parseInstagramPost(postJson));
            }
        }
    }
    
    // Parse followers and following lists
    if (profileJson.contains("raw") && profileJson["raw"].contains("followers_list")) {
        const auto& followersJson = profileJson["raw"]["followers_list"];
        if (followersJson.is_array()) {
            for (const auto& follower : followersJson) {
                if (!follower.is_null() && follower.is_string()) {
                    profile.followers_list.push_back(follower.get<std::string>());
                }
            }
        }
    }
    
    if (profileJson.contains("raw") && profileJson["raw"].contains("following_list")) {
        const auto& followingJson = profileJson["raw"]["following_list"];
        if (followingJson.is_array()) {
            for (const auto& following : followingJson) {
                if (!following.is_null() && following.is_string()) {
                    profile.following_list.push_back(following.get<std::string>());
                }
            }
        }
    }
    
    return profile;
}

WhatsAppEvent SupabaseClient::parseWhatsAppEvent(const json& eventJson) {
    WhatsAppEvent event;
    event.id = (eventJson.contains("id") && !eventJson["id"].is_null()) ? eventJson["id"].get<std::string>() : std::string();
    event.user_id = (eventJson.contains("user_id") && !eventJson["user_id"].is_null()) ? eventJson["user_id"].get<std::string>() : std::string();
    event.phone_number = (eventJson.contains("phone_number") && !eventJson["phone_number"].is_null()) ? eventJson["phone_number"].get<std::string>() : std::string();
    event.event_time = (eventJson.contains("event_time") && !eventJson["event_time"].is_null()) ? eventJson["event_time"].get<std::string>() : std::string();
    event.presence = (eventJson.contains("presence") && !eventJson["presence"].is_null()) ? eventJson["presence"].get<std::string>() : std::string();
    event.rtt = (eventJson.contains("rtt") && !eventJson["rtt"].is_null()) ? eventJson["rtt"].get<int>() : 0;
    event.device_count = (eventJson.contains("device_count") && !eventJson["device_count"].is_null()) ? eventJson["device_count"].get<int>() : 0;
    event.created_at = (eventJson.contains("created_at") && !eventJson["created_at"].is_null()) ? eventJson["created_at"].get<std::string>() : std::string();
    
    // Parse devices array defensively
    if (eventJson.contains("devices") && eventJson["devices"].is_array()) {
        for (const auto& deviceJson : eventJson["devices"]) {
            // deviceJson may contain null fields — parseDeviceInfo already uses value() safely
            event.devices.push_back(parseDeviceInfo(deviceJson));
        }
    }
    
    return event;
}

std::vector<InstagramProfile> SupabaseClient::getInstagramProfiles() {
    std::vector<InstagramProfile> profiles;
    std::string jsonStr = fetchTable("instagram_profiles");
    
    if (jsonStr.empty()) {
        std::cerr << "Failed to fetch Instagram profiles" << std::endl;
        return profiles;
    }
    
    try {
        auto jsonArray = json::parse(jsonStr);
        if (jsonArray.is_array()) {
            for (const auto& profileJson : jsonArray) {
                profiles.push_back(parseInstagramProfile(profileJson));
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
    }
    
    return profiles;
}

std::vector<WhatsAppEvent> SupabaseClient::getWhatsAppEvents() {
    std::vector<WhatsAppEvent> events;
    std::string jsonStr = fetchTable("whatsapp_events");
    
    if (jsonStr.empty()) {
        std::cerr << "Failed to fetch WhatsApp events" << std::endl;
        return events;
    }
    
    try {
        auto jsonArray = json::parse(jsonStr);
        if (jsonArray.is_array()) {
            for (const auto& eventJson : jsonArray) {
                events.push_back(parseWhatsAppEvent(eventJson));
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
    }
    
    return events;
}

// Parse a GameEvent from JSON defensively
GameEvent SupabaseClient::parseGameEvent(const json& gameJson) {
    GameEvent g;
    g.id = (gameJson.contains("id") && !gameJson["id"].is_null()) ? gameJson["id"].get<std::string>() : std::string();
    g.user_id = (gameJson.contains("user_id") && !gameJson["user_id"].is_null()) ? gameJson["user_id"].get<std::string>() : std::string();
    g.game_id = (gameJson.contains("game_id") && !gameJson["game_id"].is_null()) ? gameJson["game_id"].get<std::string>() : std::string();
    g.game_name = (gameJson.contains("game_name") && !gameJson["game_name"].is_null()) ? gameJson["game_name"].get<std::string>() : std::string();
    g.player_id = (gameJson.contains("player_id") && !gameJson["player_id"].is_null()) ? gameJson["player_id"].get<std::string>() : std::string();
    g.score = (gameJson.contains("score") && !gameJson["score"].is_null()) ? gameJson["score"].get<int>() : 0;
    g.platform = (gameJson.contains("platform") && !gameJson["platform"].is_null()) ? gameJson["platform"].get<std::string>() : std::string();
    g.event_time = (gameJson.contains("event_time") && !gameJson["event_time"].is_null()) ? gameJson["event_time"].get<std::string>() : std::string();
    g.created_at = (gameJson.contains("created_at") && !gameJson["created_at"].is_null()) ? gameJson["created_at"].get<std::string>() : std::string();

    if (gameJson.contains("metadata") && gameJson["metadata"].is_object()) {
        for (auto it = gameJson["metadata"].begin(); it != gameJson["metadata"].end(); ++it) {
            if (!it.value().is_null()) {
                try {
                    g.metadata[it.key()] = it.value().get<std::string>();
                } catch (...) {
                    // Ignore non-string metadata values
                }
            }
        }
    }

    return g;
}

std::vector<GameEvent> SupabaseClient::getGameEvents() {
    std::vector<GameEvent> events;
    std::string jsonStr = fetchTable("game_events");

    if (jsonStr.empty()) {
        std::cerr << "Failed to fetch Game events" << std::endl;
        return events;
    }

    try {
        auto jsonArray = json::parse(jsonStr);
        if (jsonArray.is_array()) {
            for (const auto& gameJson : jsonArray) {
                events.push_back(parseGameEvent(gameJson));
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "JSON parsing error (game events): " << e.what() << std::endl;
    }

    return events;
}

std::vector<GameEvent> SupabaseClient::loadGameEventsFromFile(const std::string& filepath) {
    std::vector<GameEvent> events;
    std::string jsonStr = extractRawJsonForTable(filepath, "game_events");
    if (jsonStr.empty()) {
        // Try fallback: maybe the export uses a different table name (e.g. game_devices)
        std::cerr << "Could not extract game_events JSON from: " << filepath << ". Trying fallback tables..." << std::endl;
        std::string alt = extractRawJsonForTable(filepath, "game_devices");
        if (alt.empty()) {
            std::cerr << "No suitable fallback table found in: " << filepath << std::endl;
            return events;
        }

        try {
            auto jsonArray = json::parse(alt);
            if (jsonArray.is_array()) {
                // convert device records into GameEvent records heuristically
                for (const auto& deviceJson : jsonArray) {
                    GameEvent g;
                    // Map common fields where possible
                    g.id = (deviceJson.contains("id") && !deviceJson["id"].is_null()) ? deviceJson["id"].get<std::string>() : std::string();
                    g.user_id = (deviceJson.contains("user_id") && !deviceJson["user_id"].is_null()) ? deviceJson["user_id"].get<std::string>() : std::string();
                    // Use device_id as player_id when available
                    g.player_id = (deviceJson.contains("device_id") && !deviceJson["device_id"].is_null()) ? deviceJson["device_id"].get<std::string>() : std::string();
                    // Use ping as a proxy for score if present
                    g.score = (deviceJson.contains("ping") && !deviceJson["ping"].is_null()) ? deviceJson["ping"].get<int>() : 0;
                    // platform: prefer system_language, then device_region
                    if (deviceJson.contains("system_language") && !deviceJson["system_language"].is_null())
                        g.platform = deviceJson["system_language"].get<std::string>();
                    else if (deviceJson.contains("device_region") && !deviceJson["device_region"].is_null())
                        g.platform = deviceJson["device_region"].get<std::string>();

                    // event_time -> created_at if present
                    if (deviceJson.contains("created_at") && !deviceJson["created_at"].is_null())
                        g.event_time = deviceJson["created_at"].get<std::string>();

                    // fill metadata with useful device fields
                    if (deviceJson.contains("ip") && !deviceJson["ip"].is_null()) g.metadata["ip"] = deviceJson["ip"].get<std::string>();
                    if (deviceJson.contains("timezone") && !deviceJson["timezone"].is_null()) g.metadata["timezone"] = deviceJson["timezone"].get<std::string>();
                    if (deviceJson.contains("keyboard_layout") && !deviceJson["keyboard_layout"].is_null()) g.metadata["keyboard_layout"] = deviceJson["keyboard_layout"].get<std::string>();
                    if (deviceJson.contains("gps_lat") && !deviceJson["gps_lat"].is_null()) {
                        try { g.metadata["gps_lat"] = std::to_string(deviceJson["gps_lat"].get<double>()); } catch(...) {}
                    }
                    if (deviceJson.contains("gps_lng") && !deviceJson["gps_lng"].is_null()) {
                        try { g.metadata["gps_lng"] = std::to_string(deviceJson["gps_lng"].get<double>()); } catch(...) {}
                    }

                    if (deviceJson.contains("raw") && deviceJson["raw"].is_object()) {
                        try {
                            g.metadata["raw"] = deviceJson["raw"].dump();
                        } catch(...) {}
                    }

                    events.push_back(g);
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "JSON parsing error (convert game_devices): " << e.what() << std::endl;
        }

        return events;
    }

    try {
        auto jsonArray = json::parse(jsonStr);
        if (jsonArray.is_array()) {
            for (const auto& gameJson : jsonArray) {
                events.push_back(parseGameEvent(gameJson));
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "JSON parsing error (load game events): " << e.what() << std::endl;
    }

    return events;
}

std::string SupabaseClient::getInstagramStories() {
    return fetchTable("instagram_stories");
}

std::string SupabaseClient::getGameDevices() {
    return fetchTable("game_devices");
}

// Helper: extract the Raw JSON array for a named table from an exported text file
static std::string extractRawJsonForTable(const std::string& filepath, const std::string& tableName) {
    std::ifstream file(filepath);
    if (!file.is_open()) return std::string();

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    file.close();

    // Find the TABLE header for the requested table
    std::string tableHeader = "TABLE: " + tableName;
    size_t pos = content.find(tableHeader);
    if (pos == std::string::npos) {
        // fallback: search for just the tableName if header formatting differs
        pos = content.find(tableName);
        if (pos == std::string::npos) return std::string();
    }

    // From the header, find the next occurrence of "Raw JSON:" and then the JSON array
    size_t rawPos = content.find("Raw JSON:", pos);
    if (rawPos == std::string::npos) return std::string();

    // Find first '[' after Raw JSON:
    size_t arrayStart = content.find('[', rawPos);
    if (arrayStart == std::string::npos) return std::string();

    // Find matching ']' by tracking bracket depth
    size_t i = arrayStart;
    int depth = 0;
    for (; i < content.size(); ++i) {
        if (content[i] == '[') depth++;
        else if (content[i] == ']') {
            depth--;
            if (depth == 0) {
                // include the closing bracket
                return content.substr(arrayStart, i - arrayStart + 1);
            }
        }
    }

    return std::string();
}

std::vector<InstagramProfile> SupabaseClient::loadInstagramProfilesFromFile(const std::string& filepath) {
    std::vector<InstagramProfile> profiles;
    std::string jsonStr = extractRawJsonForTable(filepath, "instagram_profiles");
    if (jsonStr.empty()) {
        std::cerr << "Could not extract instagram_profiles JSON from: " << filepath << std::endl;
        return profiles;
    }

    try {
        auto jsonArray = json::parse(jsonStr);
        if (jsonArray.is_array()) {
            for (const auto& profileJson : jsonArray) {
                profiles.push_back(parseInstagramProfile(profileJson));
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "JSON parsing error (instagram profiles): " << e.what() << std::endl;
    }

    return profiles;
}

std::vector<WhatsAppEvent> SupabaseClient::loadWhatsAppEventsFromFile(const std::string& filepath) {
    std::vector<WhatsAppEvent> events;
    std::string jsonStr = extractRawJsonForTable(filepath, "whatsapp_events");
    if (jsonStr.empty()) {
        std::cerr << "Could not extract whatsapp_events JSON from: " << filepath << std::endl;
        return events;
    }

    try {
        auto jsonArray = json::parse(jsonStr);
        if (jsonArray.is_array()) {
            for (const auto& eventJson : jsonArray) {
                events.push_back(parseWhatsAppEvent(eventJson));
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "JSON parsing error (whatsapp events): " << e.what() << std::endl;
    }

    return events;
}

