// ComputerDetectorListView.cpp : 实现文件
//

#include "stdafx.h"
#include "ComputerDetector.h"
#include "ComputerDetectorListView.h"

IMPLEMENT_DYNCREATE(CComputerDetectorListView, CListView)

CComputerDetectorListView::CComputerDetectorListView()
{
}

CComputerDetectorListView::~CComputerDetectorListView()
{
}

BEGIN_MESSAGE_MAP(CComputerDetectorListView, CListView)
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, &CComputerDetectorListView::OnLvnGetdispinfo)
END_MESSAGE_MAP()

// CComputerDetectorListView 诊断

#ifdef _DEBUG
void CComputerDetectorListView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void CComputerDetectorListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG

BOOL CComputerDetectorListView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~LVS_TYPEMASK;
	cs.style |= LVS_REPORT;

	return CListView::PreCreateWindow(cs);
}

void CComputerDetectorListView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	// Add columns
	GetListCtrl().InsertColumn(0, _T("项目"), LVCFMT_LEFT, 192);
	GetListCtrl().InsertColumn(1, _T("值"), LVCFMT_LEFT, 360);
	GetListCtrl().InsertColumn(2, _T("最大"), LVCFMT_LEFT, 360);
}

// Implemention

void CComputerDetectorListView::FreeItemMemory()
{
	int nCount = GetListCtrl().GetItemCount();
	if (nCount)
	{
		for (int i = 0; i < nCount; i++)
			delete (ITEMINFO*) GetListCtrl().GetItemData(i);
	}
}

CComputerDetectorDoc* CComputerDetectorListView::GetDocument()
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CComputerDetectorDoc)));
	return (CComputerDetectorDoc*) m_pDocument;
}

