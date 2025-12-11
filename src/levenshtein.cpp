#include "../include/levenshtein.h"

int Levenshtein::distance(const std::string& s1, const std::string& s2) {
    int m = s1.length();
    int n = s2.length();
    
    // create dp table
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));
    
    // initialize base cases
    for (int i = 0; i <= m; i++) {
        dp[i][0] = i;
    }
    for (int j = 0; j <= n; j++) {
        dp[0][j] = j;
    }
    
    // fill dp table
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                dp[i][j] = 1 + std::min({dp[i - 1][j],      // deletion
                                         dp[i][j - 1],      // insertion
                                         dp[i - 1][j - 1]}); // substitution
            }
        }
    }
    
    return dp[m][n];
}

double Levenshtein::similarity(const std::string& s1, const std::string& s2) {
    if (s1.empty() && s2.empty()) return 1.0;
    if (s1.empty() || s2.empty()) return 0.0;
    
    int maxLen = std::max(s1.length(), s2.length());
    int editDist = distance(s1, s2);
    
    return 1.0 - (static_cast<double>(editDist) / maxLen);
}

std::string Levenshtein::findBestMatch(const std::string& query,
                                       const std::vector<std::string>& candidates,
                                       double threshold) {
    if (candidates.empty()) return "";
    
    double bestSimilarity = 0.0;
    std::string bestMatch = "";
    
    for (const std::string& candidate : candidates) {
        double sim = similarity(query, candidate);
        if (sim > bestSimilarity && sim >= threshold) {
            bestSimilarity = sim;
            bestMatch = candidate;
        }
    }
    
    return bestMatch;
}

std::vector<std::pair<std::string, double>> Levenshtein::findAllMatches(
    const std::string& query,
    const std::vector<std::string>& candidates,
    double threshold) {
    
    std::vector<std::pair<std::string, double>> matches;
    
    for (const std::string& candidate : candidates) {
        double sim = similarity(query, candidate);
        if (sim >= threshold) {
            matches.push_back({candidate, sim});
        }
    }
    
    // sort by similarity descending
    std::sort(matches.begin(), matches.end(),
              [](const std::pair<std::string, double>& a,
                 const std::pair<std::string, double>& b) {
                  return a.second > b.second;
              });
    
    return matches;
}

