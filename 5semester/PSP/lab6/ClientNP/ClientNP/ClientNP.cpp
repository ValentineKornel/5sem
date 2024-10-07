#include <iostream>
#include "Winsock2.h"
#pragma comment(lib, "WS2_32.lib")
#pragma warning(disable : 4996)
using namespace std;


#define INADDR_ANY        (u_long)0x00000000 //����� �����       +++ 
#define INADDR_LOOPBACK    0x7f000001        // ���������� ����� +++
#define INADDR_BROADCAST  (u_long)0xffffffff // �������������    +++  
#define INADDR_NONE        0xffffffff        // ��� ������  
#define ADDR_ANY           INADDR_ANY        // ����� �����       

typedef struct sockaddr_in SOCKADDR_IN;    //                     +++
typedef struct sockaddr_in* PSOCKADDR_IN;
typedef struct sockaddr_in FAR* LPSOCKADDR_IN;

string GetErrorMsgText(int code)
{
	string msgText;
	switch (code)
	{
	case WSAEINTR: msgText = "������ ������� ��������"; break;
	case WSAEACCES:	msgText = "���������� ����������"; break;
	case WSAEFAULT: msgText = "��������� �����"; break;
	case WSAEINVAL:	msgText = "������ � ���������";	break;
	case WSAEMFILE:	msgText = "������� ������� ����� ������"; break;
	case WSAEWOULDBLOCK: msgText = "������ �������� ����������"; break;
	case WSAEINPROGRESS: msgText = "�������� � �������� ��������"; break;
	case WSAEALREADY: msgText = "�������� ��� �����������";	break;
	case WSAENOTSOCK: msgText = "����� ����� �����������"; break;
	case WSAEDESTADDRREQ: msgText = "��������� ����� ������������"; break;
	case WSAEMSGSIZE: msgText = "��������� ������� �������"; break;
	case WSAEPROTOTYPE: msgText = "������������ ��� ��������� ��� ������"; break;
	case WSAENOPROTOOPT: msgText = "������ � ����� ���������"; break;
	case WSAEPROTONOSUPPORT: msgText = "�������� �� ��������������"; break;
	case WSAESOCKTNOSUPPORT: msgText = "��� ������ �� ��������������"; break;
	case WSAEOPNOTSUPP: msgText = "�������� �� ��������������"; break;
	case WSAEPFNOSUPPORT: msgText = "��� ���������� �� ��������������"; break;
	case WSAEAFNOSUPPORT: msgText = "��� ������� �� �������������� ����������"; break;
	case WSAEADDRINUSE: msgText = "����� ��� ������������"; break;
	case WSAEADDRNOTAVAIL: msgText = "����������� ����� �� ����� ���� �����������"; break;
	case WSAENETDOWN: msgText = "���� ���������"; break;
	case WSAENETUNREACH: msgText = "���� �� ���������"; break;
	case WSAENETRESET: msgText = "���� ��������� ����������"; break;
	case WSAECONNABORTED: msgText = "����������� ����� �����"; break;
	case WSAECONNRESET: msgText = "����� �� �������������"; break;
	case WSAENOBUFS: msgText = "�� ������� ������ ��� �������"; break;
	case WSAEISCONN: msgText = "����� ��� ���������"; break;
	case WSAENOTCONN: msgText = "����� �� ���������"; break;
	case WSAESHUTDOWN: msgText = "������ ��������� send: ����� �������� ������"; break;
	case WSAETIMEDOUT: msgText = "���������� ���������� �������� �������"; break;
	case WSAECONNREFUSED: msgText = "���������� ���������"; break;
	case WSAEHOSTDOWN: msgText = "���� � ����������������� ���������"; break;
	case WSAEHOSTUNREACH: msgText = "��� �������� ��� �����"; break;
	case WSAEPROCLIM: msgText = "������� ����� ���������"; break;
	case WSASYSNOTREADY: msgText = "���� �� ��������"; break;
	case WSAVERNOTSUPPORTED: msgText = "������ ������ ����������"; break;
	case WSANOTINITIALISED: msgText = "�� ��������� ������������� WS2_32.dll"; break;
	case WSAEDISCON: msgText = "����������� ����������"; break;
	case WSATYPE_NOT_FOUND: msgText = "����� �� ������"; break;
	case WSAHOST_NOT_FOUND: msgText = "���� �� ������"; break;
	case WSATRY_AGAIN: msgText = "���������������� ���� �� ������"; break;
	case WSANO_RECOVERY: msgText = "�������������� ������"; break;
	case WSANO_DATA: msgText = "��� ������ ������������ ����"; break;
	case WSASYSCALLFAILURE: msgText = "��������� ���������� ���������� ������"; break;
	case 2: msgText = "��������� ����������"; break;
	case ERROR_INVALID_PARAMETER: msgText = "�������� ��������� pimax ����������� ��������  PIPE_UNLMITED_INSTANCES"; break;
	case ERROR_NO_DATA: msgText = "The pipe is being closed"; break;
	case ERROR_PIPE_CONNECTED: msgText = "There is a process on other end of the pipe"; break;
	case ERROR_PIPE_LISTENING: msgText = "Waiting for a process to open the other end of the pipe"; break;
	case ERROR_CALL_NOT_IMPLEMENTED: msgText = "This function is not supported on this system"; break;
	default: msgText = "**ERROR**"; break;
	}
	return msgText;
};

