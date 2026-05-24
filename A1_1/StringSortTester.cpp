#include "StringSortTester.h"
#include <algorithm>
#include <chrono>
#include <functional>

static long long charComparisons = 0;
static const int R = 75;

static void resetComparisons() {
    charComparisons = 0;
}

static int getCharIndex(const std::string& s, int d) {
    if (d >= (int)s.length()) return R - 1;
    char c = s[d];
    charComparisons++;
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return 26 + (c - 'a');
    if (c >= '0' && c <= '9') return 52 + (c - '0');
    std::string special = "!@#%:;^&*()-.";
    for (size_t i = 0; i < special.length(); ++i)
        if (c == special[i]) return 62 + i;
    return 0;
}

static int compareStrings(const std::string& a, const std::string& b) {
    size_t minLen = std::min(a.length(), b.length());
    for (size_t i = 0; i < minLen; ++i) {
        charComparisons++;
        if (a[i] != b[i])
            return (unsigned char)a[i] - (unsigned char)b[i];
    }
    charComparisons++;
    return a.length() - b.length();
}

static int compareAtDepth(const std::string& a, const std::string& b, int depth) {
    if (depth < (int)a.length() && depth < (int)b.length()) {
        charComparisons++;
        if (a[depth] != b[depth])
            return (unsigned char)a[depth] - (unsigned char)b[depth];
        return 0;
    }
    charComparisons++;
    return a.length() - b.length();
}

static int computeLCP(const std::string& a, const std::string& b) {
    int minLen = std::min(a.length(), b.length());
    for (int i = 0; i < minLen; ++i) {
        charComparisons++;
        if (a[i] != b[i]) return i;
    }
    return minLen;
}

// STANDARD QUICKSORT
void StringSortTester::standardQuickSort(std::vector<std::string>& arr, int left, int right) {
    if (left >= right) return;
    std::string pivot = arr[right];
    int i = left - 1;
    for (int j = left; j < right; ++j) {
        if (compareStrings(arr[j], pivot) <= 0) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[right]);
    int pivotIdx = i + 1;
    standardQuickSort(arr, left, pivotIdx - 1);
    standardQuickSort(arr, pivotIdx + 1, right);
}

// STANDARD MERGESORT
void StringSortTester::standardMergeSort(std::vector<std::string>& arr, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    standardMergeSort(arr, left, mid);
    standardMergeSort(arr, mid + 1, right);

    std::vector<std::string> temp(right - left + 1);
    int i = left, j = mid + 1, k = 0;
    while (i <= mid && j <= right) {
        if (compareStrings(arr[i], arr[j]) <= 0)
            temp[k++] = arr[i++];
        else
            temp[k++] = arr[j++];
    }
    while (i <= mid) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];
    for (int idx = 0; idx < (int)temp.size(); ++idx)
        arr[left + idx] = temp[idx];
}

// STRING QUICKSORT
void StringSortTester::stringQuickSort(std::vector<std::string>& arr, int left, int right, int depth) {
    if (left >= right) return;
    int lt = left, gt = right;
    std::string pivot = arr[left];
    int i = left + 1;
    while (i <= gt) {
        int cmp = compareAtDepth(arr[i], pivot, depth);
        if (cmp < 0) std::swap(arr[lt++], arr[i++]);
        else if (cmp > 0) std::swap(arr[i], arr[gt--]);
        else i++;
    }
    stringQuickSort(arr, left, lt - 1, depth);
    if (depth < (int)pivot.length() || depth < (int)arr[lt].length())
        stringQuickSort(arr, lt, gt, depth + 1);
    stringQuickSort(arr, gt + 1, right, depth);
}

// STRING MERGESORT with LCP
void StringSortTester::stringMergeSort(std::vector<std::string>& arr, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    stringMergeSort(arr, left, mid);
    stringMergeSort(arr, mid + 1, right);

    std::vector<std::string> temp(right - left + 1);
    int i = left, j = mid + 1, k = 0;
    while (i <= mid && j <= right) {
        int lcp = computeLCP(arr[i], arr[j]);
        if (lcp < (int)arr[i].length() && lcp < (int)arr[j].length()) {
            charComparisons++;
            if (arr[i][lcp] <= arr[j][lcp])
                temp[k++] = arr[i++];
            else
                temp[k++] = arr[j++];
        } else {
            charComparisons++;
            if (arr[i].length() <= arr[j].length())
                temp[k++] = arr[i++];
            else
                temp[k++] = arr[j++];
        }
    }
    while (i <= mid) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];
    for (int idx = 0; idx < (int)temp.size(); ++idx)
        arr[left + idx] = temp[idx];
}

// MSD RADIX SORT
void StringSortTester::msdRadixSort(std::vector<std::string>& arr) {
    if (arr.empty()) return;
    std::vector<std::string> aux(arr.size());

    std::function<void(int, int, int)> msdSort = [&](int left, int right, int d) {
        if (left >= right) return;
        std::vector<int> count(R + 1, 0);
        for (int i = left; i <= right; ++i)
            count[getCharIndex(arr[i], d) + 1]++;
        for (int r = 0; r < R; ++r)
            count[r + 1] += count[r];
        for (int i = left; i <= right; ++i) {
            int c = getCharIndex(arr[i], d);
            aux[count[c]++] = arr[i];
        }
        for (int i = left; i <= right; ++i)
            arr[i] = aux[i - left];
        for (int r = 0; r < R - 1; ++r)
            msdSort(left + count[r], left + count[r + 1] - 1, d + 1);
    };

    msdSort(0, arr.size() - 1, 0);
}

// MSD RADIX SORT with cutoff
void StringSortTester::msdRadixSortWithCutoff(std::vector<std::string>& arr, int cutoff) {
    if (arr.empty()) return;
    std::vector<std::string> aux(arr.size());

    std::function<void(int, int, int)> msdSort = [&](int left, int right, int d) {
        if (left >= right) return;
        if (right - left + 1 < cutoff) {
            stringQuickSort(arr, left, right, d);
            return;
        }
        std::vector<int> count(R + 1, 0);
        for (int i = left; i <= right; ++i)
            count[getCharIndex(arr[i], d) + 1]++;
        for (int r = 0; r < R; ++r)
            count[r + 1] += count[r];
        for (int i = left; i <= right; ++i) {
            int c = getCharIndex(arr[i], d);
            aux[count[c]++] = arr[i];
        }
        for (int i = left; i <= right; ++i)
            arr[i] = aux[i - left];
        for (int r = 0; r < R - 1; ++r)
            msdSort(left + count[r], left + count[r + 1] - 1, d + 1);
    };

    msdSort(0, arr.size() - 1, 0);
}

// MEASUREMENT
SortMetrics StringSortTester::measure(std::vector<std::string> data,
                                       std::function<void(std::vector<std::string>&)> sortFunc) {
    resetComparisons();
    auto start = std::chrono::high_resolution_clock::now();
    sortFunc(data);
    auto end = std::chrono::high_resolution_clock::now();
    SortMetrics m;
    m.comparisons = charComparisons;
    m.timeMs = std::chrono::duration<double, std::milli>(end - start).count();
    return m;
}