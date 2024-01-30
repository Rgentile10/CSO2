#include "split.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    // set this as the default
    char sep[256] = " \t"; 
    if (argc > 1) {

        // string terminator
        sep[0] = '\0';
        for (int i = 1; i < argc; i++) {
            strcat(sep, argv[i]); 
        }
    }

    char input[4001];
    while (fgets(input, sizeof(input), stdin)) {

        // if the user enters a "." then we stop!
        if (input[0] == '.' && input[1] == '\n') break;

    // get rid of newline 
        input[strcspn(input, "\n")] = 0; 

        int num_words;

        // CALL STRING_SPLIT
        char **response = string_split(input, sep, &num_words);

        // Add the brackets
        for (int i = 0; i < num_words; i++) {
            printf("[%s]", response[i]);
            free(response[i]);
        }
        printf("\n");
        free(response);
    }

    return 0;
}
