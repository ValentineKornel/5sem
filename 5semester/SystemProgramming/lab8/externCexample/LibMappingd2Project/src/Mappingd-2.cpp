/***************************************************************************
Модуль: MyLib.c
***************************************************************************/
// сюда включаются стандартные заголовочные файлы Windows и библиотеки C

// этот файл исходного кода DLL экспортирует функции и переменные
#define MYLIBAPI extern "C" __declspec(dllexport)
#define EXT extern __declspec(dllexport)
// включаем экспортируемые структуры данных, идентификаторы, функции и
// переменные
#include "Mappingd-2.h"
////////////////////////////////////////////////////////////////////////////
// здесь размещается исходный код этой DLL
struct Student* ptr = NULL;
const int structsInSegmentAmount = (1024 * 64 / sizeof(struct Student) + 1); //325
const int offsetToFirstStudent = ((1024 * 64) - (sizeof(struct Student) * structsInSegmentAmount)) * -1; //114
int listSize;
char MappingName[] = "MyMapping";
HANDLE hFile;

EXT void FunctionWithoutCNaming(int param1, char param2){
    printf("Something\n");
}

void MyOpenFile(HANDLE* hMapping, LPSTR filePath, LPINT lSize) {
    hFile = CreateFileA(
        filePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Create file failed.\nThe last error code: %ld\n", GetLastError());
        return;
    }
    *hMapping = OpenFileMappingA(FILE_MAP_WRITE | FILE_MAP_READ, FALSE, MappingName);

    if (*hMapping == NULL) {
        printf("Mapping not found. Creating new one...\n");

        DWORD writtenBytes;
        int initialListSize = *lSize;
        if (!WriteFile(hFile, &initialListSize, sizeof(int), &writtenBytes, NULL) || writtenBytes != sizeof(int)) {
            fprintf(stderr, "Failed to write initial list size.\n");
            CloseHandle(hFile);
            return;
        }

        *hMapping = CreateFileMappingA(hFile, NULL, PAGE_READWRITE, 0, sizeof(int) + sizeof(struct Student) * (*lSize), MappingName);
        if (!hMapping) {
            fprintf(stderr, "Create file mapping failed.\nThe last error code: %ld\n", GetLastError());
            CloseHandle(hFile);
            return;
        }
    }
    else {
        // Если файл уже содержит данные, считываем начальное значение listSize
        DWORD readBytes;
        SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
        if (!ReadFile(hFile, lSize, sizeof(int), &readBytes, NULL) || readBytes != sizeof(int)) {
            fprintf(stderr, "Failed to read list size.\n");
            CloseHandle(hFile);
            return;
        }
        printf("list size: %d\n", *lSize);
        printf("opened existing mapping\n");
    }

    listSize = *lSize;
    printf("File successfully opened.\n\n");
}

void AddRow(HANDLE hMapping, struct Student newStudent, int pos) {
    int position = pos;
    int maxOffset = (listSize / structsInSegmentAmount);
    if (position < -listSize || position >= listSize) {
        fprintf(stderr, "Invalid position\n");
        return;
    }

    if (position < 0) {
        position = listSize + position;
    }

    int offset = (position / structsInSegmentAmount);
    position -= offset * structsInSegmentAmount;

    int sizeToMap = sizeof(struct Student) * structsInSegmentAmount;
    if (offset == 0) {
        sizeToMap = listSize > structsInSegmentAmount ? sizeToMap + sizeof(int) : 0;
    }
    else if (offset == maxOffset) {
        sizeToMap = 0;
    }

    ptr = (struct Student*)MapViewOfFile(hMapping, FILE_MAP_WRITE, 0, 1024 * 64 * offset, sizeToMap);

    if (!ptr) {
        fprintf(stderr, "Map view of file failed.\nThe last error code: %ld\n", GetLastError());
        CloseHandle(hMapping);
        CloseHandle(hFile);
        return;
    }
    if (offset == 0) {
        ptr = (struct Student*)((char*)ptr + sizeof(int));
    }
    else {
        //ptr = (struct Student*)((char*)ptr + (offsetToFirstStudent * offset) + (sizeof(int)));
        ptr = (struct Student*)((char*)ptr + (offsetToFirstStudent)+(sizeof(int)));
    }

    if (ptr[position].Name[0] != '\0') {
        printf("row with index %d already taken\n", pos);
        return;
    }

    ptr[position] = newStudent;
    printf("Row successfully added.\n\n");
    UnmapViewOfFile(ptr);
}

void cleanStudent(struct Student* student) {
    memset(student->Name, '\0', sizeof(student->Name));
    memset(student->Surname, '\0', sizeof(student->Surname));
    student->Course = 0;
    student->Group = 0;
    memset(student->ID, '\0', sizeof(student->ID));
}

