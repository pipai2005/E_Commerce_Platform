#include"e_commerce.h"

using namespace std;
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
	// 显示选项在客户端实现
	/*cout << "*********************************************" << endl;
	cout << "*************  电子电商平台  *****************" << endl;
	cout << "*********************************************" << endl;
	cout << "*************** 1 账户注册 *******************" << endl;
	cout << "*************** 2 账户登录 *******************" << endl;
	cout << "*************** 3 查看商品 *******************" << endl;
	cout << "*************** 4 查找商品 *******************" << endl;
	cout << "*************** 0 退出平台 *******************" << endl;
	cout << "*********************************************" << endl;
	cout << endl;*/
	cout << "** 已在客户端显示平台选择界面" << endl;
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

void ECManager::registerAccount() {
	string namee;
	string passwordd;
	int typee;

	// 从客户端获取输入（数据格式：操作 类型 名称 密码）
	char buffer[BUFSIZ]= {0};
	this->mySocket->receiveData(buffer, BUFSIZ); 
	string request = buffer;
	typee = request[2] - '0';
	namee = request.substr(4, request.rfind(' ') - 4);
	passwordd = request.substr(request.rfind(' ') + 1);
	memset(buffer, 0, BUFSIZ);

	// 将注册的信息发送给用户（-1 / 0 : 用户名重复 / 注册成功）
	// 防止用户名重复
	for (vector<Account*>::iterator it = accounts.begin();it != accounts.end();it++) {
		if ((*it)->name == namee) {
			this->mySocket->sendData("-1", BUFSIZ);
			cout << "已拒绝用户注册" << endl;
			return;
		}
	}

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

	this->mySocket->sendData("0", BUFSIZ);
	cout << "客户端用户注册成功" << endl;
}

void ECManager::loginAccount(){
	string line_name;			// 输入行的账户名
	string line_password;		// 输入行的密码
	int line_type;				// 输入行的账户类型

	// 从客户端获取输入（数据格式：操作 类型 名称 密码）
	char buffer[BUFSIZ] = { 0 };
	this->mySocket->receiveData(buffer, BUFSIZ);
	string request = buffer;
	line_type = request[2] - '0';
	line_name = request.substr(4, request.rfind(' ') - 4);
	line_password = request.substr(request.rfind(' ') + 1);
	memset(buffer, 0, BUFSIZ);

	// 发送给客户端（-1 / 0 / 1 ): 用户名或密码错误 / 平台无账户信息 / 登录成功）
	// 下面匹配账户名和密码
	int isStore = 0;
	Account* ac;
	for (vector<Account*>::iterator it = accounts.begin();it != accounts.end();it++) {
		if ((*it)->name == line_name) {
			isStore = 1;
			ac = (*it);
			if ((*it)->password == line_password && (*it)->type== line_type) {
				// 登录成功, 发送给客户端,同时初始化客户端账户信息
				sprintf(buffer, "1 %d %f", ac->id, ac->balance);
				this->mySocket->sendData(buffer, BUFSIZ);
				cout << "** 客户端登录成功" << endl;
				ac->cfd = this->mySocket;
				ac->selectOption();
			}
			else {
				this->mySocket->sendData("-1", BUFSIZ);
				cout << "客户端登录失败" << endl;
			}
			break;
		}
	}
	if (isStore == 0) {
		this->mySocket->sendData("0", BUFSIZ);
		cout << "客户端登录失败" << endl;
		return;
	}
	
}

// 显示商品每一页 10
// 定义每页显示的行数
const int ROWS_PER_PAGE = 5;
void ECManager::displayPage(Database* db, int pageNumber) {
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
	string buffer;
	while (row = mysql_fetch_row(result)) {
		for (int i = 0; i < num_fields; i++) {
			buffer += string(field[i].name) + ": ";
			if (field[i].name == "type")
				buffer += string(productType[stoi(row[i])]);
			else
				buffer += string((row[i] ? row[i] : " "));
			buffer += "\n";
		}
		buffer += "\n";
	}
	this->mySocket->sendData(buffer.c_str(), buffer.size());
}
// 显示平台商品
void ECManager::showProduct() {
	Database* db = new Database;
	db->getConnect();

	// 模拟翻页操作
	int pageNumber = 1;
	while (true) {
		displayPage(db, pageNumber);

		// Enter 'n' for next page, 'p' for previous page, 'q' to quit
		char buffer[BUFSIZ] = { 0 };
		this->mySocket->receiveData(buffer, BUFSIZ);
		string request = buffer;
		pageNumber = stoi(request);
		memset(buffer, 0, BUFSIZ);
	}
}
// 查找商品
void ECManager::searchProduct() {
	this->searchProductByName();
}
void ECManager::showProductInfoByName(Database* db, string line_name) {
	MYSQL_ROW row;
	MYSQL_RES* result;
	db->op = "select product_name, originPrice, discount_rate, product_remain, type, description "
		"from products where product_name like '%" + line_name + "%'";
	db->query();
	result = mysql_store_result(&db->mysql);

	int num_fields = mysql_num_fields(result);  // 获取列数

	MYSQL_FIELD* field = mysql_fetch_field(result);

	//将结果传送给客户端
	string buffer("0 ");//查找成功标志0
	while (row = mysql_fetch_row(result)) {
		for (int i = 0; i < num_fields; i++) {
			buffer += string(field[i].name) + ": ";
			if (field[i].name == "type")
				buffer += string(productType[stoi(row[i])]);
			else
				buffer += string((row[i] ? row[i] : " "));
			buffer += "\n";
		}
		buffer += "\n";
	}
	this->mySocket->sendData(buffer.c_str(), buffer.size());
	mysql_free_result(result);
}
//查找
int ECManager::searchProductByName() {
	cout << "* 客户端进行商品查找操作" << endl;
	// 传送数据 (-1 / 0) : (商品库内无商品 / 查找成功并返回数据)
	char buffer[BUFSIZ] = { 0 };
	this->mySocket->receiveData(buffer, BUFSIZ);
	string request = buffer;
	string line_name = request;

	Database* db = new Database;

	db->getConnect();
	db->op = "select count(*) from products where product_name like '%" + line_name + "%'";
	int res_num = db->getResultNum();
	if (res_num == 0) {
		//cout << "不存在所找商品,请重新操作" << endl;
		this->mySocket->sendData("-1", BUFSIZ);
		return 0;
	}

	// 确认了商品存在，接下来进行查询、展示商品
	this->showProductInfoByName(db, line_name);

	delete db;
	return 1;
}

void ECManager::exitPlatform() {
	cout << "客户端退出操作" << endl;
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
