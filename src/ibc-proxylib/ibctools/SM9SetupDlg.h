#pragma once
#include "afxwin.h"


// CSM9SetupDlg dialog

class CSM9SetupDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSM9SetupDlg)

public:
	CSM9SetupDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSM9SetupDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SM9SETUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CEdit m_editPrv;
	CEdit m_editPub;
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnBnClicked2();
};
