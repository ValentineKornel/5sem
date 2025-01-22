#include <stdio.h>
#include <windows.h>


int main() {
  HANDLE hFile;
  BY_HANDLE_FILE_INFORMATION bhfi; // информация о файле

  // открываем файл для чтения
  hFile = CreateFile("C:\\demo_file.dat", // имя файла
                     0, // получение информации о файле
                     0, // монопольный доступ к файлу
                     NULL, // защиты нет
                     OPEN_EXISTING, // открываем существующий файл
                     FILE_ATTRIBUTE_NORMAL, // обычный файл
                     NULL                   // шаблона нет
  );
  // проверяем на успешное открытие
  if (hFile == INVALID_HANDLE_VALUE) {
    fprintf(stderr, "Create file failed.\nThe last error code: %ld\n",
            GetLastError());
    fputs("Press any key to finish.\n", stdout);
    getchar();

    return 0;
  }
  // получаем информацию о файле
  if (!GetFileInformationByHandle(hFile, &bhfi)) {
    fprintf(
        stderr,
        "Get file information by handle failed.\nThe last error code: %ld\n",
        GetLastError());
    fputs("Press any key to finish.\n", stdout);
    getchar();

    return 0;
  }
  // распечатываем информацию о файле
  fprintf(stdout, "File attributes: %ld\n", bhfi.dwFileAttributes);
  fprintf(stdout, "Creation time: high date: %ld\n",
          bhfi.ftCreationTime.dwHighDateTime);
  fprintf(stdout, "Creation time: low date: %ld\n",
          bhfi.ftCreationTime.dwLowDateTime);
  fprintf(stdout, "Last access time: high date: %ld\n",
          bhfi.ftLastAccessTime.dwHighDateTime);
  fprintf(stdout, "Last access time: low date: %ld\n",
          bhfi.ftLastAccessTime.dwLowDateTime);
  fprintf(stdout, "Last write time: high date: %ld\n",
          bhfi.ftLastWriteTime.dwHighDateTime);
  fprintf(stdout, "Last write time: low date: %ld\n",
          bhfi.ftLastWriteTime.dwLowDateTime);
  fprintf(stdout, "Volume serial number: %ld\n", bhfi.dwVolumeSerialNumber);
  fprintf(stdout, "File size high: %ld\n", bhfi.nFileSizeHigh);
  fprintf(stdout, "File size low: %ld\n", bhfi.nFileSizeLow);
  fprintf(stdout, "Number of links: %ld\n", bhfi.nNumberOfLinks);
  fprintf(stdout, "File index high: %ld\n", bhfi.nFileIndexHigh);
  fprintf(stdout, "File index low: %ld\n", bhfi.nFileIndexLow);

  // закрываем дескриптор файла
  CloseHandle(hFile);

  return 0;
}