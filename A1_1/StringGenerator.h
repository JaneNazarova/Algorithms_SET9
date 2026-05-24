#ifndef STRING_GENERATOR_H
#define STRING_GENERATOR_H

#include <string>
#include <vector>
#include <random>

class StringGenerator {
private:
    static const std::string CHARSET;
    unsigned int seed;
    mutable std::mt19937 rng;
    
public:
    StringGenerator(unsigned int s = 42);
    
    std::string generateRandomString(int minLen, int maxLen);
    std::vector<std::string> generateRandomArray(int size, int minLen, int maxLen);
    std::vector<std::string> generateReverseSorted(const std::vector<std::string>& sorted);
    std::vector<std::string> generateNearlySorted(const std::vector<std::string>& sorted, int numSwaps);
    std::vector<std::string> generateCommonPrefixArray(int size, int minLen, int maxLen,
                                                         const std::string& prefix, double prefixProb);
    std::vector<std::string> getSortedBase(const std::vector<std::string>& arr);
};

#endif