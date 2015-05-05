
// ComputerDetectorTreeView.h : CComputerDetectorTreeView ��Ľӿ�
//


#pragma once
#include "ComputerDetectorDoc.h"

#define TIMER_REFRESH 1

class CComputerDetectorTreeView : public CTreeView
{
protected: // �������л�����
	CComputerDetectorTreeView();
	DECLARE_DYNCREATE(CComputerDetectorTreeView)

// ����
public:
	CComputerDetectorDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // ������һ�ε���

// ʵ��
public:
	virtual ~CComputerDetectorTreeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
};

#ifndef _DEBUG  // ComputerDetectorTreeView.cpp �еĵ��԰汾
inline CComputerDetectorDoc* CComputerDetectorTreeView::GetDocument() const
   { return reinterpret_cast<CComputerDetectorDoc*>(m_pDocument); }
#endif

