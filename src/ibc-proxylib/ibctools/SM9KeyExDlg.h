#pragma once
#include "afxwin.h"


// CSM9KeyExDlg �Ի���

class CSM9KeyExDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSM9KeyExDlg)

public:
	CSM9KeyExDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSM9KeyExDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_SM9KEY_EX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClicked4();
	CEdit m_editG1;
	CEdit m_editIDA;
	CEdit m_editIDB;
	CEdit m_editSK;
	CEdit m_editr;
	CEdit m_editRA;
	CEdit m_editRB;
	CEdit m_editSKAorSKB;
	CEdit m_editkey_len;
	CEdit m_editSBorS1;
	CEdit m_editS2orSA;
};
