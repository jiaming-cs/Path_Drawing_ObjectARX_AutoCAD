#pragma once


// SaveOptionDlg dialog

class SaveOptionDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(SaveOptionDlg)

public:
	SaveOptionDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~SaveOptionDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	bool is_file;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
};
