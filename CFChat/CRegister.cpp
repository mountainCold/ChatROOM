// CRegister.cpp: 实现文件
//

#include "pch.h"
#include "CFChat.h"
#include "CRegister.h"
#include "afxdialogex.h"
#include <regex>


// CRegister 对话框

IMPLEMENT_DYNAMIC(CRegister, CDialogEx)

CRegister::CRegister(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
	, username(_T(""))
	, password(_T(""))
	, rPassWord(_T(""))
	, phone(_T(""))
{

}

CRegister::~CRegister()
{
}

void CRegister::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, username);
	DDX_Text(pDX, IDC_EDIT2, password);
	DDX_Text(pDX, IDC_EDIT3, rPassWord);
	DDX_Text(pDX, IDC_EDIT4, phone);
}


BEGIN_MESSAGE_MAP(CRegister, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CRegister::OnClickedButton1)
END_MESSAGE_MAP()


// CRegister 消息处理程序


void CRegister::OnClickedButton1()
{
	UpdateData(true);
	if (username.IsEmpty()||username==""||password==""||password.IsEmpty())
	{
		MessageBox(L"用户名或密码不能为空");
		return;
	}
	if (password.CompareNoCase(rPassWord))
	{
		MessageBox(L"两次输入密码不一致");
		return;
	}	
	USES_CONVERSION;
	regex e("^[1]([3-9])[0-9]{9}$");
	if (regex_match((char*)T2A(phone),e)!=true)
	{
		MessageBox(L"请输入正确的手机号格式");
		return;
	}
	::Register(&client, (char*)T2A(username), (char*)T2A(password), (char*)T2A(phone));
	recvProc(&client);
	if (g_TFlag)
	{
		MessageBox(L"注册成功，按确认返回登录界面");
		SendMessage(WM_CLOSE, 0, 0);
	}
	
	
}
