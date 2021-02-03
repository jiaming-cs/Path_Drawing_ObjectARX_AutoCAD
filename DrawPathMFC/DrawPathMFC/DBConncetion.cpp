// DBConncetion.cpp : implementation file
//

#include "StdAfx.h"
#include "DBConncetion.h"
#include "afxdialogex.h"
#include "resource.h"

// DBConncetion dialog

IMPLEMENT_DYNAMIC(DBConncetion, CAcUiDialog)

DBConncetion::DBConncetion(CWnd* pParent /*=nullptr*/)
	: CAcUiDialog(IDD_DIALOG3, pParent)
{

}

DBConncetion::~DBConncetion()
{
}

void DBConncetion::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, EDIT_IP, edit_ip);
	DDX_Control(pDX, EDIT_PORT, edit_port);
	DDX_Control(pDX, EDIT_USER, edit_user);
	DDX_Control(pDX, EDIT_PASSWD, edit_passwd);
	DDX_Control(pDX, EDIT_DB, edit_db);
	DDX_Control(pDX, EDIT_TABLE, edit_table);
}


BEGIN_MESSAGE_MAP(DBConncetion, CAcUiDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &DBConncetion::OnClickedButton1)
END_MESSAGE_MAP()


// DBConncetion message handlers


BOOL DBConncetion::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();
	edit_ip.SetWindowTextW(L"127.0.0.1");
	edit_port.SetWindowTextW(L"3306");
	edit_user.SetWindowTextW(L"root");
	edit_db.SetWindowTextW(L"my_db");

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void DBConncetion::OnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CAcUiDialog::OnOK();
	edit_ip.GetWindowTextW(ip);
	edit_port.GetWindowTextW(port);
	edit_user.GetWindowTextW(user);
	edit_passwd.GetWindowTextW(passwd);
	edit_db.GetWindowTextW(db);
	edit_table.GetWindowTextW(table);
}
