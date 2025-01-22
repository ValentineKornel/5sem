#include <iostream>
#include <windows.h>
#include <string>
#include <chrono>
#include <thread>

#define SEMAPHORE_NAME "Global\\MySemaphore"
using namespace std;

void ThreadFunction(const std::string& name, HANDLE semaphore) {
    for (int i = 1; i <= 90; ++i) {
        if (i >= 30 && i <= 60) {
            WaitForSingleObject(semaphore, INFINITE);
        }

        cout << name << " - Iteration " << i << endl;
        Sleep(100);

        if (i >= 30 && i <= 60) {
            ReleaseSemaphore(semaphore, 1, NULL);
        }
    }
}

int main() {
    HANDLE semaphore = CreateSemaphoreA(NULL, 2, 2, SEMAPHORE_NAME);
    if (semaphore == NULL) {
        cerr << "Failed to create mutex." << endl;
        return 1;
    }

    PROCESS_INFORMATION procInfoA, procInfoB;
    STARTUPINFOA startInfoA, startInfoB;

    ZeroMemory(&startInfoA, sizeof(startInfoA));
    startInfoA.cb = sizeof(startInfoA);
    ZeroMemory(&procInfoA, sizeof(procInfoA));

    ZeroMemory(&startInfoB, sizeof(startInfoB));
    startInfoB.cb = sizeof(startInfoB);
    ZeroMemory(&procInfoB, sizeof(procInfoB));

    if (!CreateProcessA(NULL, (LPSTR)"OS06_04A.exe", NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &startInfoA, &procInfoA)) {
        cerr << "Failed to create process OS06_04A" << GetLastError() << endl;
        return 1;
    }

    if (!CreateProcessA(NULL, (LPSTR)"OS06_04B.exe", NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &startInfoB, &procInfoB)) {
        cerr << "Failed to create process OS06_04B" << endl;
        return 1;
    }

    string name = "Main";

    for (int i = 1; i <= 90; ++i) {
        if (i >= 30 && i <= 60) {
            WaitForSingleObject(semaphore, INFINITE);
            cout << name << " - Iteration " << i << endl;
            Sleep(100);
            ReleaseSemaphore(semaphore, 1, NULL);
        }
        else {
            cout << name << " - Iteration " << i << endl;
        }

        Sleep(100);
    }

    WaitForSingleObject(procInfoA.hProcess, INFINITE);
    WaitForSingleObject(procInfoB.hProcess, INFINITE);

    CloseHandle(procInfoA.hProcess);
    CloseHandle(procInfoA.hThread);
    CloseHandle(procInfoB.hProcess);
    CloseHandle(procInfoB.hThread);
    CloseHandle(semaphore);
    return 0;
}
