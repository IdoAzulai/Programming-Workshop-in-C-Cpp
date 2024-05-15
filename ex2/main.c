// * TODO add documentation

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "test_bus_lines.h"
//#include "test_bus_lines.c"
#include "sort_bus_lines.h"
//#include "sort_bus_lines.c"

#define MAX_LEN_INPUT 60
#define MAX_SLICE_LEN 20
#define MAX_SLICE_LEN_1 21
#define TEN_BASE 10
#define A_LOWER_ASCII 'a'
#define Z_TOP_ASCII 'z'

#define RUN_CMD 1

#define BY_DISTANCE 0
#define BY_DURATION 1

#define DIST_TOP_LIM 1000
#define DIST_LOW_LIM 0
#define DURATION_LOW_LIM 10
#define DURATION_TOP_LIM 100


/// prints array, values comma seperated
void print_array(BusLine *start, BusLine *end)
{
    while (start != (end+1))
    {
        printf("%s,%d,%d\n", start->name, start->distance, start->duration);
        start++;
    }
}

/// checks if name is legal - only low letters and digits 0-9
int check_name_input(const char *line)
{
  for (int i = 0; line[i] != '\0'; i++)
  {
    if (!(isdigit(line[i]) ||
        (line[i] >= A_LOWER_ASCII && line[i] <= Z_TOP_ASCII)))
    {
      return 0;
    }
  }
  return 1;
}

/// checks if distance is legal - between bounds
int check_dist_input(char *distance)
{
  long dist = strtol(distance, NULL, TEN_BASE);
  return dist >= DIST_LOW_LIM && dist <= DIST_TOP_LIM;
}

/// checks if distance is legal - between bounds
int check_duration_input(char *duration)
{
  long dur = strtol(duration, NULL, TEN_BASE);
  return dur >= DURATION_LOW_LIM && dur <= DURATION_TOP_LIM;
}

/// returns a pointer to a free memory space that will hold all buses
BusLine* create_bus_lines(int arr_len)
{
  return calloc(arr_len, sizeof(BusLine));
}

/// handles num of bus lines input for the whole program from user,
/// continues until it is valid (a int bigger than 0)
void get_input(char lines_num[MAX_LEN_INPUT])
{
    while (1)
    {
        fprintf(stdout,"Enter number of lines. Then enter\n");
        if(fgets(lines_num, MAX_LEN_INPUT, stdin) == NULL)
        {
            fprintf(stdout,"ERROR: number of lines "
                           "should be a whole positive number.\n");
        }
//        long arr_len = strtol(lines_num, NULL, TEN_BASE);
        else if (strtol(lines_num, NULL, TEN_BASE) <= 0)
        {
            fprintf(stdout,"ERROR: number of lines "
                           "should be a whole positive number.\n");
        }
        else
        {
           break;
        }
    }
}

int validate_bus_name(char line[MAX_SLICE_LEN_1])
{
  if (!check_name_input (line)) {
    fprintf(stdout,
            "ERROR: bus name should contain "
                   "only digits and small characters.\n");
    return 0;
  }
  return 1;
}

int validate_distance(char *distance)
{
  if (!check_dist_input (distance))
  {
    fprintf(stdout,
            "ERROR: distance should be an integer "
                   "between 0 and 1000 (inclusive).\n");
    return 0;
  }
  return 1;
}

int validate_duration(char *duration)
{
  if (!check_duration_input (duration))
  {
    fprintf(stdout,
            "ERROR: duration should be an integer "
                   "between 10 and 100 (inclusive).\n");
    return 0;
  }
  return 1;
}

/// handles input of <name>,<distance>,<duration> to variabls. checks valid
void get_line_info(char line_info[MAX_LEN_INPUT],
                  char line[], char distance[], char duration[])
{
    while (1)
    {
        fprintf(stdout,"Enter line info. Then enter\n");
        if((fgets(line_info, MAX_LEN_INPUT, stdin) == NULL) ||
                (sscanf(line_info,
                        "%20[^,],%20[^,],%20[^,]",
                               line, distance, duration) != 3))
        {
            fprintf(stdout,
                    "ERROR: Invalid input format. Please try again.\n");
        }
        else if (validate_bus_name (line) &&
                validate_distance (distance) &&
                validate_duration (duration))
        {
            break;
        }
    }
}

/// creats a backup of the start and end of the busses array
/// in case of corruption
BusLine *mem_backup (BusLine *start, BusLine *end, BusLine *end_copy)
{
    int size = sizeof(BusLine);
    BusLine *start_copy = (BusLine*) calloc ((end-start), size);
    BusLine *runner = start_copy;
    if (start_copy != NULL)
    {
        if (start ==  end)
        {
            memcpy(start_copy,start,size);
            end_copy = start_copy;
        }
        else
        {
            for (int i = 0; i < (end-start-1); ++i) {
                memcpy(runner,start,size);
                runner++;
                start++;
            }
            memcpy(end_copy, end, sizeof(BusLine));
        }
    }
    return start_copy;
}

