// SM9CalcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ibctools.h"
#include "SM9CalcDlg.h"
#include "afxdialogex.h"

#include "sm9_proxylib_api.h"

extern "C" unsigned long Hex2Bin(const char *pbIN,int ulINLen,unsigned char *pbOUT,int * pulOUTLen);
extern "C" unsigned long Bin2Hex(const unsigned char *pbIN,int ulINLen,char *pbOUT,int * pulOUTLen);

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
	DDX_Control(pDX, IDC_EDIT1, m_edithid01);
	DDX_Control(pDX, IDC_EDIT3, m_editmsk);
	DDX_Control(pDX, IDC_EDIT4, m_edithid02);
	DDX_Control(pDX, IDC_EDIT6, m_edithid03);
}


BEGIN_MESSAGE_MAP(CSM9CalcDlg, CDialogEx)
	ON_BN_CLICKED(3, &CSM9CalcDlg::OnBnClicked3)
END_MESSAGE_MAP()


// CSM9CalcDlg message handlers



void CSM9CalcDlg::OnBnClicked3()
{
	char data_value[4096] = {0};
	int data_len = 4096;

	char data_value_id[4096] = {0};
	int data_len_id = 4096;

	char data_value_msk[SM9_BYTES_LEN_BIG] = {0};
	int data_len_msk = SM9_BYTES_LEN_BIG;

	char data_value_hid01[SM9_BYTES_LEN_G1] = {0};
	int data_len_hid01 = SM9_BYTES_LEN_G1;

	char data_value_hid02[SM9_BYTES_LEN_G2] = {0};
	int data_len_hid02 = SM9_BYTES_LEN_G2;

	char data_value_hid03[SM9_BYTES_LEN_G2] = {0};
	int data_len_hid03 = SM9_BYTES_LEN_G2;

	data_len = 4096;

	m_editmsk.GetWindowText(data_value,data_len);
	data_len = strlen(data_value);

	Hex2Bin(data_value,data_len,(unsigned char*)data_value_msk,&data_len_msk);

	if(0 == data_len_msk)
	{
		MessageBox("未设置主私钥！");
		return;
	}

	data_len = 4096;

	m_editID.GetWindowText(data_value,data_len);
	data_len = strlen(data_value);

	Hex2Bin(data_value,data_len,(unsigned char *)data_value_id,&data_len_id);

	if(0 == sm9_calculateUserKeys(data_value_msk,SM9_BYTES_LEN_BIG,data_value_id,data_len_id,data_value_hid01,&data_len_hid01,data_value_hid02,&data_len_hid02,data_value_hid03,&data_len_hid03))
	{
		MessageBox("计算成功！");
	}
	else
	{
		MessageBox("计算失败！");
		return;
	}

	data_len = 4096;
	Bin2Hex((unsigned char*)data_value_hid01,data_len_hid01,data_value,&data_len);
	m_edithid01.SetWindowText(data_value);

	data_len = 4096;
	Bin2Hex((unsigned char*)data_value_hid02,data_len_hid02,data_value,&data_len);
	m_edithid02.SetWindowText(data_value);

	data_len = 4096;
	Bin2Hex((unsigned char*)data_value_hid03,data_len_hid03,data_value,&data_len);
	m_edithid03.SetWindowText(data_value);

}
