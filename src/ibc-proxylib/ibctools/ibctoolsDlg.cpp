
// ibctoolsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ibctools.h"
#include "ibctoolsDlg.h"
#include "afxdialogex.h"
#include "SM9CalcDlg.h"
#include "SM9DecryptDlg.h"
#include "SM9EncryptDlg.h"
#include "SM9GenDlg.h"
#include "SM9SignDlg.h"
#include "SM9VerifyDlg.h"
//#include "SM9WrapDlg.h"
//#include "SM9UnwrapDlg.h"

#include "SM9KeyExPreDlg.h"
#include "SM9KeyExDlg.h"
#include "sm9_proxylib_api.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CibctoolsDlg dialog




CibctoolsDlg::CibctoolsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CibctoolsDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CibctoolsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_ALL, m_tb);
	DDX_Control(pDX, IDC_COMBO3, m_comboCryptoMode);
	DDX_Control(pDX, IDC_COMBO4, m_comboKeyEX);
}

BEGIN_MESSAGE_MAP(CibctoolsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_ALL, &CibctoolsDlg::OnTcnSelchangeTabAll)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CibctoolsDlg::OnCbnSelchangeCombo3)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CibctoolsDlg::OnCbnSelchangeCombo4)
END_MESSAGE_MAP()


// CibctoolsDlg message handlers

BOOL CibctoolsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	m_tb.InsertItem(0, "生成主密钥");
	m_tb.InsertItem(1, "计算私钥");
	m_tb.InsertItem(2, "加密");
	m_tb.InsertItem(3, "解密");
	m_tb.InsertItem(4, "签名");
	m_tb.InsertItem(5, "验证");
	//m_tb.InsertItem(6, "封装");
	//m_tb.InsertItem(7, "解封");

	m_tb.InsertItem(6, "密钥交换预处理");
	m_tb.InsertItem(7, "密钥交换");


	//创建两个对话框
	//m_page1.Create(IDD_DIALOG, &m_tb);
	//m_page2.Create(IDD_DIALOG_CERTUI_PROP_DETAIL, &m_tb);
	//m_page3.Create(IDD_DIALOG_CERTUI_PROP_PATH, &m_tb);

	//设定在Tab内显示的范围
	CRect rc;
	m_tb.GetClientRect(rc);
	rc.top += 20;
	rc.bottom -= 8;
	rc.left += 8;
	rc.right -= 8;

	for (int i = 0; i < sizeof(pDialog)/sizeof(char *); i++)
	{
		switch(i)
		{

		case 0:
			pDialog[i] = new CSM9GenDlg(this);

			pDialog[i]->Create(IDD_DIALOG_SM9GEN, &m_tb);
			break;
		case 1:
			pDialog[i] = new CSM9CalcDlg(this);

			pDialog[i]->Create(IDD_DIALOG_SM9CALC, &m_tb);
			break;
		case 2:
			pDialog[i] = new CSM9EncryptDlg(this);

			pDialog[i]->Create(IDD_DIALOG_SM9ENCRYPT, &m_tb);
			break;

		case 3:
			pDialog[i] = new CSM9DecryptDlg(this);

			pDialog[i]->Create(IDD_DIALOG_SM9DECRYPT, &m_tb);
			break;

		case 4:
			pDialog[i] = new CSM9SignDlg(this);

			pDialog[i]->Create(IDD_DIALOG_SM9SIGN, &m_tb);
			break;

		case 5:
			pDialog[i] = new CSM9VerifyDlg(this);

			pDialog[i]->Create(IDD_DIALOG_SM9VERIFY, &m_tb);
			break;
		/*case 6:
			pDialog[i] = new CSM9WrapDlg(this);

			pDialog[i]->Create(IDD_DIALOG_SM9WRAP, &m_tb);
			break;

		case 7:
			pDialog[i] = new CSM9UnwrapDlg(this);

			pDialog[i]->Create(IDD_DIALOG_SM9UNWRAP, &m_tb);
			break;*/

		case 6:
			pDialog[i] = new CSM9KeyExPreDlg(this);

			pDialog[i]->Create(IDD_DIALOG_SM9KEY_EX_PRE, &m_tb);
			break;
		case 7:
			pDialog[i] = new CSM9KeyExDlg(this);

			pDialog[i]->Create(IDD_DIALOG_SM9KEY_EX, &m_tb);
			break;	


		default:
			pDialog[i] = new CDialog();

			break;
		}

		if (i < 8)
		{
			pDialog[i]->MoveWindow(&rc);
			pDialog[i]->ShowWindow(SW_HIDE);
		}

	}

	pos = -1;


	pos = m_comboCryptoMode.InsertString(pos + 1,"基于KDF");
	pos = m_comboCryptoMode.InsertString(pos + 1,"结合KDF");

	m_comboCryptoMode.SetCurSel(0);

	pos = -1;


	pos = m_comboKeyEX.InsertString(pos + 1,"发起方");
	pos = m_comboKeyEX.InsertString(pos + 1,"响应方");

	m_comboKeyEX.SetCurSel(0);

	//显示初始页面
	pDialog[0]->ShowWindow(SW_SHOW);

	//保存当前选择
	m_CurSelTab = 0;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CibctoolsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CibctoolsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CibctoolsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

SM9_CIPHER_TYPE g_cryptoMode;

extern SM9_CIPHER_TYPE g_cryptoMode;

int g_key_ex = 0;

extern int g_key_ex;


void CibctoolsDlg::OnTcnSelchangeTabAll(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	//把当前的页面隐藏起来
	pDialog[m_CurSelTab]->ShowWindow(SW_HIDE);
	//得到新的页面索引
	m_CurSelTab = m_tb.GetCurSel();
	//把新的页面显示出来
	pDialog[m_CurSelTab]->ShowWindow(SW_SHOW);
	*pResult = 0;
}


void CibctoolsDlg::OnCbnSelchangeCombo3()
{
	if (m_comboCryptoMode.GetCurSel() == 0)
	{
		g_cryptoMode = SM9_CIPHER_KDF_BASE;
	}
	else if (m_comboCryptoMode.GetCurSel() == 1)
	{
		g_cryptoMode = SM9_CIPHER_KDF_UNION;
	}
	else
	{
		g_cryptoMode = SM9_CIPHER_KDF_BASE;
	}
}


void CibctoolsDlg::OnCbnSelchangeCombo4()
{
	if (m_comboKeyEX.GetCurSel() == 0)
	{
		g_key_ex = 0;
	}
	else if (m_comboKeyEX.GetCurSel() == 1)
	{
		g_key_ex = 1;
	}
	else
	{
		g_key_ex = 1;
	}
}
