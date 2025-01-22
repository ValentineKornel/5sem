#include <Windows.h>
#include <handleapi.h>
#include <stdio.h>
#include <winnt.h>
#pragma warning(disable : 4996)

char *service_name = "Service09"; // ��� �������
char log_file[] = "C:\\logs\\ServiceFile.log";

const char* watch_directory;
const char* log_file_directory;
char dir_log_file[50];
char srv_log_file[50];

SERVICE_STATUS service_status;
SERVICE_STATUS prev_service_status;
SERVICE_STATUS_HANDLE hServiceStatus;

VOID WINAPI ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);
VOID WINAPI ServiceCtrlHandler(DWORD dwControl);

HANDLE hDirLogOut;
HANDLE hSrvLogOut;
HANDLE hWatchDir;
HANDLE hChangeHandle;
HANDLE hStopEvent;
int nCount;  // �������

// ������� ������� ����������
int main() {
    //  �������������� ��������� ��������
    SERVICE_TABLE_ENTRY service_table[] = {
        {service_name, ServiceMain}, // ��� ������� � ������� �������
        {NULL, NULL}                 // ������ �������� ���
    };

    // ��������� ��������� �������
    if (!StartServiceCtrlDispatcher(service_table)) {
        /*hOut = CreateFileA(log_file, GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS,
            FILE_ATTRIBUTE_NORMAL, NULL);
        WriteFile(hOut, "Start service control dispatcher failed.\n", 42, NULL,
            NULL);
        CloseHandle(hOut);*/

        return 1;
    }

    return 0;
}

