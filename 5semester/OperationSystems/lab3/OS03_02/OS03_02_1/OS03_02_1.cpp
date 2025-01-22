#include <iostream>
#include <thread>
#include <windows.h>
#include <chrono>
using namespace std;


int main()
{
    DWORD processId = GetCurrentProcessId();

    for (int i = 1; i <= 50; i++) {
        cout << "Process ID: " << processId << ", Iteration: " << i << endl;

        this_thread::sleep_for(chrono::seconds(1));
    }
}
