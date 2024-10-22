#include <windows.h>
#include <conio.h>

volatile UINT  nCount;
volatile DWORD dwCount;

void thread()
{
  for (;;)
  {
    nCount++;
    // ���������������� ����� �� 100 �����������
    Sleep(100);
  }
}

int main()
{
  HANDLE   hThread;
  DWORD  IDThread;
  char c;

  hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread, NULL, 
                         0, &IDThread);
  if (hThread == NULL)
    return GetLastError();

  for (;;)
  {
    _cprintf("Input :\n");
    _cprintf("\t'n' to exit\n");
    _cprintf("\t'y' to display the count\n");
    _cprintf("\t's' to suspend thread\n");
    _cprintf("\t'r' to resume thread\n");
    _cscanf_s("%c", &c);

    if (c == 'n')
      break;
    switch (c)
    {
    case 'y':
      _cprintf("\ncount = %d\n", nCount);
      break;
    case 's':
      // ���������������� ����� thread
      dwCount = SuspendThread(hThread);
      _cprintf("\nThread suspend count = %d\n", dwCount);
      break;
    case 'r':
      // ������������ ����� thread
      dwCount = ResumeThread(hThread);
      _cprintf("\nThread suspend count = %d\n", dwCount);
      break;
    }
  }
  
  // ��������� ���������� ������ thread
  TerminateThread(hThread, 0);
  // ��������� ���������� ������ thread
  CloseHandle(hThread);

  return 0;
}