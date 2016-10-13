#pragma once
#include "afxwin.h"


// CSM9EncryptDlg dialog

class CSM9EncryptDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSM9EncryptDlg)

public:
	CSM9EncryptDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSM9EncryptDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SM9ENCRYPT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClicked2();
	CEdit m_editIn;
	CEdit m_editOut;
	afx_msg void OnBnClicked3();
};
