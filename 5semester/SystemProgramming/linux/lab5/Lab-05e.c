#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

int main() {
    long pageSize = sysconf(_SC_PAGESIZE);
    printf("Page size: %ld bytes\n", pageSize);

    size_t numPages = 256;
    size_t reserveSize = numPages * pageSize;

    void* reservedMemory = mmap(NULL, reserveSize, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (reservedMemory == MAP_FAILED) {
        perror("Error allocating virtual memory");
        return 1;
    }
    printf("Allocated memory: %p\n", reservedMemory);
    printf("Press any key to continue...\n");
    getchar();

    void* committedMemory = mmap((char*)reservedMemory + (numPages / 2) * pageSize,
                                  (numPages / 2) * pageSize, 
                                  PROT_READ | PROT_WRITE, 
                                  MAP_PRIVATE | MAP_ANONYMOUS, 
                                  -1, 0);
    if (committedMemory == MAP_FAILED) {
        perror("Error allocating physical memory");
        munmap(reservedMemory, reserveSize);
        return 1;
    }
    printf("Physical memory for the last half has been allocated\n");
    printf("Press any key to continue...\n");
    getchar();

    int* intArray = (int*)committedMemory;
    for (int i = 0; i < (numPages / 2) * pageSize / sizeof(int); i++) {
        intArray[i] = i;
    }
    printf("Memory fulfilled with the numbers sequence\n");
    printf("Press any key to continue...\n");
    getchar();

    if (mprotect(committedMemory, (numPages / 2) * pageSize, PROT_READ) != 0) {
        perror("Error changing the pages protection");
        munmap(reservedMemory, reserveSize);
        return 1;
    }
    printf("Security attributes changed to 'READ ONLY'\n");
    printf("Press any key to continue...\n");
    getchar();

    if (munmap(committedMemory, (numPages / 2) * pageSize) != 0) {
        perror("Error releasing the memory");
    } else {
        printf("Physical memory released\n");
    }
    printf("Press any key to continue...\n");
    getchar();

    if (munmap(reservedMemory, reserveSize) != 0) {
        perror("Error releasing virtual memory");
    } else {
        printf("Virtual memory released\n");
    }
    printf("Press any key to continue...\n");
    getchar();

    return 0;
}
