#pragma once
#include "afxwin.h"


// CSM9GenDlg dialog

class CSM9GenDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSM9GenDlg)

public:
	CSM9GenDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSM9GenDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SM9GEN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	
	CEdit m_editmsk;
	CEdit m_editG2;
	CEdit m_editG1;
};
