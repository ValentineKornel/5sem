#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

#define INITIAL_HEAP_SIZE (1 * 1024 * 1024) // 1 MiB
#define MAX_HEAP_SIZE (8 * 1024 * 1024)     // 8 MiB
#define BLOCK_SIZE (512 * 1024)              // 512 KiB
#define NUM_BLOCKS 10                        // Number of memory blocks

void PrintHeapInfo(size_t totalAllocated) {
    printf("Heap Information:\n");
    printf("Total Allocated Size: %zu bytes\n\n", totalAllocated);
}

int main() {
    void* blocks[NUM_BLOCKS];
    size_t totalAllocated = 0;

    for (int i = 0; i < NUM_BLOCKS; i++) {
        blocks[i] = malloc(BLOCK_SIZE);
        if (!blocks[i]) {
            printf("Memory allocation failed for block %d\n", i);
            return 1;
        }
        totalAllocated += BLOCK_SIZE;
        printf("Allocated block %d of 512 KiB at %p\n", i + 1, blocks[i]);
        PrintHeapInfo(totalAllocated);
        printf("Press any key to continue...\n");
        getchar();
    }

    for (int i = 0; i < NUM_BLOCKS; i++) {
        if (blocks[i]) {
            int* array = (int*)blocks[i];
            for (size_t j = 0; j < BLOCK_SIZE / sizeof(int); j++) {
                array[j] = j;
            }
        }
    }

    for (int i = 0; i < NUM_BLOCKS; i++) {
        if (blocks[i]) {
            free(blocks[i]);
            printf("Freed block %d at %p\n", i + 1, blocks[i]);
            PrintHeapInfo(totalAllocated);
            totalAllocated -= BLOCK_SIZE;
            printf("Press any key to continue...\n");
            getchar();
        }
    }

    printf("Heap destroyed successfully.\n");
    return 0;
}
