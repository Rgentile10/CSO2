#include <stdio.h>

static int arr[16384] __attribute__((aligned(128)));

inline void no_opt() { __asm__ volatile("" ::: "memory"); }

// Populate array with a pattern, offsetting element by 'skip'
void populate(int *a, int len, int skip) {
    for (int i = 0; i < len; ++i) a[i] = (i + skip) % len;
}

// linked list traversal
int iterate(int *a, int count) {
    int index = 0;
    while (count--) index = a[index];
    return index;
}

int main() {
    int size = sizeof(arr) / sizeof(arr[0]);
    populate(arr, size, size / 4);
    no_opt(); // make sure memory ops are preserved
    printf("%d\n", iterate(arr, 10000000));
}
