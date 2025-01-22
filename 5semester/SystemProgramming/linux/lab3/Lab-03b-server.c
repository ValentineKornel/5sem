#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h> 
#include <errno.h>
#include <ctype.h>
#include <semaphore.h>
#include <stdlib.h>


#define SEMAPHORE_NAME "/my_semaphore"
#define FIFO_NAME "/tmp/my_fifo"
#define BUFFER_SIZE 128

void to_uppercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper(str[i]);
    }
}

int main(int argc, char* argv[]) {

    pid_t processId = getpid();
    printf("pid: %d\n", processId);

    char fifo_name[50];
    if (argc > 1) {
        strncpy(fifo_name, argv[1], sizeof(fifo_name) - 1);
        fifo_name[sizeof(fifo_name) - 1] = '\0';
    }
    else{
        const char* env = getenv("FIFO_NAME");
        if(env == NULL){
            printf("cant find semaphore name\n");
            return 1;
        }
        strncpy(fifo_name, env, sizeof(fifo_name) - 1);
        fifo_name[sizeof(fifo_name) - 1] = '\0';
    }


// Создаем именованный семафор
    sem_t *sem = sem_open(SEMAPHORE_NAME, O_CREAT, 0644, 1); // Инициализация с начальным значением 1
    if (sem == SEM_FAILED) {
        perror("sem_open");
        return 1;
    }

    // Создаем именованный канал (FIFO)
    if (mkfifo(fifo_name, 0666) == -1) {
        if (errno != EEXIST) { // Игнорируем ошибку, если FIFO уже существует
            perror("mkfifo");
            return 1;
        }
    }

    for(;;){
        printf("waiting for the message...\n");

        // Открываем FIFO для чтения
        int read_fd = open(fifo_name, O_RDONLY);
        if (read_fd == -1) {
            perror("open");
            return 1;
        }

        char buffer[BUFFER_SIZE];
        ssize_t num_bytes = read(read_fd, buffer, sizeof(buffer));
        if (num_bytes > 0) {
            printf("Received: %s\n", buffer);

            if (strcmp(buffer, "quit") == 0) {
                close(read_fd);
                printf("Exiting...\n");
                break;
            }

            to_uppercase(buffer);

            close(read_fd);

            // Открываем FIFO для записи
            int write_fd = open(fifo_name, O_WRONLY);
            if (write_fd == -1) {
                perror("open for writing");
                return 1;
            }
            
            write(write_fd, buffer, strlen(buffer) + 1);

            close(write_fd);
        } else {
            perror("read");
        }
    }

    unlink(fifo_name);
    sem_unlink(SEMAPHORE_NAME);

    return 0;
}
