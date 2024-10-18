#include <windows.h>
#include <conio.h>
#include <string.h>
#include <stdio.h>
#include <locale.h>
#pragma warning(disable : 4996)

#pragma comment(lib, "user32.lib")

#define BUFFER_SIZE 10

int
main(int argc, char* argv[])
{
    setlocale(LC_ALL, "Russian");

    char machineName[80];
    HANDLE hNamedPipe;
    DWORD dwBytesWrite; // ��� ���������� ���������� ������
    DWORD dwBytesRead; // ��� ���������� ����������� ������
    char pchMessage[BUFFER_SIZE]; // ��� ���������
    int nMessageLength;  // ����� ���������
    char pipe_name[80];
    char sem_name[80];
    char fullMessage[1024] = { 0 };

    if (argc < 1) {
        fprintf(stderr, "Usage: %s <string> [pipe name] [semaphore name]\n", argv[0]);
        return 1;
    }

    strncpy(fullMessage, argv[1], sizeof(fullMessage) - 1);
    //fullMessage[sizeof(fullMessage) - 1] = '\0';

    if (argc > 2) {
        strncpy(pipe_name, argv[2], sizeof(pipe_name) - 1);
        pipe_name[sizeof(pipe_name) - 1] = '\0';
    }
    else
        if (!GetEnvironmentVariableA("PIPE_NAME", pipe_name, sizeof(pipe_name))) {
            _cprintf("env error");
            return 1;
        }
    if (argc > 3) {
        strncpy(sem_name, argv[3], sizeof(sem_name) - 1);
        sem_name[sizeof(sem_name) - 1] = '\0';
    }
    else {
        if (!GetEnvironmentVariableA("SEMAPHORE_NAME", sem_name, sizeof(sem_name))) {
            _cprintf("env error");
            return 1;
        }
        _cprintf("sem name: %s\n", sem_name);
        //sem_name[sizeof(sem_name) - 1] = '\0';
    }




    // ��������� ����������� �������
    HANDLE semaphore = OpenSemaphoreA(
        SYNCHRONIZE | SEMAPHORE_MODIFY_STATE, FALSE, sem_name
    );
    if (semaphore == NULL) {
        _cprintf("OpenSemaphore failed: %d\n", GetLastError());
        return 1;
    }
    _cprintf("waiting for the semaphore...\n");
    // ������� �������
    WaitForSingleObject(semaphore, INFINITE);

    // ����������� � ����������� �������
    hNamedPipe =
        CreateFileA(pipe_name,                     // ��� ������
            GENERIC_READ | GENERIC_WRITE, // ������ � ���������� � �����
            FILE_SHARE_READ | FILE_SHARE_WRITE, // ��������� ������ � ������
            NULL, // ������������ �� ���������
            OPEN_EXISTING, // ��������� ������������ �����
            FILE_ATTRIBUTE_NORMAL, // �������� �� ���������
            NULL); // �������������� ��������� ���

    // ��������� ����� � �������
    if (hNamedPipe == INVALID_HANDLE_VALUE) {
        _cprintf("Connection with the named pipe failed.\n");
        _cprintf("The last error code: %d.\n", GetLastError());
        _cprintf("Press any key to exit.\n");
        _getch();

        return 0;
    }

    nMessageLength = strlen(fullMessage);
    int totalBytesSent = 0;

    while (totalBytesSent < nMessageLength) {
        int chunkSize = (nMessageLength - totalBytesSent > BUFFER_SIZE) ? BUFFER_SIZE : nMessageLength - totalBytesSent;
        if (!WriteFile(hNamedPipe, // ���������� ������
            fullMessage + totalBytesSent, // ����� ������ ��� ������ ������
            chunkSize, // ���������� ������������ ������
            &dwBytesWrite, // ���������� ���������� ������
            NULL // �������� ������ ����������
        )) {
            _cprintf("Write file failed.\n");
            _cprintf("The last error code: %d.\n", GetLastError());
            CloseHandle(hNamedPipe);
            CloseHandle(semaphore);
            _cprintf("Press any key to exit.\n");
            _getch();

            return 0;
        }

        totalBytesSent += chunkSize;
    }
    _cprintf("The client sent the message to a server: \n\t%s\n", fullMessage);

    int totalBytesRead = 0;
    memset(fullMessage, 0, sizeof(fullMessage));

    do {
        // ������ ��������� �� �������
        if (!ReadFile(hNamedPipe, // ���������� ������
            pchMessage, // ����� ������ ��� ����� ������
            sizeof(pchMessage), // ���������� �������� ������
            &dwBytesRead, // ���������� ����������� ������
            NULL)) // �������� ������ ����������
        {
            _cprintf("Data reading from the named pipe failed.\n");
            _cprintf("The last error code: %d.\n", GetLastError());
            CloseHandle(hNamedPipe);
            CloseHandle(semaphore);
            _cprintf("Press any key to exit.\n");
            _getch();

            return 0;
        }

        // �������� ����������� ����� � �������� ���������
        strncat(fullMessage, pchMessage, dwBytesRead);
        totalBytesRead += dwBytesRead;

    } while (dwBytesRead == BUFFER_SIZE);
    _cprintf("The client received the message from a server: \n\t%s\n",
        fullMessage);

    _cprintf("Press any key to exit.\n");
    _getch();

    CloseHandle(hNamedPipe);
    //Sleep(2000);
    ReleaseSemaphore(semaphore, 1, NULL);
    CloseHandle(semaphore);

    return 0;
}