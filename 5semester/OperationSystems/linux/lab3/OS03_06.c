#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int main() {
    pid_t pid = getpid();

    int status = system("./OS03_06_1");

    if (status == -1) {
        printf("Error: failed to create child process\n");
        return 1;
    }

    for (int i = 1; i <= 100; i++) {
        printf("OS03_06: Process ID: %d, Iteration: %d\n", pid, i);
        sleep(1);
    }

    printf("OS03_06 completed.\n");
    return 0;
}