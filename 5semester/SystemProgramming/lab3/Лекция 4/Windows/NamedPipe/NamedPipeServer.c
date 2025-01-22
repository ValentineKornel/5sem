#include <stdio.h>
#include <conio.h>
#include <windows.h>

int
main()
{
  HANDLE hNamedPipe;

  // создаем именованный канал для чтения
  hNamedPipe = CreateNamedPipe(
    "\\\\.\\pipe\\demo_pipe", // имя канала
    PIPE_ACCESS_INBOUND,      // читаем из канала
    PIPE_TYPE_MESSAGE | PIPE_WAIT, // синхронная передача сообщений
    1, // максимальное количество экземпляров канала
    0, // размер выходного буфера по умолчанию
    0, // размер входного буфера по умолчанию
    INFINITE, // клиент ждет связь бесконечно долго
    NULL      // защита по умолчанию
  );
  // проверяем на успешное создание
  if (hNamedPipe == INVALID_HANDLE_VALUE) {
    _cprintf("Create named pipe failed.\n");
    _cprintf("The last error code: %d.\n", GetLastError());
    _cputs("Press any key to exit.\n");
    _getch();

    return 0;
  }

  // ждем, пока клиент свяжется с каналом
  _cprintf("The server is waiting for connection with a client.\n");
  if (!ConnectNamedPipe(hNamedPipe, // дескриптор канала
                        NULL        // связь синхронная
                        )) {
    _cprintf("The connection failed.\n");
    _cprintf("The last error code: %d.\n", GetLastError());
    CloseHandle(hNamedPipe);
    _cputs("Press any key to exit.\n");
    _getch();

    return 0;
  }

  // читаем данные из канала
  for (int i = 0; i < 10; i++) {
    int nData;
    DWORD dwBytesRead;
    if (!ReadFile(hNamedPipe, // дескриптор канала
                  &nData, // адрес буфера для ввода данных
                  sizeof(nData), // число читаемых байтов
                  &dwBytesRead, // число прочитанных байтов
                  NULL // передача данных синхронная
                  )) {
      _cprintf("Read file failed.\n");
      _cprintf("The last error code: %d.\n", GetLastError());
      CloseHandle(hNamedPipe);
      _cputs("Press any key to exit.\n");
      _getch();

      return 0;
    }
    // выводим прочитанные данные на консоль
    _cprintf("The number %d was read by the server\n", nData);
  }
  // закрываем дескриптор канала
  CloseHandle(hNamedPipe);
  // завершаем процесс
  _cputs("The data are read by the server.\n");
  _cputs("Press any key to exit.\n");
  _getch();

  return 0;
}