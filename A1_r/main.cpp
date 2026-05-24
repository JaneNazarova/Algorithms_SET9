#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>

const int R = 75;
const int CUTOFF = 74;

int charAt(const std::string& s, int depth) {
    if (depth < (int)s.length()) {
        return (unsigned char)s[depth];
    }
    return -1;
}

void stringQuickSort(std::vector<std::string>& arr, int left, int right, int depth) {
    if (left >= right) return;

    int lt = left, gt = right;
    int pivot = charAt(arr[left], depth);
    int i = left + 1;

    while (i <= gt) {
        int t = charAt(arr[i], depth);
        if (t < pivot) {
            std::swap(arr[lt++], arr[i++]);
        } else if (t > pivot) {
            std::swap(arr[i], arr[gt--]);
        } else {
            i++;
        }
    }

    stringQuickSort(arr, left, lt - 1, depth);
    if (pivot != -1) {
        stringQuickSort(arr, lt, gt, depth + 1);
    }
    stringQuickSort(arr, gt + 1, right, depth);
}

int getCharIndex(const std::string& s, int d) {
    if (d >= (int)s.length()) return R - 1;

    unsigned char c = s[d];

    if (c == '!') return 0;
    if (c == '#') return 1;
    if (c == '%') return 2;
    if (c == '(') return 3;
    if (c == ')') return 4;
    if (c == '*') return 5;
    if (c == '-') return 6;
    if (c == '.') return 7;
    if (c == ':') return 8;
    if (c == ';') return 9;
    if (c == '^') return 10;
    if (c == '@') return 11;

    if (c >= '0' && c <= '9') return 12 + (c - '0');

    if (c >= 'A' && c <= 'Z') return 22 + (c - 'A');

    if (c >= 'a' && c <= 'z') return 48 + (c - 'a');

    return 0;
}

void msdRadixSortWithCutoff(std::vector<std::string>& arr) {
    if (arr.empty()) return;

    std::vector<std::string> aux(arr.size());

    std::function<void(int, int, int)> msdSort = [&](int left, int right, int d) {
        if (left >= right) return;

        if (right - left + 1 < CUTOFF) {
            stringQuickSort(arr, left, right, d);
            return;
        }

        std::vector<int> count(R + 1, 0);

        for (int i = left; i <= right; ++i) {
            int c = getCharIndex(arr[i], d);
            count[c + 1]++;
        }

        for (int r = 0; r < R; ++r) {
            count[r + 1] += count[r];
        }

        for (int i = left; i <= right; ++i) {
            int c = getCharIndex(arr[i], d);
            aux[count[c]++] = arr[i];
        }

        for (int i = left; i <= right; ++i) {
            arr[i] = aux[i - left];
        }

        for (int r = 0; r < R - 1; ++r) {
            msdSort(left + count[r], left + count[r + 1] - 1, d + 1);
        }
    };

    msdSort(0, arr.size() - 1, 0);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    std::cin.ignore();

    std::vector<std::string> arr(n);
    for (int i = 0; i < n; ++i) {
        std::getline(std::cin, arr[i]);
    }

    if (n > 0) {
        msdRadixSortWithCutoff(arr);
    }

    for (const auto& s : arr) {
        std::cout << s << '\n';
    }

    return 0;
}