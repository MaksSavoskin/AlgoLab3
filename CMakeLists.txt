cmake_minimum_required(VERSION 3.10)
project(QuickSort)

set(CMAKE_CXX_STANDARD 17)

add_executable(main main.cpp QuickSort.h)

find_package(GTest REQUIRED)
add_executable(QuickSortTest QuickSortTest.cpp QuickSort.h)
target_link_libraries(QuickSortTest GTest::gtest GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(QuickSortTest)