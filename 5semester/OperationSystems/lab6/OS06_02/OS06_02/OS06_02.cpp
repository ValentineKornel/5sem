#include <iostream>
#include <thread>
#include <windows.h>
#include <chrono>
using namespace std;

CRITICAL_SECTION criticalSection;

void ThreadFunction(const string& name) {
    for (int i = 1; i <= 90; ++i) {
        if (i >= 30 && i <= 60) {
            EnterCriticalSection(&criticalSection);

            cout << "Thread: " << name << " - iteration: " << i << endl;
            Sleep(100);
            LeaveCriticalSection(&criticalSection);
        }
        else {
            cout << "Thread " << name << " - iteration: " << i << endl;
        }
        Sleep(100);
    }
}

int main() {
    InitializeCriticalSection(&criticalSection);

    thread threadA(ThreadFunction, "A");
    thread threadB(ThreadFunction, "B");

    ThreadFunction("Main");

    threadA.join();
    threadB.join();

    DeleteCriticalSection(&criticalSection);

    return 0;
}
