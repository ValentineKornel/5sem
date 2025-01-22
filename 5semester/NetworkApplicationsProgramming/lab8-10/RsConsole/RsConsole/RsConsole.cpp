#include <stdio.h>
#include <tchar.h>
#include "iostream"
#include "Windows.h"                
#include "ctime"
#include <string>
#pragma warning(disable : 4996)
using std::string;
using namespace std;

string GetErrorMsgText(int code) // cформировать текст ошибки
{
	char buff[50];
	DWORD bufflen = sizeof(buff);
	DWORD errorMessageID = ::GetLastError();
	if (errorMessageID == 0) {
		return std::string(); //No error message has been recorded
	}
	else {
		return std::system_category().message(errorMessageID);
	}
};
string SetPipeError(string msgText, int code)
{
	return msgText + to_string(code) + ". " + GetErrorMsgText(code);
};

void SendMessageToServers(char* serverName, char* message) {
	HANDLE cH = CreateFileA(serverName, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (cH == INVALID_HANDLE_VALUE) {
		cerr << "Failed to create file. Error: " << GetLastError() << endl;
	}
	else
	{
		cout << "File created successfully" << endl;
	}

	DWORD bytesSended;
	if (!WriteFile(cH, message, strlen(message), &bytesSended, NULL)) {
		cerr << "Failed to write to Mailslot. Error: " << GetLastError() << endl;
	}
	else {
		cout << "Message sent successfully. Bytes sent: " << bytesSended << endl;
	}

	CloseHandle(cH);
}


int _tmain(int argc, _TCHAR* argv[]) {
	setlocale(LC_ALL, "Russian");

	char ReadBuf[200] = "";
	char WriteBuf[2] = "";
	DWORD nBytesRead;
	DWORD nBytesWrite;

	SECURITY_DESCRIPTOR* m_pSecDesc = (SECURITY_DESCRIPTOR*)LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
	SECURITY_ATTRIBUTES m_pSecAttrib = { sizeof(SECURITY_ATTRIBUTES), m_pSecDesc, TRUE };
	InitializeSecurityDescriptor(m_pSecDesc, SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(m_pSecDesc, TRUE, (PACL)NULL, FALSE);


	int Code = 0;

	char serverName[256];
	const char PipeName[] = "\\\\.\\pipe\\Tube";
	bool result;

	try
	{
		printf_s("\n ---------- Доступные команды ---------- \n");
		printf_s("1 - start  \t (разрешить подключение клиентов к серверу)\n");
		printf_s("2 - stop  \t (запретить подключение клиентов к серверу)\n");
		printf_s("3 - exit  \t (завершить работу сервера)\n");
		printf_s("4 - statistics\t (вывод статистики)\n");
		printf_s("5 - wait  \t (приостанавливает подключение клиентов)\n");
		printf_s("6 - shutdown  \t (wait + exit)\n");
		printf_s("0 - закрыть\n");
		printf_s("\n ---------- ---------- ---------- ---------- \n");


		HANDLE hNamedPipe = CreateFileA(PipeName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, &m_pSecAttrib);

		do {
			printf_s("Команда: ");
			scanf_s("%d", &Code);
			if (Code > 0 && Code < 8) {
				if (Code == 7) {
					getchar();
					string rand_working_time, time_working_time, echo_working_time;
					cout << "input working time for RAND service in format hh:mm - hh:mm\n";
					getline(cin, rand_working_time);
					cout << "input working time for TIME service in format hh:mm - hh:mm\n";
					getline(cin, time_working_time);
					cout << "input working time for ECHO service in format hh:mm - hh:mm\n";
					getline(cin, echo_working_time);


					char nameServer[40] = "\\\\.\\mailslot\\Box";
					//char message[55] = "RAND:8:00-12:00; TIME:09:00-10:00; ECHO:11:00-12:00";
					char message[70];
					sprintf(message, "RAND:%s; TIME:%s; ECHO:%s", rand_working_time.c_str(), time_working_time.c_str(), echo_working_time.c_str());
					cout << message << endl;
					SendMessageToServers(nameServer, message);
				}
				else {
					sprintf_s(WriteBuf, "%d", Code);
					if (!WriteFile(hNamedPipe, WriteBuf, strlen(WriteBuf) + 1, &nBytesWrite, NULL)) throw "WriteFile: Ошибка ";
					if (!ReadFile(hNamedPipe, ReadBuf, sizeof(ReadBuf), &nBytesRead, NULL)) throw "ReadFile: Ошибка ";
					cout << ReadBuf << endl;
				}
			}
			if (Code == 0) break;
		} while (true);

		if (!CloseHandle(hNamedPipe)) throw SetPipeError("CloseHandle: ", GetLastError());
	}
	catch (string ErrorPipeText)
	{
		cout << endl << ErrorPipeText;
	}
	cout << "RConsole остановлена\n\n";
	return 0;
}
