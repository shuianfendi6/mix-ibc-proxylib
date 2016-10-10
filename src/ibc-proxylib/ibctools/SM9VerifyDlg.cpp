// SM9VerifyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ibctools.h"
#include "SM9VerifyDlg.h"
#include "afxdialogex.h"

#include "sm9_proxylib_api.h"

extern void *g_gParams;
extern void *g_msk;
extern void *g_mpk;
extern void *g_sk;
extern char g_id[1024];
extern int g_id_len;


// CSM9VerifyDlg dialog

IMPLEMENT_DYNAMIC(CSM9VerifyDlg, CDialogEx)

CSM9VerifyDlg::CSM9VerifyDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSM9VerifyDlg::IDD, pParent)
{

}

CSM9VerifyDlg::~CSM9VerifyDlg()
{
}

void CSM9VerifyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_editIn);
	DDX_Control(pDX, IDC_EDIT1, m_editOut);
}


BEGIN_MESSAGE_MAP(CSM9VerifyDlg, CDialogEx)
	ON_BN_CLICKED(2, &CSM9VerifyDlg::OnBnClicked2)
END_MESSAGE_MAP()


// CSM9VerifyDlg message handlers



extern "C" unsigned long Hex2Bin(const char *pbIN,int ulINLen,unsigned char *pbOUT,int * pulOUTLen);
extern "C" unsigned long Bin2Hex(const unsigned char *pbIN,int ulINLen,char *pbOUT,int * pulOUTLen);


void CSM9VerifyDlg::OnBnClicked2()
{
	char data_value[4096] = {0};
	int data_len = 4096;

	char data_value2[4096] = {0};
	int data_len2 = 4096;

	void *gParams = 0;
	void *sgn = 0;

	if(0 == g_mpk)
	{
		MessageBox("未设置主公钥！");
		return;
	}


	sm9_proxylib_generateParams(&gParams,SM9_SCHEME_SW);

	data_len = 4096;

	m_editOut.GetWindowText(data_value,data_len);
	data_len = strlen(data_value);

	if(0 == sm9_proxylib_deserializeObject(data_value, data_len, &sgn,SM9_SERIALIZE_HEXASCII))
	{

	}
	else
	{
		MessageBox("签名值格式不正确！");
		return;
	}

	data_len = 4096;

	m_editIn.GetWindowText(data_value,data_len);
	data_len = strlen(data_value);

	data_len2 = data_len;

	Hex2Bin(data_value,data_len,(unsigned char *)data_value2,&data_len2);

	if(0 == sm9_proxylib_verify(gParams,g_mpk,g_id,g_id_len,data_value2,data_len2,sgn,SM9_SCHEME_SW))
	{
		MessageBox("验证成功！");
	}
	else
	{
		MessageBox("验证失败！");
	}
}

