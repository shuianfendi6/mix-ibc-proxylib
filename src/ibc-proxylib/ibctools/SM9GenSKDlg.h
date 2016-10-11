#pragma once
#include "afxwin.h"


// CSM9GenSKDlg dialog

class CSM9GenSKDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSM9GenSKDlg)

public:
	CSM9GenSKDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSM9GenSKDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SM9GEN_SK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClicked3();
	afx_msg void OnBnClicked2();
	CEdit m_edithid01;
	CEdit m_edithid02;
	CEdit m_edithid03;
	CEdit m_editSK;
};
