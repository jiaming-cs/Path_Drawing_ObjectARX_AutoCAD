// Dialog_Licheng.cpp : implementation file
//

#include "StdAfx.h"
#include "Dialog_Licheng.h"
#include "afxdialogex.h"
#include "resource.h"

// Dialog_Licheng dialog

IMPLEMENT_DYNAMIC(Dialog_Licheng, CAcUiDialog)

Dialog_Licheng::Dialog_Licheng(CWnd* pParent /*=nullptr*/)
	: CAcUiDialog(IDD_DIALOG4, pParent)
{

}

Dialog_Licheng::~Dialog_Licheng()
{
}

void Dialog_Licheng::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LICHENG, list_licheng);
}


BEGIN_MESSAGE_MAP(Dialog_Licheng, CAcUiDialog)
END_MESSAGE_MAP()


// Dialog_Licheng message handlers


BOOL Dialog_Licheng::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	long dwStyle = list_licheng.GetExtendedStyle();//扩展属性
	dwStyle |= LVS_EX_FULLROWSELECT;//选择整行
	dwStyle |= LVS_EX_GRIDLINES;//绘制表格
	list_licheng.SetExtendedStyle(dwStyle);//设置扩展样式

	list_licheng.InsertColumn(0, L"交点号", LVCFMT_LEFT, 80);
	list_licheng.InsertColumn(1, L"直缓点里程", LVCFMT_LEFT, 100);
	list_licheng.InsertColumn(2, L"直缓点坐标", LVCFMT_LEFT, 150);
	list_licheng.InsertColumn(3, L"缓圆点里程", LVCFMT_LEFT, 100);
	list_licheng.InsertColumn(4, L"缓圆点坐标", LVCFMT_LEFT, 150);
	list_licheng.InsertColumn(5, L"曲中点里程", LVCFMT_LEFT, 100);
	list_licheng.InsertColumn(6, L"曲中点坐标", LVCFMT_LEFT, 150);
	list_licheng.InsertColumn(7, L"圆缓点里程", LVCFMT_LEFT, 100);
	list_licheng.InsertColumn(8, L"圆缓点坐标", LVCFMT_LEFT, 150);
	list_licheng.InsertColumn(9, L"缓直点里程", LVCFMT_LEFT, 100);
	list_licheng.InsertColumn(10, L"缓直点坐标", LVCFMT_LEFT, 150);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
