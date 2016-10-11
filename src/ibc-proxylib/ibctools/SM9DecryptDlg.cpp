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

extern SM9_CIPHER_TYPE g_cryptoMode;


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


#ifndef GET_ULONG_BE
#define GET_ULONG_BE(n,b,i)                             \
	{                                                       \
	(n) = ( (unsigned int) (b)[(i)    ] << 24 )        \
	| ( (unsigned int) (b)[(i) + 1] << 16 )        \
	| ( (unsigned int) (b)[(i) + 2] <<  8 )        \
	| ( (unsigned int) (b)[(i) + 3]       );       \
	}
#endif

#ifndef PUT_ULONG_BE
#define PUT_ULONG_BE(n,b,i)                             \
	{                                                       \
	(b)[(i)    ] = (unsigned char) ( (n) >> 24 );       \
	(b)[(i) + 1] = (unsigned char) ( (n) >> 16 );       \
	(b)[(i) + 2] = (unsigned char) ( (n) >>  8 );       \
	(b)[(i) + 3] = (unsigned char) ( (n)       );       \
	}
#endif

void CSM9DecryptDlg::OnBnClicked3()
{
	char data_value[4096] = {0};
	int data_len = 4096;

	char data_value2[4096] = {0};
	int data_len2 = 4096;

	void *gParams = 0;
	void *cipher = 0;


	int pos = 0;


	data_len = 4096;

	m_editIn.GetWindowText(data_value,data_len);
	data_len = strlen(data_value);

	m_editIn.GetWindowText(data_value2,data_len2);
	data_len2 = strlen(data_value2);

	if(0 == g_mpk)
	{
		MessageBox("δ��������Կ��");
		return;
	}

	sm9_proxylib_generateParams(&gParams,SM9_SCHEME_SW);

	data_len = 4096;

	m_editIn.GetWindowText(data_value,data_len);
	data_len = strlen(data_value);

	if( 0 == sm9_proxylib_encrypt(gParams,g_mpk,0,0,data_value,data_len/2 - 3*32,&cipher,g_cryptoMode,SM9_SCHEME_SW))
	{
		
	}
	else
	{
		MessageBox("����ʧ�ܣ�");
		return;
	}

	data_len = 4096;
	sm9_proxylib_getSerializeObjectSize(cipher, SM9_SERIALIZE_HEXASCII, &data_len);
	sm9_proxylib_serializeObject(cipher,data_value, &data_len, data_len, SM9_SERIALIZE_HEXASCII);
	sm9_proxylib_destroyObject(cipher);
	sm9_proxylib_deserializeObject(data_value, data_len, &cipher,SM9_SERIALIZE_HEXASCII);

	// len
	pos += 8;
	// type
	pos += 2;

	if (data_len2 < 32 * 3 * 2)
	{
		MessageBox("�����ʽ����ȷ��");
		return;
	}
	else
	{
		pos += 8;
		memcpy(data_value+pos,data_value2,64);
		pos += 64;
		pos += 8;
		memcpy(data_value+pos,data_value2+64*1,64);
		pos += 64;
		pos += 8;
		memcpy(data_value+pos,data_value2+64*2,64);
		pos += 64;
		pos += 8;
		memcpy(data_value+pos,data_value2+64*3,data_len2 - 3 * 64);
		pos += 64;

	}

	m_editOut.SetWindowText(data_value);
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
		MessageBox("δ��������Կ��");
		return;
	}

	if(0 == g_sk)
	{
		MessageBox("δ�����û�˽Կ��");
		return;
	}

	sm9_proxylib_generateParams(&gParams,SM9_SCHEME_SW);

	data_len = 4096;

	m_editIn.GetWindowText(data_value,data_len);
	data_len = strlen(data_value);

	sm9_proxylib_deserializeObject(data_value, data_len, &cipher,SM9_SERIALIZE_HEXASCII);

	if(0 == sm9_proxylib_decrypt(gParams,g_mpk,g_sk,g_id,g_id_len,cipher,&plain,g_cryptoMode,SM9_SCHEME_SW))
	{
		MessageBox("���ܳɹ���");
	}
	else
	{
		MessageBox("����ʧ�ܣ�");
		return;
	}

	data_len = 4096;
	sm9_proxylib_getSerializeObjectSize(plain, SM9_SERIALIZE_HEXASCII, &data_len);
	sm9_proxylib_serializeObject(plain,data_value, &data_len, data_len, SM9_SERIALIZE_HEXASCII);
	sm9_proxylib_destroyObject(plain);
	sm9_proxylib_deserializeObject(data_value, data_len, &plain,SM9_SERIALIZE_HEXASCII);

	m_editOut.SetWindowText(data_value+18);
}
