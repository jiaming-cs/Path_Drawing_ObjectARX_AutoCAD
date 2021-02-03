// QuxianDialog.cpp : implementation file
//

#include "StdAfx.h"
#include "QuxianDialog.h"
#include "afxdialogex.h"
#include "resource.h"

// QuxianDialog dialog

IMPLEMENT_DYNAMIC(QuxianDialog, CAcUiDialog)

QuxianDialog::QuxianDialog(CWnd* pParent /*=nullptr*/)
	: CAcUiDialog(IDD_DIALOG5, pParent)
{

}

QuxianDialog::~QuxianDialog()
{
}

void QuxianDialog::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_QUXIAN, quxian_list);
}


BEGIN_MESSAGE_MAP(QuxianDialog, CAcUiDialog)
END_MESSAGE_MAP()


// QuxianDialog message handlers


BOOL QuxianDialog::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	long dwStyle = quxian_list.GetExtendedStyle();//扩展属性
	dwStyle |= LVS_EX_FULLROWSELECT;//选择整行
	dwStyle |= LVS_EX_GRIDLINES;//绘制表格
	quxian_list.SetExtendedStyle(dwStyle);//设置扩展样式

	quxian_list.InsertColumn(0, L"交点号", LVCFMT_LEFT, 50);
	quxian_list.InsertColumn(1, L"方位角（弧度）", LVCFMT_LEFT, 100);
	quxian_list.InsertColumn(2, L"曲线转角（角度）", LVCFMT_LEFT, 110);
	quxian_list.InsertColumn(3, L"缓和曲线长", LVCFMT_LEFT, 80);
	quxian_list.InsertColumn(4, L"曲线长", LVCFMT_LEFT, 50);
	quxian_list.InsertColumn(5, L"曲线半径", LVCFMT_LEFT, 70);
	quxian_list.InsertColumn(6, L"切垂距", LVCFMT_LEFT, 50);
	quxian_list.InsertColumn(7, L"内移距", LVCFMT_LEFT, 50);
	quxian_list.InsertColumn(8, L"切线长", LVCFMT_LEFT, 50);
	quxian_list.InsertColumn(9, L"外矢距", LVCFMT_LEFT, 50);
	quxian_list.InsertColumn(10, L"圆曲线长", LVCFMT_LEFT, 70);
	quxian_list.InsertColumn(11, L"缓和曲线角", LVCFMT_LEFT, 80);
	quxian_list.InsertColumn(12, L"切曲差", LVCFMT_LEFT, 50);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
