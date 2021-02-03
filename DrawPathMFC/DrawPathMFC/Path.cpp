#include "StdAfx.h"
#include <vector>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include "Path.h"
#include <mysql.h>


#define PI acos(-1)

using namespace std;

double Path::get_lc2(double rylc) {

	int lc1 = (int)rylc / 1000;
	return rylc - lc1 * 1000;
}

Angle Path::to_angle(double a) {
	Angle angle;
	angle.degree = (int)a;
	double remainder = a - angle.degree;
	angle.minute = (int)(60 * remainder);
	remainder = a - angle.degree - angle.minute / 60.0;
	angle.second = (int)(3600 * remainder);
	return angle;
}
string to_formal_string(double input) {//保留小数点后2位
	string temp = to_string(input);
	if (temp.find('.') != temp.npos) {//存在.
		return temp.substr(0, temp.find('.') + 3);
	}
	return temp;
}

//交点的构造函数
JD::JD(double jd_x, double jd_y, double jd_r, double jd_l0) {
	this->jd_x = jd_x;
	this->jd_y = jd_y;
	this->jd_r = jd_r;
	this->jd_l0 = jd_l0;
}

//返回两个交点的距离
double JD::get_distance(const JD& jd) const {

	return sqrt(pow(jd_x - jd.jd_x, 2) + pow(jd_y - jd.jd_y, 2));

}


Path::Path() {

	this->qx_done = false; //曲线计算未完成
	this->lc_done = false; //里程计算未完成
	this->qdlc1 = 0;
	this->qdlc2 = 0;
	this->zdlc = 0;

}

//编辑交点信息
void Path::edit_jd(vector<JD>& jd_vector, int jd_index, int attribute_index, string value) {
	double v = strtod(value.c_str(), NULL);
	if (attribute_index == 1) {
		jd_vector[jd_index].jd_x = v;
	}
	else if (attribute_index == 2) {
		jd_vector[jd_index].jd_y = v;
	}
	else if (attribute_index == 3)
	{
		jd_vector[jd_index].jd_r = v;
	}
	else
	{
		jd_vector[jd_index].jd_l0 = v;
	}

}

//展示坐标形式
string ZB::show_zb() const {
	return "(" + to_formal_string(this->x) + ", " + to_formal_string(this->y) + ")";
}

//获取交点信息
vector<string> Path::get_jd(const vector<JD>& jd_vector, int index) {
	vector<string> out(5);
	out[0] = to_string(index + 1);
	out[1] = to_formal_string(jd_vector[index].jd_x);
	out[2] = to_formal_string(jd_vector[index].jd_y);
	out[3] = to_formal_string(jd_vector[index].jd_r);
	out[4] = to_formal_string(jd_vector[index].jd_l0);
	return out;
}

//获取曲线信息
vector<string> Path::get_quxian(const vector<QX>& qx_vector, int index) {
	vector<string> out(13);
	out[0] = to_string(index + 1);
	out[1] = to_formal_string(qx_vector[index].qx_fwj);
	out[2] = to_formal_string(qx_vector[index].qx_a);
	out[3] = to_formal_string(qx_vector[index].qx_l0);
	out[4] = to_formal_string(qx_vector[index].qx_L);
	out[5] = to_formal_string(qx_vector[index].qx_R);
	out[6] = to_formal_string(qx_vector[index].qx_m);
	out[7] = to_formal_string(qx_vector[index].qx_p);
	out[8] = to_formal_string(qx_vector[index].qx_T);
	out[9] = to_formal_string(qx_vector[index].qx_E);
	out[10] = to_formal_string(qx_vector[index].qx_lc);
	out[11] = to_formal_string(qx_vector[index].qx_b);
	out[12] = to_formal_string(qx_vector[index].qx_q);
	return out;

}

//获取主点里程坐标信息
vector<string> Path::get_licheng(const vector<LC>& lc_vector, int index) {
	vector<string> out(11);
	out[0] = to_string(index + 1);
	out[1] = to_formal_string(lc_vector[index].zh);
	out[2] = lc_vector[index].zh_zb.show_zb();
	out[3] = to_formal_string(lc_vector[index].hy);
	out[4] = lc_vector[index].hy_zb.show_zb();
	out[5] = to_formal_string(lc_vector[index].qz);
	out[6] = lc_vector[index].qz_zb.show_zb();
	out[7] = to_formal_string(lc_vector[index].yh);
	out[8] = lc_vector[index].yh_zb.show_zb();
	out[9] = to_formal_string(lc_vector[index].hz);
	out[10] = lc_vector[index].hz_zb.show_zb();
	return out;

}

