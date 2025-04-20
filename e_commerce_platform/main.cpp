#include"e_commerce.h"

// 2025.4.17  暂时转学数据库（补）

int main() {

	ECManager* ecManager = new ECManager;
	ecManager->initAccount();

	int select;
	while (1) {
		ecManager->showOption();
		
		cout << "请输入您的操作" << endl;
		cin >> select;

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