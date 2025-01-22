#include <windows.h>
#include <stdio.h>

#define INITIAL_HEAP_SIZE (1 * 1024 * 1024)  // 1 MiB
#define MAX_HEAP_SIZE (8 * 1024 * 1024)      // 8 MiB
#define BLOCK_SIZE (512 * 1024)              // 512 KiB
//#define BLOCK_SIZE (1024 * 1024)              // 1 ÌiB
//#define BLOCK_SIZE (1019 * 1024)              // 1 ÌiB
#define NUM_BLOCKS 10                        // Number of memory blocks


void HeapInfo(HANDLE heap) {
    PROCESS_HEAP_ENTRY entry;
    entry.lpData = NULL;
    SIZE_T totalSize = 0;

    printf("Heap Information:\n");

    while (HeapWalk(heap, &entry)) {
        printf("Address: %p, Size: %zu bytes, ", entry.lpData, entry.cbData);
        if (entry.wFlags & PROCESS_HEAP_ENTRY_BUSY) {
            printf("Type: Allocated\n");
        }
        else if (entry.wFlags & PROCESS_HEAP_REGION) {
            printf("Type: Region\n");
        }
        else {
            printf("Type: Free\n");
        }
        totalSize += entry.cbData;
    }

    printf("Total Heap Size: %zu bytes\n\n", totalSize);

    if (GetLastError() != ERROR_NO_MORE_ITEMS) {
        printf("HeapWalk error: %lu\n", GetLastError());
    }
}

int main() {
    HANDLE heap = HeapCreate(0, INITIAL_HEAP_SIZE, MAX_HEAP_SIZE);
    if (!heap) {
        printf("Heap creation failed. Error: %lu\n", GetLastError());
        return 1;
    }
    printf("Heap created with initial size of 1 MiB and max size of 8 MiB\n");
    HeapInfo(heap);
    system("pause & cls");

    void* blocks[NUM_BLOCKS];
    for (int i = 0; i < NUM_BLOCKS; i++) {
        blocks[i] = HeapAlloc(heap, HEAP_ZERO_MEMORY, BLOCK_SIZE);
        if (!blocks[i]) {
            printf("Heap allocation failed for block %d. Error: %lu\n", i, GetLastError());
            return 0;
        }
        printf("Allocated block %d of 512 KiB at %p\n", i + 1, blocks[i]);
        HeapInfo(heap);
        system("pause & cls");
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
            HeapFree(heap, 0, blocks[i]);
            printf("Freed block %d at %p\n", i + 1, blocks[i]);
            HeapInfo(heap);
            system("pause & cls");
        }
    }

    if (!HeapDestroy(heap)) {
        printf("Heap destruction failed. Error: %lu\n", GetLastError());
    }
    else {
        printf("Heap destroyed successfully.\n");
    }

    return 0;
}
