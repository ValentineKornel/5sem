#include <windows.h>
#include <iostream>
#include <bitset>
using namespace std;

int main() {
    cout << "Korneliuk Valentine" << endl;
    // ��������� �������������� �������� ��������
    DWORD processID = GetCurrentProcessId();
    cout << "Process ID: " << processID << std::endl;

    // ��������� �������������� �������� ������
    DWORD threadID = GetCurrentThreadId();
    cout << "Thread ID: " << threadID << std::endl;

    // ��������� ������������� ������ ��������
    HANDLE processHandle = GetCurrentProcess();
    DWORD priorityClass = GetPriorityClass(processHandle);
    cout << "Process Priority Class: ";
    switch (priorityClass) {
    case IDLE_PRIORITY_CLASS:
        std::cout << "IDLE_PRIORITY_CLASS" << std::endl;
        break;
    case BELOW_NORMAL_PRIORITY_CLASS:
        std::cout << "BELOW_NORMAL_PRIORITY_CLASS" << std::endl;
        break;
    case NORMAL_PRIORITY_CLASS:
        std::cout << "NORMAL_PRIORITY_CLASS" << std::endl;
        break;
    case ABOVE_NORMAL_PRIORITY_CLASS:
        std::cout << "ABOVE_NORMAL_PRIORITY_CLASS" << std::endl;
        break;
    case HIGH_PRIORITY_CLASS:
        std::cout << "HIGH_PRIORITY_CLASS" << std::endl;
        break;
    case REALTIME_PRIORITY_CLASS:
        std::cout << "REALTIME_PRIORITY_CLASS" << std::endl;
        break;
    default:
        std::cout << "Unknown Priority Class" << std::endl;
    }

    // ��������� ���������� �������� ������
    HANDLE threadHandle = GetCurrentThread();
    int threadPriority = GetThreadPriority(threadHandle);
    cout << "Thread Priority: " << threadPriority << std::endl;

    // ��������� ����� ��������� ���������� �����������
    DWORD_PTR processAffinityMask, systemAffinityMask;
    if (GetProcessAffinityMask(processHandle, &processAffinityMask, &systemAffinityMask)) {
        cout << "Process Affinity Mask (binary): " << std::bitset<64>(processAffinityMask) << std::endl;

        // ������� ���������� ��������� �����������
        DWORD processorCount = 0;
        for (DWORD_PTR mask = processAffinityMask; mask != 0; mask >>= 1) {
            processorCount += (mask & 1);
        }
        cout << "Available processors: " << processorCount << std::endl;
    }
    else {
        cerr << "Failed to get process affinity mask" << std::endl;
    }

    // ��������� ������ ����������, �� ������� ����������� ������� �����
    DWORD currentProcessorNumber = GetCurrentProcessorNumber();
    cout << "Current Processor Number: " << currentProcessorNumber << std::endl;

    // �������� ������������
    CloseHandle(processHandle);
    CloseHandle(threadHandle);

    return 0;
}
