#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <iostream>

using namespace std;

void ListRunningProcesses()
{
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        cout << "Failed to take a snapshot of processes." << endl;
        return;
    }

    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hProcessSnap, &pe32))
    {
        cout << "Failed to retrieve information about the first process." << endl;
        CloseHandle(hProcessSnap);
        return;
    }

    do
    {
        wcout << L"Process Name: " << pe32.szExeFile << L"\tProcess ID: " << pe32.th32ProcessID << endl;
    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
}

int main()
{
    cout << "List of running processes:" << endl;
    ListRunningProcesses();

    char a;
    cin >> a;
    return 0;
}
