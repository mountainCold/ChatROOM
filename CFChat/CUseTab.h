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
	// �ṩ�ӿ����ڴ����Ի���(��: ����ָ��+ID)
	void InsertTabWnd(int count, ...);

	// ��ʾ����
	void ShowTabWnd(int index);
	afx_msg void OnTcnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	// ���ڱ������еĴ���ָ��
	vector<CDialogEx*> m_WndVec;
	virtual void PreSubclassWindow();
};


