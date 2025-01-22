#include <windows.h>
#include <stdio.h>
#include <conio.h>
#pragma warning(disable : 4996)

int main(int argc, char* argv[])
{
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <nubmer of process> <start> <end> [mutex name]\n", argv[0]);
        return 1;
    }

    char mutex_name[20];
    if (argv[4] != NULL) {
        strncpy(mutex_name, argv[4], sizeof(mutex_name) - 1);
        mutex_name[sizeof(mutex_name) - 1] = '\0';
    }
    else
        if (!GetEnvironmentVariableA("MUTEX_NAME", mutex_name, sizeof(mutex_name))) {
            _cprintf("env error");
            return 1;
        }

    HANDLE mutex = CreateMutexA(NULL, FALSE, mutex_name);
    if (mutex == NULL) {
        printf("Error createing mutex: %d\n", GetLastError());
        return 1;
    }
    HANDLE lpHandles[10];

    int numberOfProcess = atoi(argv[1]);
    int start = atoi(argv[2]);
    int end = atoi(argv[3]);

    int range = end - start + 1;
    int step = range / numberOfProcess;


    
    HANDLE hWritePipe, hReadPipe;

    // ������� ��������� �����
    if (!CreatePipe(
        &hReadPipe,    // ���������� ��� ������
        &hWritePipe,   // ���������� ��� ������
        NULL,          // �������� ������ �� ���������, � ���� ������ 
        // ����������� hReadPipe � hWritePipe �������������
        0))            // ������ ������ �� ���������

    {
        _cputs("Create pipe failed.\n");
        _cputs("Press any key to finish.\n");
        _getch();
        return GetLastError();
    }


    for (int i = 0; i < numberOfProcess; i++) {
        int local_start = start + i * step;
        int local_end = local_start + step - 1;
        if (i == numberOfProcess-1)
            local_end = end;
        printf("i: %d: range %d - %d\n", i, local_start, local_end);

        char lpszComLine[100];  // ��� ��������� ������

        HANDLE hInheritWritePipe;
        
        //������ ����������� �������� ����������� hWritePipe
        if (!DuplicateHandle(
            GetCurrentProcess(),   // ���������� �������� ��������
            hWritePipe,            // �������� ���������� ������
            GetCurrentProcess(),   // ���������� �������� ��������
            &hInheritWritePipe,    // ����� ���������� ������
            0,                     // ���� �������� ������������
            TRUE,                  // ����� ���������� �����������
            DUPLICATE_SAME_ACCESS))  // ������ �� ��������
        {
            _cputs("Duplicate handle failed.\n");
            _cputs("Press any key to finish.\n");
            _getch();
            return GetLastError();
        }

        STARTUPINFOA si;
        PROCESS_INFORMATION pi;
        // ������������� �������� ������ ��������
        ZeroMemory(&si, sizeof(STARTUPINFOA));
        si.cb = sizeof(STARTUPINFOA);
        si.dwFlags = STARTF_USESTDHANDLES;
        si.hStdOutput = hInheritWritePipe;

        // ��������� ��������� ������
        sprintf_s(lpszComLine, 80, "Lab-03a-client.exe %d %d %s", local_start, local_end, mutex_name);
        printf("%s\n", lpszComLine);
        // ��������� ����� ���������� �������
        if (!CreateProcessA(
            NULL,    // ��� ��������
            lpszComLine,   // ��������� ������
            NULL,    // �������� ������ �������� �� ���������
            NULL,    // �������� ������ ���������� ������ �� ���������
            TRUE,    // ����������� ����������� �������� ��������
            // ����������� ����� ���������
            CREATE_NEW_CONSOLE,  // ����� �������  
            NULL,    // ���������� ����� ��������� �������� ������
            NULL,    // ������� ���� � �������, ��� � � �������� ������
            &si,     // ��� �������� ���� - �� ���������
            &pi      // ����� ����� ����������� � ��������������
            // ������ �������� � ��� ���������� ������
        ))
        {
            _cputs("Create process failed.\n");
            _cputs("Press any key to finish.\n");
            _getch();
            return GetLastError();
        }
        lpHandles[i] = pi.hProcess;
        // ��������� ����������� ������ ��������
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        // ��������� �������� ���������� ������
        CloseHandle(hInheritWritePipe);
    }


    // ������ �� ���������� ������
    for (int i = 0; i < numberOfProcess; i++)
    {
        char nData[250];
        DWORD dwBytesRead;
        if (!ReadFile(
            hReadPipe,
            &nData,
            sizeof(nData),
            &dwBytesRead,
            NULL))
        {
            _cputs("Read from the pipe failed.\n");
            _cputs("Press any key to finish.\n");
            _getch();
            return GetLastError();
        }
        _cprintf("The data read from the pipe: %s\n", nData);
    }

    WaitForMultipleObjects(numberOfProcess, lpHandles, TRUE, INFINITE);
    _cputs("Press any key to exit.\n");
    _getch();
    CloseHandle(hWritePipe);
    CloseHandle(hReadPipe);
    CloseHandle(mutex);
    return 0;
    
}