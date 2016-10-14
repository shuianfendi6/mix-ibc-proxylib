#pragma once
#include "afxwin.h"


// CSM9DecryptDlg dialog

class CSM9DecryptDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSM9DecryptDlg)

public:
	CSM9DecryptDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSM9DecryptDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SM9DECRYPT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_editIn;
	CEdit m_editOut;
	CEdit m_editID;
	afx_msg void OnBnClicked4();
	CEdit m_editG1;
	CEdit m_edithid03;
};
