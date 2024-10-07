#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <locale.h>

int main() {


    STARTUPINFOA si1, si2, si3;
    PROCESS_INFORMATION pi1, pi2, pi3;

    boolean pr1 = 0, pr2 = 0, pr3 = 0;

    ZeroMemory(&si1, sizeof(si1));
    si1.cb = sizeof(si1);
    ZeroMemory(&pi1, sizeof(pi1));

    if (pr1 = CreateProcessA("D:\\Study\\university\\5semester\\SP\\lab2\\build\\Release\\Lab-02x.exe 10",
        NULL,
        NULL, NULL,
        FALSE, 0, NULL, NULL,
        &si1, &pi1)) {
        printf("Process with first parameter starter. PID: %lu\n", pi1.dwProcessId);


    }
    else {
        printf("Error createing file with first parameter: %d\n", GetLastError());
    }

    ZeroMemory(&si2, sizeof(si2));
    si2.cb = sizeof(si2);
    si2.dwFlags = STARTF_USESHOWWINDOW;
    si2.wShowWindow = SW_SHOW;
    ZeroMemory(&pi2, sizeof(pi2));

    char* commandLine2 = _strdup("D:\\Study\\university\\5semester\\SP\\lab2\\build\\Release\\Lab-02x.exe 10");
    if (pr2 = CreateProcessA(NULL,
        commandLine2,
        NULL, NULL,
        FALSE, CREATE_NEW_CONSOLE, NULL, NULL,
        &si2, &pi2)) {
        printf("Process with second parameter starter. PID: %lu\n", pi2.dwProcessId);


    }
    else {
        printf("Error createing file with second parameter: %n\n", GetLastError());

    }

    ZeroMemory(&si3, sizeof(si3));
    si3.cb = sizeof(si3);
    si3.dwFlags = STARTF_USESHOWWINDOW;
    si3.wShowWindow = SW_SHOW;
    ZeroMemory(&pi3, sizeof(pi3));

    char* commandLine3 = _strdup(" 10");
    if (pr3 = CreateProcessA("D:\\Study\\university\\5semester\\SP\\lab2\\build\\Release\\Lab-02x.exe",
        commandLine3,
        NULL, NULL,
        FALSE, CREATE_NEW_CONSOLE, NULL, NULL,
        &si3, &pi3)) {
        printf("Process with two parameters starter. PID: %lu\n", pi3.dwProcessId);
        

    }
    else {
        printf("Error createing file with two parameters: %d\n", GetLastError());
    }

    if (pr1) {
        WaitForSingleObject(pi1.hProcess, INFINITE);
        CloseHandle(pi1.hProcess);
        CloseHandle(pi1.hThread);
    }

    if (pr2) {
        WaitForSingleObject(pi2.hProcess, INFINITE);
        CloseHandle(pi2.hProcess);
        CloseHandle(pi2.hThread);
    }

    if (pr3) {
        WaitForSingleObject(pi3.hProcess, INFINITE);
        CloseHandle(pi3.hProcess);
        CloseHandle(pi3.hThread);
    }


    return 0;
}
