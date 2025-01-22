#include <stdio.h>
#include <windows.h>

int main() {
  DWORD dwBinaryType;

  // определяем тип файла
  if (!GetBinaryType("C:\\temp.exe", &dwBinaryType)) {
    fprintf(stderr,
            "Get binary type failed.\nThe file may not be executable.\nThe "
            "last error code: %ld\n",
            GetLastError());
    fputs("Press any key to finish.\n", stdout);
    getchar();

    return 0;
  }

  // распечатываем тип файла
  switch (dwBinaryType) {
  case SCS_32BIT_BINARY:
    fputs("A 32-bit Windows-based application.\n", stdout);
    break;
  case SCS_64BIT_BINARY:
    fputs("A 64-bit Windows-based application.\n", stdout);
    break;
  case SCS_DOS_BINARY:
    fputs("An MS-DOS – based application .\n", stdout);
    break;
  case SCS_OS216_BINARY:
    fputs("A 16-bit OS/2-based application.\n", stdout);
    break;
  case SCS_PIF_BINARY:
    fputs("A PIF file that executes an MS-DOS – based application.\n", stdout);
    break;
  case SCS_POSIX_BINARY:
    fputs("A POSIX – based application.\n", stdout);
    break;
  case SCS_WOW_BINARY:
    fputs("A 16-bit Windows-based application.\n", stdout);
    break;
  default:
    fputs("Mistery application.\n", stdout);
    break;
  }

  return 0;
}