#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>


int is_prime(int num) {
    if (num < 2) return 0;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char *argv[]) {


if (argc < 4) {
        fprintf(stderr, "Usage: %s <number of processes> <start> <end> [mutex name]\n", argv[0]);
        return 1;
    }

    pid_t processId = getpid();
    printf("pid: %d\n", processId);

    int numberOfProcess = atoi(argv[1]);
    int start = atoi(argv[2]);
    int end = atoi(argv[3]);

    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
        perror("Failed to create pipe");
        return 1;
    }

    int range = end - start + 1;
    int step = range / numberOfProcess;

    pid_t pids[numberOfProcess];

    // Создание общей памяти для мьютекса
    pthread_mutex_t *mutex;
    pthread_mutexattr_t attr;

    // Используем mmap для создания разделяемого сегмента памяти для мьютекса
    mutex = mmap(NULL, sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (mutex == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    // Инициализируем атрибуты мьютекса
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED); // Позволяет мьютексу работать между процессами

    // Инициализация мьютекса
    pthread_mutex_init(mutex, &attr);


    for(int i = 0; i < numberOfProcess; i++){

        int local_start = start + i * step;
        int local_end = local_start + step - 1;
        if (i == numberOfProcess - 1) {
            local_end = end;
        }

        printf("Process %d: range %d - %d\n", i + 1, local_start, local_end);

        pid_t pid = fork();
        if (pid == -1) {
            perror("Failed to fork");
            return 1;
        } else if (pid == 0) {
            // Закрываем дескриптор чтения канала в дочернем процессе
            close(pipe_fd[0]);

            char buffer[250];
            int buffer_size = 0;

            for (int i = local_start; i <= local_end; i++) {
                if (is_prime(i)) {
                    buffer_size += snprintf(buffer + buffer_size, sizeof(buffer) - buffer_size, "%d ", i);
                }
            }     

            printf("Process %d found primes in range %d-%d\n", i + 1, local_start, local_end);

            // Блокируем мьютекс
            pthread_mutex_lock(mutex);
            write(pipe_fd[1], buffer, strlen(buffer) + 1);
            printf("Process %d is in the mutex.\n", i);
            sleep(3); 
            printf("Process %d is leaving the mutex\n", i);
            // Освобождаем мьютекс
            pthread_mutex_unlock(mutex);

            // Закрываем дескриптор записи канала
            close(pipe_fd[1]);

            exit(0);
        } 
    }

    //Закрываем дескриптор записи канала в родительском процессе
    close(pipe_fd[1]);

    // Читаем сообщения от дочерних процессов
    for (int i = 0; i < numberOfProcess; i++) {
        char buffer[250];
        read(pipe_fd[0], buffer, sizeof(buffer));
        printf("Received from child process: %s\n", buffer);
    }

    close(pipe_fd[0]);

    for (int i = 0; i < numberOfProcess; i++) {
        waitpid(pids[i], NULL, 0);
    }
    printf("Press any key to continue.\n");
    getchar();

    // Уничтожаем мьютекс и атрибуты
    pthread_mutex_destroy(mutex);
    pthread_mutexattr_destroy(&attr);

    // Освобождаем общую память
    munmap(mutex, sizeof(pthread_mutex_t));

    return 0;
}