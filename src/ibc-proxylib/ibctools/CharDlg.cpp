// CharDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ibctools.h"
#include "CharDlg.h"
#include "afxdialogex.h"


// CCharDlg dialog

IMPLEMENT_DYNAMIC(CCharDlg, CDialogEx)

CCharDlg::CCharDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCharDlg::IDD, pParent)
{

}

CCharDlg::~CCharDlg()
{
}

void CCharDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCharDlg, CDialogEx)
END_MESSAGE_MAP()


// CCharDlg message handlers
