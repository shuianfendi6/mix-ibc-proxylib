// Base64Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "ibctools.h"
#include "SM9GenMPKDlg.h"
#include "afxdialogex.h"

#include "sm9_proxylib_api.h"

extern void *g_gParams;
extern void *g_msk;
extern void *g_mpk;
extern void *g_mpk;

extern "C" unsigned long Hex2Bin(const char *pbIN,int ulINLen,unsigned char *pbOUT,int * pulOUTLen);
extern "C" unsigned long Bin2Hex(const unsigned char *pbIN,int ulINLen,char *pbOUT,int * pulOUTLen);

// CSM9GenMPKDlg dialog

IMPLEMENT_DYNAMIC(CSM9GenMPKDlg, CDialogEx)

CSM9GenMPKDlg::CSM9GenMPKDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSM9GenMPKDlg::IDD, pParent)
{

}

CSM9GenMPKDlg::~CSM9GenMPKDlg()
{
}

void CSM9GenMPKDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_editG1);
	DDX_Control(pDX, IDC_EDIT1, m_editG2);
	DDX_Control(pDX, IDC_EDIT4, m_editMPK);
}


BEGIN_MESSAGE_MAP(CSM9GenMPKDlg, CDialogEx)
	ON_BN_CLICKED(3, &CSM9GenMPKDlg::OnBnClicked3)
	ON_BN_CLICKED(2, &CSM9GenMPKDlg::OnBnClicked2)
END_MESSAGE_MAP()


// CSM9GenMPKDlg message handlers


void CSM9GenMPKDlg::OnBnClicked3()
{
	// TODO: Add your control notification handler code here
	char data_value[4096] = {0};
	int data_len = 4096;

	char data_value_G1[128+1] = {0};
	int data_len_G1 = 128+1;

	char data_value_G2[256+1] = {0};
	int data_len_G2 = 256+1;

	void *gParams = 0;
	void *msk = 0;
	void *mpk = 0;

	sm9_proxylib_generateParams(&gParams,SM9_SCHEME_SW);

	data_len = 4096;
	m_editG1.GetWindowText(data_value,data_len);
	data_len = strlen(data_value);

	Hex2Bin(data_value,data_len,(unsigned char *)data_value_G1,&data_len_G1);

	data_len = 4096;
	m_editG2.GetWindowText(data_value,data_len);
	data_len = strlen(data_value);

	Hex2Bin(data_value,data_len,(unsigned char *)data_value_G2,&data_len_G2);

	if (0 != data_len_G1 && data_len_G1 != 64)
	{
		MessageBox("G1格式不正确！");
		return;
	}

	if (0 != data_len_G2 && data_len_G2 != 128)
	{
		MessageBox("G2格式不正确！");
		return;
	}

	sm9_proxylib_ObjectFromItemsValueMPK(&mpk,data_len_G1==0? 0:data_value_G1,data_len_G2==0? 0:data_value_G2);
	
	data_len = 4096;

	sm9_proxylib_serializeObject(mpk,data_value, &data_len, data_len, SM9_SERIALIZE_HEXASCII);

	m_editMPK.SetWindowText(data_value);
}


void CSM9GenMPKDlg::OnBnClicked2()
{
	char data_value[4096] = {0};
	int data_len = 4096;

	data_len = 4096;

	m_editMPK.GetWindowText(data_value,data_len);
	data_len = strlen(data_value);

	if (g_mpk)
	{
		sm9_proxylib_destroyObject(g_mpk);
		g_mpk = 0;
	}

	if(0 == sm9_proxylib_deserializeObject(data_value, data_len, &g_mpk,SM9_SERIALIZE_HEXASCII))
	{
		MessageBox("设置主公钥成功！");
	}
	else
	{
		MessageBox("主公钥格式不正确，设置失败！");
		return;
	}
}
