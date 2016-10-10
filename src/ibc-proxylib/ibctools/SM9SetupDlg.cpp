// SM9SetupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ibctools.h"
#include "SM9SetupDlg.h"
#include "afxdialogex.h"

#include "sm9_proxylib_api.h"

extern void *g_gParams;
extern void *g_msk;
extern void *g_mpk;
extern void *g_sk;


// CSM9SetupDlg dialog

IMPLEMENT_DYNAMIC(CSM9SetupDlg, CDialogEx)

CSM9SetupDlg::CSM9SetupDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSM9SetupDlg::IDD, pParent)
{

}

CSM9SetupDlg::~CSM9SetupDlg()
{
}

void CSM9SetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_editPrv);
	DDX_Control(pDX, IDC_EDIT1, m_editPub);
}


BEGIN_MESSAGE_MAP(CSM9SetupDlg, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT1, &CSM9SetupDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDOK, &CSM9SetupDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSM9SetupDlg::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_EDIT2, &CSM9SetupDlg::OnEnChangeEdit2)
	ON_BN_CLICKED(2, &CSM9SetupDlg::OnBnClicked2)
END_MESSAGE_MAP()


// CSM9SetupDlg message handlers


void CSM9SetupDlg::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CSM9SetupDlg::OnBnClickedOk()
{

}


void CSM9SetupDlg::OnBnClickedCancel()
{

}


void CSM9SetupDlg::OnEnChangeEdit2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CSM9SetupDlg::OnBnClicked2()
{
	char data_value[4096] = {0};
	int data_len = 4096;

	void *gParams = 0;
	void *msk = 0;
	void *mpk = 0;

	sm9_proxylib_generateParams(&gParams,SM9_SCHEME_SW);

	m_editPrv.GetWindowText(data_value,data_len);
	data_len = strlen(data_value);


	if (g_msk)
	{
		sm9_proxylib_destroyObject(g_msk);
		g_msk = 0;
	}

	if( 0 == sm9_proxylib_deserializeObject(data_value, data_len, &g_msk,SM9_SERIALIZE_HEXASCII))
	{
		MessageBox("设置主私钥成功！");
	}
	else
	{
		MessageBox("主私钥格式不正确，设置失败！");
		return;
	}

	data_len = 4096;

	m_editPub.GetWindowText(data_value,data_len);
	data_len = strlen(data_value);

	if (g_mpk)
	{
		sm9_proxylib_destroyObject(g_mpk);
		g_mpk = 0;
	}

	if(0 == sm9_proxylib_deserializeObject(data_value, data_len, &g_mpk,SM9_SERIALIZE_HEXASCII))
	{
		MessageBox("设置主公钥成功！");
	}
	else
	{
		MessageBox("主公钥格式不正确，设置失败！");
		return;
	}
}
