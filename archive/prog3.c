#include <stdio.h>

static int arr[32768] __attribute__((aligned(128))); // improved cache performance  

inline void block_opt() { __asm__ volatile("" ::: "memory"); } // prevent compiler optimizations affecting memory

void setup(int *array, int length) {
    for (int k = 0; k < length; k++) { 
        *(array + k) = (k + 16) & (length - 1); // Set each element for circular access within the array bounds
    }
}

//workload
int cycle(int *data, int reps) {
    int idx = 0;
    while (reps--) idx = *(data + idx);
    return idx;
}

int main() {
    setup(arr, sizeof(arr) / sizeof(arr[0]));
    block_opt(); // make sure memory operations are not optimized out
    printf("%d\n", cycle(arr, 10000000));
}