VOID WINAPI ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv) {
    if (dwArgc < 3) {
        return;
    }
    // ������������ ���������� ����������� ������ ��� �������
    hServiceStatus = RegisterServiceCtrlHandler(
        service_name,      // ��� �������
        ServiceCtrlHandler // ���������� ����������� ������
    );
    if (!hServiceStatus) {
        /*hOut = CreateFileA(log_file, FILE_APPEND_DATA,
            FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS,
            FILE_ATTRIBUTE_NORMAL, NULL);
        SetFilePointer(hOut, 0l, 0l, FILE_END);
        WriteFile(hOut, "Register service control handler failed.\n", 42, NULL,
            NULL);
        CloseHandle(hOut);*/
        return;
    }

    // �������������� ��������� ��������� �������
    service_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    service_status.dwCurrentState = SERVICE_START_PENDING;
    service_status.dwControlsAccepted =
        SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_PAUSE_CONTINUE;
    service_status.dwWin32ExitCode = ERROR_SERVICE_SPECIFIC_ERROR;
    service_status.dwServiceSpecificExitCode = 0;
    service_status.dwCheckPoint = 0;
    service_status.dwWaitHint = 5000;

    // ������������� ��������� �������
    if (!SetServiceStatus(hServiceStatus, &service_status)) {
        /*hOut = CreateFileA(log_file, FILE_APPEND_DATA,
            FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS,
            FILE_ATTRIBUTE_NORMAL, NULL);
        SetFilePointer(hOut, 0l, 0l, FILE_END);
        WriteFile(hOut, "Set service status 'SERVICE_START_PENDING' failed.\n", 52,
            NULL, NULL);
        CloseHandle(hOut);*/
        return;
    }

    log_file_directory = lpszArgv[2];
    watch_directory = lpszArgv[1];
    BOOL created = CreateDirectoryA(log_file_directory, NULL);
    DWORD error = GetLastError();

    if (!created && error != ERROR_ALREADY_EXISTS) {
        // ���� �� ������� ������� ���������� �� ���� �������
        //fprintf(stdout, "Failed to create log directory: %s\n", srv_log_file_directory);
        return;
    }

    SYSTEMTIME st;
    GetLocalTime(&st);
    char timestamp[32];

    char srv_log_file_path[MAX_PATH];
    snprintf(srv_log_file_path, sizeof(srv_log_file_path),
        "%s\\%04d-%02d-%02dT%02d-%02d-%02d-srv.log",
        log_file_directory,
        st.wYear, st.wMonth, st.wDay,
        st.wHour, st.wMinute, st.wSecond);

    DWORD length;
    char *buffer;
    if (created) {
        // ���������� ���� �������
        hSrvLogOut = CreateFileA(srv_log_file_path, FILE_APPEND_DATA,
            FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS,
            FILE_ATTRIBUTE_NORMAL, NULL);
        SetFilePointer(hSrvLogOut, 0l, 0l, FILE_END);
        GetLocalTime(&st);
        sprintf(timestamp, "[%02d:%02d:%02d]", st.wHour, st.wMinute, st.wSecond);
        length = strlen(timestamp) + strlen(service_name) + strlen(log_file_directory) + 30;
        buffer = malloc(length);
        memset(buffer, 0, length);
        sprintf(buffer, "%s ����� %s ������ ������� %s\n", timestamp, service_name, log_file_directory);
        WriteFile(hSrvLogOut, buffer, strlen(buffer), NULL, NULL);
        free(buffer);
    }
    else {
        hSrvLogOut = CreateFileA(srv_log_file_path, FILE_APPEND_DATA,
            FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS,
            FILE_ATTRIBUTE_NORMAL, NULL);
        SetFilePointer(hSrvLogOut, 0l, 0l, FILE_END);
        GetLocalTime(&st);
        sprintf(timestamp, "[%02d:%02d:%02d]", st.wHour, st.wMinute, st.wSecond);
        length = strlen(timestamp) + strlen(service_name) + strlen(log_file_directory) + 30;
        buffer = malloc(length);
        memset(buffer, 0, length);
        sprintf(buffer, "%s �����! %s ��������� ������� %s\n", timestamp, service_name, log_file_directory);
        WriteFile(hSrvLogOut, buffer, strlen(buffer), NULL, NULL);
        free(buffer);
    }

    watch_directory = lpszArgv[1];
    hWatchDir = CreateFileA(
        watch_directory,
        FILE_LIST_DIRECTORY,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
        NULL
    );
    if (hWatchDir == INVALID_HANDLE_VALUE) {
        GetLocalTime(&st);
        sprintf(timestamp, "[%02d:%02d:%02d]", st.wHour, st.wMinute, st.wSecond);
        length = strlen(timestamp) + strlen(service_name) + strlen(log_file_directory) + 50;
        buffer = malloc(length);
        memset(buffer, 0, length);
        sprintf(buffer, "%s ������� ������� ��������� %s �� ���� ���������� ������� %s\n", timestamp, service_name, watch_directory);
        WriteFile(hSrvLogOut, buffer, strlen(buffer), NULL, NULL);
        free(buffer);
        return;
    }

    snprintf(srv_log_file_path, sizeof(srv_log_file_path),
        "%s\\%04d-%02d-%02dT%02d-%02d-%02d-dir.log",
        log_file_directory,
        st.wYear, st.wMonth, st.wDay,
        st.wHour, st.wMinute, st.wSecond);

    hDirLogOut = CreateFileA(srv_log_file_path, FILE_APPEND_DATA,
        FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL, NULL);
    if (hDirLogOut == INVALID_HANDLE_VALUE) {
        GetLocalTime(&st);
        sprintf(timestamp, "[%02d:%02d:%02d]", st.wHour, st.wMinute, st.wSecond);
        length = strlen(timestamp) + strlen(service_name) + 70;
        buffer = malloc(length);
        memset(buffer, 0, length);
        sprintf(buffer, "%s ������� ������� ��������� %s �� ���� ������� ���� ��� ����������� ��������� � ��������\n", timestamp, service_name);
        WriteFile(hSrvLogOut, buffer, strlen(buffer), NULL, NULL);
        free(buffer);
        return;
    }

    // ���������� ������ ��� ����������
    service_status.dwCurrentState = SERVICE_RUNNING;
    service_status.dwWin32ExitCode = NO_ERROR;
    // ������������� ����� ��������� �������
    if (!SetServiceStatus(hServiceStatus, &service_status)) {
        SetFilePointer(hSrvLogOut, 0l, 0l, FILE_END);
        WriteFile(hSrvLogOut, "Set service status 'START_RUNNING' failed.\n", 44, NULL,
            NULL);
        CloseHandle(hSrvLogOut);
        return;
    }

    SetFilePointer(hSrvLogOut, 0l, 0l, FILE_END);
    GetLocalTime(&st);
    sprintf(timestamp, "[%02d:%02d:%02d]", st.wHour, st.wMinute, st.wSecond);
    length = strlen(timestamp) + strlen(lpszArgv[1]) + strlen(lpszArgv[2]) + 50;
    buffer = malloc(length);
    memset(buffer, 0, length);
    sprintf(buffer, "%s �����! ������ ������� � ����������� %s %s\n", timestamp, lpszArgv[1], lpszArgv[2]);
    WriteFile(hSrvLogOut, buffer, strlen(buffer), NULL, NULL);
    free(buffer);


    // ������� ���� �������
    hChangeHandle = FindFirstChangeNotificationA(
        watch_directory,
        TRUE, // ����������� �����������
        FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_SIZE | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_ATTRIBUTES
        | FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_SECURITY
    );

    if (hChangeHandle == INVALID_HANDLE_VALUE) {
        GetLocalTime(&st);
        sprintf(timestamp, "[%02d:%02d:%02d]", st.wHour, st.wMinute, st.wSecond);
        length = strlen(timestamp) + strlen(service_name) + 70;
        buffer = malloc(length);
        memset(buffer, 0, length);
        sprintf(buffer, "%s ������ %s �� ���� ���������������� change notification. Code: %ld\n", timestamp, service_name, GetLastError());
        WriteFile(hSrvLogOut, buffer, strlen(buffer), NULL, NULL);
        free(buffer);
        return;
    }

    char buff[1024];
    DWORD bytesReturned;
    OVERLAPPED overlapped = { 0 }; // ��������� ��� ����������� ��������
    HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL); // ������� ��� �������������
    if (!hEvent) {
        printf("Failed to create event. Error: %ld\n", GetLastError());
        return 1;
    }
    overlapped.hEvent = hEvent;


    while (service_status.dwCurrentState != SERVICE_STOPPED) {

        if (service_status.dwCurrentState == SERVICE_PAUSED) {
            Sleep(100);
            continue;
        }

        DWORD waitStatus = WaitForSingleObject(hChangeHandle, 100);

        if (waitStatus == WAIT_OBJECT_0) {
            // �������� ������ ���������
            BOOL result = ReadDirectoryChangesW(
                hWatchDir,
                buff,
                sizeof(buff),
                TRUE, // ����������� �����������
                FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME |
                FILE_NOTIFY_CHANGE_ATTRIBUTES | FILE_NOTIFY_CHANGE_SIZE |
                FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_CREATION,
                &bytesReturned,
                &overlapped, // �������� OVERLAPPED
                NULL
            );
            if (!result && GetLastError() != ERROR_IO_PENDING) {
                //printf("Failed to read directory changes. Error: %ld\n", GetLastError());
                CloseHandle(hEvent);
                return 1;
            }

            DWORD waitReadStatus = WaitForSingleObject(hEvent, 100);
            if (waitReadStatus == WAIT_OBJECT_0) {
                FILE_NOTIFY_INFORMATION* notify = (FILE_NOTIFY_INFORMATION*)buff;

                do {
                    char actionDescription[50];
                    switch (notify->Action) {
                    case FILE_ACTION_ADDED:
                        strcpy(actionDescription, "File added: ");
                        break;
                    case FILE_ACTION_REMOVED:
                        strcpy(actionDescription, "File removed: ");
                        break;
                    case FILE_ACTION_MODIFIED:
                        strcpy(actionDescription, "File modified: ");
                        break;
                    case FILE_ACTION_RENAMED_OLD_NAME:
                        strcpy(actionDescription, "File renamed from: ");
                        break;
                    case FILE_ACTION_RENAMED_NEW_NAME:
                        strcpy(actionDescription, "File renamed to: ");
                        break;
                    default:
                        strcpy(actionDescription, "Unknown action: ");
                    }

                    char logEntry[200];
                    snprintf(logEntry, sizeof(logEntry), "%s%.*S",
                        actionDescription, notify->FileNameLength / sizeof(WCHAR), notify->FileName);

                    GetLocalTime(&st);
                    sprintf(timestamp, "[%02d:%02d:%02d]", st.wHour, st.wMinute, st.wSecond);
                    length = strlen(timestamp) + strlen(logEntry) + 5;
                    buffer = malloc(length);
                    memset(buffer, 0, length);
                    sprintf(buffer, "%s %s\n", timestamp, logEntry);
                    WriteFile(hDirLogOut, buffer, strlen(buffer), NULL, NULL);
                    free(buffer);

                    notify = notify->NextEntryOffset
                        ? (FILE_NOTIFY_INFORMATION*)((BYTE*)notify + notify->NextEntryOffset)
                        : NULL;
                } while (notify);
            }
            else if (waitReadStatus == WAIT_TIMEOUT) {
                continue;
            }

            // �������� �����������, ����� ���������� ������������
            if (!FindNextChangeNotification(hChangeHandle)) {
                GetLocalTime(&st);
                sprintf(timestamp, "[%02d:%02d:%02d]", st.wHour, st.wMinute, st.wSecond);
                length = strlen(timestamp) + strlen(service_name) + 70;
                buffer = malloc(length);
                memset(buffer, 0, length);
                sprintf(buffer, "%s ������ %s �� ���� reset notification. Code: %ld\n", timestamp, service_name, GetLastError());
                WriteFile(hSrvLogOut, buffer, strlen(buffer), NULL, NULL);
                free(buffer);
                return;
            }
        }
        else if (waitStatus == WAIT_TIMEOUT) {
            continue;
        }
        else {
            fprintf(stderr, "Error: Wait failed. Code: %ld\n", GetLastError());
            GetLocalTime(&st);
            sprintf(timestamp, "[%02d:%02d:%02d]", st.wHour, st.wMinute, st.wSecond);
            length = strlen(timestamp) + strlen(service_name) + 70;
            buffer = malloc(length);
            memset(buffer, 0, length);
            sprintf(buffer, "%s ������ %s: Wait failed. Code: %ld\n", timestamp, service_name, GetLastError());
            WriteFile(hSrvLogOut, buffer, strlen(buffer), NULL, NULL);
            free(buffer);
            return;
            break;
        }

    }
}

