#include <stdio.h>
#include <windows.h>

int main() {
  // создаем каталог
  if (!CreateDirectory("C:\\demo_dir", NULL)) {
    fprintf(stderr, "Create directory failed.\nThe last error code: %ld\n",
            GetLastError());
    fputs("Press any key to finish.\n", stdout);
    getchar();

    return 0;
  }

  fputs("The directory is created.\n", stdout);

  return 0;
}