void CComputerDetectorListView::Refresh(LPCTSTR pszItem)
{
	// Refresh all items
	CString strItem = pszItem;
	CString str;
	int nCount = 0;	// 项目计数

	ITEMINFO itemInfo;

	GetListCtrl().DeleteAllItems();		// Delete to update

	if (strItem == GetDocument()->GetHostName())
	{
		nCount = 0;
		itemInfo.strItem = _T("电脑概述：");
		itemInfo.strValue = _T("");
		itemInfo.strMaxValue = _T("");
		AddItem(nCount++, itemInfo);

		itemInfo.strItem = _T("处理器名称");
		itemInfo.strValue = GetDocument()->GetObjCPU()->GetName();
		itemInfo.strMaxValue = _T("");
		AddItem(nCount++, itemInfo);

		itemInfo.strItem = _T("主板型号");
		itemInfo.strValue = GetDocument()->GetObjBaseBoard()->GetName();;
		itemInfo.strMaxValue = _T("");
		AddItem(nCount++, itemInfo);

		itemInfo.strItem = _T("显卡名称");
		itemInfo.strValue = GetDocument()->GetObjGPU()->GetName();
		itemInfo.strMaxValue = _T("");
		AddItem(nCount++, itemInfo);
	}
	
	if (strItem == _T("中央处理器"))
	{
		nCount = 0;
		// First basic information
		itemInfo.strItem = _T("基本信息：");
		itemInfo.strValue = _T("");
		itemInfo.strMaxValue = _T("");
		AddItem(nCount++, itemInfo);

		itemInfo.strItem = _T("处理器名称");
		itemInfo.strValue = GetDocument()->GetObjCPU()->GetName();
		itemInfo.strMaxValue = _T("");
		AddItem(nCount++, itemInfo);

		itemInfo.strItem = _T("核心数量");
		str.Format(_T("%d 核心 / %d 线程"), 
			GetDocument()->GetObjCPU()->GetNumOfCores(),
			GetDocument()->GetObjCPU()->GetTotalThreads());
		itemInfo.strValue = str;
		itemInfo.strMaxValue = _T("");
		AddItem(nCount++, itemInfo);

		itemInfo.strItem = _T("版本");
		itemInfo.strValue = GetDocument()->GetObjCPU()->GetVersionInfo();
		itemInfo.strMaxValue = _T("");
		AddItem(nCount++, itemInfo);

		itemInfo.strItem = _T("核心架构");
		itemInfo.strValue = GetDocument()->GetObjCPU()->GetMicroArchitecture();
		itemInfo.strMaxValue = _T("");
		AddItem(nCount++, itemInfo);

		if (GetDocument()->GetObjCPU()->GetLithography())
		{
			itemInfo.strItem = _T("制作工艺");
			str.Format(_T("%d nm"), GetDocument()->GetObjCPU()->GetLithography());
			itemInfo.strValue = str;
			itemInfo.strMaxValue = _T("");
			AddItem(nCount++, itemInfo);
		}

		// 分割线
		itemInfo.strItem = _T("");
		itemInfo.strValue = _T("");
		itemInfo.strMaxValue = _T("");
		AddItem(nCount++, itemInfo);

		/*
		// Then real time information
		itemInfo.strItem = _T("实时信息：");
		itemInfo.strValue = _T("");
		itemInfo.strMaxValue = _T("");
		AddItem(nCount++, itemInfo);
		*/

		int i;
		int j;
		int nProcessors = GetDocument()->GetObjCPU()->GetNumOfProcessors();
		int nCores = GetDocument()->GetObjCPU()->GetNumOfCores();
		int nThreads = GetDocument()->GetObjCPU()->GetNumOfThreads();
		for (i = 0; i < nProcessors; i++)
		{
			if (nProcessors > 1)
			{
				str.Format(_T("处理器 %d："), i + 1);
				itemInfo.strItem = str;
				itemInfo.strValue = _T("");
				itemInfo.strMaxValue = _T("");
				AddItem(nCount++, itemInfo);
			}

			// 温度
			itemInfo.strItem = _T("温度：");
			itemInfo.strValue = _T("");
			itemInfo.strMaxValue = _T("");
			AddItem(nCount++, itemInfo);
    
			if (GetDocument()->GetObjCPU()->IsPackageDTS())
			{
				itemInfo.strItem = _T("CPU 封装温度");
				str.Format(_T("%d °C"), GetDocument()->GetObjCPU()->GetPackageTemperature());
				itemInfo.strValue = str;
				str.Format(_T("%d °C"), GetDocument()->GetObjCPU()->GetMaxCrtPkgTemp());
				itemInfo.strMaxValue = str;
				AddItem(nCount++, itemInfo);
			}
    
			for ( j = 0; j < nCores; j++)
			{
				str.Format(_T("核心%d温度"), j+1);
				itemInfo.strItem = str;
    
				str.Format("%d °C", GetDocument()->GetObjCPU()->GetCoreTemperature()
					[i * GetDocument()->GetObjCPU()->GetNumOfCores() + j]);
				itemInfo.strValue = str;
				str.Empty();
				str.Format("%d °C", GetDocument()->GetObjCPU()->GetMaxCrtCoreTemp()
					[i * GetDocument()->GetObjCPU()->GetNumOfCores() + j]);
				itemInfo.strMaxValue = str;
				AddItem(nCount++, itemInfo);
			}

			itemInfo.strItem = _T("");
			itemInfo.strValue = _T("");
			itemInfo.strMaxValue = _T("");
			AddItem(nCount++, itemInfo);

			// 处理器频率
			itemInfo.strItem = _T("处理器频率：");
			itemInfo.strValue = _T("");
			itemInfo.strMaxValue = _T("");
			AddItem(nCount++, itemInfo);

			itemInfo.strItem = _T("总线");
			str.Format("%.2lf MHz", GetDocument()->GetObjCPU()->GetBusFreq());
			itemInfo.strValue = str;
			itemInfo.strMaxValue = _T("");
			AddItem(nCount++, itemInfo);

			itemInfo.strItem = _T("倍频");
			str.Format("×%d ( %d - %d )", GetDocument()->GetObjCPU()->GetCPUToBusRatio(),
				GetDocument()->GetObjCPU()->GetMinRatio(),
				GetDocument()->GetObjCPU()->GetMaxTurboRatio());
			itemInfo.strValue = str;
			str.Format("×%d", GetDocument()->GetObjCPU()->GetMaxCPUToBusRatio());
			itemInfo.strMaxValue = str;
			AddItem(nCount++, itemInfo);

			itemInfo.strItem = _T("处理器");
			str.Format("%.lf MHz", GetDocument()->GetObjCPU()->GetCPUFreq());
			itemInfo.strValue = str;
			str.Format("%.lf MHz", GetDocument()->GetObjCPU()->GetMaxCPUFreq());
			itemInfo.strMaxValue = str;
			AddItem(nCount++, itemInfo);

			itemInfo.strItem = _T("占有率");
			str.Format("%d%%", GetDocument()->GetObjCPU()->GetCPUPercentage());
			itemInfo.strValue = str;
			str.Format("%d%%", GetDocument()->GetObjCPU()->GetMaxCPUPercentage());
			itemInfo.strMaxValue = str;
			AddItem(nCount++, itemInfo);

			itemInfo.strItem = _T("");
			itemInfo.strValue = _T("");
			itemInfo.strMaxValue = _T("");
			AddItem(nCount++, itemInfo);

			// 线程实际频率
			itemInfo.strItem = _T("线程实际频率：");
			itemInfo.strValue = _T("");
			itemInfo.strMaxValue = _T("");
			AddItem(nCount++, itemInfo);

			for ( j = 0; j < nCores * nThreads; j++)
			{
				str.Format(_T("线程%d频率 (占有率)"), j+1);
				itemInfo.strItem = str;
    
				str.Format("%d MHz (%d%%)", GetDocument()->GetObjCPU()->GetThreadFreq()[j],
					GetDocument()->GetObjCPU()->GetThreadPercentage()[j]);
				itemInfo.strValue = str;
				str.Format("%d MHz (%d%%)", GetDocument()->GetObjCPU()->GetMaxCrtThreadFreq()[j],
					GetDocument()->GetObjCPU()->GetMaxCrtThreadPercentage()[j]);
				itemInfo.strMaxValue = str;
				AddItem(nCount++, itemInfo);
			}

			itemInfo.strItem = _T("");
			itemInfo.strValue = _T("");
			itemInfo.strMaxValue = _T("");
			AddItem(nCount++, itemInfo);

			// 功率
			if (GetDocument()->GetObjCPU()->IsPowerMngt())
			{
				itemInfo.strItem = _T("功率：");
				itemInfo.strValue = _T("");
				itemInfo.strMaxValue = _T("");
				AddItem(nCount++, itemInfo);

				// CPU package
				itemInfo.strItem = _T("CPU 封装");
				str.Format(_T("%.2lf W"), GetDocument()->GetObjCPU()->GetPackagePower());
				itemInfo.strValue = str;
				str.Format(_T("%.2lf W"), GetDocument()->GetObjCPU()->GetMaxPackagePower());
				itemInfo.strMaxValue = str;
				AddItem(nCount++, itemInfo);

				itemInfo.strItem = _T("CPU 核心");
				str.Format(_T("%.2lf W"), GetDocument()->GetObjCPU()->GetCoresPower());
				itemInfo.strValue = str;
				str.Format(_T("%.2lf W"), GetDocument()->GetObjCPU()->GetMaxCoresPower());
				itemInfo.strMaxValue = str;
				AddItem(nCount++, itemInfo);

				itemInfo.strItem = _T("CPU 图形核心");
				str.Format(_T("%.2lf W"), GetDocument()->GetObjCPU()->GetGraphicsPower());
				itemInfo.strValue = str;
				str.Format(_T("%.2lf W"), GetDocument()->GetObjCPU()->GetMaxGraphicsPower());
				itemInfo.strMaxValue = str;
				AddItem(nCount++, itemInfo);

				itemInfo.strItem = _T("CPU DRAM");
				str.Format(_T("%.2lf W"), GetDocument()->GetObjCPU()->GetDRAMPower());
				itemInfo.strValue = str;
				str.Format(_T("%.2lf W"), GetDocument()->GetObjCPU()->GetMaxDRAMPower());
				itemInfo.strMaxValue = str;
				AddItem(nCount++, itemInfo);
			}
		}
	}

	if (strItem == _T("主板"))
	{
		nCount = 0;
		itemInfo.strItem = _T("基本信息：");
		itemInfo.strValue = _T("");
		itemInfo.strMaxValue = _T("");
		AddItem(nCount++, itemInfo);

		itemInfo.strItem = _T("主板型号");
		itemInfo.strValue = GetDocument()->GetObjBaseBoard()->GetName();
		itemInfo.strMaxValue = _T("");
		AddItem(nCount++, itemInfo);

		// 分割线
		itemInfo.strItem = _T("");
		itemInfo.strValue = _T("");
		itemInfo.strMaxValue = _T("");
		AddItem(nCount++, itemInfo);

		// Then real time information
		itemInfo.strItem = _T("实时信息：");
		itemInfo.strValue = _T("");
		itemInfo.strMaxValue = _T("");
		AddItem(nCount++, itemInfo);

		int sensorCount = GetDocument()->GetObjBaseBoard()->GetSensorCount();
		for (int i = 0; i < sensorCount; i++)
		{
			if (sensorCount > 1)
				str.Format(_T("主板温度%d："), i + 1);
			else
				str = _T("主板温度：");
			itemInfo.strItem = str;
			str.Format(_T("%.1lf °C"), GetDocument()->GetObjBaseBoard()->GetBaseBoardTemperture()[i]);
			itemInfo.strValue = str;
			itemInfo.strMaxValue = _T("");
			AddItem(nCount++, itemInfo);
		}
	}

	if (strItem == _T("显卡"))
	{
		nCount = 0;
		itemInfo.strItem = _T("基本信息：");
		itemInfo.strValue = _T("");
		itemInfo.strMaxValue = _T("");
		AddItem(nCount++, itemInfo);

		itemInfo.strItem = _T("显卡型号");
		itemInfo.strValue = GetDocument()->GetObjGPU()->GetName();
		itemInfo.strMaxValue = _T("");
		AddItem(nCount++, itemInfo);

		// 分割线
		itemInfo.strItem = _T("");
		itemInfo.strValue = _T("");
		itemInfo.strMaxValue = _T("");
		AddItem(nCount++, itemInfo);

		// Then real time information
		itemInfo.strItem = _T("实时信息：");
		itemInfo.strValue = _T("");
		itemInfo.strMaxValue = _T("");
		AddItem(nCount++, itemInfo);

		if (GetDocument()->GetObjGPU()->GetCoreVoltage() != 0.0)
		{
		itemInfo.strItem = _T("GPU 电压");
		str.Format(_T("%.3lf V"), GetDocument()->GetObjGPU()->GetCoreVoltage());
		itemInfo.strValue = str;
		str.Format(_T("%.3lf V"), GetDocument()->GetObjGPU()->GetMaxCoreVoltage());
		itemInfo.strMaxValue = str;
		AddItem(nCount++, itemInfo);
		}

		itemInfo.strItem = _T("GPU 核心时钟");
		str.Format(_T("%d MHz"), GetDocument()->GetObjGPU()->GetCoreClock());
		itemInfo.strValue = str;
		str.Format(_T("%d MHz"), GetDocument()->GetObjGPU()->GetMaxCoreClock());
		itemInfo.strMaxValue = str;
		AddItem(nCount++, itemInfo);

		itemInfo.strItem = _T("GPU 显存时钟");
		str.Format(_T("%d MHz"), GetDocument()->GetObjGPU()->GetMemoryClock());
		itemInfo.strValue = str;
		str.Format(_T("%d MHz"), GetDocument()->GetObjGPU()->GetMaxMemoryClock());
		itemInfo.strMaxValue = str;
		AddItem(nCount++, itemInfo);

		itemInfo.strItem = _T("GPU 核心温度");
		str.Format(_T("%d °C"), GetDocument()->GetObjGPU()->GetCoreTemperature());
		itemInfo.strValue = str;
		str.Format(_T("%d °C"), GetDocument()->GetObjGPU()->GetMaxCoreTemperature());
		itemInfo.strMaxValue = str;
		AddItem(nCount++, itemInfo);

		if (GetDocument()->GetObjGPU()->GetFanSpeed())
		{
			itemInfo.strItem = _T("风扇转速");
			str.Format(_T("%d RPM"), GetDocument()->GetObjGPU()->GetFanSpeed());
			itemInfo.strValue = str;
			str.Format(_T("%d RPM"), GetDocument()->GetObjGPU()->GetMaxFanSpeed());
			itemInfo.strMaxValue = str;
			AddItem(nCount++, itemInfo);
		}

		itemInfo.strItem = _T("GPU 占有率");
		str.Format(_T("%d %%"), GetDocument()->GetObjGPU()->GetGPUPercentage());
		itemInfo.strValue = str;
		str.Format(_T("%d %%"), GetDocument()->GetObjGPU()->GetMaxGPUPercentage());
		itemInfo.strMaxValue = str;
		AddItem(nCount++, itemInfo);
	}

	if (strItem == _T("物理内存"))
	{
		nCount = 0;
		itemInfo.strItem = _T("基本信息：");
		itemInfo.strValue = _T("");
		itemInfo.strMaxValue = _T("");
		AddItem(nCount++, itemInfo);

		itemInfo.strItem = _T("");
		itemInfo.strValue = _T("");
		itemInfo.strMaxValue = _T("");
		AddItem(nCount++, itemInfo);

		for (int i = 0; i < GetDocument()->GetObjPhysicalMemory()->GetNumOfDimms(); i++)
		{
			itemInfo.strItem = _T("内存位置");
			str = GetDocument()->GetObjPhysicalMemory()->GetMemoryLocator().GetAt(i).bstrVal;
			itemInfo.strValue = str;
			itemInfo.strMaxValue = _T("");
			AddItem(nCount++, itemInfo);

			itemInfo.strItem = _T("容量");
			str = GetDocument()->GetObjPhysicalMemory()->GetMemoryCapacity().GetAt(i).bstrVal;
			itemInfo.strValue = str;
			itemInfo.strMaxValue = _T("");
			AddItem(nCount++, itemInfo);

			itemInfo.strItem = _T("速度");
			str.Format(_T("%d MHz"), GetDocument()->GetObjPhysicalMemory()->GetMemorySpeed().GetAt(i).uintVal);
			itemInfo.strValue = str;
			itemInfo.strMaxValue = _T("");
			AddItem(nCount++, itemInfo);

			itemInfo.strItem = _T("序列号");
			str = GetDocument()->GetObjPhysicalMemory()->GetMemorySerialNumber().GetAt(i).bstrVal;
			itemInfo.strValue = str;
			itemInfo.strMaxValue = _T("");
			AddItem(nCount++, itemInfo);

			itemInfo.strItem = _T("");
			itemInfo.strValue = _T("");
			itemInfo.strMaxValue = _T("");
			AddItem(nCount++, itemInfo);
		}
	}
}

