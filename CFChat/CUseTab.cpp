// CUseTab.cpp: 实现文件
//

#include "pch.h"
//#include "TaskManager.h"
#include "CFChat.h"
#include "CUseTab.h"


// CUseTab

IMPLEMENT_DYNAMIC(CUseTab, CTabCtrl)

CUseTab::CUseTab()
{

}

CUseTab::~CUseTab()
{
}



BEGIN_MESSAGE_MAP(CUseTab, CTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, &CUseTab::OnTcnSelchange)
END_MESSAGE_MAP()



// CUseTab 消息处理程序
void CUseTab::InsertTabWnd(int count, ...)
{
	va_list VaList = nullptr;
	va_start(VaList, count);

	for (int i = 0; i < count; ++i)
	{
		// 获取到窗口对象指针和窗口的ID
		CDialogEx* Dialog = va_arg(VaList, CDialogEx*);
		int id = va_arg(VaList, int);

		// 创建窗口并添加到容器中
		Dialog->Create(id, this);
		m_WndVec.push_back(Dialog);
	}

	// 应该以选项卡为父窗口计算出新的位置并且移动所有的窗口
		// 以选项卡为准，重新设置窗口的位置
	CRect Rect = { 0 };
	this->GetClientRect(&Rect);
	Rect.DeflateRect(0, 20, 0, 0);
	for (int i = 0; i < m_WndVec.size(); ++i)
		m_WndVec[i]->MoveWindow(&Rect);

	va_end(VaList);

	ShowTabWnd(0);
}
void CUseTab::ShowTabWnd(int index)
{
	for (int i = 0; i < m_WndVec.size(); ++i)
	{
		m_WndVec[i]->ShowWindow(i == index ? SW_SHOWNORMAL : SW_HIDE);
	}
}
void CUseTab::OnTcnSelchange(NMHDR* pNMHDR, LRESULT* pResult)
{
	ShowTabWnd(this->GetCurSel());
}


void CUseTab::PreSubclassWindow()
{
	InsertItem(0, L"           联系人              ");
	InsertItem(1 ,L"           聊天室              ");

	CTabCtrl::PreSubclassWindow();
}