//里程格式补足0
string fill_zero(int lc) {

	string lc_string = to_string(lc);
	int lc_length = lc_string.length();
	for (int i = 0; i < 3 - lc_length; i++) {
		lc_string = "0" + lc_string;
	}
	return lc_string;

}

//里程格式DKx+xxx
string get_formal_licheng(int lc) {

	int lc1 = lc / 1000;
	int lc2 = lc % 1000;
	return "DK" + to_string(lc1) + "+" + fill_zero(lc2);

}

int Path::save_jd_to_db(const vector<JD>& jd_vector, DB& db) {
	if (jd_num(jd_vector) < 1) {
		cout << "没有可保存的交点" << endl;
		return 1;
	}
	MYSQL mysql;    //一个数据库结构体
	//初始化数据库
	mysql_init(&mysql);
	//设置编码方式
	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");
	//连接数据库
	//判断如果连接失败就输出连接失败。
	string query;

	if (mysql_real_connect(&mysql, db.ip.c_str(), db.user.c_str(), db.passwd.c_str(), db.db.c_str(), strtol(db.port.c_str(), NULL, 0), NULL, 0) == NULL)
		return 2;

	query = "create table " + db.table + CREATE_JD_TABLE;

	mysql_query(&mysql, query.c_str());
	for (int i = 0; i < jd_vector.size(); i++) {
		query = "insert into " + db.table + " values (" + to_string(i + 1) + "," + to_string(jd_vector[i].jd_x) + "," + to_string(jd_vector[i].jd_y) + "," + to_string(jd_vector[i].jd_r) + "," + to_string(jd_vector[i].jd_l0) + ")";
		mysql_query(&mysql, query.c_str());
	}

	mysql_close(&mysql);
	return 0;

}
//保存交点信息
bool Path::save_jd(const vector<JD>& jd_vector, string file_name) {
	if (jd_num(jd_vector) < 1) {
		cout << "没有可保存的交点" << endl;
		return false;
	}

	ofstream outFile;
	cout << fixed;
	//cout<<"请输入保存的文件名（无后缀）："<<endl;
	//string file_name;
	//cin>>file_name;
	outFile.open(file_name + ".csv", ios::out); //打开文件并写入（覆盖）



	outFile << "交点号" << ',' << "交点坐标X" << ',' << "交点坐标Y" << "," << "圆曲线半径R" << ',' << "缓和曲线长L0" << endl;

	for (int i = 0; i < jd_num(jd_vector); i++)
		outFile << i + 1 << ',' << jd_vector[i].jd_x << ',' << jd_vector[i].jd_y << "," << jd_vector[i].jd_r << ',' << jd_vector[i].jd_l0 << endl;

	outFile << endl;

	outFile.close(); //关闭文件

	cout << "文件：" << file_name << ".csv " << "已经被保存至当前路径。" << endl;
	return true;



}


// 保存结果到数据库

int Path::save_result_to_db(vector<JD>& jd_vector, const vector<QX>& qx_vector, const vector<LC>& lc_vector, DB &db) {
	if (!is_qx_done() || !is_lc_done()) { //如果没有进行计算
		cout << "请计算曲线和里程信息之后保存结果。" << endl;
		return 1;
	}
	MYSQL mysql;    //一个数据库结构体
	//初始化数据库
	mysql_init(&mysql);
	//设置编码方式
	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");
	//连接数据库
	//判断如果连接失败就输出连接失败。
	string query;

	if (mysql_real_connect(&mysql, db.ip.c_str(), db.user.c_str(), db.passwd.c_str(), db.db.c_str(), strtol(db.port.c_str(), NULL, 0), NULL, 0) == NULL)
		return 2;

	query = "create table " + db.table + CREATE_RESULT_TABLE;
	mysql_query(&mysql, query.c_str());

	//插入数据
	for (int i = 0; i < jd_vector.size(); i++) {
		query = "insert into "
			+ db.table + " values ("
			+ to_string(i + 1) + ","
			+ to_string(qx_vector[i].qx_fwj) + ","
			+ to_string(qx_vector[i].qx_a) + ","
			+ to_string(qx_vector[i].qx_T) + ","
			+ to_string(qx_vector[i].qx_L) + ", '"
			+ lc_vector[i].zh_zb.show_zb() + "',"
			+ to_string(lc_vector[i].zh) + ", '"
			+ lc_vector[i].hy_zb.show_zb() + "',"
			+ to_string(lc_vector[i].hy) + ", '"
			+ lc_vector[i].qz_zb.show_zb() + "',"
			+ to_string(lc_vector[i].qz) + ", '"
			+ lc_vector[i].yh_zb.show_zb() + "',"
			+ to_string(lc_vector[i].yh) + ", '"
			+ lc_vector[i].hz_zb.show_zb() + "',"
			+ to_string(lc_vector[i].hz) + ");";

		mysql_query(&mysql, query.c_str());
	}
	mysql_close(&mysql);
	return 0;
}




