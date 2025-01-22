#include <windows.h>
#include <stdio.h>

int main()
{
  // удаляем файл
  if(!DeleteFile("C:\\demo_file.dat"))
  {
    fprintf(stderr, "Delete file failed.\nThe last error code: %ld\n",
            GetLastError());
    fputs("Press any key to finish.\n", stdout);
    getchar();
    return 0;
  }
  fputs("The file is deleted.\n", stdout);

  return 0;
}