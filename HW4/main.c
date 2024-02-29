#include "mlpt.h"
#include <stdio.h>
#include <assert.h>
#include <stdint.h>

int main() {
    // Initialize ptbr to 0 at the start
    assert(ptbr == 0);

    // Existing Test 1: Translate an address before any allocation
    printf("Existing Test 1: Translate without allocation\n");
    size_t result = translate(0x12345678);
    assert(result == SIZE_MAX);
    printf("Passed: Unallocated address correctly not translated.\n\n");

    // Existing Test 2: Allocate a page and then translate an address within it
    printf("Existing Test 2: Allocate and translate\n");
    page_allocate(0x12345000);  // Allocate a page that includes the address 0x12345678
    result = translate(0x12345678);
    assert(result != SIZE_MAX);  // Should now translate to a valid physical address
    printf("Passed: Allocated address translated successfully.\n\n");

    // Existing Test 3: Translate an address in the same page to ensure no additional allocation
    printf("Existing Test 3: Translate another address in the same allocated page\n");
    size_t previous_result = result;
    result = translate(0x12345FFF);  // Last address in the allocated page
    assert(result != SIZE_MAX && (result >> 12) == (previous_result >> 12));  // Should be in the same page
    printf("Passed: Address in the same page translated correctly, no additional allocation.\n\n");

    // Existing Test 4: Allocate a new page and ensure a new allocation occurs
    printf("Existing Test 4: Allocate a new page\n");
    page_allocate(0x12346000);  // Allocate a different page
    result = translate(0x12346000);
    assert(result != SIZE_MAX && (result >> 12) != (previous_result >> 12));  // Should be a different page
    printf("Passed: New page allocated and translated correctly.\n\n");

    // New Test 1: Allocating and translating address 0x0
    printf("New Test 1: Allocating and translating address 0x0\n");
    page_allocate(0x0);  // Allocate the first page
    size_t pa = translate(0x0);
    assert(pa != SIZE_MAX);  // Ensure translation is successful
    printf("Translation of 0x0 successful, physical address: 0x%lx\n", pa);

    // New Test 2: Allocating and translating address 0x200000
    printf("New Test 2: Allocating and translating address 0x200000\n");
    page_allocate(0x200000);  // Allocate a page at a different high-level index
    pa = translate(0x200000);
    assert(pa != SIZE_MAX);  // Ensure translation is successful
    printf("Translation of 0x200000 successful, physical address: 0x%lx\n", pa);

    // New Test 3: Translating unallocated address 0x300000
    printf("New Test 3: Translating unallocated address 0x300000\n");
    pa = translate(0x300000);  // Attempt to translate an unallocated address
    assert(pa == SIZE_MAX);  // Translation should fail
    printf("Translation of unallocated address 0x300000 correctly failed\n");

    // New Test 4: Translating address 0x200100 within an allocated page
    printf("New Test 4: Translating address 0x200100 within an allocated page\n");
    pa = translate(0x200100);  // This address falls within the page allocated at 0x200000
    assert(pa != SIZE_MAX);  // Ensure translation is successful
    printf("Translation within an allocated page successful, physical address: 0x%lx\n", pa);

    printf("All tests passed!\n");
    return 0;
}

