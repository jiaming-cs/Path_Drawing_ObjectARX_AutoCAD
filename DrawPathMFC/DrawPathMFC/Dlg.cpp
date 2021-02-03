// Dlg.cpp : implementation file
//

#include "StdAfx.h"
#include "Dlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include "SaveOptionDlg.h"
#include "DBConncetion.h"
#include "Path.h"
#include "QuxianDialog.h"
#include "Dialog_Licheng.h"

string cstring_to_string(CString input) {
	CT2CA temp(input);
	string output(temp);
	return output;
}

CString string_to_csting(string input) {
	return CString(input.c_str());
}
IMPLEMENT_DYNAMIC(Dlg, CAcUiDialog)
int row, col;
Dlg::Dlg(CWnd* pParent /*=nullptr*/)
	: CAcUiDialog(IDD_DIALOG1, pParent)
{

}

Dlg::~Dlg()
{
}

void Dlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, jd_list);
	DDX_Control(pDX, IDC_EDIT1, input_edit);
	DDX_Control(pDX, IDC_EDIT2, qdlc_edit);
}


BEGIN_MESSAGE_MAP(Dlg, CAcUiDialog)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &Dlg::OnDblclkList1)
//	ON_NOTIFY(NM_KILLFOCUS, IDC_LIST1, &Dlg::OnKillfocusList1)
	ON_BN_CLICKED(IDC_BUTTON1, &Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &Dlg::OnBnClickedButton2)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &Dlg::OnRclickList1)
	ON_BN_CLICKED(IDC_BUTTON3, &Dlg::OnBnClickedButton3)
	ON_COMMAND(IDR_MENU, &Dlg::OnIdrMenu)
	ON_BN_CLICKED(IDC_BUTTON4, &Dlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &Dlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &Dlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &Dlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &Dlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &Dlg::OnBnClickedButton9)
	
	ON_EN_KILLFOCUS(IDC_EDIT1, &Dlg::OnKillfocusEdit1)
END_MESSAGE_MAP()


// Dlg message handlers


BOOL Dlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	qxdlog = new QuxianDialog();
	lcdlog = new Dialog_Licheng();
	qdlc_edit.SetWindowTextW(string_to_csting(to_string(qdlc)));
	// TODO:  Add extra initialization here
	long dwStyle = jd_list.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	jd_list.SetExtendedStyle(dwStyle);

	jd_list.InsertColumn(0, L"交点号", LVCFMT_LEFT, 50);
	jd_list.InsertColumn(1, L"X坐标", LVCFMT_LEFT, 100);
	jd_list.InsertColumn(2, L"Y坐标", LVCFMT_LEFT, 100);
	jd_list.InsertColumn(3, L"半径R", LVCFMT_LEFT, 100);
	jd_list.InsertColumn(4, L"缓长L0", LVCFMT_LEFT, 100);
	for (int i = 0; i < path.jd_num(jd_vector); i++) {//列表行
		vector<string> jd_info = path.get_jd(jd_vector, i);//获取交点
		jd_list.InsertItem(i, string_to_csting(to_string(i + 1)));//插入新行
		jd_list.SetItemText(i, 1, string_to_csting(jd_info[1]));//x
		jd_list.SetItemText(i, 2, string_to_csting(jd_info[2]));//y
		jd_list.SetItemText(i, 3, string_to_csting(jd_info[3]));//r
		jd_list.SetItemText(i, 4, string_to_csting(jd_info[4]));//l0
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void Dlg::OnDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	CRect rc;//行矩形
	CString strTemp;
	NM_LISTVIEW *pNMListView = (NM_LISTVIEW *)pNMHDR;
	row = pNMListView->iItem;//行号
	col = pNMListView->iSubItem;//列号


	if (col != 0)	// 选择子项，不可以修改交点号
	{
		jd_list.GetSubItemRect(row, col, LVIR_LABEL, rc); //得到双击所得的位置
		input_edit.SetParent(&jd_list);//设置子窗体edit的父窗体(固定)
		input_edit.MoveWindow(rc);//改变窗口大小位置
		strTemp = jd_list.GetItemText(row, col);
		input_edit.SetWindowTextW(strTemp);//获取数据传递给控件
		input_edit.ShowWindow(SW_SHOW);//显示窗口
		input_edit.SetFocus();//设置Edit交点
		input_edit.ShowCaret();//显示光标
		input_edit.SetSel(0, -1);//全选
	}
	*pResult = 0;
}





