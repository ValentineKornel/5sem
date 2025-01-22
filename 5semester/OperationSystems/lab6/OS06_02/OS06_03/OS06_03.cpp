#include <iostream>
#include <windows.h>
#include <string>
#include <chrono>
#include <thread>

#define MUTEX_NAME "Global\\MyMutex"
using namespace std;


int main() {
    HANDLE mutex = CreateMutexA(NULL, FALSE, MUTEX_NAME);
    if (mutex == NULL) {
        cerr << "Failed to create mutex." << endl;
        return 1;
    }

    STARTUPINFOA siA = { sizeof(siA) };
    PROCESS_INFORMATION piA;
    STARTUPINFOA siB = { sizeof(siB) };
    PROCESS_INFORMATION piB;

    if (!CreateProcessA(NULL, (LPSTR)"OS06_03A.exe", NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &siA, &piA)) {
        cerr << "Failed to start process A." << endl;
        CloseHandle(mutex);
        return 1;
    }

    if (!CreateProcessA(NULL, (LPSTR)"OS06_03B.exe", NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &siB, &piB)) {
        cerr << "Failed to start process B." << endl;
        CloseHandle(mutex);
        return 1;
    }

    string name = "Main";
    for (int i = 1; i <= 90; ++i) {
        if (i >= 30 && i <= 60) {
            WaitForSingleObject(mutex, INFINITE);
            cout << "Process " << name << " - iteration: " << i << endl;
            Sleep(2000);
            ReleaseMutex(mutex);
        }
        else {
            cout << "Process " << name << " - iteration: " << i << endl;
        }
        Sleep(100);
    }

    WaitForSingleObject(piA.hProcess, INFINITE);
    WaitForSingleObject(piB.hProcess, INFINITE);

    CloseHandle(piA.hProcess);
    CloseHandle(piB.hProcess);
    CloseHandle(mutex);
    return 0;
}
