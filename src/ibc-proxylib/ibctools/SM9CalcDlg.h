#pragma once
#include "afxwin.h"


// CSM9CalcDlg dialog

class CSM9CalcDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSM9CalcDlg)

public:
	CSM9CalcDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSM9CalcDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SM9CALC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_editID;
	CEdit m_editPrv;
	afx_msg void OnBnClicked3();
	afx_msg void OnBnClicked2();
};
