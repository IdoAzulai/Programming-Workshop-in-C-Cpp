#ifndef EX2_REPO_SORTBUSLINES_H
#define EX2_REPO_SORTBUSLINES_H
// write only between #define EX2_REPO_SORTBUSLINES_H and
// #endif //EX2_REPO_SORTBUSLINES_H
#include <string.h>
#define NAME_LEN 21
/**
 * this is the struct for the bus array elements,
 * pretty self explanatory
 */
typedef struct BusLine
{
    char name[NAME_LEN];
    int distance, duration;
} BusLine;
typedef enum SortType
{
    DISTANCE,
    DURATION
} SortType;

/**
 * sorts bus lines by Line name (increasing order, alphabetical).
 * Uses bubble sort algorithm.
 * Gets two pointers - *start and *end of bus lines array.
 * Prints the sorted array
*/
void bubble_sort (BusLine *start, BusLine *end);

/**
 * sorts bus lines by distance OR by travel duration (increasing order).
 * Uses quick sort algorithm.
 * Gets two pointers - *start and *end of bus lines array.
 * sort_type - an Enum struct, with the fields: DURATION, DISTANCE.
 * Prints the sorted array
*/
void quick_sort (BusLine *start, BusLine *end, SortType sort_type);

/**
 * the partition function for the bubblesort algo
 * holds the pivot and moves it
 */
BusLine *partition (BusLine *start, BusLine *end, SortType sort_type);
// write only between #define EX2_REPO_SORTBUSLINES_H and
// #endif //EX2_REPO_SORTBUSLINES_H
#endif //EX2_REPO_SORTBUSLINES_H
