#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    printf("OS03_07: Starting the process with PID: %d\n", getpid());

    for (int i = 1; i <= 100; i++) {
        printf("OS03_07: Iteration: %d, Process ID: %d\n", i, getpid());
        sleep(1); 

        if(i == 3){
            printf("OS03_07: Replacing current process with OS03_06_1\n");
            execl("./OS03_06_1", "./OS03_06_1", NULL);
        }
    }

    perror("exec failed");
    return 1;
}