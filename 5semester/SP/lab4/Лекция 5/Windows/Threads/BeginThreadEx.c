#include <windows.h>
#include <conio.h>
#include <string.h>
#include <process.h>

UINT WINAPI thread(void *pString)
{
  int  i = 1;
  char  *pLexema;

  pLexema = strtok((char*) pString," ");
  while (pLexema != NULL)
  {
    _cprintf("Thread find the lexema %d : %s\n", i, pLexema);
    pLexema = strtok(NULL, " ");
    i++;
  }

  return 0;
}

int main()
{
  char  sentence[80];
  int  i, j, k = 0;
  HANDLE  hThread;
  UINT  IDThread;

  _cputs("Input string: \n");
  _cscanf_s("%s", &sentence);
  _cputs("\n");
  j = strlen(sentence);

  // ������� ����� ��� �������� ������
  hThread = (HANDLE) 
    _beginthreadex(NULL, 0, thread, sentence, 0, &IDThread);
  if (hThread == NULL)
    return GetLastError();
  
    // ���� ������������ ���������� ���� "�" � ������
  for (i=0; i<j; i++)
    if (sentence[i] == 'a')
      k++;
  _cprintf("Number of symbols 'a' in the string = %d\n", k);
  
  // ���� ��������� ������� �� �������
  WaitForSingleObject(hThread, INFINITE);
  // ��������� ���������� ������ thread
  CloseHandle(hThread);

  return 0;
}