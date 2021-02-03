#pragma once

#include "resource.h"
#include <dbsymtb.h>
#include <dbapserv.h>
#include <adslib.h>
#include <adui.h>
#include <acui.h>
#include "QuxianDialog.h"
#include "Dialog_Licheng.h"
#include "Path.h"
// Dlg dialog

class Dlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(Dlg)

public:
	Dlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~Dlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl jd_list;
	virtual BOOL OnInitDialog();
	CEdit input_edit;
	Path path;
	vector<JD> jd_vector;
	vector<QX> qx_vector;
	vector<LC> lc_vector;
	QuxianDialog *qxdlog;
	Dialog_Licheng *lcdlog;  
	int qdlc = 0;
	afx_msg void OnDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnKillfocusList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton3();
	afx_msg void OnIdrMenu();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	CEdit qdlc_edit;
	afx_msg void OnKillfocusEdit1();
};
