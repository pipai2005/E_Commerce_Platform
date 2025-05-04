#include"e_commerce.h"
#include"tcp_socket.h"

// 2025.4.17  暂时转学数据库（补）
#define PORT 8080

int main() {

	ECManager* ecManager = new ECManager;

	MySocket* mySocket = new MySocket;
	mySocket->createSocket();
	mySocket->bindSocket(PORT);
	mySocket->listenSocket(10);
	cout << "等待客户连接..." << endl;
	mySocket->acceptSocket();
	cout << "连接成功" << endl;
	ecManager->mySocket = mySocket;
	// 暂时实现一个用户的登录，后续再实现多用户登录
	int select;
	while (1) {
		// 进入主界面初始化用户数据
		ecManager->initAccount();
		ecManager->showOption();
		
		char buffer[BUFSIZ]= {0};
		mySocket->receiveData(buffer, BUFSIZ);
		select = buffer[0] - '0';

		switch (select) 
		{
		case 0:		// 退出平台
			ecManager->exitPlatform();
			break;

		case 1:		// 账户注册
			ecManager->registerAccount();
			break;

		case 2:		// 账户登录
			ecManager->loginAccount();
			break;

		case 3:		// 查看商品
			ecManager->showProduct();
			break;

		case 4:		// 查找商品
			ecManager->searchProduct(); 
			break;
		default:
			break;
		}
	}

	return 0;
}