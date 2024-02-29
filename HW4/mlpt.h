#ifndef MLPT_H
#define MLPT_H

#include <stddef.h> // For size_t

// Page Table Base Register (global variable).
// This holds the address of the top-level page table.
extern size_t ptbr;

/**
 * Translates a virtual address to a physical address.
 * 
 * @param va The virtual address to be translated.
 * @return The corresponding physical address if the virtual address is mapped.
 *         Returns SIZE_MAX (all bits set to 1) if the address is not mapped.
 */
size_t translate(size_t va);

/**
 * Ensures there is a mapping for a given virtual address in the page table.
 * If the mapping does not exist, page tables and/or a page for data will be allocated.
 * 
 * @param va The virtual address for which to ensure a mapping.
 */
void page_allocate(size_t va);

#endif // MLPT_H
