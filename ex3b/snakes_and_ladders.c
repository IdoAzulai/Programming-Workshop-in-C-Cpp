#include <string.h> // For strlen(), strcmp(), strcpy()
#include "markov_chain.h"

#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))

#define EMPTY (-1)
#define BOARD_SIZE 100
#define MAX_GENERATION_LENGTH 60

#define DICE_MAX 6
#define NUM_OF_TRANSITIONS 20

#define ARG_NUM_3 3
#define SEED 1
#define PATH_NUM 2
#define TEN_BASE 10

#define LADDER_TO "[%d]-ladder to %d -> "
#define SNAKE_TO "[%d]-snake to %d -> "
#define NOT_ENOUGH_PARAM "Usage: Not enough params. Should be 3.\n"
#define WALK_NUMBER "Random Walk %d: "


/**
 * represents the transitions by ladders and snakes in the game
 * each tuple (x,y) represents a ladder from x to if x<y or a snake otherwise
 */
const int transitions[][2] = {{13, 4},
                              {85, 17},
                              {95, 67},
                              {97, 58},
                              {66, 89},
                              {87, 31},
                              {57, 83},
                              {91, 25},
                              {28, 50},
                              {35, 11},
                              {8,  30},
                              {41, 62},
                              {81, 43},
                              {69, 32},
                              {20, 39},
                              {33, 70},
                              {79, 99},
                              {23, 76},
                              {15, 47},
                              {61, 14}};

/**
 * struct represents a Cell in the game board
 */
typedef struct Cell {
    int number; // Cell number 1-100
    int ladder_to;
    // ladder_to represents the jump of the ladder in case there is one
    // from this square
    int snake_to;
    // snake_to represents the jump of the snake in case there is one
    // from this square
    //both ladder_to and snake_to should be -1 if the Cell doesn't have them
} Cell;


/// Get *data that is a ptr to Cell struct, we print it's number attribute
static void print_func_board(void *data)
{
  Cell *cell = (struct Cell *) data;
  // different print for last cell
  if(cell->number == BOARD_SIZE)
  {
    fprintf(stdout, "[%d]", cell->number);
    return;
  }

  // different print if we move with a ladder/snake
  if (cell->ladder_to != EMPTY)
  {
    fprintf(stdout, LADDER_TO, cell->number, cell->ladder_to);
  }
  else if (cell->snake_to != EMPTY)
  {
    fprintf(stdout, SNAKE_TO, cell->number, cell->snake_to);
  }
  else
  {
    fprintf(stdout, "[%d] -> ", cell->number);
  }
}

/// Get 2 numbers from a Cell struct, compare between them
static int comp_func_board(void *data1, void *data2)
{
  Cell *data1_cell = (struct Cell *) data1;
  Cell *data2_cell = (struct Cell *) data2;
  const long data1_int = data1_cell->number;
  const long data2_int = data2_cell->number;
  if (data1_int == data2_int) {
      return 0;
  } else if (data1_int < data2_int) {
      return -1;
  } else {
      return 1;
  }
}

/// gets a ptr to Cell to free
static void free_data_board(void *data_to_free)
{
  // Assuming the generic data type is a pointer to allocated memory
  if (data_to_free != NULL)
  {
      ///TODO: do I need to free anything else?
    Cell *cell_to_free = (struct Cell *) data_to_free;
    free(cell_to_free);
    cell_to_free= NULL;
  }
}

static void *copy_func_board(const void *data) {
  // Assuming the generic data type is a pointer to allocated memory
  if (data == NULL) {
      return NULL;
  }
  // Assuming data points to a null-terminated string
  Cell *new_cell = calloc(1, sizeof(struct Cell));
  if (new_cell == NULL)
  {
    printf("%s", ALLOCATION_ERROR_MASSAGE);
    free(new_cell);
    new_cell = NULL;
    return NULL;
  }
  memcpy(new_cell, (Cell *) data, sizeof (Cell));
  return (void *)new_cell;
}

static bool is_last_board(const void *data) {
    //check if str is the end of a word (finishes with a '.')
    const long number = (const long) data;
    return (number == BOARD_SIZE);
}

static void assign_functions(MarkovChain *markov_chain)
{
    markov_chain->print_func = print_func_board;
    markov_chain->comp_func = comp_func_board;
    markov_chain->copy_func = copy_func_board;
    markov_chain->free_data = free_data_board;
    markov_chain->is_last = is_last_board;
}

