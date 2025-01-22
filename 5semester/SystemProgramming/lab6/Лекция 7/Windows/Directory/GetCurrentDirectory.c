#include <windows.h>
#include <stdio.h>

int main()
{
  DWORD  dwNumberOfChar;
  char  szDirName[MAX_PATH];

  // определяем имя текущего каталога
  dwNumberOfChar = GetCurrentDirectory(MAX_PATH, szDirName);
  if (dwNumberOfChar == 0)
  {
    fprintf(stderr, "Get current directory failed.\nThe last error code: %ld\n", GetLastError());
    fputs("Press any key to finish.\n", stdout);
    getchar();

    return 0;
  }
  // выводим на консоль имя текущего каталога
  fprintf(stdout, "Current directory name: %s\n", szDirName);
  // устанавливаем текущий каталог для удаления из него файлов
  if (!SetCurrentDirectory("C:\\demo_dir"))
  {
    fprintf(stderr, "Set current directory failed.\nThe last error code: %ld\n", GetLastError());
    fputs("Press any key to finish.\n", stdout);
    getchar();

    return 0;
  }
  // определяем имя нового текущего каталога
  dwNumberOfChar = GetCurrentDirectory(MAX_PATH, szDirName);
  if (dwNumberOfChar == 0)
  {
    fprintf(stderr, "Get current directory failed.\nThe last error code: %ld\n", GetLastError());
    fputs("Press any key to finish.\n", stdout);
    getchar();

    return 0;
  }
  // выводим на консоль имя нового текущего каталога
  fprintf(stdout, "Current directory name: %s\n", szDirName);

  return 0;
}