// SaveOptionDlg.cpp : implementation file
//

#include "StdAfx.h"
#include "SaveOptionDlg.h"
#include "afxdialogex.h"
#include "resource.h"


// SaveOptionDlg dialog

IMPLEMENT_DYNAMIC(SaveOptionDlg, CAcUiDialog)

SaveOptionDlg::SaveOptionDlg(CWnd* pParent /*=nullptr*/)
	: CAcUiDialog(IDD_DIALOG2, pParent)
{

}

SaveOptionDlg::~SaveOptionDlg()
{
}

void SaveOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(SaveOptionDlg, CAcUiDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &SaveOptionDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &SaveOptionDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// SaveOptionDlg message handlers


void SaveOptionDlg::OnBnClickedButton2()
{
	is_file = false;
	OnOK();
}


void SaveOptionDlg::OnBnClickedButton1()
{
	is_file = true;
	OnOK();
}
