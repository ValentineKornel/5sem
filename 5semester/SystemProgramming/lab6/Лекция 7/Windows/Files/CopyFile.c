#include <windows.h>
#include <stdio.h>

int main()
{
  // копируем файл
  if(!CopyFile("C:\\demo_file.dat", "C:\\new_file.dat", FALSE))
  {
    fprintf(stderr, "Copy file failed.\nThe last error code: %ld\n", GetLastError());
    fputs("Press any key to finish.\n", stdout);
    getchar();
    return 0;
  }

  fputs("The file is copied.\n", stdout);

  return 0;
}