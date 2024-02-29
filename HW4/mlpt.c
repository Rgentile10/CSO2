#define _POSIX_C_SOURCE 200112L  // for posix_memalign
#include <stdlib.h>
#include "mlpt.h"
#include "config.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>  // For SIZE_MAX

#define PAGE_SIZE 4096  // 2^POBITS, assuming 4KB pages
#define ENTRY_COUNT (PAGE_SIZE / sizeof(size_t))  // Number of entries per page table

size_t ptbr = 0;  // Page Table Base Register

// Helper function to allocate a new aligned page and initialize it to zero
void* allocate_aligned_page() {
    void *page = NULL;
    if (posix_memalign(&page, PAGE_SIZE, PAGE_SIZE) != 0) {
        perror("posix_memalign failed");
        return NULL;
    }
    memset(page, 0, PAGE_SIZE);  // Initialize page to zeros
    return page;
}

// Recursive function to translate a virtual address to a physical address
size_t translate_recursive(size_t va, size_t table, int level) {
    if (table == 0) {
        return SIZE_MAX; // Table not allocated
    }

    size_t index = (va >> (POBITS + (level - 1) * (12 - LEVELS))) & (ENTRY_COUNT - 1);
    size_t* page_table = (size_t*)table;
    size_t entry = page_table[index]; // Get the page table entry

    if (level == LEVELS) { // At the final level, return physical address
        if (entry & 1) { // Check if entry is valid
            return (entry & ~0xFFF) | (va & 0xFFF); // Combine with offset
        }
        return SIZE_MAX; // Entry not valid
    } else { // Intermediate level, recurse into the next level page table
        if (!(entry & 1)) {
            return SIZE_MAX; // Entry not valid
        }
        return translate_recursive(va, entry & ~0xFFF, level + 1); // Recurse with the next level table address
    }
}

size_t translate(size_t va) {
    return translate_recursive(va, ptbr, 1); // Start translation from the top-level table
}

// Function to allocate pages/tables as necessary for a given virtual address
void page_allocate(size_t va) {
    size_t* current_table = &ptbr;
    for (int level = 1; level <= LEVELS; level++) {
        if (*current_table == 0) { // If table not allocated, allocate it
            *current_table = (size_t)allocate_aligned_page();
            if (*current_table == 0) return;  // Allocation failed
        }

        size_t index = (va >> (POBITS + (LEVELS - level) * (12 - LEVELS))) & (ENTRY_COUNT - 1);
        size_t* page_table = (size_t*)*current_table;
        size_t entry = page_table[index];

        if (level == LEVELS) { // At the final level, allocate a data page if not already present
            if (!(entry & 1)) { // Check if entry is not valid
                void* page = allocate_aligned_page(); // Allocate a new data page
                if (page == NULL) return; // Allocation failed
                page_table[index] = ((size_t)page & ~0xFFF) | 1; // Mark entry as valid
            }
        } else { // Intermediate level, set up for next level table
            if (!(entry & 1)) { // If entry not valid, allocate new table
                void* new_table = allocate_aligned_page();
                if (new_table == NULL) return; // Allocation failed
                page_table[index] = ((size_t)new_table & ~0xFFF) | 1; // Mark entry as valid
            }
            current_table = (size_t*)(page_table[index] & ~0xFFF); // Move to next level table
        }
    }
}