BOOL CComputerDetectorListView::AddItem(int nIndex, ITEMINFO Item)
{
	ITEMINFO* pItem;
	pItem = new ITEMINFO;
	pItem->strItem = Item.strItem;
	pItem->strValue = Item.strValue;
	pItem->strMaxValue = Item.strMaxValue;

	LV_ITEM lvi;
    lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM; 
    lvi.iItem = nIndex; 
    lvi.iSubItem = 0; 
    lvi.iImage = 0;
    lvi.pszText = LPSTR_TEXTCALLBACK; 
    lvi.lParam = (LPARAM) pItem;

    if (GetListCtrl ().InsertItem (&lvi) == -1)
        return FALSE;

    return TRUE;
}

// 消息处理函数

void CComputerDetectorListView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	// Update from timer
	if (lHint == 0x4A)
	{
		FreeItemMemory();
		GetListCtrl().DeleteAllItems();
		Refresh((LPCTSTR) pHint);
		return;
	}

	CListView::OnUpdate(pSender, lHint, pHint);
}

void CComputerDetectorListView::OnLvnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	CString str;

	if (pDispInfo->item.mask & LVIF_TEXT)
	{
		ITEMINFO* pItem = (ITEMINFO*) pDispInfo->item.lParam;
		switch (pDispInfo->item.iSubItem)
		{
		case 0:
			::lstrcpy(pDispInfo->item.pszText, pItem->strItem);
			break;

		case 1:
			::lstrcpy(pDispInfo->item.pszText, pItem->strValue);
			break;

		case 2:
			::lstrcpy(pDispInfo->item.pszText, pItem->strMaxValue);
			break;
		}
	}

	*pResult = 0;
}
