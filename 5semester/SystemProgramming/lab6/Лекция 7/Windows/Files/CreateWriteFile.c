#include <windows.h>
#include <stdio.h>

int main()
{
  HANDLE hFile;
  
  // создаем файл для записи данных
  hFile = CreateFile(
    "C:\\demo_file.dat",   // имя файла
    GENERIC_WRITE,         // запись в файл
    0,                     // монопольный доступ к файлу
    NULL,                  // защиты нет 
    CREATE_NEW,            // создаем новый файл
    FILE_ATTRIBUTE_NORMAL, // обычный файл
    NULL                   // шаблона нет
  );
  // проверяем на успешное создание
  if (hFile == INVALID_HANDLE_VALUE)
  {
    fprintf(stderr, "Create file failed.\nThe last error code: %ld\n",
            GetLastError());
    fputs("Press any key to finish.\n", stdout);
    getchar();
    return 0;
  }
  
  // пишем данные в файл
  for (int i = 0; i < 10; ++i)
  {
    DWORD dwBytesWrite;

    if (!WriteFile(
        hFile,               // дескриптор файла
        &i,                  // адрес буфера, откуда идет запись
        sizeof(i),           // количество записываемых байтов
        &dwBytesWrite,       // количество записанных байтов
        (LPOVERLAPPED)NULL   // запись синхронная
      ))
    {
      fprintf(stderr, "Write file failed.\nThe last error code: %ld\n", GetLastError());
      CloseHandle(hFile);
      fputs("Press any key to finish.\n", stdout);
      getchar();
      return 0;
    }
  }
  // закрываем дескриптор файла 
  CloseHandle(hFile);

  fputs("The file is created and written.\n", stdout);

  return 0;
}