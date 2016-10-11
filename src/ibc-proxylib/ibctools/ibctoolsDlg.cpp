
// ibctoolsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ibctools.h"
#include "ibctoolsDlg.h"
#include "afxdialogex.h"

#include "SM9GenMPKDlg.h"
#include "SM9GenSKDlg.h"
#include "SM9CalcDlg.h"
#include "SM9DecryptDlg.h"
#include "SM9EncryptDlg.h"
#include "SM9GenDlg.h"
#include "SM9SetupDlg.h"
#include "SM9SignDlg.h"
#include "SM9VerifyDlg.h"

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
}

BEGIN_MESSAGE_MAP(CibctoolsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_ALL, &CibctoolsDlg::OnTcnSelchangeTabAll)
END_MESSAGE_MAP()


// CibctoolsDlg message handlers

BOOL CibctoolsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵������ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ����Ӷ���ĳ�ʼ������

	m_tb.InsertItem(0, "��������Կ");
	m_tb.InsertItem(1, "����Կ���");
	m_tb.InsertItem(2, "��������Կ");
	m_tb.InsertItem(3, "����˽Կ");
	m_tb.InsertItem(4, "�û�˽Կ���");
	m_tb.InsertItem(5, "����");
	m_tb.InsertItem(6, "����");
	m_tb.InsertItem(7, "ǩ��");
	m_tb.InsertItem(8, "��֤");

	//���������Ի���
	//m_page1.Create(IDD_DIALOG, &m_tb);
	//m_page2.Create(IDD_DIALOG_CERTUI_PROP_DETAIL, &m_tb);
	//m_page3.Create(IDD_DIALOG_CERTUI_PROP_PATH, &m_tb);

	//�趨��Tab����ʾ�ķ�Χ
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
			pDialog[i] = new CSM9GenMPKDlg(this);

			pDialog[i]->Create(IDD_DIALOG_SM9GEN_MPK, &m_tb);
			break;

		case 2:
			pDialog[i] = new CSM9SetupDlg(this);

			pDialog[i]->Create(IDD_DIALOG_SM9SETUP, &m_tb);
			break;

		case 3:
			pDialog[i] = new CSM9CalcDlg(this);

			pDialog[i]->Create(IDD_DIALOG_SM9CALC, &m_tb);
			break;

		case 4:

			pDialog[i] = new CSM9GenSKDlg(this);

			pDialog[i]->Create(IDD_DIALOG_SM9GEN_SK, &m_tb);
			break;

		case 5:
			pDialog[i] = new CSM9EncryptDlg(this);

			pDialog[i]->Create(IDD_DIALOG_SM9ENCRYPT, &m_tb);
			break;

		case 6:
			pDialog[i] = new CSM9DecryptDlg(this);

			pDialog[i]->Create(IDD_DIALOG_SM9DECRYPT, &m_tb);
			break;

		case 7:
			pDialog[i] = new CSM9SignDlg(this);

			pDialog[i]->Create(IDD_DIALOG_SM9SIGN, &m_tb);
			break;

		case 8:
			pDialog[i] = new CSM9VerifyDlg(this);

			pDialog[i]->Create(IDD_DIALOG_SM9VERIFY, &m_tb);
			break;

		default:
			pDialog[i] = new CDialog();

			break;
		}

		if (i < 9)
		{
			pDialog[i]->MoveWindow(&rc);
			pDialog[i]->ShowWindow(SW_HIDE);
		}

	}

	pos = -1;

	//��ʾ��ʼҳ��
	pDialog[0]->ShowWindow(SW_SHOW);

	//���浱ǰѡ��
	m_CurSelTab = 0;

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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


void CibctoolsDlg::OnTcnSelchangeTabAll(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	//�ѵ�ǰ��ҳ����������
	pDialog[m_CurSelTab]->ShowWindow(SW_HIDE);
	//�õ��µ�ҳ������
	m_CurSelTab = m_tb.GetCurSel();
	//���µ�ҳ����ʾ����
	pDialog[m_CurSelTab]->ShowWindow(SW_SHOW);
	*pResult = 0;
}