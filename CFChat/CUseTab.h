#pragma once


// CUseTab
#include <vector>

using namespace std;
class CUseTab : public CTabCtrl
{
	DECLARE_DYNAMIC(CUseTab)

public:
	CUseTab();
	virtual ~CUseTab();

protected:
	DECLARE_MESSAGE_MAP()
public:
	// 提供接口用于创建对话框(组: 窗口指针+ID)
	void InsertTabWnd(int count, ...);

	// 显示窗口
	void ShowTabWnd(int index);
	afx_msg void OnTcnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	// 用于保存所有的窗口指针
	vector<CDialogEx*> m_WndVec;
	virtual void PreSubclassWindow();
};


