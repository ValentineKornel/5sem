#include <iostream>
#include <unistd.h>       // для getpid() и sysconf()
#include <pthread.h>      // для pthread_self()
#include <sched.h>        // для sched_getparam() и sched_get_priority_max()
#include <sys/syscall.h>  // для получения идентификатора потока (gettid)
#include <sys/resource.h> // для nice()

int main() {
    std::cout << "Korneliuk Valentine\n";
    // Идентификатор текущего процесса
    pid_t pid = getpid();
    std::cout << "Process ID: " << pid << std::endl;

    // Идентификатор текущего (main) потока
    pid_t tid = syscall(SYS_gettid);
    std::cout << "Thread ID: " << tid << std::endl;

    // Приоритет (nice) текущего потока
    int priority = getpriority(PRIO_PROCESS, 0);
    std::cout << "Thread nice value (priority): " << priority << std::endl;

    // Номера доступных процессоров
    cpu_set_t set;
    CPU_ZERO(&set);
    if (sched_getaffinity(0, sizeof(set), &set) == 0) {
        std::cout << "Available processors: ";
        for (int i = 0; i < CPU_SETSIZE; i++) {
            if (CPU_ISSET(i, &set)) {
                std::cout << i << " ";
            }
        }
        std::cout << std::endl;
    } else {
        std::cerr << "Error getting available processors." << std::endl;
    }

    return 0;
}
