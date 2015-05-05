#pragma once
#include "ComputerDetectorDoc.h"
#include "WMI.h"

typedef struct tagITEMINFO
{
	CString strItem;
	CString strValue;
	CString strMaxValue;
} ITEMINFO;

class CComputerDetectorListView : public CListView
{
	DECLARE_DYNCREATE(CComputerDetectorListView)

protected:
	CComputerDetectorListView();           // 动态创建所使用的受保护的构造函数
	virtual ~CComputerDetectorListView();

public:
	CComputerDetectorDoc* GetDocument();
protected:
	void FreeItemMemory();
	void Refresh(LPCTSTR pszItem);
	BOOL AddItem(int nIndex, ITEMINFO Item);

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:
	virtual void OnInitialUpdate();
protected:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
public:
	afx_msg void OnLvnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult);
};


