#ifndef STRING_SORT_TESTER_H
#define STRING_SORT_TESTER_H

#include <string>
#include <vector>
#include <functional>

struct SortMetrics {
    long long comparisons;
    double timeMs;
    SortMetrics() : comparisons(0), timeMs(0.0) {}
};

class StringSortTester {
public:
    static SortMetrics measure(std::vector<std::string> data,
                                std::function<void(std::vector<std::string>&)> sortFunc);
    
    static void standardQuickSort(std::vector<std::string>& arr, int left, int right);
    static void standardMergeSort(std::vector<std::string>& arr, int left, int right);
    static void stringQuickSort(std::vector<std::string>& arr, int left, int right, int depth = 0);
    static void stringMergeSort(std::vector<std::string>& arr, int left, int right);
    static void msdRadixSort(std::vector<std::string>& arr);
    static void msdRadixSortWithCutoff(std::vector<std::string>& arr, int cutoff = 74);
};

#endif