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
	/*cout << "*********************************************" << endl;
	cout << "*************  消费者进行操作  ***************" << endl;
	cout << "*********************************************" << endl;
	cout << "*************** 1 购物车添加 *****************" << endl;
	cout << "*************** 2 购物车删除 *****************" << endl;
	cout << "*************** 3 购物车修改 *****************" << endl;
	cout << "*************** 4 生成订单 *******************" << endl;
	cout << "*************** 5 修改密码 *******************" << endl;
	cout << "*************** 6 余额查询 *******************" << endl;
	cout << "*************** 7 余额充值 *******************" << endl;
	cout << "*************** 8 用户信息 *******************" << endl;
	cout << "*************** 9 查找商品 *******************" << endl;
	cout << "*************** 0 退出登录 *******************" << endl;
	cout << "*********************************************" << endl;
	cout << endl;*/
}
void Consumer::selectOption() {
	int select;
	while (1) {
		this->showAccountOption();

		char buffer[BUFSIZ] = { 0 };
		this->cfd->receiveData(buffer, BUFSIZ);
		select = buffer[0] - '0';

		switch (select)
		{
		case 0:		// 退出登录
			cout << "恭喜消费者退出登录成功" << endl;
			return;

		case 1:		// 购物车添加
			this->addShopCart();
			break;

		case 2:		// 购物车删除
			this->delShopCart();
			break;

		case 3:
			this->updateShopCartProductNum();
			break;

		case 4:		// 生成订单
			this->generateOrder();
			break;

		case 5:		// 修改密码
			this->updatePassword();
			break;

		case 6:		// 余额查询
			this->getBalance();
			break;

		case 7:		// 余额充值
			this->paying();
			break;

		case 8:		// 显示账户信息
			this->showAccount();
			break;
		
		case 9:		// 查找商品
			this->searchProduct();
			break;

		default:
			break;
		}
		cout << endl;

	}
}

