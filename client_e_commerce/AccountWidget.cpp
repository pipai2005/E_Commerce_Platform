#include"AccountWidget.h"

extern map<int, string> productType;


// 显示账户信息
void Account::showAccount() {
	cout << "账户类型：" << getAccountType() << endl;
	cout << "账户名：" << name << endl;
	cout << "密码：" << password << endl;
	cout << "账户余额：" << balance << endl;
	cout << endl;
}

//修改密码
void Account::updatePassword() {
	string new_pwd;
	string line_password;
	cout << "下面开始修改密码" << endl;
	cout << "请输入你的密码" << endl;
	cin >> line_password;
	while (line_password != this->password) {
		cout << "密码与原密码不符，请重新输入" << endl;
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cin >> line_password;
	}

	cout << "请输入新密码" << endl;
	cin >> new_pwd;

	// 打开文件读取数据
	

}

// 显示商品每一页 10
// 定义每页显示的行数
const int ROWS_PER_PAGE = 5;
void Account::displayPage(int pageNumber) {
	// 计算偏移量
	int offset = (pageNumber - 1) * ROWS_PER_PAGE;
	char buffer[BUFSIZ * ROWS_PER_PAGE] = { 0 };
	//接收服务端数据
	this->cfd->receiveData(buffer, sizeof(buffer));
	cout << buffer;
}
// 显示平台商品
void Account::showProduct() {
	cout << "* 客户端在平台上查看商品 " << endl;
	// 模拟翻页操作
	int pageNumber = 1;
	string buffer;
	while (true) {
		cout << "正在请求数据" << endl;
		cout << "==========   Page " << pageNumber << "  ==========" << endl;
		displayPage(pageNumber);
		cout << "====================================" << endl;

		std::cout << "Enter 'n' for next page, 'p' for previous page, 'q' to quit: ";
		char choice;
		cin >> choice;
		while (choice != 'n' && choice != 'p' && choice != 'q') {
			cout << "输入错误，请重新输入" << endl;
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			cin >> choice;
		}
		if (choice == 'n') {
			pageNumber++;
		}
		else if (choice == 'p') {
			if (pageNumber == 1) {
				cout << "上一页没有了,请重新输入" << endl;
			}
			else {
				pageNumber--;
			}
		}
		else if (choice == 'q') {
			break;
		}
		// 发送请求数据
		buffer += to_string(pageNumber);
		this->cfd->sendData(buffer.c_str(), BUFSIZ);
		buffer.clear();
		system("pause");
		system("cls");
	}

}
// 查找商品
void Account::searchProduct() {
	/*暂时只实现用户名查找*/

	/*int select = 0;
	cout << "请选择查询方式" << endl;
	cout << "1: 商品名" << endl;
	cout << "2: 商品类型" << endl;
	cin >> select;
	while (select <= 0 || select >= 3) {
		cout << "输入失败，请重新输入" << endl;
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cin >> select;
	}

	switch (select) {
	case 1:*/
		searchProductByName();
	//	break;
	//case 2:
	//	searchProductByType();
	//}
}

// 余额查询
void Account::getBalance() {
	cout << "您当前的余额：" << this->balance;
}
// 余额充值
void Account::paying() {
	double e;
	cout << "请输入充值额度" << endl;
	cin >> e;
	while (e <= 0) {
		cout << "充值额度输入错误，请重新输入" << endl;
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cin >> e;
	}
	// 改变消费者或商家余额
	
}
// 余额消费
void Account::consuming(double e) {
	double rem = this->balance - e;
	if (rem < 0) {
		cout << "余额不足" << endl;
	}
	else {
		this->balance = rem;
	}
}

// 显示商品信息
void Account::showProductInfoByName() {
	char buffer[BUFSIZ * 10] = { 0 };
	this->cfd->receiveData(buffer, sizeof(buffer));
	if (buffer[0] == '-') {
		cout << "不存在所找商品,请重新操作" << endl;
		return;
	}
	else {
		string data = string(buffer).substr(2);
		cout << "相关商品信息如下：" << endl;
		cout << data << endl;
	}
}
//查找
int Account::searchProductByName() {
	string line_name;
	cout << "请输入您要查找的商品名称" << endl;
	cin >> line_name;
	string buffer(line_name);
	this->cfd->sendData(buffer.c_str(), BUFSIZ);
	buffer.clear();
	this->showProductInfoByName();

	return 1;
}
int Account::searchProductByType() {
	int line_type;
	int num = productType.size();
	for (int i = 1; i <= num; i++) {
		cout << i << ": " << productType[i] << endl;
	}
	cout << "请输入您要查找的商品类型" << endl;
	cin >> line_type;
	while (line_type <= 0 || line_type > num) {
		cout << "选择错误请重新操作" << endl;
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cin >> line_type;
	}
	return 1;
}