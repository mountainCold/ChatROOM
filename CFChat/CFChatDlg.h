
// CFChatDlg.h: 头文件
//

#pragma once
#include "CUseTab.h"


// CCFChatDlg 对话框
class CCFChatDlg : public CDialogEx
{
// 构造
public:
	CCFChatDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CFCHAT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// 用户界面选项卡
	CUseTab UseTab;

	// 用户名
	CString username;
};
