#include <string.h> // allows strcmp()
#include <stdio.h>
#include <stdlib.h>
//#include <ctype.h>
#include "tests.h"
//#include "tests.c"
#include "cipher.h"
#include <limits.h>

#define INT_BOTTOM_ASCII '0'
#define INT_TOP_ASCII '9'
#define MINUS_SGN '-'

#define TEST_ARG_NUM 2
#define RUN_ARG_NUM 5

#define MAX_LINE 1025
#define CMD 1
#define K_SIZE 2
#define SRC_FILE 3
#define DEST_FILE 4
#define BASE_10 10

/// check encode
int run_tests ()
{
    if (test_encode_non_cyclic_lower_case_positive_k() != 0)
    {
        return EXIT_FAILURE;
    }
    else if (test_encode_cyclic_lower_case_special_char_positive_k() != 0)
    {
        return EXIT_FAILURE;
    }
    else if (test_encode_non_cyclic_lower_case_special_char_negative_k() != 0)
    {
        return EXIT_FAILURE;
    }
    else if (test_encode_cyclic_lower_case_negative_k() != 0)
    {
        return EXIT_FAILURE;
    }
    else if (test_encode_cyclic_upper_case_positive_k() != 0)
    {
        return EXIT_FAILURE;
    }
    // check decode
    else if (test_decode_non_cyclic_lower_case_positive_k() != 0)
    {
        return EXIT_FAILURE;
    }
    else if (test_decode_cyclic_lower_case_special_char_positive_k() != 0)
    {
        return EXIT_FAILURE;
    }
    else if (test_decode_non_cyclic_lower_case_special_char_negative_k() != 0)
    {
        return EXIT_FAILURE;
    }
    else if (test_decode_cyclic_lower_case_negative_k() != 0)
    {
        return EXIT_FAILURE;
    }
    else if (test_decode_cyclic_upper_case_positive_k() != 0 )
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

/// check if k is valid (an integer, no signs or letters)
int iskokay(const char *k)
{
    for (int i = 0; k[i] != '\0'; i++) {
      if (i==0) {
        if (k[i] == MINUS_SGN){
            continue;
        }
      }
      if ((k[i] < INT_BOTTOM_ASCII) || (k[i] > INT_TOP_ASCII))
      {// checks if ASCII value between 0 to 9
        return 1;
      }
    }
    return 0;
}

int apply_cipher (FILE *f, FILE *out_f, char *argv[]){
    char data[MAX_LINE] = {};
//    fgets(data, sizeof(data), f);
    if (strcmp("encode", argv[CMD]) == 0){
        while (fgets(data, sizeof(data), f) != NULL){
//            data[strcspn(data, "\n")] = '\0';
            encode(data, strtol(argv[K_SIZE], NULL, BASE_10));
            fprintf(out_f, data);
        }
    }
    else { // CMD is to decode
        while (fgets(data, sizeof(data), f) != NULL){
//            data[strcspn(data, "\n")] = '\0';
            decode(data, strtol(argv[K_SIZE], NULL, BASE_10));
            fprintf(out_f, data);
        }
    }
//    fprintf(out_f, data);
    fclose(f);
    fclose(out_f);
    return EXIT_SUCCESS;
}

int enough_args(char *argv[])
{
    // checks if cmd is legal (encode/decode)
    if ((strcmp("encode", argv[CMD]) != 0) \
      && (strcmp("decode", argv[CMD]) != 0))
    {
      fprintf(stderr, "The given command is invalid.\n");
      return EXIT_FAILURE;
    }
    // checks if k, num of steps, is a whole number
    if (iskokay(argv[K_SIZE]) != 0)
    {
      fprintf(stderr, "The given shift value is invalid.\n");
      return EXIT_FAILURE;
    }
    // tries to open the input and output files
    FILE *f = fopen (argv[SRC_FILE], "r");
    FILE* out_f = fopen(argv[DEST_FILE], "w");
    if (f == NULL)
    {
      fprintf(stderr, "The given file is invalid.\n");
      fclose(out_f);
      return EXIT_FAILURE;
    }
    if (out_f == NULL)
    {
      fprintf(stderr, "The given file is invalid.\n");
      fclose(f);
      return EXIT_FAILURE;
    }
    // all params are ok, moves on to en/decode the file
    else
    {
        return apply_cipher(f, out_f, argv);
    }
}


int main (int argc, char *argv[])
{
    // if an illegal num of arg were passed
    if (argc != TEST_ARG_NUM && argc != RUN_ARG_NUM)
    {
        fprintf(stderr, "The program receives 1 or 4 arguments only.\n");
        return EXIT_FAILURE;
    }
    // tests the program
    else if (argc == TEST_ARG_NUM)
    {
        if (strcmp(argv[CMD], "test") != 0)
        {
            fprintf(stderr, "Usage: cipher test\n");
            return EXIT_FAILURE;
        }
        return run_tests();
    }
    else
    {
      return enough_args (argv);
    }
}


//            char line[MAX_LINE] = {0};
//            while(fgets (line, MAX_LINE, f)){
//                printf ("%s", line);
//            }
//        str word = f;
//        encode(f)
        // send to cipher
//    char *runner = argv[1];
//    while (&runner>='0'){
//
//    }
//    printf("%s", argv[1]);
