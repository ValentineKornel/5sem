#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char* argv[]) {
    pid_t pid1, pid2;

    pid_t processId = getpid();

    pid1 = fork();

    if (pid1== -1) {
        perror("fork failed");
        return 1;
    } else if (pid1 == 0) {
        execl("./Lab-02x", "Lab-02h", "1000", NULL);
    
    } else {
        setenv("ITER_NUM", "1500", 1);
        pid2 = fork();

        if (pid2== -1) {
            perror("fork failed");
            return 1;
        } else if (pid2 == 0) {
          execl("./Lab-02x", "", NULL);
    
        } else {
            printf("main\n"); 
        }
    }

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    printf("Оба дочерних процесса завершены.\n");

    return 0;
}