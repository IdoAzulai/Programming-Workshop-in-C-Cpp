#ifndef EX2_REPO_TESTBUSLINES_H
#define EX2_REPO_TESTBUSLINES_H
// write only between #define EX2_REPO_TESTBUSLINES_H and
// #endif //EX2_REPO_TESTBUSLINES_H
#include "sort_bus_lines.h"

/**
 * uses quicksort and then checks if the array is sorted by dist
 * if array length is one, returns that the array is sorted
 */
int is_sorted_by_distance (BusLine *start, BusLine *end);

/**
 * uses quicksort and then checks if the array is sorted by duration
 * if array length is one, returns that the array is sorted
 */
int is_sorted_by_duration (BusLine *start, BusLine *end);

/**
* uses bubblesort and then checks if the array is sorted by name
 * if array length is one, returns that the array is sorted */
int is_sorted_by_name (BusLine *start, BusLine *end);

/**
 * Uses binary search to check is two arrays are the same (by name)
 * because one array is sorted by name and the other isn't, we will iterate
 * over the non-sorted one, and for each element we will check if it appears
 * in the sorted one. Because it's sorted, we will use binary search.
 */
int is_equal (BusLine *start_sorted,
              BusLine *end_sorted, BusLine *start_original,
              BusLine *end_original);
// write only between #define EX2_REPO_TESTBUSLINES_H and
// #endif //EX2_REPO_TESTBUSLINES_H
#endif //EX2_REPO_TESTBUSLINES_H
