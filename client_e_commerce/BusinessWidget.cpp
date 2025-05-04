#include"AccountWidget.h"

extern map<int, string> productType;

Bussiness::Bussiness() {

}

Bussiness::Bussiness(int id, string name, string p, double b, int t) {
	this->id = id;
	this->name = name;
	this->password = p;
	this->balance = b;
	this->type = t;
}

Bussiness::~Bussiness() {

}

string Bussiness::getAccountType() {
	return (string)"商家";
}


// 商家界面选项
void Bussiness::showAccountOption() {
	cout << "*********************************************" << endl;
	cout << "**************  商家进行操作  ****************" << endl;
	cout << "*********************************************" << endl;
	cout << "*************** 1 商品打折 *******************" << endl;
	cout << "*************** 2 商品添加 *******************" << endl;
	cout << "*************** 3 查看库存 *******************" << endl;
	cout << "*************** 4 修改密码 *******************" << endl;
	cout << "*************** 5 余额查询 *******************" << endl;
	cout << "*************** 6 余额充值 *******************" << endl;
	cout << "*************** 7 用户信息 *******************" << endl;
	cout << "*************** 8 查找商品 *******************" << endl;
	cout << "*************** 0 退出登录 *******************" << endl;
	cout << "*********************************************" << endl;
	cout << endl;
}
void Bussiness::selectOption() {
	int select;
	while (1) {
		this->showAccountOption();

		cout << "请输入您的操作" << endl;
		cin >> select;

		switch (select)
		{
		case 0:		// 退出登录
			cout << "恭喜商家退出登录成功" << endl;
			return;

		case 1:		// 打折
			this->discount();
			break;

		case 2:		// 添加商品
			this->addProductStock();
			break;

		case 3:		// 查看库存
			this->showStock();
			break;

		case 4:		// 修改密码
			this->updatePassword();
			break;

		case 5:		// 余额查询
			this->getBalance();
			break;

		case 6:		// 余额充值
			this->paying();
			break;

		case 7:		// 显示账户信息
			this->showAccount();
			break;

		case 8: 	// 查找商品
			this->searchProduct();
			break;
		default:
			system("cls");
			break;
		}

		system("pause");
		system("cls");
	}
}

// 商品打折
void Bussiness::discount() {
	
}

// 添加商品入库
void Bussiness::addProductStock() {

	int line_type;
	int line_purchaseNum = 0;
	string line_name;
	string line_desc;
	double line_price = 0;

	cout << "请输入您要添加的商品名称" << endl;
	cin >> line_name;

	cout << "请选择商品类型" << endl;
	int num = productType.size();
	for (int i = 1; i <= num; i++) {
		cout << i << ": " << productType[i] << endl;
	}
	cin >> line_type;
	while (line_type <= 0 || line_type > num) {
		cout << "选择错误请重新操作" << endl;
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cin >> line_type;
	}
	cout << "请输入对商品的描述" << endl;
	cin >> line_desc;
	cout << "请安排商品的价格" << endl;
	cin >> line_price;
	cout << "请确定进货量" << endl;
	cin >> line_purchaseNum;

	

	cout << "商品添加成功" << endl;

}

// 查看库存
void Bussiness::showStock() {
	
}