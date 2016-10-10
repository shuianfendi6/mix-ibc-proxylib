#pragma once


// CBase64Dlg dialog

class CBase64Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBase64Dlg)

public:
	CBase64Dlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBase64Dlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_B64 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
