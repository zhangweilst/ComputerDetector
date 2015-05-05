
// ComputerDetectorTreeView.cpp : CComputerDetectorTreeView 类的实现
//

#include "stdafx.h"
#include "ComputerDetector.h"
#include "ComputerDetectorDoc.h"
#include "ComputerDetectorTreeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CComputerDetectorTreeView

IMPLEMENT_DYNCREATE(CComputerDetectorTreeView, CTreeView)

BEGIN_MESSAGE_MAP(CComputerDetectorTreeView, CTreeView)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CComputerDetectorTreeView::OnTvnSelchanged)
	ON_WM_TIMER()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

// CComputerDetectorTreeView 构造/析构

CComputerDetectorTreeView::CComputerDetectorTreeView()
{
	// TODO: 在此处添加构造代码

}

CComputerDetectorTreeView::~CComputerDetectorTreeView()
{
}

BOOL CComputerDetectorTreeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	cs.style |= TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT;

	return CTreeView::PreCreateWindow(cs);
}

void CComputerDetectorTreeView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	HTREEITEM hRoot = GetTreeCtrl().InsertItem(GetDocument()->GetHostName(), 0, 0, TVI_ROOT, TVI_SORT);
	HTREEITEM hCPU = GetTreeCtrl().InsertItem(_T("中央处理器"), 1, 1, hRoot);
	//HTREEITEM hCPUForNerds = GetTreeCtrl().InsertItem(_T("Information for Nerds"), 1, 1, hCPU);
	HTREEITEM hMainboard = GetTreeCtrl().InsertItem(_T("主板"), 1, 1, hRoot);
	HTREEITEM hGraphicAdp = GetTreeCtrl().InsertItem(_T("显卡"), 1, 1, hRoot);
	HTREEITEM hPhysicalMem = GetTreeCtrl().InsertItem(_T("物理内存"), 1, 1, hRoot);

	// Set the timer to update real time information
	SetTimer(TIMER_REFRESH, 1000, NULL);
}

void CComputerDetectorTreeView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CComputerDetectorTreeView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CComputerDetectorTreeView 诊断

#ifdef _DEBUG
void CComputerDetectorTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CComputerDetectorTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CComputerDetectorDoc* CComputerDetectorTreeView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CComputerDetectorDoc)));
	return (CComputerDetectorDoc*)m_pDocument;
}
#endif //_DEBUG


// CComputerDetectorTreeView 消息处理程序

void CComputerDetectorTreeView::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CString strItem = GetTreeCtrl().GetItemText(pNMTreeView->itemNew.hItem);
	GetDocument()->UpdateAllViews(this, 0x4A, (CObject*)(LPCTSTR) strItem);		// 0x4A for update
	*pResult = 0;
}

void CComputerDetectorTreeView::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_REFRESH)
	{
		CString str;
		str = GetTreeCtrl().GetItemText(GetTreeCtrl().GetSelectedItem());
		GetDocument()->UpdateAllViews(this, 0x4A, (CObject*)(LPCTSTR) str);
		return;
	}

	CTreeView::OnTimer(nIDEvent);
}

void CComputerDetectorTreeView::OnClose()
{
	// Destroy the timer
	KillTimer(TIMER_REFRESH);

	CTreeView::OnClose();
}