//保存计算结果
bool Path::save_result(vector<JD>& jd_vector, const vector<QX>& qx_vector, const vector<LC>& lc_vector, string file_name) {
	if (!is_qx_done() || !is_lc_done()) { //如果没有进行计算
		cout << "请计算曲线和里程信息之后保存结果。" << endl;
		return false;
	}

	ofstream outFile;
	cout << fixed;
	//cout<<"请输入保存的文件名（无后缀）："<<endl;
	//string file_name;
	//cin>>file_name;
	outFile.open(file_name + ".csv", ios::out); //打开文件并写入（覆盖）



	outFile << "交点号" << ',' << "方位角（弧度）" << ',' << "转向角（角度）" << "," << "切线长" << ',' << "曲线长" << ',' << "直缓点坐标" \
		<< ',' << "直缓点里程" << ',' << "缓圆点坐标" << "," << "缓圆点里程" << "," << "曲中点坐标" << "," << "曲中点里程" << "," << "圆缓点坐标" << ","\
		<< "圆缓点里程" << "," << "缓直点坐标" << "," << "缓直点里程" << endl;

	for (int i = 0; i < jd_num(jd_vector); i++)
		outFile << i + 1 << ',' << qx_vector[i].qx_fwj << ',' << qx_vector[i].qx_a << "," << qx_vector[i].qx_T << ',' << qx_vector[i].qx_L << ',' << lc_vector[i].zh_zb.show_zb()\
		<< ',' << lc_vector[i].zh << ',' << lc_vector[i].hy_zb.show_zb() << "," << lc_vector[i].hy << "," << lc_vector[i].qz_zb.show_zb() << "," << lc_vector[i].qz << "," << lc_vector[i].yh_zb.show_zb() << ","\
		<< lc_vector[i].yh << "," << lc_vector[i].hz_zb.show_zb() << "," << lc_vector[i].hz << endl;

	outFile << endl;

	outFile << "起点里程\t," << get_formal_licheng(get_qdlc1() * 1000 + get_qdlc2()) << endl;
	outFile << "终点里程\t," << get_formal_licheng(get_zdlc()) << endl;

	outFile.close(); //关闭文件

	cout << "文件：" << file_name << ".csv " << "已经被保存至当前路径。" << endl;
	return true;

}


int Path::read_data_from_db(vector<JD>& jd_vector, DB &db) {
	MYSQL mysql;
	MYSQL_RES *mysql_result;
	MYSQL_ROW mysql_row;
	mysql_init(&mysql);
	//设置编码方式
	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");
	//连接数据库
	//判断如果连接失败就输出连接失败。
	string query;

	if (mysql_real_connect(&mysql, db.ip.c_str(), db.user.c_str(), db.passwd.c_str(), db.db.c_str(), strtol(db.port.c_str(), NULL, 0), NULL, 0) == NULL)
		return 2;

	query = "select * from " + db.table + ";";

	if (mysql_query(&mysql, query.c_str()) == 0) {
		mysql_result = mysql_store_result(&mysql);
		int row = mysql_num_rows(mysql_result);
		if (row == 0)
			return 1; //没有数据
		for (int i = 0; i < row; i++) {
			JD jd;
			mysql_row = mysql_fetch_row(mysql_result);
			jd.jd_x = strtod(mysql_row[1], NULL);
			jd.jd_y = strtod(mysql_row[2], NULL);
			jd.jd_l0 = strtod(mysql_row[3], NULL);
			jd.jd_r = strtod(mysql_row[4], NULL);
			jd_vector.push_back(jd);
		}
	}
	else { //没有这个表
		return 3;
	}
	mysql_close(&mysql);

	return 0;



}

