#pragma once


// ChatROOM 对话框

class ChatROOM : public CDialogEx
{
	DECLARE_DYNAMIC(ChatROOM)

public:
	ChatROOM(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~ChatROOM();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CString m_Msg;
	CListBox m_listBox;
	// 群聊成员
	CListCtrl m_roomlist;
	virtual BOOL OnInitDialog();
	afx_msg void OnClickedButton1();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
};
