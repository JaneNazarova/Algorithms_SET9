#include "StringGenerator.h"
#include <algorithm>
#include <random>

const std::string StringGenerator::CHARSET =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789"
    "!@#%:;^&*()-.";

StringGenerator::StringGenerator(unsigned int s) : seed(s), rng(seed) {}
std::string StringGenerator::generateRandomString(int minLen, int maxLen) {
    std::uniform_int_distribution<int> lenDist(minLen, maxLen);
    std::uniform_int_distribution<int> charDist(0, CHARSET.size() - 1);

    int length = lenDist(rng);
    std::string result;
    result.reserve(length);
    for (int i = 0; i < length; ++i) {
        result += CHARSET[charDist(rng)];
    }
    return result;
}

std::vector<std::string> StringGenerator::generateRandomArray(int size, int minLen, int maxLen) {
    std::vector<std::string> arr;
    arr.reserve(size);
    for (int i = 0; i < size; ++i) {
        arr.push_back(generateRandomString(minLen, maxLen));
    }
    return arr;
}

std::vector<std::string> StringGenerator::generateReverseSorted(const std::vector<std::string>& sorted) {
    std::vector<std::string> arr = sorted;
    std::reverse(arr.begin(), arr.end());
    return arr;
}

std::vector<std::string> StringGenerator::generateNearlySorted(const std::vector<std::string>& sorted, int numSwaps) {
    std::vector<std::string> arr = sorted;
    std::mt19937 localRng(seed + 1);
    std::uniform_int_distribution<int> posDist(0, arr.size() - 1);

    for (int i = 0; i < numSwaps; ++i) {
        int p1 = posDist(localRng);
        int p2 = posDist(localRng);
        std::swap(arr[p1], arr[p2]);
    }
    return arr;
}

std::vector<std::string> StringGenerator::generateCommonPrefixArray(int size, int minLen, int maxLen,
                                                                     const std::string& prefix, double prefixProb) {
    std::vector<std::string> arr;
    arr.reserve(size);
    std::mt19937 localRng(seed + 2);
    std::uniform_real_distribution<double> probDist(0.0, 1.0);

    for (int i = 0; i < size; ++i) {
        if (probDist(localRng) < prefixProb) {
            int restMinLen = std::max(1, minLen - (int)prefix.length());
            int restMaxLen = maxLen - (int)prefix.length();
            if (restMinLen > restMaxLen) restMinLen = restMaxLen;
            std::string rest = generateRandomString(restMinLen, restMaxLen);
            arr.push_back(prefix + rest);
        } else {
            arr.push_back(generateRandomString(minLen, maxLen));
        }
    }
    return arr;
}

std::vector<std::string> StringGenerator::getSortedBase(const std::vector<std::string>& arr) {
    std::vector<std::string> sorted = arr;
    std::sort(sorted.begin(), sorted.end());
    return sorted;
}