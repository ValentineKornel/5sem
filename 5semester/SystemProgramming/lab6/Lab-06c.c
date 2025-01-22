#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

void ListDirectoryContents(const char* path) {
    WIN32_FIND_DATAA findFileData;
    HANDLE hFind;
    char searchPath[MAX_PATH];

    snprintf(searchPath, MAX_PATH, "%s\\*", path);

    hFind = FindFirstFileA(searchPath, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Error: Directory not found or access denied.\n");
        exit(1);
    }

    do {
        // Пропускаем текущий и родительский каталоги (".", "..")
        if (strcmp(findFileData.cFileName, ".") == 0 || strcmp(findFileData.cFileName, "..") == 0) {
            continue;
        }

        // Определяем, является ли текущая запись каталогом или файлом
        const char* type = (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? "Directory" : "File";
        printf("%-12s %s\\%s\n", type, path, findFileData.cFileName);

        // Если это каталог, вызываем функцию рекурсивно
        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            char subPath[MAX_PATH];
            snprintf(subPath, MAX_PATH, "%s\\%s", path, findFileData.cFileName);
            ListDirectoryContents(subPath);
        }
    } while (FindNextFileA(hFind, &findFileData) != 0);

    FindClose(hFind);
}

void WatchDirectory(const char* path) {
    HANDLE hDir = CreateFileA(
        path,
        FILE_LIST_DIRECTORY,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS,
        NULL
    );

    if (hDir == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Error: Could not open directory. Code: %ld\n", GetLastError());
        return;
    }

    HANDLE hChangeHandle = FindFirstChangeNotification(
        path,
        TRUE, // Отслеживать подкаталоги
        FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_SIZE | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_ATTRIBUTES 
        | FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_SECURITY
    );

    if (hChangeHandle == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Error: Could not initialize change notification. Code: %ld\n", GetLastError());
        return;
    }

    char buffer[1024];
    DWORD bytesReturned;

    while (1) {
        DWORD waitStatus = WaitForSingleObject(hChangeHandle, INFINITE);

        if (waitStatus == WAIT_OBJECT_0) {
            // Получить детали изменений
            if (ReadDirectoryChangesW(
                hDir,
                buffer,
                sizeof(buffer),
                TRUE,
                FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME |
                FILE_NOTIFY_CHANGE_ATTRIBUTES | FILE_NOTIFY_CHANGE_SIZE |
                FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_SECURITY | FILE_NOTIFY_CHANGE_CREATION,
                &bytesReturned,
                NULL,
                NULL
            )) {
                FILE_NOTIFY_INFORMATION* notify = (FILE_NOTIFY_INFORMATION*)buffer;

                do {
                    switch (notify->Action) {
                    case FILE_ACTION_ADDED:
                        printf("File added: ");
                        break;
                    case FILE_ACTION_REMOVED:
                        printf("File removed: ");
                        break;
                    case FILE_ACTION_MODIFIED:
                        printf("File modified: ");
                        break;
                    case FILE_ACTION_RENAMED_OLD_NAME:
                        printf("File renamed from: ");
                        break;
                    case FILE_ACTION_RENAMED_NEW_NAME:
                        printf("File renamed to: ");
                        break;
                    default:
                        printf("Unknown action: ");
                    }

                    wprintf(L"%.*s\n", notify->FileNameLength / sizeof(WCHAR), notify->FileName);
                    notify = notify->NextEntryOffset
                        ? (FILE_NOTIFY_INFORMATION*)((BYTE*)notify + notify->NextEntryOffset)
                        : NULL;
                } while (notify);
            }

            // Сбросить уведомление, чтобы продолжить отслеживание
            if (!FindNextChangeNotification(hChangeHandle)) {
                fprintf(stderr, "Error: Could not reset notification. Code: %ld\n", GetLastError());
                break;
            }
        }
        else {
            fprintf(stderr, "Error: Wait failed. Code: %ld\n", GetLastError());
            break;
        }
    }

    CloseHandle(hDir);
    FindCloseChangeNotification(hChangeHandle);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <directory_path>\n", argv[0]);
        return 1;
    }

    const char* directoryPath = argv[1];

    ListDirectoryContents(directoryPath);

    printf("\nWatching directory for changes...\n");
    WatchDirectory(directoryPath);

    return 0;
}