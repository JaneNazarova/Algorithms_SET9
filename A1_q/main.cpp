#include <iostream>
#include <vector>
#include <string>

int charAt(const std::string& s, int depth) {
    if (depth < (int)s.length()) {
        return (unsigned char)s[depth];
    }
    return -1;
}

void stringQuickSort(std::vector<std::string>& arr, int left, int right, int depth) {
    if (left >= right) return;

    int lt = left;
    int gt = right;
    int pivot = charAt(arr[left], depth);
    int i = left + 1;

    while (i <= gt) {
        int t = charAt(arr[i], depth);

        if (t < pivot) {
            std::swap(arr[lt], arr[i]);
            lt++;
            i++;
        }
        else if (t > pivot) {
            std::swap(arr[i], arr[gt]);
            gt--;
        }
        else {
            i++;
        }
    }

    stringQuickSort(arr, left, lt - 1, depth);

    if (pivot != -1) {
        stringQuickSort(arr, lt, gt, depth + 1);
    }

    stringQuickSort(arr, gt + 1, right, depth);
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
        stringQuickSort(arr, 0, n - 1, 0);
    }

    for (const auto& s : arr) {
        std::cout << s << '\n';
    }

    return 0;
}