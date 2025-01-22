#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

int main() {
    int fd;
    char buf[BUFFER_SIZE];
    ssize_t nread;

    // Open the file "input.txt" for reading
    fd = open("input.txt", O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    // Read data from the file and write it to the standard output
    while ((nread = read(fd, buf, BUFFER_SIZE)) > 0) {
        write(STDOUT_FILENO, buf, nread);
    }
    if (nread == -1) {
        perror("read");
        exit(1);
    }

    // Close the file descriptor
    close(fd);

    // Open the file "output.txt" for writing
    fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    // Write some data to the file
    const char *msg = "Hello, World!\n";
    write(fd, msg, strlen(msg));

    // Close the file descriptor
    close(fd);

    return 0;
}