# Newlib RISC-V Optimization Challenge

This repository contains two implementations of the `memset` function for RISC-V as part of a coding challenge to optimize Newlib for RISC-V. The goal of this challenge is to reduce static code size and lower the number of instructions (cycles) used by important library functions.

## Project Overview

This project provides two versions of `memset` written in RISC-V assembly:

- **memset_original.s**  
  A baseline implementation that fills memory byte-by-byte.

- **memset_optimized.s**  
  An optimized version that:
  - Checks the destination pointer’s alignment.
  - Processes a misaligned prefix using byte stores.
  - Uses 32-bit (word) stores for the aligned bulk of memory.
  - Handles any trailing bytes with a tail loop.  
  In this file, all pseudo‑instructions have been replaced with their base-instruction equivalents (for example, `mv` is replaced with `addi rd, rs, 0`), ensuring compatibility with the RISC-V GNU toolchain.

In addition, a test suite is provided in **test_memset.c** to verify correctness. The tests cover various cases including:
- Zero-length operations.
- Misaligned destination pointers.
- Various sizes (including sizes not a multiple of 4).
- Direct comparison between the original and optimized implementations.

## Building the Project

### Prerequisites

- A RISC-V GNU toolchain must be installed on your computer.
- This example assumes a 64-bit RISC-V target. If you are targeting a 32-bit architecture, adjust the compiler command accordingly.

### Compilation Instructions

Open a terminal in the repository directory and compile the test program with the following command:

```bash
riscv64-unknown-elf-gcc -DUNIT_TEST -o test_memset test_memset.c memset_original.s memset_optimized.s
