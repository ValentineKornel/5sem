#include <windows.h>
#include <conio.h>
#include <string.h>

int
main()
{
  HANDLE hNamedPipe;
  DWORD dwBytesRead; // для количества прочитанных байтов
  DWORD dwBytesWrite; // для количества записанных байтов
  char pchMessage[80]; // для сообщения
  int nMessageLength;  // длина сообщения

  // создаем именованный канал для чтения и записи
  hNamedPipe = CreateNamedPipe(
    "\\\\.\\pipe\\demo_pipe", // имя канала
    PIPE_ACCESS_DUPLEX, // читаем из канала и пишем в канал
    PIPE_TYPE_MESSAGE | PIPE_WAIT, // синхронная передача сообщений
    1, // максимальное количество экземпляров канала
    0, // размер выходного буфера по умолчанию
    0, // размер входного буфера по умолчанию
    INFINITE, // клиент ждет связь бесконечно долго
    NULL      // безопасность по умолчанию
  );
  // проверяем на успешное создание
  if (hNamedPipe == INVALID_HANDLE_VALUE) {
    _cprintf("Create named pipe failed.\n");
    _cprintf("The last error code: %d.\n", GetLastError());
    _cprintf("Press any key to exit.\n");
    _getch();

    return 0;
  }

  // ждем, пока клиент свяжется с каналом
  _cprintf("The server is waiting for connection with a client.\n");
  ;
  if (!ConnectNamedPipe(hNamedPipe, // дескриптор канала
                        NULL        // связь синхронная
                        )) {
    _cprintf("Connect named pipe failed.\n");
    _cprintf("The last error code: %d.\n", GetLastError());
    CloseHandle(hNamedPipe);
    _cprintf("Press any key to exit.\n");
    _getch();

    return 0;
  }
  // читаем сообщение от клиента
  if (!ReadFile(hNamedPipe, // дескриптор канала
                pchMessage, // адрес буфера для ввода данных
                sizeof(pchMessage), // количество читаемых байтов
                &dwBytesRead, // количество прочитанных байтов
                NULL)) // передача данных синхронная
  {
    _cprintf("Data reading from the named pipe failed.\n");
    _cprintf("The last error code: %d.\n", GetLastError());
    CloseHandle(hNamedPipe);
    _cprintf("Press any key to exit.\n");
    _getch();

    return 0;
  }

  // выводим полученное от клиента сообщение на консоль
  _cprintf("The server received the message from a client: \n\t%s\n",
           pchMessage);

  // вводим строку
  _cprintf("Input a string:\n");
  _cscanf_s("%s", &pchMessage);
  // определяем длину строки
  nMessageLength = strlen(pchMessage) + 1;

  // отвечаем клиенту
  if (!WriteFile(hNamedPipe, // дескриптор канала
                 pchMessage, // адрес буфера для вывода данных
                 nMessageLength, // количество записываемых байтов
                 &dwBytesWrite, // количество записанных байтов
                 NULL // передача данных синхронная
                 )) {
    _cprintf("Write file failed.\n");
    _cprintf("The last error code: %d.\n", GetLastError());
    CloseHandle(hNamedPipe);
    _cprintf("Press any key to exit.\n");
    _getch();

    return 0;
  }

  // выводим посланное клиенту сообщение на консоль
  _cprintf("The server sent the message to a client:\n\t%s\n", pchMessage);
  // закрываем дескриптор канала
  CloseHandle(hNamedPipe);
  // завершаем процесс
  _cprintf("Press any key to exit.\n");
  _getch();

  return 0;
}