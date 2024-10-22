#include <windows.h>
#include <conio.h>

int main()
{
  HANDLE  hThread;

  // �������� ���������������� �������� ������
  hThread = GetCurrentThread();
  // ������� ���������������� �� �������
  _cprintf("n = %d\n", hThread);

  _getch();

  return 0;
}