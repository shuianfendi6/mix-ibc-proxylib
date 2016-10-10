// SM9CalcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ibctools.h"
#include "SM9CalcDlg.h"
#include "afxdialogex.h"

#include "sm9_proxylib_api.h"

extern void *g_gParams;
extern void *g_msk;
extern void *g_mpk;
extern void *g_sk;
extern char g_id[1024];
extern int g_id_len;

// CSM9CalcDlg dialog

IMPLEMENT_DYNAMIC(CSM9CalcDlg, CDialogEx)

CSM9CalcDlg::CSM9CalcDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSM9CalcDlg::IDD, pParent)
{

}

CSM9CalcDlg::~CSM9CalcDlg()
{
}

void CSM9CalcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_editID);
	DDX_Control(pDX, IDC_EDIT1, m_editPrv);
}


BEGIN_MESSAGE_MAP(CSM9CalcDlg, CDialogEx)
	ON_BN_CLICKED(3, &CSM9CalcDlg::OnBnClicked3)
	ON_BN_CLICKED(2, &CSM9CalcDlg::OnBnClicked2)
END_MESSAGE_MAP()


// CSM9CalcDlg message handlers

extern "C" unsigned long Hex2Bin(const char *pbIN,int ulINLen,unsigned char *pbOUT,int * pulOUTLen);
extern "C" unsigned long Bin2Hex(const unsigned char *pbIN,int ulINLen,char *pbOUT,int * pulOUTLen);

void CSM9CalcDlg::OnBnClicked3()
{
	char data_value[4096] = {0};
	int data_len = 4096;

	char data_value2[4096] = {0};
	int data_len2 = 4096;

	void *gParams = 0;
	void *sk = 0;

	sm9_proxylib_generateParams(&gParams,SM9_SCHEME_SW);

	data_len = 4096;

	m_editID.GetWindowText(data_value,data_len);
	data_len = strlen(data_value);

	data_len2 = data_len;

	Hex2Bin(data_value,data_len,(unsigned char *)data_value2,&data_len2);

	sm9_proxylib_calculateUserKeys(gParams,g_msk,data_value2,data_len2,&sk,SM9_SCHEME_SW);

	data_len = 4096;
	sm9_proxylib_getSerializeObjectSize(sk, SM9_SERIALIZE_HEXASCII, &data_len);
	sm9_proxylib_serializeObject(sk,data_value, &data_len, data_len, SM9_SERIALIZE_HEXASCII);
	sm9_proxylib_destroyObject(sk);
	sm9_proxylib_deserializeObject(data_value, data_len, &sk,SM9_SERIALIZE_HEXASCII);

	m_editPrv.SetWindowText(data_value);
}


void CSM9CalcDlg::OnBnClicked2()
{
	char data_value[4096] = {0};
	int data_len = 4096;

	char data_value2[4096] = {0};
	int data_len2 = 4096;

	void *gParams = 0;

	sm9_proxylib_generateParams(&gParams,SM9_SCHEME_SW);

	m_editPrv.GetWindowText(data_value,data_len);
	data_len = strlen(data_value);


	if (g_sk)
	{
		sm9_proxylib_destroyObject(g_sk);
		g_sk = 0;
	}

	sm9_proxylib_deserializeObject(data_value, data_len, &g_sk,SM9_SERIALIZE_HEXASCII);

	data_len = 4096;

	m_editID.GetWindowText(data_value,data_len);
	data_len = strlen(data_value);

	data_len2 = data_len;

	Hex2Bin(data_value,data_len,(unsigned char *)data_value2,&data_len2);

	memcpy(g_id, data_value2,data_len2);

	g_id_len = data_len2;
}
