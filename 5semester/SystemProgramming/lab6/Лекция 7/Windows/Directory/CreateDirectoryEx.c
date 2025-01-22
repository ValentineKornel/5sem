#include <stdio.h>
#include <windows.h>


int main() {
  // создаем подкаталог
  if (!CreateDirectoryEx("C:\\demo_dir", "C:\\demo_dir\\demo_subdir", NULL)) {
    fprintf(stderr, "Create directory failed.\nThe last error code: %ld\n",
            GetLastError());
    fputs("Press any key to finish.\n", stdout);
    getchar();

    return 0;
  }

  fputs("The directory is created.\n",stdout);

  return 0;
}