#pragma once
#include <cstddef>
#include <utility>
#include <algorithm>

namespace QuickSort {

    template<typename T, typename Compare>
    T median_of_three(T& a, T& b, T& c, Compare comp) {
        if (comp(a, b)) {
            if (comp(b, c)) return b;
            if (comp(a, c)) return c;
            return a;
        }
        else {
            if (comp(a, c)) return a;
            if (comp(b, c)) return c;
            return b;
        }
    }

    template<typename T, typename Compare>
    void insertion_sort(T* first, T* last, Compare comp) {
        if (first >= last) return;

        for (T* i = first + 1; i <= last; ++i) {
            T key = std::move(*i);
            T* j = i - 1;

            while (j >= first && comp(key, *j)) {
                *(j + 1) = std::move(*j);
                --j;
            }
            *(j + 1) = std::move(key);
        }
    }

    template<typename T, typename Compare>
    T* partition(T* first, T* last, T pivot, Compare comp) {
        T* i = first - 1;
        T* j = last + 1;

        while (true) {
            do {
                ++i;
            } while (comp(*i, pivot));

            do {
                --j;
            } while (comp(pivot, *j));

            if (i >= j) {
                return j;
            }

            std::swap(*i, *j);
        }
    }

    template<typename T, typename Compare>
    void quick_sort_param(T* first, T* last, Compare comp, std::size_t INSERTION_THRESHOLD) {
        while (last - first > INSERTION_THRESHOLD) {
            T* mid = first + (last - first) / 2;
            T pivot = median_of_three(*first, *mid, *last, comp);

            T* p = partition(first, last, pivot, comp);

            if (p - first < last - p) {
                quick_sort_param(first, p, comp, INSERTION_THRESHOLD);
                first = p + 1;
            }
            else {
                quick_sort_param(p + 1, last, comp, INSERTION_THRESHOLD);
                last = p;
            }
        }

        if (first < last) {
            insertion_sort(first, last, comp);
        }
    }

    template<typename T, typename Compare>
    void sort_param(T* first, T* last, Compare comp, std::size_t INSERTION_THRESHOLD = 16) {
        if (first == nullptr || last == nullptr || first >= last) {
            return;
        }
        quick_sort_param(first, last - 1, comp, INSERTION_THRESHOLD);
    }

    template<typename T, typename Compare>
    void quick_sort(T* first, T* last, Compare comp) {
        constexpr std::size_t INSERTION_THRESHOLD = 16;
        while (last - first > INSERTION_THRESHOLD) {
            T* mid = first + (last - first) / 2;
            T pivot = median_of_three(*first, *mid, *last, comp);

            T* p = partition(first, last, pivot, comp);

            if (p - first < last - p) {
                quick_sort(first, p, comp);
                first = p + 1;
            }
            else {
                quick_sort(p + 1, last, comp);
                last = p;
            }
        }

        if (first < last) {
            insertion_sort(first, last, comp);
        }
    }

    template<typename T, typename Compare>
    void sort(T* first, T* last, Compare comp) {
        if (first == nullptr || last == nullptr || first >= last) {
            return;
        }

        quick_sort(first, last - 1, comp);
    }

    template<typename T>
    void sort(T* first, T* last) {
        sort(first, last, [](const T& a, const T& b) { return a < b; });
    }

}