void RemRow(HANDLE hMapping, int pos) {
    int position = pos;
    int maxOffset = (listSize / structsInSegmentAmount);
    if (position < -listSize || position >= listSize) {
        fprintf(stderr, "Invalid position\n");
        return;
    }

    if (position < 0) {
        position = listSize + position;
    }

    int offset = (position / structsInSegmentAmount);
    position -= offset * structsInSegmentAmount;

    int sizeToMap = sizeof(struct Student) * structsInSegmentAmount;
    if (offset == 0) {
        sizeToMap = listSize > structsInSegmentAmount ? sizeToMap + sizeof(int) : 0;
    }
    else if (offset == maxOffset) {
        sizeToMap = 0;
    }

    ptr = (struct Student*)MapViewOfFile(hMapping, FILE_MAP_WRITE, 0, 1024 * 64 * offset, sizeToMap);

    if (!ptr) {
        fprintf(stderr, "Map view of file failed.\nThe last error code: %ld\n", GetLastError());
        CloseHandle(hMapping);
        CloseHandle(hFile);
        return;
    }
    if (offset == 0) {
        ptr = (struct Student*)((char*)ptr + sizeof(int));
    }
    else {
        //ptr = (struct Student*)((char*)ptr + (offsetToFirstStudent * offset) + (sizeof(int)));
        ptr = (struct Student*)((char*)ptr + (offsetToFirstStudent)+(sizeof(int)));

    }

    if (ptr[position].Name[0] == '\0') {
        printf("Row with index %d is empty\n", pos);
        return;
    }

    cleanStudent(&ptr[position]);
    printf("Row successfully deleted.\n\n");
    UnmapViewOfFile(ptr);
}


void printStudent(struct Student* student) {
    if (student->Name[0] != '\0') {
        printf("%s %s %u course %u group. ID: %s\n", student->Name, student->Surname, student->Course, student->Group, student->ID);
    }
    else {
        printf("null\n");
    }
}

void PrintRow(HANDLE hMapping, int pos) {
    int position = pos;
    int maxOffset = (listSize / structsInSegmentAmount);
    if (position < -listSize || position >= listSize) {
        fprintf(stderr, "Invalid position\n");
        return;
    }
    if (position < 0) {
        position = listSize + position;
    }

    int offset = (position / structsInSegmentAmount);
    position -= offset * structsInSegmentAmount;

    int sizeToMap = sizeof(struct Student) * structsInSegmentAmount;
    if (offset == 0) {
        sizeToMap = listSize > structsInSegmentAmount ? sizeToMap + sizeof(int) : 0;
    }
    else if (offset == maxOffset) {
        sizeToMap = 0;
    }

    ptr = (struct Student*)MapViewOfFile(hMapping, FILE_MAP_WRITE, 0, 1024 * 64 * offset, sizeToMap);

    if (!ptr) {
        fprintf(stderr, "Map view of file failed.\nThe last error code: %ld\n", GetLastError());
        CloseHandle(hMapping);
        CloseHandle(hFile);
        return;
    }
    if (offset == 0) {
        ptr = (struct Student*)((char*)ptr + sizeof(int));
    }
    else {
        //ptr = (struct Student*)((char*)ptr + (offsetToFirstStudent * offset) + (sizeof(int)));
        ptr = (struct Student*)((char*)ptr + (offsetToFirstStudent)+(sizeof(int)));

    }

    if (ptr[position].Name[0] == '\0') {
        printf("Row with index %d is empty\n", pos);
        return;
    }

    printStudent(&ptr[position]);
    UnmapViewOfFile(ptr);
}

void PrintRows(HANDLE hMapping) {
    int maxOffset = (listSize / structsInSegmentAmount);
    int rowid = 0;

    for (int offset = 0; offset <= maxOffset; offset++) {
        int sizeToMap = sizeof(struct Student) * structsInSegmentAmount;
        if (offset == 0) {
            sizeToMap = listSize > structsInSegmentAmount ? sizeToMap + sizeof(int) : 0;
        }
        else if (offset == maxOffset) {
            sizeToMap = 0;
        }

        ptr = (struct Student*)MapViewOfFile(hMapping, FILE_MAP_WRITE, 0, 1024 * 64 * offset, sizeToMap);

        if (!ptr) {
            fprintf(stderr, "Map view of file failed.\nThe last error code: %ld\n", GetLastError());
            CloseHandle(hMapping);
            CloseHandle(hFile);
            return;
        }
        if (offset == 0) {
            int lsize = listSize > structsInSegmentAmount ? structsInSegmentAmount : listSize;
            ptr = (struct Student*)((char*)ptr + sizeof(int));
            for (int i = 0; i < lsize; i++) {
                printf("Row %d: ", rowid++);
                printStudent(&ptr[i]);
            }
        }
        else {
            ptr = (struct Student*)((char*)ptr + (offsetToFirstStudent)+(sizeof(int)));
            int lsize = offset == maxOffset ? (listSize % structsInSegmentAmount) : structsInSegmentAmount;
            for (int i = 0; i < lsize; i++) {
                printf("Row %d: ", rowid++);
                printStudent(&ptr[i]);
            }
        }
        UnmapViewOfFile(ptr);
    }
}

void CloseFile(HANDLE hMapping) {
    if (hMapping) {
        CloseHandle(hMapping);
    }
    if (hFile) {
        CloseHandle(hFile);
    }
    printf("File successfully closed.\n");
}

////////////////////////////// Конец файла ////////////////////////////////