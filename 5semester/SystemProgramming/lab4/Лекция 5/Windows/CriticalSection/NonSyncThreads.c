#include <windows.h>
#include <conio.h>

DWORD WINAPI thread(LPVOID)
{
  int i,j;
  
  for (j = 0; j < 10; ++j)
  {
    // ������� ������ ����� j
    for (i = 0; i < 10; ++i)
    {
      _cprintf("%d ", j);
      Sleep(17);
    }
    _cputs("\n");
  }
    
  return 0;
}

int main()
{
  int i,j;
  HANDLE  hThread;
  DWORD  IDThread;

  hThread=CreateThread(NULL, 0, thread, NULL, 0, &IDThread);
  if (hThread == NULL)
    return GetLastError();

for (j = 10; j < 20; ++j)
  {
    for (i = 0; i < 10; ++i)
    {
      // ������� ������ ����� j
      _cprintf("%d ", j);
      Sleep(17);
    }
    _cputs("\n");
  }
  // ����, ���� ����� thread �������� ���� ������
  WaitForSingleObject(hThread, INFINITE);

  return 0;
}