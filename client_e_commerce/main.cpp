// UTF-8 编码

#include"../client_e_commerce/tcp_socket.h"
#include"../client_e_commerce/Widget.h"
#include"../client_e_commerce/AccountWidget.h"
#include<Windows.h>

using namespace std;

// 客户端主函数
int main() {
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	ECManager* ecManager = new ECManager;

	MySocket* mySocket = new MySocket;
	mySocket->createSocket();
	cout << "服务器连接中..." << endl;
	mySocket->connectSocket();
	cout << "服务器连接成功! " << endl;
	ecManager->mySocket = mySocket;
	ecManager->account->cfd = mySocket;

	// 连接成功后，发送登录请求
	int select;
	while (1) {
		// 进入主界面初始化用户数据
		ecManager->showOption();

		cout << "请输入您的操作" << endl;
		cin >> select;
		char buffer[BUFSIZ] = { 0 };
		sprintf(buffer, "%d", select);
		mySocket->sendData(buffer, BUFSIZ);

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
			system("cls");
			break;
		}

		system("pause");
		system("cls");
	}

	return 0;
}