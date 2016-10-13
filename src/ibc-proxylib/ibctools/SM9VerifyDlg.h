#pragma once
#include "afxwin.h"


// CSM9VerifyDlg dialog

class CSM9VerifyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSM9VerifyDlg)

public:
	CSM9VerifyDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSM9VerifyDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SM9VERIFY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_editIn;
	CEdit m_editOut;
	afx_msg void OnBnClicked2();
	afx_msg void OnBnClicked3();
};
