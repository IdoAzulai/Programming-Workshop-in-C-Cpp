#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "cipher.h"

/// IN THIS FILE, IMPLEMENT EVERY FUNCTION THAT'S DECLARED IN cipher.h.

#define A2Z_LEN 26
#define UPPER_ASCII_A 'A'
#define UPPER_ASCII_Z 'Z'
#define LOWER_ASCII_A 'a'
#define LOWER_ASCII_Z 'z'

char shift_char_in_range(char c, int k, char first_char, char last_char) {
    int range_size = last_char - first_char + 1;

    // Ensure k is within the range [0, range_size)
    k = (k % range_size + range_size) % range_size;

    // Shift the character within the specified range
    if (c >= first_char && c <= last_char) {
        c = (c - first_char + k) % range_size + first_char;
    }

    return c;
}

char unshift_char_in_range(char c, int k, char first_char, char last_char) {
    int range_size = last_char - first_char + 1;

    // Ensure k is within the range [0, range_size)
    k = (k % range_size + range_size) % range_size;

    // Shift the character in reverse within the specified range
    if (c >= first_char && c <= last_char) {
        c = (c - first_char - k + range_size) % range_size + first_char;
    }

    return c;
}



// See full documentation in header file
void encode (char s[], int k)
{
    for (int i=0; s[i] != '\0'; i++)
    {
        if (isupper(s[i]))
        {// adds k and takes care of circularity for Upper (65-90)
            s[i] = shift_char_in_range(s[i], k, UPPER_ASCII_A,UPPER_ASCII_Z);
        }
        else if (islower(s[i]))
        {// adds k and takes care of circularity for Lower (97-122)
            s[i] = shift_char_in_range(s[i], k, LOWER_ASCII_A,LOWER_ASCII_Z);
        }
    }
}

// See full documentation in header file
void decode (char s[], int k)
{
    for (int i=0; s[i] != '\0'; i++)
    {
        if (isupper(s[i]))
        {// subs k and takes care of circularity for Upper (65-90)
           s[i] = unshift_char_in_range(s[i], k,UPPER_ASCII_A,UPPER_ASCII_Z);
        }
        else if (islower(s[i]))
        {// subs k and takes care of circularity for Lower (97-122)
           s[i] = unshift_char_in_range(s[i], k,LOWER_ASCII_A,LOWER_ASCII_Z);
        }
    }
}




