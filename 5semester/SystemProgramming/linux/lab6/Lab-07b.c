#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

struct Student {
    char Name[64];
    char Surname[128];
    unsigned char Course;
    unsigned char Group;
    char ID[8];
};

int fd;
struct Student* ptr = NULL;
char filePath[100];
size_t fileSize = 0;
int listSize;

void printStudent(struct Student* student) {
    if (student->Name[0] != '\0') {
        printf("%s %s %u course %u group. ID: %s\n", student->Name, student->Surname, student->Course, student->Group, student->ID);
    } else {
        printf("null\n");
    }
}

void MyOpenFile(int fd, char filePath[100], int* listSize);
void AddRow(int fd, struct Student newStudent, int pos);
void RemRow(int fd, int pos);
void PrintRow(int fd, int pos);
void PrintRows(int fd);
void CloseFile(int fd);
void cleanStudent(struct Student* student);

int main() {
    int option;
    int rowPosition;
    struct Student newStudent;

    while (1) {
        //printf("Choose option: \n1.Open file.\n2.Input row.\n3.Remove row.\n4.Print row.\n5.Print file.\n6.Close file.\n0.Exit.\n");
        printf("choose option: 1.Open file. 2.Input row. 3.Remove row. 4.Print row. 5.Print file. 6.Close file. 0.Exit.\n");

        if (scanf("%d", &option) != 1 || option > 6 || option < 0) {
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
            printf("Eneter file path\n");
            scanf("%s", filePath);
            while (getchar() != '\n');
            printf("Input list max size:\n");
            scanf("%d", &listSize);
            while (getchar() != '\n');

            MyOpenFile(fd, filePath, &listSize);
            break;
        case 2:
            printf("Enter new student details (Name, Surname, Course, Group, ID):\n");
            scanf("%s %s %hhu %hhu %s", newStudent.Name, newStudent.Surname, &newStudent.Course, &newStudent.Group, newStudent.ID);
            while (getchar() != '\n');
            printf("Enter position to insert (-1 for end): ");
            scanf("%d", &rowPosition);
            while (getchar() != '\n');

            AddRow(fd, newStudent, rowPosition);
            break;
        case 3:
            printf("Enter row index to delete\n");
            scanf("%d", &rowPosition);
            while (getchar() != '\n');

            RemRow(fd, rowPosition);
            break;
        case 4:
            printf("Enter row index to print\n");
            scanf("%d", &rowPosition);
            while (getchar() != '\n');

            PrintRow(fd, rowPosition);
            break;
        case 5:
            PrintRows(fd);
            break;
        case 6:
            CloseFile(fd);
            break;
        }
    }

    return 0;
}

void MyOpenFile(int fd, char filePath[100], int* listSize) {
    // Open the file with read/write permissions, creating it if it doesn't exist
    fd = open(filePath, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Failed to open file");
        return;
    }

    // Get the current file size
    struct stat fileStat;
    if (fstat(fd, &fileStat) == -1) {
        perror("Failed to get file size");
        close(fd);
        return;
    }
    fileSize = fileStat.st_size;

    // Check if the file needs to be initialized
    if (fileSize == 0) {
        // Set initial list size if the file is new
        fileSize = sizeof(int) + sizeof(struct Student) * (*listSize);
        if (ftruncate(fd, fileSize) == -1) {
            perror("Failed to set file size");
            close(fd);
            return;
        }

        // Write initial list size to the beginning of the file
        if (write(fd, listSize, sizeof(int)) != sizeof(int)) {
            perror("Failed to write initial list size");
            close(fd);
            return;
        }
    } else {
        // Read existing list size from the file if it already contains data
        if (pread(fd, listSize, sizeof(int), 0) != sizeof(int)) {
            perror("Failed to read list size");
            close(fd);
            return;
        }
        printf("List size: %d\n", *listSize);
    }

    // Map the file into memory
    ptr = mmap(NULL, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("Failed to map file");
        close(fd);
        return;
    }

    // Move the pointer past the list size header
    ptr = (struct Student*)((char*)ptr + sizeof(int));

    printf("File successfully opened and mapped.\n");
}

void AddRow(int fd, struct Student newStudent, int pos) {
    int totalStudentsAmount = listSize;
    if (pos < -totalStudentsAmount|| pos >= listSize) {
        fprintf(stderr, "Invalid position\n");
        return;
    }

    if (pos < 0) {
    pos = totalStudentsAmount + pos;
    }

    if (ptr[pos].Name[0] != '\0') {
        printf("Row with index %d already taken\n", pos);
        return;
    }

    ptr[pos] = newStudent;
    printf("Row successfully added.\n\n");
}

void RemRow(int fd, int pos) {
    int totalStudentsAmount = listSize;
    if (pos < -totalStudentsAmount|| pos >= listSize) {
        fprintf(stderr, "Invalid position\n");
        return;
    }

    if (pos < 0) {
    pos = totalStudentsAmount + pos;
    }

    cleanStudent(&ptr[pos]);
    printf("Row successfully deleted.\n\n");
}

void cleanStudent(struct Student* student) {
    memset(student->Name, '\0', sizeof(student->Name));
    memset(student->Surname, '\0', sizeof(student->Surname));
    student->Course = 0;
    student->Group = 0;
    memset(student->ID, '\0', sizeof(student->ID));
}

void PrintRow(int fd, int pos) {
    int totalStudentsAmount = listSize;
    if (pos < -totalStudentsAmount|| pos >= listSize) {
        fprintf(stderr, "Invalid position\n");
        return;
    }

    if (pos < 0) {
    pos = totalStudentsAmount + pos;
    }

    printStudent(&ptr[pos]);
}

void PrintRows(int fd) {
    for (int i = 0; i < listSize; i++) {
        printf("Row %d: ", i);
        printStudent(&ptr[i]);
    }
}

void CloseFile(int fd) {
    if (ptr) {
        munmap(ptr, fileSize);
    }
    if (fd != -1) {
        close(fd);
    }
    printf("File successfully closed.\n");
}
