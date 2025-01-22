#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <semaphore.h>
#include <stdlib.h>

#define SEMAPHORE_NAME "/my_semaphore"
#define FIFO_NAME "/tmp/my_fifo"
#define BUFFER_SIZE 128

int main(int argc, char* argv[]) {

    char message[80];

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <string> [fifo name] [semaphore name]\n", argv[0]);
        return 1;
    }
    pid_t processId = getpid();
    printf("pid: %d\n", processId);

    strncpy(message, argv[1], sizeof(message) - 1);
    message[sizeof(message) - 1] = '\0';

    char fifo_name[50];

    if (argc > 2) {
        strncpy(fifo_name, argv[2], sizeof(fifo_name) - 1);
        fifo_name[sizeof(fifo_name) - 1] = '\0';
    }
    else{
        const char* env = getenv("FIFO_NAME");
        if(env == NULL){
            printf("cant find fifo name\n");
            return 1;
        }
        strncpy(fifo_name, env, sizeof(fifo_name) - 1);
    }


    char sem_name[50];
    if (argc > 3) {
        strncpy(sem_name, argv[3], sizeof(sem_name) - 1);
        sem_name[sizeof(sem_name) - 1] = '\0';
    }
    else{
        const char* env = getenv("SEMAPHORE_NAME");
        if(env == NULL){
            printf("cant find fifo name");
            return 1;
        }
        strncpy(sem_name, env, sizeof(sem_name) - 1);
    }

    sem_t *sem = sem_open(sem_name, 0); // Открытие существующего семафора
    if (sem == SEM_FAILED) {
        perror("sem_open");
        return 1;
    }

    // Открываем FIFO для записи
    int fd = open(fifo_name, O_WRONLY);
    if (fd == -1) {
        perror("open for writing");
        return 1;
    }

    printf("waiting for semaphore...\n");
    sem_wait(sem);
    // Отправляем сообщение
    write(fd, message, strlen(message) + 1); // +1 для завершающего нуля
    close(fd); // Закрываем FIFO для записи

    // Открываем FIFO для чтения
    fd = open(fifo_name, O_RDONLY);
    if (fd == -1) {
        perror("open for reading");
        return 1;
    }

    char buffer[BUFFER_SIZE];
    ssize_t num_bytes = read(fd, buffer, sizeof(buffer)); // Читаем ответ
    if (num_bytes > 0) {
        buffer[num_bytes] = '\0'; // Завершаем строку
        printf("Received from server: %s\n", buffer);
    } else {
        perror("read");
    }

    close(fd); // Закрываем FIFO для чтения

    sleep(5);
    sem_post(sem);
    sem_close(sem);

    return 0;
}
