#include <stdio.h>
#include <stdlib.h>
#include "split.h"
#include <string.h>

#define MAX_INPUT_SIZE 4000

// constructs a value for sep by the command line arguments, or, if no command line arguments are provided, 
// uses " \t". When there are command-line arguments, the value of sep should be the result of concatenating 
// all the command line arguments.

// int main() {
    
//     int size;
    //string_split("Hello", "H", &size);


    // char *input = (char*) malloc(4000*(sizeof(char)));

    // char *input = (char*) malloc(4000*(sizeof(char)));

    // char *input = (char*) malloc(4000*(sizeof(char)));
    
    
    // strcpy(input, "Hello");

    // char *sep = "1";

    // int *num_words;

    // test1(input, sep, num_words);

    // printf("num of words: %d", num_words);


    // main.c


int main(int argc, char *argv[]) {


    printf("stest100");

    // char *sep = "\t";

    // if (argc > 1) {
    //     int total_length = 0;
        
    //     for (int i = 1; i < argc; i++) {
    //         total_length += strlen(argv[i]);
    //     }

    //     sep = malloc(total_length + 1);  // +1 for null-terminator
    //     sep[0] = '\0';

    // printf("stest22");
    //     for (int i = 1; i < argc; i++) {
    //         strcat(sep, argv[i]);
    //     }
    // } 
    
    // else {
    //     sep = malloc(3);  // " \t" plus null-terminator
    //     strcpy(sep, "\t");
    // }

    // test if we counted the number of characters, and read in the value of sep correctly
    printf("Value of Sep: %s\n", sep);

    // free allocated memory
    free(sep);

    return 0;
}


    // Now, enter the input 
//     char input[MAX_INPUT_SIZE];
//     while (1) {
//         printf("> ");
//         if (!fgets(input, sizeof(input), stdin) || input[0] == '.' && (input[1] == '\n' || input[1] == '\0')) {
//             break; // Exit if the line is "."
//         }

//         // Remove trailing newline if present
//         size_t input_length = strlen(input);
//         if (input[input_length - 1] == '\n') {
//             input[input_length - 1] = '\0';
//         }

//         // Call string_split and print the result
//         char **result;
//         int size;
//         result = string_split(input, sep, &size);

//         if (result != NULL) {
//             for (int i = 0; i < size; i++) {
//                 printf("[%s]", result[i]);
//             }
//             printf("\n");

//             // Free the result
//             free_memory(result, size);
//         } else {
//             printf("Error in string_split\n");
//         }
//     }

//     // Free dynamically allocated memory for sep
//     if (argc > 1) {
//         free(sep);
//     }

//     return 0;
// }