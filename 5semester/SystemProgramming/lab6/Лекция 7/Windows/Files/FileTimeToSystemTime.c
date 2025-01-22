#include <windows.h>
#include <stdio.h>

int main()
{
  HANDLE  hFile;
  BY_HANDLE_FILE_INFORMATION  bhfi;  // информация о файле
  SYSTEMTIME  st;    // системное время

  // открываем файл для чтения
  hFile = CreateFile(
    "C:\\demo_file.dat",   // имя файла
    0,                     // получение информации о файле
    0,                     // монопольный доступ к файлу
    NULL,                  // защиты нет
    OPEN_EXISTING,         // открываем существующий файл
    FILE_ATTRIBUTE_NORMAL, // обычный файл
    NULL                   // шаблона нет
  );
  // проверяем на успешное открытие
  if (hFile == INVALID_HANDLE_VALUE)
  {
    fprintf(stderr, "Create file failed.\nThe last error code: %ld\n",
            GetLastError());
    fputs("Press any key to finish.\n", stdout);
    getchar();

    return 0;
  }  
  // получаем информацию о файле
  if (!GetFileInformationByHandle(hFile, &bhfi))
  {
    fprintf(stderr, "Get file information by handle failed.\nThe last error code: %ld\n",
            GetLastError());
    fputs("Press any key to finish.\n", stdout);
    getchar();

    return 0;
  }
  // переводим время создания файла в системное время
  if (!FileTimeToSystemTime(&(bhfi.ftCreationTime), &st))
  {
    fprintf(stderr, "File time to system time failed.\nThe last error code: %ld\n",
            GetLastError());
    fputs("Press any key to finish.\n", stdout);
    getchar();

    return 0;
  }
  // распечатываем системное время
  fprintf(stdout, "File creation time in system format: \n");
  fprintf(stdout, "\tYear: %d\n", st.wYear);
  fprintf(stdout, "\tMonth: %d\n", st.wMonth);
  fprintf(stdout, "\tDay of week: %d\n", st.wDayOfWeek);
  fprintf(stdout, "\tDay: %d\n", st.wDay);
  fprintf(stdout, "\tHour: %d\n", st.wHour);
  fprintf(stdout, "\tMinute: %d\n", st.wMinute);
  fprintf(stdout, "\tSecond: %d\n", st.wSecond);
  fprintf(stdout, "\tMilliseconds: %d\n", st.wMilliseconds);

  // закрываем дескриптор файла 
  CloseHandle(hFile);

  return 0;
}