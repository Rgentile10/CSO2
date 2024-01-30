#include "split.h"
#include <stdlib.h>
#include <string.h>

char **string_split(const char *input, const char *sep, int *num_words) {
    
    // avoid segmentation faults, set to initial size and reallocate later
    int total_length = 20;

    // return value
    char **all_words = malloc(total_length * sizeof(char *));

    *num_words = 0;

    // Check of the beginning of the input string starts with a delimiter
    if (*input && strchr(sep, *input)) {
        all_words[(*num_words)++] = strdup("");
    }

// Let's go through the entire input and search for delimiters from the sep
    while (*input) {
        input += strspn(input, sep); 

        //leave loop if end of input
        if (!*input) break;

        int word_len = strcspn(input, sep); // Length of the next word
        if (*num_words >= total_length) {
            total_length *= 2;
            all_words = realloc(all_words, total_length * sizeof(char *));
        }

        all_words[*num_words] = strndup(input, word_len);
        (*num_words)++;

        input += word_len;

    }

    // Check beginning of input string and look for delimiters

    if (*num_words > 0 && strchr(sep, input[-1])) {
        
        if (*num_words >= total_length) {
            total_length += 1;
            all_words = realloc(all_words, total_length * sizeof(char *));
        }
        all_words[(*num_words)++] = strdup("");
    }

// Free the array
    if (*num_words > 0) {
        all_words = realloc(all_words, *num_words * sizeof(char *));
    } 
    
    
    else {
        // no words were added, so free n set 2 null
        free(all_words);
        all_words = NULL;
    }

    return all_words;
    }
