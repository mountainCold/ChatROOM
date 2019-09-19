// ChatROOM.cpp: 实现文件
//

#include "pch.h"
#include "CFChat.h"
#include "ChatROOM.h"
#include "afxdialogex.h"


// ChatROOM 对话框

IMPLEMENT_DYNAMIC(ChatROOM, CDialogEx)
static int a = 0;
ChatROOM::ChatROOM(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG4, pParent)
	, m_Msg(_T(""))
{

}

ChatROOM::~ChatROOM()
{
}

void ChatROOM::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Msg);
	DDX_Control(pDX, IDC_LIST1, m_listBox);
	DDX_Control(pDX, IDC_LIST2, m_roomlist);
}


BEGIN_MESSAGE_MAP(ChatROOM, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON1, &ChatROOM::OnClickedButton1)
	ON_WM_COPYDATA()
END_MESSAGE_MAP()


// ChatROOM 消息处理程序


void ChatROOM::OnClose()
{
	a = 0;
	DestroyWindow();

	CDialogEx::OnClose();
}


void ChatROOM::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	CString deName;
	this->GetWindowTextW(deName);
	a++;
	if (a==1)
	{
		::GetRoomMembers(&client, CWTOCA(deName));
		::GetHistory(&client, CWTOCA(deName), 1);
	}

}


BOOL ChatROOM::OnInitDialog()
{

	CDialogEx::OnInitDialog();
	hChatRoom = this->m_hWnd;
	m_roomlist.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_roomlist.InsertColumn(0, L"成员", 0, 50);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void ChatROOM::OnClickedButton1()
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
	m_listBox.AddString(strTmp);
	USES_CONVERSION;

	char* Nextbuffer = NULL;
	char* status = NULL;
	status = strtok_s(T2A(m_Msg), "\r\n", &Nextbuffer);
	while (status != NULL)
	{
		m_listBox.AddString(CATOCS(status));
		status = strtok_s(NULL, "\r\n", &Nextbuffer);
	}
	CString deName;
	this->GetWindowTextW(deName);
	::SendChatMSG(&client, CWTOCA(deName), CWTOCA(m_Msg), 1);
	m_Msg = "";
	UpdateData(false);
}


BOOL ChatROOM::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	char* Nextbuffer = NULL;
	char* status = NULL;
	char* buffa = NULL;
	status = strtok_s((char*)pCopyDataStruct->lpData, "\\", &Nextbuffer);
	buffa = strtok_s(NULL, "\\", &Nextbuffer);
	if (strcmp(status, "13") == 0)//群成员消息
	{
		int i = 0;
		while (buffa != NULL)
		{
			m_roomlist.InsertItem(i, L"");
			m_roomlist.SetItemText(i, 0, CATOCS(buffa));
			buffa = strtok_s(NULL, "\\", &Nextbuffer);
			++i;
		}
	}
	else if (strcmp(status, "8") == 0)
	{
		char* str = NULL;
		str = strtok_s(buffa, "\x10", &Nextbuffer);
		m_listBox.AddString(CATOCS(str));
		str = strtok_s(NULL, "\x10", &Nextbuffer);
		decode(str);
		str = strtok_s(str, "\r\n", &Nextbuffer);
		while (str!=NULL)
		{
			m_listBox.AddString(CATOCS(str));
			str = strtok_s(NULL, "\r\n", &Nextbuffer);
		}
		
	}
	if (strcmp(status, "9") == 0)
	{
		char* sta = NULL;
		m_listBox.AddString(L"历史消息：");
		sta = strtok_s((char*)buffa, "\x11", &Nextbuffer);
		sta = strtok_s(NULL, "\x11", &Nextbuffer);
		while (sta != NULL)
		{
			char* nexda = NULL;
			char* date = NULL;
			char* dat1 = NULL;
			date = strtok_s((char*)sta, "\x10", &nexda);
			m_listBox.AddString(CATOCS(date));
			dat1 = strtok_s(NULL, "\x10", &nexda);
			decode(dat1);
			char* stra = strtok_s(dat1, "\r\n", &nexda);
			while (stra != NULL)
			{
				m_listBox.AddString(CATOCS(stra));
				stra = strtok_s(NULL, "\r\n", &nexda);
			}
			sta = strtok_s(NULL, "\x11", &Nextbuffer);
		}
		m_listBox.AddString(L"-------------------------");
	}
	return CDialogEx::OnCopyData(pWnd, pCopyDataStruct);
}
