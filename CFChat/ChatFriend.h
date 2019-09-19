#pragma once


// ChatFriend 对话框

class ChatFriend : public CDialogEx
{
	DECLARE_DYNAMIC(ChatFriend)

public:
	ChatFriend(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~ChatFriend();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	// 信息显示窗口
	CListBox m_ListBox;
	// 聊天消息
	CString m_MSG;
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
