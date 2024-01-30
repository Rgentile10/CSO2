#include "split.h"
#include <stdio.h>

// constructs a value for sep by the command line arguments, or, if no command line arguments are provided, 
// uses " \t". When there are command-line arguments, the value of sep should be the result of concatenating 
// all the command line arguments.

int main() {
    
    int size;
    //string_split("Hello", "H", &size);


    char *input = (char*) malloc(4000*(sizeof(char)));
    
    
    strcpy(input, "Hello");

    char *sep = "1";

    int *num_words;

    test1(input, sep, num_words);

    printf("num of words: %d", num_words);
    
    return 0;
}