/**
 * checks input validity (needs to get 3 or 4 params from user)
 * also casts char inputs into int. Returns fail
 */
static int check_and_cast_input(int argc, char *argv[], int *seed,
                         int *num_of_paths)
{
    if (argc != ARG_NUM_3)
    {
        fprintf(stdout, NOT_ENOUGH_PARAM);
        return 1;
    }
    *seed = strtol (argv[SEED], NULL, TEN_BASE);
    *num_of_paths = strtol (argv[PATH_NUM], NULL, TEN_BASE);
    return 0;
}

/** Error handler **/
static int handle_error(char *error_msg, MarkovChain **database)
{
    printf("%s", error_msg);
    if (database != NULL)
    {
        free_markov_chain(database);
    }
    return EXIT_FAILURE;
}


static int create_board(Cell *cells[BOARD_SIZE])
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        cells[i] = malloc(sizeof(Cell));
        if (cells[i] == NULL)
        {
            for (int j = 0; j < i; j++) {
                free(cells[j]);
            }
            handle_error(ALLOCATION_ERROR_MASSAGE,NULL);
            return EXIT_FAILURE;
        }
        *(cells[i]) = (Cell) {i + 1, EMPTY, EMPTY};
    }

    for (int i = 0; i < NUM_OF_TRANSITIONS; i++)
    {
        int from = transitions[i][0];
        int to = transitions[i][1];
        if (from < to)
        {
            cells[from - 1]->ladder_to = to;
        }
        else
        {
            cells[from - 1]->snake_to = to;
        }
    }
    return EXIT_SUCCESS;
}

/**
 * fills database
 * @param markov_chain
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
static int fill_database(MarkovChain *markov_chain)
{
    Cell* cells[BOARD_SIZE];
    if(create_board(cells) == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }
    MarkovNode *from_node = NULL, *to_node = NULL;
    size_t index_to;
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        add_to_database(markov_chain, cells[i]);
    }

    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        from_node = get_node_from_database(markov_chain,cells[i])->data;

        if (cells[i]->snake_to != EMPTY || cells[i]->ladder_to != EMPTY)
        {
            index_to = MAX(cells[i]->snake_to,cells[i]->ladder_to) - 1;
            to_node = get_node_from_database(markov_chain, cells[index_to])
                    ->data;
            add_node_to_counter_list(from_node, to_node, markov_chain);
        }
        else
        {
            for (int j = 1; j <= DICE_MAX; j++)
            {
                index_to = ((Cell*) (from_node->data))->number + j - 1;
                if (index_to >= BOARD_SIZE)
                {
                    break;
                }
                to_node = get_node_from_database(markov_chain, cells[index_to])
                        ->data;
                add_node_to_counter_list(from_node, to_node, markov_chain);
            }
        }
    }
    // free temp arr
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        free(cells[i]);
    }
    return EXIT_SUCCESS;
}

static void create_path(MarkovChain *markov_chain)
{
  MarkovNode *first_node_in_tweet = markov_chain->database->first->data;
  generate_random_sequence(markov_chain,
                           first_node_in_tweet,
                           MAX_GENERATION_LENGTH);
}

/**
 * @param argc num of arguments
 * @param argv 1) Seed
 *             2) Number of sentences to generate
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int main(int argc, char *argv[])
{
// validate input and cast into int
  int seed, num_of_paths;
  if (check_and_cast_input(argc, argv, &seed, &num_of_paths))
  {
      return EXIT_FAILURE;
  }
  // validate file path
  srand((unsigned) seed); // initialize seed

  MarkovChain *markov_chain = calloc (1, sizeof (MarkovChain));
  markov_chain->database = calloc (1, sizeof (LinkedList));

  if(markov_chain == NULL || markov_chain->database == NULL)
  {
    printf("%s", ALLOCATION_ERROR_MASSAGE);
    return EXIT_FAILURE;
  }

  assign_functions(markov_chain); // assign generic funcs to str-compatible

  if(fill_database(markov_chain))
  {
      free_markov_chain (&markov_chain);
      return EXIT_FAILURE;
  }

  //   create tweets and print them
  int i;
  for (i = 1; i <= num_of_paths; ++i)
  {
      printf(WALK_NUMBER, i);
      create_path(markov_chain);
      printf ("\n");
  }
  free_markov_chain (&markov_chain);

  return EXIT_SUCCESS;
}
