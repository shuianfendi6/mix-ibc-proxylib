#pragma once
#include "afxwin.h"


// CSM9KeyExPreDlg �Ի���

class CSM9KeyExPreDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSM9KeyExPreDlg)

public:
	CSM9KeyExPreDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSM9KeyExPreDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_SM9KEY_EX_PRE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClicked4();
	CEdit m_editG1;
	CEdit m_editID;
	CEdit m_editr;
	CEdit m_editR;
};
