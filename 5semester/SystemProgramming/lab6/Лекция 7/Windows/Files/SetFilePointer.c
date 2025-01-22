#include <stdio.h>
#include <windows.h>


int main() {
  HANDLE hFile;      // дескриптор файла
  long n;            // для номера записи
  long p;            // для указателя позиции
  DWORD dwBytesRead; // количество прочитанных байт
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
  // сдвигаем указатель позиции файла
  p = SetFilePointer(hFile, n * sizeof(int), NULL, FILE_BEGIN);
  if (p == -1) {
    fprintf(stderr, "Set file pointer failed.\nThe last error code: %ld\n",
            GetLastError());
    CloseHandle(hFile);
    fputs("Press any key to finish.\n", stdout);
    getchar();
    return 0;
  }
  // выводим на консоль значение указателя позиции файла
  fprintf(stdout, "File pointer: %ld\n", p);
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