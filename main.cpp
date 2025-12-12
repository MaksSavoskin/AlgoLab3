#include "QuickSort.h"
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <chrono>

int main() {
    {
        int arr[] = { 5, 2, 9, 1, 5, 6, 3, 8, 4, 7 };
        int size = sizeof(arr) / sizeof(arr[0]);

        std::cout << "Original array: ";
        for (int num : arr) std::cout << num << " ";
        std::cout << std::endl;

        QuickSort::sort(arr, arr + size);

        std::cout << "Sorted array: ";
        for (int num : arr) std::cout << num << " ";
        std::cout << std::endl;
    }

    {
        std::string words[] = { "banana", "apple", "cherry", "date", "blueberry" };
        int size = sizeof(words) / sizeof(words[0]);

        std::cout << "\nOriginal words: ";
        for (const auto& w : words) std::cout << w << " ";
        std::cout << std::endl;

        QuickSort::sort(words, words + size,
            [](const std::string& a, const std::string& b) {
                return a.length() < b.length();
            });

        std::cout << "Sorted by length: ";
        for (const auto& w : words) std::cout << w << " ";
        std::cout << std::endl;
    }

    {
        const int SIZE = 1000000;
        std::vector<int> big_arr(SIZE);

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 1000000);

        for (int i = 0; i < SIZE; ++i) {
            big_arr[i] = dis(gen);
        }

        auto start = std::chrono::high_resolution_clock::now();
        QuickSort::sort(big_arr.data(), big_arr.data() + SIZE);
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "\nSorted " << SIZE << " elements in " << duration.count() << " ms" << std::endl;

        bool sorted = true;
        for (int i = 1; i < SIZE; ++i) {
            if (big_arr[i] < big_arr[i - 1]) {
                sorted = false;
                break;
            }
        }
        std::cout << "Array is " << (sorted ? "sorted correctly" : "NOT sorted") << std::endl;
    }

    return 0;
}