//购物车添加
void Consumer::addShopCart() {
	cout << "客户端购物车添加商品操作" << endl;
	// 传送给客户端数据 （0 / 1）: (商家库存太少 / 成功)

	char buffer[BUFSIZ] = { 0 };
	this->cfd->receiveData(buffer, BUFSIZ);
	string line_name = buffer;
	memset(buffer, 0, BUFSIZ);

	Database* db = new Database;
	MYSQL_ROW row;
	MYSQL_RES* result;
	db->getConnect();
	db->op = "select count(*) from products where product_name like '%" + line_name + "%'";
	int res_num = db->getResultNum();
	if (res_num == 0) {
		//cout << "不存在所找商品,请重新操作" << endl;
		this->cfd->sendData("-1", BUFSIZ);
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
	// 发送给客户端遍历结果集
	// 1. 发送给客户端商品数量
	string sendBuffer = to_string(res_num);
	this->cfd->sendData(sendBuffer.c_str(), sendBuffer.size());
	// 2. 遍历结果集
	int id_num = 1; // 商品序号
	for (int i = 1; i <= res_num;i++) {
		p[i - 1]->showProductInfo(this->cfd, 1);
	}

	// 接收从客户端传送的 选择的 对应编号的商品
	this->cfd->receiveData(buffer, BUFSIZ);
	int select_id = atoi(buffer);
	memset(buffer, 0, BUFSIZ);
   
	Product* selectedProduct = p[select_id - 1];
	// 从客户端接收预计买入数量
	this->cfd->receiveData(buffer, BUFSIZ);
	int num = atoi(buffer);
	memset(buffer,0,BUFSIZ);
	
	// 1. 判断商家现有商品数大于等于购物车内商品数
	db->op = "select product_remain from products where product_id = "
			+ to_string(selectedProduct->product_id);
	db->query();
	result = mysql_store_result(&db->mysql);
	if (result == NULL) {
		std::cerr << "mysql_store_result() failed: " << mysql_error(&db->mysql) << std::endl;
		return;
	}
	row = mysql_fetch_row(result);
	int product_remain = atoi(row[0]);
	if (product_remain < num) {
		/*cout << "商家现有商品数：" << product_remain << endl;
		cout << "商品数不足，无法加入购物车" << endl;*/
		this->cfd->sendData("0", BUFSIZ);
		return;
	}
	// 2. 添加购物车
	db->op = "insert into shopcart values (0, " + to_string(this->id)
		+ "," + to_string(selectedProduct->business_id) + "," + to_string(selectedProduct->product_id)
		+ "," + to_string(num) + ", '" + selectedProduct->name + "', 0,"
		+ to_string(selectedProduct->type) + ")";
	db->query();
	// 3. 改变商家冻结商品数和可卖商品数
	db->op = "update products set product_remain = product_remain - "
		+ to_string(num) + ", freeze_num = freeze_num +"
		+ to_string(num)
		+ " where product_id = " + to_string(selectedProduct->product_id);
	db->query();

	this->cfd->sendData("1", BUFSIZ);
	mysql_free_result(result);
	delete db;
}
//购物车删除
void Consumer::delShopCart() {
	// 购物车删除
	cout << "* 客户端购物车删除商品操作" << endl;

	char buffer[BUFSIZ] = { 0 };
	this->cfd->receiveData(buffer, BUFSIZ);
	string line_name;
	line_name = buffer;
	memset(buffer, 0, BUFSIZ);

	Database* db = new Database;
	db->getConnect();
	// 先算出查询商品在购物车是否存在
	db->op = "select count(*) from products where product_name like '%" + line_name + "%'";
	int res_num = db->getResultNum();
	if (res_num == 0) {
		this->cfd->sendData("-1", BUFSIZ);
		return;
	}
	// 确认了商品存在，接下来进行删除商品操作
	db->op = "delete from shopcart where product_name like '%" + line_name + "%'";
	db->query();

	this->cfd->sendData("1", BUFSIZ);
	cout << "客户端删除购物车成功！" << endl;

	delete db;
}

void Consumer::updateShopCartProductNum() {
	cout << "* 客户端修改购物车内商品买入数量" << endl;

	char buffer[BUFSIZ] = { 0 };
	string sendBuffer;
	string line_name;
	this->cfd->receiveData(buffer, BUFSIZ);
	line_name = buffer;
	memset(buffer, 0, BUFSIZ);

	Database* db = new Database;
	MYSQL_RES* result;
	MYSQL_ROW row;
	db->getConnect();
	// 先算出查询商品在购物车是否存在
	db->op = "select count(*) from shopcart where product_name like '%" + line_name + "%'";
	int res_num = db->getResultNum();
	if (res_num == 0) {
		this->cfd->sendData("-1", BUFSIZ);
		return;
	}
	// 确认了商品存在（上面是模糊匹配，要选择具体的商品进行订单生成）, 对没有生成订单的商品进行查询
	db->op = "select shopcart_id,product_id,business_id,product_name,num,type "
		" from shopcart where product_name like '%" + line_name + "%' "
		" and consumer_id = " + to_string(this->id)
	  + " and isCreateOrder = 0";
	db->query();
	result = mysql_store_result(&db->mysql);
	if (result == NULL) {
		std::cerr << "mysql_store_result() failed: " << mysql_error(&db->mysql) << std::endl;
		return;
	}
	// 存储结果集
	vector<Product*> p;
	while (row = mysql_fetch_row(result)) {
		int type = atoi(row[5]); // 规定type在第6列
		switch (type) {		// shopcart_id暂时存在product->product_remain中
		case 1:
			p.push_back(new Book(atoi(row[1]), atoi(row[2]), row[3], "", 1, 1, atoi(row[0]), atoi(row[4]), type));
			break;
		case 2:
			p.push_back(new Clothes(atoi(row[1]), atoi(row[2]), row[3], "", 1, 1, atoi(row[0]), atoi(row[4]), type));
			break;
		case 3:
			p.push_back(new Electronic(atoi(row[1]), atoi(row[2]), row[3], "", 1, 1, atoi(row[0]), atoi(row[4]), type));
			break;
		case 4:
			p.push_back(new Food(atoi(row[1]), atoi(row[2]), row[3], "", 1, 1, atoi(row[0]), atoi(row[4]), type));
			break;
		default:
			cout << "商品类型错误" << endl;
			break;
		}
	}
	// 发送给客户端遍历结果集
	// 1. 发送给客户端商品数量
	sendBuffer = to_string(res_num);
	this->cfd->sendData(sendBuffer.c_str(), sendBuffer.size());
	// 2. 遍历结果集
	for (int i = 1; i <= res_num;i++) {
		p[i - 1]->showProductInfo(this->cfd, 1);
	}

	// 接收从客户端传送的 选择的 对应编号的商品
	this->cfd->receiveData(buffer, BUFSIZ);
	int select_id = atoi(buffer);
	memset(buffer, 0, BUFSIZ);
	mysql_free_result(result);
	Product* selectedProduct = p[select_id - 1];

	// 确认了商品存在，接下来进行更新商品操作
	// 1. 从客户端接收预计买入数量
	this->cfd->receiveData(buffer, BUFSIZ);
	int num = atoi(buffer);
	memset(buffer, 0, BUFSIZ);
	// 2. 判断商家现有商品数大于等于购物车内商品数
	db->op = "select product_remain from products where product_id = "
		+ to_string(selectedProduct->product_id);
	db->query();
	result = mysql_store_result(&db->mysql);
	if (result == NULL) {
		std::cerr << "mysql_store_result() failed: " << mysql_error(&db->mysql) << std::endl;
		return;
	}
	row = mysql_fetch_row(result);
	int product_remain = atoi(row[0]);
	if (product_remain < num) {
		/*cout << "商家现有商品数：" << product_remain << endl;
		cout << "商品数不足，无法加入购物车" << endl;*/
		this->cfd->sendData("0", BUFSIZ);
		return;
	}
	// 3. 改变购物车内商品数
	db->op = "update shopcart set num = " + to_string(num)
		+ " where shopcart_id = " + to_string(selectedProduct->product_remain);
	db->query();
	// 4. 改变商家冻结商品数和可卖商品数
	int dif_num = num - selectedProduct->freeze_num;
	db->op = "update products set product_remain = product_remain - "
		+ to_string(dif_num) + ", freeze_num = freeze_num +"
		+ to_string(dif_num)
		+ " where product_id = " + to_string(selectedProduct->product_id);
	db->query();

	this->cfd->sendData("1", BUFSIZ);
	cout << "客户端更新购物车成功！" << endl;

	delete db;
}

//生成订单
void Consumer::generateOrder() {
	cout << "* 客户端进行生成订单操作" << endl;
	// 返回给客户端数据 （-1 / 0 / 1）: (商品名未找到 / 余额不足 / 成功)

	char buffer[BUFSIZ] = { 0 };
	string sendBuffer;
	string line_name;
	this->cfd->receiveData(buffer, BUFSIZ);
	line_name = buffer;
	memset(buffer, 0, BUFSIZ);

	Database* db = new Database;
	MYSQL_ROW row;
	MYSQL_RES* result;
	db->getConnect();
	// 先算出查询商品在购物车是否存在
	db->op = "select count(*) from shopcart where product_name like '%" + line_name + "%' and"
		     " consumer_id = " + to_string(this->id)
	       + " and isCreateOrder = 0";

	int res_num = db->getResultNum();
	if (res_num == 0) {
		this->cfd->sendData("-1", BUFSIZ);
		return;
	}
	// 确认了商品存在（上面是模糊匹配，要选择具体的商品进行订单生成）, 对没有生成订单的商品进行查询
	db->op = "select shopcart_id,product_id,business_id,product_name,num,type "
		" from shopcart where product_name like '%" + line_name + "%'"
		" and consumer_id = " + to_string(this->id)
	  + " and isCreateOrder = 0";
	db->query();
	result = mysql_store_result(&db->mysql);
	if (result == NULL) {
		std::cerr << "mysql_store_result() failed: " << mysql_error(&db->mysql) << std::endl;
		return;
	}
	// 存储结果集
	vector<Product*> p;
	while (row = mysql_fetch_row(result)) {
		int type = atoi(row[5]); // 规定type在第6列
		switch (type) {		// shopcart_id暂时存在product->product_remain中
		case 1:
			p.push_back(new Book(atoi(row[1]), atoi(row[2]), row[3], "", 1, 1, atoi(row[0]), atoi(row[4]), type));
			break;
		case 2:
			p.push_back(new Clothes(atoi(row[1]), atoi(row[2]), row[3], "", 1, 1, atoi(row[0]), atoi(row[4]), type));
			break;
		case 3:
			p.push_back(new Electronic(atoi(row[1]), atoi(row[2]), row[3], "", 1, 1, atoi(row[0]), atoi(row[4]), type));
			break;
		case 4:
			p.push_back(new Food(atoi(row[1]), atoi(row[2]), row[3], "", 1, 1, atoi(row[0]), atoi(row[4]), type));
			break;
		default:
			cout << "商品类型错误" << endl;
			break;
		}
	}
	// 发送给客户端遍历结果集
	// 1. 发送给客户端商品数量
	sendBuffer = to_string(res_num);
	this->cfd->sendData(sendBuffer.c_str(), sendBuffer.size());
	// 2. 遍历结果集
	for (int i = 1; i <= res_num;i++) {
		p[i - 1]->showProductInfo(this->cfd, 1);
	}

	// 接收从客户端传送的 选择的 对应编号的商品
	this->cfd->receiveData(buffer, BUFSIZ);
	int select_id = atoi(buffer);
	memset(buffer, 0, BUFSIZ);
	mysql_free_result(result);
	Product* selectedProduct = p[select_id - 1];
	// 判断消费者余额是否足够(先算出订单总价值)
	db->op = "select originPrice, discount_rate from products where product_id = "
		+ to_string(selectedProduct->product_id);
	db->query();
	result = mysql_store_result(&db->mysql);
	if (result == NULL) {
		std::cerr << "mysql_store_result() failed: " << mysql_error(&db->mysql) << std::endl;
		return;
	}
	row = mysql_fetch_row(result);
	double originPrice = atof(row[0]);
	double discount_rate = atof(row[1]);
	double totalPrice = selectedProduct->freeze_num * originPrice * discount_rate;
	if (this->balance < totalPrice) {
		/*cout << "余额不足，无法生成订单" << endl;*/
		this->cfd->sendData("0", BUFSIZ);
		return;
	}

	// 接下来进行生成订单操作 
	// 1. 将购物车内的商品状态改为已生成订单
	db->op = "update shopcart set isCreateOrder = 1"
		" where shopcart_id = " + to_string(selectedProduct->product_remain);
	db->query();
	// 2. 改变商家冻结商品数和可卖商品数
	db->op = "update products set freeze_num = freeze_num -" 
		+ to_string(selectedProduct->freeze_num)
		+ " where product_id = " + to_string(selectedProduct->product_id);
	db->query();
	// 3. 改变消费者和商家余额
	ifstream ifs;
	ifs.open(ACCOUNT_FILENAME, ios::in);
	vector<Account*> accounts;
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
	int business_id = selectedProduct->business_id;
	accounts[business_id - 1]->balance += totalPrice;
	accounts[this->id - 1]->balance -= totalPrice;
	this->balance = accounts[this->id - 1]->balance;

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
	// 订单信息发送给客户端
	sendBuffer = "1 ";		// 订单生成成功
	sendBuffer += "==========================================\n";
	sendBuffer += "订单编号：" + to_string(selectedProduct->product_id) + "\n";
	sendBuffer += "商家编号：" + to_string(selectedProduct->business_id) + "\n";
	sendBuffer += "商品名称：" + selectedProduct->name + "\n";
	sendBuffer += "商品类型：" + productType[selectedProduct->type] + "\n";
	sendBuffer += "商品数量：" + to_string(selectedProduct->freeze_num) + "\n";
	sendBuffer += "商品单价：" + to_string(selectedProduct->getPrice()) + "\n";
	sendBuffer += "订单总价：" + to_string(totalPrice) + "\n";
	sendBuffer += "==========================================\n";
	this->cfd->sendData(sendBuffer.c_str(), sendBuffer.size());

	delete db;
}

