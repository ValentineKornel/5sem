//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <windows.h>
//#include <time.h>
//#pragma warning(disable : 4996)
//
//struct Student {
//    char Name[64];
//    char Surname[128];
//    unsigned char Course;
//    unsigned char Group;
//    char ID[8];
//};
//
//HANDLE hFile, hMapping;
//struct Student* ptr = NULL; // Указатель на отображение файла
//LPSTR FlePath = (LPSTR)"D:\\Study\\university\\5semester\\SP\\lab6\\build\\students.bin";
//size_t fileSize = 0;
//int listSize;
//char MappingName[] = "MyMapping";
//
//void printStudent(struct Student* student) {
//    if (student->Name[0] != '\0') {
//        printf("%s %s %u course %u group. ID: %s\n", student->Name, student->Surname, student->Course, student->Group, student->ID);
//    }
//    else {
//        //printf("Error. Null pointer given to printStudent.\n");
//        printf("null\n");
//    }
//}
//
//void MyOpenFile(LPSTR filePath, LPINT listSize);
//void AddRow(struct Student newStudent, int pos);
//void RemRow(int pos);
//void PrintRow(int pos);
//void PrintRows();
//void CloseFile();
//void cleanStudent(struct Student* student);
//
//int main(int argc, char* argv[]) {
//    int option;
//    int rowPosition;
//    //struct Student newStudent;
//    struct Student newStudent = {
//        .Name = "test",
//        .Surname = "test",
//        .Course = 3,
//        .Group = 4,
//        .ID = "1234567"
//    };
//
//    while (1) {
//        printf("choose option: \n1.Open file.\n2.Input row.\n3.Remove row.\n4.Print row.\n5.Print file.\n6.Close file.\n0.Exit.\n");
//        //printf("choose option: 1.Open file. 2.Input row. 3.Remove row. 4.Print row. 5.Print file. 6.Close file. 0.Exit.\n");
//
//        if ((scanf("%d", &option) != 1) || option > 6 || option < 0) {
//            printf("Wrong input command!\n");
//            while (getchar() != '\n');
//            continue;
//        }
//        while (getchar() != '\n');
//
//        switch (option) {
//        case 0:
//            printf("Exit the program");
//            exit(0);
//        case 1:
//            printf("input list maxsize in case of the lack of the mapping:\n");
//            scanf("%d", &listSize);
//            while (getchar() != '\n');
//
//            MyOpenFile(FlePath, &listSize);
//            break;
//        case 2:
//
//            LARGE_INTEGER frequency, start, end;
//            double elapsedTime;
//
//            if (!QueryPerformanceFrequency(&frequency)) {
//                fprintf(stderr, "High-resolution performance counter not supported.\n");
//                return 1;
//            }
//
//            // Фиксируем начальное время
//            QueryPerformanceCounter(&start);
//            AddRow(newStudent, 10006);
//            QueryPerformanceCounter(&end);
//            elapsedTime = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;
//            printf("Elapsed time: %.9f seconds\n", elapsedTime);
//            /*printf("Enter new student details (Name, Surname, Course, Group, ID):\n");
//            scanf("%s %s %hhu %hhu %s", newStudent.Name, newStudent.Surname, &newStudent.Course, &newStudent.Group, newStudent.ID);
//            while (getchar() != '\n');
//            printf("Enter position to insert (-1 for end): ");
//            scanf("%d", &rowPosition);
//            while (getchar() != '\n');
//
//            AddRow(newStudent, rowPosition);*/
//            break;
//        case 3:
//            printf("Enter row index to delete\n");
//            scanf("%d", &rowPosition);
//            while (getchar() != '\n');
//
//            RemRow(rowPosition);
//            break;
//        case 4:
//            printf("Enter row index to print\n");
//            scanf("%d", &rowPosition);
//            while (getchar() != '\n');
//
//            PrintRow(rowPosition);
//            break;
//        case 5:
//            PrintRows();
//            break;
//        case 6:
//            CloseFile();
//            break;
//        }
//        system("pause & cls");
//    }
//
//    return 0;
//}
//
//void MyOpenFile(LPSTR filePath, LPINT listSize) {
//    hFile = CreateFileA(
//        filePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
//
//    if (hFile == INVALID_HANDLE_VALUE) {
//        fprintf(stderr, "Create file failed.\nThe last error code: %ld\n", GetLastError());
//        return;
//    }
//
//    fileSize = GetFileSize(hFile, NULL);
//    if (fileSize == INVALID_FILE_SIZE) {
//        fprintf(stderr, "Error getting file size.\n");
//        CloseHandle(hFile);
//        return;
//    }
//
//    hMapping = OpenFileMappingA(FILE_MAP_WRITE | FILE_MAP_READ, FALSE, MappingName);
//
//    if (hMapping == NULL) {
//        printf("Mapping not found. Creating new one...\n");
//
//        if (fileSize == 0) {
//            DWORD writtenBytes;
//            int initialListSize = *listSize;
//            if (!WriteFile(hFile, &initialListSize, sizeof(int), &writtenBytes, NULL) || writtenBytes != sizeof(int)) {
//                fprintf(stderr, "Failed to write initial list size.\n");
//                CloseHandle(hFile);
//                return;
//            }
//            fileSize += sizeof(int);
//
//            hMapping = CreateFileMappingA(hFile, NULL, PAGE_READWRITE, 0, sizeof(int) + sizeof(struct Student) * (*listSize), MappingName);
//        }
//        else {
//            //hMapping = CreateFileMappingA(hFile, NULL, PAGE_READWRITE, 0, fileSize + sizeof(struct Student) * (*listSize), MappingName);
//            hMapping = CreateFileMappingA(hFile, NULL, PAGE_READWRITE, 0, sizeof(int) + sizeof(struct Student) * (*listSize), MappingName);//!!!!!!!!!!!!!!
//        }
//        if (!hMapping) {
//            fprintf(stderr, "Create file mapping failed.\nThe last error code: %ld\n", GetLastError());
//            CloseHandle(hFile);
//            return;
//        }
//    }
//    else {
//        // Если файл уже содержит данные, считываем начальное значение listSize
//        DWORD readBytes;
//        SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
//        if (!ReadFile(hFile, listSize, sizeof(int), &readBytes, NULL) || readBytes != sizeof(int)) {
//            fprintf(stderr, "Failed to read list size.\n");
//            CloseHandle(hFile);
//            return;
//        }
//        printf("list size: %d\n", *listSize);
//        printf("opened existing mapping\n");
//    }
//    ptr = (struct Student*)MapViewOfFile(hMapping, FILE_MAP_WRITE, 0, 0, 0);
//    if (!ptr) {
//        fprintf(stderr, "Map view of file failed.\nThe last error code: %ld\n", GetLastError());
//        CloseHandle(hMapping);
//        CloseHandle(hFile);
//        return;
//    }
//
//    int* size = (int*)ptr;
//    if (*size != *listSize) {
//        *size = *listSize;
//    }
//
//    ptr = (struct Student*)((char*)ptr + sizeof(int));
//
//    printf("File successfully opened.\n\n");
//}
//
//void AddRow(struct Student newStudent, int pos) {
//    int totalStudentsAmount = listSize;
//    if (pos < -totalStudentsAmount || pos >= totalStudentsAmount) {
//        fprintf(stderr, "Invalid position\n");
//        return;
//    }
//
//    if (pos < 0) {
//        pos = totalStudentsAmount + pos;
//    }
//
//    // Расширяем отображение, если добавляем в конец
//    /*if (pos == -2 || pos == totalStudentsAmount) {
//        totalStudentsAmount++;
//        UnmapViewOfFile(ptr);
//        CloseHandle(hMapping);
//
//        fileSize += sizeof(Student);
//        hMapping = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, fileSize, NULL);
//        ptr = (Student*)MapViewOfFile(hMapping, FILE_MAP_WRITE, 0, 0, 0);
//        pos = totalStudentsAmount - 1;
//    }*/
//
//    if (ptr[pos].Name[0] != '\0') {
//        printf("row with index %d already taken\n", pos);
//        return;
//    }
//
//    ptr[pos] = newStudent;
//    printf("Row successfully added.\n\n");
//}
//
//void RemRow(int pos) {
//    int totalStudentsAmount = listSize;
//    if (pos < -totalStudentsAmount || pos >= totalStudentsAmount) {
//        fprintf(stderr, "Invalid position\n");
//        return;
//    }
//
//    if (pos < 0) {
//        pos = totalStudentsAmount + pos;
//    }
//
//    if (ptr[pos].Name[0] == '\0') {
//        printf("Row with index %d is empty\n", pos);
//        return;
//    }
//
//    cleanStudent(&ptr[pos]);
//    {
//        ////Закрываем текущее отображение и уменьшаем размер файла
//        //UnmapViewOfFile(ptr);
//        //CloseHandle(hMapping);
//
//        //fileSize -= sizeof(Student);
//        //SetFilePointer(hFile, fileSize, NULL, FILE_BEGIN);
//        //SetEndOfFile(hFile); // Устанавливаем новый конец файла
//
//        //// Создаем новое отображение с уменьшенным размером
//        //hMapping = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, fileSize, NULL);
//        //if (hMapping == NULL) {
//        //    fprintf(stderr, "CreateFileMapping failed: %ld\n", GetLastError());
//        //    return;
//        //}
//        //ptr = (Student*)MapViewOfFile(hMapping, FILE_MAP_WRITE, 0, 0, 0);
//        //if (ptr == NULL) {
//        //    fprintf(stderr, "MapViewOfFile failed: %ld\n", GetLastError());
//        //    return;
//        //}
//    }
//
//    printf("Row successfully deleted.\n\n");
//}
//
//void cleanStudent(struct Student* student) {
//    memset(student->Name, '\0', sizeof(student->Name));
//    memset(student->Surname, '\0', sizeof(student->Surname));
//    student->Course = 0;
//    student->Group = 0;
//    memset(student->ID, '\0', sizeof(student->ID));
//}
//
//
//void PrintRow(int pos) {
//    int totalStudentsAmount = listSize;
//    if (pos < -totalStudentsAmount || pos >= totalStudentsAmount) {
//        fprintf(stderr, "Invalid position\n");
//        return;
//    }
//    if (pos < 0) {
//        pos = totalStudentsAmount + pos;
//    }
//
//    printStudent(&ptr[pos]);
//}
//
//void PrintRows() {
//    //int totalStudents = fileSize / sizeof(Student);
//    int totalStudents = listSize;
//    for (int i = 0; i < totalStudents; i++) {
//        printf("Row %d: ", i);
//        printStudent(&ptr[i]);
//    }
//}
//
//void CloseFile() {
//    if (ptr) {
//        UnmapViewOfFile(ptr);
//    }
//    if (hMapping) {
//        CloseHandle(hMapping);
//    }
//    if (hFile) {
//        CloseHandle(hFile);
//    }
//    printf("File successfully closed.\n");
//}