string SetPipeError(string msgText, int code)
{
    return msgText + GetErrorMsgText(code);
}

//4
//int main()
//{
//    setlocale(LC_ALL, "Russian");
//    HANDLE hPipe;
//
//    try {
//        if ((hPipe = CreateFileA(
//            "\\\\.\\pipe\\Tube",
//            GENERIC_READ | GENERIC_WRITE,
//            FILE_SHARE_READ | FILE_SHARE_WRITE,
//            NULL, OPEN_EXISTING, NULL,
//            NULL)) == INVALID_HANDLE_VALUE)
//            throw SetPipeError("createfile: ", GetLastError());
//
//        cout << "connected to the server." << endl;
//
//
//		char wbuf[50] = "hello from client";
//		LPDWORD wbufl = 0;
//		if (!WriteFile(hPipe, wbuf, sizeof(wbuf), wbufl, NULL))
//			throw SetPipeError("write: ", GetLastError());
//		cout << "message sent." << endl;
//
//
//		char rbuf[50];
//		LPDWORD rbufl = 0;
//		if (!ReadFile(hPipe, rbuf, sizeof(rbuf), rbufl, NULL));
//
//		cout << "Message: " << rbuf << endl;
//
//
//
//        DisconnectNamedPipe(hPipe);
//        CloseHandle(hPipe);
//    }
//    catch (string errorMsgText) {
//        cout << endl << "Error: " << errorMsgText;
//        CloseHandle(hPipe);
//    }
//
//
//    return 0;
//}

//5
int main()
{
	setlocale(LC_ALL, "Russian");
	HANDLE hPipe;

	try {
		if ((hPipe = CreateFileA(
			"\\\\DESKTOP-0M3BPJP\\pipe\\Tube",
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, OPEN_EXISTING, NULL,
			NULL)) == INVALID_HANDLE_VALUE)
			throw SetPipeError("createfile: ", GetLastError());

		cout << "connected to the server." << endl;

		int n;
		cout << "input nubmer: ";
		cin >> n;
		clock_t start = clock();

		for (int i = 0; i < n; i++) {
			char wbuf[50];
			sprintf(wbuf, "Hello server from Client %d", i);
			LPDWORD wbufl = 0;
			if (!WriteFile(hPipe, wbuf, sizeof(wbuf), wbufl, NULL))
				throw SetPipeError("write: ", GetLastError());
			cout << "message sent." << endl;



			char rbuf[50];
			DWORD rbufl = 0;
			if (!ReadFile(hPipe, rbuf, sizeof(rbuf)-1, &rbufl, NULL));

			cout << "Message: " << rbuf << endl;
		}
		char wbuf[3] = "\0";
		if (!WriteFile(hPipe, wbuf, sizeof(wbuf)-1, 0, NULL))
			throw SetPipeError("write: ", GetLastError());
		cout << "last message sent." << endl;

		clock_t end = clock();
		double duration = static_cast<double>(end - start) / CLOCKS_PER_SEC;
		std::cout << "����� ����������: " << duration << " ������" << std::endl;


		DisconnectNamedPipe(hPipe);
		CloseHandle(hPipe);
	}
	catch (string errorMsgText) {
		cout << endl << "Error: " << errorMsgText;
		CloseHandle(hPipe);
	}


	return 0;
}
