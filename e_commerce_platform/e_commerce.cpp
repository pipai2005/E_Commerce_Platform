#include"e_commerce.h"


// 商品类型枚举
map<int, string> productType{
	{1, "books"},
	{2, "clothes"},
	{3, "electronic"},
	{4, "food"}
};

ECManager::ECManager() {

}

ECManager::~ECManager() {
	
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

void ECManager::initAccount()                                                                                                                                                                  {

	// 打开文件读取数据
	ifstream ifs;
	ifs.open(ACCOUNT_FILENAME, ios::in);

	// 如果文件中有账户信息
	if (ifs.is_open()) {
		ifs >> this->product_num;
		ifs >> this->bussiness_num;
		ifs >> this->consumer_num;
		ifs >> this->account_num;

		for (int i = 0;i < this->account_num;i++) {
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
	}
	else {
		cout << "平台内暂时无账户信息，请先注册" << endl;
	}
	ifs.close();
}

bool ECManager::searchAccount(const Account* ac) const {
	return true;
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

	// 防止用户名重复
	for (vector<Account*>::iterator it = accounts.begin();it != accounts.end();it++) {
		if ((*it)->name == namee) {
			cout << "用户名重复，换一个试试呢" << endl;
			return;
		}
	}

	if (typee != 1 && typee != 2) {
		cout << "账户类型选择错误，请重新操作" << endl;
		system("pause");
		system("cls");
	}
	else {
		Account* ac;
		if (typee == 1) {
			ac = new Bussiness(account_num + 1, namee, passwordd, 0, typee);
			bussiness_num++;
			account_num++;
		}
		else {
			ac = new Consumer(account_num + 1, namee, passwordd, 0, typee);
			consumer_num++;
			account_num++;
		}

		this->accounts.push_back(ac);

		// 数据库存入
		this->addAccountDatabase(*ac);

		// 接下来存用户信息
		ofstream ofs;
		ofs.open(ACCOUNT_FILENAME, ios::out);
		ofs << this->product_num << " " << this->bussiness_num << " " 
			<< this->consumer_num << " " << this->account_num << endl;

		for (vector<Account*>::iterator it = accounts.begin();it != accounts.end();it++) {
			Account* acc = *it;
			ofs << acc->id << " " << acc->name << " " << acc->password << " " << acc->balance << " " << acc->type << endl;
		}
		ofs.close();

	}

}

void ECManager::loginAccount(){
	string line_name;				// 输入行的账户名
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

	// 下面匹配账户名和密码
	int isStore = 0;
	Account* ac;
	for (vector<Account*>::iterator it = accounts.begin();it != accounts.end();it++) {
		if ((*it)->name == line_name) {
			isStore = 1;
			ac = (*it);
			if ((*it)->password == line_password) {
				cout << "登录成功！" << endl;
				cout << endl;
				system("pause");
				system("cls");
				ac->selectOption();
			}
			else {
				cout << "密码错误，登录失败，请重新操作" << endl;
				cout << endl;
			}
			break;
		}
	}
	if (isStore == 0) {
		cout << "平台内暂无账户信息，请先注册" << endl;
		return;
	}
	
}

void ECManager::showProduct() {
	// 保底有一个用户
	this->accounts[0]->showProduct();
}

void ECManager::searchProduct() {
	this->accounts[0]->searchProduct();
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

// 将商家信息添加进入database
void ECManager::addAccountDatabase(const Account& ac) {
	Database* db = new Database;
	db->getConnect();

	string sid = to_string(ac.id);

	db->op = "insert into accounts values (" 
		+ sid + ",'" + ac.name + "','" + ac.password + "')";
	db->query();

	delete db;
}
