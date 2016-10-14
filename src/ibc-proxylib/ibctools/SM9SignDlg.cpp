// SM9SignDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ibctools.h"
#include "SM9SignDlg.h"
#include "afxdialogex.h"

#include "sm9_proxylib_api.h"

extern "C" unsigned long Hex2Bin(const char *pbIN,int ulINLen,unsigned char *pbOUT,int * pulOUTLen);
extern "C" unsigned long Bin2Hex(const unsigned char *pbIN,int ulINLen,char *pbOUT,int * pulOUTLen);

// CSM9SignDlg dialog

IMPLEMENT_DYNAMIC(CSM9SignDlg, CDialogEx)

CSM9SignDlg::CSM9SignDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSM9SignDlg::IDD, pParent)
{

}

CSM9SignDlg::~CSM9SignDlg()
{
}

void CSM9SignDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_editIn);
	DDX_Control(pDX, IDC_EDIT1, m_editOut);
	DDX_Control(pDX, IDC_EDIT6, m_edithid01);
	DDX_Control(pDX, IDC_EDIT4, m_editG2);
}


BEGIN_MESSAGE_MAP(CSM9SignDlg, CDialogEx)
	ON_BN_CLICKED(4, &CSM9SignDlg::OnBnClicked4)
END_MESSAGE_MAP()


// CSM9SignDlg message handlers

void CSM9SignDlg::OnBnClicked4()
{
	char data_value[4096] = {0};
	int data_len = 4096;

	char data_value_G2[SM9_BYTES_LEN_G2] = {0};
	int data_len_G2 = SM9_BYTES_LEN_G2;

	char data_value_hid01[SM9_BYTES_LEN_G1] = {0};
	int data_len_hid01 = SM9_BYTES_LEN_G1;

	char data_value_cipher[4096] = {0};
	int data_len_cipher = 4096;

	char data_value_plain[4096] = {0};
	int data_len_plain = 4096;

	data_len = 4096;
	m_editG2.GetWindowText(data_value,data_len);
	data_len = strlen(data_value);
	Hex2Bin(data_value,data_len,(unsigned char *)data_value_G2,&data_len_G2);

	if(0 == data_len_G2)
	{
		MessageBox("未设置主公钥！");
		return;
	}

	data_len = 4096;
	m_edithid01.GetWindowText(data_value,data_len);
	data_len = strlen(data_value);
	Hex2Bin(data_value,data_len,(unsigned char *)data_value_hid01,&data_len_hid01);

	if(0 == data_len_hid01)
	{
		MessageBox("未设置用户私钥！");
		return;
	}

	data_len = 4096;
	m_editIn.GetWindowText(data_value,data_len);
	data_len = strlen(data_value);
	Hex2Bin(data_value,data_len,(unsigned char *)data_value_plain,&data_len_plain);

	if(0 == sm9_sign(data_value_G2,data_len_G2,data_value_hid01,data_len_hid01,data_value_plain,data_len_plain,data_value_cipher,&data_len_cipher))
	{
		MessageBox("签名成功！");
	}
	else
	{
		MessageBox("签名失败！");
		return;
	}

	data_len = 4096;
	Bin2Hex((unsigned char*)data_value_cipher,data_len_cipher,data_value,&data_len);
	m_editOut.SetWindowText(data_value);
}
