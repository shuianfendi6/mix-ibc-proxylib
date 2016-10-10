// SM9GenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ibctools.h"
#include "SM9GenDlg.h"
#include "afxdialogex.h"

#include "sm9_proxylib_api.h"

extern void *g_gParams;
extern void *g_msk;
extern void *g_mpk;
extern void *g_sk;

// CSM9GenDlg dialog

IMPLEMENT_DYNAMIC(CSM9GenDlg, CDialogEx)

CSM9GenDlg::CSM9GenDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSM9GenDlg::IDD, pParent)
{

}

CSM9GenDlg::~CSM9GenDlg()
{
}

void CSM9GenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_editPub);
	DDX_Control(pDX, IDC_EDIT2, m_editPrv);
}


BEGIN_MESSAGE_MAP(CSM9GenDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSM9GenDlg::OnBnClickedOk)
	ON_BN_CLICKED(2, &CSM9GenDlg::OnBnClicked2)
END_MESSAGE_MAP()


// CSM9GenDlg message handlers


void CSM9GenDlg::OnBnClickedOk()
{
	char data_value[4096] = {0};
	int data_len = 4096;

	void *gParams = 0;
	void *msk = 0;
	void *mpk = 0;

	sm9_proxylib_generateParams(&gParams,SM9_SCHEME_SW);
	data_len = 4096;
	sm9_proxylib_getSerializeObjectSize(gParams, SM9_SERIALIZE_HEXASCII, &data_len);
	sm9_proxylib_serializeObject(gParams,data_value, &data_len, data_len, SM9_SERIALIZE_HEXASCII);
	sm9_proxylib_destroyObject(gParams);
	sm9_proxylib_deserializeObject(data_value, data_len, &gParams,SM9_SERIALIZE_HEXASCII);

	sm9_proxylib_generateMasterKeys(gParams, &mpk,&msk,SM9_SCHEME_SW);
	data_len = 4096;
	sm9_proxylib_getSerializeObjectSize(mpk, SM9_SERIALIZE_HEXASCII, &data_len);
	sm9_proxylib_serializeObject(mpk,data_value, &data_len, data_len, SM9_SERIALIZE_HEXASCII);
	sm9_proxylib_destroyObject(mpk);
	sm9_proxylib_deserializeObject(data_value, data_len, &mpk,SM9_SERIALIZE_HEXASCII);
	m_editPub.SetWindowText(data_value);
	
	data_len = 4096;
	sm9_proxylib_getSerializeObjectSize(msk, SM9_SERIALIZE_HEXASCII, &data_len);
	sm9_proxylib_serializeObject(msk,data_value, &data_len, data_len, SM9_SERIALIZE_HEXASCII);
	sm9_proxylib_destroyObject(msk);
	sm9_proxylib_deserializeObject(data_value, data_len, &msk,SM9_SERIALIZE_HEXASCII);

	m_editPrv.SetWindowText(data_value);
}


void CSM9GenDlg::OnBnClicked2()
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

	sm9_proxylib_deserializeObject(data_value, data_len, &g_msk,SM9_SERIALIZE_HEXASCII);



	data_len = 4096;

	m_editPub.GetWindowText(data_value,data_len);
	data_len = strlen(data_value);

	if (g_mpk)
	{
		sm9_proxylib_destroyObject(g_mpk);
		g_mpk = 0;
	}

	sm9_proxylib_deserializeObject(data_value, data_len, &g_mpk,SM9_SERIALIZE_HEXASCII);
}
