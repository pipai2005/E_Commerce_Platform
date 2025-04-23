#include"Account.h"

extern map<int, string> productType;

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
	cout << "请输入想买商品的名称" << endl;		//暂时不支持模糊选择
	cin >> line_name;

	Database* db = new Database;
	MYSQL_ROW row;
	MYSQL_RES* result;
	db->getConnect();
	db->op = "select count(*) from products where product_name like '%" + line_name + "%'";
	int res_num = db->getResultNum();
	if (res_num == 0) {
		cout << "不存在所找商品,请重新操作" << endl;
		return;
	}

	// 确认了商品存在，接下来进行查询、展示商品
	db->op = "select * from products where product_name like '%" + line_name + "%'";
	db->query();
	result = mysql_store_result(&db->mysql);
	if (result == NULL) {
		std::cerr << "mysql_store_result() failed: " << mysql_error(&db->mysql) << std::endl;
		return;
	}
	int num_fields = mysql_num_fields(result);  // 获取列数
	MYSQL_FIELD* field = mysql_fetch_field(result);
	// 存储结果集
	vector<Product*> p;
	while (row = mysql_fetch_row(result)) {
		int type = atoi(row[8]);
		switch (type) {
		case 1:
			p.push_back(new Book(atoi(row[0]), atoi(row[1]), row[2], row[3], atof(row[4]), atof(row[5]), atoi(row[6]), atoi(row[7]), type));
			break;
		case 2:
			p.push_back(new Clothes(atoi(row[0]), atoi(row[1]), row[2], row[3], atof(row[4]), atof(row[5]), atoi(row[6]), atoi(row[7]), type));
			break;
		case 3:
			p.push_back(new Electronic(atoi(row[0]), atoi(row[1]), row[2], row[3], atof(row[4]), atof(row[5]), atoi(row[6]), atoi(row[7]), type));
			break;
		case 4:
			p.push_back(new Food(atoi(row[0]), atoi(row[1]), row[2], row[3], atof(row[4]), atof(row[5]), atoi(row[6]), atoi(row[7]), type));
			break;
		default:
			cout << "商品类型错误" << endl;
			break;
		}
	}
	// 遍历结果集
	int id_num = 1; // 商品序号
	for (int i = 1; i <= res_num;i++) {
		cout << "查询商品序号：" << i << endl;
		p[i - 1]->showProductInfo();
	}
	// 进一步选择对应编号的商品
	cout << "请输入想购买列表中的商品序号" << endl;
    int select_id;
    cin >> select_id;
    // 判断选择是否合法
	while (select_id < 0 || select_id >= res_num) {
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cout << "输入商品序号错误，请重新操作" << endl;
		cin>>select_id;
	}	

	Product* selectedProduct = p[select_id - 1];
	cout << "请输入您预计买入数量" << endl;
	int num;
	cin >> num;
	db->op = "insert into shopcart values (" + to_string(this->id) 
			  + "," + to_string(selectedProduct->business_id) + "," + to_string(selectedProduct->product_id)
			  + "," + to_string(num) + ", '" + selectedProduct->name + "', 0)";

	db->query();

	cout << "添加购物车成功！" << endl;
	mysql_free_result(result);
	delete db;
}
//购物车删除
void Consumer::delShopCart() {
	// 购物车删除
	string line_name;
	cout << "请输入想删除的商品的名称" << endl;
	cin >> line_name;

	Database* db = new Database;
	db->getConnect();
	// 先算出查询商品在购物车是否存在
	db->op = "select count(*) from products where product_name like '%" + line_name + "%'";
	int res_num = db->getResultNum();
	if (res_num == 0) {
		cout << "不存在所找商品,请重新操作" << endl;
		return;
	}
	// 确认了商品存在，接下来进行删除商品操作
	db->op = "delete from shopcart where product_name like '%" + line_name + "%'";
	db->query();

	cout << "删除购物车成功！" << endl;

	delete db;
}

