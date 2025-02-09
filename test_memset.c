#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// External declarations for the assembly routines.
extern void memset_original(void *dest, int c, size_t n);
extern void memset_optimized(void *dest, int c, size_t n);

#define BUFFER_SIZE 256

// Helper function: returns 1 if the first n bytes of buf equal value, else 0.
int check_buffer(const unsigned char *buf, size_t n, unsigned char value) {
    for (size_t i = 0; i < n; i++) {
        if (buf[i] != value)
            return 0;
    }
    return 1;
}

// The test_memset function exercises both implementations.
void test_memset(void) {
    int tests_passed = 1;
    int errors = 0;
    printf("Starting memset tests...\n");

    // --- Test 1: Zero-Length Test ---
    {
        unsigned char buf[16];
        // Initialize with a known pattern.
        memset(buf, 0xAA, sizeof(buf));

        // Optimized version with n = 0 should leave buffer unchanged.
        memset_optimized(buf, 0x55, 0);
        if (!check_buffer(buf, sizeof(buf), 0xAA)) {
            printf("Error: Zero-length test failed for memset_optimized!\n");
            tests_passed = 0;
            errors++;
        }

        // Original version with n = 0 should leave buffer unchanged.
        memset(buf, 0xAA, sizeof(buf));
        memset_original(buf, 0x55, 0);
        if (!check_buffer(buf, sizeof(buf), 0xAA)) {
            printf("Error: Zero-length test failed for memset_original!\n");
            tests_passed = 0;
            errors++;
        }
    }

    // --- Test 2: Misaligned Pointer Test ---
    {
        // Allocate extra room to test various misalignment offsets.
        unsigned char *buffer = malloc(BUFFER_SIZE + 8);
        if (!buffer) {
            fprintf(stderr, "Memory allocation failed!\n");
            exit(1);
        }

        for (int offset = 0; offset < 8; offset++) {
            unsigned char *ptr = buffer + offset;

            // Test optimized version.
            memset(ptr, 0, BUFFER_SIZE);
            memset_optimized(ptr, 0xCC, BUFFER_SIZE);
            if (!check_buffer(ptr, BUFFER_SIZE, 0xCC)) {
                printf("Error: memset_optimized failed for misaligned pointer at offset %d\n", offset);
                tests_passed = 0;
                errors++;
            }

            // Test original version.
            memset(ptr, 0, BUFFER_SIZE);
            memset_original(ptr, 0xDD, BUFFER_SIZE);
            if (!check_buffer(ptr, BUFFER_SIZE, 0xDD)) {
                printf("Error: memset_original failed for misaligned pointer at offset %d\n", offset);
                tests_passed = 0;
                errors++;
            }
        }
        free(buffer);
    }

    // --- Test 3: Various Sizes (including sizes not a multiple of 4) ---
    {
        unsigned char buf[64];
        for (size_t size = 1; size <= sizeof(buf); size++) {
            // Test optimized version.
            memset(buf, 0, sizeof(buf));
            memset_optimized(buf, 0xEF, size);
            if (!check_buffer(buf, size, 0xEF)) {
                printf("Error: memset_optimized failed for size %zu\n", size);
                tests_passed = 0;
                errors++;
            }

            // Test original version.
            memset(buf, 0, sizeof(buf));
            memset_original(buf, 0xAB, size);
            if (!check_buffer(buf, size, 0xAB)) {
                printf("Error: memset_original failed for size %zu\n", size);
                tests_passed = 0;
                errors++;
            }
        }
    }

    // --- Test 4: Compare Both Implementations ---
    {
        unsigned char buf1[BUFFER_SIZE];
        unsigned char buf2[BUFFER_SIZE];
        for (size_t size = 0; size < BUFFER_SIZE; size++) {
            memset(buf1, 0, BUFFER_SIZE);
            memset(buf2, 0, BUFFER_SIZE);
            memset_original(buf1, 0x5A, size);
            memset_optimized(buf2, 0x5A, size);
            if (memcmp(buf1, buf2, BUFFER_SIZE) != 0) {
                printf("Error: Mismatch between implementations for size %zu\n", size);
                tests_passed = 0;
                errors++;
            }
        }
    }

    if (tests_passed)
        printf("All tests passed successfully.\n");
    else
        printf("Some tests failed. Total errors: %d\n", errors);
}

#ifdef UNIT_TEST
int main(void) {
    test_memset();
    return 0;
}
#endif
