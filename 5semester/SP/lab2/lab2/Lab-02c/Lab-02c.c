#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <locale.h>

int main() {

    if (SetCurrentDirectory("D:\\Study\\university\\5semester\\SP\\lab2\\testFolder\n")) {
        printf("Currenct directory changed to: D:\\Study\\university\\5semester\\SP\\lab2\\testFolder\n");
    }
    else {
        printf("Error changing directory: %d\n", GetLastError());
        return 1;
    }

    //char systemDir[MAX_PATH];
    //char windowsDir[MAX_PATH];
    //if (GetSystemDirectoryA(systemDir, MAX_PATH)) {
    //    std::cout << "Системный каталог: " << systemDir << std::endl;
    //}
    //if (GetWindowsDirectoryA(windowsDir, MAX_PATH)) {
    //    std::cout << "Основной каталог Windows: " << windowsDir << std::endl;
    //}

    STARTUPINFOA si1, si2, si3, si4, si5;
    PROCESS_INFORMATION pi1, pi2, pi3, pi4, pi5;

    LPCSTR applicationName = "Lab-02x.exe";
    char* commandLine = _strdup(" 10");

    boolean pr1 = 0, pr2 = 0, pr3 = 0, pr4 = 0, pr5 = 0;

    ZeroMemory(&si1, sizeof(si1));
    si1.cb = sizeof(si1);
    si1.dwFlags = STARTF_USESHOWWINDOW;
    si1.wShowWindow = SW_SHOW;
    ZeroMemory(&pi1, sizeof(pi1));

    if (pr1 = CreateProcessA(NULL,
        (LPSTR)"Lab-02hw-1.exe",
        NULL, NULL,
        FALSE, CREATE_NEW_CONSOLE, NULL, NULL,
        &si1, &pi1)) {
        printf("Process 1 started. PID: %d\n", pi1.dwProcessId);


    }
    else {
        printf("Error starting process 1: %d\n", GetLastError());
    }

    ZeroMemory(&si2, sizeof(si2));
    si2.cb = sizeof(si2);
    si2.dwFlags = STARTF_USESHOWWINDOW;
    si2.wShowWindow = SW_SHOW;
    ZeroMemory(&pi2, sizeof(pi2));

    if (pr2 = CreateProcessA("Lab-02hw-2.exe",
        NULL,
        NULL, NULL,
        FALSE, CREATE_NEW_CONSOLE, NULL, NULL,
        &si2, &pi2)) {
        printf("Process 2 started. PID: %d\n", pi2.dwProcessId);


    }
    else {
        printf("Error starting process 2: %d\n", GetLastError());
    }

    ZeroMemory(&si3, sizeof(si3));
    si3.cb = sizeof(si3);
    si3.dwFlags = STARTF_USESHOWWINDOW;
    si3.wShowWindow = SW_SHOW;
    ZeroMemory(&pi3, sizeof(pi3));

    if (pr3 = CreateProcessA(NULL,
        (LPSTR)"Lab-02hw-3.exe",
        NULL, NULL,
        FALSE, CREATE_NEW_CONSOLE, NULL, NULL,
        &si3, &pi3)) {
        printf("Process 3 started. PID: %d\n", pi3.dwProcessId);


    }
    else {
        printf("Error starting process 3: %d\n", GetLastError());

    }

    ZeroMemory(&si4, sizeof(si4));
    si4.cb = sizeof(si4);
    si4.dwFlags = STARTF_USESHOWWINDOW;
    si4.wShowWindow = SW_SHOW;
    ZeroMemory(&pi4, sizeof(pi4));

    if (pr4 = CreateProcessA(NULL,
        (LPSTR)"Lab-02hw-4.exe",
        NULL, NULL,
        FALSE, CREATE_NEW_CONSOLE, NULL, NULL,
        &si4, &pi4)) {
        printf("Process 4 started. PID: %d\n", pi4.dwProcessId);


    }
    else {
        printf("Error starting process 4: %d\n", GetLastError());

    }

    ZeroMemory(&si5, sizeof(si5));
    si5.cb = sizeof(si5);
    si5.dwFlags = STARTF_USESHOWWINDOW;
    si5.wShowWindow = SW_SHOW;
    ZeroMemory(&pi5, sizeof(pi5));

    if (pr5 = CreateProcessA(NULL,
        (LPSTR)"Lab-02hw-5.exe",
        NULL, NULL,
        FALSE, CREATE_NEW_CONSOLE, NULL, NULL,
        &si5, &pi5)) {
        printf("Process 5 started. PID: %d\n", pi5.dwProcessId);


    }
    else {
        printf("Error starting process 5: %d\n", GetLastError());

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

    if (pr4) {
        WaitForSingleObject(pi4.hProcess, INFINITE);
        CloseHandle(pi4.hProcess);
        CloseHandle(pi4.hThread);
    }

    if (pr5) {
        WaitForSingleObject(pi5.hProcess, INFINITE);
        CloseHandle(pi5.hProcess);
        CloseHandle(pi5.hThread);
    }


    return 0;
}
