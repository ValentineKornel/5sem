#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>

int main() {
    // Получаем идентификатор процесса
    pid_t pid = getpid();
    printf("Process ID: %d\n", pid);

    // Получаем текущее значение nice
    int priority = getpriority(PRIO_PROCESS, 0);
    printf("Initial nice value (priority): %d\n", priority);

    // Долгий цикл
    long long counter = 0;
    while (1) {
        counter++;
        if (counter % 1000000000 == 0) {
            printf("Still running... Counter: %lld\n", counter);
        }
    }

    return 0;
}
