#pragma once

#pragma once
#include<winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib,"ws2_32.lib")

#include <iostream>

#define PORT 8080

class MySocket {
public:
	MySocket();
	~MySocket();
	void createSocket();
	void connectSocket();
	void sendData(const char* data, int size);
	void receiveData(char* buffer, int size);
	void closeSocket();
	WSADATA wsaData;
	SOCKET clientSock;
};
