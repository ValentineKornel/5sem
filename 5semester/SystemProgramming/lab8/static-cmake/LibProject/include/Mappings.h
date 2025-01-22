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

void MyOpenFile(HANDLE* hMapping, LPSTR filePath, LPINT lSize);
void AddRow(HANDLE hMapping, struct Student newStudent, int pos);
void cleanStudent(struct Student* student);
void RemRow(HANDLE hMapping, int pos);
void printStudent(struct Student* student);
void PrintRow(HANDLE hMapping, int pos);
void PrintRows(HANDLE hMapping);
void CloseFile(HANDLE hMapping);