void Dlg::OnBnClickedButton1()
{
	SaveOptionDlg import_dlg;
	INT_PTR select = import_dlg.DoModal();

	if (select == IDOK) {
		if (import_dlg.is_file) {
			CFileDialog   dlg(TRUE);//打开对话框
			if (dlg.DoModal() != IDOK)//不按OK返回
				return;
			CString file_name_cstring = dlg.GetPathName();//cstring转string
			CT2CA temp(file_name_cstring);
			string file_name(temp);

			if (path.read_data(jd_vector, file_name)) { // 如果成功导入文件

				for (int i = jd_list.GetItemCount(); i < path.jd_num(jd_vector); i++) {//列表行
					vector<string> jd_info = path.get_jd(jd_vector, i);//获取交点
					jd_list.InsertItem(i, string_to_csting(to_string(i + 1)));//插入新行
					jd_list.SetItemText(i, 1, string_to_csting(jd_info[1]));//x
					jd_list.SetItemText(i, 2, string_to_csting(jd_info[2]));//y
					jd_list.SetItemText(i, 3, string_to_csting(jd_info[3]));//r
					jd_list.SetItemText(i, 4, string_to_csting(jd_info[4]));//l0
				}

				MessageBox(L"成功导入文件！", L"提示");
			}
			else {

				MessageBox(L"不能打开文件：" + file_name_cstring, L"提示");
			}
		}
		else {
			DBConncetion db_conn;
			if (db_conn.DoModal() == IDOK) {
				DB db_info;
				db_info.ip = cstring_to_string(db_conn.ip);
				db_info.port = cstring_to_string(db_conn.port);
				db_info.user = cstring_to_string(db_conn.user);
				db_info.passwd = cstring_to_string(db_conn.passwd);
				db_info.db = cstring_to_string(db_conn.db);
				db_info.table = cstring_to_string(db_conn.table);
				int status = path.read_data_from_db(jd_vector, db_info);
				if (status == 0) {
					for (int i = jd_list.GetItemCount(); i < path.jd_num(jd_vector); i++) {//列表行
						vector<string> jd_info = path.get_jd(jd_vector, i);//获取交点
						jd_list.InsertItem(i, string_to_csting(to_string(i + 1)));//插入新行
						jd_list.SetItemText(i, 1, string_to_csting(jd_info[1]));//x
						jd_list.SetItemText(i, 2, string_to_csting(jd_info[2]));//y
						jd_list.SetItemText(i, 3, string_to_csting(jd_info[3]));//r
						jd_list.SetItemText(i, 4, string_to_csting(jd_info[4]));//l0
					}
					MessageBox(L"成功读取交点！");
				}
				else if (status == 1) {
					MessageBox(L"当前表没有可导入的交点！");
				}
				else if (status == 2) {
					MessageBox(L"数据库连接失败，请检查输入信息！");
				}
				else {
					MessageBox(L"此表不存在！");
				}

			}
			else {
				return;
			}

		}


	}

}


void Dlg::OnBnClickedButton2()
{
	CString str_temp;
	row = jd_list.GetItemCount();//行
	str_temp.Format(_T("%d"), row + 1);//行+1
	JD jd;
	jd_vector.push_back(jd);
	path.set_qx_done(false);//曲线计算未完成
	path.set_lc_done(false);//里程计算未完成
	jd_list.InsertItem(row, str_temp);//添新行
	jd_list.SetItemText(row, 1, string_to_csting("0"));//x 默认0
	jd_list.SetItemText(row, 2, string_to_csting("0"));//y
	jd_list.SetItemText(row, 3, string_to_csting("0"));//r
	jd_list.SetItemText(row, 4, string_to_csting("0"));//l0
	jd_list.SetItemState(row, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);//每一行 选择|不选择 状态
	jd_list.EnsureVisible(row, FALSE);//定位行可视
}


