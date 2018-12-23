#include "stdafx.h"
#include <ws2tcpip.h>
#include <iostream>
#include <string>

#pragma comment (lib, "Ws2_32.lib")


using namespace std;

int main()
{
	// initialize winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);
	int ws0k = WSAStartup(ver,&wsData);
	if (ws0k != 0)
	{
		cerr << "Can't Initialize winsock! Quitting" << endl;
		return 0;
	}
	// create a socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		cerr << "Can't create a socket! Quitting" << endl;
		return 0;
	}
	// Bind the socket to an ip address and port
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(listening, (sockaddr*)&hint, sizeof(hint));
	// Tell winsock the socket is for listening
	listen(listening, SOMAXCONN);
	//Wait for a connection
	sockaddr_in client;
	int clientsize = sizeof(client);

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientsize);
	char host[NI_MAXHOST]; // client's remote name
	char service[NI_MAXHOST]; //Service (i.e. port) the client is connect on

	ZeroMemory(host, NI_MAXHOST); // same as memset(host,0,NI_MAXHOST)
	ZeroMemory(service, NI_MAXHOST);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		cout << host << "connected on port" << service << endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << "connected on port" <<
			ntohs(client.sin_port) << endl;

	}
	// Close listening socket
	closesocket(listening);

	//While loop: accept and echo message back to client
	char buf[4096];
	while (true)
	{
		ZeroMemory(buf, 4096);
		//Wait for client to send data
		int bytesReceived = recv(clientSocket, buf, 4096, 0);
		if (bytesReceived == SOCKET_ERROR)
		{
			cerr << "Error in recv(). Quitting" << endl;
			break;
		}
		int a = stoi(string(buf, 0, bytesReceived));
		if (a == 0)
		{
			cout << "Client disconnected" << endl;
			break;
		}
		// Echo message back to client
		
		cout << "Dostalem liczbe: " << a<<endl;
		int suma=0;
		for (int i = 1; i < a; i++)
		{
			if (a%i == 0)
			{
				suma = suma + i;
			}
		}
		string odp;
		if (suma == a)
		{
			cout << "Liczba jest doskonala"<<endl;
			odp = "Liczba jest doskonala";
		}
		else
		{
			cout << "Liczba nie jest doskonala" << endl;
			odp = "Liczba nie jest doskonala";
		}
		
		send(clientSocket, odp.c_str(), odp.size(), 0);
			
	}
	// Close the socket
	closesocket(clientSocket);
	// cleanup winsock
	WSACleanup();
	return 0;
}
