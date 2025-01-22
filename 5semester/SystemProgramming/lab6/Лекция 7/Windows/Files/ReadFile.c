#include <windows.h>
#include <stdio.h>

int main()
{
  HANDLE  hFile;

  // открываем файл для чтения
  hFile = CreateFile(
    "C:\\demo_file.dat",     // имя файла
    GENERIC_READ,            // чтение из файла
    0,                       // монопольный доступ к файлу
    NULL,                    // защиты нет 
    OPEN_EXISTING,           // открываем существующий файл
    FILE_ATTRIBUTE_NORMAL,   // обычный файл
    NULL        // шаблона нет
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
  
  // читаем данные из файла
  for (;;)
  {
    DWORD  dwBytesRead;
    int    n;
    
    // читаем одну запись
    if (!ReadFile(
        hFile,               // дескриптор файла
        &n,                  // адрес буфера, куда читаем данные
        sizeof(n),           // количество читаемых байтов
        &dwBytesRead,        // количество прочитанных байтов
        (LPOVERLAPPED)NULL   // чтение синхронное
      ))
    {
      fprintf(stderr, "Read file failed.\nThe last error code: %ld\n", GetLastError());
      CloseHandle(hFile);
      fputs("Press any key to finish.\n", stdout);
      getchar();
      return 0;
    }
    // проверяем на конец файла
    if (dwBytesRead == 0)
      // если да, то выходим из цикла
      break;
    else
      // иначе выводим запись на консоль
      fprintf(stdout, "%d ", n);
  }

  fputs("\n", stdout);

  // закрываем дескриптор файла 
  CloseHandle(hFile);

  fputs("The file is opened and read.\n", stdout);

  return 0;
}