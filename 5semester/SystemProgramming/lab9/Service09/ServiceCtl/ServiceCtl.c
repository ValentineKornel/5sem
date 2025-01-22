#include <stdio.h>
#include <windows.h>
#pragma warning(disable : 4996)

int main(DWORD argc, LPTSTR* argv) {

    if (argc < 3) {
        fprintf(stdout, "Not enough parameters. Usage: %s <service name> <operation> [parameters]\n", argv[0]);
        return 0;
    }

    const char* operation = argv[2];

    if (strcmp(operation, "create") == 0) {
        if (argc < 4) {
            fprintf(stdout, "Not enough parameters. Usage: %s <service name> create <BinPath>\n", argv[0]);
            return 0;
        }

        SC_HANDLE hServiceControlManager, hService;

        // связываемся с менеджером сервисов
        hServiceControlManager =
            OpenSCManager(NULL, // локальная машина
                NULL, // активная база данных сервисов
                SC_MANAGER_ALL_ACCESS // возможно создание сервиса
            );
        if (hServiceControlManager == NULL) {
            fprintf(stdout,
                "Open service control manager failed.\n"
                "The last error code: %lu\n",
                GetLastError());

            return 0;
        }
        fprintf(stdout, "Service control manager is opened.\n");

        // устанавливаем новый сервис
        hService =
            CreateServiceA(hServiceControlManager, // дескриптор менеджера сервисов
                argv[1], // внутреннее имя сервиса, используемое SCM
                argv[1], // внешнее имя сервиса в панели управления
                SERVICE_ALL_ACCESS, // полный контроль над сервисом
                SERVICE_WIN32_OWN_PROCESS, // сервис является процессом
                SERVICE_DEMAND_START, // запускается сервис по требованию
                SERVICE_ERROR_NORMAL, // обработка ошибок нормальная
                argv[3], // путь к сервису
                NULL, // сервис не принадлежит к группе
                NULL, // тэг группы не изменяется
                NULL, // сервис не зависит от других сервисов
                NULL, // имя совпадает с текущим именем учетной записи
                NULL  // пароля нет
            );
        if (hService == NULL) {
            fprintf(stdout,
                "Create service failed.\n"
                "The last error code: %lu\n",
                GetLastError());

            CloseServiceHandle(hServiceControlManager);

            return 0;
        }

        fprintf(stdout, "Service is installed.\n");

        CloseServiceHandle(hService);
        CloseServiceHandle(hServiceControlManager);

    }
    else if (strcmp(operation, "start") == 0) {
        if (argc < 5) {
            fprintf(stdout, "Not enough parameters. Usage: %s <service name> start <logPath> <watchPath>\n", argv[0]);
            return 0;
        }
        SC_HANDLE hServiceControlManager, hService;

        char* param[] = { argv[3], argv[4] };  // список параметров

        // связываемся с менеджером сервисов
        hServiceControlManager =
            OpenSCManager(NULL, // локальная машина
                NULL, // активная база данных сервисов
                SC_MANAGER_CONNECT // соединение с менеджером сервисов
            );
        if (hServiceControlManager == NULL) {
            fprintf(stdout,
                "Open service control manager failed.\n"
                "The last error code: %lu\n",
                GetLastError());

            return 0;
        }

        fprintf(stdout, "Service control manager is opened.\n");

        // открываем сервис
        hService =
            OpenServiceA(hServiceControlManager, // дескриптор менеджера сервисов
                argv[1],      // имя сервиса
                SERVICE_ALL_ACCESS // полный доступ к сервису
            );
        if (hService == NULL) {
            fprintf(stdout,
                "Open service failed.\n"
                "The last error code: %lu\n",
                GetLastError());

            CloseServiceHandle(hServiceControlManager);

            return 0;
        }

        fprintf(stdout, "Service is opened.\n");

        // стартуем сервис
        if (!StartServiceW(hService, // дескриптор сервиса
            2,        // три параметра
            (const char**)param // указатель на массив параметров
        )) {
            fprintf(stdout,
                "Start service failed.\n"
                "The last error code: %lu\n",
                GetLastError());

            CloseServiceHandle(hServiceControlManager);
            CloseServiceHandle(hService);

            return 0;
        }

        fprintf(stdout, "The service is started.\n");

        CloseServiceHandle(hServiceControlManager);
        CloseServiceHandle(hService);

        return 0;
    }
    else if (strcmp(operation, "stop") == 0) {
        SC_HANDLE hServiceControlManager, hService;
        SERVICE_STATUS service_status;

        // связываемся с менеджером сервисов
        hServiceControlManager =
            OpenSCManager(NULL, // локальная машина
                NULL, // активная база данных сервисов
                SC_MANAGER_CONNECT // соединение с менеджером сервисов
            );
        if (hServiceControlManager == NULL) {
            fprintf(stdout,
                "Open service control manager failed.\n"
                "The last error code: %lu\n",
                GetLastError());

            return 0;
        }

        fprintf(stdout, "Service control manager is opened.\n");

        // открываем сервис
        hService =
            OpenServiceA(hServiceControlManager, // дескриптор менеджера сервисов
                argv[1],      // имя сервиса
                SERVICE_ALL_ACCESS // полный доступ к сервису
            );
        if (hService == NULL) {
            fprintf(stdout,
                "Open service failed.\n"
                "The last error code: %lu\n",
                GetLastError());

            CloseServiceHandle(hServiceControlManager);

            return 0;
        }

        fprintf(stdout, "Service is opened.\n");

        // получаем состояние сервиса
        if (!QueryServiceStatus(hService, &service_status)) {
            fprintf(stdout,
                "Open service status failed.\n"
                "The last error code: %lu\n",
                GetLastError());
            CloseServiceHandle(hServiceControlManager);
            CloseServiceHandle(hService);

            return 0;
        }

        // если сервис работает, то останавливаем его
        if (service_status.dwCurrentState != SERVICE_STOPPED) {
            fprintf(stdout, "Service is working. It will be stoped\n");
            if (!ControlService(hService, SERVICE_CONTROL_STOP, &service_status)) {
                fprintf(stdout,
                    "Control service failed.\n"
                    "The last error code: %lu\n",
                    GetLastError());

                CloseServiceHandle(hServiceControlManager);
                CloseServiceHandle(hService);

                return 0;
            }
            // ждем, пока сервис остановится
            Sleep(500);
        }

        CloseServiceHandle(hServiceControlManager);
        CloseServiceHandle(hService);

        fprintf(stdout, "Service is stopped.\n");
        return 0;
    }
    else if (strcmp(operation, "pause") == 0) {
        SC_HANDLE hServiceControlManager, hService;
        SERVICE_STATUS service_status;

        // связываемся с менеджером сервисов
        hServiceControlManager =
            OpenSCManager(NULL, // локальная машина
                NULL, // активная база данных сервисов
                SC_MANAGER_CONNECT // соединение с менеджером сервисов
            );
        if (hServiceControlManager == NULL) {
            fprintf(stdout,
                "Open service control manager failed.\n"
                "The last error code: %lu\n",
                GetLastError());

            return 0;
        }

        fprintf(stdout, "Service control manager is opened.\n");

        // открываем сервис
        hService =
            OpenServiceA(hServiceControlManager, // дескриптор менеджера сервисов
                argv[1],      // имя сервиса
                SERVICE_ALL_ACCESS // полный доступ к сервису
            );
        if (hService == NULL) {
            fprintf(stdout,
                "Open service failed.\n"
                "The last error code: %lu\n",
                GetLastError());

            CloseServiceHandle(hServiceControlManager);

            return 0;
        }

        fprintf(stdout, "Service is opened.\n");

        // приостанавливаем сервис
        if (!ControlService(hService, // дескриптор сервиса
            SERVICE_CONTROL_PAUSE, // управл¤юща¤ команда
            &service_status // адрес структуры состо¤ни¤ сервиса
        )) {
            fprintf(stdout,
                "Control service failed.\n"
                "The last error code: %lu\n",
                GetLastError());
            CloseServiceHandle(hServiceControlManager);
            CloseServiceHandle(hService);

            return 0;
        }
        fprintf(stdout, "The service is paused.\n");

        CloseServiceHandle(hServiceControlManager);
        CloseServiceHandle(hService);
        return 0;
    }
    else if (strcmp(operation, "continue") == 0) {
        SC_HANDLE hServiceControlManager, hService;
        SERVICE_STATUS service_status;

        // связываемся с менеджером сервисов
        hServiceControlManager =
            OpenSCManager(NULL, // локальная машина
                NULL, // активная база данных сервисов
                SC_MANAGER_CONNECT // соединение с менеджером сервисов
            );
        if (hServiceControlManager == NULL) {
            fprintf(stdout,
                "Open service control manager failed.\n"
                "The last error code: %lu\n",
                GetLastError());

            return 0;
        }

        fprintf(stdout, "Service control manager is opened.\n");

        // открываем сервис
        hService =
            OpenServiceA(hServiceControlManager, // дескриптор менеджера сервисов
                argv[1],      // имя сервиса
                SERVICE_ALL_ACCESS // полный доступ к сервису
            );
        if (hService == NULL) {
            fprintf(stdout,
                "Open service failed.\n"
                "The last error code: %lu\n",
                GetLastError());

            CloseServiceHandle(hServiceControlManager);

            return 0;
        }

        fprintf(stdout, "Service is opened.\n");

        if (!ControlService(hService, // дескриптор сервиса
            SERVICE_CONTROL_CONTINUE, // управл¤юща¤ команда
            &service_status // адрес структуры состо¤ни¤ сервиса
        )) {
            fprintf(stdout,
                "Control service failed.\n"
                "The last error code: %lu\n",
                GetLastError());
            CloseServiceHandle(hServiceControlManager);
            CloseServiceHandle(hService);

            return 0;
        }
        fprintf(stdout, "The service is running.\n");

        CloseServiceHandle(hServiceControlManager);
        CloseServiceHandle(hService);
        return 0;
    }
    else if (strcmp(operation, "delete") == 0) {
        SC_HANDLE hServiceControlManager, hService;
        SERVICE_STATUS service_status;

        // связываемся с менеджером сервисов
        hServiceControlManager =
            OpenSCManager(NULL, // локальная машина
                NULL, // активная база данных сервисов
                SC_MANAGER_CONNECT // соединение с менеджером сервисов
            );
        if (hServiceControlManager == NULL) {
            fprintf(stdout,
                "Open service control manager failed.\n"
                "The last error code: %lu\n",
                GetLastError());

            return 0;
        }

        fprintf(stdout, "Service control manager is opened.\n");

        // открываем сервис
        hService =
            OpenServiceA(hServiceControlManager, // дескриптор менеджера сервисов
                argv[1],      // имя сервиса
                SERVICE_ALL_ACCESS // полный доступ к сервису
            );
        if (hService == NULL) {
            fprintf(stdout,
                "Open service failed.\n"
                "The last error code: %lu\n",
                GetLastError());

            CloseServiceHandle(hServiceControlManager);

            return 0;
        }

        fprintf(stdout, "Service is opened.\n");

        // получаем состояние сервиса
        if (!QueryServiceStatus(hService, &service_status)) {
            fprintf(stdout,
                "Open service status failed.\n"
                "The last error code: %lu\n",
                GetLastError());
            CloseServiceHandle(hServiceControlManager);
            CloseServiceHandle(hService);

            return 0;
        }

        // если сервис работает, то останавливаем его
        if (service_status.dwCurrentState != SERVICE_STOPPED) {
            fprintf(stdout, "Service is working. It will be stoped\n");
            if (!ControlService(hService, SERVICE_CONTROL_STOP, &service_status)) {
                fprintf(stdout,
                    "Control service failed.\n"
                    "The last error code: %lu\n",
                    GetLastError());

                CloseServiceHandle(hServiceControlManager);
                CloseServiceHandle(hService);

                return 0;
            }

            // ждем, пока сервис остановится
            Sleep(500);
        }

        // удаляем сервис
        if (!DeleteService(hService)) {
            fprintf(stdout,
                "Delete service failed.\n"
                "The last error code: %lu\n",
                GetLastError());

            CloseServiceHandle(hServiceControlManager);
            CloseServiceHandle(hService);

            return 0;
        }
        fprintf(stdout, "Service is deleted.\n");
        return 0;
    }
    else if (strcmp(operation, "get_info") == 0) {
        SC_HANDLE hServiceControlManager, hService;
        SERVICE_STATUS service_status;

        // связываемся с менеджером сервисов
        hServiceControlManager =
            OpenSCManager(NULL, // локальная машина
                NULL, // активная база данных сервисов
                SC_MANAGER_CONNECT // соединение с менеджером сервисов
            );
        if (hServiceControlManager == NULL) {
            fprintf(stdout,
                "Open service control manager failed.\n"
                "The last error code: %lu\n",
                GetLastError());

            return 0;
        }

        // открываем сервис
        hService =
            OpenServiceA(hServiceControlManager, // дескриптор менеджера сервисов
                argv[1],      // имя сервиса
                SERVICE_ALL_ACCESS // полный доступ к сервису
            );
        if (hService == NULL) {
            fprintf(stdout,
                "Open service failed.\n"
                "The last error code: %lu\n",
                GetLastError());

            CloseServiceHandle(hServiceControlManager);

            return 0;
        }

        // получаем состояние сервиса
        if (!QueryServiceStatus(hService, &service_status)) {
            fprintf(stdout,
                "Open service status failed.\n"
                "The last error code: %lu\n",
                GetLastError());
            CloseServiceHandle(hServiceControlManager);
            CloseServiceHandle(hService);
            return 0;
        }


        fprintf(stdout, "\n");
        switch (service_status.dwServiceType){
            case SERVICE_KERNEL_DRIVER:
                fprintf(stdout, "\tTYPE               : 1 SERVICE_KERNEL_DRIVER\n");
                break;
            case SERVICE_FILE_SYSTEM_DRIVER:
                fprintf(stdout, "\tTYPE               : 2 SERVICE_FILE_SYSTEM_DRIVER\n");
                break;
            case SERVICE_ADAPTER:
                fprintf(stdout, "\tTYPE               : 4 SERVICE_ADAPTER\n");
                break;
            case SERVICE_RECOGNIZER_DRIVER:
                fprintf(stdout, "\tTYPE               : 8 SERVICE_RECOGNIZER_DRIVER\n");
                break;
            case SERVICE_WIN32_OWN_PROCESS:
                fprintf(stdout, "\tTYPE               : 10 SERVICE_WIN32_OWN_PROCESS\n");
                break;
            case SERVICE_WIN32_SHARE_PROCESS:
                fprintf(stdout, "\tTYPE               : 20 SERVICE_WIN32_SHARE_PROCESS\n");
                break;
            case SERVICE_USER_SERVICE:
                fprintf(stdout, "\tTYPE               : 40 SERVICE_USER_SERVICE\n");
                break;
            case SERVICE_USERSERVICE_INSTANCE:
                fprintf(stdout, "\tTYPE               : 80 SERVICE_USERSERVICE_INSTANCE\n");
                break;
            case SERVICE_INTERACTIVE_PROCESS:
                fprintf(stdout, "\tTYPE               : 100 SERVICE_INTERACTIVE_PROCESS\n");
                break;
            case SERVICE_PKG_SERVICE:
                fprintf(stdout, "\tTYPE               : 200 SERVICE_PKG_SERVICE\n");
                break;
        default:
            break;
        }

        switch (service_status.dwCurrentState) {
        case SERVICE_STOPPED:
            fprintf(stdout, "\tSTATE              : 1 STOPPED\n");
            break;
        case SERVICE_START_PENDING:
            fprintf(stdout, "\tSTATE              : 2 START_PENDING\n");
            break;
        case SERVICE_STOP_PENDING:
            fprintf(stdout, "\tSTATE              : 3 STOP_PENDING\n");
            break;
        case SERVICE_RUNNING:
            fprintf(stdout, "\tSTATE              : 4 RUNNING\n");
            break;
        case SERVICE_CONTINUE_PENDING:
            fprintf(stdout, "\tSTATE              : 5 CONTINUE_PENDING\n");
            break;
        case SERVICE_PAUSE_PENDING:
            fprintf(stdout, "\tSTATE              : 6 PAUSE_PENDING\n");
            break;
        case SERVICE_PAUSED:
            fprintf(stdout, "\tSTATE              : 7 PAUSED\n");
            break;
        }

        fprintf(stdout, "\tWIN32_EXIT_CODE    : %ld\n", service_status.dwWin32ExitCode);
        fprintf(stdout, "\tSERVICE_EXIT_CODE  : %ld\n", service_status.dwServiceSpecificExitCode);
        fprintf(stdout, "\tCHECKPOINT          : %ld\n", service_status.dwCheckPoint);
        fprintf(stdout, "\tWAIT_HINT           : %d\n", service_status.dwWaitHint);

        return 0;
        }
    else {
        fprintf(stdout, "Wrong command. Supported commands: create, start, pause, continue, stop, delete, get_info\n"
            "Press any key to continue.\n");
        getchar();
        return 0;
    }

    return 0;
    
}