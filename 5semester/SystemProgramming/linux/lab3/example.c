#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;



// Функция, выполняемая процессами
void use_MyMutex(pthread_mutex_t *mutex, int process_num) {
    for (int i = 0; i < 5; i++) {
        pthread_mutex_lock(mutex);
        printf("Process %d is in the mutex.\n", process_num);
        sleep(5); 
        printf("Process %d is leaving the mutex\n", process_num);
        pthread_mutex_unlock(mutex);
        sleep(1);

    }
}

int main() {
    
    for(int i = 0; i < 3; i++){

        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(1);
        } else if (pid == 0) {
            use_MyMutex(&mutex, i);
            exit(0);
        } 
    }
    for(int i = 0; i < 3; i++){
        wait(NULL);
    }

    pthread_mutex_destroy(&mutex);

    return 0;
}