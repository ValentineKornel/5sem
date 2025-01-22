#include <iostream>
#include <windows.h>
#include <string>

#define EVENT_NAME "Global\\MyEvent"

using namespace std;


int main() {
    HANDLE eventHandle = OpenEventA(EVENT_ALL_ACCESS, FALSE, EVENT_NAME);
    if (eventHandle == NULL) {
        cerr << "Failed to open event. Error code: " << GetLastError() << endl;
        return 1;
    }
    else {
        cout << "Event opened successfully." << endl;
    }

    string name = "OS06_05B";

    WaitForSingleObject(eventHandle, INFINITE);
    for (int i = 1; i <= 90; ++i) {
        cout << name << " - Iteration " << i << endl;
        Sleep(100);
    }

    CloseHandle(eventHandle);
    cout << "Process completed." << endl;
    return 0;
}
