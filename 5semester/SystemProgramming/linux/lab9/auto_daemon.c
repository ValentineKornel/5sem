/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2024.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 37-3 */

/* daemon_SIGHUP.c

   Demonstrate the use of SIGHUP as a mechanism to tell a daemon to
   reread its configuration file and reopen its log file.

   In the version of this code printed in the book, logOpen(), logClose(),
   logMessage(), and readConfigFile() were omitted for brevity. The version
   of the code in this file is complete, and can be compiled and run.
*/
#include <sys/stat.h>
#include <signal.h>
#include <sys/inotify.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include "become_daemon.h"
#include "tlpi_hdr.h"

const char *daemon_name = "Service09d";

#define SBUF_SIZE 128
static char log_dir[SBUF_SIZE];
static char watch_dir[SBUF_SIZE];

static char DIR_LOG_FILE[SBUF_SIZE+50];
static char SRV_LOG_FILE[SBUF_SIZE+50];
static const char *CONFIG_FILE = "/etc/Serivce09d/service09d.conf";

#include <time.h>
#include <stdarg.h>

static FILE *logfp;                 /* Log file stream */
static FILE *dirlogfp;
static FILE *srvlogfp;

void createLogFiles(void)
{
    time_t t;
    struct tm *tm_info;
    char datetime_str[20];  // Строка для хранения даты и времени в формате YYYY-MM-DD_HH-MM-SS

    // Получаем текущее время
    t = time(NULL);
    tm_info = localtime(&t);

    if (tm_info == NULL) {
        perror("localtime failed");
        return;
    }

    // Форматируем дату и время в строку
    strftime(datetime_str, sizeof(datetime_str), "%F_%H-%M-%S", tm_info);

    // Формируем полный путь к лог-файлу
    snprintf(DIR_LOG_FILE, sizeof(DIR_LOG_FILE), "%s/%s-dir.log", log_dir, datetime_str);

    // Открываем файл для записи
    FILE *logfp = fopen(DIR_LOG_FILE, "w");
    if (logfp == NULL) {
        perror("Unable to open log file");
        return;
    }

    // Закрываем файл
    fclose(logfp);
    

    printf("dir-log file created: %s\n", DIR_LOG_FILE);


    // Форматируем дату и время в строку
    strftime(datetime_str, sizeof(datetime_str), "%F_%H-%M-%S", tm_info);

    // Формируем полный путь к лог-файлу
    snprintf(SRV_LOG_FILE, sizeof(SRV_LOG_FILE), "%s/%s-srv.log", log_dir, datetime_str);

    // Открываем файл для записи
    logfp = fopen(SRV_LOG_FILE, "w");
    if (logfp == NULL) {
        perror("Unable to open log file");
        return;
    }

    // Закрываем файл
    fclose(logfp);

    printf("srv-log file created: %s\n", SRV_LOG_FILE);
}


static void
logMessage(const char *format, ...)
{
    va_list argList;
    const char *TIMESTAMP_FMT = "%X";        /* = YYYY-MM-DD HH:MM:SS */
#define TS_BUF_SIZE sizeof("HH:MM:SS")       /* Includes '\0' */
    char timestamp[TS_BUF_SIZE];
    time_t t;
    struct tm *loc;

    t = time(NULL);
    loc = localtime(&t);
    if (loc == NULL ||
           strftime(timestamp, TS_BUF_SIZE, TIMESTAMP_FMT, loc) == 0)
        fprintf(dirlogfp, "???Unknown time????: ");
    else
        fprintf(dirlogfp, "%s: ", timestamp);

    va_start(argList, format);
    vfprintf(dirlogfp, format, argList);
    fprintf(dirlogfp, "\n");
    va_end(argList);
}

static void
srvLogMessage(const char *format, ...)
{
    va_list argList;
    const char *TIMESTAMP_FMT = "%X";        /* = YYYY-MM-DD HH:MM:SS */
#define TS_BUF_SIZE sizeof("HH:MM:SS")       /* Includes '\0' */
    char timestamp[TS_BUF_SIZE];
    time_t t;
    struct tm *loc;

    t = time(NULL);
    loc = localtime(&t);
    if (loc == NULL ||
           strftime(timestamp, TS_BUF_SIZE, TIMESTAMP_FMT, loc) == 0)
        fprintf(srvlogfp, "???Unknown time????: ");
    else
        fprintf(srvlogfp, "%s: ", timestamp);

    va_start(argList, format);
    vfprintf(srvlogfp, format, argList);
    fprintf(srvlogfp, "\n");
    va_end(argList);
}

