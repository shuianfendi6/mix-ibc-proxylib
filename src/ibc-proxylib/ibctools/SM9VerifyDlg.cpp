// SM9VerifyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ibctools.h"
#include "SM9VerifyDlg.h"
#include "afxdialogex.h"

#include "sm9_proxylib_api.h"

extern "C" unsigned long Hex2Bin(const char *pbIN,int ulINLen,unsigned char *pbOUT,int * pulOUTLen);
extern "C" unsigned long Bin2Hex(const unsigned char *pbIN,int ulINLen,char *pbOUT,int * pulOUTLen);

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
	DDX_Control(pDX, IDC_EDIT5, m_editID);
	DDX_Control(pDX, IDC_EDIT4, m_editG2);
}


BEGIN_MESSAGE_MAP(CSM9VerifyDlg, CDialogEx)
	ON_BN_CLICKED(4, &CSM9VerifyDlg::OnBnClicked4)
END_MESSAGE_MAP()


// CSM9VerifyDlg message handlers




void CSM9VerifyDlg::OnBnClicked4()
{
	char data_value[4096] = {0};
	int data_len = 4096;

	char id[4096] = {0};
	int id_len = 4096;

	char data_value_cipher[4096] = {0};
	int data_len_cipher = 4096;

	char data_value_plain[4096] = {0};
	int data_len_plain = 4096;

	char data_value_G2[SM9_BYTES_LEN_G2] = {0};
	int data_len_G2 = SM9_BYTES_LEN_G2;

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
	m_editID.GetWindowText(data_value,data_len);
	data_len = strlen(data_value);
	Hex2Bin(data_value,data_len,(unsigned char *)id,&id_len);

	data_len = 4096;
	m_editIn.GetWindowText(data_value,data_len);
	data_len = strlen(data_value);
	Hex2Bin(data_value,data_len,(unsigned char *)data_value_plain,&data_len_plain);

	data_len = 4096;
	m_editOut.GetWindowText(data_value,data_len);
	data_len = strlen(data_value);
	Hex2Bin(data_value,data_len,(unsigned char *)data_value_cipher,&data_len_cipher);

	if(0 == data_len_cipher)
	{
		MessageBox("未设置签名值！");
		return;
	}

	if(0 == sm9_verify(data_value_G2,data_len_G2,id,id_len,data_value_plain,data_len_plain,data_value_cipher,data_len_cipher))
	{
		MessageBox("验证成功！");
	}
	else
	{
		MessageBox("验证失败！");
	}
}
