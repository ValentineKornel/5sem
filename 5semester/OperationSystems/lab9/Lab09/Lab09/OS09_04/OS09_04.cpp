﻿#define _CRT_NON_CONFORMING_WCSTOK
#define _CRT_SECURE_NO_WARNINGS


#include <iostream>
#include <cstdlib>
#include "Windows.h"

using namespace std;

#define FILE_PATH L"D:/Study/university/5semester/OS/lab9/Lab09/OS09_01.txt"
int rowC = 0;


BOOL printWatchRowFileTxt(LPWSTR FileName, DWORD mlsec);

int main()
{
    setlocale(LC_ALL, "ru");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    LPWSTR fileName = (LPWSTR)FILE_PATH;

    printWatchRowFileTxt(fileName, 50000);
}

BOOL printWatchRowFileTxt(LPWSTR FileName, DWORD mlsec)
{
    PLARGE_INTEGER fileSize = new LARGE_INTEGER();
    LPWSTR path = (LPWSTR)L"D:\\Study\\university\\5semester\\OS\\lab9\\Lab09";
    char* cFileName = new char[wcslen(FileName) * sizeof(char) + 1];
    wcstombs(cFileName, FileName, strlen(cFileName));
    int i = 0;
    int lenName = 0;
    try
    {
        HANDLE notif = FindFirstChangeNotification(path, false, FILE_NOTIFY_CHANGE_LAST_WRITE);
        DWORD err = GetLastError();
        clock_t t1 = clock();
        clock_t t2 = clock();
        DWORD dwWaitStatus;
        printf("Filewatch timestamp: %d\n", t1);
        while (true)
        {
            dwWaitStatus = WaitForSingleObject(notif, mlsec);
            switch (dwWaitStatus)
            {
            case WAIT_OBJECT_0:
            {
                if (FindNextChangeNotification(notif) == FALSE)
                    break;
                else
                {
                    int position = 0;
                    int rowCount = 0;
                    HANDLE of = CreateFileW(
                        FileName,
                        GENERIC_READ,
                        FILE_SHARE_READ | FILE_SHARE_WRITE,
                        NULL,
                        OPEN_ALWAYS,
                        FILE_ATTRIBUTE_NORMAL,
                        NULL);
                    if (of == INVALID_HANDLE_VALUE)
                    {
                        CloseHandle(of);
                        cout << "Error: Open file failed" << endl;
                    }
                    if (GetFileSizeEx(of, fileSize))
                    {
                        char* buf = new char[(fileSize->QuadPart + 1) * sizeof(char)];
                        ZeroMemory(buf, (fileSize->QuadPart + 1) * sizeof(char));
                        DWORD n = NULL;
                        if (ReadFile(of, buf, fileSize->QuadPart, &n, NULL))
                            while (buf[position++] != '\0')
                                if (buf[position] == '\n')
                                    rowCount++;
                    }
                    if (rowC != rowCount)
                    {
                        printf("\nRows: %d\n", rowCount);
                        rowC = rowCount;
                    }
                    CloseHandle(of);
                }
            }
            }
            t2 = clock();
            if (t2 - t1 > mlsec)
                break;
        }
        CloseHandle(notif);
        printf("\nFilewatch end timestamp: %d\n", t2);
    }
    catch (const char* err)
    {
        //cout << "Error: " << err << "\n";
        return false;
    }
    return true;
}