//读数据
bool Path::read_data(vector<JD>& jd_vector, string file_name) {
	//cout<<"请输入读入文件的全名："<<endl;
	//string file_name;
	//cin>>file_name;
	ifstream f(file_name, ios::in);
	//输入要打卡的文件名并试图打开

	if (!f.is_open()) {
		cout << "找不到文件：" << file_name << " 请重新输入！" << endl;
		return false;
	}

	//没有打开文件则退出，支持csv文件或者txt文件

	string line;
	getline(f, line); // 读取第一行表头
	while (getline(f, line)) {
		if (line == "") break;
		replace(line.begin(), line.end(), '\t', ' '); //excel 粘贴到txt
		replace(line.begin(), line.end(), ',', ' '); //csv文件
		stringstream input(line);
		cout << line << endl;
		string num;
		vector<double> nums;
		while (input >> num) {
			if (num != "") {
				double n = stod(num);
				nums.push_back(n);
			}

		}

		//顺序读取四个数字 跳过第一个交点号
		JD jd = JD(nums[1], nums[2], nums[3], nums[4]);

		jd_vector.push_back(jd);
	}

	f.close();
	cout << "成功读取信息!" << endl;
	show_jd(jd_vector);

	MYSQL mysql;    //一个数据库结构体
	MYSQL_RES* res; //一个结果集结构体
	MYSQL_ROW row;  //char** 二维数组，存放一条条记录
	//初始化数据库
	mysql_init(&mysql);
	//设置编码方式
	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");
	//连接数据库
	//判断如果连接失败就输出连接失败。
	string query;


	if (mysql_real_connect(&mysql, "localhost", "root", "qAz@123456", "my_db", 3306, NULL, 0) == NULL)
		cout << "Fail to conncet" << endl;
	//查询数据
	for (int i = 0; i < jd_vector.size(); i++) {
		query = "insert into jiaodian values (" + to_string(i + 1) + "," + to_string(jd_vector[i].jd_x) + "," + to_string(jd_vector[i].jd_y) + "," + to_string(jd_vector[i].jd_r) + "," + to_string(jd_vector[i].jd_l0) + ");";
		mysql_query(&mysql, query.c_str());
	}
	mysql_close(&mysql);

	return true;
}

//计算曲线信息
bool Path::compute_quxian(const vector<JD>& jd_vector, vector<QX>& qx_vector) {
	if (jd_num(jd_vector) < 3)
	{
		cout << "交点数量不足，无法计算！" << endl;
		return false;
	}

	else
	{
		//方位角
		for (int i = 0; i < (jd_num(jd_vector) - 1); i++)
		{
			QX qx;

			if (jd_vector[i + 1].jd_y == jd_vector[i].jd_y)
			{
				qx.qx_fwj = 90;
			}
			else
			{
				double fwj = atan2((jd_vector[i + 1].jd_x - jd_vector[i].jd_x), (jd_vector[i + 1].jd_y - jd_vector[i].jd_y));
				if (fwj >= 0)
				{
					qx.qx_fwj = fwj;

				}
				else
				{
					qx.qx_fwj = fwj + PI;

				}

			}
			qx_vector.push_back(qx);

		}

		QX qx;
		qx_vector.push_back(qx);

		for (int i = 1; i < jd_num(jd_vector) - 1; i++) {
			// 转向角
			qx_vector[i].qx_a = (qx_vector[i].qx_fwj - qx_vector[i - 1].qx_fwj) / PI * 180;
			// 曲线半径
			qx_vector[i].qx_R = jd_vector[i].jd_r;
			// 缓和曲线长
			qx_vector[i].qx_l0 = jd_vector[i].jd_l0;
			// 内移距
			qx_vector[i].qx_p = pow(qx_vector[i].qx_l0, 2) / (24 * qx_vector[i].qx_R) - pow(qx_vector[i].qx_l0, 4) / (2688 * pow(qx_vector[i].qx_R, 3));
			// 切垂距
			qx_vector[i].qx_m = qx_vector[i].qx_l0 / 2 - pow(qx_vector[i].qx_l0, 3) / (240 * pow(qx_vector[i].qx_R, 2));
			// 切线长
			qx_vector[i].qx_T = (qx_vector[i].qx_R + qx_vector[i].qx_p) * tan(abs(qx_vector[i].qx_a) * PI / 360) + qx_vector[i].qx_m;
			// 曲线长
			qx_vector[i].qx_L = PI * pow(pow(qx_vector[i].qx_a, 2), 0.5) * qx_vector[i].qx_R / 180 + qx_vector[i].qx_l0;
			// 外矢距
			qx_vector[i].qx_E = (qx_vector[i].qx_R + qx_vector[i].qx_p) / cos(qx_vector[i].qx_a * PI / 360) - qx_vector[i].qx_R;
			// 缓和曲线角
			qx_vector[i].qx_b = qx_vector[i].qx_l0 * 180 / (2 * qx_vector[i].qx_R * PI);
			// 圆曲线长
			qx_vector[i].qx_lc = abs(qx_vector[i].qx_R * PI * (qx_vector[i].qx_a - 2 * qx_vector[i].qx_b) / 180);
			// 切曲差
			qx_vector[i].qx_q = 2 * qx_vector[i].qx_T - qx_vector[i].qx_L;

		}



	}

	set_qx_done(true);
	cout << "曲线信息计算完成！" << endl;
	show_quxian(qx_vector);
	return true;
}

