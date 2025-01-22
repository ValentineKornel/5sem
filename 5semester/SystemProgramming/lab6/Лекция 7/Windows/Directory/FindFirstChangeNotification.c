#include <windows.h>
#include <stdio.h>

int main()
{
  HANDLE  hChangeHandle;
  
  // находим первое изменение в каталоге
  hChangeHandle = FindFirstChangeNotification(
    "C:\\demo_dir",    // имя каталога
    TRUE,              // отслеживать также подкаталоги
    FILE_NOTIFY_CHANGE_FILE_NAME // отслеживать изменение имени каталога
    | FILE_NOTIFY_CHANGE_SIZE    // и изменение его размера
  );

  if (hChangeHandle == INVALID_HANDLE_VALUE)
  {
    fprintf(stderr, "Find first change notification failed.\nThe last error code: %ld\n", GetLastError());
    puts("Press any key to finish\n");
    getchar();

    return 0;
  }
  // нужно изменить каталог
  puts("Wait for changes in the directory.\n");
  // ждем первого изменения в каталоге
  if (WaitForSingleObject(hChangeHandle, INFINITE) == WAIT_OBJECT_0)
    puts("First notification: the directory was changed.\n");
  else
  {
    fprintf(stderr, "Wait for single object failed.\nThe last error code: %ld\n", GetLastError());
    getchar();

    return 0;
  }
  // находим второе изменение в каталоге
  if (!FindNextChangeNotification(hChangeHandle))
  {
    fprintf(stderr, "Find next change notification failed.\nThe last error code: %ld\n", GetLastError());
    fputs("Press any key to finish.\n", stdout);
    getchar();

    return 0;
  }
  // ждем второго изменения в каталоге
  if (WaitForSingleObject(hChangeHandle, INFINITE) == WAIT_OBJECT_0)
    puts("Next notification: the directory was changed.\n");
  else
  {
    fprintf(stderr, "Wait for single object failed.\nThe last error code: %ld\n", GetLastError());
    fputs("Press any key to finish.\n", stdout);
    getchar();

    return 0;
  }
  // закрываем дескриптор поиска
  FindCloseChangeNotification(hChangeHandle);

  return 0;
}