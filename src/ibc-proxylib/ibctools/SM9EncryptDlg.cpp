// SM9EncryptDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ibctools.h"
#include "SM9EncryptDlg.h"
#include "afxdialogex.h"

#include "sm9_proxylib_api.h"

extern void *g_gParams;
extern void *g_msk;
extern void *g_mpk;
extern void *g_sk;

extern SM9_CIPHER_TYPE g_cryptoMode;


// CSM9EncryptDlg dialog

IMPLEMENT_DYNAMIC(CSM9EncryptDlg, CDialogEx)

CSM9EncryptDlg::CSM9EncryptDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSM9EncryptDlg::IDD, pParent)
{

}

CSM9EncryptDlg::~CSM9EncryptDlg()
{
}

void CSM9EncryptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_editIn);
	DDX_Control(pDX, IDC_EDIT1, m_editOut);
	DDX_Control(pDX, IDC_EDIT5, m_editID);
}


BEGIN_MESSAGE_MAP(CSM9EncryptDlg, CDialogEx)
	ON_BN_CLICKED(2, &CSM9EncryptDlg::OnBnClicked2)
	ON_BN_CLICKED(3, &CSM9EncryptDlg::OnBnClicked3)
	ON_BN_CLICKED(4, &CSM9EncryptDlg::OnBnClicked4)
END_MESSAGE_MAP()


// CSM9EncryptDlg message handlers


extern "C" unsigned long Hex2Bin(const char *pbIN,int ulINLen,unsigned char *pbOUT,int * pulOUTLen);
extern "C" unsigned long Bin2Hex(const unsigned char *pbIN,int ulINLen,char *pbOUT,int * pulOUTLen);


void CSM9EncryptDlg::OnBnClicked2()
{
	char data_value[4096] = {0};
	int data_len = 4096;

	char id[4096] = {0};
	int id_len = 4096;

	char data_value2[4096] = {0};
	int data_len2 = 4096;

	void *gParams = 0;
	void *cipher = 0;

	if(0 == g_mpk)
	{
		MessageBox("未设置主公钥！");
		return;
	}


	sm9_proxylib_generateParams(&gParams,SM9_SCHEME_SW);

	data_len = 4096;

	m_editID.GetWindowText(data_value,data_len);
	data_len = strlen(data_value);

	Hex2Bin(data_value,data_len,(unsigned char *)id,&id_len);

	data_len = 4096;

	m_editIn.GetWindowText(data_value,data_len);
	data_len = strlen(data_value);

	data_len2 = data_len;

	Hex2Bin(data_value,data_len,(unsigned char *)data_value2,&data_len2);


	if( 0 == sm9_proxylib_encrypt(gParams,g_mpk,id,id_len,data_value2,data_len2,&cipher,g_cryptoMode,SM9_SCHEME_SW))
	{
		MessageBox("加密成功！");
	}
	else
	{
		MessageBox("加密失败！");
		return;
	}


	data_len = 4096;
	sm9_proxylib_getSerializeObjectSize(cipher, SM9_SERIALIZE_HEXASCII, &data_len);
	sm9_proxylib_serializeObject(cipher,data_value, &data_len, data_len, SM9_SERIALIZE_HEXASCII);
	sm9_proxylib_destroyObject(cipher);
	sm9_proxylib_deserializeObject(data_value, data_len, &cipher,SM9_SERIALIZE_HEXASCII);

	m_editOut.SetWindowText(data_value);
}


void CSM9EncryptDlg::OnBnClicked3()
{
	// TODO: Add your control notification handler code here
	char data_value[4096] = {0};
	int data_len = 4096;

	char data_value2[4096] = {0};
	int data_len2 = 4096;

	int C2_len = 4096;

	void *cipher = NULL;

	data_len = 4096;

	m_editIn.GetWindowText(data_value,data_len);
	data_len = strlen(data_value);

	sm9_proxylib_deserializeObject(data_value, data_len, &cipher,SM9_SERIALIZE_HEXASCII);

	if(!cipher)
	{
		MessageBox("输入不正确！");
		return;
	}

	sm9_proxylib_ObjectToItemsValueCipher(cipher,data_value,data_value+64,data_value+64+32,&C2_len);

	data_len = 64+32+C2_len;

	Bin2Hex((const unsigned char*)data_value,data_len,data_value2,&data_len2);

	m_editOut.SetWindowText(data_value2);

}


void CSM9EncryptDlg::OnBnClicked4()
{
	char data_value[4096] = {0};
	int data_len = 4096;

	char id[4096] = {0};
	int id_len = 4096;

	char data_value2[4096] = {0};
	int data_len2 = 4096;

	void *gParams = 0;
	void *cipher = 0;
	int C2_len = 4096;

	if(0 == g_mpk)
	{
		MessageBox("未设置主公钥！");
		return;
	}

	sm9_proxylib_generateParams(&gParams,SM9_SCHEME_SW);

	data_len = 4096;

	m_editID.GetWindowText(data_value,data_len);
	data_len = strlen(data_value);

	Hex2Bin(data_value,data_len,(unsigned char *)id,&id_len);

	data_len = 4096;

	m_editIn.GetWindowText(data_value,data_len);
	data_len = strlen(data_value);

	data_len2 = data_len;

	Hex2Bin(data_value,data_len,(unsigned char *)data_value2,&data_len2);


	if( 0 == sm9_proxylib_encrypt(gParams,g_mpk,id,id_len,data_value2,data_len2,&cipher,g_cryptoMode,SM9_SCHEME_SW))
	{
		MessageBox("加密成功！");
	}
	else
	{
		MessageBox("加密失败！");
		return;
	}

	sm9_proxylib_ObjectToItemsValueCipher(cipher,data_value,data_value+64,data_value+64+32,&C2_len);

	data_len = 64+32+C2_len;

	data_len2 = 4096;

	Bin2Hex((const unsigned char*)data_value,data_len,data_value2,&data_len2);

	m_editOut.SetWindowText(data_value2);
}
