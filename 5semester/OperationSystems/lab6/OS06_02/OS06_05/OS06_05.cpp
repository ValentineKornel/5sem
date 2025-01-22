#include <iostream>
#include <windows.h>
#include <string>

#define EVENT_NAME "Global\\MyEvent"

using namespace std;

int main() {
    HANDLE eventHandle = CreateEventA(NULL, TRUE, FALSE, EVENT_NAME);
    if (eventHandle == NULL) {
        cerr << "Failed to create event. Error code: " << GetLastError() << endl;
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

    if (!CreateProcessA(NULL, (LPSTR)"OS06_05A.exe", NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &startInfoA, &procInfoA)) {
        cerr << "Failed to create process OS06_05A. Error code: " << GetLastError() << endl;
        CloseHandle(eventHandle);
        return 1;
    }

    if (!CreateProcessA(NULL, (LPSTR)"OS06_05B.exe", NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &startInfoB, &procInfoB)) {
        cerr << "Failed to create process OS06_05B. Error code: " << GetLastError() << endl;
        TerminateProcess(procInfoA.hProcess, 0);
        CloseHandle(procInfoA.hProcess);
        CloseHandle(procInfoA.hThread);
        CloseHandle(eventHandle);
        return 1;
    }

    for (int i = 1; i <= 90; ++i) {
        cout << "Main - Iteration " << i << endl;
        if (i == 15) {
            SetEvent(eventHandle);
        }
        Sleep(100);
    }

    WaitForSingleObject(procInfoA.hProcess, INFINITE);
    WaitForSingleObject(procInfoB.hProcess, INFINITE);

    CloseHandle(procInfoA.hProcess);
    CloseHandle(procInfoA.hThread);
    CloseHandle(procInfoB.hProcess);
    CloseHandle(procInfoB.hThread);
    CloseHandle(eventHandle);
    return 0;
}
