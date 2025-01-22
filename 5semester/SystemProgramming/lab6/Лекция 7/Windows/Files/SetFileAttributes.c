#include <windows.h>
#include <stdio.h>

int main()
{
  DWORD  file_attr;

  // читаем атрибуты файла
  file_attr = GetFileAttributes("C:\\demo_file.dat");
  if(file_attr == -1)
  {
    fprintf(stderr, "Get file attributes failed.\nThe last error code: %ld\n",
            GetLastError());
    fputs("Press any key to finish.\n", stdout);
    getchar();
    return 0;
  }
  // проверяем, является ли файл нормальным
  if(file_attr == FILE_ATTRIBUTE_NORMAL)
    fputs("This is a normal file.\n", stdout);
  else
  {
    fputs("This is a not normal file.\n", stdout);
    return 0;
  }
  // устанавливаем атрибут скрытого файла
  if(!SetFileAttributes("C:\\demo_file.dat", FILE_ATTRIBUTE_HIDDEN))
  {
    fprintf(stderr, "Set file attributes failed.\nThe last error code: %ld\n",
            GetLastError());
    fputs("Press any key to finish.\n", stdout);
    getchar();

    return 0;
  }
  // Теперь можно проверить, что файл скрылся
  fputs("Now the file is hidden.\nPress any key to continue.", stdout);
  getchar();
  // Обратно делаем файл обычным
  if(!SetFileAttributes("C:\\demo_file.dat", FILE_ATTRIBUTE_NORMAL))
  {
    fprintf(stderr, "Set file attributes failed.\nThe last error code: %ld\n",
            GetLastError());
    fputs("Press any key to finish.\n", stdout);
    getchar();

    return 0;
  }
  fputs("Now the file is again normal.\n", stdout);
  
  return 0;
}