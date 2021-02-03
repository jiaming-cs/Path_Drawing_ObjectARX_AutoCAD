#pragma once


// QuxianDialog dialog

class QuxianDialog : public CAcUiDialog
{
	DECLARE_DYNAMIC(QuxianDialog)

public:
	QuxianDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~QuxianDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG5 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl quxian_list;
	virtual BOOL OnInitDialog();
};
