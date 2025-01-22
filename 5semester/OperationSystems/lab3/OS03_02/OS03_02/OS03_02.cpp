#include <iostream>
#include <thread>
#include <windows.h>
#include <chrono>
using namespace std;


int main()
{
    DWORD processId = GetCurrentProcessId();


    STARTUPINFO si1, si2;
    PROCESS_INFORMATION pi1, pi2;
    ZeroMemory(&si1, sizeof(si1));
    si1.cb = sizeof(si1);
    ZeroMemory(&pi1, sizeof(pi1));

    ZeroMemory(&si2, sizeof(si2));
    si2.cb = sizeof(si2);
    ZeroMemory(&pi2, sizeof(pi2));

    BOOL bRes1 = CreateProcessW(L"D:\\Study\\university\\5semester\\OS\\lab3\\OS03_02\\OS03_02_1\\Release\\OS03_02_1.exe", NULL,
        NULL, NULL, FALSE, 0, NULL, NULL, &si1, &pi1);
    if (!bRes1) {
        printf("Error creating process 1: %d\n", GetLastError());
        return 1;
    }
    cout << "Waiting for the process 1..." << endl;

    BOOL bRes2 = CreateProcessW(L"D:\\Study\\university\\5semester\\OS\\lab3\\OS03_02\\OS03_02_2\\Release\\OS03_02_2.exe", NULL,
        NULL, NULL, FALSE, 0, NULL, NULL, &si2, &pi2);
    if (!bRes2) {
        printf("Error creating process 2: %d\n", GetLastError());
        CloseHandle(pi1.hProcess);
        CloseHandle(pi1.hThread);
        return 1;
    }
    cout << "Waiting for the process 2..." << endl;

    WaitForSingleObject(pi1.hProcess, INFINITE);
    WaitForSingleObject(pi2.hProcess, INFINITE);

    CloseHandle(pi1.hProcess);
    CloseHandle(pi1.hThread);
    cout << "Process 1 was closed" << endl;

    CloseHandle(pi2.hProcess);
    CloseHandle(pi2.hThread);
    cout << "Process 2 was closed" << endl;


    for (int i = 1; i <= 100; i++) {
        cout << "Process ID: " << processId << ", Iteration: " << i << endl;

        this_thread::sleep_for(chrono::seconds(1));
    }
}
