#include"AccountWidget.h"

extern map<int, string> productType;

Consumer::Consumer() {

}
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
	cout << "*************** 3 购物车修改 *****************" << endl;
	cout << "*************** 4 生成订单 *******************" << endl;
	cout << "*************** 5 修改密码 *******************" << endl;
	cout << "*************** 6 余额查询 *******************" << endl;
	cout << "*************** 7 余额充值 *******************" << endl;
	cout << "*************** 8 用户信息 *******************" << endl;
	cout << "*************** 9 查找商品 *******************" << endl;
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
		char buffer[BUFSIZ] = { 0 };
		buffer[0] = select + '0';
		this->cfd->sendData(buffer, BUFSIZ);

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
			system("cls");
			break;
		}
		cout << endl;
		system("pause");
		system("cls");
	}
}

//购物车添加
void Consumer::addShopCart() {
	char buffer[BUFSIZ] = { 0 };
	string sendBuffer;
	string line_name;
	cout << "请输入想买商品的名称" << endl;
	cin >> line_name;
	this->cfd->sendData(line_name.c_str(), line_name.size());

	// 接收服务器返回的根据商品名称查找的结果
	this->cfd->receiveData(buffer, BUFSIZ);
	cout << buffer << endl;
	int res_num = atoi(buffer);
	if (buffer[0] == '-') {
		cout << "不存在所找商品,请重新操作" << endl;
		return;
	}
	memset(buffer, 0, BUFSIZ);
	// 展示 res_num 个商品信息
	for (int i = 0;i < res_num;i++) {
		cout << "查询商品序号：" << i + 1 << endl;
		this->cfd->receiveData(buffer, BUFSIZ);
		string data = string(buffer).substr(2);
		cout << data << endl;
		memset(buffer, 0, BUFSIZ);
	}

	// 进一步选择对应编号的商品
	cout << "请输入想购买列表中的商品序号: " << endl;
	int select_id;
	cin >> select_id;
	// 判断选择是否合法
	while (select_id < 0 || select_id > res_num) {
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cout << "输入商品序号错误，请重新操作" << endl;
		cin >> select_id;
	}
	sendBuffer = to_string(select_id);
	this->cfd->sendData(sendBuffer.c_str(), sendBuffer.size());
	sendBuffer.clear();

	// 预计买入数量
	int num;
	cout << "请输入预计买入数量" << endl;
	cin >> num;
	while (num <= 0) {
		cout << "输入商品数量错误，请重新操作" << endl;
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cin >> num;
	}
	sendBuffer = to_string(num);
	this->cfd->sendData(sendBuffer.c_str(), sendBuffer.size());

	// 判断购物车是否添加成功
	this->cfd->receiveData(buffer, BUFSIZ);
	if (buffer[0] == '1') {
		cout << "添加购物车成功" << endl;
	}
	else if (buffer[0] == '0') {
		cout << "商家现有商品数不足，无法加入购物车" << endl;
	}
	else {
		cout << "添加购物车失败，请重新操作" << endl;
	}
}
//购物车删除
void Consumer::delShopCart() {
	// 购物车删除

	char buffer[BUFSIZ] = { 0 };
	string line_name;
	cout << "请输入想删除的商品的名称" << endl;
	cin >> line_name;
	this->cfd->sendData(line_name.c_str(), line_name.size());
	
	this->cfd->receiveData(buffer, BUFSIZ);
	if (buffer[0] == '-') {
		cout << "不存在所找商品, 请重新操作" << endl;
	}
	else if(buffer[0] == '1') {
		cout << "成功删除购物车" << endl;
	}
	else {
		cout << "删除购物车失败，请重新操作" << endl;
	}
}

