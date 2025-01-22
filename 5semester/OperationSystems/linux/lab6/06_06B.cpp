#include <iostream>
#include <pthread.h>
#include <chrono>
#include <thread>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define SHARED_MUTEX_NAME "/my_shared_mutex"

using namespace std;

int main() {
    // Открываем разделяемый мьютекс
    int shm_fd = shm_open(SHARED_MUTEX_NAME, O_RDWR, 0666);
    if (shm_fd == -1) {
        cerr << "Failed to open shared memory." << endl;
        return 1;
    }

    pthread_mutex_t* mutex = (pthread_mutex_t*)mmap(NULL, sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    close(shm_fd);

    if (mutex == MAP_FAILED) {
        cerr << "Failed to map shared memory." << endl;
        return 1;
    }

    // Выполняем цикл с использованием мьютекса
    for (int i = 1; i <= 90; ++i) {
        if (i >= 30 && i <= 60) {
            pthread_mutex_lock(mutex);
            cout << "Process B - iteration: " << i << endl;
            pthread_mutex_unlock(mutex);
        } else {
            cout << "Process B - iteration: " << i << endl;
        }
        this_thread::sleep_for(chrono::milliseconds(100));
    }

    return 0;
}

