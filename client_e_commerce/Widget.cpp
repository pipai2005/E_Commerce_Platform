#include"Widget.h"
#include"AccountWidget.h"

// 商品类型枚举
map<int, string> productType{
	{1, "books"},
	{2, "clothes"},
	{3, "electronic"},
	{4, "food"}
};

ECManager::ECManager() {
	this->account = new Bussiness;
}

ECManager::~ECManager() {
	if (this->account != nullptr)
		delete this->account;
}

void ECManager::showOption() {
	cout << "*********************************************" << endl;
	cout << "*************  电子电商平台  *****************" << endl;
	cout << "*********************************************" << endl;
	cout << "*************** 1 账户注册 *******************" << endl;
	cout << "*************** 2 账户登录 *******************" << endl;
	cout << "*************** 3 查看商品 *******************" << endl;
	cout << "*************** 4 查找商品 *******************" << endl;
	cout << "*************** 0 退出平台 *******************" << endl;
	cout << "*********************************************" << endl;
	cout << endl;
}

void ECManager::registerAccount() {
	string namee;
	string passwordd;
	int typee;

	cout << "下面进行用户注册操作" << endl;

	cout << "请输入您想注册的账户类型" << endl;
	cout << "* 1. 商家 " << endl;
	cout << "* 2. 普通用户" << endl;
	cout << endl;
	cin >> typee;
	while (typee != 1 && typee != 2) {
		cout << "输入错误，请重新操作" << endl;
		cin.clear();
		cin.ignore(INT_MAX, '\n');  // 忽略错误缓存
	}
	cout << "请输入您的用户名:" << endl;
	cin >> namee;
	cout << "请输入您的账户密码" << endl;
	cin >> passwordd;

	char buffer[BUFSIZ];
	sprintf(buffer, "%d %d %s %s", 1, typee, namee.c_str(), passwordd.c_str());
	this->mySocket->sendData(buffer,BUFSIZ);
	cout << "正在注册，请稍后..." << endl;
	memset(buffer, 0, BUFSIZ);
	this->mySocket->receiveData(buffer, BUFSIZ);
	if (buffer[0] == '-') {
		cout << "用户名重复，要不重新试试" << endl;
	}
	else {
		cout << "注册成功，请登录" << endl;
	}
}

void ECManager::loginAccount() {
	string line_name;			// 输入行的账户名
	string line_password;		// 输入行的密码
	int line_type;				// 输入行的账户类型

	cout << "请输入您想进行登录的账户类型" << endl;
	cout << "* 1. 商家 " << endl;
	cout << "* 2. 普通用户" << endl;
	cout << endl;
	cin >> line_type;

	while (line_type != 1 && line_type != 2) {
		cout << "账户类型输入错误，请重新操作" << endl;
		cin.clear();
		cin.ignore(INT_MAX, '\n');  // 忽略错误缓存

		cin >> line_type;
	}

	cout << "下面进行登录操作" << endl;
	cout << "请输入您的账户名：" << endl;
	cin >> line_name;
	cout << "请输入您的密码：" << endl;
	cin >> line_password;

	// 下面匹配账户名和密码（）
	char buffer[BUFSIZ];
	sprintf(buffer, "%d %d %s %s", 2, line_type, line_name.c_str(), line_password.c_str());
	this->mySocket->sendData(buffer, BUFSIZ);
	cout << "正在登录，请稍后..." << endl;
	memset(buffer, 0, BUFSIZ);
	// 接收服务器返回的结果（ - 1 / 0 / 1 : 用户名或密码错误 / 平台无账户信息 / 登录成功）
	this->mySocket->receiveData(buffer, BUFSIZ);
	if (buffer[0] == '-') {
		cout << "用户名或密码错误，请重新输入" << endl;
	}
	else if (buffer[0] == '0') {
		cout << "平台无账户信息，请先注册" << endl;
	}
	else if (buffer[0] == '1') {
		cout << "登录成功，欢迎您！" << endl;
		system("pause");
		system("cls");
		// 登录成功，显示菜单初始化账户信息
		int id;
		double balance;
		// 解析 buffer
		sscanf(buffer, "1 %d %lf", &id, &balance);
		memset(buffer, 0, BUFSIZ);
		
		if (line_type == 1) {
			// 先消除之前的账户信息
			if (this->account != nullptr)
				delete this->account;
			Account* account = new Bussiness(id, line_name, line_password, balance, line_type);
			account->cfd = this->mySocket;
			account->selectOption();
		}
		else {
			if (this->account != nullptr)
				delete this->account;
			Account* account = new Consumer(id, line_name, line_password, balance, line_type);
			account->cfd = this->mySocket;
			account->selectOption();
		}
	}
}

void ECManager::showProduct() {
	// 用户
	this->account->showProduct();
}

void ECManager::searchProduct() {
	this->account->searchProduct();
}

void ECManager::exitPlatform() {
	int select;
	cout << "确定要退出吗，亲 O.o" << endl;
	cout << "** 1：继续留在本平台 **" << endl;
	cout << "** 2：忍痛退出 *******" << endl;
	cout << endl;
	cin >> select;
	if (select == 1) {
		system("cls");
	}
	else if (select == 2) {
		cout << "欢迎下次使用" << endl;
		system("pause");
		exit(0);
	}
	else {
		cout << "选择错误，请重新操作" << endl;
		system("cls");
	}
}