void Dlg::OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR; //list-view 消息的结构体
	if (pNMListView->iItem != -1)//点击如果有内容 返回行号
	{
		row = pNMListView->iItem;//行号
		DWORD dwPos = GetMessagePos(); //返回鼠标位置 DWORD，低16位为x, 高16位y
		CPoint Point(LOWORD(dwPos), HIWORD(dwPos));
		CMenu Menu;
		Menu.LoadMenu(IDR_MENU1);  //你要弹的菜单ID 
		CMenu* Popup = Menu.GetSubMenu(0); //菜单子项
		ASSERT(Popup != NULL);
		Popup->TrackPopupMenu(TPM_CENTERALIGN | TPM_RIGHTBUTTON, Point.x, Point.y, this); //设置菜单水平，用户可以选择菜单的选项左右按键
		Popup->Detach();
	}

	*pResult = 0;
}


void Dlg::OnBnClickedButton3()
{
	CString qdlc_cstring;
	GetDlgItem(IDC_EDIT2)->GetWindowText(qdlc_cstring);//把编辑框1输入的值赋给qdlc
	qdlc = strtol(cstring_to_string(qdlc_cstring).c_str(), NULL, 10);
	string qdlc_string = cstring_to_string(qdlc_cstring);
	long qdlc_int = strtol(qdlc_string.c_str(), NULL, 10);//string转cstring//NULL不返回非法字符串//base为10（合法字符为012...9）
	int qdlc_1 = qdlc_int / 1000;//里程+号前面部分
	int qdlc_2 = qdlc_int % 1000;//里程+号后面部分
	path.set_qdlc1(qdlc_1);//设置qdlc_1为qdlc1
	path.set_qdlc2(qdlc_2);//设置qdlc_2为qdlc2
	path.clean_lc(lc_vector);//清除里程信息
	path.set_lc_done(false);//里程坐标计算设置为未完成状态
	lcdlog->DestroyWindow();//里程对话框销毁
	string info = get_formal_licheng(qdlc_1 * 1000 + qdlc_2);//起始里程
	MessageBox(L"起点里程设置为: " + string_to_csting(info), L"起点里程");
}


void Dlg::OnIdrMenu()
{
	for (int i = row; i < jd_vector.size(); i++) {
		jd_list.DeleteItem(row);//删除列表行
	}
	jd_vector.erase(jd_vector.begin() + row);//删除数据
	for (int i = row; i < jd_vector.size(); i++) {
		vector<string> jd_info = path.get_jd(jd_vector, i);//获取交点
		jd_list.InsertItem(i, string_to_csting(to_string(i + 1)));//序号
		jd_list.SetItemText(i, 1, string_to_csting(jd_info[1]));//x
		jd_list.SetItemText(i, 2, string_to_csting(jd_info[2]));//y
		jd_list.SetItemText(i, 3, string_to_csting(jd_info[3]));//r
		jd_list.SetItemText(i, 4, string_to_csting(jd_info[4]));//l0
	}
	path.set_qx_done(false);//曲线计算设置为未完成状态
	path.set_lc_done(false);//里程坐标计算设置为未完成状态
}


