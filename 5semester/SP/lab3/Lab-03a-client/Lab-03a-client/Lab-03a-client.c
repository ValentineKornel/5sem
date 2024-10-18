#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#pragma warning(disable : 4996)

int is_prime(int num) {
    if (num < 2) return 0;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <start> <end> [mutex name]\n", argv[0]);
        return 1;
    }

    int start = atoi(argv[1]);
    int end = atoi(argv[2]);

    if (start > end) {
        fprintf(stderr, "wrong range given");
        return 1;
    }
    _cprintf("start %d end %d\n", start, end);

    char mutex_name[20];
    if (argv[3] != NULL) {
        strncpy(mutex_name, argv[3], sizeof(mutex_name) - 1);
        mutex_name[sizeof(mutex_name) - 1] = '\0';
    }
    else
        if (!GetEnvironmentVariableA("MUTEX_NAME", mutex_name, sizeof(mutex_name))) {
            _cprintf("env error");
            return 1;
        }   


    
    HANDLE hWritePipe = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE mutex = OpenMutexA(SYNCHRONIZE, FALSE, mutex_name);
    if (mutex == NULL) {
        printf("Error opening mutex: %d\n", GetLastError());
        return 1;
    }

    char buffer[250];
    int buffer_size = 0;

    for (int i = start; i <= end; i++) {
        if (is_prime(i)) {
            buffer_size += snprintf(buffer + buffer_size, sizeof(buffer) - buffer_size, "%d ", i);
        }
    }

    _cprintf("waiting for mutex...\n");
    WaitForSingleObject(mutex, INFINITE);

    DWORD dwBytesWritten;
    if (!WriteFile(
        hWritePipe,
        &buffer,
        sizeof(buffer),
        &dwBytesWritten,
        NULL))
    {
        _cputs("Write to file failed.\n");
        _cputs("Press any key to finish.\n");
        _getch();
        return GetLastError();
    }
    _cprintf("The result is written to the pipe.\n");

    Sleep(2000);
    ReleaseMutex(mutex);
    CloseHandle(mutex);
    CloseHandle(hWritePipe);
    _cputs("Press any key to exit.\n");
    _getch();

    return 0;
}
