#include <windows.h>
#include <conio.h>

CRITICAL_SECTION cs;

DWORD WINAPI thread(LPVOID)
{
  int i,j;
  
  for (j = 0; j < 10; ++j)
  {
    // ������ � ����������� ������
    EnterCriticalSection (&cs);
     for (i = 0; i < 10; ++i)
    {
      _cprintf("%d ", j);
      Sleep(7);
    }
    _cputs("\n");
    // ������� �� ����������� ������
    LeaveCriticalSection(&cs);
  }
    
  return 0;
}

int main()
{
  int i,j;
  HANDLE  hThread;
  DWORD  IDThread;

  // �������������� ����������� ������
  InitializeCriticalSection(&cs);

  hThread=CreateThread(NULL, 0, thread, NULL, 0, &IDThread);
  if (hThread == NULL)
    return GetLastError();

  for (j = 10; j < 20; ++j)
  {
    // ������ � ����������� ������
    EnterCriticalSection(&cs);
     for (i = 0; i < 10; ++i)
    {
      _cprintf("%d ", j);
      Sleep(7);
    }
    _cputs("\n");
    // ������� �� ����������� ������
    LeaveCriticalSection(&cs);
  }
  // ����, ���� ����� thread �������� ���� ������
  WaitForSingleObject(hThread, INFINITE);  
  // ��������� ����������� ������
  DeleteCriticalSection(&cs);

  return 0;
}