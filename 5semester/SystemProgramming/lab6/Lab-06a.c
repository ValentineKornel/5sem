#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>


HANDLE hFile;

void PrintInfo(LPSTR FileName);
void PrintText(LPSTR FileName);


int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Error. give the file name a parameter\n");
        return 1;
    }

    LPSTR FileName = argv[1];
    PrintInfo(FileName);
    PrintText(FileName);

    return 0;
}

void PrintInfo(LPSTR FilePath) {
    BY_HANDLE_FILE_INFORMATION bhfi; // ���������� � �����

    const char* fileName = strrchr(FilePath, '\\');
    if (fileName != NULL) {
        fileName++;
    }
    else {
        fileName = FilePath;
    }

    // ��������� ���� ��� ������
    hFile = CreateFileA(FilePath, // ��� �����
        0, // ��������� ���������� � �����
        0, // ����������� ������ � �����
        NULL, // ������ ���
        OPEN_EXISTING, // ��������� ������������ ����
        FILE_ATTRIBUTE_NORMAL, // ������� ����
        NULL                   // ������� ���
    );
    // ��������� �� �������� ��������
    if (hFile == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Create file failed.\nThe last error code: %ld\n",
            GetLastError());
        fputs("Press any key to finish.\n", stdout);
        getchar();

        return;
    }

    // �������� ���������� � �����
    if (!GetFileInformationByHandle(hFile, &bhfi)) {
        fprintf(
            stderr,
            "Get file information by handle failed.\nThe last error code: %ld\n",
            GetLastError());
        fputs("Press any key to finish.\n", stdout);
        getchar();

        return;
    }

    printf("File name: %s\n", fileName);

    // ������ �����
    LARGE_INTEGER fileSize;
    fileSize.LowPart = bhfi.nFileSizeLow;
    fileSize.HighPart = bhfi.nFileSizeHigh;
    printf("File size: %lld B (%.2f K, %.2f MB)\n",
        fileSize.QuadPart,
        fileSize.QuadPart / 1024.0,
        fileSize.QuadPart / (1024.0 * 1024.0));


    // ���������� ��� �����
    DWORD  dwFileType;
    dwFileType = GetFileType(hFile);
    // ������������� ��� �����
    switch (dwFileType)
    {
    case FILE_TYPE_UNKNOWN:
        fputs("Unknown type file.\n", stdout);
        break;
    case FILE_TYPE_DISK:
        fputs("Disk type file.\n", stdout);
        break;
    case FILE_TYPE_CHAR:
        fputs("Char type file.\n", stdout);
        break;
    case FILE_TYPE_PIPE:
        fputs("Pipe type file.\n", stdout);
        break;
    default:
        break;
    }

    // ����� ��������, ���������� ������� � ���������
    FILETIME ftCreate, ftAccess, ftWrite;
    SYSTEMTIME stUTC, stLocal;
    ftCreate = bhfi.ftCreationTime;
    ftAccess = bhfi.ftLastAccessTime;
    ftWrite = bhfi.ftLastWriteTime;

    // ����� ��������
    FileTimeToSystemTime(&ftCreate, &stUTC);
    SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
    printf("Creation time: %02d/%02d/%d %02d:%02d\n",
        stLocal.wDay, stLocal.wMonth, stLocal.wYear, stLocal.wHour, stLocal.wMinute);

    // ����� ���������� �������
    FileTimeToSystemTime(&ftAccess, &stUTC);
    SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
    printf("Last access time: %02d/%02d/%d %02d:%02d\n",
        stLocal.wDay, stLocal.wMonth, stLocal.wYear, stLocal.wHour, stLocal.wMinute);

    // ����� ���������� ���������
    FileTimeToSystemTime(&ftWrite, &stUTC);
    SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
    printf("Last change time: %02d/%02d/%d %02d:%02d\n",
        stLocal.wDay, stLocal.wMonth, stLocal.wYear, stLocal.wHour, stLocal.wMinute);

    // ��������, �������� �� ���� ��������
    DWORD dwBinaryType;
    if (!GetBinaryTypeA(FilePath, &dwBinaryType)) {
        fputs("The fiel is not binary type.\n", stdout);

        return;
    }

    // ������������� ��� �����
    switch (dwBinaryType) {
    case SCS_32BIT_BINARY:
        fputs("A 32-bit Windows-based application.\n", stdout);
        break;
    case SCS_64BIT_BINARY:
        fputs("A 64-bit Windows-based application.\n", stdout);
        break;
    case SCS_DOS_BINARY:
        fputs("An MS-DOS � based application .\n", stdout);
        break;
    case SCS_OS216_BINARY:
        fputs("A 16-bit OS/2-based application.\n", stdout);
        break;
    case SCS_PIF_BINARY:
        fputs("A PIF file that executes an MS-DOS � based application.\n", stdout);
        break;
    case SCS_POSIX_BINARY:
        fputs("A POSIX � based application.\n", stdout);
        break;
    case SCS_WOW_BINARY:
        fputs("A 16-bit Windows-based application.\n", stdout);
        break;
    default:
        fputs("Mistery application.\n", stdout);
        break;
    }

    CloseHandle(hFile);
}

void PrintText(LPSTR FilePath) {

    const char* dot = strrchr(FilePath, '.');

    if (!(dot && strcmp(dot, ".txt") == 0)) {
        fprintf(stderr,
            "Error printin the file data.\nThe file must have '.txt' extention");
        return;
    }

    // ��������� ���� ��� ������
    hFile = CreateFileA(
        FilePath,     // ��� �����
        GENERIC_READ,            // ������ �� �����
        0,                       // ����������� ������ � �����
        NULL,                    // ������ ��� 
        OPEN_EXISTING,           // ��������� ������������ ����
        FILE_ATTRIBUTE_NORMAL,   // ������� ����
        NULL        // ������� ���
    );
    // ��������� �� �������� ��������
    if (hFile == INVALID_HANDLE_VALUE)
    {
        fprintf(stderr, "Create file failed.\nThe last error code: %ld\n",
            GetLastError());
        fputs("Press any key to finish.\n", stdout);
        getchar();
        return;
    }

    // ������ ������ �� �����
    for (;;)
    {
        DWORD  dwBytesRead;
        char    n;

        // ������ ���� ������
        if (!ReadFile(
            hFile,               // ���������� �����
            &n,                  // ����� ������, ���� ������ ������
            sizeof(n),           // ���������� �������� ������
            &dwBytesRead,        // ���������� ����������� ������
            (LPOVERLAPPED)NULL   // ������ ����������
        ))
        {
            fprintf(stderr, "Read file failed.\nThe last error code: %ld\n", GetLastError());
            CloseHandle(hFile);
            fputs("Press any key to finish.\n", stdout);
            getchar();
            return;
        }
        // ��������� �� ����� �����
        if (dwBytesRead == 0)
            // ���� ��, �� ������� �� �����
            break;
        else
            // ����� ������� ������ �� �������
            fprintf(stdout, "%c", n);
    }

    fputs("\n", stdout);

    // ��������� ���������� ����� 
    CloseHandle(hFile);

    fputs("The file is opened and read.\n", stdout);
}
