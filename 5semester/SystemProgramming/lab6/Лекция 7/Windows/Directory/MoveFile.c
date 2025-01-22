#include <windows.h>
#include <stdio.h>

int main()
{
  // перемещаем каталог
  if(!MoveFile("C:\\demo_dir", "C:\\new_dir"))
  {
    fprintf(stderr, "Move file failed.\nThe last error code: %ld\n", GetLastError());
    fputs("Press any key to finish.\n", stdout);
    getchar();

    return 0;
  }

  fputs("The directory is moved.\n", stdout);

  return 0;
}