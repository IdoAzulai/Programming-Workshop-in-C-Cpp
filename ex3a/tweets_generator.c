#include "markov_chain.h"
#include <stdio.h>  // For printf(), sscanf()
#include <stdlib.h> // For exit(), malloc()
#include <stdbool.h> // for bool
#include <string.h> // for strcpy

#define ARG_NUM_3 4
#define ARG_NUM_4 5

#define SEED 1
#define TWEET_NUM 2
#define SRC_FILE 3
#define READ_NUM 4
#define TEN_BASE 10

#define ROW_LIM 1000
#define WORD_MAX_LEN 100

#define WORDS_MAX_NUM 1000000000
#define TWEET_MAX_LEN 20

/**
 * checks input validity (needs to get 3 or 4 params from user)
 * also casts char inputs into int. Returns fail
 */
int check_and_cast_input(int argc, char *argv[], int *seed,
                         int *tweet_num, int *words_to_read)
{
  if (argc != ARG_NUM_3 && argc != ARG_NUM_4)
  {
    fprintf(stdout, "Usage: Not enough params. should be 3 or 4.\n");
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
int try_to_open_file(FILE *f)
{
  if (!f)
  {
    fprintf (stdout, "Error: failed to open Text Corpus file.\n");
    fclose (f);
    return 1;
  }
  return 0;
}


/// TODO: for some reason after I return from add_to_database the
/// freq_list is a NULL pointer, and not a (NULL) struct anymore.
int add_words_to_db (char *text_corpus, int *words_cnt,
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
            // if words end with '.' don't add word2 to its freq list
            if (word2->data[strlen(word2->data) - 1] != '.')
            {
                add_node_to_counter_list(word2, word1);
            }

        }
        word2 = word1;

      }
      token = strtok(NULL, " \n"); // Move to the next token
    }
    return 1; // Successfully processed words
}


int fill_database (FILE *fp, int words_to_read, MarkovChain *markov_chain)
{
  char *text_corpus = malloc (ROW_LIM);
  int *words_cnt = calloc (1, sizeof (int));

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

void create_tweet(MarkovChain *markov_chain)
{
  MarkovNode *first_node_in_tweet = get_first_random_node (markov_chain);
  /// TODO: what is the max_length for?
  generate_random_sequence(markov_chain, first_node_in_tweet, TWEET_MAX_LEN);
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
    return EXIT_FAILURE;
  }

  srand((unsigned) seed); // initialize seed

  MarkovChain *markov_chain = calloc (1, sizeof (MarkovChain));
  markov_chain->database = calloc (1, sizeof (LinkedList));
  
  if (argc == ARG_NUM_3)
  {
    words_to_read = WORDS_MAX_NUM;
  }
  if(fill_database(f, words_to_read, markov_chain))
  {
    free_markov_chain (&markov_chain);
    fclose(f);
    return EXIT_FAILURE;
  }

//   create tweets and print them
  int i;
  for (i = 1; i <= tweet_num; ++i)
  {
    printf("Tweet %d: ", i);
    create_tweet(markov_chain);
    printf ("\n");
  }
  free_markov_chain (&markov_chain);
  fclose(f);
}