void Dlg::OnBnClickedButton4()
{
	if (!qxdlog->GetSafeHwnd()) {//安全获取窗口
		qxdlog->Create(IDD_DIALOG5);//创建曲线信息对话框
	}

	if (path.jd_num(jd_vector) >= 3) { //如果有至少三个交点
		if (!path.is_qx_done()) { // 如果还没有计算曲线
			qxdlog->quxian_list.DeleteAllItems();//清空曲线列表
			path.compute_quxian(jd_vector, qx_vector);//计算曲线信息
			for (int i = 0; i < path.jd_num(jd_vector); i++) {//行
				vector<string> qx_info = path.get_quxian(qx_vector, i);
				qxdlog->quxian_list.InsertItem(i, string_to_csting(to_string(i)));
				for (int j = 0; j < 12; j++) {//列
					qxdlog->quxian_list.SetItemText(i, j + 1, string_to_csting(qx_info[j + 1]));
				}
			}
		}
		else {
			if (qxdlog->quxian_list.GetItemCount() != path.jd_num(jd_vector)) {
				for (int i = 0; i < path.jd_num(jd_vector); i++) {//行
					vector<string> qx_info = path.get_quxian(qx_vector, i);
					qxdlog->quxian_list.InsertItem(i, string_to_csting(to_string(i)));
					for (int j = 0; j < 12; j++) {//列
						qxdlog->quxian_list.SetItemText(i, j + 1, string_to_csting(qx_info[j + 1]));
					}
				}
			}
			qxdlog->ShowWindow(SW_SHOWNORMAL);//展示
		}
	}
	else
	{
		MessageBox(L"请导入至少3个交点!", L"提示");
	}
}


void Dlg::OnBnClickedButton5()
{
	if (!path.is_qx_done()) {//未完成曲线计算
		MessageBox(L"请先计算曲线信息", L"提示");
		return;
	}
	if (!lcdlog->GetSafeHwnd()) {
		lcdlog->Create(IDD_DIALOG4);//创建里程坐标对话框
	}

	if (!path.is_lc_done()) {//未完成里程坐标计算
		lcdlog->list_licheng.DeleteAllItems();//里程坐标信息清空
		path.compute_licheng(jd_vector, qx_vector, lc_vector, path.get_qdlc1(), path.get_qdlc2());//计算里程坐标
		for (int i = 0; i < path.jd_num(jd_vector); i++) {//行
			vector<string> lc_info = path.get_licheng(lc_vector, i);
			lcdlog->list_licheng.InsertItem(i, string_to_csting(to_string(i)));
			for (int j = 0; j < 10; j++) {//列
				lcdlog->list_licheng.SetItemText(i, j + 1, string_to_csting(lc_info[j + 1]));
			}
		}
		lcdlog->list_licheng.InsertItem(path.jd_num(jd_vector), L"起点里程");
		lcdlog->list_licheng.SetItemText(path.jd_num(jd_vector), 1, string_to_csting(get_formal_licheng(path.get_qdlc1() * 1000 + path.get_qdlc2())));
		lcdlog->list_licheng.InsertItem(path.jd_num(jd_vector) + 1, L"终点里程");
		lcdlog->list_licheng.SetItemText(path.jd_num(jd_vector) + 1, 1, string_to_csting(get_formal_licheng(path.get_zdlc())));
	}
	else {
	
		if (lcdlog->list_licheng.GetItemCount() != path.jd_num(jd_vector)) {
			for (int i = 0; i < path.jd_num(jd_vector); i++) {//行
				vector<string> lc_info = path.get_licheng(lc_vector, i);
				lcdlog->list_licheng.InsertItem(i, string_to_csting(to_string(i)));
				for (int j = 0; j < 10; j++) {//列
					lcdlog->list_licheng.SetItemText(i, j + 1, string_to_csting(lc_info[j + 1]));
				}
			}
			lcdlog->list_licheng.InsertItem(path.jd_num(jd_vector), L"起点里程");
			lcdlog->list_licheng.SetItemText(path.jd_num(jd_vector), 1, string_to_csting(get_formal_licheng(path.get_qdlc1() * 1000 + path.get_qdlc2())));
			lcdlog->list_licheng.InsertItem(path.jd_num(jd_vector) + 1, L"终点里程");
			lcdlog->list_licheng.SetItemText(path.jd_num(jd_vector) + 1, 1, string_to_csting(get_formal_licheng(path.get_zdlc())));
		}
	}

	lcdlog->ShowWindow(SW_SHOWNORMAL);
}


