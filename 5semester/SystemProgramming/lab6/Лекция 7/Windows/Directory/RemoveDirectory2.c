#include <windows.h>
#include <stdio.h>

int main()
{
  HANDLE  hFindFile;
  WIN32_FIND_DATA  fd;
  char  szFullFileName[MAX_PATH];

  // находим первый файл
  hFindFile = FindFirstFile("C:\\demo_dir\\*", &fd);
  if (hFindFile == INVALID_HANDLE_VALUE)
  {
    fprintf(stderr, "Find first file failed.\nThe last error code: %ld\n", GetLastError());
    fputs("Press any key to finish.\n", stdout);
    getchar();

    return 0;
  }
  // выводим на консоль имя первого файла
  fprintf(stdout, "The first file name: %s\n", fd.cFileName);
  // удаляем из каталога файлы
  while (FindNextFile(hFindFile, &fd))
  {
    // если это не подкаталог, то удаляем его
    if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
    {
      // формируем имя файла
      sprintf(szFullFileName, "C:\\demo_dir\\%s", fd.cFileName);
      // удаляем файл
      if (!DeleteFile(szFullFileName))
      {
        fprintf(stderr, "Delete file failed.\nThe last error code: %ld\n", GetLastError());
        fputs("Press any key to finish.\n", stdout);
        getchar();

        return 0;
      }
      else
        fprintf(stdout, "The next file: %s is deleted.\n", fd.cFileName);
    }
    else
    fprintf(stdout, "The next directory: %s is not deleted.\n", fd.cFileName);
  }
  // закрываем дескриптор поиска
  if (!FindClose(hFindFile))
  {
    fputs("Find close failed.\n", stderr);
	  return 0;
  }
  // удалаем каталог
  if (!RemoveDirectory("C:\\demo_dir"))
  {
    fprintf(stderr, "Remove directory failed.\nThe last error code: %ld\n", GetLastError());
    fputs("Press any key to finish.\n", stdout);
    getchar();

    return 0;
  }

  fputs("The directory is removed.\n", stdout);

  // закрываем дескриптор поиска
  FindClose(hFindFile);

  return 0;
}