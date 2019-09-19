// CFRIEDLG.cpp: 实现文件
//

#include "pch.h"
#include "CFChat.h"
#include "CFRIEDLG.h"
#include "afxdialogex.h"
#include "ChatFriend.h"


// CFRIEDLG 对话框

IMPLEMENT_DYNAMIC(CFRIEDLG, CDialogEx)
CFRIEDLG::CFRIEDLG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLGFRI, pParent)
	, friendName(_T(""))
{

}

CFRIEDLG::~CFRIEDLG()
{
}

void CFRIEDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTFRIE, m_friendList);
	DDX_Text(pDX, IDC_EDIT1, friendName);
}


BEGIN_MESSAGE_MAP(CFRIEDLG, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CFRIEDLG::OnClickedButton1)
	ON_WM_COPYDATA()
	ON_NOTIFY(NM_DBLCLK, IDC_LISTFRIE, &CFRIEDLG::OnDblclkListfrie)
END_MESSAGE_MAP()


// CFRIEDLG 消息处理程序


BOOL CFRIEDLG::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	hFriend = this->m_hWnd;
	m_friendList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_friendList.InsertColumn(0, L"账户", 0, 300);
	UpdateFriendList();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CFRIEDLG::UpdateFriendList()
{

	if (OldList.size()==0)
	{
		for (int i = 0; i < user.friendList.size(); i++)
		{
			m_friendList.InsertItem(i, L"");
			m_friendList.SetItemText(i, 0, CATOCS(user.friendList[i]));
		}
	}
	else if(OldList.size()> user.friendList.size())
	{
		//int index = 0;

		//删除
		for (size_t i = 0; i < OldList.size(); i++)
		{
			bool del = true;
			for (size_t j = 0; j < user.friendList.size(); j++)
			{
				if(OldList[i].CompareNoCase(user.friendList[j])==0)
				{
					del = false;
				}
				else
				{
					continue;
				}
			}
			if (del)
			{
				m_friendList.DeleteItem(i);
			}
		}
		
	}
	else if (OldList.size() < user.friendList.size())
	{
		//新增

		for (size_t i = 0; i < user.friendList.size(); i++)
		{
			bool ins = true;
			for (size_t j = 0; j < OldList.size(); j++)
			{
				if (OldList[j].CompareNoCase(user.friendList[i]) == 0)
				{
					ins = false;
				}
				else
				{
					continue;
				}
			}
			if (ins)
			{
				m_friendList.InsertItem(i, L"");
				
				m_friendList.SetItemText(i, 0, CATOCS(user.friendList[i]));
			}
		}
	}
	
	OldList=user.friendList;

}


void CFRIEDLG::OnClickedButton1()
{
	UpdateData(true);
	USES_CONVERSION;
	AddFriend(&client, (char*)T2A(friendName));
}


BOOL CFRIEDLG::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	char* Nextbuffer = NULL;
	char* type = NULL;
	char* status = NULL;
	char* buffa = NULL;
	type = strtok_s((char*)pCopyDataStruct->lpData, "\\", &Nextbuffer);
	status = strtok_s(NULL, "\\", &Nextbuffer);
	buffa = strtok_s(NULL, "\\", &Nextbuffer);
	if (strcmp(type,"4")==0)
	{
		if (strcmp(status, "0") == 0)
		{
			CStringA buff;
			buff.Format("%s", buffa);
			user.friendList.push_back(buff);
			UpdateFriendList();
		}
	}
	return CDialogEx::OnCopyData(pWnd, pCopyDataStruct);
}


void CFRIEDLG::OnDblclkListfrie(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	int index = (int)m_friendList.GetFirstSelectedItemPosition();
	if (index == 0) {
		return;
	}
	index -= 1;
	CStringA name = user.friendList[index];
	int len = MultiByteToWideChar(CP_ACP, 0, name, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len];
	memset(wstr, 0, len * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, name, -1, wstr, len);
	CStringW wName = wstr;
	CDialog* cfDlg = new ChatFriend;
	cfDlg->Create(IDD_DIALOG3);
	cfDlg->SetWindowText(wName);
	cfDlg->ShowWindow(SW_SHOWNORMAL);


}
