#include"../e_commerce_platform/tcp_socket.h"

MySocket::MySocket() {
	// Initialize Winsock( Windows Socket Async)
	if (0 != WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		std::cerr << "WSAStartup failed: " << WSAGetLastError() << std::endl;
		exit(1);
	}
}
MySocket::~MySocket() {
	// Cleanup Winsock
	WSACleanup();
}
void MySocket::createSocket() {
	//           协议族（ipv4/6）  socket类型(流式/数据报)  协议
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) {
		std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
		exit(1);
	}
};
void MySocket::bindSocket(int port) {
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);  // “host to network short” 转成网络字节序，大端序
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	if (bind(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
		exit(1);
	}
};
void MySocket::listenSocket(int backlog) {
	if (listen(sock, backlog) == SOCKET_ERROR) {
		std::cerr << "Listen failed: " << WSAGetLastError() << std::endl;
		exit(1);
	}
};
void MySocket::acceptSocket() {
	sockaddr_in clientAddr;
	int addrLen = sizeof(clientAddr);
	clientSock = accept(sock, (sockaddr*)&clientAddr, &addrLen);
	if (clientSock == INVALID_SOCKET) {
		std::cerr << "Accept failed: " << WSAGetLastError() << std::endl;
		exit(1);
	}
};
void MySocket::sendData(const char* data, int size) {
	if (send(clientSock, data, size, 0) == SOCKET_ERROR) {
		std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
		exit(1);
	}
};
void MySocket::receiveData(char* buffer, int size) {
	if (recv(clientSock, buffer, size, 0) == SOCKET_ERROR) {
		std::cerr << "Receive failed: " << WSAGetLastError() << std::endl;
		exit(1);
	}
};
void MySocket::closeSocket() {
	closesocket(clientSock);
	closesocket(sock);
}