//计算里程坐标
bool Path::compute_licheng(const vector<JD>& jd_vector, const vector<QX>& qx_vector, vector<LC>& lc_vector, int qdlc1, int qdlc2) {



	set_qdlc1(qdlc1);
	set_qdlc2(qdlc2);

	//坐标计算
	double qd = qdlc1 * 1000 + qdlc2;
	for (int i = 0; i < jd_num(jd_vector); i++) {

		if (i == 0 || i == jd_num(jd_vector) - 1) { //第一个以及最后一个交点的坐标不变

			//lc_vector[i].qd = qdlc1 * 1000 +qdlc2; // 初始化起点
			//lc_vector[i].zh = lc_vector[i].qd + dis - qx_vector[i].qx_T; //初始化zh
			//lc_vector[i].zh_zb.x = jd_vector[0].jd_x + (dis - qx_vector[0].qx_T) * cos(qx_vector[0].qx_fwj);
			//lc_vector[i].zh_zb.y = jd_vector[0].jd_y + (dis - qx_vector[0].qx_T) * sin(qx_vector[0].qx_fwj);、
			LC lc;
			lc.zh_zb.x = jd_vector[i].jd_x;
			lc.zh_zb.y = jd_vector[i].jd_y;
			lc.hy_zb.x = jd_vector[i].jd_x;
			lc.hy_zb.y = jd_vector[i].jd_y;
			lc.qz_zb.x = jd_vector[i].jd_x;
			lc.qz_zb.y = jd_vector[i].jd_y;
			lc.yh_zb.x = jd_vector[i].jd_x;
			lc.yh_zb.y = jd_vector[i].jd_y;
			lc.hz_zb.x = jd_vector[i].jd_x;
			lc.hz_zb.y = jd_vector[i].jd_y;
			if (i == jd_num(jd_vector) - 1) {
				double distance = sqrt(pow((jd_vector[i].jd_x - lc_vector[i - 1].hz_zb.x), 2) + pow((jd_vector[i].jd_y - lc_vector[i - 1].hz_zb.y), 2));// 最后一个交点与倒数第二个缓直坐标的距离
				set_zdlc(lc_vector[i - 1].hz + distance); // 终点里程：缓直里程加以上距离
			}

			lc_vector.push_back(lc);
			continue;
		}

		double x0 = qx_vector[i].qx_l0 - pow(qx_vector[i].qx_l0, 5) / (pow(qx_vector[i].qx_R, 2) * pow(qx_vector[i].qx_l0, 2) * 40);
		double y0 = pow(qx_vector[i].qx_l0, 3) / (qx_vector[i].qx_R * qx_vector[i].qx_l0 * 6) - pow(qx_vector[i].qx_l0, 7) / (pow(qx_vector[i].qx_R, 3) * pow(qx_vector[i].qx_l0, 3) * 336);

		double xqz = qx_vector[i].qx_m + qx_vector[i].qx_R * sin((qx_vector[i].qx_L / 2 - qx_vector[i].qx_l0) / qx_vector[i].qx_R + qx_vector[i].qx_l0 / (2 * qx_vector[i].qx_R));
		double yqz = qx_vector[i].qx_p + qx_vector[i].qx_R * (1 - cos((qx_vector[i].qx_L / 2 - qx_vector[i].qx_l0) / qx_vector[i].qx_R + qx_vector[i].qx_l0 / (2 * qx_vector[i].qx_R)));


		double Q = -1;
		if (qx_vector[i].qx_a > 0)
			Q = 1;

		double jd_x = jd_vector[i].jd_x;
		double jd_y = jd_vector[i].jd_y;
		double a1 = qx_vector[i - 1].qx_fwj;
		double a2 = qx_vector[i].qx_fwj;

		LC lc;

		lc.zh_zb.x = jd_x + qx_vector[i].qx_T * sin(a1 + PI);
		lc.zh_zb.y = jd_y + qx_vector[i].qx_T * cos(a1 + PI);
		lc.hz_zb.x = jd_x + qx_vector[i].qx_T * sin(a2);
		lc.hz_zb.y = jd_y + qx_vector[i].qx_T * cos(a2);
		lc.hy_zb.x = lc.zh_zb.x - x0 * sin(2 * PI - a1) + Q * y0 * cos(2 * PI - a1);
		lc.hy_zb.y = lc.zh_zb.y + x0 * cos(2 * PI - a1) + Q * y0 * sin(2 * PI - a1);
		lc.yh_zb.x = lc.hz_zb.x + x0 * sin(2 * PI - a2) + Q * y0 * cos(2 * PI - a2);
		lc.yh_zb.y = lc.hz_zb.y - x0 * cos(2 * PI - a2) + Q * y0 * sin(2 * PI - a2);
		lc.qz_zb.x = lc.zh_zb.x - xqz * sin(2 * PI - a1) + Q * yqz * cos(2 * PI - a1);
		lc.qz_zb.y = lc.zh_zb.y + xqz * cos(2 * PI - a1) + Q * yqz * sin(2 * PI - a1);

		//里程计算
		double dis = jd_vector[i].get_distance(jd_vector[i - 1]);

		if (i == 1) {
			//第1个曲线有不同的计算方法
			lc.zh = qd + dis - qx_vector[i].qx_T;
			lc.hy = lc.zh + qx_vector[i].qx_l0;
			lc.qz = lc.zh + qx_vector[i].qx_L / 2;
			lc.yh = lc.zh + qx_vector[i].qx_L - qx_vector[i].qx_l0;
			lc.hz = lc.zh + qx_vector[i].qx_L;
			lc_vector.push_back(lc);
			continue;
		}

		lc.zh = lc_vector[i - 1].hz + dis - qx_vector[i].qx_T - qx_vector[i - 1].qx_T;
		lc.hy = lc.zh + qx_vector[i].qx_l0;
		lc.qz = lc.zh + qx_vector[i].qx_L / 2;
		lc.yh = lc.zh + qx_vector[i].qx_L - qx_vector[i].qx_l0;
		lc.hz = lc.zh + qx_vector[i].qx_L;

		lc_vector.push_back(lc);
		cout << i << "\tzh: " << lc_vector[i].zh_zb.show_zb() << "\thz: " << lc_vector[i].hz_zb.show_zb() << "\thy: " << lc_vector[i].hy_zb.show_zb() << "\tyh: " << lc_vector[i].yh_zb.show_zb() << "\tqz:" << lc_vector[i].qz_zb.show_zb() << endl;


	}

	set_lc_done(true);
	cout << "里程信息计算完毕" << endl;
	show_licheng(lc_vector);
	return true;
}