void Consumer::updateShopCartProductNum() {
	// 购物车删除
	string line_name;
	cout << "请输入想修改购物车内的商品名称" << endl;
	cin >> line_name;

	Database* db = new Database;
	db->getConnect();
	// 先算出查询商品在购物车是否存在
	db->op = "select count(*) from products where product_name like '%" + line_name + "%'";
	int res_num = db->getResultNum();
	if (res_num == 0) {
		cout << "不存在所找商品,请重新操作" << endl;
		return;
	}
	// 确认了商品存在，接下来进行更新商品操作
	cout << "请输入更改后的买入商品数量" << endl;
	int num;
	cin >> num;
	db->op = "update shopcart set num = " + to_string(num)
		      +" where product_name like '%" + line_name + "%'";
	db->query();

	cout << "更新购物车成功！" << endl;

	delete db;
}

//生成订单
void Consumer::generateOrder() {
	// 购物车删除
	string line_name;
	cout << "请输入您想在购物车中生成订单的商品名称" << endl;
	cin >> line_name;

	Database* db = new Database;
	MYSQL_ROW row;
	MYSQL_RES* result;
	db->getConnect();
	// 先算出查询商品在购物车是否存在
	db->op = "select count(*) from shopcart where product_name like '%" + line_name + "%' and"
		     " consumer_id = " + to_string(this->id);
	        +" isCreateOrder <> 0";

	int res_num = db->getResultNum();
	if (res_num == 0) {
		cout << "不存在所找商品,请重新操作" << endl;
		return;
	}
	// 确认了商品存在（上面是模糊匹配，要选择具体的商品进行订单生成）, 对没有生成订单的商品进行查询
	db->op = "select product_id,business_id,product_name "
		"from shopcart where product_name like '%" + line_name + "%'"
		"and consumer_id = " + to_string(this->id);
	    +" isCreateOrder <> 0";
	db->query();
	result = mysql_store_result(&db->mysql);
	if (result == NULL) {
		std::cerr << "mysql_store_result() failed: " << mysql_error(&db->mysql) << std::endl;
		return;
	}
	int num_fields = mysql_num_fields(result);         // 获取列数
	MYSQL_FIELD* field = mysql_fetch_field(result);    // 获取列名
	// 遍历结果集
	int id_num = 1; // 商品序号
	while (row = mysql_fetch_row(result)) {
		cout << "查询商品序号：" << id_num << endl;
		for (int i = 0; i < num_fields; i++) {
			cout << field[i].name << ": ";
			cout << (row[i] ? row[i] : "NULL") << endl;
		}
		cout << endl;
		id_num++;
	}

	// 进一步选择对应编号的商品
	cout << "请输入想购买列表中的商品序号" << endl;
	int select_id;
	cin >> select_id;
	// 判断选择是否合法
	while (select_id < 0 || select_id >= res_num) {
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cout << "输入商品序号错误，请重新操作" << endl;
		cin >> select_id;
	}
	mysql_free_result(result);
	// 接下来进行生成订单操作
	db->op = "select product_id,business_id,product_name,num "
		"from shopcart where product_name like '%" + line_name + "%'"
		"and consumer_id = " + to_string(this->id);
	    +" isCreateOrder <> 0";
	db->query();
	result = mysql_store_result(&db->mysql);
	for (int i = 1; i <= select_id; i++) {  // 跳过前面几行
		row = mysql_fetch_row(result);
	}
	assert(row != NULL);
	int product_id = atoi(row[0]);
	int business_id = atoi(row[1]);
	string product_name = row[2];
	int freeze_num = atoi(row[3]);
	// 生成订单的条件，商家现有商品数大于等于购物车内商品数

	// 1. 将购物车内的商品状态改为已生成订单
	db->op = "update shopcart set isCreateOrder = 1"
		" where product_id = " + to_string(product_id)
		+ " and consumer_id = " + to_string(this->id);
	db->query();
	// 2. 改变商家冻结商品数和可卖商品数
	db->op = "update products set product_remain = product_remain - "
		+ to_string(freeze_num) + ", freeze_num = freeze_num +" + to_string(freeze_num)
		+ " where product_id = " + to_string(product_id);
	db->query();

	cout << "生成订单成功！" << endl;

	delete db;
}

