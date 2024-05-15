#include "markov_chain.h"
#include <stdio.h>  // For printf(), sscanf()
#include <stdlib.h> // For exit(), malloc()
#include <stdbool.h> // for bool
#include <string.h> // for strcpy

#define OPEN_FILE_ERROR "Error: Failed to open path in file.\n"
//"Error: failed to open Text Corpus file.\n"
#define TWEET_SENTENCE_NUM "Tweet %d: "

#define ARG_NUM_3 4
#define ARG_NUM_4 5

#define SEED 1
#define TWEET_NUM 2
#define SRC_FILE 3
#define READ_NUM 4
#define TEN_BASE 10
#define DOT '.'

#define ROW_LIM 1000
#define WORD_MAX_LEN 100

#define WORDS_MAX_NUM 1000000000
#define TWEET_MAX_LEN 20

#define PARAM_ERROR "Usage: Not enough params. should be 3 or 4.\n"

char *my_strdup(const char* str)
{
  char *dst = malloc (strlen (str)+1);
  if (dst)
  {
    strcpy (dst,str);
  }
  return dst;
}

static bool is_last_str(const void *data) {
  //check if str is the end of a word (finishes with a '.')
  char *word = (char *) data;
  if(strlen(word) > 0)
  {
    return (word[strlen (word) - 1] == DOT);
  }
  return false;
//  return (word[strlen (word) - 1] == '.');
}

static void print_func_str(void *data)
{
  const char *text = (const char*) data;
  if (is_last_str(data))
  {
    fprintf(stdout, "%s", text);
    return;
  }
  fprintf(stdout, "%s ", text);
}

static int comp_func_str(void *data1, void *data2)
{
  const char *data1_char = (const char*) data1;
  const char *data2_char = (const char*) data2;
  return strcmp(data1_char, data2_char);
}

static void free_data_str(void *data_to_free)
{
  // Assuming the generic data type is a pointer to allocated memory
  if (data_to_free != NULL)
  {
      free(data_to_free);
  }
}

static void *copy_func_str(const void *data) {
  // Assuming the generic data type is a pointer to allocated memory
  if (data == NULL) {
      return NULL;
  }

  // Assuming data points to a null-terminated string
  // strdup creates the memory needed, so it needs to be freed at the end
  const char *copy = my_strdup((const char *)data);
  return (void *)copy;
}



/**
 * checks input validity (needs to get 3 or 4 params from user)
 * also casts char inputs into int. Returns fail
 */
static int check_and_cast_input(int argc, char *argv[], int *seed,
                         int *tweet_num, int *words_to_read)
{
  if (argc != ARG_NUM_3 && argc != ARG_NUM_4)
  {
    fprintf(stdout, PARAM_ERROR);
    return 1;
  }
  *seed = strtol (argv[SEED], NULL, TEN_BASE);
  *tweet_num = strtol (argv[TWEET_NUM], NULL, TEN_BASE);

  // Program supports 3\4 params, read all text\ specific number, accordingly
  if (argc == ARG_NUM_4)
  {
    *words_to_read = strtol (argv[READ_NUM], NULL, TEN_BASE);
  }
  return 0;
}

/**
 * Tries to open file input path. if fails, returns false.
 */
static int try_to_open_file(FILE *f)
{
  if (!f)
  {
    fprintf (stdout, OPEN_FILE_ERROR);
    fclose (f);
    return 1;
  }
  return 0;
}

/**
 * Gets a line from file and adds all distinct words to chain
 * @param text_corpus a single line from
 * @param words_cnt
 * @param markov_chain
 * @param words_to_read
 * @return
 */
static int add_words_to_db (char *text_corpus, int *words_cnt,
                     MarkovChain *markov_chain, int words_to_read)
{
    char *token = strtok(text_corpus, " \n"); // Tokenize the input string
    MarkovNode *word1 = NULL, *word2 = NULL;
    while (token != NULL && (*words_cnt < words_to_read) ) {
      if (strlen(token) > 0) { // Skip empty tokens
        Node *temp = add_to_database(markov_chain, token);
        if (temp == NULL) {
          return 0; // Error occurred while adding word to the database
        }
        *words_cnt += 1;

        // adding a word2 to word1's freq list
        word1 = temp->data;
        if (word2 != NULL)
        {
                if (!(markov_chain->is_last(word2)))
                {
                    add_node_to_counter_list(word2, word1, markov_chain);
                }

        }
        word2 = word1;
      }
      token = strtok(NULL, " \n"); // Move to the next token
    }
    return 1; // Successfully processed words
}


static int fill_database (FILE *fp,
                          int words_to_read,
                          MarkovChain *markov_chain)
{
  char *text_corpus = malloc (ROW_LIM);
  int *words_cnt = calloc (1, sizeof (int));
  if (text_corpus == NULL || words_cnt == NULL)
  {
    printf("%s", ALLOCATION_ERROR_MASSAGE);
    return 1;
  }

  //convert text (up to words_to_read) from file into a string that holds it
  while (fgets (text_corpus, ROW_LIM, fp) &&
         (*words_cnt < words_to_read))
  {
    // go through all str and every time I have a word
    // (including its 'wrappers'), use add_to_db
    int inserted_correctly = add_words_to_db (text_corpus,
                                              words_cnt,
                                              markov_chain,
                                              words_to_read);
    if (!inserted_correctly)
    {
      free (words_cnt);
      return 1;
    }
  }
  free (words_cnt);
  free(text_corpus);
  return 0;
}

static void create_tweet(MarkovChain *markov_chain)
{
  MarkovNode *first_node_in_tweet = get_first_random_node (markov_chain);
  generate_random_sequence(markov_chain, first_node_in_tweet, TWEET_MAX_LEN);
}

static void assign_functions(MarkovChain *markov_chain)
{
    markov_chain->print_func = print_func_str;
    markov_chain->comp_func = comp_func_str;
    markov_chain->copy_func = copy_func_str;
    markov_chain->free_data = free_data_str;
    markov_chain->is_last = is_last_str;
}

int main (int argc, char *argv[])
{
  // validate input and cast into int
  int seed, tweet_num, words_to_read;
  if (check_and_cast_input(argc, argv, &seed, &tweet_num, &words_to_read))
  {
    return EXIT_FAILURE;
  }
  // validate file path
  FILE *f = fopen (argv[SRC_FILE], "r");
  if (try_to_open_file(f))
  {
    printf ("%s", OPEN_FILE_ERROR);
    return EXIT_FAILURE;
  }
  srand((unsigned) seed); // initialize seed
  MarkovChain *markov_chain = calloc (1, sizeof (MarkovChain));
  markov_chain->database = calloc (1, sizeof (LinkedList));
  if(markov_chain == NULL || markov_chain->database == NULL)
  {
    printf("%s", ALLOCATION_ERROR_MASSAGE);
    return EXIT_FAILURE;
  }

  if (argc == ARG_NUM_3)
  {
    words_to_read = WORDS_MAX_NUM;
  }

  assign_functions(markov_chain); // assign generic funcs to str-compatible

  if(fill_database(f, words_to_read, markov_chain))
  {
    free_markov_chain (&markov_chain);
    fclose(f);
    return EXIT_FAILURE;
  }

//   create tweets and print them
  for (int i = 1; i <= tweet_num; ++i)
  {
    printf(TWEET_SENTENCE_NUM, i);
    create_tweet(markov_chain);
    printf ("\n");
  }
  free_markov_chain (&markov_chain);
  fclose(f);

  return EXIT_SUCCESS;
}