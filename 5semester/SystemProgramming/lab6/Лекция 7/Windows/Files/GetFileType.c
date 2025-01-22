#include <windows.h>
#include <stdio.h>

int main()
{
  HANDLE  hFile;
  DWORD  dwFileType;

  // открываем файл для чтения
  hFile = CreateFile(
    "C:\\demo_file.dat",   // имя файла
    0,                     // получение информации о файле
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
  // определяем тип файла
  dwFileType = GetFileType(hFile);
    // распечатываем тип файла
  switch (dwFileType)
  {
  case FILE_TYPE_UNKNOWN:
    fputs("Unknown type file.\n", stdout);
    break;
  case FILE_TYPE_DISK:
    fputs("Disk type file.\n", stdout);
    break;
  case FILE_TYPE_CHAR:
    fputs("Char type file.\n", stdout);
    break;
  case FILE_TYPE_PIPE:
    fputs("Pipe type file.\n", stdout);
    break;
  default:
    break;
  }

  return 0;
}