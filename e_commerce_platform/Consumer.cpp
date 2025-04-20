#include"Account.h"

Consumer::Consumer(int id, string name, string p, double b, int t) {
	this->id = id;
	this->name = name;
	this->password = p;
	this->balance = b;
	this->type = t;
}

Consumer::~Consumer() {
	cout << "账户类型：" << getAccountType() << endl;
	cout << "账户名：" << name << endl;
	cout << "密码：" << password << endl;
	cout << "账户余额：" << balance << endl;
	cout << endl;
}

string Consumer::getAccountType() {
	return (string)"消费者";
}


// 消费者的界面选项
void Consumer::showAccountOption() {
	cout << "*********************************************" << endl;
	cout << "*************  消费者进行操作  ***************" << endl;
	cout << "*********************************************" << endl;
	cout << "*************** 1 购物车添加 *****************" << endl;
	cout << "*************** 2 购物车删除 *****************" << endl;
	cout << "*************** 3 生成订单 *******************" << endl;
	cout << "*************** 4 修改密码 *******************" << endl;
	cout << "*************** 5 余额查询 *******************" << endl;
	cout << "*************** 6 余额充值 *******************" << endl;
	cout << "*************** 7 用户信息 *******************" << endl;
	cout << "*************** 8 查找商品 *******************" << endl;
	cout << "*************** 0 退出登录 *******************" << endl;
	cout << "*********************************************" << endl;
	cout << endl;
}
void Consumer::selectOption() {
	int select;
	while (1) {
		this->showAccountOption();

		cout << "请输入您的操作" << endl;
		cin >> select;

		switch (select)
		{
		case 0:		// 退出登录
			return;

		case 1:		// 购物车添加
			this->addShopCart();
			break;

		case 2:		// 购物车删除
			this->delShopCart();
			break;

		case 3:		// 生成订单
			this->generateOrder();
			break;

		case 4:		// 修改密码
			this->updatePassword();
			break;

		case 5:
			this->getBalance();
			break;

		case 6:
			this->paying();
			break;

		case 7:
			this->showAccount();
			break;
		
		case 8: 
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

//购物车添加
void Consumer::addShopCart() {
	string line_name;
	cout << "请输入想买商品的全名" << endl;		//暂时不支持模糊选择
	cin >> line_name;

	Database* db = new Database;
	db->getConnect();
	db->op = "select product_id,business_id,product_name, originPrice, discount_rate, product_remain "
		"from products where product_name = '" + line_name + "'";
	db->query();
	unsigned long* len = mysql_fetch_lengths(db->result);
	if (len == 0) {
		cout << "商品库中没有所需商品，请重新操作" << endl;
		return;
	}
	// 进一步选择对应编号的商品
	MYSQL_ROW row = mysql_fetch_row(db->result);

	cout << "请输入您预计买入数量" << endl;
	int num;
	cin >> num;
	// 接下来进行购物车写入操作
	db->op = "insert into shopcart values (";

	delete db;
}
//购物车删除
void Consumer::delShopCart() {

}
//生成订单
void Consumer::generateOrder() {

}

