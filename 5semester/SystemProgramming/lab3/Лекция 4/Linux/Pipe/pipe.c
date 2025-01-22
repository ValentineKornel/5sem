#include <unistd.h>
#include <stdio.h>

int main() {
    int fd[2];
    char buffer[20];

    pipe(fd);

    if (fork() == 0) {
        close(fd[0]);
        write(fd[1], "Hello World!", 13);
        close(fd[1]);
    } else {
        close(fd[1]);
        read(fd[0], buffer, 20);
        printf("%s\n", buffer);
        close(fd[0]);
    }

    return 0;
}
