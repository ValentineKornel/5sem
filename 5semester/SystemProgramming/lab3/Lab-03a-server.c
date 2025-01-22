#include <windows.h>
#include <stdio.h>
#include <conio.h>
#pragma warning(disable : 4996)

int main(int argc, char* argv[])
{
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <nubmer of process> <start> <end> [mutex name]\n", argv[0]);
        return 1;
    }

    char mutex_name[20];
    if (argv[4] != NULL) {
        strncpy(mutex_name, argv[4], sizeof(mutex_name) - 1);
        mutex_name[sizeof(mutex_name) - 1] = '\0';
    }
    else
        if (!GetEnvironmentVariableA("MUTEX_NAME", mutex_name, sizeof(mutex_name))) {
            _cprintf("env error");
            return 1;
        }

    HANDLE mutex = CreateMutexA(NULL, FALSE, mutex_name);
    if (mutex == NULL) {
        printf("Error createing mutex: %d\n", GetLastError());
        return 1;
    }
    HANDLE lpHandles[10];

    int numberOfProcess = atoi(argv[1]);
    int start = atoi(argv[2]);
    int end = atoi(argv[3]);

    int range = end - start + 1;
    int step = range / numberOfProcess;


    
    HANDLE hWritePipe, hReadPipe;

    // создаем анонимный канал
    if (!CreatePipe(
        &hReadPipe,    // дескриптор для чтения
        &hWritePipe,   // дескриптор для записи
        NULL,          // атрибуты защиты по умолчанию, в этом случае 
        // дескрипторы hReadPipe и hWritePipe ненаследуемые
        0))            // размер буфера по умолчанию

    {
        _cputs("Create pipe failed.\n");
        _cputs("Press any key to finish.\n");
        _getch();
        return GetLastError();
    }


    for (int i = 0; i < numberOfProcess; i++) {
        int local_start = start + i * step;
        int local_end = local_start + step - 1;
        if (i == numberOfProcess-1)
            local_end = end;
        printf("i: %d: range %d - %d\n", i, local_start, local_end);

        char lpszComLine[100];  // для командной строки

        HANDLE hInheritWritePipe;
        
        //делаем наследуемый дубликат дескриптора hWritePipe
        if (!DuplicateHandle(
            GetCurrentProcess(),   // дескриптор текущего процесса
            hWritePipe,            // исходный дескриптор канала
            GetCurrentProcess(),   // дескриптор текущего процесса
            &hInheritWritePipe,    // новый дескриптор канала
            0,                     // этот параметр игнорируется
            TRUE,                  // новый дескриптор наследуемый
            DUPLICATE_SAME_ACCESS))  // доступ не изменяем
        {
            _cputs("Duplicate handle failed.\n");
            _cputs("Press any key to finish.\n");
            _getch();
            return GetLastError();
        }

        STARTUPINFOA si;
        PROCESS_INFORMATION pi;
        // устанавливаем атрибуты нового процесса
        ZeroMemory(&si, sizeof(STARTUPINFOA));
        si.cb = sizeof(STARTUPINFOA);
        si.dwFlags = STARTF_USESTDHANDLES;
        si.hStdOutput = hInheritWritePipe;

        // формируем командную строку
        sprintf_s(lpszComLine, 80, "Lab-03a-client.exe %d %d %s", local_start, local_end, mutex_name);
        printf("%s\n", lpszComLine);
        // запускаем новый консольный процесс
        if (!CreateProcessA(
            NULL,    // имя процесса
            lpszComLine,   // командная строка
            NULL,    // атрибуты защиты процесса по умолчанию
            NULL,    // атрибуты защиты первичного потока по умолчанию
            TRUE,    // наследуемые дескрипторы текущего процесса
            // наследуются новым процессом
            CREATE_NEW_CONSOLE,  // новая консоль  
            NULL,    // используем среду окружения процесса предка
            NULL,    // текущий диск и каталог, как и в процессе предке
            &si,     // вид главного окна - по умолчанию
            &pi      // здесь будут дескрипторы и идентификаторы
            // нового процесса и его первичного потока
        ))
        {
            _cputs("Create process failed.\n");
            _cputs("Press any key to finish.\n");
            _getch();
            return GetLastError();
        }
        lpHandles[i] = pi.hProcess;
        // закрываем дескрипторы нового процесса
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        // закрываем ненужный дескриптор канала
        CloseHandle(hInheritWritePipe);
    }


    // читаем из анонимного канала
    for (int i = 0; i < numberOfProcess; i++)
    {
        char nData[250];
        DWORD dwBytesRead;
        if (!ReadFile(
            hReadPipe,
            &nData,
            sizeof(nData),
            &dwBytesRead,
            NULL))
        {
            _cputs("Read from the pipe failed.\n");
            _cputs("Press any key to finish.\n");
            _getch();
            return GetLastError();
        }
        _cprintf("The data read from the pipe: %s\n", nData);
    }

    WaitForMultipleObjects(numberOfProcess, lpHandles, TRUE, INFINITE);
    _cputs("Press any key to exit.\n");
    _getch();
    CloseHandle(hWritePipe);
    CloseHandle(hReadPipe);
    CloseHandle(mutex);
    return 0;
    
}