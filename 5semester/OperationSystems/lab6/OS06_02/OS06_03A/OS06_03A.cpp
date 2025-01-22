#include <iostream>
#include <windows.h>
#include <string>
#include <chrono>
#include <thread>

#define MUTEX_NAME "Global\\MyMutex"
using namespace std;

int main() {
    string name = "A";
    HANDLE mutex = OpenMutexA(SYNCHRONIZE, FALSE, MUTEX_NAME);
    if (mutex == NULL) {
        cerr << "Failed to open mutex." << endl;
        return 1;
    }

    for (int i = 1; i <= 90; ++i) {
        if (i >= 30 && i <= 60) {
            WaitForSingleObject(mutex, INFINITE);
            cout << "Process " << name << " - iteration: " << i << endl;
            ReleaseMutex(mutex);
        }
        else {
            cout << "Process " << name << " - iteration: " << i << endl;
        }
        Sleep(100);
    }

    CloseHandle(mutex);
    return 0;
}