VOID WINAPI ServiceCtrlHandler(DWORD dwControl) {
    SYSTEMTIME st;
    char timestamp[32];
    DWORD length;
    char* buffer;
    prev_service_status.dwCurrentState = service_status.dwCurrentState;
    switch (dwControl) {
    case SERVICE_CONTROL_STOP: // ���������� ������
        SetFilePointer(hSrvLogOut, 0l, 0l, FILE_END);
        GetLocalTime(&st);
        sprintf(timestamp, "[%02d:%02d:%02d]", st.wHour, st.wMinute, st.wSecond);
        length = strlen(timestamp) + strlen(service_name) + strlen(log_file_directory) + 30;
        buffer = malloc(length);
        memset(buffer, 0, length);
        sprintf(buffer, "%s �����! %s ���������\n", timestamp, service_name);
        WriteFile(hSrvLogOut, buffer, strlen(buffer), NULL, NULL);
        free(buffer);
        // ������������� ��������� ���������
        service_status.dwCurrentState = SERVICE_STOPPED;
        SetServiceStatus(hServiceStatus, &service_status);
        FindCloseChangeNotification(hChangeHandle);
        CloseHandle(hSrvLogOut);
        CloseHandle(hWatchDir);
        CloseHandle(hDirLogOut);
        break;

    case SERVICE_CONTROL_PAUSE:
        service_status.dwCurrentState = SERVICE_PAUSED;
        SetFilePointer(hSrvLogOut, 0l, 0l, FILE_END);
        GetLocalTime(&st);
        sprintf(timestamp, "[%02d:%02d:%02d]", st.wHour, st.wMinute, st.wSecond);
        length = strlen(timestamp) + strlen(service_name) + strlen(log_file_directory) + 30;
        buffer = malloc(length);
        memset(buffer, 0, length);
        sprintf(buffer, "%s �����! %s ������ ��������� � RUNNING �� PAUSED\n", timestamp, service_name);
        WriteFile(hSrvLogOut, buffer, strlen(buffer), NULL, NULL);
        free(buffer);
        break;
        
    case SERVICE_CONTROL_CONTINUE:
        service_status.dwCurrentState = SERVICE_RUNNING;
        SetFilePointer(hSrvLogOut, 0l, 0l, FILE_END);
        GetLocalTime(&st);
        sprintf(timestamp, "[%02d:%02d:%02d]", st.wHour, st.wMinute, st.wSecond);
        length = strlen(timestamp) + strlen(service_name) + strlen(log_file_directory) + 30;
        buffer = malloc(length);
        memset(buffer, 0, length);
        sprintf(buffer, "%s �����! %s ������ ��������� � PAUSED �� RUNNING\n", timestamp, service_name);
        WriteFile(hSrvLogOut, buffer, strlen(buffer), NULL, NULL);
        free(buffer);

        break;

    default:
        // ����������� �������� ����������� �����
        ++service_status.dwCheckPoint;
        // ��������� ��������� ������� ��� ���������
        break;
    }
    // �������� ��������� �������
    SetServiceStatus(hServiceStatus, &service_status);
    return;
}