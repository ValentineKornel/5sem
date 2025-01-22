
// #define _WIN32_WINNT 0x0500  // или определить этот макрос вместо функции,
// если новая платформа

#include <stdio.h>
#include <windows.h>


extern WINBASEAPI BOOL WINAPI SetFilePointerEx(HANDLE hFile,
                                               LARGE_INTEGER liDistanceToMove,
                                               PLARGE_INTEGER lpNewFilePointer,
                                               DWORD dwMoveMethod);

int main() {
  HANDLE hFile;       // дескриптор файла
  int n;              // для номера записи
  LARGE_INTEGER p, q; // для указателя позиции
  DWORD dwBytesRead; // количество прочитанных байтов
  int m;             // прочитанное число

  // открываем файл для чтения
  hFile = CreateFile("C:\\demo_file.dat", // имя файла
                     GENERIC_READ,        // чтение из файла
                     0,    // монопольный доступ к файлу
                     NULL, // защиты нет
                     OPEN_EXISTING, // открываем существующий файл
                     FILE_ATTRIBUTE_NORMAL, // обычный файл
                     NULL                   // шаблона нет
  );
  // проверяем на успешное открытие
  if (hFile == INVALID_HANDLE_VALUE) {
    fprintf(stderr, "Create file failed.\nThe last error code: %ld\n",
            GetLastError());
    fputs("Press any key to finish.\n", stdout);
    getchar();
    return 0;
  }

  // вводим номер нужной записи
  fputs("Input a number from 0 to 9: ", stdout);
  fscanf_s(stdin, "%ld", &n);
  q.HighPart = 0;
  q.LowPart = n * sizeof(int);
  // сдвигаем указатель позиции файла
  if (!SetFilePointerEx(hFile, q, &p, FILE_BEGIN)) {
    fprintf(stderr, "Set file pointer failed.\nThe last error code: %ld\n",
            GetLastError());
    CloseHandle(hFile);
    fputs("Press any key to finish.\n", stdout);
    getchar();
    return 0;
  }
  // выводим на консоль значение указателя позиции файла
  fprintf(stdout, "File pointer: %ld %ld\n", p.HighPart, p.LowPart);
  // читаем данные из файла
  if (!ReadFile(hFile, // дескриптор файла
                &m, // адрес буфера, куда читаем данные
                sizeof(m), // количество читаемых байтов
                &dwBytesRead, // количество прочитанных байтов
                (LPOVERLAPPED)NULL // чтение синхронное
                )) {
    fprintf(stderr, "Read file failed.\nThe last error code: %ld\n",
            GetLastError());
    CloseHandle(hFile);
    fputs("Press any key to finish.\n", stdout);
    getchar();

    return 0;
  }
  // выводим прочитанное число на консоль
  fprintf(stdout, "The read number: %d\n", m);
  // закрываем дескриптор файла
  CloseHandle(hFile);

  return 0;
}