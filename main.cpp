#include "QuickSort.h"
#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <fstream>
#include <string>

template<typename T>
long long measure_sorting_time(std::vector<T>& data, std::size_t threshold) {
    auto data_copy = data;
    auto start = std::chrono::high_resolution_clock::now();

    QuickSort::sort_param(data_copy.data(),
        data_copy.data() + data_copy.size(),
        [](const T& a, const T& b) { return a < b; },
        threshold);

    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
}

template<typename T>
bool is_sorted(const std::vector<T>& arr) {
    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i] < arr[i - 1]) return false;
    }
    return true;
}

void insertion_sort_test(std::vector<int>& data) {
    for (size_t i = 1; i < data.size(); ++i) {
        int key = data[i];
        int j = i - 1;
        while (j >= 0 && data[j] > key) {
            data[j + 1] = data[j];
            j--;
        }
        data[j + 1] = key;
    }
}

int main() {
    std::vector<int> array_sizes = {2, 3, 4, 8, 16, 24, 32, 64, 128 };
    std::vector<std::size_t> thresholds = { 2, 4, 8, 16, 24, 32, 48, 64, 96, 128 };

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, 1000000);

    std::ofstream csv_file("sorting_results.csv");

    csv_file << "Threshold";
    for (int size : array_sizes) {
        csv_file << "," << size;
    }
    csv_file << "\n";

    for (std::size_t threshold : thresholds) {
        std::cout << "Testing threshold: " << threshold << "\n";
        csv_file << threshold;

        for (int size : array_sizes) {
            std::vector<int> data(size);
            for (int& val : data) {
                val = dist(gen);
            }

            long long total_time = 0;
            int runs = 5;

            for (int i = 0; i < runs; ++i) {
                total_time += measure_sorting_time(data, threshold);
            }

            long long avg_time = total_time / runs;

            std::vector<int> data_check = data;
            QuickSort::sort_param(data_check.data(),
                data_check.data() + data_check.size(),
                [](int a, int b) { return a < b; },
                threshold);

            if (!is_sorted(data_check)) {
                std::cerr << "ERROR: Array not sorted! Size: " << size
                    << ", Threshold: " << threshold << "\n";
            }

            std::cout << "  Size " << size << ": " << avg_time << " μs\n";
            csv_file << "," << avg_time;
        }

        csv_file << "\n";
        std::cout << "---\n";
    }

    csv_file.close();
    std::cout << "\nResults saved to sorting_results.csv\n";

    std::cout << "\nTesting with different data types...\n";

    std::vector<double> double_data(10000);
    std::uniform_real_distribution<double> double_dist(0.0, 1.0);
    for (double& val : double_data) {
        val = double_dist(gen);
    }

    std::cout << "Double array (size 10000):\n";
    for (std::size_t threshold : {8, 16, 32, 64}) {
        auto start = std::chrono::high_resolution_clock::now();
        QuickSort::sort_param(double_data.data(), double_data.data() + double_data.size(),
            [](double a, double b) { return a < b; },
            threshold);
        auto end = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        std::cout << "  Threshold " << threshold << ": " << time << " μs\n";
    }

    std::cout << "\n=== Insertion Sort Performance Test ===\n";
    std::ofstream insertion_csv("insertion_sort_results.csv");

    insertion_csv << "ArraySize,InsertionSortTime(μs)\n";

    std::vector<int> insertion_sizes = { 2, 3, 4, 8, 16, 24, 32, 64, 128};

    for (int size : insertion_sizes) {
        std::vector<int> data(size);
        for (int& val : data) {
            val = dist(gen);
        }

        long long total_time = 0;
        int runs = 3;

        for (int i = 0; i < runs; ++i) {
            auto data_copy = data;
            auto start = std::chrono::high_resolution_clock::now();
            insertion_sort_test(data_copy);
            auto end = std::chrono::high_resolution_clock::now();
            total_time += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

            if (!is_sorted(data_copy)) {
                std::cerr << "ERROR: Insertion sort failed for size " << size << "\n";
            }
        }

        long long avg_time = total_time / runs;
        std::cout << "Size " << size << ": " << avg_time << " μs\n";
        insertion_csv << size << "," << avg_time << "\n";
    }

    insertion_csv.close();
    std::cout << "Insertion sort results saved to insertion_sort_results.csv\n";

    std::cout << "\n=== Combined Results for Excel ===\n";
    std::ofstream combined_csv("combined_results.csv");

    combined_csv << "Algorithm/Threshold";
    for (int size : array_sizes) {
        combined_csv << "," << size;
    }
    combined_csv << "\n";

    for (size_t i = 0; i < thresholds.size(); ++i) {
        std::size_t threshold = thresholds[i];
        combined_csv << "QuickSort_th" << threshold;

        for (int size : array_sizes) {
            std::vector<int> data(size);
            for (int& val : data) {
                val = dist(gen);
            }

            long long total_time = 0;
            int runs = 3;
            for (int i = 0; i < runs; ++i) {
                total_time += measure_sorting_time(data, threshold);
            }
            combined_csv << "," << (total_time / runs);
        }
        combined_csv << "\n";
    }

    combined_csv << "InsertionSort";
    for (int size : array_sizes) {
        if (size <= 5000) {
            std::vector<int> data(size);
            for (int& val : data) {
                val = dist(gen);
            }

            long long total_time = 0;
            int runs = 3;
            for (int i = 0; i < runs; ++i) {
                auto data_copy = data;
                auto start = std::chrono::high_resolution_clock::now();
                insertion_sort_test(data_copy);
                auto end = std::chrono::high_resolution_clock::now();

                total_time += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
            }
            combined_csv << "," << (total_time / runs);
        }
        else {
            combined_csv << ",N/A";
        }
    }
    combined_csv << "\n";

    combined_csv.close();
    std::cout << "Combined results saved to combined_results.csv\n";

    return 0;
}