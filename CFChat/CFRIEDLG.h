#pragma once


// CFRIEDLG 对话框

class CFRIEDLG : public CDialogEx
{
	DECLARE_DYNAMIC(CFRIEDLG)

public:
	CFRIEDLG(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CFRIEDLG();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLGFRI };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_friendList;
	vector<CStringA> OldList;
	virtual BOOL OnInitDialog();
	void UpdateFriendList();
	CString friendName;
	afx_msg void OnClickedButton1();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnDblclkListfrie(NMHDR* pNMHDR, LRESULT* pResult);
};
