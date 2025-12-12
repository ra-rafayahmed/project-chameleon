#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <string>
#include <vector>
#include <map>
#include <variant>
#include <functional>

// json value type
using JsonValue = std::variant<std::string, int, double, bool, std::nullptr_t>;

// json node structure
struct JsonNode {
    std::string key;
    JsonValue value;
    std::vector<JsonNode> children;
    bool isObject;
    bool isArray;
    
    JsonNode() : isObject(false), isArray(false) {}
};

// json parser using dfs traversal for normalization
class JsonParser {
private:
    // dfs helper for parsing
    JsonNode parseObject(const std::string& json, size_t& pos);
    JsonNode parseArray(const std::string& json, size_t& pos);
    JsonNode parseValue(const std::string& json, size_t& pos);
    std::string parseString(const std::string& json, size_t& pos);
    
    // skip whitespace
    void skipWhitespace(const std::string& json, size_t& pos);
    
    // dfs traversal helper
    void dfsTraverse(const JsonNode& node, int depth, std::string& result);

public:
    JsonParser();
    
    // parse json string into structured format
    JsonNode parse(const std::string& json);
    
    // normalize json (convert to structured format)
    std::string normalize(const std::string& json);
    
    // extract all keys from json
    std::vector<std::string> extractKeys(const std::string& json);
    
    // extract value by key path (e.g., "user.name")
    JsonValue getValueByPath(const std::string& json, const std::string& path);
    
    // convert json node to string
    std::string nodeToString(const JsonNode& node, int indent = 0);
};

#endif

