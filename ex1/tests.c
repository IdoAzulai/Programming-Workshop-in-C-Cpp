#include <string.h> // allows strcmp()
#include "tests.h"
#include "cipher.h"
//#include "cipher.c"

#define K_1 3
#define K_2 2
#define K_3 -1
#define K_4 -3
#define K_5 29

//abcdefghijklmnopqrstuvwxyz

// See full documentation in header file
int test_encode_non_cyclic_lower_case_positive_k ()
{
  char in[] = "abc";
  char out[] = "def";
  encode (in, K_1);
  return strcmp (in, out) != 0;
}

// See full documentation in header file
int test_encode_cyclic_lower_case_special_char_positive_k ()
{
    char in[] = "yza .,-!@#$%^&*()_+={}[]''?><:";
    char out[] = "abc .,-!@#$%^&*()_+={}[]''?><:";
    encode (in, K_2);
    return strcmp (in, out) != 0;
}

// See full documentation in header file
int test_encode_non_cyclic_lower_case_special_char_negative_k ()
{
    char in[] = "b .,-!@#$%^&*()_+={}[]''?><:";
    char out[] = "a .,-!@#$%^&*()_+={}[]''?><:";
    encode (in, K_3);
    return strcmp (in, out) != 0;
}

// See full documentation in header file
int test_encode_cyclic_lower_case_negative_k ()
{
    char in[] = "abcd";
    char out[] = "xyza";
    encode (in, K_4);
    return strcmp (in, out) != 0;
}
// See full documentation in header file
int test_encode_cyclic_upper_case_positive_k ()
{
    char in[] = "ABC";
    char out[] = "DEF";
    encode (in, K_5);
    return strcmp (in, out) != 0;
}

// See full documentation in header file
int test_decode_non_cyclic_lower_case_positive_k ()
{
  char in[] = "def";
  char out[] = "abc";
  decode (in, K_1);
  return strcmp (in, out) != 0;
}

// See full documentation in header file
int test_decode_cyclic_lower_case_special_char_positive_k ()
{
    char in[] = "abc .,-!@#$%^&*()_+={}[]''?><:";
    char out[] = "yza .,-!@#$%^&*()_+={}[]''?><:";
    decode (in, K_2);
    return strcmp (in, out) != 0;
}

// See full documentation in header file
int test_decode_non_cyclic_lower_case_special_char_negative_k ()
{
    char in[] = "a .,-!@#$%^&*()_+={}[]''?><:";
    char out[] = "b .,-!@#$%^&*()_+={}[]''?><:";
    decode (in, K_3);
    return strcmp (in, out) != 0;
}

// See full documentation in header file
int test_decode_cyclic_lower_case_negative_k ()
{
    char in[] = "xyza";
    char out[] = "abcd";
    decode (in, K_4);
    return strcmp (in, out) != 0;
}

// See full documentation in header file
int test_decode_cyclic_upper_case_positive_k ()
{
    char in[] = "DEF";
    char out[] = "ABC";
    decode (in, K_5);
    return strcmp (in, out) != 0;
}