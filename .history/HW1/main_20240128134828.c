#include "split.c"
#include <stdio.h>

// constructs a value for sep by the command line arguments, or, if no command line arguments are provided, 
// uses " \t". When there are command-line arguments, the value of sep should be the result of concatenating 
// all the command line arguments.

int main() {
    
    int size;
    string_split("Hello", "H", &size);
    
    return 0;
}