#pragma once


// CROOMDLG 对话框

class CROOMDLG : public CDialogEx
{
	DECLARE_DYNAMIC(CROOMDLG)

public:
	CROOMDLG(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CROOMDLG();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIAROOM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString roomName;
	CListCtrl m_ListRoom;
	virtual BOOL OnInitDialog();
	void UpdateRoomList();
	vector<CStringA> OldList;
	afx_msg void OnClickedButton1();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnDblclkListroom(NMHDR* pNMHDR, LRESULT* pResult);
};
