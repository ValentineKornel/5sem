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
    BY_HANDLE_FILE_INFORMATION bhfi; // информация о файле

    const char* fileName = strrchr(FilePath, '\\');
    if (fileName != NULL) {
        fileName++;
    }
    else {
        fileName = FilePath;
    }

    // открываем файл для чтения
    hFile = CreateFileA(FilePath, // имя файла
        0, // получение информации о файле
        0, // монопольный доступ к файлу
        NULL, // защиты нет
        OPEN_EXISTING, // открываем существующий файл
        FILE_ATTRIBUTE_NORMAL, // обычный файл
        NULL                   // шаблона нет
    );
    // проверяем на успешное открытие
    if (hFile == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Create file failed.\nThe last error code: %ld\n",
            GetLastError());
        fputs("Press any key to finish.\n", stdout);
        getchar();

        return;
    }

    // получаем информацию о файле
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

    // Размер файла
    LARGE_INTEGER fileSize;
    fileSize.LowPart = bhfi.nFileSizeLow;
    fileSize.HighPart = bhfi.nFileSizeHigh;
    printf("File size: %lld B (%.2f K, %.2f MB)\n",
        fileSize.QuadPart,
        fileSize.QuadPart / 1024.0,
        fileSize.QuadPart / (1024.0 * 1024.0));


    // определяем тип файла
    DWORD  dwFileType;
    dwFileType = GetFileType(hFile);
    // распечатываем тип файла
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

    // Время создания, последнего доступа и изменения
    FILETIME ftCreate, ftAccess, ftWrite;
    SYSTEMTIME stUTC, stLocal;
    ftCreate = bhfi.ftCreationTime;
    ftAccess = bhfi.ftLastAccessTime;
    ftWrite = bhfi.ftLastWriteTime;

    // Время создания
    FileTimeToSystemTime(&ftCreate, &stUTC);
    SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
    printf("Creation time: %02d/%02d/%d %02d:%02d\n",
        stLocal.wDay, stLocal.wMonth, stLocal.wYear, stLocal.wHour, stLocal.wMinute);

    // Время последнего доступа
    FileTimeToSystemTime(&ftAccess, &stUTC);
    SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
    printf("Last access time: %02d/%02d/%d %02d:%02d\n",
        stLocal.wDay, stLocal.wMonth, stLocal.wYear, stLocal.wHour, stLocal.wMinute);

    // Время последнего изменения
    FileTimeToSystemTime(&ftWrite, &stUTC);
    SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
    printf("Last change time: %02d/%02d/%d %02d:%02d\n",
        stLocal.wDay, stLocal.wMonth, stLocal.wYear, stLocal.wHour, stLocal.wMinute);

    // Проверка, является ли файл бинарным
    DWORD dwBinaryType;
    if (!GetBinaryTypeA(FilePath, &dwBinaryType)) {
        fputs("The fiel is not binary type.\n", stdout);

        return;
    }

    // распечатываем тип файла
    switch (dwBinaryType) {
    case SCS_32BIT_BINARY:
        fputs("A 32-bit Windows-based application.\n", stdout);
        break;
    case SCS_64BIT_BINARY:
        fputs("A 64-bit Windows-based application.\n", stdout);
        break;
    case SCS_DOS_BINARY:
        fputs("An MS-DOS – based application .\n", stdout);
        break;
    case SCS_OS216_BINARY:
        fputs("A 16-bit OS/2-based application.\n", stdout);
        break;
    case SCS_PIF_BINARY:
        fputs("A PIF file that executes an MS-DOS – based application.\n", stdout);
        break;
    case SCS_POSIX_BINARY:
        fputs("A POSIX – based application.\n", stdout);
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

    // открываем файл для чтения
    hFile = CreateFileA(
        FilePath,     // имя файла
        GENERIC_READ,            // чтение из файла
        0,                       // монопольный доступ к файлу
        NULL,                    // защиты нет 
        OPEN_EXISTING,           // открываем существующий файл
        FILE_ATTRIBUTE_NORMAL,   // обычный файл
        NULL        // шаблона нет
    );
    // проверяем на успешное открытие
    if (hFile == INVALID_HANDLE_VALUE)
    {
        fprintf(stderr, "Create file failed.\nThe last error code: %ld\n",
            GetLastError());
        fputs("Press any key to finish.\n", stdout);
        getchar();
        return;
    }

    // читаем данные из файла
    for (;;)
    {
        DWORD  dwBytesRead;
        char    n;

        // читаем одну запись
        if (!ReadFile(
            hFile,               // дескриптор файла
            &n,                  // адрес буфера, куда читаем данные
            sizeof(n),           // количество читаемых байтов
            &dwBytesRead,        // количество прочитанных байтов
            (LPOVERLAPPED)NULL   // чтение синхронное
        ))
        {
            fprintf(stderr, "Read file failed.\nThe last error code: %ld\n", GetLastError());
            CloseHandle(hFile);
            fputs("Press any key to finish.\n", stdout);
            getchar();
            return;
        }
        // проверяем на конец файла
        if (dwBytesRead == 0)
            // если да, то выходим из цикла
            break;
        else
            // иначе выводим запись на консоль
            fprintf(stdout, "%c", n);
    }

    fputs("\n", stdout);

    // закрываем дескриптор файла 
    CloseHandle(hFile);

    fputs("The file is opened and read.\n", stdout);
}
