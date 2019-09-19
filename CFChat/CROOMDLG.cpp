// CROOMDLG.cpp: 实现文件
//

#include "pch.h"
#include "CFChat.h"
#include "CROOMDLG.h"
#include "ChatROOM.h"
#include "afxdialogex.h"


// CROOMDLG 对话框

IMPLEMENT_DYNAMIC(CROOMDLG, CDialogEx)

CROOMDLG::CROOMDLG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIAROOM, pParent)
	, roomName(_T(""))
{

}

CROOMDLG::~CROOMDLG()
{
}

void CROOMDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, roomName);
	DDX_Control(pDX, IDC_LISTROOM, m_ListRoom);
}


BEGIN_MESSAGE_MAP(CROOMDLG, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CROOMDLG::OnClickedButton1)
	ON_WM_COPYDATA()
	ON_NOTIFY(NM_DBLCLK, IDC_LISTROOM, &CROOMDLG::OnDblclkListroom)
END_MESSAGE_MAP()


// CROOMDLG 消息处理程序


BOOL CROOMDLG::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	hRoom = this->m_hWnd;
	m_ListRoom.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ListRoom.InsertColumn(0, L"群名", 0, 300);
	UpdateRoomList();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
void CROOMDLG::UpdateRoomList()
{

	if (OldList.size() == 0)
	{
		for (int i = 0; i < user.roomdList.size(); i++)
		{
			m_ListRoom.InsertItem(i, L"");
			m_ListRoom.SetItemText(i, 0, CATOCS(user.roomdList[i]));
		}
	}
	else if (OldList.size() > user.roomdList.size())
	{
		//int index = 0;
		//删除
		for (size_t i = 0; i < OldList.size(); i++)
		{
			bool del = true;
			for (size_t j = 0; j < user.roomdList.size(); j++)
			{
				if (OldList[i].CompareNoCase(user.roomdList[j]) == 0)
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
				m_ListRoom.DeleteItem(i);
			}
		}

	}
	else if (OldList.size() < user.roomdList.size())
	{

		//删除
		for (size_t i = 0; i < user.roomdList.size(); i++)
		{
			bool ins = true;
			for (size_t j = 0; j < OldList.size(); j++)
			{

				if (OldList[j].CompareNoCase(user.roomdList[i]) == 0)
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
				m_ListRoom.InsertItem(i, L"");			
				m_ListRoom.SetItemText(i, 0, CATOCS(user.roomdList[i]));
			}
		}
	}

	OldList = user.roomdList;

}


void CROOMDLG::OnClickedButton1()
{
	UpdateData(true);
	USES_CONVERSION;
	if (roomName == "" || roomName.IsEmpty())
	{
		MessageBox(L"请输入名称");
		return;
	}
	::SelectRoom(&client, (char*)T2A(roomName));
	roomName = "";
	UpdateData(false);
}


BOOL CROOMDLG::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	char* Nextbuffer = NULL;
	char* type = NULL;
	char* buffa = NULL;
	type = strtok_s((char*)pCopyDataStruct->lpData, "\\", &Nextbuffer);
	buffa = strtok_s(NULL, "\\", &Nextbuffer);
	if (strcmp(type, "10") == 0|| strcmp(type, "11") == 0)
	{

		CStringA buff;
		buff.Format("%s", buffa);
		user.roomdList.push_back(buff);
		UpdateRoomList();
	}

	return CDialogEx::OnCopyData(pWnd, pCopyDataStruct);
}


void CROOMDLG::OnDblclkListroom(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	int index = (int)m_ListRoom.GetFirstSelectedItemPosition();
	if (index == 0) {
		return;
	}
	index -= 1;
	CStringA name = user.roomdList[index];
	int len = MultiByteToWideChar(CP_ACP, 0, name, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len];
	memset(wstr, 0, len * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, name, -1, wstr, len);
	CStringW wName = wstr;
	CDialog* cfDlg = new ChatROOM;
	cfDlg->Create(IDD_DIALOG4);
	cfDlg->SetWindowText(wName);
	cfDlg->ShowWindow(SW_SHOWNORMAL);
}
