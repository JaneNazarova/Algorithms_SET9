#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

int computeLCP(const std::string& a, const std::string& b) {
    int minLen = std::min(a.length(), b.length());
    for (int i = 0; i < minLen; ++i) {
        if (a[i] != b[i]) return i;
    }
    return minLen;
}

int compareWithLCP(const std::string& a, const std::string& b, int lcp) {
    if (lcp < (int)a.length() && lcp < (int)b.length()) {
        if (a[lcp] != b[lcp]) {
            return (unsigned char)a[lcp] - (unsigned char)b[lcp];
        }
        return 0;
    }
    return a.length() - b.length();
}

void mergeWithLCP(std::vector<std::string>& arr, int left, int mid, int right) {
    std::vector<std::string> temp(right - left + 1);
    int i = left, j = mid + 1, k = 0;

    while (i <= mid && j <= right) {
        int lcp = computeLCP(arr[i], arr[j]);
        if (compareWithLCP(arr[i], arr[j], lcp) <= 0) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }

    while (i <= mid) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];

    for (int idx = 0; idx < (int)temp.size(); ++idx) {
        arr[left + idx] = temp[idx];
    }
}

void mergeSort(std::vector<std::string>& arr, int left, int right) {
    if (left >= right) return;

    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    mergeWithLCP(arr, left, mid, right);
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
        mergeSort(arr, 0, n - 1);
    }

    for (const auto& s : arr) {
        std::cout << s << '\n';
    }

    return 0;
}