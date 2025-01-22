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

void MyOpenFile(int fd, char filePath[100], int* listSize);
void AddRow(int fd, struct Student newStudent, int pos);
void RemRow(int fd, int pos);
void PrintRow(int fd, int pos);
void PrintRows(int fd);
void CloseFile(int fd);