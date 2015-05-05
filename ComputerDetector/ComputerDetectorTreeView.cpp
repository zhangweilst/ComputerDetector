
// ComputerDetectorTreeView.cpp : CComputerDetectorTreeView ���ʵ��
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

// CComputerDetectorTreeView ����/����

CComputerDetectorTreeView::CComputerDetectorTreeView()
{
	// TODO: �ڴ˴���ӹ������

}

CComputerDetectorTreeView::~CComputerDetectorTreeView()
{
}

BOOL CComputerDetectorTreeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	cs.style |= TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT;

	return CTreeView::PreCreateWindow(cs);
}

void CComputerDetectorTreeView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	HTREEITEM hRoot = GetTreeCtrl().InsertItem(GetDocument()->GetHostName(), 0, 0, TVI_ROOT, TVI_SORT);
	HTREEITEM hCPU = GetTreeCtrl().InsertItem(_T("���봦����"), 1, 1, hRoot);
	//HTREEITEM hCPUForNerds = GetTreeCtrl().InsertItem(_T("Information for Nerds"), 1, 1, hCPU);
	HTREEITEM hMainboard = GetTreeCtrl().InsertItem(_T("����"), 1, 1, hRoot);
	HTREEITEM hGraphicAdp = GetTreeCtrl().InsertItem(_T("�Կ�"), 1, 1, hRoot);
	HTREEITEM hPhysicalMem = GetTreeCtrl().InsertItem(_T("�����ڴ�"), 1, 1, hRoot);

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


// CComputerDetectorTreeView ���

#ifdef _DEBUG
void CComputerDetectorTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CComputerDetectorTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CComputerDetectorDoc* CComputerDetectorTreeView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CComputerDetectorDoc)));
	return (CComputerDetectorDoc*)m_pDocument;
}
#endif //_DEBUG


// CComputerDetectorTreeView ��Ϣ�������

void CComputerDetectorTreeView::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
