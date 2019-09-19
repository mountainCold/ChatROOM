// ChatFriend.cpp: 实现文件
//

#include "pch.h"
#include "CFChat.h"
#include "ChatFriend.h"
#include "afxdialogex.h"


// ChatFriend 对话框

IMPLEMENT_DYNAMIC(ChatFriend, CDialogEx)
static int b = 0;
ChatFriend::ChatFriend(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG3, pParent)
	, m_MSG(_T(""))
{

}

ChatFriend::~ChatFriend()
{
}

void ChatFriend::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Text(pDX, IDC_EDIT1, m_MSG);
}


BEGIN_MESSAGE_MAP(ChatFriend, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON1, &ChatFriend::OnBnClickedButton1)
	ON_WM_COPYDATA()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// ChatFriend 消息处理程序


void ChatFriend::OnClose()
{
	b = 0;
	DestroyWindow();
	CDialogEx::OnClose();
}
void ChatFriend::OnBnClickedButton1()
{
	UpdateData(true);
	CString strTmp;
	SYSTEMTIME sysTime = { 0 };
	GetLocalTime(&sysTime);
	CStringA name = user.username;
	strTmp.Format(L"%s:%04d/%02d/%02d %02d:%02d:%02d", CATOCS(name),
		sysTime.wYear,
		sysTime.wMonth,
		sysTime.wDay,
		sysTime.wHour,
		sysTime.wMinute,
		sysTime.wSecond);

	m_ListBox.AddString(strTmp);
	USES_CONVERSION;

	char* Nextbuffer = NULL;
	char* status = NULL;
	status = strtok_s(T2A(m_MSG), "\r\n", &Nextbuffer);
	while (status != NULL)
	{
		m_ListBox.AddString(CATOCS(status));
		status = strtok_s(NULL, "\r\n", &Nextbuffer);
	}
	CString deName;
	this->GetWindowTextW(deName);
	::SendChatMSG(&client,CWTOCA(deName), CWTOCA(m_MSG), 0);
	m_MSG = "";
	UpdateData(false);
}


BOOL ChatFriend::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	hChatFriend = this->m_hWnd;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


BOOL ChatFriend::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	char* Nextbuffer = NULL;
	char* sta = NULL;
	char* status = NULL;
	char* buffa = NULL;
	status = strtok_s((char*)pCopyDataStruct->lpData, "\\", &Nextbuffer);
	buffa = strtok_s(NULL, "\\", &Nextbuffer);
	if (strcmp(status, "8") == 0)
	{
		CString strTmp;
		SYSTEMTIME sysTime = { 0 };
		GetLocalTime(&sysTime);
		CString deName;
		this->GetWindowTextW(deName);
		strTmp.Format(L"%s:%04d/%02d/%02d %02d:%02d:%02d", deName,
			sysTime.wYear,
			sysTime.wMonth,
			sysTime.wDay,
			sysTime.wHour,
			sysTime.wMinute,
			sysTime.wSecond);
		m_ListBox.AddString(strTmp);
		sta = strtok_s((char*)buffa, "\r\n", &Nextbuffer);
		while (sta !=NULL)
		{
			m_ListBox.AddString(CATOCS(sta));
			sta = strtok_s(NULL, "\r\n", &Nextbuffer);
		}
		
	
	}
	char* nexda = NULL;
	char*  date = NULL;
	char* dat1 = NULL;
	if (strcmp(status, "9") == 0)
	{
		m_ListBox.AddString(L"历史消息：");
		sta = strtok_s((char*)buffa, "\x11", &Nextbuffer);
		sta = strtok_s(NULL, "\x11", &Nextbuffer);
		while (sta !=NULL)
		{
			date=strtok_s((char*)sta, "\x10", &nexda);
			m_ListBox.AddString(CATOCS(date));
			dat1=strtok_s(NULL, "\x10", &nexda);
			decode(dat1);
			char* stra = strtok_s(dat1, "\r\n", &nexda);
			while (stra != NULL)
			{
				m_ListBox.AddString(CATOCS(stra));
				stra = strtok_s(NULL, "\r\n", &nexda);
			}
			sta = strtok_s(NULL, "\x11", &Nextbuffer);
		}
		m_ListBox.AddString(L"-------------------------");
	}

	return CDialogEx::OnCopyData(pWnd, pCopyDataStruct);
}


void ChatFriend::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	CString deName;
	this->GetWindowTextW(deName);
	b++;
	if (b==1)
	{
		::GetHistory(&client, CWTOCA(deName), 0);
	}

}
