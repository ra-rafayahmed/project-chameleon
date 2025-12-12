#include "../include/json_parser.h"
#include <sstream>
#include <cctype>
#include <stdexcept>

JsonParser::JsonParser() {}

void JsonParser::skipWhitespace(const std::string& json, size_t& pos) {
    while (pos < json.length() && std::isspace(json[pos])) {
        pos++;
    }
}

std::string JsonParser::parseString(const std::string& json, size_t& pos) {
    skipWhitespace(json, pos);
    if (pos >= json.length() || json[pos] != '"') {
        throw std::runtime_error("expected string");
    }
    pos++; // skip opening quote
    
    std::string result;
    while (pos < json.length() && json[pos] != '"') {
        if (json[pos] == '\\' && pos + 1 < json.length()) {
            pos++;
            switch (json[pos]) {
                case 'n': result += '\n'; break;
                case 't': result += '\t'; break;
                case '\\': result += '\\'; break;
                case '"': result += '"'; break;
                default: result += json[pos]; break;
            }
        } else {
            result += json[pos];
        }
        pos++;
    }
    
    if (pos < json.length() && json[pos] == '"') {
        pos++; // skip closing quote
    }
    
    return result;
}

JsonNode JsonParser::parseValue(const std::string& json, size_t& pos) {
    skipWhitespace(json, pos);
    
    if (pos >= json.length()) {
        throw std::runtime_error("unexpected end of json");
    }
    
    JsonNode node;
    
    if (json[pos] == '{') {
        return parseObject(json, pos);
    } else if (json[pos] == '[') {
        return parseArray(json, pos);
    } else if (json[pos] == '"') {
        node.value = parseString(json, pos);
    } else if (json.substr(pos, 4) == "true") {
        node.value = true;
        pos += 4;
    } else if (json.substr(pos, 5) == "false") {
        node.value = false;
        pos += 5;
    } else if (json.substr(pos, 4) == "null") {
        node.value = nullptr;
        pos += 4;
    } else {
        // number
        std::string numStr;
        bool isDouble = false;
        while (pos < json.length() && 
               (std::isdigit(json[pos]) || json[pos] == '.' || json[pos] == '-' || json[pos] == '+')) {
            if (json[pos] == '.') isDouble = true;
            numStr += json[pos];
            pos++;
        }
        
        if (isDouble) {
            node.value = std::stod(numStr);
        } else {
            node.value = std::stoi(numStr);
        }
    }
    
    return node;
}

JsonNode JsonParser::parseObject(const std::string& json, size_t& pos) {
    skipWhitespace(json, pos);
    if (pos >= json.length() || json[pos] != '{') {
        throw std::runtime_error("expected object");
    }
    pos++; // skip '{'
    
    JsonNode node;
    node.isObject = true;
    
    skipWhitespace(json, pos);
    if (pos < json.length() && json[pos] == '}') {
        pos++; // empty object
        return node;
    }
    
    while (pos < json.length()) {
        skipWhitespace(json, pos);
        std::string key = parseString(json, pos);
        
        skipWhitespace(json, pos);
        if (pos >= json.length() || json[pos] != ':') {
            throw std::runtime_error("expected ':'");
        }
        pos++; // skip ':'
        
        JsonNode child = parseValue(json, pos);
        child.key = key;
        node.children.push_back(child);
        
        skipWhitespace(json, pos);
        if (pos >= json.length()) break;
        
        if (json[pos] == '}') {
            pos++;
            break;
        } else if (json[pos] == ',') {
            pos++;
        } else {
            break;
        }
    }
    
    return node;
}

JsonNode JsonParser::parseArray(const std::string& json, size_t& pos) {
    skipWhitespace(json, pos);
    if (pos >= json.length() || json[pos] != '[') {
        throw std::runtime_error("expected array");
    }
    pos++; // skip '['
    
    JsonNode node;
    node.isArray = true;
    
    skipWhitespace(json, pos);
    if (pos < json.length() && json[pos] == ']') {
        pos++; // empty array
        return node;
    }
    
    while (pos < json.length()) {
        JsonNode child = parseValue(json, pos);
        node.children.push_back(child);
        
        skipWhitespace(json, pos);
        if (pos >= json.length()) break;
        
        if (json[pos] == ']') {
            pos++;
            break;
        } else if (json[pos] == ',') {
            pos++;
        } else {
            break;
        }
    }
    
    return node;
}

JsonNode JsonParser::parse(const std::string& json) {
    size_t pos = 0;
    skipWhitespace(json, pos);
    
    if (pos < json.length() && json[pos] == '{') {
        return parseObject(json, pos);
    } else if (pos < json.length() && json[pos] == '[') {
        return parseArray(json, pos);
    } else {
        return parseValue(json, pos);
    }
}

void JsonParser::dfsTraverse(const JsonNode& node, int depth, std::string& result) {
    std::string indent(depth * 2, ' ');
    
    if (node.isObject) {
        result += indent + "{\n";
        for (size_t i = 0; i < node.children.size(); i++) {
            const JsonNode& child = node.children[i];
            result += indent + "  \"" + child.key + "\": ";
            dfsTraverse(child, depth + 1, result);
            if (i < node.children.size() - 1) result += ",";
            result += "\n";
        }
        result += indent + "}";
    } else if (node.isArray) {
        result += indent + "[\n";
        for (size_t i = 0; i < node.children.size(); i++) {
            dfsTraverse(node.children[i], depth + 1, result);
            if (i < node.children.size() - 1) result += ",";
            result += "\n";
        }
        result += indent + "]";
    } else {
        if (std::holds_alternative<std::string>(node.value)) {
            result += "\"" + std::get<std::string>(node.value) + "\"";
        } else if (std::holds_alternative<int>(node.value)) {
            result += std::to_string(std::get<int>(node.value));
        } else if (std::holds_alternative<double>(node.value)) {
            result += std::to_string(std::get<double>(node.value));
        } else if (std::holds_alternative<bool>(node.value)) {
            result += std::get<bool>(node.value) ? "true" : "false";
        } else {
            result += "null";
        }
    }
}

std::string JsonParser::normalize(const std::string& json) {
    try {
        JsonNode root = parse(json);
        std::string result;
        dfsTraverse(root, 0, result);
        return result;
    } catch (...) {
        return json; // return original if parsing fails
    }
}

std::vector<std::string> JsonParser::extractKeys(const std::string& json) {
    std::vector<std::string> keys;
    try {
        JsonNode root = parse(json);
        std::function<void(const JsonNode&)> extract = [&](const JsonNode& node) {
            if (!node.key.empty()) {
                keys.push_back(node.key);
            }
            for (const JsonNode& child : node.children) {
                extract(child);
            }
        };
        extract(root);
    } catch (...) {
        // ignore parsing errors
    }
    return keys;
}

JsonValue JsonParser::getValueByPath(const std::string& json, const std::string& path) {
    // simplified implementation
    return nullptr;
}

std::string JsonParser::nodeToString(const JsonNode& node, int indent) {
    std::string result;
    dfsTraverse(node, indent, result);
    return result;
}

