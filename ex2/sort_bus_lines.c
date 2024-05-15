#include "sort_bus_lines.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_NUM 0
#define DIST 1
#define DURATION 2
#define DISTANCE_ENUM 0
#define DURATION_ENUM 1

void swap(BusLine *start_copy, BusLine *next)
{
  BusLine temp = *start_copy;
  *start_copy = *next;
  *next = temp;
}

BusLine *partition(BusLine *start, BusLine *end, SortType sort_type)
{
    BusLine *pivot = end;
    BusLine *i = start - 1;

    for (BusLine *j = start; j < end; j++)
    {
        if ((sort_type == DISTANCE_ENUM && j->distance <= pivot->distance) ||
            (sort_type == DURATION_ENUM && j->duration <= pivot->duration))
        {
            i++;
            swap(i, j);
        }
    }

    swap(i + 1, end);
    return i + 1;
}

void quick_sort(BusLine *start, BusLine *end, SortType sort_type)
{
  if (start < end)
  {
    BusLine *pi = partition (start, end, sort_type);
    quick_sort (start, pi-1, sort_type);
    quick_sort (pi+1, end, sort_type);
  }
}

void bubble_sort(BusLine *start, BusLine *end) {
  BusLine *start_copy = start, *end_copy = end;

  while (start_copy != end_copy) {
    BusLine *next = start;
    while (next != end_copy) {
      BusLine *next_line = next + 1;
      if (strcmp(next->name, next_line->name) > 0) {
        swap(next, next_line);
      }
      next++;
    }
    end_copy--;
  }
}