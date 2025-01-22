#include <windows.h>
#include <stdio.h>

int main()
{
  // ןונולושאול פאיכ
  if(!MoveFile("C:\\demo_file.dat", "C:\\new_file.dat"))
  {
    fprintf(stderr, "Move file failed.\nThe last error code: %ld\n",
            GetLastError());
    fputs("Press any key to finish.\n", stdout);
    getchar();
    return 0;
  }

  fputs("The file is moved.\n", stdout);

  return 0;
}