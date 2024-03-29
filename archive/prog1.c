#include <stdio.h>

#define ARRAY_SIZE 8192  
#define JUMP_SIZE 128     
#define ITERS (10 * 1024 * 1024 / (ARRAY_SIZE / JUMP_SIZE))  

int global_array[ARRAY_SIZE] __attribute__((aligned(64))); // Aligned array for better cache use

int main() {
    
    // Initialize array elements with a pattern based on JUMP_SIZE
    for (int i = 0; i < ARRAY_SIZE; i += JUMP_SIZE) {
        global_array[i] = (i + JUMP_SIZE) % ARRAY_SIZE;
    }

    int index = 0;
    
    for (int iter = 0; iter < ITERS; ++iter) {
        for (int i = 0; i < ARRAY_SIZE; i += JUMP_SIZE) {
            index = global_array[index];
        }
    }

    printf("Last accessed index: %d\n", index); 
    return 0;
}