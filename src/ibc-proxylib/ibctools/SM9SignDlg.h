#pragma once
#include "afxwin.h"


// CSM9SignDlg dialog

class CSM9SignDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSM9SignDlg)

public:
	CSM9SignDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSM9SignDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SM9SIGN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_editIn;
	CEdit m_editOut;
	afx_msg void OnBnClicked4();
	CEdit m_edithid01;
	CEdit m_editG2;
};
