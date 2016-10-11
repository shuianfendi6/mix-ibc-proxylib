// CharDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ibctools.h"
#include "SM9GenSKDlg.h"
#include "afxdialogex.h"

#include "sm9_proxylib_api.h"

extern void *g_gParams;
extern void *g_msk;
extern void *g_mpk;
extern void *g_sk;


// CSM9GenSKDlg dialog

IMPLEMENT_DYNAMIC(CSM9GenSKDlg, CDialogEx)

CSM9GenSKDlg::CSM9GenSKDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSM9GenSKDlg::IDD, pParent)
{

}

CSM9GenSKDlg::~CSM9GenSKDlg()
{
}

void CSM9GenSKDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_edithid01);
	DDX_Control(pDX, IDC_EDIT1, m_edithid02);
	DDX_Control(pDX, IDC_EDIT3, m_edithid03);
	DDX_Control(pDX, IDC_EDIT4, m_editSK);
}


BEGIN_MESSAGE_MAP(CSM9GenSKDlg, CDialogEx)
	ON_BN_CLICKED(3, &CSM9GenSKDlg::OnBnClicked3)
	ON_BN_CLICKED(2, &CSM9GenSKDlg::OnBnClicked2)
END_MESSAGE_MAP()


// CSM9GenSKDlg message handlers


void CSM9GenSKDlg::OnBnClicked3()
{
	// TODO: Add your control notification handler code here

	char data_value[4096] = {0};
	int data_len = 4096;

	char data_value_hid01[128+1] = {0};
	int data_len_hid01 = 128+1;

	char data_value_hid02[256+1] = {0};
	int data_len_hid02 = 256+1;

	char data_value_hid03[256+1] = {0};
	int data_len_hid03 = 256+1;

	void *gParams = 0;
	void *msk = 0;
	void *mpk = 0;

	if(0 == g_msk)
	{
		MessageBox("未设置主私钥！");
		return;
	}

	if(0 == g_sk)
	{
		MessageBox("未设置用户私钥！");
		return;
	}

	sm9_proxylib_generateParams(&gParams,SM9_SCHEME_SW);

	data_len = 4096;

	sm9_proxylib_getSerializeObjectSize(g_sk, SM9_SERIALIZE_HEXASCII, &data_len);
	sm9_proxylib_serializeObject(g_sk,data_value, &data_len, data_len, SM9_SERIALIZE_HEXASCII);
	sm9_proxylib_destroyObject(g_sk);
	sm9_proxylib_deserializeObject(data_value, data_len, &g_sk,SM9_SERIALIZE_HEXASCII);

	m_edithid01.GetWindowText(data_value_hid01,data_len_hid01);
	data_len_hid01 = strlen(data_value_hid01);

	m_edithid02.GetWindowText(data_value_hid02,data_len_hid02);
	data_len_hid02 = strlen(data_value_hid02);

	m_edithid03.GetWindowText(data_value_hid03,data_len_hid03);
	data_len_hid03 = strlen(data_value_hid03);

	int pos = 0;

	// len
	pos += 8;
	// type
	pos += 2;

	if (0 != data_len_hid01 && data_len_hid01 != 128)
	{
		MessageBox("签名私钥格式不正确！");
		return;
	}
	else
	{
		if(data_len_hid01 == 128)
		{
			pos += 8;
			memcpy(data_value+pos,data_value_hid01,data_len_hid01/2);
			pos += data_len_hid01/2;
			pos += 8;
			memcpy(data_value+pos,data_value_hid01+data_len_hid01/2,data_len_hid01/2);
			pos += data_len_hid01/2;
		}
		else
		{
			pos += 8;
			pos += 64;
			pos += 8;
			pos += 64;
		}
	}


	if (0 != data_len_hid02 && data_len_hid02 != 256)
	{
		MessageBox("交换私钥格式不正确！");
		return;
	}
	else
	{
		if(data_len_hid02 == 256)
		{
			pos += 8;
			memcpy(data_value+pos,data_value_hid02,data_len_hid02/4);
			pos += data_len_hid02/4;
			pos += 8;
			memcpy(data_value+pos,data_value_hid02+data_len_hid02/4*1,data_len_hid02/4);
			pos += data_len_hid02/4;

			pos += 8;
			memcpy(data_value+pos,data_value_hid02+data_len_hid02/4*2,data_len_hid02/4);
			pos += data_len_hid02/4;
			pos += 8;
			memcpy(data_value+pos,data_value_hid02+data_len_hid02/4*3,data_len_hid02/4);
			pos += data_len_hid02/4;
		}
		else
		{
			pos += 8;
			pos += 64;
			pos += 8;
			pos += 64;
			pos += 8;
			pos += 64;
			pos += 8;
			pos += 64;
		}
	}

	if (0 != data_len_hid03 && data_len_hid03 != 256)
	{
		MessageBox("加密私钥格式不正确！");
		return;
	}
	else
	{
		if(data_len_hid03 == 256)
		{
			pos += 8;
			memcpy(data_value+pos,data_value_hid03,data_len_hid03/4);
			pos += data_len_hid03/4;
			pos += 8;
			memcpy(data_value+pos,data_value_hid03+data_len_hid03/4*1,data_len_hid03/4);
			pos += data_len_hid03/4;

			pos += 8;
			memcpy(data_value+pos,data_value_hid03+data_len_hid03/4*2,data_len_hid03/4);
			pos += data_len_hid03/4;
			pos += 8;
			memcpy(data_value+pos,data_value_hid03+data_len_hid03/4*3,data_len_hid03/4);
			pos += data_len_hid03/4;
		}
		else
		{
			pos += 8;
			pos += 64;
			pos += 8;
			pos += 64;
			pos += 8;
			pos += 64;
			pos += 8;
			pos += 64;
		}
	}

	m_editSK.SetWindowText(data_value);
}


void CSM9GenSKDlg::OnBnClicked2()
{
	char data_value[4096] = {0};
	int data_len = 4096;

	char data_value2[4096] = {0};
	int data_len2 = 4096;

	void *gParams = 0;

	sm9_proxylib_generateParams(&gParams,SM9_SCHEME_SW);

	m_editSK.GetWindowText(data_value,data_len);
	data_len = strlen(data_value);


	if (g_sk)
	{
		sm9_proxylib_destroyObject(g_sk);
		g_sk = 0;
	}

	if( 0 == sm9_proxylib_deserializeObject(data_value, data_len, &g_sk,SM9_SERIALIZE_HEXASCII))
	{
		MessageBox("设置用户私钥成功！");
	}
	else
	{
		MessageBox("用户私钥格式不正确，设置失败！");
		return;
	}
}
