#pragma once


// CSM9VerifyDlg dialog

class CSM9VerifyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSM9VerifyDlg)

public:
	CSM9VerifyDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSM9VerifyDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SM9VERIFY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
