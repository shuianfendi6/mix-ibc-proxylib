// SM9KeyExPreDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ibctools.h"
#include "SM9KeyExPreDlg.h"
#include "afxdialogex.h"

#include "sm9_proxylib_api.h"
extern "C" unsigned long Hex2Bin(const char *pbIN,int ulINLen,unsigned char *pbOUT,int * pulOUTLen);
extern "C" unsigned long Bin2Hex(const unsigned char *pbIN,int ulINLen,char *pbOUT,int * pulOUTLen);



// CSM9KeyExPreDlg 对话框

IMPLEMENT_DYNAMIC(CSM9KeyExPreDlg, CDialogEx)

CSM9KeyExPreDlg::CSM9KeyExPreDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSM9KeyExPreDlg::IDD, pParent)
{

}

CSM9KeyExPreDlg::~CSM9KeyExPreDlg()
{
}

void CSM9KeyExPreDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT4, m_editG1);
	DDX_Control(pDX, IDC_EDIT5, m_editID);
	DDX_Control(pDX, IDC_EDIT2, m_editr);
	DDX_Control(pDX, IDC_EDIT1, m_editR);
}


BEGIN_MESSAGE_MAP(CSM9KeyExPreDlg, CDialogEx)
	ON_BN_CLICKED(4, &CSM9KeyExPreDlg::OnBnClicked4)
END_MESSAGE_MAP()


// CSM9KeyExPreDlg 消息处理程序


void CSM9KeyExPreDlg::OnBnClicked4()
{
	char data_value[4096] = {0};
	int data_len = 4096;

	char data_value_r[4096] = {0};
	int data_len_r = 4096;

	char data_value_R[SM9_BYTES_LEN_G1] = {0};
	int data_len_R = SM9_BYTES_LEN_G1;

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

	data_len_r = 4096;
	if( 0 == sm9_keyExchange_pre(data_value_G1,data_len_G1,id,id_len,data_value_r,&data_len_r,data_value_R,&data_len_R))
	{
		MessageBox("执行成功！");
	}
	else
	{
		MessageBox("执行失败！");
		return;
	}

	data_len = 4096;
	Bin2Hex((unsigned char*)data_value_r,data_len_r,data_value,&data_len);
	m_editr.SetWindowText(data_value);

	data_len = 4096;
	Bin2Hex((unsigned char*)data_value_R,data_len_R,data_value,&data_len);
	m_editR.SetWindowText(data_value);
}
