// SM9WrapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ibctools.h"
#include "SM9WrapDlg.h"
#include "afxdialogex.h"

#include "sm9_proxylib_api.h"

extern "C" unsigned long Hex2Bin(const char *pbIN,int ulINLen,unsigned char *pbOUT,int * pulOUTLen);
extern "C" unsigned long Bin2Hex(const unsigned char *pbIN,int ulINLen,char *pbOUT,int * pulOUTLen);

// CSM9WrapDlg dialog

IMPLEMENT_DYNAMIC(CSM9WrapDlg, CDialogEx)

CSM9WrapDlg::CSM9WrapDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSM9WrapDlg::IDD, pParent)
{

}

CSM9WrapDlg::~CSM9WrapDlg()
{
}

void CSM9WrapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT4, m_editG1);
	DDX_Control(pDX, IDC_EDIT5, m_editID);
	DDX_Control(pDX, IDC_EDIT2, m_editIn);
	DDX_Control(pDX, IDC_EDIT1, m_editOut);
}


BEGIN_MESSAGE_MAP(CSM9WrapDlg, CDialogEx)
	ON_BN_CLICKED(ID_OK_WRAP, &CSM9WrapDlg::OnBnClickedOkWrap)
END_MESSAGE_MAP()


// CSM9WrapDlg message handlers


void CSM9WrapDlg::OnBnClickedOkWrap()
{
	char data_value[4096] = {0};
	int data_len = 4096;

	char data_value_plain[4096] = {0};
	int data_len_plain = 4096;

	char data_value_cipher[4096] = {0};
	int data_len_cipher = 4096;

	char id[4096] = {0};
	int id_len = 4096;

	char data_value_G1[SM9_BYTES_LEN_G1] = {0};
	int data_len_G1 = SM9_BYTES_LEN_G1;


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
	m_editID.GetWindowText(data_value,data_len);
	data_len = strlen(data_value);
	Hex2Bin(data_value,data_len,(unsigned char *)id,&id_len);

	data_len = 4096;

	if( 0 == sm9_wrap(data_value_G1,data_len_G1,id,id_len,data_value_plain,&data_len_plain,data_value_cipher,&data_len_cipher))
	{
		MessageBox("封装成功！");
	}
	else
	{
		MessageBox("封装失败！");
		return;
	}

	data_len = 4096;
	Bin2Hex((unsigned char*)data_value_cipher,data_len_cipher,data_value,&data_len);
	m_editIn.SetWindowText(data_value);

	data_len = 4096;
	Bin2Hex((unsigned char*)data_value_plain,data_len_plain,data_value,&data_len);
	m_editOut.SetWindowText(data_value);
}
