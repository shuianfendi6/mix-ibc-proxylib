// SM9SignDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ibctools.h"
#include "SM9SignDlg.h"
#include "afxdialogex.h"

#include "sm9_proxylib_api.h"

extern void *g_gParams;
extern void *g_msk;
extern void *g_mpk;
extern void *g_sk;
extern char g_id[1024];
extern int g_id_len;

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
}


BEGIN_MESSAGE_MAP(CSM9SignDlg, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT2, &CSM9SignDlg::OnEnChangeEdit2)
	ON_BN_CLICKED(2, &CSM9SignDlg::OnBnClicked2)
	ON_BN_CLICKED(3, &CSM9SignDlg::OnBnClicked3)
END_MESSAGE_MAP()


// CSM9SignDlg message handlers


void CSM9SignDlg::OnEnChangeEdit2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


extern "C" unsigned long Hex2Bin(const char *pbIN,int ulINLen,unsigned char *pbOUT,int * pulOUTLen);
extern "C" unsigned long Bin2Hex(const unsigned char *pbIN,int ulINLen,char *pbOUT,int * pulOUTLen);


void CSM9SignDlg::OnBnClicked2()
{
	char data_value[4096] = {0};
	int data_len = 4096;

	char data_value2[4096] = {0};
	int data_len2 = 4096;

	void *gParams = 0;
	void *sgn = 0;


	if(0 == g_mpk)
	{
		MessageBox("未设置主公钥！");
		return;
	}


	if(0 == g_sk)
	{
		MessageBox("未设置用户私钥！");
		return;
	}


	sm9_proxylib_generateParams(&gParams,SM9_SCHEME_SW);

	data_len = 4096;

	m_editIn.GetWindowText(data_value,data_len);
	data_len = strlen(data_value);

	data_len2 = data_len;

	Hex2Bin(data_value,data_len,(unsigned char *)data_value2,&data_len2);

	if(0 == sm9_proxylib_sign(gParams,g_mpk,g_sk,data_value2,data_len2,&sgn,SM9_SCHEME_SW))
	{
		MessageBox("签名成功！");
	}
	else
	{
		MessageBox("签名失败！");
		return;
	}

	data_len = 4096;
	sm9_proxylib_getSerializeObjectSize(sgn, SM9_SERIALIZE_HEXASCII, &data_len);
	sm9_proxylib_serializeObject(sgn,data_value, &data_len, data_len, SM9_SERIALIZE_HEXASCII);
	sm9_proxylib_destroyObject(sgn);
	sm9_proxylib_deserializeObject(data_value, data_len, &sgn,SM9_SERIALIZE_HEXASCII);

	m_editOut.SetWindowText(data_value);
}



void CSM9SignDlg::OnBnClicked3()
{
	// TODO: Add your control notification handler code here
	char data_value[4096] = {0};
	int data_len = 4096;

	char data_value2[4096] = {0};
	int data_len2 = 4096;

	int pos = 0;

	data_len = 4096;

	m_editIn.GetWindowText(data_value,data_len);
	data_len = strlen(data_value);

	// len
	pos += 8;
	// type
	pos += 2;

	if (data_len < 32 * 3 * 2)
	{
		MessageBox("输入格式不正确！");
		return;
	}
	else
	{
		pos += 8;
		memcpy(data_value2,data_value+pos,64);
		pos += 64;
		pos += 8;
		memcpy(data_value2+64*1,data_value+pos,64);
		pos += 64;
		pos += 8;
		memcpy(data_value2+64*2,data_value+pos,64);
		pos += 64;
	}

	m_editOut.SetWindowText(data_value2);

}
