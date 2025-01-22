#include <windows.h>
#include <conio.h>

int main()
{
  HANDLE  hMutex;
  char  lpszAppName[] = "ConsoleProcess.exe";
  STARTUPINFO  si;
  PROCESS_INFORMATION  pi;

  // ������� �������
  hMutex = CreateMutex(NULL, FALSE, "DemoMutex");
  if (hMutex == NULL)
  {
    _cputs("Create mutex failed.\n");
    _cputs("Press any key to exit.\n");
    _getch();

    return GetLastError();
  }

  ZeroMemory(&si, sizeof(STARTUPINFO));
  si.cb = sizeof(STARTUPINFO);

  // ������� ����� ���������� �������
  if (!CreateProcess(lpszAppName, NULL, NULL, NULL, FALSE,
      0, NULL, NULL, &si, &pi))
  {
	_cputs("The new process is not created.\n");
    _cputs("Press any key to exit.\n");
    _getch();

    return GetLastError();
  }

  // ������� �� ����� ������
  for (int j = 0; j < 10; ++j)
  {
    // ����������� �������
    WaitForSingleObject(hMutex, INFINITE);
    for (int i = 0; i < 10; i++)
    {
      _cprintf("%d ", j);
      Sleep(10);
    }
    _cputs("\n");
    // ����������� �������
    ReleaseMutex(hMutex);
  }
  // ��������� ���������� ��������
  CloseHandle(hMutex);

  // ���� ���� �������� ������� �������� ������
  WaitForSingleObject(pi.hProcess, INFINITE);
  
  // ��������� ����������� ��������� �������� � ������� ��������
  CloseHandle(pi.hThread);
  CloseHandle(pi.hProcess);

  return 0;
}