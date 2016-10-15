#pragma once
#include "afxwin.h"


// CSM9KeyExPreDlg 对话框

class CSM9KeyExPreDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSM9KeyExPreDlg)

public:
	CSM9KeyExPreDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSM9KeyExPreDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_SM9KEY_EX_PRE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClicked4();
	CEdit m_editG1;
	CEdit m_editID;
	CEdit m_editr;
	CEdit m_editR;
};
