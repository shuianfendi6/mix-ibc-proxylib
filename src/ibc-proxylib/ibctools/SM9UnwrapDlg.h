#pragma once
#include "afxwin.h"


// CSM9UnwrapDlg dialog

class CSM9UnwrapDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSM9UnwrapDlg)

public:
	CSM9UnwrapDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSM9UnwrapDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SM9UNWRAP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClicked4();
	CEdit m_editG1;
	CEdit m_edithid03;
	CEdit m_editID;
	CEdit m_editIn;
	CEdit m_editOut;
};
