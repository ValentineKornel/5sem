#include <windows.h>
#include <conio.h>

volatile int n;

DWORD WINAPI Add(LPVOID iNum)
{
  _cputs("Thread is started.\n");
  n += (int)iNum;
  _cputs("Thread is finished.\n");

  return 0;
}

int main()
{
  int  inc = 10;
  HANDLE  hThread;
  DWORD  IDThread;

  _cprintf("n = %d\n", n);
  // ��������� ����� Add
  hThread = CreateThread(NULL, 0, Add, (void*)inc, 0, &IDThread);
  if (hThread == NULL)
    return GetLastError();

  // ����, ���� ����� Add �������� ������
  WaitForSingleObject(hThread, INFINITE);
  // ��������� ���������� ������ Add
  CloseHandle(hThread);

  _cprintf("n = %d\n", n);

  return 0;
}