#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <locale.h>
#pragma warning(disable : 4996)


int main(int argc, char* argv[]) {

    int iterations = 0;

    if (argc == 2) {
        iterations = atoi(argv[1]);
        if (iterations <= 0) {
            printf("Error. number of iterations must be positive.\n");
            ExitProcess(1);
        }
    }
    else {
        char buff[20];
        if (GetEnvironmentVariableA("ITER_NUM", buff, sizeof(buff))) {
            iterations = atoi(buff);
        }
    }

    if (iterations <= 0) {
        printf("Error. Set number of iteration via command line argument or env variable ITER_NUM.\n");
        ExitProcess(1);
    }


    printf("Number of iterations: %d\n", iterations);

    DWORD processId = GetCurrentProcessId();

    for (int i = 1; i <= iterations; i++) {
        printf("Iteration %d, PID: %lu\n", i, processId);
        Sleep(500);
    }

    printf("Ending programm\n");
    system("pause");
    return 0;
}
