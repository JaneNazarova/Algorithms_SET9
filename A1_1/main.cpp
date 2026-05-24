#include "StringGenerator.h"
#include "StringSortTester.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <functional>
#include <filesystem>

int main() {
    std::cout << "STRING SORTING ALGORITHMS" << std::endl;

    StringGenerator gen(42);

    std::filesystem::create_directories("results");

    auto randomBase = gen.generateRandomArray(3000, 10, 200);
    auto sortedBase = gen.getSortedBase(randomBase);
    auto reverseSortedBase = gen.generateReverseSorted(sortedBase);
    auto nearlySortedBase = gen.generateNearlySorted(sortedBase, 100);
    auto commonPrefixBase = gen.generateCommonPrefixArray(3000, 10, 200, "ABC", 0.7);

    std::vector<int> sizes = {100, 500, 1000, 1500, 2000, 2500, 3000};
    std::vector<std::pair<std::string, std::vector<std::string>>> testCases = {
        {"random", randomBase},
        {"reverse_sorted", reverseSortedBase},
        {"nearly_sorted", nearlySortedBase},
        {"common_prefix", commonPrefixBase}
    };

    std::ofstream out("results/benchmark_results.csv");
    out << "size,array_type,algorithm,comparisons,time_ms\n";

    const int RUNS = 3;

    for (auto& tc : testCases) {
        std::cout << "\nTesting: " << tc.first << std::endl;
        for (int size : sizes) {
            std::cout << "  Size: " << size << std::flush;

            std::vector<std::string> data(tc.second.begin(), tc.second.begin() + size);

            auto measure_algo = [&](const std::string& name,
                                    std::function<void(std::vector<std::string>&)> func) {
                long long totalCmp = 0;
                double totalTime = 0;
                for (int r = 0; r < RUNS; ++r) {
                    std::vector<std::string> dataCopy = data;
                    auto m = StringSortTester::measure(dataCopy, func);
                    totalCmp += m.comparisons;
                    totalTime += m.timeMs;
                }
                out << size << "," << tc.first << "," << name << ","
                    << (totalCmp / RUNS) << "," << (totalTime / RUNS) << "\n";
            };

            measure_algo("std_quicksort", [](std::vector<std::string>& v) {
                if (!v.empty())
                    StringSortTester::standardQuickSort(v, 0, v.size() - 1);
            });

            measure_algo("std_mergesort", [](std::vector<std::string>& v) {
                if (!v.empty())
                    StringSortTester::standardMergeSort(v, 0, v.size() - 1);
            });

            measure_algo("string_quicksort", [](std::vector<std::string>& v) {
                if (!v.empty())
                    StringSortTester::stringQuickSort(v, 0, v.size() - 1);
            });

            measure_algo("string_mergesort", [](std::vector<std::string>& v) {
                if (!v.empty())
                    StringSortTester::stringMergeSort(v, 0, v.size() - 1);
            });

            measure_algo("msd_radix", [](std::vector<std::string>& v) {
                if (!v.empty())
                    StringSortTester::msdRadixSort(v);
            });

            measure_algo("msd_radix_cutoff", [](std::vector<std::string>& v) {
                if (!v.empty())
                    StringSortTester::msdRadixSortWithCutoff(v, 74);
            });

            std::cout << "" << std::endl;
        }
    }

    out.close();
    std::cout << "\n\nResults saved to results/benchmark_results.csv" << std::endl;

    return 0;
}