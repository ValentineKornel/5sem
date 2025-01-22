#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#pragma warning(disable : 4996)


HANDLE hFile;
char* fileBuffer = NULL;     // ����� ��� �������� ����������� �����
DWORD bufferSize = 0;        // ������ ������
char* newRow = NULL;

//const char* FlePath = "D:\\Study\\university\\5semester\\SP\\lab6\\build\\Release\\file.txt";
char* FilePath = NULL;

void MyOpenFile(LPSTR filePath);
void AddRow(LPSTR row, INT pos);
void RemRow(INT pos);
void PrintRow(INT pos);
void PrintRows();
void CloseFile();


int main(int argc, char* argv[]) {
    int option;
    int rowPosition;

    while (1) {
        //printf("choose option: 1.Open file. 2.Input row. 3.Remove row. 4.Print row. 5.Print file. 6.Close file. 0.Exit.\n");
        printf("choose option: \n1.Open file.\n2.Input row.\n3.Remove row.\n4.Print row.\n5.Print file.\n6.Close file.\n0.Exit.\n");


        if ((scanf("%d", &option) != 1) || option > 6 || option < 0) {
            printf("Wrong input command!\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        switch (option) {
        case 0:
        {
            printf("Exit the program");
            exit(0);
        }
        case 1:
        {
            FilePath = (char*)malloc(100 * sizeof(char));
            printf("Eneter file path\n");
            if (!fgets(FilePath, 100, stdin)) {
                printf("Can't read file path\n");
                break;
            }
            size_t len = strlen(FilePath);
            if (len > 0 && FilePath[len - 1] == '\n') {
                FilePath[len - 1] = '\0';
            }
            printf("file path: %s\n", FilePath);
            MyOpenFile(FilePath);
            break;
        }
        case 2:
        {
            if (newRow == NULL) {
                fprintf(stderr, "Error. Memory for the string wasn't allocated.\n");
                break;
            }
            printf("Enter new row:\n");
            if (!fgets(newRow, 100, stdin)) {
                printf("Can't read new row");
                break;
            }
            printf("str: %s", newRow);
            printf("Enter new row position:");
            if (!(scanf("%d", &rowPosition))) {
                printf("Can't read new row postion");
                break;
            }
            AddRow(newRow, rowPosition);
            break;
        }
        case 3:
        {
            printf("Enter row index to delete\n");
            if ((scanf("%d", &rowPosition) < 1) || rowPosition < -1) {
                printf("Wrong input command!\n");
                while (getchar() != '\n');
                continue;
            }
            while (getchar() != '\n');
            RemRow(rowPosition);
            break;
        }
        case 4:
        {
            printf("Enter row index to print\n");
            if ((scanf("%d", &rowPosition) < 1) || rowPosition < -1) {
                printf("Wrong input command!\n");
                while (getchar() != '\n');
                continue;
            }
            while (getchar() != '\n');
            PrintRow(rowPosition);
            break;
        }
        case 5:
        {
            PrintRows();
            break;
        }
        case 6:
        {
            CloseFile();
            break;
        }
        }

        //system("pause & cls");
    }
    
    return 0;
}

void MyOpenFile(LPSTR filePath) {

    // ��������� ���� ��� ������
    hFile = CreateFileA(
        filePath,     // ��� �����
        GENERIC_READ | GENERIC_WRITE,            // ������ �� �����
        FILE_SHARE_READ,                       // ����������� ������ � �����
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

    newRow = (char*)malloc(100 * sizeof(char));

    printf("File successfully opened.\n\n");

}

void AddRow(LPSTR row, INT pos) {
    if (hFile == NULL || row == NULL) {
        fprintf(stderr, "Error. file descriptor or new row are NULL\n");
        return;
    }

    // ��������� �������� ������� �����
    DWORD fileSize = GetFileSize(hFile, NULL);
    if (fileSize == INVALID_FILE_SIZE) {
        fprintf(stderr, "Error gaining file size\n");
        return;
    }

    DWORD bytesRead, bytesWritten;
    char* buffer = NULL;

    if (pos == -1) {
        // ��������� ��������� ����, ����� ���������, �������� �� �� �������� ����� ������
        if (fileSize > 0) {
            char lastChar;
            SetFilePointer(hFile, fileSize - 1, NULL, FILE_BEGIN); // ������� � ���������� �����
            if (!ReadFile(hFile, &lastChar, 1, &bytesRead, NULL) || bytesRead != 1) {
                fprintf(stderr, "������ ��� ������ ���������� �������\n");
                return;
            }


            // ���� ��������� ������ �� ����� ������, ��������� � ����� ������� ����� ������
            if (lastChar != '\n') {
                DWORD bytesWritten;
                SetFilePointer(hFile, 0, NULL, FILE_END); // ������� � ����� �����
                if (!WriteFile(hFile, "\n", 1, &bytesWritten, NULL) || bytesWritten != 1) {
                    fprintf(stderr, "������ ��� ������ ������� ����� ������\n");
                    return;
                }
            }
        }
        if (!WriteFile(hFile, row, strlen(row) - 1, &bytesWritten, NULL) || bytesWritten != strlen(row) - 1) {
            fprintf(stderr, "Error writing to the end of file\n");
            return;
        }
        return;
    }
    bytesRead = 0;

    if (pos == 0) {
        // ���������� � ������: ������� ���� ���� � �����, ��������� ������, �������������� ����
        buffer = (char*)malloc(fileSize);
        if (buffer == NULL) {
            fprintf(stderr, "Error allocation memory.\n");
            return;
        }

        SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
        if (!ReadFile(hFile, buffer, fileSize, &bytesRead, NULL) || bytesRead != fileSize) {
            fprintf(stderr, "Error reading file\n");
            free(buffer);
            return;
        }

        SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
        WriteFile(hFile, row, strlen(row), &bytesWritten, NULL);
        WriteFile(hFile, buffer, fileSize, &bytesWritten, NULL);

        free(buffer);
        return;
    }

    // ���������� � ��������
    // � ������ ������� ��� �������� ������ ���������� ������, �.�. ���������� �����
    int currentLine = 1;
    DWORD lineStartPos = 0;
    SetFilePointer(hFile, 0, NULL, FILE_BEGIN);

    // ���� ������ ������ ������
    while (currentLine < pos && lineStartPos < fileSize) {
        char ch;
        if (!ReadFile(hFile, &ch, 1, &bytesRead, NULL) || bytesRead == 0) break;
        if (ch == '\n') currentLine++;
        lineStartPos++;
    }

    if (currentLine != pos) {
        fprintf(stderr, "Incorrect index\n");
        return;
    }

    buffer = (char*)malloc(fileSize - lineStartPos);
    if (buffer == NULL) {
        fprintf(stderr, "Error allocation memory\n");
        return;
    }

    SetFilePointer(hFile, lineStartPos, NULL, FILE_BEGIN);
    if (!ReadFile(hFile, buffer, fileSize - lineStartPos, &bytesRead, NULL)) {
        fprintf(stderr, "Error reading file\n");
        free(buffer);
        return;
    }

    SetFilePointer(hFile, lineStartPos, NULL, FILE_BEGIN);
    WriteFile(hFile, row, strlen(row), &bytesWritten, NULL);
    WriteFile(hFile, buffer, bytesRead, &bytesWritten, NULL);

    free(buffer);
    printf("Row successfully added.\n\n");
    return;
}

void RemRow(INT pos) {
    if (hFile == NULL || pos < -1) {
        fprintf(stderr, "Error. Incorrect file descriptor or index\n");
        return;
    }

    DWORD fileSize = GetFileSize(hFile, NULL);
    if (fileSize == INVALID_FILE_SIZE) {
        fprintf(stderr, "Error gaining file size\n");
        return;
    }

    char* buffer = (char*)malloc(fileSize);
    if (buffer == NULL) {
        fprintf(stderr, "Error allocating memory\n");
        return;
    }

    SetFilePointer(hFile, 0, NULL, FILE_BEGIN);

    DWORD bytesRead;
    if (!ReadFile(hFile, buffer, fileSize, &bytesRead, NULL) || bytesRead != fileSize) {
        fprintf(stderr, "Error reading file\n");
        free(buffer);
        return;
    }

    int currentLine = 1;
    DWORD startPos = 0, endPos = 0;
    int found = 0;

    if (pos == 0) {
        // ������� ������ ������
        endPos = 0;
        while (endPos < fileSize && buffer[endPos] != '\n') {
            endPos++;
        }
        endPos++; // �������� ������ ����� ������
        found = 1;
    }
    else if (pos == -1) {
        // ������� ��������� ������
        DWORD i = fileSize - 1;
        while (i >= 0 && buffer[i] != '\n' && i > 0) {
            i--;
        }
        startPos = i;
        endPos = fileSize;
        found = 1;
    }
    else {
        // ������� ������ �� ������� pos
        for (DWORD i = 0; i < fileSize; i++) {
            if (buffer[i] == '\n') {
                if (currentLine == pos) {
                    endPos = i + 1;  // ����� ������ ��� ��������
                    found = 1;
                    break;
                }
                currentLine++;
                startPos = i + 1;  // ������ ��������� ������
            }
        }
    }

    if (!found) {
        fprintf(stderr, "Error. Index is out of bounds\n");
        free(buffer);
        return;
    }

    // �������� ������ ����� ������ ����� �� ����� ��������� ������
    DWORD remainingSize = fileSize - endPos;
    memmove(buffer + startPos, buffer + endPos, remainingSize);

    // �������� ����, �������� ��� ������
    SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
    DWORD bytesWritten;
    WriteFile(hFile, buffer, fileSize - (endPos - startPos), &bytesWritten, NULL);
    SetEndOfFile(hFile); // ������������� ����� ����� �����

    free(buffer);
    printf("Row successfully deleted.\n\n");
    return;
}

void PrintRow(INT pos) {
    if (hFile == NULL || pos < -1) {
        fprintf(stderr, "Error. Incorrect file descriptor or index\n");
        return;
    }

    DWORD fileSize = GetFileSize(hFile, NULL);
    if (fileSize == INVALID_FILE_SIZE) {
        fprintf(stderr, "Error gaining file size\n");
        return;
    }

    char* buffer = (char*)malloc(fileSize + 1);  // +1 ��� ������������ ����
    if (buffer == NULL) {
        fprintf(stderr, "Error allocating memory\n");
        return;
    }

    SetFilePointer(hFile, 0, NULL, FILE_BEGIN);

    DWORD bytesRead;
    if (!ReadFile(hFile, buffer, fileSize, &bytesRead, NULL) || bytesRead != fileSize) {
        fprintf(stderr, "Error reading file\n");
        free(buffer);
        return;
    }

    buffer[fileSize] = '\0';  // ��������� ������ ������� ������ ��� �������� ������

    char* lineStart = buffer;
    char* lastLineStart = NULL;
    int currentLine = 1;

    // ���������� ������ ������ ������
    while (*lineStart) {
        if (currentLine == pos) {
            break;  // ���� ����� ������ � �������� pos, ��������� �����
        }
        if (*lineStart == '\n') {
            currentLine++;
            lastLineStart = lineStart + 1;  // ��������� ��������� �� ������ ��������� ������
        }
        lineStart++;
    }

    // ���� ������� ������� -1, �� ������������� ��������� �� ��������� ������
    if (pos == -1) {
        if (lastLineStart) {
            lineStart = lastLineStart;
        }
        else {
            lineStart = buffer;  // ���� ���� ���� ��� �������� ������ ���� ������
        }
    }

    // ���������, ������� �� ������ �� ������ �������
    if ((pos != -1 && currentLine != pos) || *lineStart == '\0') {
        fprintf(stderr, "Error: row with index %d wasn't found.\n", pos);
        free(buffer);
        return;
    }

    // ���� ����� ������
    char* lineEnd = lineStart;
    while (*lineEnd && *lineEnd != '\n') {
        lineEnd++;
    }

    // ������� ������ �� �������
    printf("Row %d: ", (pos == -1) ? currentLine : pos);
    while (lineStart < lineEnd) {
        putchar(*lineStart);
        lineStart++;
    }
    putchar('\n');

    free(buffer);
    return;
}


void PrintRows() {

    if (hFile == NULL) {
        fprintf(stderr, "Error. Incorrect file descriptor\n");
        return;
    }

    SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
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

}

void CloseFile() {
    if (hFile == NULL) {
        fprintf(stderr, "Error. Incorrect file descriptor\n");
        return;
    }

    CloseHandle(hFile);
    hFile = NULL;
    free(newRow);
    printf("File successfully closed.\n");
}