#define _POSIX_C_SOURCE 200112L  // for posix_memalign
#include <stdlib.h>
#include "mlpt.h"
#include "config.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>  // For SIZE_MAX

#define PAGE_SIZE (1 << POBITS)  // Pages occupy 2^POBITS bytes
#define ENTRY_COUNT (PAGE_SIZE / sizeof(size_t))  // Each page table contains PAGE_SIZE/8 entries

size_t ptbr = 0;  // Page Table Base Register

void* allocate_aligned_page() {
    void *page = NULL;
    if (posix_memalign(&page, PAGE_SIZE, PAGE_SIZE) != 0) {
        perror("posix_memalign failed");
        return NULL;
    }
    memset(page, 0, PAGE_SIZE);  // Initialize page to zeros
    return page;
}

size_t translate_recursive(size_t va, size_t table, int level) {
    if (table == 0) {
        return SIZE_MAX; // Table not allocated
    }

    size_t index = (va >> (POBITS + (level - 1) * (12 - LEVELS))) & (ENTRY_COUNT - 1);
    size_t* page_table = (size_t*)table;
    size_t entry = page_table[index]; // Get the page table entry

    if (level == LEVELS) { 
        if (entry & 1) { // Check if entry is valid
            return (entry & ~((1 << POBITS) - 1)) | (va & ((1 << POBITS) - 1)); // Combine PPN with offset
        }
        return SIZE_MAX; // Entry not valid
    } else { 
        if (!(entry & 1)) {
            return SIZE_MAX; // Entry not valid
        }
        return translate_recursive(va, entry & ~((1 << POBITS) - 1), level + 1); // Recurse with the next level table address
    }
}

size_t translate(size_t va) {
    return translate_recursive(va, ptbr, 1); // Start translation from the top-level table
}

void page_allocate(size_t va) {
    size_t* current_table = &ptbr;
    for (int level = 1; level <= LEVELS; level++) {
        if (*current_table == 0) { // If table not allocated, allocate it
            *current_table = ((size_t)allocate_aligned_page() & ~((1 << POBITS) - 1)) | 1; // Allocate and mark as valid
            if (*current_table == 0) return;  // Allocation failed
        }

        size_t index = (va >> (POBITS + (LEVELS - level) * (12 - LEVELS))) & (ENTRY_COUNT - 1);
        size_t* page_table = (size_t*)(*current_table & ~((1 << POBITS) - 1));
        size_t entry = page_table[index];

        if (level == LEVELS) { 
            if (!(entry & 1)) { // Check if entry is not valid
                void* page = allocate_aligned_page(); // Allocate a new data page
                if (page == NULL) return; // Allocation failed
                page_table[index] = ((size_t)page & ~((1 << POBITS) - 1)) | 1; // Mark entry as valid
            }
        } else { 
            if (!(entry & 1)) { // If entry not valid, allocate new table
                void* new_table = allocate_aligned_page();
                if (new_table == NULL) return; // Allocation failed
                page_table[index] = ((size_t)new_table & ~((1 << POBITS) - 1)) | 1; // Mark entry as valid
            }
            current_table = (size_t*)(page_table[index] & ~((1 << POBITS) - 1)); // Move to next level table
        }
    }
}
