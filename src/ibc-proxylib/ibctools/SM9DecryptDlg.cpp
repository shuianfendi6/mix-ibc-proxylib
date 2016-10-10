// SM9DecryptDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ibctools.h"
#include "SM9DecryptDlg.h"
#include "afxdialogex.h"

#include "sm9_proxylib_api.h"

extern void *g_gParams;
extern void *g_msk;
extern void *g_mpk;
extern void *g_sk;
extern char g_id[1024];
extern int g_id_len;


// CSM9DecryptDlg dialog

IMPLEMENT_DYNAMIC(CSM9DecryptDlg, CDialogEx)

CSM9DecryptDlg::CSM9DecryptDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSM9DecryptDlg::IDD, pParent)
{

}

CSM9DecryptDlg::~CSM9DecryptDlg()
{
}

void CSM9DecryptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_editIn);
	DDX_Control(pDX, IDC_EDIT1, m_editOut);
}


BEGIN_MESSAGE_MAP(CSM9DecryptDlg, CDialogEx)
	ON_BN_CLICKED(3, &CSM9DecryptDlg::OnBnClicked3)
	ON_BN_CLICKED(2, &CSM9DecryptDlg::OnBnClicked2)
END_MESSAGE_MAP()


// CSM9DecryptDlg message handlers


void CSM9DecryptDlg::OnBnClicked3()
{
	// TODO: Add your control notification handler code here
}


void CSM9DecryptDlg::OnBnClicked2()
{
	char data_value[4096] = {0};
	int data_len = 4096;

	void *gParams = 0;
	void *cipher = 0;
	void *plain = 0;

	if(0 == g_mpk)
	{
		MessageBox("未设置主公钥！");
		return;
	}

	if(0 == g_sk)
	{
		MessageBox("未设置用户私钥！");
		return;
	}

	sm9_proxylib_generateParams(&gParams,SM9_SCHEME_SW);

	data_len = 4096;

	m_editIn.GetWindowText(data_value,data_len);
	data_len = strlen(data_value);

	sm9_proxylib_deserializeObject(data_value, data_len, &cipher,SM9_SERIALIZE_HEXASCII);

	if(0 == sm9_proxylib_decrypt(gParams,g_mpk,g_sk,g_id,g_id_len,cipher,&plain,SM9_CIPHER_KDF_UNION,SM9_SCHEME_SW))
	{
		MessageBox("解密成功！");
	}
	else
	{
		MessageBox("解密失败！");
		return;
	}

	data_len = 4096;
	sm9_proxylib_getSerializeObjectSize(plain, SM9_SERIALIZE_HEXASCII, &data_len);
	sm9_proxylib_serializeObject(plain,data_value, &data_len, data_len, SM9_SERIALIZE_HEXASCII);
	sm9_proxylib_destroyObject(plain);
	sm9_proxylib_deserializeObject(data_value, data_len, &plain,SM9_SERIALIZE_HEXASCII);

	m_editOut.SetWindowText(data_value+18);
}
