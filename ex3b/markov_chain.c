#include "markov_chain.h"
#include <stdio.h>  // For printf(), sscanf()
#include <stdlib.h> // For exit(), malloc()
#include <stdbool.h> // for bool
#include <string.h> // for strcpy


MarkovNode *create_new_node(MarkovChain *markov_chain, const void *data_ptr)
{
  // Copy the data string
  MarkovNode *new_node = calloc(1, sizeof(MarkovNode));
  new_node->data = markov_chain->copy_func(data_ptr);

  // Initialize other members
  new_node->frequencies_list = NULL; // Initially, no frequencies
  new_node->freq_array_size = 0;

  return new_node;
}


/// adds a distinct data to the chain, return pointer to data
Node* add_to_database(MarkovChain *markov_chain, void *data_ptr)
{
  // if markov_chain input is null, create a node and connect it
  if (markov_chain->database->first == NULL)
  {
    // creating the structs and connecting the new node
    MarkovNode *new_node = create_new_node (markov_chain, data_ptr);
    if (add (markov_chain->database, new_node))
    {
      return NULL;
    }
    return markov_chain->database->first;
  }

  // chain is not null, check if data already appears. return its node if so
  Node *get_node = get_node_from_database(markov_chain, data_ptr);
  if (get_node)
  {
    return get_node;
  }

  // data is not in chain, add it to the end of chain
  MarkovNode *get_node1 = create_new_node (markov_chain, data_ptr);
  if (!add(markov_chain->database, get_node1))
  {
    return markov_chain->database->last;
  }
  return NULL;
}

/// searches for the string in chain, return it's Node pointer
Node* get_node_from_database(MarkovChain *markov_chain, void *data_ptr)
{
  // initializes the runner to be the first node from the list
  Node *list_ptr = markov_chain->database->first;
  while ((list_ptr != NULL) && (list_ptr->data != NULL))
  {
    if (!(list_ptr->data->data))
    {
        break; // Skip nodes with uninitialized data
    }
    if(markov_chain->comp_func(data_ptr, list_ptr->data->data) == 0)
    {
      return list_ptr;
    }
    list_ptr = list_ptr->next;
  }
  return NULL;
}

bool add_node_to_counter_list(MarkovNode *first_node, MarkovNode *second_node,
                              MarkovChain *markov_chain)
{
  // Check if the first node is NULL
  if (first_node == NULL || markov_chain == NULL)
  {
      return false;
  }
  // If the frequencies list is NULL, allocate memory for it
  if (first_node->frequencies_list == NULL)
  {
      first_node->frequencies_list = malloc(sizeof(NextNodeCounter *));
      if (first_node->frequencies_list == NULL)
      {
          return false; // Memory allocation error
      }
      first_node->freq_array_size = 0;
  }
  // Check if the second node is already in the frequencies list
  for (int i = 0; i < first_node->freq_array_size; ++i){
      if (first_node->frequencies_list[i]->markov_node == second_node)
      {
          // If found, increment the frequency and return
          first_node->frequencies_list[i]->frequency++;
          return true;
      }
  }
  // If the second node is not found, add it to the list
  NextNodeCounter *new_frequency = malloc(sizeof(NextNodeCounter));
  if (new_frequency == NULL)
  {
      return false; // Memory allocation error
  }
  new_frequency->markov_node = second_node;
  new_frequency->frequency = 1;
  // Resize the frequencies list array
  NextNodeCounter **bigger_array = realloc(first_node->frequencies_list,
                                               sizeof(NextNodeCounter *)
                                   * (first_node->freq_array_size + 1));
  if (bigger_array == NULL)
  {
      free(new_frequency); // Free allocated memory for the new frequency
      return false; // Memory reallocation error
  }
  // Update the frequencies list and size
  first_node->frequencies_list = bigger_array;
  first_node->frequencies_list[first_node->freq_array_size] = new_frequency;
  first_node->freq_array_size++;
  return true;
}


void free_markov_chain(MarkovChain **markov_chain)
{
    if (markov_chain == NULL || *markov_chain == NULL)
    {
        return; // Nothing to free if the chain is NULL
    }

    if ((*markov_chain)->database != NULL)
    {
        Node *current_node = (*markov_chain)->database->first;
        while (current_node != NULL)
        {
            MarkovNode *current_markov_node = current_node->data;
            if (current_markov_node != NULL)
            {
                // Free the data associated with the current node
                (*markov_chain)->free_data(current_markov_node->data);

                // Free the frequency list array of the current node
                if (current_markov_node->frequencies_list != NULL)
                {
                    for (int i = 0;
                         i < current_markov_node->freq_array_size;
                         i++)
                    {
                        free(current_markov_node->frequencies_list[i]);
                    }
                    free(current_markov_node->frequencies_list);
                }
                free(current_markov_node);

            }
            // Move to the next node and free the current node
            Node *next_node = current_node->next;
            free(current_node);
            current_node = next_node;
        }

        // Free the linked list structure
        free((*markov_chain)->database);
    }

    // Finally, free the MarkovChain structure itself
    free(*markov_chain);

    // Set the pointer to NULL after freeing memory
    *markov_chain = NULL;
}

