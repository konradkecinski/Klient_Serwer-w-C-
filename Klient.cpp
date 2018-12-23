#include"stdafx.h"
#include<iostream>
#include<WS2tcpip.h>
#include<string>
#pragma comment(lib,"ws2_32.lib")

using namespace std;

void main()
{
	string ipAddress = "127.0.0.1";
	int port = 54000;
	// Initialize winsock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		cerr << "Can't start Winsock, Err #" << wsResult << endl;
		return;
	}

	// Create sock
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock == INVALID_SOCKET)
		{
			cerr << "Can't create socket, ERR #" << WSAGetLastError() << endl;
		}

	// Fill in a hint structure
		sockaddr_in hint;
		hint.sin_family = AF_INET;
		hint.sin_port = htons(port);
		inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);
	// Connect to server
		int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
		if (connResult == SOCKET_ERROR)
		{
			cerr << "Can't connect to server, Err #" << WSAGetLastError() << endl;
			WSACleanup();
			return;
		}
	// Do-while look to sand and receive data
		char buf[4096];
		string userInput;
		int liczba;
		do
		{
			//Prompt the user
			cout << "Wpisz liczbe by sprawdzic czy jest doskonala: ";
			cin >> liczba;
			userInput = to_string(liczba);
			if (userInput.size() > 0)
			{


				// Send the text

				int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
				if (sendResult != SOCKET_ERROR)
				{
					//Wait for response
					ZeroMemory(buf, 4096);
					int bytesReceived = recv(sock, buf, 4096, 0);
					if (bytesReceived > 0)
					{
						// Echo response to console
						cout << "SERVER> " << string(buf, 0, bytesReceived) << endl;
					}

				}
			}
		} while (liczba!=0);
	// Gracefully close down everything
		closesocket(sock);
		WSACleanup();

}
