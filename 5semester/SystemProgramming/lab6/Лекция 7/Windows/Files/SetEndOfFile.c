#include <windows.h>
#include <stdio.h>

int main()
{
  HANDLE  hFile;       // дескриптор файла
  DWORD   dwFileSize;  // размер файла
  long    p;           // указатель позиции
  
  // открываем файл для чтения
  hFile = CreateFile(
    "C:\\demo_file.dat",   // имя файла
    GENERIC_WRITE,         // запись в файл
    0,                     // монопольный доступ к файлу
    NULL,                  // защиты нет 
    OPEN_EXISTING,         // открываем существующий файл
    FILE_ATTRIBUTE_NORMAL, // обычный файл
    NULL                   // шаблона нет
  );
  // проверяем на успешное открытие
  if (hFile == INVALID_HANDLE_VALUE)
  {
    fprintf(stderr, "Create file failed.\nThe last error code: %ld\n",
            GetLastError());
    fputs("Press any key to finish.\n", stdout);
    getchar();

    return 0;
  }
  // определяем размер файла
  dwFileSize = GetFileSize(hFile, NULL);
  if (dwFileSize == -1)
  {
    fprintf(stderr, "Get file size failed.\nThe last error code: %ld\n",
            GetLastError());
    CloseHandle(hFile);
    fputs("Press any key to finish.\n", stdout);
    getchar();

    return 0;
  }
  // выводим на консоль размер файла
  fprintf(stdout, "Old file size: %ld\n", dwFileSize);
  // уменьшаем размер файла вдвое
  dwFileSize /= 2;
  // сдвигаем указатель позиции файла
  p = SetFilePointer(hFile, dwFileSize, NULL, FILE_BEGIN);
  if(p == -1)
  {
    fprintf(stderr, "Set file pointer failed.\nThe last error code: %ld\n",
            GetLastError());
    CloseHandle(hFile);
    fputs("Press any key to finish.\n", stdout);
    getchar();

    return 0;
  }
  // устанавливаем новый размер файла
  if (!SetEndOfFile(hFile))
  {
    fprintf(stderr, "Set end of file failed.\nThe last error code: %ld\n",
            GetLastError());
    CloseHandle(hFile);
    fputs("Press any key to finish.\n", stdout);
    getchar();

    return 0;
  }
  // определяем новый размер файла
  dwFileSize = GetFileSize(hFile, NULL);
  if (dwFileSize == -1)
  {
    fprintf(stderr, "Get file size failed.\nThe last error code: %ld\n",
            GetLastError());
    CloseHandle(hFile);
    fputs("Press any key to finish.\n", stdout);
    getchar();

    return 0;
  }
  // выводим на консоль размер файла
  fprintf(stdout, "New file size: %ld\n", dwFileSize);
  // закрываем дескриптор файла 
  CloseHandle(hFile);

  return 0;
}