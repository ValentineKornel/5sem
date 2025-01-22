#include <stdio.h>
#include <Windows.h>
#include <tlhelp32.h>
#include <string.h>
#include <errors.h>

HANDLE ProcessEnumerateAndSearch(PWCHAR ProcessName)
{
    HANDLE hSnapshot;
    HANDLE hProcess = NULL;
    PROCESSENTRY32W pe = { .dwSize = sizeof(PROCESSENTRY32W) };

    ;
    if ((hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)) == INVALID_HANDLE_VALUE)
    {
        printf("[X] CreateToolhelp32Snapshot has failed with error %lu", GetLastError());
    }

    if (Process32FirstW(hSnapshot, &pe) == FALSE)
    {
        printf("[X] Process32First has failed with error %lu", GetLastError());
    }

    do {

        if (wcscmp(pe.szExeFile, ProcessName) == 0)
        {

            if ((hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe.th32ProcessID)) == NULL)
            {
                printf("[X] OpenProcess has failed with error %lu", GetLastError());
                break;
            }
            else
            {
                printf("Process PID: %lu has been opened", pe.th32ProcessID);
                break;
            }
        }

    } while (Process32NextW(hSnapshot, &pe));
    return hProcess;
}


BOOL InjectDLL(PCWCHAR dllName, SIZE_T szDllName, HANDLE hTargetProc)
{
    HMODULE hK32Module;
    FARPROC hLoadLibrary, hFreeLibrary;
    LPVOID pLibAddr;
    SIZE_T szWrittenBytes;
    HANDLE hThread;
    HMODULE  pRemoteThreadRetVal;
    

    // Locating the LoadLibrary DLL

    hK32Module = GetModuleHandleW(L"kernel32.dll");
    hLoadLibrary = GetProcAddress(hK32Module, "LoadLibraryW");

    if (hLoadLibrary == NULL)
    {
        printf("Error while loading LoadLibraryW: %lu\n", GetLastError());
        return FALSE;
    }

    // Allocating memory into the target process
    pLibAddr = VirtualAllocEx(hTargetProc, NULL, szDllName, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (pLibAddr == NULL)
    {
        printf("Error in VirtualAllocEX: %lu\n", GetLastError());
        return FALSE;
    }
    
    // Writing into the target process the dll payload name
    if (!WriteProcessMemory(hTargetProc, pLibAddr, dllName, szDllName, &szWrittenBytes))
    {
        printf("Error in WriteProcessMemory: %lu\n", GetLastError());
        return FALSE;
    }
    
    // Run a thread into the target process that will load the payload through LoadLibraryW

    hThread = CreateRemoteThread(hTargetProc, NULL, 0, (LPTHREAD_START_ROUTINE)hLoadLibrary, pLibAddr, 0, NULL);

    if (hThread == NULL)
    {
        printf("Error in CreateRemoteThread: %lu\n", GetLastError());
        return FALSE;
    }

    WaitForSingleObject(hThread, INFINITE);
    if (!GetExitCodeThread(hThread, (LPDWORD)&pRemoteThreadRetVal)) {
        printf("Error retrieving thread exit code: %lu\n", GetLastError());
        return FALSE;
    }

    CloseHandle(hThread);



    hFreeLibrary = GetProcAddress(hK32Module, "FreeLibrary");
    if (hFreeLibrary == NULL)
    {
        printf("Error while loading FreeLibrary: %lu\n", GetLastError());
        return FALSE;
    }

    hThread = CreateRemoteThread(hTargetProc, NULL, 0, (LPTHREAD_START_ROUTINE)hFreeLibrary, (LPVOID)pRemoteThreadRetVal, 0, NULL);
    if (hThread == NULL)
    {
        printf("Error in CreateRemoteThread: %lu\n", GetLastError());
        return FALSE;
    }

    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);

    if (VirtualFreeEx(hTargetProc, pLibAddr, 0, MEM_RELEASE) == 0) {
        printf("Error in VirtualFreeEx: %lu\n", GetLastError());
        return FALSE;
    }



    return TRUE;
}

int main()
{
    HANDLE hTarget;
    LPCWSTR strLibName = L"D:\\Study\\university\\5semester\\SP\\lab8\\dll-cmake - Copy\\build\\Debug\\mylib.dll";

    hTarget = ProcessEnumerateAndSearch(L"Lab-08bVictim.exe");
    
    if (!InjectDLL(strLibName, (wcslen(strLibName) + 1) * sizeof(WCHAR), hTarget))
        return -1;
    return 0;

}