#pragma once
#include "afxwin.h"


// CSM9GenMPKDlg dialog

class CSM9GenMPKDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSM9GenMPKDlg)

public:
	CSM9GenMPKDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSM9GenMPKDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SM9GEN_MPK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_editG1;
	CEdit m_editG2;
	CEdit m_editMPK;
	afx_msg void OnBnClicked3();
	afx_msg void OnBnClicked2();
};
