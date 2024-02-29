#ifndef CONFIG_H
#define CONFIG_H

// LEVELS defines the number of levels in the page table hierarchy.
// This simulation supports values between 1 and 6 for LEVELS.
#define LEVELS  1  // Example: Set to 1 for a single-level page table

// POBITS defines the number of bits used for the page offset within a virtual address.
// Valid range for POBITS is from 4 to 18, inclusive.
#define POBITS  12  // Example: Set to 12 for 4KB pages (2^12 bytes per page)

#endif // CONFIG_H