static void
logOpen(const char *logFilename)
{
    mode_t m;

    m = umask(077);
    dirlogfp = fopen(logFilename, "a");
    umask(m);

    /* If opening the log fails we can't display a message... */

    if (dirlogfp == NULL)
        exit(EXIT_FAILURE);

    setbuf(dirlogfp, NULL);                    /* Disable stdio buffering */

    logMessage("Opened log file");
}

static void
srvLogOpen(const char *logFilename)
{
    mode_t m;

    m = umask(077);
    srvlogfp = fopen(logFilename, "a");
    umask(m);

    /* If opening the log fails we can't display a message... */

    if (srvlogfp == NULL)
        exit(EXIT_FAILURE);

    setbuf(srvlogfp, NULL);                    /* Disable stdio buffering */

    srvLogMessage("Opened log file");
}

static void
logClose(void)
{
    logMessage("Closing log file");
    fclose(dirlogfp);
}

static void
srvLogClose(void)
{
    srvLogMessage("Closing log file");
    fclose(srvlogfp);
}

static void readConfigFile(const char *configFilename)
{
    FILE *configfp;
    char str[SBUF_SIZE];

    configfp = fopen(configFilename, "r");
    if (configfp == NULL) {
        perror("Unable to open config file");
        return;
    }
    int log_dir_set = 0;

    while (fgets(str, SBUF_SIZE, configfp) != NULL) {
        // Убираем символ новой строки, если он есть
        str[strcspn(str, "\n")] = '\0';

        // Проверяем, не пустая ли строка или комментарий
        if (str[0] == '\0' || str[0] == '#') {
            continue; // Пропускаем пустые строки и комментарии
        }

        // Ищем разделитель '='
        char *equals_pos = strchr(str, '=');
        if (equals_pos != NULL) {
            *equals_pos = '\0'; // Разделяем строку на ключ и значение
            char *key = str;
            char *value = equals_pos + 1;

            // Убираем возможные пробелы в конце значения
            value[strcspn(value, "\r\n")] = '\0';

            // Проверяем, что за ключ и записываем в соответствующие переменные
            if (strcmp(key, "log_dir") == 0) {
                if (value[0] != '\0') { // Убедимся, что значение не пустое
                strncpy(log_dir, value, SBUF_SIZE - 1);
                log_dir[SBUF_SIZE - 1] = '\0'; // Убедимся, что строка не выйдет за пределы
                log_dir_set = 1; // Указываем, что значение log_dir было задано
            }
            } else if (strcmp(key, "watch_dir") == 0) {
                strncpy(watch_dir, value, SBUF_SIZE - 1);
                watch_dir[SBUF_SIZE - 1] = '\0';
            }
        }
        if (!log_dir_set) {
            strncpy(log_dir, "/var/log/Service09d", SBUF_SIZE - 1);
            log_dir[SBUF_SIZE - 1] = '\0';
        }
    }

    fclose(configfp);

    // Печать значений для проверки
    printf("Log directory: %s\n", log_dir);
    printf("Watch directory: %s\n", watch_dir);
}

static int checkAndCreateLogDir(const char *log_dir) {
    struct stat st;

    // Проверяем, существует ли директория
    if (stat(log_dir, &st) == 0) {
        // Проверяем, что это директория
        if (S_ISDIR(st.st_mode)) {
            return 0;
        }
    } else {
        // Если директория не существует, создаем её
        if (errno == ENOENT) {
            if (mkdir(log_dir, 0700) == 0) {
                return 1;
            } else {
                printf("Error creating directory");
                return -1;
            }
        } else {
            return -1;
        }
    }
}


static volatile sig_atomic_t hupReceived = 0;
                        /* Set nonzero on receipt of SIGHUP */
static volatile sig_atomic_t termReceived = 0;

static void
sighupHandler(int sig)
{
    hupReceived = 1;
}

static void
sigtermHandler(int sig)
{
    termReceived = 1;
}

