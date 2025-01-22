#include <stdio.h>
#include <windows.h>


int main() {
  HANDLE hFindFile;
  WIN32_FIND_DATA fd;

  // находим первый файл
  hFindFile = FindFirstFile("C:\\demo_dir\\*", &fd);
  if (hFindFile == INVALID_HANDLE_VALUE) {
    fprintf(stderr, "Find first file failed.\nThe last error code: %ld\n",
            GetLastError());
    fputs("Press any key to finish.\n", stdout);
    getchar();

    return 0;
  }
  // выводим на консоль имя первого файла
  fprintf(stdout, "The first file name: %s\n", fd.cFileName);
  // находим следующий файл и выводим на консоль его имя
  while (FindNextFile(hFindFile, &fd))
    fprintf(stdout, "The next file name: %s\n", fd.cFileName);
  // закрываем дескриптор поиска
  FindClose(hFindFile);

  return 0;
}