void Dlg::OnBnClickedButton6()
{
	SaveOptionDlg save_option_dlg;
	INT_PTR slection = save_option_dlg.DoModal();
	if (slection == IDOK) {
		if (save_option_dlg.is_file) { // 如果选择使用文件系统
			BOOL IsOpen = false;     //是否打开(否则为保存)    
			CString DefaultType = CString("*.csv");   //默认文件类型    
			CString DefaultfileName = CString("Result.csv");         //默认文件名    
			CString TypesFilter = CString("Save as(*.csv)|*.csv||");   //文件过虑的类型    
			CFileDialog openFileDlg(IsOpen, DefaultType, DefaultfileName, OFN_HIDEREADONLY | OFN_READONLY, TypesFilter, NULL);
			if (openFileDlg.DoModal() == IDOK) {//按ok执行代码

				string file_name = cstring_to_string(openFileDlg.GetPathName()); //读取文件名称
				string file_path(file_name);
				if (path.save_result(jd_vector, qx_vector, lc_vector, file_path)) {
					MessageBox(L"成功保存计算结果！", L"提示");
				}
				else {
					MessageBox(L"请先设置里程！", L"提示");
				}
			}
		}
		else { //如果使用数据库系统
			DBConncetion db_conn;
			if (db_conn.DoModal() == IDOK) {
				DB db_info;
				db_info.ip = cstring_to_string(db_conn.ip);
				db_info.port = cstring_to_string(db_conn.port);
				db_info.user = cstring_to_string(db_conn.user);
				db_info.passwd = cstring_to_string(db_conn.passwd);
				db_info.db = cstring_to_string(db_conn.db);
				db_info.table = cstring_to_string(db_conn.table);
				int status = path.save_result_to_db(jd_vector, qx_vector, lc_vector, db_info);
				if (status == 0) {
					MessageBox(L"成功导入交点至数据库！");
				}
				else if (status == 1) {
					MessageBox(L"请先计算曲线和里程信息！");
				}
				else {
					MessageBox(L"连接数据库失败，请检查输入信息！");
				}
			}
			else {
				return;
			}
		}

	}


	else { //否则返回
		return;
	}

}




void Dlg::OnBnClickedButton7()
{
	SaveOptionDlg save_dlg;
	INT_PTR select = save_dlg.DoModal();

	if (select == IDOK) {

		if (save_dlg.is_file) {

			string file_name;  //读取的文件名称
			BOOL IsOpen = false;     //是否打开(否则为保存)    
			CString DefaultType = CString("*.csv");   //默认文件类型    
			CString DefaultfileName = CString("JD.csv");         //默认文件名    
			CString TypesFilter = CString("Save as(*.csv)|*.csv||");   //文件过虑的类型    
			CFileDialog openFileDlg(IsOpen, DefaultType, DefaultfileName, OFN_HIDEREADONLY | OFN_READONLY, TypesFilter, NULL);
			if (openFileDlg.DoModal() == IDOK) {//按ok执行代码
				file_name = cstring_to_string(openFileDlg.GetPathName());

				if (path.save_jd(jd_vector, file_name)) {
					MessageBox(L"成功保存交点！", L"提示");
				}
				else {
					MessageBox(L"请先设置里程！", L"提示");
				}
			}

		}

		else {
			DBConncetion db_conn;
			if (db_conn.DoModal() == IDOK) {
				DB db_info;
				db_info.ip = cstring_to_string(db_conn.ip);
				db_info.port = cstring_to_string(db_conn.port);
				db_info.user = cstring_to_string(db_conn.user);
				db_info.passwd = cstring_to_string(db_conn.passwd);
				db_info.db = cstring_to_string(db_conn.db);
				db_info.table = cstring_to_string(db_conn.table);
				int status = path.save_jd_to_db(jd_vector, db_info);
				if (status == 0) {
					MessageBox(L"成功导入交点至数据库！");
				}
				else if (status == 1) {
					MessageBox(L"没有可导入的交点！");
				}
				else {
					MessageBox(L"导入失败，请检查输入信息！");
				}
			}
			else {
				return;
			}
		}
	}
	else {//不按ok返回
		return;
	}

}


