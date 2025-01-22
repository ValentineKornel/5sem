#define _WIN32_WINNT 0x0500

#include <windows.h>
#include <stdio.h>

int main()
{
  // перемещаем файл
  if(!ReplaceFile(
    "C:\\demo_file.dat",       // имя замесщаемого файла
    "C:\\new_file.dat",        // имя файла заместителя
    "C:\\back_file.dat",       // имя резервного файла
    REPLACEFILE_WRITE_THROUGH, // освободить буферы
    NULL, NULL                 // не используются
    ))
  {
    fprintf(stderr, "Replace file failed.\nThe last error code: %ld\n",
            GetLastError());
    fputs("Press any key to finish.\n", stdout);
    getchar();
    return 0;
  }

  fputs("The file is replaced.\n", stdout);

  return 0;
}