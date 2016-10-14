// SM9DecryptDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ibctools.h"
#include "SM9DecryptDlg.h"
#include "afxdialogex.h"

#include "sm9_proxylib_api.h"

extern "C" unsigned long Hex2Bin(const char *pbIN,int ulINLen,unsigned char *pbOUT,int * pulOUTLen);
extern "C" unsigned long Bin2Hex(const unsigned char *pbIN,int ulINLen,char *pbOUT,int * pulOUTLen);

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
	DDX_Control(pDX, IDC_EDIT5, m_editID);
	DDX_Control(pDX, IDC_EDIT4, m_editG1);
	DDX_Control(pDX, IDC_EDIT6, m_edithid03);
}


BEGIN_MESSAGE_MAP(CSM9DecryptDlg, CDialogEx)
	ON_BN_CLICKED(4, &CSM9DecryptDlg::OnBnClicked4)
END_MESSAGE_MAP()

void CSM9DecryptDlg::OnBnClicked4()
{
	char data_value[4096] = {0};
	int data_len = 4096;

	char data_value_cipher[4096] = {0};
	int data_len_cipher = 4096;

	char data_value_plain[4096] = {0};
	int data_len_plain = 4096;

	char id[4096] = {0};
	int id_len = 4096;

	char data_value_G1[SM9_BYTES_LEN_G1] = {0};
	int data_len_G1 = SM9_BYTES_LEN_G1;

	char data_value_hid03[SM9_BYTES_LEN_G2] = {0};
	int data_len_hid03 = SM9_BYTES_LEN_G2;


	data_len = 4096;
	m_editG1.GetWindowText(data_value,data_len);
	data_len = strlen(data_value);
	Hex2Bin(data_value,data_len,(unsigned char *)data_value_G1,&data_len_G1);

	if(0 == data_len_G1)
	{
		MessageBox("未设置主公钥！");
		return;
	}

	data_len = 4096;
	m_editIn.GetWindowText(data_value,data_len);
	data_len = strlen(data_value);
	Hex2Bin(data_value,data_len,(unsigned char *)data_value_cipher,&data_len_cipher);

	if(0 == data_len_G1)
	{
		MessageBox("未设置密文！");
		return;
	}

	data_len = 4096;
	m_edithid03.GetWindowText(data_value,data_len);
	data_len = strlen(data_value);
	Hex2Bin(data_value,data_len,(unsigned char *)data_value_hid03,&data_len_hid03);

	if (0 == data_len_hid03)
	{
		MessageBox("未设置用户私钥！");
		return;
	}

	data_len = 4096;
	m_editID.GetWindowText(data_value,data_len);
	data_len = strlen(data_value);
	Hex2Bin(data_value,data_len,(unsigned char *)id,&id_len);
	
	if(0 == sm9_decrypt(data_value_G1,data_len_G1,data_value_hid03,data_len_hid03,id,id_len,data_value_cipher,data_len_cipher,data_value_plain,&data_len_plain,g_cryptoMode))
	{
		MessageBox("解密成功！");
	}
	else
	{
		MessageBox("解密失败！");
		return;
	}

	data_len = 4096;
	Bin2Hex((unsigned char*)data_value_plain,data_len_plain,data_value,&data_len);
	m_editOut.SetWindowText(data_value);
}
