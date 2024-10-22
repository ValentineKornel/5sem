#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#pragma warning(disable : 4996)

CRITICAL_SECTION cs;
DWORD dwTlsIndex;

char globalPrimes[4096];
int globalPrimesSize = 0;

__declspec(thread) char buffer[1024];

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

    memset(buffer, 0, sizeof(buffer));

    //Сохраняем указатель на буфер в TLS
    if (!TlsSetValue(dwTlsIndex, buffer)) {
        ErrorExit("TlsSetValue error");
    }

    for (int i = param->start; i <= param->end; i++) {
        if (is_prime(i)) {
            current_buffer_size += snprintf(buffer + current_buffer_size, sizeof(buffer) - current_buffer_size, "%d ", i);
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

    //Sleep(2000);
    LeaveCriticalSection(&cs);

    return 0;
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
    DeleteCriticalSection(&cs);
    _cprintf("\nGlobal primes: %s\n", globalPrimes);

    return 0;
}

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
