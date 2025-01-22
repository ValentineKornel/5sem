#include <iostream>
#include <windows.h>
#include <string>
#include <chrono>
#include <thread>

#define SEMAPHORE_NAME "Global\\MySemaphore"

using namespace std;

int main() {
    cout << "Starting process..." << endl;

    // Открываем семафор, созданный основным процессом
    HANDLE semaphore = OpenSemaphoreA(SEMAPHORE_ALL_ACCESS, FALSE, SEMAPHORE_NAME);
    if (semaphore == NULL) {
        cerr << "Failed to open semaphore. Error code: " << GetLastError() << endl;
        getchar();
        return 1;
    }
    else {
        cout << "Semaphore opened successfully." << endl;
    }

    string name = "OS06_04B";

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

    CloseHandle(semaphore);
    cout << "Process completed." << endl;
    return 0;
}
