#pragma once
#include<winsock2.h>
#pragma comment(lib,"ws2_32.lib")

#include <iostream>

class MySocket {
public:
	MySocket();
	~MySocket();

	void createSocket();
	void bindSocket(int port);
	void listenSocket(int backlog);
	void acceptSocket();
	void sendData(const char* data, int size);
	void receiveData(char* buffer, int size);
	void closeSocket();

	WSADATA wsaData;
	SOCKET sock;
	SOCKET clientSock;
};

