#include <windows.h>
#include <stdio.h>

int main()
{
  HANDLE  hFile;
  DWORD  dwFileSize;
  
  // открываем файл для записи
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
  // блокируем файл
  if (!LockFile(hFile, 0, 0, dwFileSize, 0))
  {
    fprintf(stderr, "Lock file failed.\nThe last error code: %ld\n",
            GetLastError());
    CloseHandle(hFile);
    fputs("Press any key to finish.\n", stdout);
    getchar();

    return 0;
  }
  fputs("Now the file is locked.\nPress any key to continue.\n", stdout);
  getchar();
  // разблокируем файл
  if (!UnlockFile(hFile, 0, 0, dwFileSize, 0))
  {
    fprintf(stderr, "Unlock file failed.\nThe last error code: %ld\n",
            GetLastError());
    CloseHandle(hFile);
    fputs("Press any key to finish.\n", stdout);
    getchar();

    return 0;
  }
  fputs("Now the file is unlocked.\nPress any key to continue.\n", stdout);
  getchar();
  // закрываем дескриптор файла 
  CloseHandle(hFile);

  return 0;
}