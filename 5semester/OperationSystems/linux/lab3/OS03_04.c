#include <stdio.h>
#include <unistd.h>

int main(){
    int processId = getpid();

    for(int i = 1; i <= 100; i++){
        printf("Process ID: %d, Iteration: %d\n", processId, i);

        sleep(1);
    }

    printf("Process completed.\n");
    return 0;
}