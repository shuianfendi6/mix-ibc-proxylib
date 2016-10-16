// SM9KeyEx.cpp : 实现文件
//

#include "stdafx.h"
#include "ibctools.h"
#include "SM9KeyExDlg.h"
#include "afxdialogex.h"

#include "sm9_proxylib_api.h"

extern int g_key_ex;

extern "C" unsigned long Hex2Bin(const char *pbIN,int ulINLen,unsigned char *pbOUT,int * pulOUTLen);
extern "C" unsigned long Bin2Hex(const unsigned char *pbIN,int ulINLen,char *pbOUT,int * pulOUTLen);


// CSM9KeyExDlg 对话框

IMPLEMENT_DYNAMIC(CSM9KeyExDlg, CDialogEx)

CSM9KeyExDlg::CSM9KeyExDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSM9KeyExDlg::IDD, pParent)
{

}

CSM9KeyExDlg::~CSM9KeyExDlg()
{
}

void CSM9KeyExDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT4, m_editG1);
	DDX_Control(pDX, IDC_EDIT5, m_editIDA);
	DDX_Control(pDX, IDC_EDIT6, m_editIDB);
	DDX_Control(pDX, IDC_EDIT7, m_editSK);
	DDX_Control(pDX, IDC_EDIT2, m_editr);
	DDX_Control(pDX, IDC_EDIT1, m_editRA);
	DDX_Control(pDX, IDC_EDIT8, m_editRB);
	DDX_Control(pDX, IDC_EDIT10, m_editSKAorSKB);
	DDX_Control(pDX, IDC_EDIT12, m_editkey_len);
	DDX_Control(pDX, IDC_EDIT9, m_editSBorS1);
	DDX_Control(pDX, IDC_EDIT11, m_editS2orSA);
}


BEGIN_MESSAGE_MAP(CSM9KeyExDlg, CDialogEx)
	ON_BN_CLICKED(4, &CSM9KeyExDlg::OnBnClicked4)
END_MESSAGE_MAP()


// CSM9KeyExDlg 消息处理程序


void CSM9KeyExDlg::OnBnClicked4()
{
	char data_value[4096] = {0};
	int data_len = 4096;

	char data_value_r[4096] = {0};
	int data_len_r = 4096;

	char data_value_RA[SM9_BYTES_LEN_G1] = {0};
	int data_len_RA = SM9_BYTES_LEN_G1;

	char data_value_RB[SM9_BYTES_LEN_G1] = {0};
	int data_len_RB = SM9_BYTES_LEN_G1;

	char data_value_SK[SM9_BYTES_LEN_G2] = {0};
	int data_len_SK = SM9_BYTES_LEN_G2;

	char data_value_SKAorSKB[SM9_BYTES_LEN_G1] = {0};
	int data_len_SKAorSKB = SM9_BYTES_LEN_G1;

	char data_value_SBorS1[SM9_BYTES_LEN_G1] = {0};
	int data_len_SBorS1 = SM9_BYTES_LEN_G1;

	char data_value_S2orSA[SM9_BYTES_LEN_G2] = {0};
	int data_len_S2orSA = SM9_BYTES_LEN_G2;

	char ida[4096] = {0};
	int ida_len = 4096;

	char idb[4096] = {0};
	int idb_len = 4096;

	char data_value_G1[SM9_BYTES_LEN_G1] = {0};
	int data_len_G1 = SM9_BYTES_LEN_G1;

	int key_len = 16;

	data_len = 4096;
	m_editkey_len.GetWindowText(data_value,data_len);

	key_len=atoi(data_value);

	if(0 == key_len)
	{
		MessageBox("未设置协商密钥长度！");
		return;
	}


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
	m_editr.GetWindowText(data_value,data_len);
	data_len = strlen(data_value);
	Hex2Bin(data_value,data_len,(unsigned char *)data_value_r,&data_len_r);

	if(0 == data_len_r)
	{
		MessageBox("未设置临时私钥！");
		return;
	}

	data_len = 4096;
	m_editSK.GetWindowText(data_value,data_len);
	data_len = strlen(data_value);
	Hex2Bin(data_value,data_len,(unsigned char *)data_value_SK,&data_len_SK);

	if(0 == data_len_r)
	{
		MessageBox("未设置己方私钥！");
		return;
	}

	data_len = 4096;
	m_editRA.GetWindowText(data_value,data_len);
	data_len = strlen(data_value);
	Hex2Bin(data_value,data_len,(unsigned char *)data_value_RA,&data_len_RA);

	if(0 == data_len_RA)
	{
		MessageBox("未设置RA！");
		return;
	}


	data_len = 4096;
	m_editRB.GetWindowText(data_value,data_len);
	data_len = strlen(data_value);
	Hex2Bin(data_value,data_len,(unsigned char *)data_value_RA,&data_len_RB);

	if(0 == data_len_RB)
	{
		MessageBox("未设置RB！");
		return;
	}

	data_len = 4096;
	m_editIDA.GetWindowText(data_value,data_len);
	data_len = strlen(data_value);
	Hex2Bin(data_value,data_len,(unsigned char *)ida,&ida_len);

	data_len = 4096;
	m_editIDB.GetWindowText(data_value,data_len);
	data_len = strlen(data_value);
	Hex2Bin(data_value,data_len,(unsigned char *)idb,&idb_len);

	if( 0 == sm9_keyExchange(data_value_G1,data_len_G1,
		data_value_SK,data_len_SK,
		ida,ida_len,
		idb,idb_len,
		key_len,
		data_value_r,data_len_r,
		data_value_RA,data_len_RA,
		data_value_RB,data_len_RB,
		data_value_SKAorSKB,&data_len_SKAorSKB,
		data_value_SBorS1,&data_len_SBorS1,
		data_value_S2orSA,&data_len_S2orSA,g_key_ex,SM9_KEY_EX_OPTION_YES
		))
	{
		MessageBox("协商成功！");
	}
	else
	{
		MessageBox("协商失败！");
		return;
	}

	data_len = 4096;
	Bin2Hex((unsigned char*)data_value_SKAorSKB,data_len_SKAorSKB,data_value,&data_len);
	m_editSKAorSKB.SetWindowText(data_value);

	data_len = 4096;
	Bin2Hex((unsigned char*)data_value_SBorS1,data_len_SBorS1,data_value,&data_len);
	m_editSBorS1.SetWindowText(data_value);


	data_len = 4096;
	Bin2Hex((unsigned char*)data_value_S2orSA,data_len_S2orSA,data_value,&data_len);
	m_editS2orSA.SetWindowText(data_value);
}
