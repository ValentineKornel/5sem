#include <stdio.h>
#include <windows.h>

int main() {
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    DWORD pageSize = si.dwPageSize;
    printf("Page size: %lu bite\n", pageSize);

    SIZE_T numPages = 256;
    SIZE_T reserveSize = numPages * pageSize;

    LPVOID reservedMemory = VirtualAlloc(NULL, reserveSize, MEM_RESERVE, PAGE_NOACCESS);
    if (reservedMemory == NULL) {
        printf("Error allocation virtual memory: %lu\n", GetLastError());
        return 1;
    }
    printf("Allocated memory: %p\n", reservedMemory);
    printf("Press any key to continue...\n");
    getchar();

    LPVOID committedMemory = VirtualAlloc((LPVOID)((char*)reservedMemory + (numPages / 2) * pageSize),
        (numPages / 2) * pageSize, MEM_COMMIT, PAGE_READWRITE);
    if (committedMemory == NULL) {
        printf("Error allocatin physical memory: %lu\n", GetLastError());
        VirtualFree(reservedMemory, 0, MEM_RELEASE);
        return 1;
    }
    printf("Physical memory for the last half has benn allocated\n");
    printf("Press any key to continue...\n");
    getchar();

    int* intArray = (int*)committedMemory;
    for (int i = 0; i < (numPages / 2) * pageSize / sizeof(int); i++) {
        intArray[i] = i;
    }
    printf("Memmory fulfilled with the numbers sequence\n");
    printf("Press any key to continue...\n");
    getchar();

    DWORD oldProtect;
    if (!VirtualProtect(committedMemory, (numPages / 2) * pageSize, PAGE_READONLY, &oldProtect)) {
        printf("Error changin the pages protection: %lu\n", GetLastError());
        VirtualFree(reservedMemory, 0, MEM_RELEASE);
        return 1;
    }
    printf("Security attributes changed to 'READ ONLY'\n");
    printf("Press any key to continue...\n");
    getchar();

    if (!VirtualFree(committedMemory, (numPages / 2) * pageSize, MEM_DECOMMIT)) {
        printf("error releasing the memory: %lu\n", GetLastError());
    }
    else {
        printf("Physical memory released\n");
    }
    printf("Press any key to continue...\n");
    getchar();

    if (!VirtualFree(reservedMemory, 0, MEM_RELEASE)) {
        printf("Error releasing virtual memory: %lu\n", GetLastError());
    }
    else {
        printf("Virtual memory released\n");
    }
    printf("Press any key to continue...\n");
    getchar();

    return 0;
}
