// CLogin.cpp: 实现文件
//

#include "pch.h"
#include "CFChat.h"
#include "CLogin.h"
#include "afxdialogex.h"
#include "methon.h"
#include "Client.h"
#include "CRegister.h"

// CLogin 对话框

IMPLEMENT_DYNAMIC(CLogin, CDialogEx)

CLogin::CLogin(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, username(_T(""))
	, password(_T(""))
{

}

CLogin::~CLogin()
{
}

void CLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, username);
	DDX_Text(pDX, IDC_EDIT2, password);
}


BEGIN_MESSAGE_MAP(CLogin, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CLogin::OnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CLogin::OnClickedButton2)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CLogin 消息处理程序


void CLogin::OnClickedButton1()
{
	if (g_isConnet == false) return;
	UpdateData(true);
	USES_CONVERSION;
	::Login(&client, (char*)T2A(username), (char*)T2A(password));
	recvProc(&client);
	if (g_isLogin==true)
	{
		user.username = username;
		g_isLogin = true;	
		SendMessage(WM_CLOSE, 0, 0);
	}
}


void CLogin::OnClickedButton2()
{
	if (g_isConnet == false) return;
	ShowWindow(SW_HIDE);
	CRegister rdig;
	rdig.DoModal();
	ShowWindow(SW_SHOW);
}


void CLogin::OnClose()
{


	CDialogEx::OnClose();

}
