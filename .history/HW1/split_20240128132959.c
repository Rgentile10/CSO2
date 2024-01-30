#include <stdio.h>
#include <string.h>


 void string_split(const char *input, const char *sep, int *num_words) {

    // first, lets get how many words are in here so that we can allocate appropriate memory space

    // If the input starts or ends with a separating character, this word will be empty (zero-length). Empty words must not be generated in any other circumstance.
    // lets account for if there is a starting character with a sep value

    // need to set the *num_words value once we find the number of words

    // *sep is the reject set
    
    int num_of_words = strcspn(input, sep);

    printf("Number of words: %d\n", num_of_words);

 }



 