//展示里程信息
bool Path::show_licheng(const vector<LC>& lc_vector) {
	cout.precision(10);
	if (!is_lc_done()) {
		cout << "请先计算里程信息！" << endl;
		return  false;
	}
	cout << "起始里程：DK" << get_qdlc1() << "+" << get_qdlc2() << endl;
	for (int i = 0; i < lc_vector.size(); i++) {
		LC lc = lc_vector[i];
		cout << i << ":\t直缓点里程: " << lc.zh << \
			"\t缓圆点里程: " << lc.hy << \
			"\t曲直点里程: " << lc.qz << \
			"\t圆缓点里程: " << lc.yh << \
			"\t缓直点里程:" << lc.hz << endl;
	}

	return true;
}

//展示曲线信息
bool Path::show_quxian(const vector<QX>& qx_vector) {
	cout.precision(10);
	if (!is_qx_done()) {
		cout << "请先计算曲线信息！" << endl;
		return false;
	}

	cout << "曲线要素：" << endl;
	for (int i = 0; i < qx_vector.size(); i++) {
		QX qx = qx_vector[i];
		cout << "交点" << (i + 1) << ":\t";
		cout << "方位角：" << qx.qx_fwj << "\t";
		cout << "曲线转角：" << qx.qx_a << "*\t";
		cout << "缓和曲线长：" << qx.qx_l0 << "\t";
		cout << "曲线长：" << qx.qx_L << "\t";
		cout << "曲线半径：" << qx.qx_R << endl;
		cout << "切垂距：" << qx.qx_m << "\t";
		cout << "内移距：" << qx.qx_p << "\t";
		cout << "切线长：" << qx.qx_T << "\t";
		cout << "外失距：" << qx.qx_E << "\t";
		cout << "圆曲线长：" << qx.qx_lc << "\t";
		cout << "缓和曲线角：" << qx.qx_b << "\t";
		cout << "切曲差：" << qx.qx_q << endl << endl;
	}
	return true;
}