void Consumer::updateShopCartProductNum() {
	cout << "修改购物车内商品买入数量" << endl;

	char buffer[BUFSIZ] = { 0 };
	string sendBuffer;
	string line_name;
	cout << "请输入想修改购物车内的商品名称" << endl;
	cin >> line_name;
	sendBuffer = line_name;
	this->cfd->sendData(sendBuffer.c_str(), sendBuffer.size());
	
	// 接收服务器返回的根据商品名称查找的结果
	this->cfd->receiveData(buffer, BUFSIZ);
	cout << buffer << endl;
	int res_num = atoi(buffer);
	if (buffer[0] == '-') {
		cout << "不存在所找商品,请重新操作" << endl;
		return;
	}
	memset(buffer, 0, BUFSIZ);
	// 展示 res_num 个商品信息
	for (int i = 0;i < res_num;i++) {
		cout << "查询商品序号：" << i + 1 << endl;
		this->cfd->receiveData(buffer, BUFSIZ);
		string data = string(buffer).substr(2);
		cout << data << endl;
		memset(buffer, 0, BUFSIZ);
	}

	// 进一步选择对应编号的商品
	cout << "请输入想购买列表中的商品序号: " << endl;
	int select_id;
	cin >> select_id;
	// 判断选择是否合法
	while (select_id < 0 || select_id > res_num) {
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cout << "输入商品序号错误，请重新操作" << endl;
		cin >> select_id;
	}
	sendBuffer = to_string(select_id);
	this->cfd->sendData(sendBuffer.c_str(), sendBuffer.size());
	sendBuffer.clear();

	// 预计买入数量
	int num;
	cout << "请输入预计买入数量" << endl;
	cin >> num;
	while (num <= 0) {
		cout << "输入商品数量错误，请重新操作" << endl;
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cin >> num;
	}
	sendBuffer = to_string(num);
	this->cfd->sendData(sendBuffer.c_str(), sendBuffer.size());
	
	// 判断购物车是否添加成功
	this->cfd->receiveData(buffer, BUFSIZ);
	if (buffer[0] == '1') {
		cout << "修改购物车成功" << endl;
	}
	else if (buffer[0] == '0') {
		cout << "商家现有商品数不足，无法加入购物车" << endl;
	}
	else {
		cout << "添加购物车失败，请重新操作" << endl;
	}
}

//生成订单
void Consumer::generateOrder() {
	cout << "进行生成订单操作" << endl;
	// 返回给客户端数据 （-1 / 0 / 1）: (商品名未找到 / 余额不足 / 成功)

	char buffer[BUFSIZ] = { 0 };
	string sendBuffer;
	string line_name;
	cout << "请输入想生成订单的购物车内的商品名称" << endl;
	cin >> line_name;
	sendBuffer = line_name;
	this->cfd->sendData(sendBuffer.c_str(), sendBuffer.size());
	sendBuffer.clear();

	// 接收服务器返回的根据商品名称查找的结果
	this->cfd->receiveData(buffer, BUFSIZ);
	cout << buffer << endl;
	system("pause");
	int res_num = atoi(buffer);
	if (buffer[0] == '-') {
		cout << "不存在所找商品,请重新操作" << endl;
		return;
	}
	memset(buffer, 0, BUFSIZ);
	// 展示 res_num 个商品信息
	for (int i = 0;i < res_num;i++) {
		cout << "查询商品序号：" << i + 1 << endl;
		this->cfd->receiveData(buffer, BUFSIZ);
		string data = string(buffer).substr(2);
		cout << data << endl;
		memset(buffer, 0, BUFSIZ);
	}
	// 进一步选择对应编号的商品
	cout << "请输入想生成订单列表中的商品序号" << endl;
	int select_id;
	cin >> select_id;
	// 判断选择是否合法
	while (select_id < 0 || select_id > res_num) {
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cout << "输入商品序号错误，请重新操作" << endl;
		cin >> select_id;
	}
	sendBuffer = to_string(select_id);
	this->cfd->sendData(sendBuffer.c_str(), sendBuffer.size());
	sendBuffer.clear();

	// 判断购物车是否添加成功
	this->cfd->receiveData(buffer, BUFSIZ);
	if (buffer[0] == '1') {
		cout << "修改购物车成功" << endl;
		string data = string(buffer).substr(2);
		cout << "订单信息如下：" << endl;
		cout << data << endl;
		cout << "感谢您的使用，祝您购物愉快！" << endl;
	}
	else if (buffer[0] == '0') {
		cout << "余额不足，请进行充值" << endl;
	}
}


