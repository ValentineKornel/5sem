#include <windows.h>
#include <conio.h>
#include <string.h>

#pragma comment(lib, "user32.lib")

int
main()
{
  char machineName[80];
  char pipeName[80];
  HANDLE hNamedPipe;
  DWORD dwBytesWritten; // для количества записанных байтов
  DWORD dwBytesRead; // для количества прочитанных байтов
  char pchMessage[80]; // для сообщения
  int nMessageLength;  // длина сообщения

  // вводим имя машины в сети, на которой работает сервер
  _cprintf("Enter a name of the server machine:\n");
  _cscanf_s("%s", &machineName);
  // подставляем имя машины в имя канала
  wsprintfA(pipeName, "\\\\%s\\pipe\\demo_pipe", machineName);

  // связываемся с именованным каналом
  hNamedPipe =
    CreateFile(pipeName,                     // имя канала
               GENERIC_READ | GENERIC_WRITE, // читаем и записываем в канал
               FILE_SHARE_READ | FILE_SHARE_WRITE, // разрешаем чтение и запись
               NULL, // безопасность по умолчанию
               OPEN_EXISTING, // открываем существующий канал
               FILE_ATTRIBUTE_NORMAL, // атрибуты по умолчанию
               NULL); // дополнительных атрибутов нет

  // проверяем связь с каналом
  if (hNamedPipe == INVALID_HANDLE_VALUE) {
    _cprintf("Connection with the named pipe failed.\n");
    _cprintf("The last error code: %d.\n", GetLastError());
    _cprintf("Press any key to exit.\n");
    _getch();

    return 0;
  }

  // вводим строку
  _getch();
  _cprintf("Input a string:\n");
  _cscanf_s("%s", &pchMessage);
  // определяем длину строки
  nMessageLength = strlen(pchMessage) + 1;

  // пишем в именованный канал
  if (!WriteFile(hNamedPipe,     // дескриптор канала
                 pchMessage,     // данные
                 nMessageLength, // размер данных
                 &dwBytesWritten, // количество записанных байтов
                 NULL)) // синхронная запись
  {
    // ошибка записи
    _cprintf("Write file failed: \n");
    _cprintf("The last error code: %d.\n", GetLastError());
    CloseHandle(hNamedPipe);
    _cprintf("Press any key to exit.\n");
    _getch();

    return 0;
  }
  // выводим посланное сообщение на консоль
  _cprintf("The client sent the message to a server: \n\t%s\n", pchMessage);
  // читаем из именованного канала
  if (!ReadFile(hNamedPipe,         // дескриптор канала
                pchMessage,         // данные
                sizeof(pchMessage), // размер данных
                &dwBytesRead, // количество записанных байтов
                NULL))        // синхронное чтение
  {
    // ошибка чтения
    _cprintf("Read file failed: \n");
    _cprintf("The last error code: %d.\n", GetLastError());
    CloseHandle(hNamedPipe);
    _cprintf("Press any key to exit.\n");
    _getch();

    return 0;
  }
  // выводим полученное сообщение на консоль
  _cprintf("The client received the message from a server: \n\t%s\n",
           pchMessage);
  // закрываем дескриптор канала
  CloseHandle(hNamedPipe);
  // завершаем процесс
  _cprintf("Press any key to exit.\n");
  _getch();

  return 0;
}