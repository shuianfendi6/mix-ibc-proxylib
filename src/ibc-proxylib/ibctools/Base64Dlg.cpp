// Base64Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "ibctools.h"
#include "Base64Dlg.h"
#include "afxdialogex.h"


// CBase64Dlg dialog

IMPLEMENT_DYNAMIC(CBase64Dlg, CDialogEx)

CBase64Dlg::CBase64Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBase64Dlg::IDD, pParent)
{

}

CBase64Dlg::~CBase64Dlg()
{
}

void CBase64Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBase64Dlg, CDialogEx)
END_MESSAGE_MAP()


// CBase64Dlg message handlers