int
main(int argc, char *argv[])
{
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = sighupHandler;
    if (sigaction(SIGHUP, &sa, NULL) == -1)
        exit(EXIT_FAILURE);

    sa.sa_handler = sigtermHandler;
    if (sigaction(SIGTERM, &sa, NULL) == -1)
        exit(EXIT_FAILURE);

    // if (becomeDaemon(0) == -1)
    //     exit(1);


    readConfigFile(CONFIG_FILE);
    int created = checkAndCreateLogDir(log_dir);
    createLogFiles();
    srvLogOpen(SRV_LOG_FILE);
    if(created == 1){
        srvLogMessage("Успех! Демон %s создал каталог %s", daemon_name, log_dir);
    }else if(created == 0){
        srvLogMessage("Успех! Демон %s обнаружил каталог %s", daemon_name, log_dir);
    }else{
        srvLogMessage("Попытка запуска провалена. Ошибка создания директории для логирования");
    }

    int fd = inotify_init();
    if (fd == -1) {
        srvLogMessage("Error: Could not initialize inotify");
        exit(1);
    }

    // Установка неблокирующего режима
    int flags = fcntl(fd, F_GETFL, 0);  // Получаем текущие флаги
    if (flags == -1) {
        srvLogMessage("Err1");
        exit(1);
    }

    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {  // Устанавливаем флаг O_NONBLOCK
        srvLogMessage("Err2");
        perror("fcntl");
        exit(1);
    }

    int wd = inotify_add_watch(fd, watch_dir, IN_ALL_EVENTS);
    if (wd == -1) {
        srvLogMessage("Попытка запуска провалена! Демон %s не смог обнаружить каталог %s", daemon_name, watch_dir);
        close(fd);
        exit(1);
    }

    srvLogMessage("Успех! Демон %s запущен с параметрами %s %s", daemon_name, watch_dir, log_dir );


    logOpen(DIR_LOG_FILE);
    char buffer[1024];
    while (!termReceived) {

        if (hupReceived){
            hupReceived = 0;
            inotify_rm_watch(fd, wd);
            close(fd);
            logClose();
            srvLogClose();

            readConfigFile(CONFIG_FILE);
            created = checkAndCreateLogDir(log_dir);
            createLogFiles();
            srvLogOpen(SRV_LOG_FILE);
            if(created == 1){
                srvLogMessage("Успех! Демон %s создал каталог %s", daemon_name, log_dir);
            }else if(created == 0){
                srvLogMessage("Успех! Демон %s обнаружил каталог %s", daemon_name, log_dir);
            }else{
                srvLogMessage("Попытка запуска провалена. Ошибка создания директории для логирования");
            }

            fd = inotify_init();
            if (fd == -1) {
                srvLogMessage("Error: Could not initialize inotify");
                exit(1);
            }

            wd = inotify_add_watch(fd, watch_dir, IN_ALL_EVENTS);
            if (wd == -1) {
                srvLogMessage("Попытка запуска провалена! Демон %s не смог обнаружить каталог %s", daemon_name, watch_dir);
                close(fd);
                exit(1);
            }

            srvLogMessage("Успех! Демон %s теперь работает с параметрами %s %s", daemon_name, watch_dir, log_dir );
            logOpen(DIR_LOG_FILE);

        }

        ssize_t length = read(fd, buffer, sizeof(buffer));
        if (length == -1) {
            if(errno != EAGAIN && errno != EWOULDBLOCK){
                logMessage("Error: Could not read directory changes");
                break;
            }
            usleep(100000); //0.1s
            continue;
        }

        for (char* ptr = buffer; ptr < buffer + length; ) {
            struct inotify_event* event = (struct inotify_event*) ptr;

            if (event->mask & IN_ACCESS) {
                logMessage("File was accessed: %s\n", event->name);
            }
            if (event->mask & IN_MODIFY) {
                logMessage("File was modified: %s\n", event->name);

            }
            if (event->mask & IN_ATTRIB) {
                logMessage("File was changed: %s\n", event->name);
            }
            if (event->mask & IN_CLOSE_WRITE) {
                logMessage("File was closed: %s\n", event->name);
            }
            if (event->mask & IN_OPEN) {
                logMessage("File was opened: %s\n", event->name);
            }
            if (event->mask & IN_MOVED_FROM) {
                logMessage("File was moved from: %s\n", event->name);
            }
            if (event->mask & IN_MOVED_TO) {
                logMessage("File was moved to: %s\n", event->name);
            }
            if (event->mask & IN_CREATE) {
                logMessage("File was created: %s\n", event->name);
            }
            if (event->mask & IN_DELETE) {
                logMessage("File was deleted: %s\n", event->name);
            }
            if (event->mask & IN_DELETE_SELF) {
                logMessage("File was deleted: %s\n", event->name);
            }
            if (event->mask & IN_MOVE_SELF) {
                logMessage("File was moved: %s\n", event->name);
            }

            ptr += sizeof(struct inotify_event) + event->len;
        }
    }

    inotify_rm_watch(fd, wd);
    close(fd);
    srvLogMessage("Демон %s завершил свою работу", daemon_name);
    logClose();
    srvLogClose();
    return 0;
}
