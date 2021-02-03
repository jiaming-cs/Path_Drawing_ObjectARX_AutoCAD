#pragma once


// Dialog_Licheng dialog

class Dialog_Licheng : public CAcUiDialog
{
	DECLARE_DYNAMIC(Dialog_Licheng)

public:
	Dialog_Licheng(CWnd* pParent = nullptr);   // standard constructor
	virtual ~Dialog_Licheng();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl list_licheng;
};
