#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>


static void lookup()
{
    DIR *dirp;
    struct dirent *dp;


    if ((dirp = opendir(".")) == NULL) {
        perror("couldn't open '.'");
        return;
    }


    do {
        errno = 0;
        if ((dp = readdir(dirp)) != NULL) {
			(void) printf("found %s\n", dp->d_name);
        }
    } while (dp != NULL);


    if (errno != 0)
        perror("error reading directory");
    (void) closedir(dirp);
    return;
}


int main(int argc, char *argv[])
{
    lookup();
    return (0);
}