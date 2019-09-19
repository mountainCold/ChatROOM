
// CFChatDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "CFChat.h"
#include "CFChatDlg.h"
#include "CFRIEDLG.h"
#include "CROOMDLG.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "CLogin.h"
#include "Client.h"


// CCFChatDlg 对话框



CCFChatDlg::CCFChatDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CFCHAT_DIALOG, pParent)
	, username(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCFChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, UseTab);
	DDX_Text(pDX, IDC_STATIC33, username);
}

BEGIN_MESSAGE_MAP(CCFChatDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CCFChatDlg 消息处理程序

BOOL CCFChatDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	hWin = this->m_hWnd;

	CLogin cdlg;
	cdlg.DoModal();
	if (g_isConnet == true)
	{
		if (g_isLogin != true)
		{
			SendMessage(WM_CLOSE, 0, 0);
			return true;
		}
		username = user.username;
		UpdateData(false);
		::UpdateData(&client, 0);
		recvProc(&client);
		::UpdateData(&client, 1);
		recvProc(&client);
		UseTab.InsertTabWnd(2, new CFRIEDLG, IDD_DLGFRI, new CROOMDLG, IDD_DIAROOM);
		CreateThread(0, 0, ::recvProc1, &client, 0, 0);
		
	}
	else
	{
		SendMessage(WM_CLOSE, 0, 0);
	}
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCFChatDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCFChatDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


