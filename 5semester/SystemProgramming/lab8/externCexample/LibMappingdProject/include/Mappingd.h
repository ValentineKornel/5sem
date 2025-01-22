/***************************************************************************/
#ifdef MYLIBAPI
// MYLIBAPI должен быть определен во всех модулях исходного кода DLL
// до включения этого файла
// здесь размещаются все экспортируемые функции и переменные
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#pragma warning(disable : 4996)
#else
// этот заголовочный файл включается в исходный код EXE-файла;
// указываем, что все функции и переменные импортируются
#define MYLIBAPI extern __declspec(dllimport)
#endif
////////////////////////////////////////////////////////////////////////////
// здесь определяются все структуры данных и идентификаторы (символы)
////////////////////////////////////////////////////////////////////////////
// Здесь определяются экспортируемые переменные.
// Примечание: избегайте экспорта переменных.
struct Student {
    char Name[64];
    char Surname[128];
    unsigned char Course;
    unsigned char Group;
    char ID[8];
};
////////////////////////////////////////////////////////////////////////////
// здесь определяются прототипы экспортируемых функций
void MyOpenFile(HANDLE* hMapping, LPSTR filePath, LPINT lSize);
void AddRow(HANDLE hMapping, struct Student newStudent, int pos);
void RemRow(HANDLE hMapping, int pos);
void PrintRow(HANDLE hMapping, int pos);
void PrintRows(HANDLE hMapping);
void CloseFile(HANDLE hMapping);
////////////////////////////// Конец файла /////////////////////////////////