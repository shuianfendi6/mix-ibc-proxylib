#pragma once
#include "afxwin.h"


// CSM9WrapDlg dialog

class CSM9WrapDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSM9WrapDlg)

public:
	CSM9WrapDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSM9WrapDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SM9WRAP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_editG1;
	CEdit m_editID;
	CEdit m_editIn;
	CEdit m_editOut;
	afx_msg void OnBnClickedOkWrap();
};
