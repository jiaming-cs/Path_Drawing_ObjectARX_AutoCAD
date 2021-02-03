#pragma once


// DBConncetion dialog

class DBConncetion : public CAcUiDialog
{
	DECLARE_DYNAMIC(DBConncetion)

public:
	DBConncetion(CWnd* pParent = nullptr);   // standard constructor
	virtual ~DBConncetion();
	CEdit edit_ip;
	CEdit edit_port;
	CEdit edit_user;
	CEdit edit_passwd;
	CEdit edit_db;
	CEdit edit_table;
	CString ip;
	CString port;
	CString user;
	CString passwd;
	CString db;
	CString table;
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	afx_msg void OnClickedButton1();
};
