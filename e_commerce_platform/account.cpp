#include"Account.h"

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
	ifstream ifs;
	ifs.open(ACCOUNT_FILENAME, ios::in);

	vector<Account*> accounts;
	int a1, a2, a3, a;

	// 如果文件中有账户信息
	if (ifs.is_open()) {
		ifs >> a1;
		ifs >> a2;
		ifs >> a3;
		ifs >> a;

		for (int i = 0;i < a;i++) {
				int tmp_id;
				string tmp_name;
				string tmp_password;
				double tmp_balance;
				int tmp_type;

				Account* ac;
				ifs >> tmp_id >> tmp_name >> tmp_password >> tmp_balance >> tmp_type;
				if (tmp_type == 1) {
					ac = new Bussiness(tmp_id, tmp_name, tmp_password, tmp_balance, tmp_type);

				}
				else {
					ac = new Consumer(tmp_id, tmp_name, tmp_password, tmp_balance, tmp_type);
				}
				accounts.push_back(ac);
			}
		ifs.close();

		for (vector<Account*>::iterator it = accounts.begin();it != accounts.end();it++) {
			string t_name = (*it)->name;
			if (t_name == this->name) {
				this->password = new_pwd;
				(*it)->password = new_pwd;
				break;
			}
		}

		ofstream ofs;
		ofs.open(ACCOUNT_FILENAME, ios::out);
		ofs << a1 << " " << a2 << " " << a3 << " " << a << endl;

		for (vector<Account*>::iterator it = accounts.begin();it != accounts.end();it++) {
			Account* acc = *it;
			ofs << acc->id << " " << acc->name << " " << acc->password << " " << acc->balance << " " << acc->type << endl;
		}
		ofs.close();

		// 下面是更改数据库内的商家信息
		Database* db = new Database;
		db->getConnect();
		db->op = "update accounts set password = " + new_pwd + " where account_name = " 
			      + "'" + this->name + "'";
		cout << db->op;
		system("pause");

		db->query();
		delete db;

		cout << "密码修改成功！" << endl;
	}
	else {
			cout << "文件打开失败，请重新操作" << endl;
		}


}

// 显示商品每一页 10
// 定义每页显示的行数
const int ROWS_PER_PAGE = 5;
void Account::displayPage(Database* db, int pageNumber) {
	// 计算偏移量
	int offset = (pageNumber - 1) * ROWS_PER_PAGE;

	// 构建包含 LIMIT 和 OFFSET 的 SQL 查询语句
	db->op = "select product_name, originPrice, discount_rate, product_remain, type, description "
		"from products LIMIT " + std::to_string(ROWS_PER_PAGE)
		+ " OFFSET " + std::to_string(offset);

	// 执行 SQL 查询
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
	while (row = mysql_fetch_row(result)) {
		for (int i = 0; i < num_fields; i++) {
			cout << field[i].name << ": ";
			if (field[i].name == "type")
				cout << productType[stoi(row[i])] << endl;
			else
				cout << (row[i] ? row[i] : " ") << endl;
		}
		cout << endl;
	}
}
// 显示平台商品
void Account::showProduct() {
	Database* db = new Database;
	db->getConnect();

	// 模拟翻页操作
	int pageNumber = 1;
	while (true) {
		std::cout << "Page " << pageNumber << ":" << std::endl;
		displayPage(db, pageNumber);

		std::cout << "Enter 'n' for next page, 'p' for previous page, 'q' to quit: ";
		char choice;
		std::cin >> choice;

		if (choice == 'n') {
			pageNumber++;
		}
		else if (choice == 'p') {
			if (pageNumber == 1) {
				cout << "上一页没有了,请重新输入" << endl;
			}
			pageNumber--;
		}
		else if (choice == 'q') {
			break;
		}
		system("cls");
	}
}