/**
 *  Get random number between 0 and max_number [0, max_number).
 *  @param max_number maximal number to return (not including)
 *  @return Random number
 */
 int get_random_number(int max_number)
{
  if (max_number == 0)
  {
      return 0;
  }
  return rand() % max_number;
}

/**
 * Gets a random number and finds it in the words list. Checks if word is
 * at the end of a sentence.
 * @param markov_chain pointer to the markov chain that holds the words
 * @param rand_num a number between 0 and the number of words in markov_chain
 * @param chosen_node pointer to node that holds the word at the rand num ind.
 * @return True if the word at rand number index is not at the end of the
 * sentence, False otherwise.
 */
int get_rand_node(MarkovChain *markov_chain, int rand_num, Node **chosen_node)
{
  Node *node_runner = markov_chain->database->first;
  for (int i = 0; i < rand_num; ++i)
  {
    node_runner = node_runner->next;
  }
  if (markov_chain->is_last(node_runner->data->data))
  {
    return 1;
  }
  *chosen_node = node_runner;
  return 0;
}

/**
 * Gets the first word for the tweet creator.
 * If a word is at the end of a sentence, it's not valid so continue trying
 * @param markov_chain pointer to the markov chain that holds the words
 * @return pointer to the MarkovNode at the random index
 */
MarkovNode* get_first_random_node(MarkovChain *markov_chain)
{
  int rand_num = get_random_number (markov_chain->database->size);
  Node *chosen_node = NULL;
  while (get_rand_node(markov_chain, rand_num, &chosen_node))
  {
    rand_num = get_random_number (markov_chain->database->size);
    chosen_node = NULL;
  }
  return chosen_node->data;
}

/**
 * Gets a random number (from 0 to number of words in the
 * MarkovNodeFrequency array), and advances the runner to that place.
 * Words that appear more than once after the original word, will get more
 * weight, means more chances to get picked from list.
 * @param state_struct_ptr pointer to first word, that
 * holds the MarkovNodeFrequency array we will iterate through
 * @return pointer to a MarkovNode of the next random word
 */
MarkovNode* get_next_random_node(MarkovNode *state_struct_ptr)
{
  if (state_struct_ptr == NULL ||
      state_struct_ptr->frequencies_list == NULL)
  {
      return NULL; // Invalid input
  }

  int total_frequency_sum = 0;
  for (int i = 0; i < state_struct_ptr->freq_array_size; i++)
  {
      total_frequency_sum +=
              state_struct_ptr->frequencies_list[i]->frequency;
  }

  if (total_frequency_sum == 0) {
      return NULL; // No valid next states
  }

  int rand_num = get_random_number(total_frequency_sum);
  int cumulative_frequency = 0;

  for (int i = 0; i < state_struct_ptr->freq_array_size; i++)
  {
      cumulative_frequency +=
              state_struct_ptr->frequencies_list[i]->frequency;
      if (rand_num < cumulative_frequency) {
          return state_struct_ptr->frequencies_list[i]->markov_node;
      }
  }

  // This point should never be reached,
  // but return NULL in case of unexpected behavior
  return NULL;
}


void generate_random_sequence(MarkovChain *markov_chain,
                              MarkovNode *first_node,
                              int max_length)
{
  if (first_node == NULL)
  {
    first_node = get_first_random_node(markov_chain);
  }
  MarkovNode *next_node;
  int num_of_data_nodes = 0;

  // add first word to tweet
  markov_chain->print_func(first_node->data);
  num_of_data_nodes++;

  while (num_of_data_nodes < max_length)
  {
    // if word ends with "." means it's the end of a sentence, finish tweet
    if (markov_chain->is_last(first_node->data))
    {
      break;
    }

    // get next word from words list related to first_word
    next_node = get_next_random_node(first_node);
    if (!next_node)
    {
      break; /// TODO: what now??
    }
    // concat new rand word to tweet and advance runners of while loop
    markov_chain->print_func(next_node->data);
    num_of_data_nodes++;
    first_node = next_node;
  }
}