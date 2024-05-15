#include "test_bus_lines.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LEN 21

#define ARRAY_START 0

#define BY_DISTANCE 0
#define BY_DURATION 1

/// checks which str is bigger, using strcmp().
/// I could actually just use strcmp in the code instead
int bigger_str (char a[], char b[])
{
  int result = strcmp(a, b);
  if (result > 0)
  {
    // String 'a' is lexicographically greater than string 'b'
    return 1;
  }
  else if (result < 0)
  {
    // String 'a' is lexicographically smaller than string 'b'
    return 0;
  }
  else
  {
    // Strings are equal
    return -1;
  }
}

/// iterates over array,returns len
int arr_len(BusLine *start_sorted,BusLine *end_sorted)
{
    int i = 0;
    while(start_sorted != end_sorted)
    {
        i++;
        start_sorted++;
    }
    return i;
}

int is_sorted_by_duration(BusLine *start, BusLine *end)
{
  // check id array is empty or only one object
 if (start == end)
 {
     return 1;
 }
 quick_sort(start,end,BY_DURATION);
 int prev_duration = start[ARRAY_START].duration;
 start++;
 while (start != end)
 {
   if (start->duration < prev_duration)
   {
     return 0;
   }
   else
   {
     prev_duration = start->duration;
     start++;
   }
 }
 return 1;
}

int is_sorted_by_distance(BusLine *start, BusLine *end)
{
  if (start == end)
  {
      return 1;
  }
  quick_sort(start,end,BY_DISTANCE);
  int prev_dist = start[ARRAY_START].distance;
  start++;
  while (start != end)
  {
    if (start->distance < prev_dist)
    {
      return 0;//  for (int i=0; i<arr_len; i++){
    }
    else
    {
      prev_dist = start->distance;
        start++;
    }
  }
  return 1;
}

int is_sorted_by_name(BusLine *start, BusLine *end)
{
  if (start == end)
  {
      return 0;
  }
  bubble_sort(start, end);
  char prev_name[NAME_LEN];
  strcpy (prev_name, start->name);
  start++;
  while (start != end)
  {
    if (bigger_str (start->name, prev_name) == 1)
    {
      return 0;
    }
    else
    {
      strcpy (prev_name, start->name);
      start++;
    }
  }
  return 1;
}

int is_equal(BusLine *start_sorted, BusLine *end_sorted,
             BusLine *start_original, BusLine *end_original)
{
    for (BusLine *original_runner = start_original;
            original_runner < end_original ; ++original_runner) {
        BusLine *low = start_sorted;
        BusLine *high = end_sorted;
        while (low <= high)
        {
            int size = arr_len(start_sorted, end_sorted);
            BusLine *mid = start_sorted + size / 2; // middle of sorted array

            int comparison = strcmp(original_runner->name, mid->name);

            if (comparison == 0) {
                // Key found, return the pointer to the element
                break;
            } else if (comparison < 0) {
                // Key is greater, search in the right half
                low = mid + 1;
            } else {
                // Key is smaller, search in the left half
                high = mid - 1;
            }
            if ((mid == low) || (mid == high))
            {
                return 0;
            }
        }
    }
    return 1;
}