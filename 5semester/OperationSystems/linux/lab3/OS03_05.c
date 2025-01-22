#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/prctl.h>

int main() {
    pid_t processId = getpid();

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        // Это выполняется в дочернем процессе
        printf("Child process started. PID: %d, Parent PID: %d\n", getpid(), getppid());
        
        for (int i = 1; i <= 50; i++) {
            printf("Child process (PID: %d), Iteration: %d\n", getpid(), i);
            sleep(1);
        }
        
        printf("Child process (PID: %d) finished.\n", getpid());
    } else {
        // Это выполняется в родительском процессе
        printf("Parent process (PID: %d) created child process (PID: %d)\n", processId, pid);
        
        for (int i = 1; i <= 100; i++) {
            printf("Parent process (PID: %d), Iteration: %d\n", processId, i);
            sleep(1);
        }
        
        wait(NULL);
        printf("Child process (PID: %d) has finished. Parent process (PID: %d) is exiting.\n", pid, processId);
    }

    return 0;
}