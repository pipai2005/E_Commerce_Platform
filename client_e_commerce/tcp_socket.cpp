#include"../client_e_commerce/tcp_socket.h"


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
	clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSock == INVALID_SOCKET) {
		std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
		exit(1);
	}
};
void MySocket::connectSocket() {
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);  // “host to network short” 转成网络字节序，大端序
	//serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	// 使用 inet_pton 替代 inet_addr
	if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) <= 0) {
		std::cerr << "无效的地址或地址不支持" << std::endl;
		exit(1);
	}
	if ((connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)) {
		std::cerr << "Connect failed: " << WSAGetLastError() << std::endl;
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
	/*if (recv(clientSock, buffer, size, 0) == SOCKET_ERROR) {
		std::cerr << "Receive failed: " << WSAGetLastError() << std::endl;
		exit(1);
	}*/
	int result;
	int retryCount = 3; // 重试次数
	while (retryCount > 0) {
		result = recv(clientSock, buffer, size, 0);
		if (result == SOCKET_ERROR) {
			int errorCode = WSAGetLastError();
			if (errorCode == WSAECONNRESET) {
				std::cerr << "连接被对方重置，尝试重新连接..." << std::endl;
				// 关闭当前套接字
				closesocket(clientSock);
				// 创建新的套接字
				createSocket();
				// 重新连接服务器
				connectSocket();
				retryCount--;
			}
			else {
				std::cerr << "Receive failed: " << errorCode << std::endl;
				exit(1);
			}
		}
		else {
			break;
		}
	}
	if (retryCount == 0) {
		std::cerr << "重试次数已用完，无法接收数据。" << std::endl;
		exit(1);
	}
};
void MySocket::closeSocket() {
	closesocket(clientSock);
}