#ifndef LEVENSHTEIN_H
#define LEVENSHTEIN_H

#include <string>
#include <vector>
#include <algorithm>

// levenshtein distance for fuzzy user/phone matching
class Levenshtein {
public:
    // compute edit distance between two strings
    static int distance(const std::string& s1, const std::string& s2);
    
    // compute similarity ratio (0.0 to 1.0)
    static double similarity(const std::string& s1, const std::string& s2);
    
    // find best match from list of candidates
    static std::string findBestMatch(const std::string& query, 
                                     const std::vector<std::string>& candidates,
                                     double threshold = 0.5);
    
    // find all matches above threshold
    static std::vector<std::pair<std::string, double>> findAllMatches(
        const std::string& query,
        const std::vector<std::string>& candidates,
        double threshold = 0.5);
};

#endif

