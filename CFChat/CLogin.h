#pragma once


// CLogin 对话框

class CLogin : public CDialogEx
{
	DECLARE_DYNAMIC(CLogin)

public:
	CLogin(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CLogin();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedButton1();
	// 用户名
	CString username;
	// 密码
	CString password;
	afx_msg void OnClickedButton2();
	afx_msg void OnClose();
};
