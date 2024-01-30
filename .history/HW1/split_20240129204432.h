#ifndef SPLIT_H
#define SPLIT_H

 /* char **string_split(const char *input, const char *sep, int *num_words);
input is a sequence of potential words
sep is a list of character delimiters
input should be considered to always start and end with a word. 
If the input starts or ends with a separating character, this word will be empty (zero-length). Empty words must not be generated in any other circumstance.
Overall, the function should take input and break it into a sequence of words
The function should return the words using a dynamically allocated array of \0-terminated strings. (I think of malloc)
It must be possible to free the return array by calling free() on each element of the array and the array itself. The length of the array must be stored in *num_words.
*/
 char **string_split(const char *input, const char *sep, int *num_words);
 
 int* test1(const char *input, const char *sep, int *num_words);

 #endif
 