/// frees memory - if w have only one line we don't want to double-free
void free_copy(BusLine *start_copy,BusLine * end_copy)
{
    if (start_copy == end_copy)
    {
        free(start_copy);
    }
    else
    {
        free(start_copy);
        free(end_copy);
    }
}

/// creats a copy of start&end, tests the algos and if pointers have changed
void run_tests (BusLine *start, BusLine *end)
{
    BusLine *end_copy = (BusLine*) calloc ((end-start), sizeof(BusLine));
    BusLine *start_copy = mem_backup(start, end, end_copy);
    if (!is_sorted_by_distance(start, end))
    {
        printf("TEST 1 FAILED: Not sorted by distance\n");
    } else
    {
        printf("TEST 1 PASSED: The array is sorted by distance\n");
    }
    if (!is_equal(start, end, start_copy, end_copy))
    {
        printf("TEST 2 FAILED: Array changed\n");
    } else
    {
        printf("TEST 2 PASSED: The array has the same items after sorting\n");
    }
    if (!is_sorted_by_duration(start, end))
    {
        printf("TEST 3 FAILED: Not sorted by duration\n");
    } else
    {
        printf("TEST 3 PASSED: The array is sorted by duration\n");
    }
    if (!is_equal(start, end, start_copy, end_copy))
    {
        printf("TEST 4 FAILED: Array changed\n");
    } else
    {
        printf("TEST 4 PASSED: The array has the same items after sorting\n");
    }
    if (is_sorted_by_name(start, end))
    {
        printf("TEST 5 FAILED: Not sorted by name\n");
    } else
    {
        printf("TEST 5 PASSED: The array is sorted by name\n");
    }
    if (!is_equal(start, end, start_copy, end_copy))
    {
        printf("TEST 6 FAILED: Array changed");
    } else
    {
        printf("TEST 6 PASSED: The array has the same items after sorting");
    }
    free_copy(start_copy, end_copy);
}

/// handles input into arguments, operates the program
int run_program(int argc, char *argv[], BusLine *start, BusLine *end)
{
    if (argc != 2)
    {
        return 0;
    }
    if (strcmp(argv[RUN_CMD], "test") == 0)
    {
        run_tests(start, end);
        return 1;
    }
    else if (strcmp(argv[RUN_CMD], "by_distance") == 0)
    {
        quick_sort(start, end, BY_DISTANCE);
        print_array(start, end);
        return 1;
    }
    else if (strcmp(argv[RUN_CMD], "by_duration") == 0)
    {
        quick_sort(start, end, BY_DURATION);
        print_array(start, end);
        return 1;
    }
    else if (strcmp(argv[RUN_CMD], "by_name") == 0)
    {
        bubble_sort(start, end);
        print_array(start, end);
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[])
{
  char lines_num[MAX_LEN_INPUT];
  get_input (lines_num); // gets input - total num of lines
  long arr_len = strtol(lines_num, NULL, TEN_BASE);
  BusLine *arr_dynamic = create_bus_lines (arr_len);
  BusLine *arr_dynamic_backup = arr_dynamic;
  if (arr_dynamic == NULL)
  {// checks if memory allocation for buses array failed
    fprintf(stdout,"ERROR: Memory allocation failed.\n");
    free(arr_dynamic_backup);
    return EXIT_FAILURE;
  }

  for (int i = 0; i < arr_len; i++)
  {
    //handles input for each line
    char line_info[MAX_LEN_INPUT], line[MAX_SLICE_LEN_1],
    distance[MAX_SLICE_LEN_1], duration[MAX_SLICE_LEN_1];
    int distance_long, duration_long;
    get_line_info (line_info, line, distance, duration);

    // saves info into array of buses
    distance_long = strtol (distance, NULL, TEN_BASE);
    duration_long = strtol (duration, NULL, TEN_BASE);
    strncpy (arr_dynamic->name, line, MAX_SLICE_LEN);
    arr_dynamic->distance = distance_long;
    arr_dynamic->duration = duration_long;
    arr_dynamic++;
  }

  BusLine *start = arr_dynamic_backup;
  BusLine *end = arr_dynamic_backup + arr_len - 1;
  // frees memory after program runs (tests or just sorting)
  if (!run_program(argc, argv, start, end))
  {
      free(arr_dynamic_backup);
      return EXIT_FAILURE;
  }
  else
  {
      free(arr_dynamic_backup);
      return EXIT_SUCCESS;
  }
}