void Dlg::OnBnClickedButton8()
{
	CString rylc_cstring;
	GetDlgItem(IDC_EDIT3)->GetWindowText(rylc_cstring);//把编辑框2输入的值赋给rylc
	CT2CA temp(rylc_cstring);//cstring转string
	string rylc_string(temp);
	int rylc = stoi(rylc_string);//string转int
	ZB zb = path.lc_to_zb(jd_vector, qx_vector, lc_vector, rylc);//path中计算里程转坐标
	if (zb.x == -1) {//返回-1
		MessageBox(L"请先计算里程信息！", L"提示");
	}
	else if (zb.x == -2) {//返回-2
		MessageBox(L"输入的里程不在起点与终点的区间！", L"提示");
	}
	else {
		string result = "任意里程:" + get_formal_licheng(rylc) + " 的坐标为: \n" + zb.show_zb();
		CString text = CString(result.c_str());//string转cstring
		GetDlgItem(IDC_STATIC)->SetWindowText(text);//result传送到static2
		GetDlgItem(IDC_STATIC)->ShowWindow(SW_SHOW);//在static2的位置以原尺寸显示
		MessageBox(text, L"里程转坐标");
	}
}


void Dlg::OnBnClickedButton9()
{
	if (jd_vector.size() >= 3) {
		CString qdlc_cstring;
		GetDlgItem(IDC_EDIT2)->GetWindowText(qdlc_cstring);//把编辑框1输入的值赋给qdlc
		qdlc = strtol(cstring_to_string(qdlc_cstring).c_str(), NULL, 10);
		string qdlc_string = cstring_to_string(qdlc_cstring);
		long qdlc_int = strtol(qdlc_string.c_str(), NULL, 10);//string转cstring//NULL不返回非法字符串//base为10（合法字符为012...9）
		int qdlc_1 = qdlc_int / 1000;//里程+号前面部分
		int qdlc_2 = qdlc_int % 1000;//里程+号后面部分
		path.set_qdlc1(qdlc_1);//设置qdlc_1为qdlc1
		path.set_qdlc2(qdlc_2);//设置qdlc_2为qdlc2
		path.clean_lc(lc_vector);//清除里程信息
		path.set_lc_done(false);//里程坐标计算设置为未完成状态
		lcdlog->DestroyWindow();//里程对话框销毁
		string info = get_formal_licheng(qdlc_1 * 1000 + qdlc_2);//起始里程
		path.compute_quxian(jd_vector, qx_vector);
		path.compute_licheng(jd_vector, qx_vector, lc_vector, path.get_qdlc1(), path.get_qdlc2());
		CAcUiDialog::OnOK();
	}
	else {
		MessageBox(L"请至少添加3个交点", L"提示");
	}
	
}



void Dlg::OnKillfocusEdit1()
{
	CString str;
	input_edit.GetWindowTextW(str);//获取数据
	if (str.IsEmpty()) {
		input_edit.SetWindowTextW(L"0");
		path.edit_jd(jd_vector, row, col, 0);//编辑更新交点信息
		path.clean_qx(qx_vector);//清除曲线信息
		path.clean_lc(lc_vector);//清除里程坐标
		path.set_lc_done(false);
		path.set_qx_done(false);
		return ;
	}
	jd_list.SetItemText(row, col, str);//设置数据给列表中x行x列
	input_edit.ShowWindow(SW_HIDE);//隐藏窗口
	if (!str.IsEmpty()) {
		path.edit_jd(jd_vector, row, col, cstring_to_string(str));//编辑更新交点信息
		path.clean_qx(qx_vector);//清除曲线信息
		path.clean_lc(lc_vector);//清除里程坐标
		path.set_lc_done(false);
		path.set_qx_done(false);
	}
}
