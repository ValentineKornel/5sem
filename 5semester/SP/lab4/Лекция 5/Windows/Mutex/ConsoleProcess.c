#include <windows.h>
#include <conio.h>

int main()
{
  HANDLE  hMutex;
  int    i,j;

  // ��������� �������
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
    // ����������� �������
    WaitForSingleObject(hMutex, INFINITE);
    for (i = 0; i < 10; i++)
    {
      _cprintf("%d ", j);
      Sleep(5);
    }
    _cputs("\n");
    // ����������� �������
    ReleaseMutex(hMutex);
  }
  // ��������� ���������� �������
  CloseHandle(hMutex);
    
  return 0;
}
