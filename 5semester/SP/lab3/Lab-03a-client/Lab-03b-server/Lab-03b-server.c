#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <locale.h>
#pragma warning(disable : 4996)

#define BUFFER_SIZE 10

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "Russian");

    char pipe_name[80];
    if (argv[1] != NULL) {
        strncpy(pipe_name, argv[1], sizeof(pipe_name) - 1);
        pipe_name[sizeof(pipe_name) - 1] = '\0';
    }
    else
        if (!GetEnvironmentVariableA("PIPE_NAME", pipe_name, sizeof(pipe_name))) {
            _cprintf("env error");
            return 1;
        }

    HANDLE semaphore = CreateSemaphoreA(
        NULL, 1, 1, "Global\\MySemaphore"
    );
    if (semaphore == NULL) {
        printf("CreateSemaphore failed: %d\n", GetLastError());
        return 1;
    }

    HANDLE hNamedPipe;
    DWORD dwBytesRead; // дл€ количества прочитанных байтов
    DWORD dwBytesWrite; // дл€ количества записанных байтов
    char pchMessage[BUFFER_SIZE]; // дл€ сообщени€
    int nMessageLength;  // длина сообщени€

    // создаем именованный канал дл€ чтени€ и записи
    hNamedPipe = CreateNamedPipeA(
        pipe_name, // им€ канала
        PIPE_ACCESS_DUPLEX, // читаем из канала и пишем в канал
        PIPE_TYPE_MESSAGE | PIPE_WAIT, // синхронна€ передача сообщений
        1, // максимальное количество экземпл€ров канала
        BUFFER_SIZE, // размер выходного буфера по умолчанию
        BUFFER_SIZE, // размер входного буфера по умолчанию
        INFINITE, // клиент ждет св€зь бесконечно долго
        NULL      // безопасность по умолчанию
    );
    // провер€ем на успешное создание
    if (hNamedPipe == INVALID_HANDLE_VALUE) {
        _cprintf("Create named pipe failed.\n");
        _cprintf("The last error code: %d.\n", GetLastError());
        _cprintf("Press any key to exit.\n");
        _getch();

        return 0;
    }



    for (;;) {
        // ждем, пока клиент св€жетс€ с каналом
        _cprintf("The server is waiting for connection with a client.\n");
        ;
        if (!ConnectNamedPipe(hNamedPipe, // дескриптор канала
            NULL        // св€зь синхронна€
        )) {
            _cprintf("Connect named pipe failed.\n");
            _cprintf("The last error code: %d.\n", GetLastError());
            CloseHandle(hNamedPipe);
            _cprintf("Press any key to exit.\n");
            _getch();

            return 0;
        }



        char fullMessage[1024] = { 0 };
        int totalBytesRead = 0;

        do {
            // читаем сообщение от клиента
            if (!ReadFile(hNamedPipe, // дескриптор канала
                pchMessage, // адрес буфера дл€ ввода данных
                sizeof(pchMessage), // количество читаемых байтов
                &dwBytesRead, // количество прочитанных байтов
                NULL)) // передача данных синхронна€
            {
                _cprintf("Data reading from the named pipe failed.\n");
                _cprintf("The last error code: %d.\n", GetLastError());
                CloseHandle(hNamedPipe);
                _cprintf("Press any key to exit.\n");
                _getch();

                return 0;
            }

            //  опируем прочитанную часть в итоговое сообщение
            strncat(fullMessage, pchMessage, dwBytesRead);
            totalBytesRead += dwBytesRead;

        } while (dwBytesRead == BUFFER_SIZE);
        _cprintf("The server received the message from a client: \n\t%s\n",
            fullMessage);

        for (int i = 0; fullMessage[i] != '\0'; i++) {
            fullMessage[i] = toupper(fullMessage[i]);
        }
        _cprintf("Formatted message: \n\t%s\n", fullMessage);


        nMessageLength = strlen(fullMessage);
        int totalBytesSent = 0;

        while (totalBytesSent < nMessageLength) {
            int chunkSize = (nMessageLength - totalBytesSent > BUFFER_SIZE) ? BUFFER_SIZE : nMessageLength - totalBytesSent;
            if (!WriteFile(hNamedPipe, // дескриптор канала
                fullMessage + totalBytesSent, // адрес буфера дл€ вывода данных
                chunkSize, // количество записываемых байтов
                &dwBytesWrite, // количество записанных байтов
                NULL // передача данных синхронна€
            )) {
                _cprintf("Write file failed.\n");
                _cprintf("The last error code: %d.\n", GetLastError());
                CloseHandle(hNamedPipe);
                _cprintf("Press any key to exit.\n");
                _getch();

                return 0;
            }

            totalBytesSent += chunkSize;
        }

        /*_cprintf("Press any key to continue.\n");
        _getch();*/

        DisconnectNamedPipe(hNamedPipe);
    }
    CloseHandle(hNamedPipe);
    CloseHandle(semaphore);

    return 0;

}