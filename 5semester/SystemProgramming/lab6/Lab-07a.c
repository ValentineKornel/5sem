#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#pragma warning(disable : 4996)

struct Student {
    char Name[64];
    char Surname[128];
    unsigned char Course;
    unsigned char Group;
    char ID[8];
};

HANDLE hFile, hMapping;
struct Student* ptr = NULL; // Указатель на отображение файла
LPSTR FilePath;
int listSize;
char MappingName[] = "MyMapping";
const int structsInSegmentAmount = (1024 * 64 / sizeof(struct Student) + 1); //325
const int offsetToFirstStudent = -((1024 * 64) - (sizeof(struct Student) * structsInSegmentAmount)); //114

void MyOpenFile(LPSTR filePath, LPINT listSize);
void AddRow(HANDLE hMapping, struct Student newStudent, int pos);
void RemRow(HANDLE hMapping, int pos);
void PrintRow(HANDLE hMapping, int pos);
void PrintRows(HANDLE hMapping);
void CloseFile(HANDLE hMapping);
void cleanStudent(struct Student* student);

int main(int argc, char* argv[]) {
    int option;
    int rowPosition;
    //struct Student newStudent;
    struct Student newStudent = {
        .Name = "test",
        .Surname = "test",
        .Course = 3,
        .Group = 4,
        .ID = "1234567"
    };

    while (1) {
        printf("choose option: \n1.Open file.\n2.Input row.\n3.Remove row.\n4.Print row.\n5.Print file.\n6.Close file.\n0.Exit.\n");
        //printf("choose option: 1.Open file. 2.Input row. 3.Remove row. 4.Print row. 5.Print file. 6.Close file. 0.Exit.\n");

        if ((scanf("%d", &option) != 1) || option > 6 || option < 0) {
            printf("Wrong input command!\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        switch (option) {
        case 0:
            printf("Exit the program");
            exit(0);
        case 1:
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

            printf("input list maxsize in case of the lack of the mapping:\n");
            scanf("%d", &listSize);
            while (getchar() != '\n');

            MyOpenFile(FilePath, &listSize);
            break;
        case 2:
        {/*LARGE_INTEGER frequency, start, end;
        double elapsedTime;
        if (!QueryPerformanceFrequency(&frequency)) {
            fprintf(stderr, "High-resolution performance counter not supported.\n");
            return 1;
        }
        QueryPerformanceCounter(&start);
        AddRow(hMapping, newStudent, 4900002);
        QueryPerformanceCounter(&end);
        elapsedTime = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;
            printf("Elapsed time: %.9f seconds\n", elapsedTime);*/
        }

        printf("Enter new student details (Name, Surname, Course, Group, ID):\n");
        scanf("%s %s %hhu %hhu %s", newStudent.Name, newStudent.Surname, &newStudent.Course, &newStudent.Group, newStudent.ID);
        while (getchar() != '\n');
        printf("Enter position to insert (-1 for end): ");
        scanf("%d", &rowPosition);
        while (getchar() != '\n');

        AddRow(hMapping, newStudent, rowPosition);
        break;
        case 3:
            printf("Enter row index to delete\n");
            scanf("%d", &rowPosition);
            while (getchar() != '\n');

            RemRow(hMapping, rowPosition);
            break;
        case 4:
            printf("Enter row index to print\n");
            scanf("%d", &rowPosition);
            while (getchar() != '\n');

            PrintRow(hMapping, rowPosition);
            break;
        case 5:
            PrintRows(hMapping);
            break;
        case 6:
            CloseFile(hMapping);
            break;
        }
        system("pause & cls");
    }

    return 0;
}

void MyOpenFile(LPSTR filePath, LPINT listSize) {
    hFile = CreateFileA(
        filePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Create file failed.\nThe last error code: %ld\n", GetLastError());
        return;
    }

    hMapping = OpenFileMappingA(FILE_MAP_WRITE | FILE_MAP_READ, FALSE, MappingName);

    if (hMapping == NULL) {
        printf("Mapping not found. Creating new one...\n");

        DWORD writtenBytes;
        int initialListSize = *listSize;
        if (!WriteFile(hFile, &initialListSize, sizeof(int), &writtenBytes, NULL) || writtenBytes != sizeof(int)) {
            fprintf(stderr, "Failed to write initial list size.\n");
            CloseHandle(hFile);
            return;
        }

        hMapping = CreateFileMappingA(hFile, NULL, PAGE_READWRITE, 0, sizeof(int) + sizeof(struct Student) * (*listSize), MappingName);
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
        if (!ReadFile(hFile, listSize, sizeof(int), &readBytes, NULL) || readBytes != sizeof(int)) {
            fprintf(stderr, "Failed to read list size.\n");
            CloseHandle(hFile);
            return;
        }
        printf("list size: %d\n", *listSize);
        printf("opened existing mapping\n");
    }

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

void cleanStudent(struct Student* student) {
    memset(student->Name, '\0', sizeof(student->Name));
    memset(student->Surname, '\0', sizeof(student->Surname));
    student->Course = 0;
    student->Group = 0;
    memset(student->ID, '\0', sizeof(student->ID));
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