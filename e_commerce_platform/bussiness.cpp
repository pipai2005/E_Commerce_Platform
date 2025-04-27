#include"Account.h"

extern map<int, string> productType;

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
	Database* db = new Database;
	db->getConnect();

	cout << "请输入需要改成的折扣率" << endl;
	double rate;
	cin >> rate;

	db->op = "update products set discount_rate = " + to_string(rate);
	db->query();
	
	cout << "商品折扣率更改成功" << endl;
	delete db;
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
	for (int i = 1;i <= num;i++) {
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

	// 将商品信息加入数据库表格products
	Database* db = new Database;
	db->getConnect();
	// eg. insert into products values (0,1,'name','desc',0.55,0.25,10,3);

	db->op = "insert into products values ( 0, " 
		+ to_string(this->id) + ",'" + line_name + "','" + line_desc + "'," + to_string(line_price)
		+ ", 1.0," + to_string(line_purchaseNum) + ",0 ," + to_string(line_type) + ")";

	//db->op = "insert into products values(0, 1, 'abc', '23333', 3.000000, 1, 10, 4)";  // 这是可以正常输入的

	db->query();

	cout << "商品添加成功" << endl;
	delete db;
}

// 查看库存
void Bussiness::showStock() {
	Database* db = new Database;
	
	db->getConnect();
	db->op = "select product_name, originPrice, discount_rate, product_remain, type, description "
			  "from products where business_id = " + to_string(this->id);
	db->query();
	MYSQL_RES* result = mysql_store_result(&db->mysql);
	if (result == NULL) {
		std::cerr << "mysql_store_result() failed: " << mysql_error(&db->mysql) << std::endl;
		return;
	}
	int num_fields = mysql_num_fields(result);  // 获取列数
	
	MYSQL_FIELD* field = mysql_fetch_field(result);
	MYSQL_ROW row;
	// 遍历结果集
	int index = 1;
	while (row = mysql_fetch_row(result)) {
		cout << "商品序号：" << index++ << endl;
		for (int i = 0; i < num_fields; i++) {
			cout << field[i].name << ": ";
			if (field[i].name == "type")
				cout << productType[stoi(row[i])] << endl;
			else
				cout<< (row[i] ? row[i] : " ") << endl;
		}
		cout << endl;
	}

	cout << endl << "数据查询结束" << endl;

	mysql_free_result(result);
	delete db;
}