// 查找商品
void Account::searchProduct() {
	int select = 0;
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
	case 1: 
		searchProductByName();
		break;
	case 2:
		searchProductByType();
	}
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
	ifstream ifs;
	ifs.open(ACCOUNT_FILENAME, ios::in);
	vector<Account*> accounts;
	// 如果文件中有账户信息
	int product_num;
	int bussiness_num;
	int consumer_num;
	int account_num;
	if (ifs.is_open()) {

		ifs >> product_num;
		ifs >> bussiness_num;
		ifs >> consumer_num;
		ifs >> account_num;

		for (int i = 0;i < account_num;i++) {
			int tmp_id;
			string tmp_name;
			string tmp_password;
			double tmp_balance;
			int tmp_type;

			Account* ac;
			ifs >> tmp_id >> tmp_name >> tmp_password >> tmp_balance >> tmp_type;
			if (tmp_type == 1) {
				ac = new Bussiness(tmp_id, tmp_name, tmp_password, tmp_balance, tmp_type);

			}
			else {
				ac = new Consumer(tmp_id, tmp_name, tmp_password, tmp_balance, tmp_type);
			}
			accounts.push_back(ac);

		}
		ifs.close();
	}
	// 先找到商家
	int id = this->id;
	accounts[id - 1]->balance += e;
	this->balance = accounts[id - 1]->balance;
	// 接下来存用户信息(文件操作)
	ofstream ofs;
	ofs.open(ACCOUNT_FILENAME, ios::out);
	ofs << product_num << " " << bussiness_num << " "
		<< consumer_num << " " << account_num << endl;
	for (vector<Account*>::iterator it = accounts.begin();it != accounts.end();it++) {
		Account* acc = *it;
		ofs << acc->id << " " << acc->name << " " << acc->password << " " << acc->balance << " " << acc->type << endl;
	}
	ofs.close();

	cout << "余额充值成功！当前余额数：" << accounts[id-1]->balance <<endl;
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
void Account::showProductInfoByName(Database* db, string line_name) {
	MYSQL_ROW row;
	MYSQL_RES* result;
	db->op = "select product_name, originPrice, discount_rate, product_remain, type, description "
		"from products where product_name like '%" + line_name + "%'";
	db->query();
	result = mysql_store_result(&db->mysql);

	int num_fields = mysql_num_fields(result);  // 获取列数

	MYSQL_FIELD* field = mysql_fetch_field(result);

	// 遍历结果集
	while (row = mysql_fetch_row(result)) {
		for (int i = 0; i < num_fields; i++) {
			cout << field[i].name << ": ";
			if (field[i].name == "type")
				cout << productType[stoi(row[i])] << endl;
			else
				cout << (row[i] ? row[i] : " ") << endl;
		}
		cout << endl;
	}

	mysql_free_result(result);
}
//查找
int Account::searchProductByName() {
	string line_name;
	cout << "请输入您要查找的商品名称" << endl;
	cin >> line_name;

	Database* db = new Database;

	db->getConnect();
	db->op = "select count(*) from products where product_name like '%" + line_name + "%'";
	int res_num = db->getResultNum();
	if (res_num == 0) {
		cout << "不存在所找商品,请重新操作" << endl;
		return 0;
	}

	// 确认了商品存在，接下来进行查询、展示商品
	this->showProductInfoByName(db, line_name);

	delete db;
	return 1;
}
int Account::searchProductByType() {
	int line_type;
	cout << "请输入您要查找的商品类型" << endl;
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

	Database* db = new Database;
	db->getConnect();
	db->op = "select product_name, originPrice, discount_rate, product_remain, type, description "
		"from products where type = " + to_string(line_type);
	db->query();
	MYSQL_RES* result = mysql_store_result(&db->mysql);
	if (result == NULL) {
		std::cerr << "mysql_store_result() failed: " << mysql_error(&db->mysql) << std::endl;
		return 0;
	}
	int num_fields = mysql_num_fields(result);  // 获取列数

	MYSQL_FIELD* field = mysql_fetch_field(result);
	MYSQL_ROW row;

	// 遍历结果集
	while (row = mysql_fetch_row(result)) {
		for (int i = 0; i < num_fields; i++) {
			cout << field[i].name << ": ";
			if (field[i].name == "type")
				cout << productType[stoi(row[i])] << endl;
			else
				cout << (row[i] ? row[i] : " ") << endl;
		}
		cout << endl;
	}

	mysql_free_result(result);
	delete db;
	return 1;
}