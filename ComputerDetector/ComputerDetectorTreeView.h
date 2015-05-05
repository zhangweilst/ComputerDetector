
// ComputerDetectorTreeView.h : CComputerDetectorTreeView 类的接口
//


#pragma once
#include "ComputerDetectorDoc.h"

#define TIMER_REFRESH 1

class CComputerDetectorTreeView : public CTreeView
{
protected: // 仅从序列化创建
	CComputerDetectorTreeView();
	DECLARE_DYNCREATE(CComputerDetectorTreeView)

// 属性
public:
	CComputerDetectorDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 构造后第一次调用

// 实现
public:
	virtual ~CComputerDetectorTreeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
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

#ifndef _DEBUG  // ComputerDetectorTreeView.cpp 中的调试版本
inline CComputerDetectorDoc* CComputerDetectorTreeView::GetDocument() const
   { return reinterpret_cast<CComputerDetectorDoc*>(m_pDocument); }
#endif

