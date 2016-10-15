// SM9KeyExPreDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ibctools.h"
#include "SM9KeyExPreDlg.h"
#include "afxdialogex.h"


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
}


BEGIN_MESSAGE_MAP(CSM9KeyExPreDlg, CDialogEx)
	ON_BN_CLICKED(4, &CSM9KeyExPreDlg::OnBnClicked4)
END_MESSAGE_MAP()


// CSM9KeyExPreDlg 消息处理程序


void CSM9KeyExPreDlg::OnBnClicked4()
{
	
}