//展示交点信息
bool Path::show_jd(const vector<JD>& jd_vector) {
	cout.precision(10);
	if (jd_num(jd_vector) == 0)
	{
		cout << "当前记录为空！" << endl;
		return false;
	}
	else
	{
		cout << "交点信息如下： " << endl;
		for (int i = 0; i < jd_num(jd_vector); i++)
		{
			cout << "交点" << (i + 1) << "： ";
			cout << "交点坐标x：" << jd_vector[i].jd_x << " ";
			cout << "交点坐标y：" << jd_vector[i].jd_y << " ";
			cout << "缓和曲线长(m)：" << jd_vector[i].jd_l0 << " ";
			cout << "圆曲线半径(m)：" << jd_vector[i].jd_r << endl;
		}
	}
	return true;
}

//计算里程转坐标
ZB Path::lc_to_zb(const vector<JD>& jd_vector, const vector<QX>& qx_vector, const vector<LC>& lc_vector, int rylc)//里程转坐标
{
	if (!is_lc_done()) {   // 判断里程信息是否已经计算完毕
		cout << "请先计算里程" << endl;
		return ZB(-1, -1);
	}
	int qdlc = get_qdlc1() * 1000 + get_qdlc2();

	if (rylc < qdlc || rylc > get_zdlc()) {  // 判断输入里程是否越界
		cout << "Out of Range!" << endl;
		return ZB(-2, -2);
	}

	double x, y; // 坐标xy
	double dx, dy; // x与y方向上的偏移量
	double lp; // 任意点p与特定点的距离

	if (rylc < lc_vector[1].zh || rylc >= lc_vector[jd_num(jd_vector) - 2].hz) {
		if (rylc < lc_vector[1].zh) {  // 如果任意里程小于第一个直缓点
			lp = rylc - qdlc; // 任意里程到起点的距离 
			dx = sin(qx_vector[0].qx_fwj) * lp;
			dy = cos(qx_vector[0].qx_fwj) * lp;
			x = jd_vector[0].jd_x + dx;
			y = jd_vector[0].jd_y + dy;
			cout << "QD - ZH" << endl;
			return ZB(x, y);
		}

		else { // 任意里程在最后一个缓直之后（包含最后一个直缓点）
			lp = get_zdlc() - rylc;
			dx = sin(qx_vector[jd_num(jd_vector) - 2].qx_fwj + PI) * lp;
			dy = cos(qx_vector[jd_num(jd_vector) - 2].qx_fwj + PI) * lp;
			x = jd_vector[jd_num(jd_vector) - 1].jd_x + dx;
			y = jd_vector[jd_num(jd_vector) - 1].jd_y + dy;
			cout << "HZ - ZD" << endl;
			return ZB(x, y);

		}
	}


	for (int i = 1; i < jd_num(jd_vector) - 1; i++) // 遍历每一个曲线部分（前闭后开区间， 从自身直缓点开始直到下一个直缓点（如果有）之前）
	{
		LC lc = lc_vector[i];
		if (rylc >= lc.zh && rylc < lc.hz) {
			double a1 = qx_vector[i - 1].qx_fwj;
			double a2 = qx_vector[i].qx_fwj;
			double Q = -1;
			if (qx_vector[i].qx_a > 0)
				Q = 1;

			if (rylc < lc.hy) { // ZH - HY
				lp = rylc - lc.zh;
				dx = lp - pow(lp, 5) / (40 * pow(qx_vector[i].qx_R, 2) * pow(qx_vector[i].qx_l0, 2));
				dy = pow(lp, 3) / (6 * qx_vector[i].qx_R * qx_vector[i].qx_l0) - pow(lp, 7) / (336 * pow(qx_vector[i].qx_R, 3) * pow(qx_vector[i].qx_l0, 3));
				x = lc.zh_zb.x - dx * sin(2 * PI - a1) + Q * dy * cos(2 * PI - a1);
				y = lc.zh_zb.y + dx * cos(2 * PI - a1) + Q * dy * sin(2 * PI - a1);
				cout << "ZH - HY" << endl;
			}

			else if (rylc < lc.qz) { // HY - QZ

				lp = rylc - lc.zh;
				dx = qx_vector[i].qx_m + qx_vector[i].qx_R * sin((lp - qx_vector[i].qx_l0) / qx_vector[i].qx_R + qx_vector[i].qx_l0 / (2 * qx_vector[i].qx_R));
				dy = qx_vector[i].qx_p + qx_vector[i].qx_R * (1 - cos((lp - qx_vector[i].qx_l0) / qx_vector[i].qx_R + qx_vector[i].qx_l0 / (2 * qx_vector[i].qx_R)));
				x = lc.zh_zb.x - dx * sin(2 * PI - a1) + Q * dy * cos(2 * PI - a1);
				y = lc.zh_zb.y + dx * cos(2 * PI - a1) + Q * dy * sin(2 * PI - a1);
				cout << "HY - QZ" << endl;

			}

			else if (rylc < lc.yh) { // QZ - YH
				lp = lc.hz - rylc;
				dx = qx_vector[i].qx_m + qx_vector[i].qx_R * sin((lp - qx_vector[i].qx_l0) / qx_vector[i].qx_R + qx_vector[i].qx_l0 / (2 * qx_vector[i].qx_R));
				dy = qx_vector[i].qx_p + qx_vector[i].qx_R * (1 - cos((lp - qx_vector[i].qx_l0) / qx_vector[i].qx_R + qx_vector[i].qx_l0 / (2 * qx_vector[i].qx_R)));
				x = lc.hz_zb.x + dx * sin(2 * PI - a2) + Q * dy * cos(2 * PI - a2);
				y = lc.hz_zb.y - dx * cos(2 * PI - a2) + Q * dy * sin(2 * PI - a2);
				cout << "QZ - YH" << endl;
			}

			else { // YH - HZ
				lp = lc.hz - rylc;
				dx = lp - pow(lp, 5) / (40 * pow(qx_vector[i].qx_R, 2) * pow(qx_vector[i].qx_l0, 2));
				dy = pow(lp, 3) / (6 * qx_vector[i].qx_R * qx_vector[i].qx_l0) - pow(lp, 7) / (336 * pow(qx_vector[i].qx_R, 3) * pow(qx_vector[i].qx_l0, 3));
				x = lc.hz_zb.x + dx * sin(2 * PI - a2) + Q * dy * cos(2 * PI - a2);
				y = lc.hz_zb.y - dx * cos(2 * PI - a2) + Q * dy * sin(2 * PI - a2);
				cout << "YH - HZ" << endl;
			}
			break;
		}

		if (i != jd_num(jd_vector) - 2) { // 如果不是倒数第二个点，看是否在 HZ - 下一个直缓
			if (rylc >= lc.hz && rylc < lc_vector[i + 1].zh) {
				double a2 = qx_vector[i].qx_fwj;
				lp = rylc - lc.hz;
				dx = lp * sin(a2);
				dy = lp * cos(a2);
				x = lc.hz_zb.x + dx;
				y = lc.hz_zb.y + dy;;
				cout << "HZ - ZH" << endl;
				break;
			}
		}

	}
	return ZB(x, y);

}
