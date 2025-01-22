#include <windows.h>
#include <stdio.h>

int main()
{
  // удалаем каталог
  if (!RemoveDirectory("C:\\demo_dir"))
  {
    fprintf(stderr, "Remove directory failed.\nThe last error code: %ld\n", GetLastError());
    fputs("Press any key to finish.\n", stdout);
    getchar();

    return 0;
  }

  fputs("The directory is removed.\n", stdout);

  return 0;
}