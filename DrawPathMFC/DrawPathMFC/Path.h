



#ifndef PATH_H
#define PATH_H
#include<vector>
#include<string>
using namespace std;

const string CREATE_RESULT_TABLE = "(交点号 int,\
方位角 double,\
转向角 double,\
切线长 double,\
曲线长 double,\
直缓点坐标 char(30),\
直缓点里程 double,\
缓圆点坐标 char(30),\
缓圆点里程 double,\
曲中点坐标 char(30),\
曲中点里程 double,\
圆缓点坐标 char(30),\
圆缓点里程 double,\
缓直点坐标 char(30),\
缓直点里程 double);";

const string CREATE_JD_TABLE = "(交点号 int,\
交点坐标x double,\
交点坐标y double,\
缓和曲线长 double,\
圆曲线半径 double);";
struct Angle {
	int degree = 0;
	int minute = 0;
	int second = 0;
};

struct DB {
	string ip;
	string port;
	string user;
	string passwd;
	string db;
	string table;
};



struct  QX // 曲线信息
{

	double qx_fwj = 0; // 方位角 （弧度）
	double qx_a = 0; // 曲线转角 （角度）
	double qx_l0 = 0; // 缓和曲线长
	double qx_L = 0; // 曲线长
	double qx_R = 0; // 曲线半径
	double qx_m = 0; // 切垂距
	double qx_p = 0; // 内移距
	double qx_T = 0; // 切线长
	double qx_E = 0; // 外矢距
	double qx_lc = 0; // 圆曲线长
	double qx_b = 0; // 缓和曲线角（角度） 
	double qx_q = 0; //切曲差

};

struct ZB {//坐标
	ZB(double x = 0, double y = 0) { this->x = x; this->y = y; };
	double x = 0;
	double y = 0;
	string show_zb() const;
};

struct LC //里程信息
{

	double zh = 0; //直缓点里程
	ZB zh_zb; //直缓点坐标
	double hy = 0; //缓圆点里程
	ZB hy_zb;//缓圆点坐标
	double qz = 0; //曲中点里程
	ZB qz_zb;//曲中点坐标
	double yh = 0; //圆缓点里程
	ZB yh_zb;//圆缓点坐标
	double hz = 0; //缓直点里程
	ZB hz_zb;//缓直点坐标
};

struct JD
{

	JD(double jd_x = 0, double jd_y = 0, double jd_l0 = 0, double jd_r = 0);
	double get_distance(const JD& jd) const;//两个交点的距离
	double jd_x = 0;//交点坐标x
	double jd_y = 0;//交点坐标y
	double jd_l0 = 0;//缓和曲线长
	double jd_r = 0;//圆曲线半径

};

class Path {
public:
	Path(); //构造函数
	int jd_num(const vector<JD>& jd_vector) { return jd_vector.size(); }; // 交点个数
	int get_qdlc1() const { return qdlc1; };//获取起点里程+前部分
	int get_qdlc2() const { return qdlc2; };//获取起点里程+后部分
	double get_zdlc() const { return zdlc; };//获取终点里程
	bool is_qx_done() const { return qx_done; };//曲线计算完成
	bool is_lc_done() const { return lc_done; };//里程计算完成
	void set_qx_done(bool b) { this->qx_done = b; };//设置曲线计算是否完成
	void set_lc_done(bool b) { this->lc_done = b; };//设置里程计算是否完成
	void set_qdlc1(int qdlc1) { this->qdlc1 = qdlc1; };//设置qdlc1
	void set_qdlc2(int qdlc2) { this->qdlc2 = qdlc2; };//设置qdlc2
	void set_zdlc(double zdlc) { this->zdlc = zdlc; }//设置zdlc
	void clean_jd(vector<JD>& jd_vector) { jd_vector.clear(); }; //清除交点信息
	void clean_qx(vector<QX>& qx_vector) { qx_vector.clear(); };//清除曲线信息
	void clean_lc(vector<LC>& lc_vector) { lc_vector.clear(); };//清除里程信息
	vector <string> get_jd(const vector<JD>& jd_vector, int jd_index);//获取交点信息
	vector <string> get_quxian(const vector<QX>& qx_vector, int index);//获取曲线信息
	vector <string> get_licheng(const vector<LC>& lc_vector, int index);//获取里程信息
	double get_lc2(double rylc); //返回任意里程的后面部分
	Angle to_angle(double a);
	bool read_data(vector<JD>& jd_vector, string file_nmae); //读数据
	int read_data_from_db(vector<JD>& jd_vector, DB &db_info);
	bool compute_quxian(const vector<JD>& jd_vector, vector<QX>& qx_vector); //计算曲线信息
	bool compute_licheng(const vector<JD>& jd_vector, const vector<QX>& qx_vector, vector<LC>& lc_vector, int qdlc1, int qdlc2); //计算里程坐标
	bool show_licheng(const vector<LC>& lc_vector); //展示里程
	bool show_jd(const vector<JD>& jd_vector); //展示交点
	bool show_quxian(const vector<QX>& qx_vector);//展示曲线信息
	bool save_result(vector<JD>& jd_vector, const vector<QX>& qx_vector, const vector<LC>& lc_vector, string file_name); //保存所有信息
	int save_result_to_db(vector<JD>& jd_vector, const vector<QX>& qx_vector, const vector<LC>& lc_vector, DB &db); // 保存结果到数据库
	bool save_jd(const vector<JD>& jd_vector, string file_name); //保存交点信息
	int save_jd_to_db(const vector<JD>& jd_vector, DB& db); //保存交点到数据库
	void edit_jd(vector<JD>& jd_vector, int jd_index, int attribute_index, string value);//编辑交点
	ZB lc_to_zb(const vector<JD>& jd_vector, const vector<QX>& qx_vector, const vector<LC>& lc_vector, int rylc); //里程转坐标


private:
	bool qx_done; //曲线计算完成
	bool lc_done; //里程计算完成
	int qdlc1;//起点里程+号前面部分
	int qdlc2;//起点里程+号后面部分
	double zdlc; //终点里程

};


string get_formal_licheng(int lc);//调整里程格式DKx+xxx
string fill_zero(int lc); //按位数补零
#endif PATH_H