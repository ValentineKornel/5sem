#include <windows.h>
#include <stdio.h>

int main()
{
  HANDLE  hFile;
  LARGE_INTEGER  liFileSize;  // размер файла

  // открываем файл для чтения
  hFile = CreateFile(
    "C:\\demo_file.dat",   // имя файла
    GENERIC_READ,          // чтение из файла
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
  if (!GetFileSizeEx(hFile, &liFileSize)){
    fprintf(stderr, "Get file size failed.\nThe last error code: %ld\n",
            GetLastError());
    CloseHandle(hFile);
    fputs("Press any key to finish.\n", stdout);
    getchar();

    return 0;
  }
  // выводим размер файла
  fprintf(stdout, "File size: %ld\n", liFileSize.LowPart);
  // закрываем дескриптор файла 
  CloseHandle(hFile);

  return 0;
}