#include "QuickSort.h"
#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <string>
#include <random>

template<typename T>
bool is_sorted(const T* first, const T* last) {
    for (const T* it = first + 1; it < last; ++it) {
        if (*it < *(it - 1)) return false;
    }
    return true;
}

TEST(QuickSortTest, EmptyArray) {
    int* arr = nullptr;
    QuickSort::sort(arr, arr);
    EXPECT_TRUE(true);
}

TEST(QuickSortTest, SingleElement) {
    int arr[] = { 42 };
    QuickSort::sort(arr, arr + 1);
    EXPECT_EQ(arr[0], 42);
}

TEST(QuickSortTest, TwoElements) {
    int arr[] = { 5, 3 };
    QuickSort::sort(arr, arr + 2);
    EXPECT_TRUE(is_sorted(arr, arr + 2));
}

TEST(QuickSortTest, AlreadySorted) {
    int arr[] = { 1, 2, 3, 4, 5 };
    QuickSort::sort(arr, arr + 5);
    EXPECT_TRUE(is_sorted(arr, arr + 5));
}

TEST(QuickSortTest, ReverseSorted) {
    int arr[] = { 5, 4, 3, 2, 1 };
    QuickSort::sort(arr, arr + 5);
    EXPECT_TRUE(is_sorted(arr, arr + 5));
}

TEST(QuickSortTest, DuplicateElements) {
    int arr[] = { 3, 1, 2, 3, 1, 2, 3 };
    QuickSort::sort(arr, arr + 7);
    EXPECT_TRUE(is_sorted(arr, arr + 7));
}

TEST(QuickSortTest, LargeRandomArray) {
    const int SIZE = 10000;
    std::vector<int> arr(SIZE);
    std::vector<int> arr_copy(SIZE);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 10000);

    for (int i = 0; i < SIZE; ++i) {
        arr[i] = dis(gen);
        arr_copy[i] = arr[i];
    }

    QuickSort::sort(arr.data(), arr.data() + SIZE);

    std::sort(arr_copy.begin(), arr_copy.end());

    EXPECT_EQ(arr, arr_copy);
}

TEST(QuickSortTest, StringArray) {
    std::string arr[] = { "banana", "apple", "cherry", "date", "blueberry" };
    std::string arr_copy[] = { "banana", "apple", "cherry", "date", "blueberry" };

    QuickSort::sort(arr, arr + 5);
    std::sort(arr_copy, arr_copy + 5);

    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(arr[i], arr_copy[i]);
    }
}

TEST(QuickSortTest, CustomComparator) {
    int arr[] = { 5, 2, 9, 1, 5, 6 };

    QuickSort::sort(arr, arr + 6, [](int a, int b) { return a > b; });

    for (int i = 1; i < 6; ++i) {
        EXPECT_GE(arr[i - 1], arr[i]);
    }
}

struct Person {
    std::string name;
    int age;

    bool operator<(const Person& other) const {
        return age < other.age;
    }
};

TEST(QuickSortTest, CustomStruct) {
    Person people[] = {
        {"Alice", 30},
        {"Bob", 25},
        {"Charlie", 35},
        {"David", 20}
    };

    QuickSort::sort(people, people + 4);

    for (int i = 1; i < 4; ++i) {
        EXPECT_LE(people[i - 1].age, people[i].age);
    }
}

class MovableInt {
    int value;
    bool moved = false;
public:
    MovableInt(int v) : value(v) {}

    MovableInt(const MovableInt& other) : value(other.value) {}
    MovableInt& operator=(const MovableInt& other) {
        value = other.value;
        return *this;
    }

    MovableInt(MovableInt&& other) noexcept : value(other.value) {
        other.moved = true;
    }
    MovableInt& operator=(MovableInt&& other) noexcept {
        value = other.value;
        other.moved = true;
        moved = false;
        return *this;
    }

    int get() const { return value; }
    bool wasMoved() const { return moved; }

    bool operator<(const MovableInt& other) const {
        return value < other.value;
    }
};
TEST(QuickSortTest, MoveSemantics) {
    MovableInt arr[] = { 5, 2, 8, 1, 9 };
    QuickSort::sort(arr, arr + 5);

    for (int i = 1; i < 5; ++i) {
        EXPECT_LE(arr[i - 1].get(), arr[i].get());
    }
}