
// ComputerDetectorDoc.h : CComputerDetectorDoc 类的接口
//

#pragma once
#include "CPUID.h"
#include "WMI.h"
#include "CPU.h"
#include "BaseBoard.h"
#include "PhysicalMemory.h"
#include "Intel.h"
#include "AMD.h"
#include "Nvidia.h"
#include "ATI.h"

typedef struct tagLocalTimeFormat
{
	CString strTimeString;
	CString strTimeNumerical;
} LocalTimeForamt;

class CComputerDetectorDoc : public CDocument
{
protected: // 仅从序列化创建
	CComputerDetectorDoc();
	DECLARE_DYNCREATE(CComputerDetectorDoc)

// 重写
public:
	virtual void Serialize(CArchive& ar);

// 实现
public:
	virtual ~CComputerDetectorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// 成员变量
protected:
	CCPUID m_CPUID;					// CPUID 对象用来执行CPUID指令
	CWMI m_WMI;
	CPUBrand m_CPUBrand;
	GPUBrand m_GPUBrand;

	DWORD m_TimeInterval;		// 日志记录时间间隔
	UINT_PTR	m_RecordTimer;

public:
	CCPU* m_CPU;
	CGraphicsAdapter* m_GraphicsAdapter;
	CBaseBoard* m_BaseBoard;
	CPhysicalMemory* m_PhysicalMemory;
	CString m_strHostName;					// Name of computer
	static DWORD m_RecordCount;			// Won't wrap-around in 100 years
	static CString m_RecordFileName;		// Performance record file name

// 成员函数
public:
	CCPU* GetObjCPU();
	CGraphicsAdapter* GetObjGPU();
	CBaseBoard* GetObjBaseBoard();
	CPhysicalMemory* GetObjPhysicalMemory();
	CString GetHostName();
	LocalTimeForamt GetLocalTimeString();

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileRecord();
	afx_msg void OnStopRecord();
	afx_msg void OnUpdateFileRecord(CCmdUI *pCmdUI);
};
