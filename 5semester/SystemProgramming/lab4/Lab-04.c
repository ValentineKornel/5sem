#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#pragma warning(disable : 4996)

CRITICAL_SECTION cs;
DWORD dwTlsIndex;

#define MAX_NUMBERS 1024

char globalPrimes[4096];
int globalPrimesSize = 0;

typedef struct {
    int start;
    int end;
} ThreadParams;

VOID ErrorExit(LPCSTR message)
{
    fprintf(stderr, "%s\n", message);
    ExitProcess(0);
}

int is_prime(int num) {
    if (num < 2) return 0;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) {
            return 0;
        }
    }
    return 1;
}

DWORD WINAPI ThreadFunc(LPVOID lpParam)
{

    ThreadParams* param = (ThreadParams*)lpParam;

    int current_buffer_size = 0;
    int buffer_capacity = 1024;

    char* buffer = (char*)LocalAlloc(LPTR, buffer_capacity * sizeof(char));
    if (buffer == NULL) {
        ErrorExit("LocalAlloc failed");
    }

     //Сохраняем указатель на буфер в TLS
    if (!TlsSetValue(dwTlsIndex, buffer)) {
        ErrorExit("TlsSetValue error");
    }

    for (int i = param->start; i <= param->end; i++) {
        if (is_prime(i)) {
            current_buffer_size += snprintf(buffer + current_buffer_size, buffer_capacity - current_buffer_size, "%d ", i);
        }
    }

    // входим в критическую секцию
    EnterCriticalSection(&cs);
    _cprintf("Thread %d in critical section...   param: %d - %d\n", GetCurrentThreadId(), param->start, param->end);

    char* tlsBuffer = (char*)TlsGetValue(dwTlsIndex);
    if (tlsBuffer == NULL) {
        ErrorExit("TlsGetValue error");
    }

     //Выводим содержимое буфера в консоль
    _cprintf("Thread %d found primes: %s\n", GetCurrentThreadId(), tlsBuffer);

    if (globalPrimesSize + current_buffer_size < sizeof(globalPrimes)) {
        strcat(globalPrimes, tlsBuffer);
        globalPrimesSize += current_buffer_size;
    }
    else {
        _cprintf("Global buffer overflow, can't add primes\n");
    }

    Sleep(200000);
    LeaveCriticalSection(&cs);
    LocalFree(buffer);

    return 0;
}

// Функция для сортировки чисел
int compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

// Функция для сортировки массива простых чисел
void sortGlobalPrimes() {
    int numbers[MAX_NUMBERS];
    int numCount = 0;

    // Разбираем строку на числа
    char* token = strtok(globalPrimes, " ");
    while (token != NULL && numCount < MAX_NUMBERS) {
        numbers[numCount++] = atoi(token);
        token = strtok(NULL, " ");
    }

    // Сортируем массив чисел
    qsort(numbers, numCount, sizeof(int), compare);

    // Записываем отсортированные числа обратно в строку
    globalPrimes[0] = '\0';
    for (int i = 0; i < numCount; i++) {
        char temp[16];
        snprintf(temp, sizeof(temp), "%d ", numbers[i]);
        strcat(globalPrimes, temp);
    }
}


int main(int argc, char* argv[])
{

    if (argc < 3) {
        fprintf(stderr, "Usage: %s <nubmer of process> <start> <end>\n", argv[0]);
        return 1;
    }

    int numberOfProcess = atoi(argv[1]);
    int start = atoi(argv[2]);
    int end = atoi(argv[3]);
    DWORD IDThread;
    HANDLE hThread[10];
    int i;

    InitializeCriticalSection(&cs);

    int range = end - start + 1;
    int step = range / numberOfProcess;

     //Allocate a TLS index.
    if ((dwTlsIndex = TlsAlloc()) == TLS_OUT_OF_INDEXES)
        ErrorExit("TlsAlloc failed");

     //Create multiple threads. 
    for (i = 0; i < numberOfProcess; i++)
    {
        int local_start = start + i * step;
        int local_end = local_start + step - 1;
        if (i == numberOfProcess - 1)
            local_end = end;
        ThreadParams* params = (ThreadParams*)malloc(sizeof(ThreadParams));
        params->start = local_start;
        params->end = local_end;

        hThread[i] = CreateThread(NULL, // default security attributes 
            0,                           // use default stack size 
            (LPTHREAD_START_ROUTINE)ThreadFunc, // thread function 
            params,                    // no thread function argument 
            0,                       // use default creation flags 
            &IDThread);              // returns thread identifier 

        // Check the return value for success. 
        if (hThread[i] == NULL)
            ErrorExit("CreateThread error\n");
    }

    WaitForMultipleObjects(numberOfProcess, hThread, TRUE, INFINITE);
    TlsFree(dwTlsIndex);
    DeleteCriticalSection(&cs);
    sortGlobalPrimes();
    _cprintf("\nGlobal primes: %s\n", globalPrimes);

    return 0;
}
