#include <windows.h>
#include <conio.h>

using namespace std;

volatile int a[10];
HANDLE hSemaphore;

DWORD WINAPI thread(LPVOID)
{
  for (int i = 0; i < 10; i++)
  {
    a[i] = i + 1;
    // отмечаем, что один элемент готов
    ReleaseSemaphore(hSemaphore,1,NULL);
    Sleep(500);
  }
    
  return 0;
}

int main()
{
  int i;
  HANDLE  hThread;
  DWORD  IDThread;
 
  _cputs("An initial state of the array: ");
  for (i = 0; i < 10; i++)
    _cprintf("%d ", a[i]);
  _cputs("\n");
  // создаем семафор    
  hSemaphore=CreateSemaphore(NULL, 0, 10, NULL);
  if (hSemaphore == NULL)
    return GetLastError();
    
  // создаем поток, который готовит элементы массива
  hThread = CreateThread(NULL, 0, thread, NULL, 0, &IDThread);
  if (hThread == NULL)
    return GetLastError();

  // поток main выводит элементы массива 
  // только после их подготовки потоком thread
  _cputs("A final state of the array: ");
  for (i = 0; i < 10; i++)
  {
    WaitForSingleObject(hSemaphore, INFINITE);
	_cprintf("%d \a", a[i]);
  }
  _cputs("\n");

  CloseHandle(hSemaphore);
  CloseHandle(hThread);

  return 0;
}