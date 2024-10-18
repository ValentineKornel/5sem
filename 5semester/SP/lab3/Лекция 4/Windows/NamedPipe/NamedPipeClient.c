#include <Windows.h>
#include <stdio.h>
#include <conio.h>

int
main()
{
  HANDLE hNamedPipe;
  char pipeName[] = "\\\\.\\pipe\\demo_pipe";

  // связываемся с именованным каналом
  hNamedPipe =
    CreateFile(pipeName,      // имя канала
               GENERIC_WRITE, // записываем в канал
               FILE_SHARE_READ, // разрешаем одновременное чтение из канала
               NULL,          // защита по умолчанию
               OPEN_EXISTING, // открываем существующий канал
               0,             // атрибуты по умолчанию
               NULL // дополнительных атрибутов нет
    );

  // проверяем связь с каналом
  if (hNamedPipe == INVALID_HANDLE_VALUE) {
    _cprintf("Connection with the named pipe failed.\n");
    _cprintf("The last error code: %d.\n", GetLastError());
    _cputs("Press any key to exit.\n");

    return 0;
  }

  // пишем в именованный канал
  for (int i = 0; i < 10; i++) {
    DWORD dwBytesWritten;
    if (!WriteFile(hNamedPipe, // дескриптор канала
                   &i,         // данные
                   sizeof(i),  // размер данных
                   &dwBytesWritten, // количество записанных байтов
                   NULL // синхронная запись
                   )) {
      // ошибка записи
      _cprintf("Writing to the named pipe failed: \n");
      _cprintf("The last error code: %d.\n", GetLastError());
      CloseHandle(hNamedPipe);
      _cputs("Press any key to exit.\n");
      _getch();

      return 0;
    }
    // выводим число на консоль
    _cprintf("The number %d is written to the named pipe.\n", i);
    Sleep(1000);
  }
  // закрываем дескриптор канала
  CloseHandle(hNamedPipe);
  // завершаем процесс
  _cprintf("The data are written by the client.\n");
  _cprintf("Press any key to exit.");
  _getch();

  return 0;
}