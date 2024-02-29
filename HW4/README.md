# Multi-Level Page Table Simulation

This project simulates a multi-level page table system used for translating virtual addresses to physical addresses in a computer's memory management unit (MMU). It's designed to be configurable and extendable to support different levels of page tables and page sizes.

## Features

- Configurable levels of page tables (`LEVELS`) and page offset bits (`POBITS`).
- Implementation of `translate` function for virtual-to-physical address translation.
- Implementation of `page_allocate` function for dynamic memory allocation and mapping.

## Getting Started

### Prerequisites

Ensure you have `gcc` and `make` installed on your system. You can check their presence with `gcc --version` and `make --version`.

### Building the Project

1. **Clone the Repository**

   ```bash
   git clone <repository-url>
   cd <project-directory>



