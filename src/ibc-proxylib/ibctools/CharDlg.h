#pragma once


// CCharDlg dialog

class CCharDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCharDlg)

public:
	CCharDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCharDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_CHAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
