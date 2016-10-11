
// ibctoolsDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CibctoolsDlg dialog
class CibctoolsDlg : public CDialogEx
{
// Construction
public:
	CibctoolsDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_IBCTOOLS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTcnSelchangeTabAll(NMHDR *pNMHDR, LRESULT *pResult);
	CTabCtrl m_tb;

	int m_CurSelTab;
	CDialog * pDialog[9];
	int pos;
	CComboBox m_comboCryptoMode;
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeCombo3();
};
#pragma once
