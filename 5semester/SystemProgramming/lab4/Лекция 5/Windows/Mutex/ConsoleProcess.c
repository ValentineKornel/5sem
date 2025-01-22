#include <windows.h>
#include <conio.h>

int main()
{
  HANDLE  hMutex;
  int    i,j;

  // открываем мьютекс
  hMutex = OpenMutex(SYNCHRONIZE, FALSE, "DemoMutex");
  if (hMutex == NULL)
  {
    _cputs("Open mutex failed.\n");
    _cputs("Press any key to exit.\n");
    _getch();

    return GetLastError();
  }

  for (j = 10; j < 20; j++)
  {
    // захватываем мьютекс
    WaitForSingleObject(hMutex, INFINITE);
    for (i = 0; i < 10; i++)
    {
      _cprintf("%d ", j);
      Sleep(5);
    }
    _cputs("\n");
    // освобождаем мьютекс
    ReleaseMutex(hMutex);
  }
  // закрываем дескриптор объекта
  CloseHandle(hMutex);
    
  return 0;
}
