#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>
#include <chrono>
#include <thread>

#define SHARED_MUTEX_NAME "/my_shared_mutex"
using namespace std;

void executeLoop(const string& name, pthread_mutex_t* mutex) {
    for (int i = 1; i <= 90; ++i) {
        if (i >= 30 && i <= 60) {
            pthread_mutex_lock(mutex);
            cout << "Process " << name << " - iteration: " << i << endl;
            pthread_mutex_unlock(mutex);
        } else {
            cout << "Process " << name << " - iteration: " << i << endl;
        }
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

int main() {
    // Создаем разделяемую память для мьютекса
    int shm_fd = shm_open(SHARED_MUTEX_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        cerr << "Failed to create shared memory." << endl;
        return 1;
    }
    if (ftruncate(shm_fd, sizeof(pthread_mutex_t)) == -1) {
        cerr << "Failed to set size of shared memory." << endl;
        close(shm_fd);
        shm_unlink(SHARED_MUTEX_NAME);
        return 1;
    }

    pthread_mutex_t* mutex = (pthread_mutex_t*)mmap(NULL, sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    close(shm_fd);

    if (mutex == MAP_FAILED) {
        cerr << "Failed to map shared memory." << endl;
        shm_unlink(SHARED_MUTEX_NAME);
        return 1;
    }

    // Инициализация мьютекса
    pthread_mutexattr_t mutexAttr;
    pthread_mutexattr_init(&mutexAttr);
    pthread_mutexattr_setpshared(&mutexAttr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(mutex, &mutexAttr);
    pthread_mutexattr_destroy(&mutexAttr);

    // Запуск процессов A и B
    pid_t pidA = fork();
    if (pidA == 0) {
        execl("./OS06_06A", "OS06_06A", NULL);
        cerr << "Failed to start process A" << endl;
        return 1;
    }

    pid_t pidB = fork();
    if (pidB == 0) {
        execl("./OS06_06B", "OS06_06B", NULL);
        cerr << "Failed to start process B" << endl;
        return 1;
    }

    // Выполнение цикла в основном процессе
    executeLoop("Main", mutex);

    // Ожидание завершения дочерних процессов
    waitpid(pidA, NULL, 0);
    waitpid(pidB, NULL, 0);

    // Удаление мьютекса и освобождение ресурсов
    pthread_mutex_destroy(mutex);
    munmap(mutex, sizeof(pthread_mutex_t));
    shm_unlink(SHARED_MUTEX_NAME);

    return 0;
}

