// SM9GenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ibctools.h"
#include "SM9GenDlg.h"
#include "afxdialogex.h"
#include "sm9_proxylib_api.h"

extern "C" unsigned long Hex2Bin(const char *pbIN,int ulINLen,unsigned char *pbOUT,int * pulOUTLen);
extern "C" unsigned long Bin2Hex(const unsigned char *pbIN,int ulINLen,char *pbOUT,int * pulOUTLen);

// CSM9GenDlg dialog

IMPLEMENT_DYNAMIC(CSM9GenDlg, CDialogEx)

CSM9GenDlg::CSM9GenDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSM9GenDlg::IDD, pParent)
{

}

CSM9GenDlg::~CSM9GenDlg()
{
}

void CSM9GenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_editG1);
	DDX_Control(pDX, IDC_EDIT2, m_editmsk);
	DDX_Control(pDX, IDC_EDIT5, m_editG2);
}


BEGIN_MESSAGE_MAP(CSM9GenDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSM9GenDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSM9GenDlg message handlers


void CSM9GenDlg::OnBnClickedOk()
{
	char data_value[4096] = {0};
	int data_len = 4096;

	char data_value_G1[SM9_BYTES_LEN_G1] = {0};
	int data_len_G1 = SM9_BYTES_LEN_G1;

	char data_value_G2[SM9_BYTES_LEN_G2] = {0};
	int data_len_G2 = SM9_BYTES_LEN_G2;

	char data_value_msk[SM9_BYTES_LEN_BIG] = {0};
	int data_len_msk = SM9_BYTES_LEN_BIG;

	if(0 == sm9_generateMasterKeys(data_value_msk,&data_len_msk, data_value_G1,&data_len_G1,data_value_G2,&data_len_G2))
	{
		MessageBox("生成主密钥成功！");
	}
	else
	{
		MessageBox("生成主密钥失败！");
		return;
	}

	data_len = 4096;
	Bin2Hex((unsigned char*)data_value_G1,data_len_G1,data_value,&data_len);
	m_editG1.SetWindowText(data_value);

	data_len = 4096;
	Bin2Hex((unsigned char*)data_value_G2,data_len_G2,data_value,&data_len);
	m_editG2.SetWindowText(data_value);

	data_len = 4096;
	Bin2Hex((unsigned char*)data_value_msk,data_len_msk,data_value,&data_len);
	m_editmsk.SetWindowText(data_value);
}