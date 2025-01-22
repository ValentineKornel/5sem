#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    // Create a new directory named "testdir"
    if (mkdir("testdir", 0755) != 0) {
        perror("mkdir");
        exit(1);
    }
    printf("Directory 'testdir' created\n");

    // Change to the newly created directory
    if (chdir("testdir") != 0) {
        perror("chdir");
        exit(1);
    }
    printf("Changed to directory 'testdir'\n");

    // Create a file named "testfile" in the current directory
    FILE *file = fopen("testfile", "w");
    if (file == NULL) {
        perror("fopen");
        exit(1);
    }
    fclose(file);
    printf("File 'testfile' created\n");

    // Remove the file "testfile"
    if (remove("testfile") != 0) {
        perror("remove");
        exit(1);
    }
    printf("File 'testfile' removed\n");

    if (chdir("..")!= 0) {
        perror("chdir");
        exit(1);
    }
    printf("Changed to directory 'testdir'\n");

    // Remove the directory "testdir" and its contents
    if (rmdir("testdir") != 0) {
        perror("rmdir");
        exit(1);
    }
    printf("Directory 'testdir' removed\n");

    return 0;
}